#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "playground_internal.h"

const char* CFG_FILE = "config.bin";

void cfg_init(struct cfg_t* cfg)
{
    FILE* file = fopen(CFG_FILE, "rb");
    if (file == NULL)
        return;
    fread(&cfg->entry_count, sizeof(cfg->entry_count), 1, file);
    fread(&cfg->entries, sizeof(struct cfg_entry_t), cfg->entry_count, file);
    fclose(file);
}

void cfg_flush(struct cfg_t* cfg)
{
    if (!cfg->dirty)
        return;

    FILE* file = fopen(CFG_FILE, "wb");
    fwrite(&cfg->entry_count, sizeof(cfg->entry_count), 1, file);
    fwrite(&cfg->entries, sizeof(struct cfg_entry_t), cfg->entry_count, file);
    fclose(file);

    cfg->dirty = false;
}

void cfg_set_int(struct cfg_t* cfg, const char* key, int value)
{
    struct cfg_entry_t* existing = NULL;
    for (int i = 0; i < cfg->entry_count; ++i)
    {
        struct cfg_entry_t* entry = &cfg->entries[i];
        if (strcmp(entry->key, key) == 0)
        {
            existing = entry;
            break;
        }
    }

    if (existing == NULL)
    {
        existing = &cfg->entries[cfg->entry_count++];
        strcpy(existing->key, key);
    }

    existing->intVal = value;

    cfg->dirty = true;
}

int cfg_get_int(struct cfg_t* cfg, const char* key, int defaultValue)
{
    for (int i = 0; i < cfg->entry_count; ++i)
    {
        struct cfg_entry_t* entry = &cfg->entries[i];
        if (strcmp(entry->key, key) == 0)
        {
            return entry->intVal;
        }
    }
    return defaultValue;
}
