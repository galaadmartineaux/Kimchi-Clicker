
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <assert.h>

#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>
#include <miniaudio.h>

#include "playground.h"
#include "playground_internal.h"

static const char* CFG_KEY_DISPLAY_SCALE = "DisplayScale";

//-------------------------------------------------------------------------
// Struct
//-------------------------------------------------------------------------
struct u32_color
{
    unsigned char r;
    unsigned char g;
    unsigned char b;
    unsigned char a;
};

struct vertex_t
{
    float pos[2];
    float uv[2];
};

struct vertex_pos_t
{
    float pos[2];
};

struct vertex_uv_color_t
{
    unsigned short uv[2];
    unsigned int fg_color;
    unsigned int bg_color;
};

struct tile_t
{
    int value;
    unsigned int fg_color;
    unsigned int bg_color;
};

struct layer_t
{
    // Static data
    int width; // In tiles (not pixels)
    int height;
    int tile_width;  // Width of a tile in pixels
    int tile_height; // Height of a tile in pixels

    // Dynamic data
    int tileset;
    int viewport[4]; // In tiles
    struct tile_t* tiles;

    // Output
    struct vertex_pos_t* vertex_pos;
    struct vertex_uv_color_t* vertex_uv_color;
    GLuint vertex_pos_buffer;
    GLuint vertex_uv_color_buffer;
    GLuint vao;
};

struct image_t
{
    int width;
    int height;
    struct u32_color* pixels; // unsigned bytes 4 channels
};

struct texture_t
{
    GLuint name;
    int width;
    int height;
};

struct framebuffer_t
{
    GLuint name;
    struct texture_t texture;
};

struct tileset_t
{
    char* path;
    struct texture_t texture;
};

struct sound_item_t
{
    ma_sound sound;
    struct sound_item_t* next;
};

struct sound_list_t
{
    float base_volume;
    struct sound_item_t head;
};

struct context_t
{
    int tileset_count;
    struct tileset_t* tilesets;

    int sound_count;
    struct sound_list_t* sounds;
    ma_sound_group sound_group;

    int music_count;
    char** music_paths;
    ma_sound music; // Current music
    ma_sound_group music_group;

    unsigned int defaut_bg_color;
    unsigned int defaut_fg_color;
    unsigned int fade_color;
    float fade_amount;

    int layer_count;
    struct layer_t* layers;
    struct layer_t* current_layer;
    int display_scale;

    // OpenGL data
    struct texture_t white;
    struct framebuffer_t framebuffer;
    GLuint program;
    GLuint fs_program;
    GLuint fs_vbo; // Fullscreen
    GLuint fs_vao;

    // Config system
    struct cfg_t config;

    // Communication with backend
    struct pg_io io;

    // Audio engine
    ma_engine audio;
};

//-------------------------------------------------------------------------
// layer CONTEXT
//-------------------------------------------------------------------------
static struct context_t ctx = {0};

//-------------------------------------------------------------------------
// Various helpers
//-------------------------------------------------------------------------
static char* string_duplicate(const char* str)
{
    size_t len = strlen(str);
    char* res = calloc(len + 1, 1);
    memcpy(res, str, len);
    return res;
}

static float volume_to_linear(float v)
{
    if (v < 0.f)
        return v;
    return sqrtf(v);
}

static float linear_to_volume(float v)
{
    if (v < 0.f)
        return v;
    return v*v;
}

//-------------------------------------------------------------------------
// Math helpers
//-------------------------------------------------------------------------
static int m_clamps32(int x, int min_value, int max_value)
{
    if (x < min_value)
        return min_value;
    else if (x > max_value)
        return max_value;
    else
        return x;
}

static unsigned int m_clampu32(unsigned int x, unsigned int min_value, unsigned int max_value)
{
    if (x < min_value)
        return min_value;
    else if (x > max_value)
        return max_value;
    else
        return x;
}

static int m_mins32(int x, int min_value)
{
    return x < min_value ? x : min_value;
}

static unsigned int m_minu32(unsigned int x, unsigned int min_value)
{
    return x < min_value ? x : min_value;
}

static int m_maxs32(int x, int max_value)
{
    return x < max_value ? max_value : x;
}

static unsigned int m_maxu32(unsigned int x, unsigned int max_value)
{
    return x < max_value ? max_value : x;
}

//-------------------------------------------------------------------------
// Color helpers
//-------------------------------------------------------------------------
// TODO: Handle endianness
#define SHIFT_ALPHA (24)
#define SHIFT_RED   (16)
#define SHIFT_GREEN (8)
#define SHIFT_BLUE  (0)

#define MASK_ALPHA (0xFF << SHIFT_ALPHA)
#define MASK_RED   (0xFF << SHIFT_RED)
#define MASK_GREEN (0xFF << SHIFT_GREEN)
#define MASK_BLUE  (0xFF << SHIFT_BLUE)

static unsigned int float_to_color(float src[4])
{
    return 
          m_clampu32((unsigned int)(src[0] * 0xFF), 0, 0xFF) << 24
        | m_clampu32((unsigned int)(src[1] * 0xFF), 0, 0xFF) << 16
        | m_clampu32((unsigned int)(src[2] * 0xFF), 0, 0xFF) << 8
        | m_clampu32((unsigned int)(src[3] * 0xFF), 0, 0xFF) << 0;
}

static unsigned int from_argb(unsigned int argb[4])
{
    return (argb[0] << SHIFT_ALPHA | argb[1] << SHIFT_RED | argb[2] << SHIFT_GREEN | argb[3] << SHIFT_BLUE);
}

static unsigned int from_argb_clamped(unsigned int argb[4])
{
    argb[0] = m_clampu32(argb[0], 0, 0xFF);
    argb[1] = m_clampu32(argb[1], 0, 0xFF);
    argb[2] = m_clampu32(argb[2], 0, 0xFF);
    argb[3] = m_clampu32(argb[3], 0, 0xFF);
    return (argb[0] << SHIFT_ALPHA | argb[1] << SHIFT_RED | argb[2] << SHIFT_GREEN | argb[3] << SHIFT_BLUE);
}

static void to_argb(unsigned int dst[4], unsigned int color)
{
    dst[0] = (color & MASK_ALPHA) >> SHIFT_ALPHA;
    dst[1] = (color & MASK_RED)   >> SHIFT_RED;
    dst[2] = (color & MASK_GREEN) >> SHIFT_GREEN;
    dst[3] = (color & MASK_BLUE)  >> SHIFT_BLUE;
}

static float* color_to_float(float dst[4], unsigned int color)
{
    dst[0] = ((color & MASK_ALPHA) >> SHIFT_ALPHA) / 255.f;
    dst[1] = ((color & MASK_RED)   >> SHIFT_RED)   / 255.f;
    dst[2] = ((color & MASK_GREEN) >> SHIFT_GREEN) / 255.f;
    dst[3] = ((color & MASK_BLUE)  >> SHIFT_BLUE)  / 255.f;
    return dst;
}

static float* multiply_float_color(float dst[4], float a[4], float b[4])
{
    for (int i = 0; i < 4; ++i)
        dst[i] = a[i] * b[i];
    return dst;
}

static unsigned int color_max(unsigned int a, unsigned int b)
{
    return 
          m_maxu32(a & MASK_ALPHA, b & MASK_ALPHA)
        | m_maxu32(a & MASK_RED,   b & MASK_RED)
        | m_maxu32(a & MASK_GREEN, b & MASK_GREEN)
        | m_maxu32(a & MASK_BLUE,  b & MASK_BLUE);
}

static unsigned int color_min(unsigned int a, unsigned int b)
{
    return 
          m_minu32(a & MASK_ALPHA, b & MASK_ALPHA)
        | m_minu32(a & MASK_RED,   b & MASK_RED)
        | m_minu32(a & MASK_GREEN, b & MASK_GREEN)
        | m_minu32(a & MASK_BLUE,  b & MASK_BLUE);
}

static unsigned int color_add(unsigned int a, unsigned int b)
{
    unsigned int a_argb[4];
    unsigned int b_argb[4];
    to_argb(a_argb, a);
    to_argb(b_argb, b);

    return from_argb_clamped((unsigned int[4]){
            a_argb[0] + b_argb[0],
            a_argb[1] + b_argb[1],
            a_argb[2] + b_argb[2],
            a_argb[3] + b_argb[3],
    });
}

static unsigned int lerp_colors(unsigned int a_color, unsigned int b_color, float alpha)
{
    unsigned int a[4]; to_argb(a, a_color);
    unsigned int b[4]; to_argb(b, b_color);

    // Saturate
    if (alpha < 0.f) alpha = 0.f;
    if (alpha > 1.f) alpha = 1.f;

    // Mix
    unsigned int res[4];
    for (int i = 0; i < 4; ++i)
        res[i] = (int)((1.f - alpha) * a[i] + alpha * b[i]);

    return from_argb(res);
}

//-------------------------------------------------------------------------
// Viewport<->Layer coordinates conversion
//-------------------------------------------------------------------------
static bool viewport_to_layer(int* x, int* y)
{
    int left   = ctx.current_layer->viewport[0];
    int top    = ctx.current_layer->viewport[1];
    int right  = ctx.current_layer->viewport[0] + ctx.current_layer->viewport[2];
    int bottom = ctx.current_layer->viewport[1] + ctx.current_layer->viewport[3];

    if (x != NULL) *x += left;
    if (y != NULL) *y += top;

    bool in_viewport = ((x != NULL) && (*x >= left && *x < right)) 
                    && ((y != NULL) && (*y >= top && *y < bottom));
                    
    bool in_screen = ((x != NULL) && (*x >= 0 && *x < ctx.current_layer->width)) 
                  && ((y != NULL) && (*y >= 0 && *y < ctx.current_layer->height));

    return in_viewport && in_screen;
}

static bool layer_to_viewport(int* x, int* y)
{
    *x -= ctx.current_layer->viewport[0];
    *y -= ctx.current_layer->viewport[1];

    return ((x != NULL) && (*x >= 0 && *x < ctx.current_layer->viewport[2])) 
        && ((y != NULL) && (*y >= 0 && *y < ctx.current_layer->viewport[3]));
}

static void transform_with_viewport(int viewport[4], float* x, float* y)
{
    *x = (*x - viewport[0]) / viewport[2];
    *y = (*y - viewport[1]) / viewport[3];
}
//-------------------------------------------------------------------------
// image_t class
//-------------------------------------------------------------------------
static struct image_t image_load_from_file(const char* imageFile)
{
    struct image_t image = {0};
    stbi_set_flip_vertically_on_load(0);
    image.pixels = (struct u32_color*)stbi_load(imageFile, &image.width, &image.height, NULL, STBI_rgb_alpha);

    if (image.pixels == NULL)
    {
        fprintf(stderr, "Cannot load image: %s\n", imageFile);
        return image;
    }

    return image;
}

static void image_unload(struct image_t image)
{
    free(image.pixels);
}

//-------------------------------------------------------------------------
// texture_t class
//-------------------------------------------------------------------------
static struct texture_t texture_load(int width, int height, struct u32_color* pixels)
{
    struct texture_t texture = {0};

    texture.width  = width;
    texture.height = height;
    glGenTextures(1, &texture.name);

    glBindTexture(GL_TEXTURE_2D, texture.name);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture.width, texture.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    return texture;
}

static void texture_unload(struct texture_t texture)
{
    glDeleteTextures(1, &texture.name);
}

//-------------------------------------------------------------------------
// framebuffer_t class
//-------------------------------------------------------------------------
static struct framebuffer_t framebuffer_load(int width, int height)
{
    struct framebuffer_t framebuffer = {0};

    GLint previousFramebufferBinding;
    glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &previousFramebufferBinding);

    glGenFramebuffers(1, &framebuffer.name);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer.name);

    framebuffer.texture.width  = width;
    framebuffer.texture.height = height;
    glGenTextures(1, &framebuffer.texture.name);
    glBindTexture(GL_TEXTURE_2D, framebuffer.texture.name);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + 0, GL_TEXTURE_2D, framebuffer.texture.name, 0);

    unsigned int drawAttachments[1] = { GL_COLOR_ATTACHMENT0 + 0 };
    glDrawBuffers(1, drawAttachments);

    GLenum framebufferStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (framebufferStatus != GL_FRAMEBUFFER_COMPLETE)
        fprintf(stderr, "framebuffer_load() FBO failed to complete (0x%x)\n", framebufferStatus);

    glBindFramebuffer(GL_FRAMEBUFFER, previousFramebufferBinding);

    return framebuffer;
}

static void framebuffer_unload(struct framebuffer_t framebuffer)
{
    glDeleteTextures(1, &framebuffer.texture.name);
    glDeleteFramebuffers(1, &framebuffer.name);
}

//-------------------------------------------------------------------------
// OpenGL objects creation/deletion
//-------------------------------------------------------------------------
static void layer_gl_create_data(struct layer_t* cur)
{
    // Create vertex buffer
    {
        glGenBuffers(1, &cur->vertex_pos_buffer);
        glBindBuffer(GL_ARRAY_BUFFER, cur->vertex_pos_buffer);
        glBufferData(GL_ARRAY_BUFFER, cur->width * cur->height * 6 * sizeof(struct vertex_pos_t), NULL, GL_STREAM_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glGenBuffers(1, &cur->vertex_uv_color_buffer);
        glBindBuffer(GL_ARRAY_BUFFER, cur->vertex_uv_color_buffer);
        glBufferData(GL_ARRAY_BUFFER, cur->width * cur->height * 6 * sizeof(struct vertex_uv_color_t), NULL, GL_STREAM_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    // Create vertex array object
    {
        glGenVertexArrays(1, &cur->vao);
        glBindVertexArray(cur->vao);
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glEnableVertexAttribArray(2);
        glEnableVertexAttribArray(3);

        glBindBuffer(GL_ARRAY_BUFFER, cur->vertex_pos_buffer);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(struct vertex_pos_t), (void*)offsetof(struct vertex_pos_t, pos));
        glBindBuffer(GL_ARRAY_BUFFER, cur->vertex_uv_color_buffer);
        glVertexAttribPointer(1, 2, GL_UNSIGNED_SHORT, GL_TRUE, sizeof(struct vertex_uv_color_t), (void*)offsetof(struct vertex_uv_color_t, uv));
        glVertexAttribPointer(2, 4, GL_UNSIGNED_BYTE, GL_TRUE,  sizeof(struct vertex_uv_color_t), (void*)offsetof(struct vertex_uv_color_t, fg_color));
        glVertexAttribPointer(3, 4, GL_UNSIGNED_BYTE, GL_TRUE,  sizeof(struct vertex_uv_color_t), (void*)offsetof(struct vertex_uv_color_t, bg_color));

        glBindVertexArray(0);
    }
}

static void layer_gl_delete_data(struct layer_t* cur)
{
    glDeleteVertexArrays(1, &cur->vao);
    glDeleteBuffers(1, &cur->vertex_pos_buffer);
    glDeleteBuffers(1, &cur->vertex_uv_color_buffer);
}

static void gl_create_data(void)
{
    // Compile program
    {
        char bufferLog[1024];

        ctx.program = glCreateProgram();

        GLuint vertShader = glCreateShader(GL_VERTEX_SHADER);
        GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);

        const char* vertShaderStr =
"#version 330\n"
"layout(location = 0) in vec2 aPosition;\n"
"layout(location = 1) in vec2 aUV;\n"
"layout(location = 2) in vec4 aFgColor;\n"
"layout(location = 3) in vec4 aBgColor;\n"
"out vec2 vUV;\n"
"out vec4 vFgColor;\n"
"out vec4 vBgColor;\n"
"void main()\n"
"{\n"
"    vUV = aUV;\n"
"    vFgColor = vec4(aFgColor.b, aFgColor.g, aFgColor.r, aFgColor.a);\n" // Fast way to convert BGRA to RGBA
"    vBgColor = vec4(aBgColor.b, aBgColor.g, aBgColor.r, aBgColor.a);\n"
"    gl_Position = vec4(aPosition, 0.0, 1.0);\n"
"}\n"
"\n";

        const char* fragShaderStr =
"#version 330\n"
"uniform sampler2D uTexture;\n"
"in vec2 vUV;\n"
"in vec4 vFgColor;\n"
"in vec4 vBgColor;\n"
"out vec4 oColor;\n"
"void main()\n"
"{\n"
"    vec4 fgColor = vFgColor * texture(uTexture, vUV);\n"
"    oColor = mix(vBgColor, fgColor, fgColor.a);\n"
"}\n"
"\n";

        glShaderSource(vertShader, 1, &vertShaderStr, NULL);
        glShaderSource(fragShader, 1, &fragShaderStr, NULL);

        glCompileShader(vertShader);
        {
            GLint compileStatus; glGetShaderiv(vertShader, GL_COMPILE_STATUS, &compileStatus);
            if (compileStatus == GL_FALSE)
            {
                glGetShaderInfoLog(vertShader, PG_ARRAYSIZE(bufferLog), NULL, bufferLog);
                fprintf(stderr, "Vertex shader compile error:\n%s", bufferLog);
            }
        }
        glCompileShader(fragShader);
        {
            GLint compileStatus; glGetShaderiv(fragShader, GL_COMPILE_STATUS, &compileStatus);
            if (compileStatus == GL_FALSE)
            {
                glGetShaderInfoLog(fragShader, PG_ARRAYSIZE(bufferLog), NULL, bufferLog);
                fprintf(stderr, "Fragment shader compile error:\n%s", bufferLog);
            }
        }

        glAttachShader(ctx.program, vertShader);
        glAttachShader(ctx.program, fragShader);

        glLinkProgram(ctx.program);
        {
            GLint linkStatus; glGetProgramiv(ctx.program, GL_LINK_STATUS, &linkStatus);
            if (linkStatus == GL_FALSE)
            {
                glGetProgramInfoLog(ctx.program, PG_ARRAYSIZE(bufferLog), NULL, bufferLog);
                fprintf(stderr, "%s", bufferLog);
            }
        }
    }

    // Compile fs program
    {
        char bufferLog[1024];

        ctx.fs_program = glCreateProgram();

        GLuint vertShader = glCreateShader(GL_VERTEX_SHADER);
        GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);

        const char* vertShaderStr =
"#version 330\n"
"layout(location = 0) in vec2 aPosition;\n"
"layout(location = 1) in vec2 aUV;\n"
"out vec2 vUV;\n"
"void main()\n"
"{\n"
"    vUV = aUV;\n"
"    gl_Position = vec4(aPosition, 0.0, 1.0);\n"
"}\n"
"\n";

        const char* fragShaderStr =
"#version 330\n"
"uniform sampler2D uTexture;\n"
"in vec2 vUV;\n"
"out vec4 oColor;\n"
"void main()\n"
"{\n"
"    oColor = texture(uTexture, vUV);\n"
"}\n"
"\n";

        glShaderSource(vertShader, 1, &vertShaderStr, NULL);
        glShaderSource(fragShader, 1, &fragShaderStr, NULL);

        glCompileShader(vertShader);
        {
            GLint compileStatus; glGetShaderiv(vertShader, GL_COMPILE_STATUS, &compileStatus);
            if (compileStatus == GL_FALSE)
            {
                glGetShaderInfoLog(vertShader, PG_ARRAYSIZE(bufferLog), NULL, bufferLog);
                fprintf(stderr, "Vertex shader compile error:\n%s", bufferLog);
            }
        }
        glCompileShader(fragShader);
        {
            GLint compileStatus; glGetShaderiv(fragShader, GL_COMPILE_STATUS, &compileStatus);
            if (compileStatus == GL_FALSE)
            {
                glGetShaderInfoLog(fragShader, PG_ARRAYSIZE(bufferLog), NULL, bufferLog);
                fprintf(stderr, "Fragment shader compile error:\n%s", bufferLog);
            }
        }

        glAttachShader(ctx.fs_program, vertShader);
        glAttachShader(ctx.fs_program, fragShader);

        glLinkProgram(ctx.fs_program);
        {
            GLint linkStatus; glGetProgramiv(ctx.fs_program, GL_LINK_STATUS, &linkStatus);
            if (linkStatus == GL_FALSE)
            {
                glGetProgramInfoLog(ctx.fs_program, PG_ARRAYSIZE(bufferLog), NULL, bufferLog);
                fprintf(stderr, "%s", bufferLog);
            }
        }
    }

    // Create layer data
    for (int i = 0; i < ctx.layer_count; ++i)
        layer_gl_create_data(&ctx.layers[i]);

    // Create fullscreen vertex buffer and vertex array object
    {
        struct vertex_t quad[6] = {
            { { -1.f,-1.f }, { 0.f, 0.f } },
            { {  1.f,-1.f }, { 1.f, 0.f } },
            { {  1.f, 1.f }, { 1.f, 1.f } },
            { { -1.f,-1.f }, { 0.f, 0.f } },
            { {  1.f, 1.f }, { 1.f, 1.f } },
            { { -1.f, 1.f }, { 0.f, 1.f } },
        };

        glGenBuffers(1, &ctx.fs_vbo);
        glBindBuffer(GL_ARRAY_BUFFER, ctx.fs_vbo);
        glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(struct vertex_t), quad, GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glGenVertexArrays(1, &ctx.fs_vao);
        glBindVertexArray(ctx.fs_vao);
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);

        glBindBuffer(GL_ARRAY_BUFFER, ctx.fs_vbo);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(struct vertex_t), (void*)offsetof(struct vertex_t, pos));
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_TRUE, sizeof(struct vertex_t), (void*)offsetof(struct vertex_t, uv));

        glBindVertexArray(0);
    }

    // Create white texture
    {
        struct u32_color white_pixel = { 0xFF, 0xFF, 0xFF, 0xFF };
        ctx.white = texture_load(1, 1, &white_pixel);
    }

    // Create framebuffer
    {
        int framebuffer_width  = ctx.layers[0].width  * ctx.layers[0].tile_width;
        int framebuffer_height = ctx.layers[0].height * ctx.layers[0].tile_height;
        ctx.framebuffer = framebuffer_load(framebuffer_width, framebuffer_height);
    }

    // Load tileset texture
    for (int i = 0; i < ctx.tileset_count; ++i)
    {
        struct image_t tileset = image_load_from_file(ctx.tilesets[i].path);

        // Replace mask color by fully transparent pixel
        struct u32_color mask = tileset.pixels[0];
        for (int i = 0; i < tileset.width * tileset.height; ++i)
        {
            if (   tileset.pixels[i].r == mask.r
                && tileset.pixels[i].g == mask.g
                && tileset.pixels[i].b == mask.b
                && tileset.pixels[i].a == mask.a)
                tileset.pixels[i] = (struct u32_color){0};
        }

        ctx.tilesets[i].texture = texture_load(tileset.width, tileset.height, tileset.pixels);
        image_unload(tileset);
    }
}

static void gl_delete_data(void)
{
    for (int i = 0; i < ctx.tileset_count; ++i)
        texture_unload(ctx.tilesets[i].texture);

    texture_unload(ctx.white);
    framebuffer_unload(ctx.framebuffer);
    
    for (int i = 0; i < ctx.layer_count; ++i)
        layer_gl_delete_data(&ctx.layers[i]);
    glDeleteVertexArrays(1, &ctx.fs_vao);
    glDeleteBuffers(1, &ctx.fs_vbo);
    glDeleteProgram(ctx.fs_program);
    glDeleteProgram(ctx.program);
}

static void layer_gl_build_mesh_pos(struct layer_t* cur)
{
    {
        struct vertex_pos_t* triangles = &cur->vertex_pos[0];
        for (int i = 0; i < cur->width * cur->height; ++i)
        {
            int dstX = (i % cur->width) * cur->tile_width;
            int dstY = (i / cur->width) * cur->tile_height;
            
            struct vertex_pos_t* verts = &triangles[i * 6];
            verts[0].pos[0] = (float)(dstX);
            verts[0].pos[1] = (float)(dstY);
            verts[1].pos[0] = (float)(dstX + cur->tile_width);
            verts[1].pos[1] = (float)(dstY);
            verts[2].pos[0] = (float)(dstX);
            verts[2].pos[1] = (float)(dstY + cur->tile_height);
            verts[3]        = verts[1];
            verts[4].pos[0] = (float)(dstX + cur->tile_width);
            verts[4].pos[1] = (float)(dstY + cur->tile_height);
            verts[5]        = verts[2];

            // Transform pixel space to ndc space
            for (int j = 0; j < 6; ++j)
            {
                verts[j].pos[0] = verts[j].pos[0] * 2 / ctx.framebuffer.texture.width - 1.f;
                verts[j].pos[1] = (ctx.framebuffer.texture.height - verts[j].pos[1]) * 2 / ctx.framebuffer.texture.height - 1.f;
            }
        }
    }
    glBindBuffer(GL_ARRAY_BUFFER, cur->vertex_pos_buffer);
    glBufferData(GL_ARRAY_BUFFER, cur->width * cur->height * 6 * sizeof(struct vertex_pos_t), cur->vertex_pos, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

static void layer_gl_build_mesh_uv_color(struct layer_t* cur)
{
    struct texture_t tileset = ctx.tilesets[cur->tileset].texture;

    struct vertex_uv_color_t* triangles = &cur->vertex_uv_color[0];
    for (int i = 0; i < cur->width * cur->height; ++i)
    {
        struct tile_t* tile = &cur->tiles[i];

        int srcX = (tile->value % 16) * cur->tile_width;
        int srcY = (tile->value / 16) * cur->tile_height;

        // Set fg/bg color
        struct vertex_uv_color_t* verts = &triangles[i * 6];
        for (int j = 0; j < 6; ++j)
        {
            verts[j].fg_color = lerp_colors(tile->fg_color, ctx.fade_color, ctx.fade_amount);
            verts[j].bg_color = lerp_colors(tile->bg_color, ctx.fade_color, ctx.fade_amount);
        }

        // TODO: Custom tileset per layer
        verts[0].uv[0] = (unsigned short)(((srcX)                    / (float)tileset.width ) * ((unsigned short)-1));
        verts[0].uv[1] = (unsigned short)(((srcY)                    / (float)tileset.height) * ((unsigned short)-1));
        verts[1].uv[0] = (unsigned short)(((srcX + cur->tile_width)  / (float)tileset.width ) * ((unsigned short)-1));
        verts[1].uv[1] = (unsigned short)(((srcY)                    / (float)tileset.height) * ((unsigned short)-1));
        verts[2].uv[0] = (unsigned short)(((srcX)                    / (float)tileset.width ) * ((unsigned short)-1));
        verts[2].uv[1] = (unsigned short)(((srcY + cur->tile_height) / (float)tileset.height) * ((unsigned short)-1));
        verts[4].uv[0] = (unsigned short)(((srcX + cur->tile_width)  / (float)tileset.width ) * ((unsigned short)-1));
        verts[4].uv[1] = (unsigned short)(((srcY + cur->tile_height) / (float)tileset.height) * ((unsigned short)-1));
        verts[3]       = verts[1];
        verts[5]       = verts[2];
    }

    // Update buffer on GPU
    glBindBuffer(GL_ARRAY_BUFFER, cur->vertex_uv_color_buffer);
    glBufferData(GL_ARRAY_BUFFER, cur->width * cur->height * 6 * sizeof(struct vertex_uv_color_t), cur->vertex_uv_color, GL_STREAM_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

//-------------------------------------------------------------------------
// layer_t class
//-------------------------------------------------------------------------
static void layer_init(struct layer_t* cur, int width, int height)
{
    cur->width = width;
    cur->height = height;
    cur->tiles = calloc(cur->width * cur->height, sizeof(struct tile_t));
    cur->viewport[0] = 0;
    cur->viewport[1] = 0;
    cur->viewport[2] = cur->width;
    cur->viewport[3] = cur->height;
    
    cur->vertex_pos = calloc(cur->width * cur->height * 6, sizeof(struct vertex_pos_t));
    cur->vertex_uv_color = calloc(cur->width * cur->height * 6, sizeof(struct vertex_uv_color_t));
}

static void layer_delete(struct layer_t* cur)
{
    free(cur->vertex_uv_color);
    free(cur->vertex_pos);
    free(cur->tiles);
    *cur = (struct layer_t){0};
}

//-------------------------------------------------------------------------
// Playground main functions
//-------------------------------------------------------------------------
int pg_init(pg_glloadfunc loadFunc, struct pg_config_t config)
{
    // TODO: Assert that ctx is not already initialized
    ctx = (struct context_t){0};

    // Init glad
    if (!gladLoadGL(loadFunc))
    {
        fprintf(stderr, "gladLoadGL failed\n");
        return 1;
    }

    // Print OpenGL info
    {
        printf("GL_VERSION: %s\n", glGetString(GL_VERSION));
        printf("GL_VENDOR: %s\n", glGetString(GL_VENDOR));
        printf("GL_RENDERER: %s\n", glGetString(GL_RENDERER));
    }
    
    // Init miniaudio audio engine
    if (ma_engine_init(NULL, &ctx.audio) != MA_SUCCESS)
        fprintf(stderr, "Failed to init audio engine\n");
    
    // Print miniaudio info
    {
        ma_device_info device_info;
        ma_device_get_info(ma_engine_get_device(&ctx.audio), ma_device_type_playback, &device_info);
        uint32_t major, minor, revision;
        ma_version(&major, &minor, &revision);
        printf("MINIAUDIO_VERSION: %d.%d.%d\n", major, minor, revision);
        printf("MINIAUDIO_BACKEND: %s\n", ma_get_backend_name(ma_device_get_context(ma_engine_get_device(&ctx.audio))->backend));
        printf("MINIAUDIO_DEVICE: %s\n", device_info.name);
    }

    // Init config system
    cfg_init(&ctx.config);

    // Pre-load sounds
    ma_sound_group_init(&ctx.audio, 0, 0, &ctx.sound_group);
    ctx.sound_count = config.sound_count;
    ctx.sounds = calloc(ctx.sound_count, sizeof(struct sound_list_t));
    for (int i = 0; i < ctx.sound_count; ++i)
    {
        struct sound_list_t* sound_list = &ctx.sounds[i];
        sound_list->base_volume = linear_to_volume(config.sounds[i].base_volume);
        if (sound_list->base_volume == 0.f)
            sound_list->base_volume = 1.f;

        if (ma_sound_init_from_file(&ctx.audio, config.sounds[i].path, 0, &ctx.sound_group, NULL, &sound_list->head.sound) != MA_SUCCESS)
            fprintf(stderr, "Cannot load sound: '%s'\n", config.sounds[i].path);
    }

    // Register music paths
    ma_sound_group_init(&ctx.audio, 0, 0, &ctx.music_group);
    ctx.music_count = config.music_count;
    ctx.music_paths = calloc(ctx.music_count, sizeof(char*));
    for (int i = 0; i < ctx.music_count; ++i)
        ctx.music_paths[i] = string_duplicate(config.music_paths[i]);

    // Tilesets
    ctx.tileset_count = config.tileset_count;
    ctx.tilesets = calloc(ctx.tileset_count, sizeof(struct tileset_t));

    for (int i = 0; i < ctx.tileset_count; ++i)
        ctx.tilesets[i].path = string_duplicate(config.tileset_paths[i]);

    ctx.defaut_bg_color = 0x00000000;
    ctx.defaut_fg_color = 0xFFFFFFFF;

    // Create layers
    ctx.layer_count = config.layer_count;
    ctx.layers = calloc(ctx.layer_count, sizeof(struct layer_t));

    // Extract tile size from tileset image
    {
        for (int i = 0; i < ctx.layer_count; ++i)
        {
            struct pg_layer_config_t* layer_config = &config.layers[i];
            struct layer_t* cur = &ctx.layers[i];
            int tileset_pixel_width;
            int tileset_pixel_height;
            if (stbi_info(ctx.tilesets[layer_config->tileset_id].path, &tileset_pixel_width, &tileset_pixel_height, NULL) == 0)
            {
                fprintf(stderr, "Cannot read %s (%s)\n", ctx.tilesets[layer_config->tileset_id].path, stbi_failure_reason());
                exit(1);
            }
            cur->tile_width  = (tileset_pixel_width  / 16);
            cur->tile_height = (tileset_pixel_height / 16);
            cur->tileset = layer_config->tileset_id;
        }
    }

    layer_init(&ctx.layers[0], config.width, config.height);

    // Other layers size are set to fit in layer 0 size
    int pixel_width  = ctx.layers[0].width  * ctx.layers[0].tile_width;
    int pixel_height = ctx.layers[0].height * ctx.layers[0].tile_height;
    for (int i = 1; i < ctx.layer_count; ++i)
    {
        int width  = pixel_width  / ctx.layers[i].tile_width;
        int height = pixel_height / ctx.layers[i].tile_height;
        layer_init(&ctx.layers[i], width, height);
    }

    ctx.current_layer = &ctx.layers[0];

    ctx.display_scale = cfg_get_int(&ctx.config, CFG_KEY_DISPLAY_SCALE, 1);
    ctx.display_scale = m_clamps32(ctx.display_scale, 1, 3);

    // HACK: Do not apply scale directly to allow size limit on the backend size
    // TODO: Find a proper way
    ctx.io.display_width  = ctx.current_layer->width  * ctx.layers[0].tile_width;// * ctx.display_scale;
    ctx.io.display_height = ctx.current_layer->height * ctx.layers[0].tile_height;// * ctx.display_scale;

    gl_create_data();

    // Build grid mesh since position won't change during execution
    for (int i = 0; i < ctx.layer_count; ++i)
        layer_gl_build_mesh_pos(&ctx.layers[i]);

    // Print buffer size info
    {
        int vertexCount = 0;
        for (int i = 0; i < ctx.layer_count; ++i)
        {
            struct layer_t* layer = &ctx.layers[i];
            vertexCount += layer->width * layer->height * 6; // 6 vertices per quad, one quad per tile (see layer_init())
        }
        printf("Vertex buffers size: %.2f KB\n", vertexCount * (sizeof(struct vertex_pos_t) + sizeof(struct vertex_uv_color_t)) / 1024.f);
    }

    pg_clear();

    return 0;
}

void pg_shutdown(void)
{
    // TILES
    for (int i = 0; i < ctx.tileset_count; ++i)
        free(ctx.tilesets[i].path);
    free(ctx.tilesets);

    // SOUNDS
    for (int i = 0; i < ctx.sound_count; ++i)
    {
        for (struct sound_item_t* sound = &ctx.sounds[i].head; sound != NULL; sound = sound->next)
            ma_sound_uninit(&sound->sound);
    }
    free(ctx.sounds);

    // MUSICS
    for (int i = 0; i < ctx.music_count; ++i)
        free(ctx.music_paths[i]);
    free(ctx.music_paths);

    for (int i = 0; i < ctx.layer_count; ++i)
    {
        layer_gl_delete_data(&ctx.layers[i]);
        layer_delete(&ctx.layers[i]);
    }
    free(ctx.layers);

    gl_delete_data();

    ma_engine_uninit(&ctx.audio);

    ctx = (struct context_t){0};
}

void pg_render(void)
{
    {
        for (int i = 0; i < 3; ++i)
        {
            // TODO: GLFW_KEY_F1 == 290 -> Add mapping
            if (pg_io_key_pressed(290 + i))
            {
                ctx.display_scale = i+1;
                cfg_set_int(&ctx.config, CFG_KEY_DISPLAY_SCALE, ctx.display_scale);
            }
        }
    }

    // GLOBAL SOUND CONTROL
#if 1
    if (pg_io_key_down(GLFW_KEY_LEFT_ALT))
    {
        if (pg_io_key_pressed(GLFW_KEY_I))
            pg_music_set_global_volume(0.f);
        if (pg_io_key_pressed(GLFW_KEY_O))
            pg_music_set_global_volume(1.f);
        if (pg_io_key_pressed(GLFW_KEY_K))
            pg_sound_set_global_volume(0.f);
        if (pg_io_key_pressed(GLFW_KEY_L))
            pg_sound_set_global_volume(1.f);
    }
#endif

#if 0
    static float timer = 0;
    if (timer <= 0)
    {
        printf("%3d FPS\n", (int)(1.f / ctx.io.frame_time));
        timer += 1.f;
    }
    else
    {
        timer -= ctx.io.frame_time;
    }
#endif

#if 0
    for (int i = 0; i < ctx.sound_count; ++i)
    {
        int j = 0;
        for (struct sound_item_t* sound = &ctx.sounds[i].head; sound != NULL; sound = sound->next, j++)
        {
            if (ma_sound_is_playing(&sound->sound))
                pg_put_char(j, i, 'p', PG_COL_REPLACE);
            else if (ma_sound_at_end(&sound->sound))
                pg_put_char(j, i, 'e', PG_COL_REPLACE);
            else
                pg_put_char(j, i, 'n', PG_COL_REPLACE);
        }
    }
#endif

    ctx.io.display_width  = ctx.layers[0].width  * ctx.layers[0].tile_width  * ctx.display_scale;
    ctx.io.display_height = ctx.layers[0].height * ctx.layers[0].tile_height * ctx.display_scale;

    int gl_viewport[4]; glGetIntegerv(GL_VIEWPORT, gl_viewport);

    for (int i = 0; i < ctx.layer_count; ++i)
    {
        layer_gl_build_mesh_uv_color(&ctx.layers[i]);
    }

    // Draw
    {
        // Setup
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glUseProgram(ctx.program);

        glBindFramebuffer(GL_FRAMEBUFFER, ctx.framebuffer.name);
        {
            glViewport(0, 0, ctx.framebuffer.texture.width, ctx.framebuffer.texture.height);
            glClear(GL_COLOR_BUFFER_BIT);
            for (int i = 0; i < ctx.layer_count; ++i)
            {
                GLuint vao = ctx.layers[i].vao;
                int nb_tiles = ctx.layers[i].width * ctx.layers[i].height;

                glBindVertexArray(vao);

                // Draw grid
                glEnable(GL_BLEND);
                glBindTexture(GL_TEXTURE_2D, ctx.tilesets[ctx.layers[i].tileset].texture.name);
                glDrawArrays(GL_TRIANGLES, 0, nb_tiles * 6);
                glDisable(GL_BLEND);
            }
        }

        // TODO: Clean this with mouse inputs
        float scale = (float)m_maxs32(1, m_mins32(ctx.io.display_width / ctx.framebuffer.texture.width, ctx.io.display_height / ctx.framebuffer.texture.height));
        //float scale = m_minf(ctx.io.display_width / (float)ctx.framebuffer.texture.width, ctx.io.display_height / (float)ctx.framebuffer.texture.height);

        // TODO: Should be relative to original viewport
        glViewport(
            (int)((ctx.io.display_width  - scale * ctx.framebuffer.texture.width) / 2.f),
            (int)((ctx.io.display_height - scale * ctx.framebuffer.texture.height) / 2.f),
            (int)(scale * ctx.framebuffer.texture.width),
            (int)(scale * ctx.framebuffer.texture.height)
        );

#if 1
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glUseProgram(ctx.fs_program);
        glClear(GL_COLOR_BUFFER_BIT);
        glBindVertexArray(ctx.fs_vao);
        glBindTexture(GL_TEXTURE_2D, ctx.framebuffer.texture.name);
        glDrawArrays(GL_TRIANGLES, 0, 6);
#endif
    }

    cfg_flush(&ctx.config);
}

struct pg_io* pg_get_io(void)
{
    return &ctx.io;
}

struct cfg_t* pg_get_config(void)
{
    return &ctx.config;
}

int pg_io_key_down(int native_key)
{
    return ctx.io.keys_down[native_key];
}

int pg_io_key_pressed(int native_key)
{
    return !ctx.io.keys_down_prev[native_key] && ctx.io.keys_down[native_key];
}

int pg_io_key_up(int native_key)
{
    return !ctx.io.keys_down[native_key];
}

int pg_io_key_released(int native_key)
{
    return ctx.io.keys_down_prev[native_key] && !ctx.io.keys_down[native_key];
}

int pg_io_mouse_button_down(int native_key)
{
    return ctx.io.mouse_buttons_down[native_key];
}

int pg_io_mouse_button_pressed(int native_key)
{
    return !ctx.io.mouse_buttons_down_prev[native_key] && ctx.io.mouse_buttons_down[native_key];
}

int pg_io_mouse_button_up(int native_key)
{
    return !ctx.io.mouse_buttons_down[native_key];
}

int pg_io_mouse_button_released(int native_key)
{
    return ctx.io.mouse_buttons_down_prev[native_key] && !ctx.io.mouse_buttons_down[native_key];
}

float pg_io_get_frame_time(void)
{
    return ctx.io.frame_time;
}

static void pg_clear_with(int fg, int bg)
{
    for (int i = 0; i < ctx.current_layer->width * ctx.current_layer->height; ++i)
    {
        struct tile_t* tile = &ctx.current_layer->tiles[i];
        tile->value = ' ';
        tile->fg_color = fg;
        tile->bg_color = bg;
    }
}

void pg_clear(void)
{
    // Set alpha to 0 for layers other than 0
    unsigned int bg = ctx.defaut_bg_color;
    if (ctx.current_layer != &ctx.layers[0])
        bg = (~MASK_ALPHA & bg);

    pg_clear_with(ctx.defaut_fg_color, bg);
}

void pg_clear_all(void)
{
    struct layer_t* cur = ctx.current_layer;
    for (int i = 0; i < ctx.layer_count; ++i)
    {
        pg_set_layer(i);
        pg_clear();
    }
    ctx.current_layer = cur;
}

void pg_set_viewport(int x, int y, int width, int height)
{
    ctx.current_layer->viewport[0] = x;
    ctx.current_layer->viewport[1] = y;
    ctx.current_layer->viewport[2] = width;
    ctx.current_layer->viewport[3] = height;
}

void pg_get_viewport(int* x, int* y, int* width, int* height)
{
    if (x      != NULL) *x      = ctx.current_layer->viewport[0];
    if (y      != NULL) *y      = ctx.current_layer->viewport[1];
    if (width  != NULL) *width  = ctx.current_layer->viewport[2];
    if (height != NULL) *height = ctx.current_layer->viewport[3];
}

int pg_get_viewport_width(void)
{
    return ctx.current_layer->viewport[2];
}

int pg_get_viewport_height(void)
{
    return ctx.current_layer->viewport[3];
}

int pg_get_viewport_mouse_coords(int* x, int* y)
{
    pg_get_mouse_coords(x, y);
    return layer_to_viewport(x, y);
}

void pg_reset_viewport(void)
{
    pg_set_viewport(0, 0, ctx.current_layer->width, ctx.current_layer->height);
}

void pg_set_layer(int layer_id)
{
    assert(layer_id >= 0 && layer_id < ctx.layer_count);
    ctx.current_layer = &ctx.layers[layer_id];
}

void pg_get_layer_dimensions(int* width, int* height)
{
    if (width  != NULL) *width  = ctx.current_layer->width;
    if (height != NULL) *height = ctx.current_layer->height;
}

static unsigned int blend_colors(unsigned int dst_color, unsigned int src_color, enum pg_color_effect fx)
{
    float src[4];
    float dst[4];
    float res[4];
    switch (fx)
    {
    case PG_COL_NONE:
        return dst_color;

    default:
    case PG_COL_REPLACE:
        return src_color;

    case PG_COL_MULTIPLY:
        return float_to_color(
            multiply_float_color(res,
                color_to_float(dst, dst_color),
                color_to_float(src, src_color)
            )
        );

    case PG_COL_LIGHTEN:
        return color_max(dst_color, src_color);

    case PG_COL_DARKEN:
        return color_min(dst_color, src_color);

    case PG_COL_ADD:
        return color_add(dst_color, src_color);
    }
}

void pg_set_default_fg_color(unsigned int color)
{
    ctx.defaut_fg_color = color;
}

void pg_set_default_bg_color(unsigned int color)
{
    ctx.defaut_bg_color = color;
}

void pg_set_fade(float amount, unsigned int color)
{
    ctx.fade_amount = amount;
    ctx.fade_color = color;
}

void pg_set_tile_fg_color(int x, int y, unsigned int color)
{
    if (!viewport_to_layer(&x, &y))
        return;

    struct tile_t* tile = &ctx.current_layer->tiles[x + y * ctx.current_layer->width];
    tile->fg_color = color;
}

void pg_set_tile_bg_color(int x, int y, unsigned int color)
{
    if (!viewport_to_layer(&x, &y))
        return;

    struct tile_t* tile = &ctx.current_layer->tiles[x + y * ctx.current_layer->width];
    tile->bg_color = color;
}

void pg_set_tile_bg_color_ex(int x, int y, unsigned int color, enum pg_color_effect fx)
{
    if (!viewport_to_layer(&x, &y))
        return;

    struct tile_t* tile = &ctx.current_layer->tiles[x + y * ctx.current_layer->width];
    tile->bg_color = blend_colors(tile->bg_color, color, fx);
}

void pg_set_tile_char(int x, int y, unsigned char c)
{
    if (!viewport_to_layer(&x, &y))
        return;

    struct tile_t* tile = &ctx.current_layer->tiles[x + y * ctx.current_layer->width];
    tile->value = c;
}

void pg_put_char(int x, int y, unsigned char c, enum pg_color_effect fx)
{
    if (!viewport_to_layer(&x, &y))
        return;

    struct tile_t* tile = &ctx.current_layer->tiles[x + y * ctx.current_layer->width];
    tile->value = c;
    tile->fg_color = ctx.defaut_fg_color;
    tile->bg_color = blend_colors(tile->bg_color, ctx.defaut_bg_color, fx);
}

void pg_put_char_ex(int x, int y, unsigned char c, unsigned int fg_color, unsigned int bg_color, enum pg_color_effect fx)
{
    if (!viewport_to_layer(&x, &y))
        return;

    struct tile_t* tile = &ctx.current_layer->tiles[x + y * ctx.current_layer->width];
    tile->value = c;
    tile->fg_color = fg_color;
    tile->bg_color = blend_colors(tile->bg_color, bg_color, fx);
}

int pg_get_mouse_coords(int* x, int* y)
{
    struct pg_io* io = &ctx.io;

    float scale = (float)m_maxs32(1, m_mins32(ctx.io.display_width / ctx.framebuffer.texture.width, ctx.io.display_height / ctx.framebuffer.texture.height));
    int viewport[4] = {
        (int)((ctx.io.display_width  - scale * ctx.framebuffer.texture.width) / 2.f),
        (int)((ctx.io.display_height - scale * ctx.framebuffer.texture.height) / 2.f),
        (int)(scale * ctx.framebuffer.texture.width),
        (int)(scale * ctx.framebuffer.texture.height)
    };

    float mouseX = io->mouse_position[0];
    float mouseY = io->mouse_position[1];
    transform_with_viewport(viewport, &mouseX, &mouseY);

    // Transform to tile coords
    if (x != NULL) *x = (int)floorf(mouseX * ctx.current_layer->width);
    if (y != NULL) *y = (int)floorf(mouseY * ctx.current_layer->height);

    if (((x != NULL) && (*x < 0 || *x >= ctx.current_layer->width))
     || ((y != NULL) && (*y < 0 || *y >= ctx.current_layer->height)))
        return 0;
    else
        return 1;
}

void pg_sound_play(int sound_id, float volume)
{
    assert(sound_id >= 0 && sound_id < ctx.sound_count);

    struct sound_item_t* head = &ctx.sounds[sound_id].head;

    // TODO: Lock ?
    // Search free sound
    struct sound_item_t* freeSound = NULL;
    struct sound_item_t* last = NULL;
    for (struct sound_item_t* sound = head; sound != NULL; sound = sound->next)
    {
        if (!ma_sound_is_playing(&sound->sound))
        {
            freeSound = sound;
            break;
        }
        last = sound;
    }

    if (freeSound == NULL) // No free sound found
    {
        // Add sound to linked list
        freeSound = calloc(1, sizeof(struct sound_item_t));
        last->next = freeSound;

        ma_sound_init_copy(&ctx.audio, &head->sound, 0, &ctx.sound_group, &freeSound->sound);
    }

    ma_sound_start(&freeSound->sound);
    ma_sound_set_volume(&freeSound->sound, linear_to_volume(ctx.sounds[sound_id].base_volume * volume));
}

float pg_sound_get_global_volume(void)
{
    return volume_to_linear(ma_sound_group_get_volume(&ctx.sound_group));
}

void pg_sound_set_global_volume(float volume)
{
    ma_sound_group_set_volume(&ctx.sound_group, linear_to_volume(volume));
}

void pg_music_start(int music_id)
{
    assert(music_id >= 0 && music_id < ctx.music_count);
    ma_sound_uninit(&ctx.music);

    ma_result rc = ma_sound_init_from_file(&ctx.audio, ctx.music_paths[music_id], MA_SOUND_FLAG_STREAM, &ctx.music_group, NULL, &ctx.music);
    if (rc != MA_SUCCESS)
        fprintf(stderr, "Cannot play music: '%s'\n", ctx.music_paths[music_id]);

    ma_sound_start(&ctx.music);
    ma_sound_set_looping(&ctx.music, true);
}

void pg_music_stop(void)
{
    ma_sound_stop(&ctx.music);
}

void pg_music_fade_in(float start_volume, float end_volume, float time_in_secs)
{
    ma_sound_set_fade_in_milliseconds(&ctx.music, linear_to_volume(start_volume), linear_to_volume(end_volume), (ma_uint64)(time_in_secs * 1000.f));
}

void pg_music_set_volume(float volume)
{
    ma_sound_set_volume(&ctx.music, linear_to_volume(volume));
}

float pg_music_get_global_volume(void)
{
    return volume_to_linear(ma_sound_group_get_volume(&ctx.music_group));
}

void pg_music_set_global_volume(float volume)
{
    ma_sound_group_set_volume(&ctx.music_group, linear_to_volume(volume));
}
