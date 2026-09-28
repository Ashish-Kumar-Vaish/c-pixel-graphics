#ifndef EFFECT_REGISTRY_H
#define EFFECT_REGISTRY_H

#include <stdint.h>

typedef void (*RenderFn)(uint32_t frame);

typedef struct
{
    const char *name;
    RenderFn render;
} Effect;

extern const Effect g_effects[];
extern const int g_effect_count;

#endif
