#include "aiv_rasterizer.h"
#include <stdlib.h>
#include <stdio.h>

static void put_pixel(context_t *ctx, int x, int y)
{
    if (x < 0 || y < 0 || x >= ctx->width || y >= ctx->height)
        return;
    int offset = ((y * ctx->width) * 4) + x * 4;
    ctx->framebuffer[offset++] = 255;
    ctx->framebuffer[offset++] = 0;
    ctx->framebuffer[offset++] = 0;
    ctx->framebuffer[offset] = 255;
}

void rasterizer(context_t *ctx)
{
    for (int i = 0; i < ctx->array_size; i++)
    {
        triangle_t *triangle = &ctx->triangles[i];
        vector3_t vertex_a = pixel_point_to_screen_point(triangle->a.position, *ctx);
        vector3_t vertex_b = pixel_point_to_screen_point(triangle->b.position, *ctx);
        vector3_t vertex_c = pixel_point_to_screen_point(triangle->c.position, *ctx);

        //order from low y to high y
        vector3_t P[3] = {vertex_a, vertex_b, vertex_c};

        vector3_t p_temp;
        if (P[1].y < P[0].y)
        {
            p_temp = P[0];
            P[0] = P[1];
            P[1] = p_temp;
        }

        if (P[2].y < P[1].y)
        {
            p_temp = P[2];
            P[2] = P[1];
            P[1] = p_temp;
        }

        if (P[1].y < P[0].y)
        {
            p_temp = P[0];
            P[0] = P[1];
            P[1] = p_temp;
        }
        

        if (P[1].y == P[2].y)
        {
            for (float k = P[0].y; k < P[2].y; k++)
            {
                float gradient_P0_P2 = get_gradient(k, P[0].y, P[2].y);
                float start_x = lerp(P[0].x, P[1].x, gradient_P0_P2);
                float end_x = lerp(P[0].x, P[2].x, gradient_P0_P2);
                for (float u = start_x; u <= end_x; u++)
                {
                    put_pixel(ctx, u, k);
                }
            }
            return;
        }
        else if (P[0].y == P[1].y)
        {
            for (float k = P[0].y; k < P[2].y; k++)
            {
                float gradient_P0_P2 = get_gradient(k, P[0].y, P[2].y);
                float start_x = lerp(P[0].x, P[2].x, gradient_P0_P2);
                float end_x = lerp(P[1].x, P[2].x, gradient_P0_P2);
                for (float u = start_x; u <= end_x; u++)
                {
                    put_pixel(ctx, u, k);
                }
            }
            return;
        }

        //Prima metà di sopra
        for (float i = P[0].y; i < P[1].y; i++)
        {
            float gradient_P0_P1 = get_gradient(i, P[0].y, P[1].y);
            float start_x = lerp(P[0].x, P[1].x, gradient_P0_P1);

            float gradient_P0_P2 = get_gradient(i, P[0].y, P[2].y);
            float end_x = lerp(P[0].x, P[2].x, gradient_P0_P2);

            for (float j = start_x; j <= end_x; j++)
            {
                put_pixel(ctx, j, i);
            }
        }

        //seconda metà
        for (float i = P[1].y; i < P[2].y; i++)
        {
            float gradient_P1_P2 = get_gradient(i, P[1].y, P[2].y);
            float start_x = lerp(P[1].x, P[2].x, gradient_P1_P2);

            float gradient_P0_P2 = get_gradient(i, P[0].y, P[2].y);
            float end_x = lerp(P[0].x, P[2].x, gradient_P0_P2);

            for (float j = start_x; j <= end_x; j++)
            {
                put_pixel(ctx, j, i);
            }
        }
    }
}

int append_triangle(context_t *ctx, triangle_t new_triangle)
{
    ctx->array_size++;
    void *resized_area = realloc(ctx->triangles, sizeof(triangle_t) * ctx->array_size);
    if (!resized_area)
    {
        ctx->array_size--;
        return -1;
    }

    ctx->triangles = (triangle_t *)resized_area;
    ctx->triangles[ctx->array_size - 1] = new_triangle;
    return 0;
}

context_t init_ctx(int width, int height)
{
    context_t ctx;
    ctx.width = width;
    ctx.height = height;
    ctx.framebuffer = NULL;
    ctx.array_size = 0;
    ctx.triangles = NULL;
    return ctx;
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

void draw_line(float start_y, float start_x, float end_x, float end_y, context_t *ctx)
{
    for (float i = start_y; i < end_y; i++)
    {
        float gradient_pixel_p0_p1 = get_gradient(i, end_y, start_y);

        float lerp_value = lerp(end_x, start_x, gradient_pixel_p0_p1);
        vector3_t pixel;
        pixel.y = i;
        pixel.x = lerp_value;

        put_pixel(ctx, pixel.x, pixel.y);
    }
}
