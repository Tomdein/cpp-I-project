#ifndef PAINT_INC_DATA_PIXEL_H
#define PAINT_INC_DATA_PIXEL_H

#include <exception>
#include <cstring>

#include "unit.h"
#include "point.h"

namespace paint
{
    /**
     * @brief A exception class
     * 
     * \ref error_data_mismatch is thrown when trying to copy from different type of DataPixels (different pixel sizes or pixel count)
     * 
     */
    class error_data_mismatch : public std::exception
    {
    public:
        error_data_mismatch() = default;
        virtual ~error_data_mismatch() {}

        virtual const char *what() const noexcept override { return "Copying data with different underlying structure"; }
    };

    /**
     * @brief A class used for storing and access of pixel data
     * 
     * This class is used for storing and access of pixel data without any color information, just the size of a single pixel.
     * 
     * DataPixels gives the option to access the underlying pixels with DataPixels::operator[] or DataPixels::at().
     * DataPixels::at() performs bounds checking and throws std::out_of_range if arguments are out of bounds.
     * 
     * DataPixel also implements DataPixels::iterator used in range-based for loop
     *  
     */
    class DataPixels
    {
    public:
        DataPixels(Point image_size, std::unique_ptr<Color> &&color_type) : pixel_struct_size_byte_(color_type->GetDataSize()),
                                                                            image_size_(image_size),
                                                                            pixel_count_(image_size.x * image_size.y),
                                                                            data_color_(std::move(color_type))
        {
            data_ = new uint8_t[pixel_count_ * pixel_struct_size_byte_];
        }

        DataPixels(const DataPixels &other) : pixel_struct_size_byte_(other.data_color_->GetDataSize()),
                                              image_size_(other.image_size_),
                                              pixel_count_(other.pixel_count_),
                                              data_color_(std::unique_ptr<Color>(other.data_color_->clone()))
        {
            size_t byte_count = pixel_count_ * pixel_struct_size_byte_;
            data_ = new uint8_t[byte_count];
            mempcpy(this->data_, other.data_, byte_count);
        }

        ~DataPixels()
        {
            delete static_cast<uint8_t *>(data_);
        }

        // The data in iterator is not dereferencable. It is just a 'void *'
        class iterator
        {
        public:
            iterator(uint8_t *ptr, size_t pixel_struct_size_byte) : ptr_(ptr), pixel_struct_size_byte_(pixel_struct_size_byte) {}

            void *operator*() { return ptr_; }

            iterator &operator++(int)
            {
                ptr_ += pixel_struct_size_byte_;
                return *this;
            }
            bool operator==(const iterator &other) { return ptr_ == other.ptr_; }
            bool operator!=(const iterator &other) { return !(*this == other); }

        private:
            uint8_t *ptr_;
            size_t pixel_struct_size_byte_;
        };

        void *operator[](size_t pos) { return &data_[pos]; }
        void *at(Unit x, Unit y)
        {
            if (x >= image_size_.x || y >= image_size_.y)
            {
                throw std::out_of_range("Accesssing data pixels out of range.");
            }
            return &data_[x * image_size_.x + y];
        }

        iterator begin() { return iterator(data_, pixel_struct_size_byte_); }
        iterator end() { return iterator(data_ + pixel_count_, pixel_struct_size_byte_); }

        iterator GetIterator() const { return iterator(data_, pixel_struct_size_byte_); }

        void CopyData(const DataPixels &other)
        {
            if ((pixel_struct_size_byte_ != other.pixel_struct_size_byte_) || (pixel_count_ != other.pixel_count_))
                throw error_data_mismatch();

            iterator this_it = this->GetIterator();
            iterator other_it = other.GetIterator();

            for (; this_it != this->end(); this_it++)
            {
                std::memcpy(*this_it, *other_it, pixel_struct_size_byte_);
            }
        }

    private:
        size_t pixel_struct_size_byte_;
        Point image_size_;
        size_t pixel_count_;
        std::unique_ptr<Color> data_color_;
        uint8_t *data_;

        friend class Painter;
    };
}

#endif // PAINT_INC_DATA_PIXEL_H