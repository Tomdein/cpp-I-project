#ifndef PAINT_INC_ROTATION_H_
#define PAINT_INC_ROTATION_H_

namespace paint
{
    /**
     * @brief Enum class signifying the rotation direction.
     * 
     */
    enum class Rotation
    {
        kClock,        /// Clockwise rotation.
        kCounterClock, /// Counter clockwise rotation.
    };
}

#endif // PAINT_INC_ROTATION_H_