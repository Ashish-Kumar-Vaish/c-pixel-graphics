#include "framebuffer.h"

uint32_t frameBuffer[WIDTH * HEIGHT];

void framebuffer_clear(uint32_t color)
{
    for (int i = 0; i < WIDTH * HEIGHT; i++)
    {
        frameBuffer[i] = color;
    }
}

void put_pixel(int x, int y, uint32_t color)
{
    if (x < 0 || x >= WIDTH ||
        y < 0 || y >= HEIGHT)
    {
        return;
    }

    frameBuffer[y * WIDTH + x] = color;
}
