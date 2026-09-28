#include <stdint.h>
#include <stdlib.h>

#include "framebuffer.h"

#define STAR_COUNT 300

typedef struct
{
    float x;
    float y;
    float z;
} Star;

static Star stars[STAR_COUNT];
static int initialized = 0;

static void init_stars(void)
{
    for (int i = 0; i < STAR_COUNT; i++)
    {
        stars[i].x = (float)((rand() % 200) - 100);
        stars[i].y = (float)((rand() % 200) - 100);
        stars[i].z = (float)((rand() % 100) + 1);
    }
}

void starfield_render(uint32_t frame)
{
    (void)frame;

    if (!initialized)
    {
        init_stars();
        initialized = 1;
    }

    framebuffer_clear(0x000000);

    for (int i = 0; i < STAR_COUNT; i++)
    {
        Star *star = &stars[i];

        star->z -= 0.5f;

        if (star->z <= 1.0f)
        {
            star->x = (float)((rand() % 200) - 100);
            star->y = (float)((rand() % 200) - 100);
            star->z = 100.0f;
        }

        int x = (int)((WIDTH / 2.0f) + ((star->x / star->z) * 100.0f));
        int y = (int)((HEIGHT / 2.0f) + ((star->y / star->z) * 100.0f));

        if (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT)
        {
            int brightness = 255 - (int)(star->z * 2.0f);

            if (brightness < 40)
            {
                brightness = 40;
            }

            uint32_t color = (brightness << 16) | (brightness << 8) | brightness;

            put_pixel(x, y, color);
        }
    }
}