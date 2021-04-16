#ifndef PAINT_INC_POINT_H_
#define PAINT_INC_POINT_H_

#include <cmath>

#include "unit.h"

namespace paint
{
    struct Point
    {
        Unit x = 0;
        Unit y = 0;
    };

    class BasePoint
    {
    public:
        BasePoint() = default;
        virtual ~BasePoint(){};

        virtual Point GetPointPX(const Point &dimensions) const = 0;
    };

    class PointPX : public BasePoint
    {
    public:
        PointPX(int32_t x, int32_t y) : point_{x, y} {};
        virtual ~PointPX(){};

        virtual Point GetPointPX([[maybe_unused]] const Point &dimensions) const override { return point_; };

    private:
        Point point_;
    };

    class PointPer : public BasePoint
    {
    public:
        PointPer(float x, float y) : x_(x), y_(y){};
        virtual ~PointPer(){};

        virtual Point GetPointPX(const Point &dimensions) const override
        {
            return Point{static_cast<Unit>(std::roundf(dimensions.x * x_)),
                         static_cast<Unit>(roundf(dimensions.y * y_))};
        };

    private:
        float x_;
        float y_;
    };
}
#endif // PAINT_INC_POINT_H_