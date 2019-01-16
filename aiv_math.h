#include <math.h>

typedef struct vector3
{
    float x, y, z;
} vector3_t;

vector3_t new_vector3(float x, float y, float z);
vector3_t vector3_zero();
float lerp(float start, float end, float gradient);
float inverted_slope(float x0, float y0, float x1, float y1);
float get_magnitude(vector3_t a, vector3_t b);
float get_gradient(float i, float P0, float P1);
float dot_product(vector3_t a, vector3_t b);