
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <playground.h>

#include "../src/gui.h"

int clickCount1 = 0;
int clickCount2 = 0;

void test_button_update(void)
{
    if (im_button(1, 1, "Test button 1"))
        clickCount1 += 1;
    if (im_button(1, 2, "Test button 2"))
        clickCount2 += 1;
    
    im_print(1, 4, "Click count 1: %d", clickCount1);
    im_print(1, 5, "Click count 2: %d", clickCount2);
}
