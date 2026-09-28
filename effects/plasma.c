#include <stdint.h>
#include <math.h>

#include "framebuffer.h"

void plasma_render(uint32_t frame)
{
    for (int y = 0; y < HEIGHT; y++)
    {
        for (int x = 0; x < WIDTH; x++)
        {
            float v = sinf((x * 0.04f) + (frame * 0.05f)) +
                      sinf((y * 0.05f) + (frame * 0.03f)) +
                      sinf(((x + y) * 0.03f) + (frame * 0.04f));

            uint8_t r = (uint8_t)(128.0f + (127.0f * sinf(v)));

            uint8_t g = (uint8_t)(128.0f + (127.0f * sinf(v + 2.0f)));

            uint8_t b = (uint8_t)(128.0f + (127.0f * sinf(v + 4.0f)));

            uint32_t color = (r << 16) | (g << 8) | b;

            put_pixel(x, y, color);
        }
    }
}
