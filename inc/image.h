#ifndef PAINT_INC_IMAGE_H_
#define PAINT_INC_IMAGE_H_

#include <deque>
#include <memory>

#include "file.h"
#include "image_data.h"

namespace paint
{

    class Image
    {
    public:
        virtual void LoadImage() = 0;
        virtual void SaveImage() = 0;
        virtual void GenerateMetadata() = 0;

    protected:
        // std::unique_ptr<ImageData<T>> image_data_;
        // std::deque<std::unique_ptr<ImageData<T>>> image_data_history_;

    private:
        File file_;
    };
}
#endif // PAINT_INC_IMAGE_H_