#include "aiv_math.h"
#include <string.h>

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

typedef struct context
{
    int width, height;

    unsigned char *framebuffer;
    triangle_t *triangles;
    size_t array_size;

} context_t;

void rasterizer(context_t *ctx);
vertex_t vertex_new(vector3_t position);
triangle_t triangle_new(vertex_t a, vertex_t b, vertex_t c);
static void put_pixel(context_t *ctx, int x, int y);
vector3_t pixel_point_to_screen_point(vector3_t screen_point, context_t screen_size);
vector3_t screen_point_to_pixel_point(vector3_t pixel_point, context_t screen_size);
void draw_line(float start_y, float start_x,float end_x, float end_y, context_t* ctx);
int append_triangle(context_t *ctx, triangle_t new_triangle);
context_t init_ctx(int width,int height);