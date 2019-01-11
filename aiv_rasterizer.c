#include "aiv_rasterizer.h"
#include <stdio.h>

static void put_pixel(context_t *ctx, int x, int y)
{
    if (x < 0 || y < 0 || x >= ctx->width || y >= ctx->height)
        return;
    int offset = ((y * ctx->width) + x) * 4;
    ctx->framebuffer[offset++] = 255;
    ctx->framebuffer[offset++] = 0;
    ctx->framebuffer[offset++] = 0;
    ctx->framebuffer[offset] = 255;
}

void rasterizer(context_t *ctx, triangle_t *triangle)
{
    vertex_t *ptr = (vertex_t *)triangle;

    // for (int i = 0; i < 3; i++)
    // {
    //     vector3_t return_value = pixel_point_to_screen_point(ptr[i].position, *ctx);
    //     put_pixel(ctx, return_value.x, return_value.y);
    // }

    vertex_t ptr_temp;
    if (ptr[1].position.y > ptr[0].position.y)
    {
        ptr_temp = ptr[0];
        ptr[0] = ptr[1];
        ptr[1] = ptr_temp;
    }

    if (ptr[2].position.y > ptr[1].position.y)
    {
        ptr_temp = ptr[1];
        ptr[1] = ptr[2];
        ptr[2] = ptr_temp;
    }

    if (ptr[1].position.y > ptr[0].position.y)
    {
        ptr_temp = ptr[0];
        ptr[0] = ptr[1];
        ptr[1] = ptr_temp;
    }

    if (inverted_slope(ptr[0].position.x, ptr[0].position.y, ptr[1].position.x, ptr[1].position.y) < inverted_slope(ptr[0].position.x, ptr[0].position.y, ptr[2].position.x, ptr[2].position.y))
    {
        //printf("right");
    }
    else
    {
        for (float i = ptr[0].position.y; i > ptr[1].position.y; i++)
        {
            float temp;
            temp = (1 - i) / (ptr[1].position.y - i);
            printf("%f\n", temp);
            float x0 = lerp(ptr[0].position.x,ptr[1].position.x,temp);
            printf("%f\n", x0);
            float x1 = lerp(ptr[0].position.x, ptr[2].position.x, temp);
            printf("%f\n\n", x1);

            for (float j = x0; j > x1; j++)
            {
                put_pixel(ctx, lerp(x0, x1, j), i);
            }
        }

        //printf("left");
    }
}

vertex_t vertex_new(vector3_t position)
{
    vertex_t vertex;
    memset(&vertex, 0, sizeof(vertex_t));
    vertex.position = position;
    return vertex;
}

triangle_t triangle_new(vertex_t a, vertex_t b, vertex_t c)
{
    triangle_t triangle = {.a = a,
                           .b = b,
                           .c = c};
    return triangle;
}

vector3_t screen_point_to_pixel_point(vector3_t screen_point, context_t screen_size)
{
    vector3_t out_screen_point;
    out_screen_point.x = (screen_point.x / (screen_size.width / 2)) - 1;
    out_screen_point.y = (screen_point.y / (screen_size.height / 2)) - 1;
    out_screen_point.z = 0;

    if (out_screen_point.x > 1)
        out_screen_point.x = 1;
    else if (out_screen_point.x < -1)
        out_screen_point.x = -1;

    if (out_screen_point.y > 1)
        out_screen_point.y = 1;
    else if (out_screen_point.y < -1)
        out_screen_point.y = -1;

    return out_screen_point;
}

vector3_t pixel_point_to_screen_point(vector3_t pixel_point, context_t screen_size)
{
    vector3_t out_pixel_point;
    out_pixel_point.x = ((pixel_point.x + 1) / 2) * screen_size.width;
    out_pixel_point.y = screen_size.height - (((pixel_point.y + 1) / 2) * screen_size.height);
    return out_pixel_point;
}