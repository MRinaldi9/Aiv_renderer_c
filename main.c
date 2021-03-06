#include "aiv_renderer.h"
#include <stdlib.h>
#include <stdio.h>
#define SDL_MAIN_HANDLED
#include "C:\Users\marco\Desktop\SDL2-2.0.9\include\SDL.h"

#define triangle(x0, y0, z0, x1, y1, z1, x2, y2, z2) \
    triangle_new(                                    \
        vertex_new(new_vector3(x0, y0, z0)),         \
        vertex_new(new_vector3(x1, y1, z1)),         \
        vertex_new(new_vector3(x2, y2, z2)))

int main(int argc, char const *argv[])
{

    context_t ctx = init_ctx(600, 600);
    triangle_t triangle = triangle(0, 0.5, 0, 1, 0.8, 0, -0.5, 0, 0);
    if (append_triangle(&ctx, triangle) == -1)
    {
        printf("1");
    }
    //don't work with values of 0.6 in sù, draw only 2 triangles even if there are more triangles to write
    triangle = triangle(-1, 0.5, 0, 0.8, -0.5, 0, 0, 0.5, 0);
    if (append_triangle(&ctx, triangle) == -1)
    {
        printf("2");
    }
    // triangle = triangle(-0.9, 0.9, 0, 0, 0.9, 0, -0.9, 0, 0);
    // if (append_triangle(&ctx, triangle) == -1)
    // {
    //     printf("3");
    // }
    triangle = triangle(-0.9, 0, 0, 0, -0.9, 0, 0.9, -0.9, 0);
    if (append_triangle(&ctx, triangle) == -1)
    {
        printf("4");
    }
    printf("ctx resize: %zu",ctx.array_size);
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window *window = SDL_CreateWindow("Title here", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 600, 600, 0);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_Texture *texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, 600, 600);
    for (;;)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
                return 0;
        }
        int pitch;
        SDL_LockTexture(texture, NULL, (void **)&ctx.framebuffer, &pitch);
        rasterizer(&ctx);
        SDL_UnlockTexture(texture);
        SDL_RenderCopy(renderer, texture, NULL, NULL);
        SDL_RenderPresent(renderer);
    }
    return 0;
}
