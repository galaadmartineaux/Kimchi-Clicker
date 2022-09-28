#pragma once

//-------------------------------------------------------------------------
// Common macros
//-------------------------------------------------------------------------
#define PG_ARRAYSIZE(arr) (int)(sizeof(arr) / sizeof(arr[0]))

//-------------------------------------------------------------------------
// Backend communication data
//-------------------------------------------------------------------------
struct pg_io
{
    int display_width;
    int display_height;

    double time;
    float frame_time;

    int keys_down[512];
    int keys_down_prev[512];

    float mouse_position[2];
    int mouse_buttons_down[7];
    int mouse_buttons_down_prev[7];

    float mouse_wheel_offset;
};

struct pg_io* pg_get_io(void);
void pg_get_optimal_window_size(int* width, int* height);

//-------------------------------------------------------------------------
// Config system
//-------------------------------------------------------------------------
enum cfg_entry_type
{
    CFG_INT
};

struct cfg_entry_t
{
    char key[64];
    enum cfg_entry_type type;
    union
    {
        int intVal;
    };
};

struct cfg_t
{
    bool dirty;
    int entry_count;
    struct cfg_entry_t entries[128];
};

struct cfg_t* pg_get_config(void);

void cfg_init(struct cfg_t* cfg);
void cfg_flush(struct cfg_t* cfg);
void cfg_set_int(struct cfg_t* cfg, const char* key, int value);
int cfg_get_int(struct cfg_t* cfg, const char* key, int defaultValue);
