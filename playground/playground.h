#pragma once

// OpenGL "GetProcAddress" style function type
typedef void (*pg_apiproc)(void);
typedef pg_apiproc (*pg_glloadfunc)(const char* name);

enum pg_color_effect
{
    PG_COL_NONE,
    PG_COL_REPLACE = 1,
    PG_COL_MULTIPLY,
    PG_COL_LIGHTEN,
    PG_COL_DARKEN,
    PG_COL_ADD,
    PG_COL_DEFAULT
};

struct pg_layer_config_t
{
    int tileset_id;
};

struct pg_sound_config_t
{
    const char* path;
    float base_volume;
};

struct pg_config_t
{
    // Relative to layer[0] tiles
    int width;
    int height;

    int tileset_count;
    const char** tileset_paths;

    int layer_count;
    struct pg_layer_config_t* layers;

    int sound_count;
    struct pg_sound_config_t* sounds;

    int music_count;
    const char** music_paths;
};

// Helper macro to create a simple 1 tileset + 1 layer configuration
#define pg_config_simple(w, h, tileset_path) \
    (struct pg_config_t) \
    { \
        .width = w, \
        .height = h, \
        .tileset_count = 1, \
        .tileset_paths = (const char*[]){ \
            tileset_path \
        }, \
        .layer_count = 1, \
        .layers = (struct pg_layer_config_t[]){ \
            { 0 }, \
        } \
    }

int pg_init(pg_glloadfunc loadFunc, struct pg_config_t config);
void pg_shutdown(void);
void pg_render(void);

// TODO: Remove native keys and copy glfw enum
int pg_io_key_down(int native_key);
int pg_io_key_pressed(int native_key);
int pg_io_key_up(int native_key);
int pg_io_key_released(int native_key);

int pg_io_mouse_button_down(int native_key);
int pg_io_mouse_button_pressed(int native_key);
int pg_io_mouse_button_up(int native_key);
int pg_io_mouse_button_released(int native_key);

float pg_io_get_frame_time(void);

// Per app
void pg_clear_all(void); // Clear all layers with bg and fg colors (on layers other than 0, bg is transparent)
void pg_set_default_fg_color(unsigned int color);
void pg_set_default_bg_color(unsigned int color);
void pg_set_fade(float amount, unsigned int color);
void pg_set_layer(int layer_id); // Change layer

// Audio API
void pg_sound_play(int sound_id, float volume);
float pg_sound_get_global_volume(void);
void pg_sound_set_global_volume(float volume);

void pg_music_start(int music_id);
void pg_music_stop(void);
void pg_music_fade_in(float start_volume, float end_volume, float time_in_secs);
void pg_music_set_volume(float volume);
float pg_music_get_global_volume(void);
void pg_music_set_global_volume(float volume);

// Per layer
void pg_clear(void); // Clear layer with bg and fg colors (on layers other than 0, bg is transparent)
void pg_reset_viewport(void);
void pg_set_viewport(int x, int y, int width, int height);
void pg_get_viewport(int* x, int* y, int* width, int* height);
int pg_get_mouse_coords(int* x, int* y);
void pg_get_layer_dimensions(int* width, int* height);

// Per layer/viewport
int pg_get_viewport_mouse_coords(int* x, int* y);
void pg_set_tile_fg_color(int x, int y, unsigned int color);
void pg_set_tile_bg_color(int x, int y, unsigned int color);
void pg_set_tile_bg_color_ex(int x, int y, unsigned int color, enum pg_color_effect fx);
void pg_set_tile_char(int x, int y, unsigned char c);
void pg_put_char(int x, int y, unsigned char c, enum pg_color_effect fx);
void pg_put_char_ex(int x, int y, unsigned char c, unsigned int fg_color, unsigned int bg_color, enum pg_color_effect fx);
