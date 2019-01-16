#include "aiv_math.h"

vector3_t new_vector3(float x, float y, float z)
{
    vector3_t new_vector3;
    new_vector3.x = x;
    new_vector3.y = y;
    new_vector3.z = z;
    return new_vector3;
}

vector3_t vector3_zero()
{
    vector3_t vector3_zero;
    vector3_zero.x = 0;
    vector3_zero.y = 0;
    vector3_zero.z = 0;
    return vector3_zero;
}

float lerp(float start, float end, float gradient)
{
    return start + (end - start) * gradient;
}

float inverted_slope(float x0, float y0, float x1, float y1)
{
    return (x1 - x0) / (y1 - y0);
}

//distance between two points
float get_magnitude(vector3_t a, vector3_t b)
{
    return sqrt(pow(b.x - a.x, 2) + pow(b.y - a.y, 2));
}

float get_gradient(float i, float P0, float P1)
{
    float gradient;
    if (P0 != P1)
        return gradient = (i - P0) / (P1 - P0);

    return P0 || P1;
}

float dot_product(vector3_t a, vector3_t b)
{
    return (a.x * b.x) + (a.y * b.y);
}