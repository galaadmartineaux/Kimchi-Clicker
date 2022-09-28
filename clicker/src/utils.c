#include <stdio.h>
#include <stdarg.h>

#include <playground.h>

#include "utils.h"

static int gDebugY = 0;

int vprint_text(int x, int y, const char* format, va_list list)
{
    char buffer[1024];
    int written = vsprintf(buffer, format, list);

    for (int i = 0; buffer[i] != '\0'; ++i)
        pg_put_char(x + i, y, buffer[i], PG_COL_REPLACE);
    
    return written;
}

int print_text(int x, int y, const char* format, ...)
{
    va_list list;
    va_start(list, format);

    int written = vprint_text(x, y, format, list);

    va_end(list);

    return written;
}

void debug_new_frame(void)
{
    gDebugY = 0;
}

int debug_print(const char* format, ...)
{
    va_list list;
    va_start(list, format);

    int written = vprint_text(0, gDebugY++, format, list);

    va_end(list);

    return written;
}

void draw_box(int left, int top, int right, int bottom)
{
    char leftRight   = 0xCD;
    char topBottom   = 0xBA;
    char topLeft     = 0xC9;
    char topRight    = 0xBB;
    char bottomLeft  = 0xC8;
    char bottomRight = 0xBC;

    pg_put_char(left, top, topLeft, PG_COL_REPLACE);
    pg_put_char(right, top, topRight, PG_COL_REPLACE);
    pg_put_char(left, bottom, bottomLeft, PG_COL_REPLACE);
    pg_put_char(right, bottom, bottomRight, PG_COL_REPLACE);

    // Draw top border
    for (int x = left + 1; x <= right - 1; ++x)
        pg_put_char(x, top, leftRight, PG_COL_REPLACE);

    // Draw bottom border
    for (int x = left + 1; x <= right - 1; ++x)
        pg_put_char(x, bottom, leftRight, PG_COL_REPLACE);

    // Draw left border
    for (int y = top + 1; y <= bottom - 1; ++y)
        pg_put_char(left, y, topBottom, PG_COL_REPLACE);

    // Draw right border
    for (int y = top + 1; y <= bottom - 1; ++y)
        pg_put_char(right, y, topBottom, PG_COL_REPLACE);
}
