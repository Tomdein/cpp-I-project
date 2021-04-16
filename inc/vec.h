#ifndef PAINT_INC_VEC_H_
#define PAINT_INC_VEC_H_

#include <cmath>

#include "unit.h"

namespace paint
{
    struct vec2
    {
        Unit u;
        Unit v;
    };

    struct vec2f
    {
        float u;
        float v;
    };

    inline float norm(const vec2 &v)
    {
        return std::hypot(v.u, v.v);
    }

    inline float norm(const vec2f &v)
    {
        return std::hypot(v.u, v.v);
    }

    /**
     * @brief A struct representing a line equation: ax + by + c = 0
     * 
     */
    struct line
    {
        float a;
        float b;
        float c;
    };

    inline vec2f line_intersection(const line &l1, const line &l2)
    {
        vec2f vec;

        if (l1.a == l2.a && l1.b == l2.b)
        {
            vec.u = -1;
            vec.v = -1;
            return vec;
        }

        vec.u = (l1.b * l2.c - l2.b * l1.c) / (l1.a * l2.b - l2.a * l1.b);
        vec.v = (l1.c * l2.a - l2.c * l1.a) / (l1.a * l2.b - l2.a * l1.b);
        return vec;
    }

    inline float line_get_y(const line &l, const float &x)
    {
        return -(l.a * x + l.c) / l.b;
    }

    inline float line_get_x(const line &l, const float &y)
    {
        return -(l.b * y + l.c) / l.a;
    }
}

#endif // PAINT_INC_VEC_H_