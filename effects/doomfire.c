#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../framebuffer.h"

// Intensity from 0 to 36
static const uint32_t palette[37] = {
    0x070707, 0x1F0707, 0x2F0F07, 0x470F07, 0x571707, 0x671F07, 0x771F07,
    0x8F2707, 0x9F2F07, 0xAF3F07, 0xBF4707, 0xC74707, 0xDF4F07, 0xDF5707,
    0xDF570F, 0xDF670F, 0xDF6F17, 0xDF7717, 0xDF7F1F, 0xDF871F, 0xDF8F27,
    0xDF9727, 0xDF9F2F, 0xDFA72F, 0xDFAF3F, 0xDFB73F, 0xDFB747, 0xDFBF47,
    0xDFBF4F, 0xC7B747, 0xDFCF57, 0xDFD767, 0xDFDF77, 0xEFEF8F, 0xEFEFC7,
    0xEFEFFF, 0xFFFFFF};

static uint8_t fire_pixels[HEIGHT * WIDTH];
static bool initialized = false;

static void spreadFire(int src_idx)
{
    uint8_t pixel_heat = fire_pixels[src_idx];

    if (pixel_heat == 0)
    {
        fire_pixels[src_idx - WIDTH] = 0;
    }
    else
    {
        int rand_idx = rand() & 3; // 0, 1, 2, or 3
        int decay = rand_idx & 1;  // 0, 1, 0, or 1
        int new_heat = pixel_heat - decay;

        if (new_heat < 0)
        {
            new_heat = 0;
        }

        int dst_idx = (src_idx - WIDTH) - rand_idx + 1; // -2, -1, 0, or +1 on x-axis

        if (dst_idx >= 0 && dst_idx < WIDTH * HEIGHT)
        {
            fire_pixels[dst_idx] = (uint8_t)new_heat;
        }
    }
}

void doomfire_render(uint32_t frame)
{
    if (!initialized)
    {
        for (int i = 0; i < WIDTH * HEIGHT; i++)
        {
            fire_pixels[i] = 0;
        }

        for (int x = 0; x < WIDTH; x++)
        {
            fire_pixels[((HEIGHT - 1) * WIDTH) + x] = 36;
        }

        initialized = true;
    }

    for (int x = 0; x < WIDTH; x++)
    {
        for (int y = 1; y < HEIGHT; y++)
        {
            spreadFire((y * WIDTH) + x);
        }
    }

    for (int y = 0; y < HEIGHT; y++)
    {
        for (int x = 0; x < WIDTH; x++)
        {
            uint8_t heat = fire_pixels[(y * WIDTH) + x];
            uint32_t color = palette[heat];

            put_pixel(x, y, color);
        }
    }
}
