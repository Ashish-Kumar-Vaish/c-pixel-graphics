#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include <SDL3/SDL.h>

#include "framebuffer.h"
#include "effect_registry.h"

static RenderFn pick_effect(int argc, char **argv)
{
    if (g_effect_count == 0)
    {
        fprintf(stderr, "No effects found in effects/\n");

        exit(EXIT_FAILURE);
    }

    const char *name = (argc >= 2) ? argv[1] : NULL;

    if (name == NULL)
    {
        printf("No effect given (pass one as an argument).\n");
        printf("Defaulting to \"%s\".\n", g_effects[0].name);
        printf("Available effects:\n");

        for (int i = 0; i < g_effect_count; i++)
        {
            printf(" - %s\n", g_effects[i].name);
        }

        printf("\n");

        return g_effects[0].render;
    }

    for (int i = 0; i < g_effect_count; i++)
    {
        if (strcmp(name, g_effects[i].name) == 0)
        {
            return g_effects[i].render;
        }
    }

    fprintf(stderr, "Unknown effect \"%s\". Available effects:\n", name);

    for (int i = 0; i < g_effect_count; i++)
    {
        fprintf(stderr, " - %s\n", g_effects[i].name);
    }

    exit(EXIT_FAILURE);
}

int main(int argc, char **argv)
{
    RenderFn render = pick_effect(argc, argv);

    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    SDL_Texture *texture = NULL;
    SDL_Event event;

    const double target_frame = 1.0 / 60.0;

    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        fprintf(stderr, "SDL_Init failed: %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }

    window = SDL_CreateWindow("C Pixel Graphics", WIDTH * 4, HEIGHT * 4, 0);

    if (window == NULL)
    {
        fprintf(stderr, "SDL_CreateWindow failed: %s\n", SDL_GetError());
        SDL_Quit();
        return EXIT_FAILURE;
    }

    renderer = SDL_CreateRenderer(window, NULL);

    if (renderer == NULL)
    {
        fprintf(stderr, "SDL_CreateRenderer failed: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return EXIT_FAILURE;
    }

    texture = SDL_CreateTexture(renderer,
                                SDL_PIXELFORMAT_XRGB8888,
                                SDL_TEXTUREACCESS_STREAMING,
                                WIDTH,
                                HEIGHT);

    if (texture == NULL)
    {
        fprintf(stderr, "SDL_CreateTexture failed: %s\n", SDL_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return EXIT_FAILURE;
    }

    SDL_SetTextureScaleMode(texture, SDL_SCALEMODE_NEAREST);

    uint8_t isRunning = 1;
    uint32_t frame = 0;

    while (isRunning)
    {
        uint64_t start = SDL_GetPerformanceCounter();

        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_EVENT_QUIT)
            {
                isRunning = 0;
            }
        }

        render(frame);

        SDL_UpdateTexture(texture, NULL, frameBuffer, WIDTH * sizeof(uint32_t));
        SDL_RenderClear(renderer);
        SDL_RenderTexture(renderer, texture, NULL, NULL);
        SDL_RenderPresent(renderer);

        uint64_t end = SDL_GetPerformanceCounter();

        double elapsed = (double)(end - start) / (double)SDL_GetPerformanceFrequency();

        if (elapsed < target_frame)
        {
            SDL_Delay((uint32_t)((target_frame - elapsed) * 1000.0));
        }

        frame++;
    }

    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return EXIT_SUCCESS;
}