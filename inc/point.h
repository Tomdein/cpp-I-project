#ifndef PAINT_INC_POINT_H_
#define PAINT_INC_POINT_H_

#include <cmath>

#include "unit.h"

namespace paint
{
    /**
     * @brief A basic point structure used to store 2D coordinates.
     * 
     */
    struct Point
    {
        Unit x = 0;
        Unit y = 0;
    };

    /**
     * @brief Interface for different types of points (different unit type).
     * 
     * Declares BasePoint::GetPointPX() that can be used to retrieve poition of the point in image.
     * 
     */
    class BasePoint
    {
    public:
        BasePoint() = default;
        virtual ~BasePoint(){};

        /**
         * @brief GetPointPX returns the point in image.
         * 
         * @param dimensions size of image.
         * @return Point point in image.
         */
        virtual Point GetPointPX(const Point &dimensions) const = 0;
    };

    /**
     * @brief Class representing a point in absolute units of pixel.
     * 
     */
    class PointPX : public BasePoint
    {
    public:
        PointPX(int32_t x, int32_t y) : point_{x, y} {};
        virtual ~PointPX(){};

        /**
         * @brief GetPointPX returns the point in image.
         * 
         * @param dimensions size of image.
         * @return Point point in image.
         */
        virtual Point GetPointPX([[maybe_unused]] const Point &dimensions) const override { return point_; };

    private:
        Point point_; /// Absolute position in pixels.
    };

    /**
     * @brief Class representing a point in relative % (percent) units.
     * 
     * This class stores the point position as percents, where 1.0f represents 100% i.e.: the edge of picture.
     * Any number above 1.0f means a point outside of image.
     * 
     */
    class PointPer : public BasePoint
    {
    public:
        PointPer(float x, float y) : x_(x), y_(y){};
        virtual ~PointPer(){};

        /**
         * @brief GetPointPX returns the point in image.
         * 
         * @param dimensions size of image.
         * @return Point point in image.
         */
        virtual Point GetPointPX(const Point &dimensions) const override
        {
            return Point{static_cast<Unit>(std::roundf(dimensions.x * x_)),
                         static_cast<Unit>(roundf(dimensions.y * y_))};
        };

    private:
        float x_; /// Relative x position.
        float y_; /// Relative y position.
    };
}
#endif // PAINT_INC_POINT_H_