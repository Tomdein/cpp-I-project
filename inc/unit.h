#ifndef PAINT_INC_UNIT_H_
#define PAINT_INC_UNIT_H_

#include <cstdint>

#include "image_data.h"
#include "point.h"

namespace paint
{

    using Unit = uint32_t;
    struct Point;

    class BaseUnit
    {
    public:
        BaseUnit() = default;

        virtual Unit GetPX(const Point &dimensions, uint32_t dpi) = 0;
    };

    class UnitPX : public BaseUnit
    {
    public:
        UnitPX(uint32_t x) : x_(x){};

        virtual Unit GetPX(const Point &dimensions, uint32_t dpi) override;

    private:
        Unit x_;
    };

    class UnitPer : public BaseUnit
    {
    public:
        UnitPer(float x) : x_(x){};

        virtual Unit GetPX(const Point &dimensions, uint32_t dpi) override;

    private:
        float x_;
    };
}
#endif // PAINT_INC_UNIT_H_