#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include <stdint.h>

#define WIDTH 320
#define HEIGHT 200

extern uint32_t frameBuffer[WIDTH * HEIGHT];

void framebuffer_clear(uint32_t color);

void put_pixel(int x, int y, uint32_t color);

#endif
