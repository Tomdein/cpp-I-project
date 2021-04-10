#ifndef PAINT_INC_POINT_H_
#define PAINT_INC_POINT_H_

//#include "image_data.h"
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

        virtual Point GetPointPX(const Point &dimensions) = 0;
    };

    class PointPX : public BasePoint
    {
    public:
        PointPX(uint32_t x, uint32_t y) : point_{x, y} {};

        virtual Point GetPointPX(const Point &dimensions) override;

    private:
        Point point_;
    };

    class PointPer : public BasePoint
    {
    public:
        PointPer(float x, float y) : x_(x), y_(y){};

        virtual Point GetPointPX(const Point &dimensions) override;

    private:
        float x_;
        float y_;
    };
}
#endif // PAINT_INC_POINT_H_