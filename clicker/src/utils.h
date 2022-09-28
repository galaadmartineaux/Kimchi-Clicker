#pragma once

int print_text(int x, int y, const char* format, ...);

void debug_new_frame(void);
int debug_print(const char* format, ...);

void draw_box(int left, int top, int right, int bottom);