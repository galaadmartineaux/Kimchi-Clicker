
#include <stdbool.h>

int im_print_text(int x, int y, const char* text);
int im_print(int x, int y, const char* format, ...);
int im_print_color(int x, int y, unsigned int fg, unsigned int bg, const char* txt, ...);
bool im_button(int x, int y, const char* text);