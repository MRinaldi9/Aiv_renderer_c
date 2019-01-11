#include "aiv_math.h"
#include <string.h>

typedef struct context
{
    int width, height;

    unsigned char *framebuffer;
} context_t;

typedef struct vertex
{
    vector3_t position;
    vector3_t normal;
    vector3_t color;

    int raster_x;
    int raster_y;
} vertex_t;

typedef struct triangle
{
    vertex_t a;
    vertex_t b;
    vertex_t c;
} triangle_t;

void rasterizer(context_t *ctx, triangle_t *triangle);
vertex_t vertex_new(vector3_t position);
triangle_t triangle_new(vertex_t a,vertex_t b,vertex_t c);
static void put_pixel(context_t *ctx, int x, int y);
vector3_t screen_point_to_pixel_point(vector3_t screen_point,context_t screen_size);
vector3_t pixel_point_to_screen_point(vector3_t pixel_point,context_t screen_size);