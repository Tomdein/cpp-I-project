#ifndef PAINT_INC_DATA_PIXEL_H
#define PAINT_INC_DATA_PIXEL_H

#include <exception>
#include <memory>
#include <cstring>

#include "unit.h"
#include "point.h"
#include "color.h"

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
        virtual ~error_data_mismatch() override {}

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
                                                                            data_color_(std::move(color_type)),
                                                                            data_(std::unique_ptr<uint8_t[]>(new uint8_t[pixel_count_ * pixel_struct_size_byte_]))
        {
        }

        DataPixels(const DataPixels &other) : pixel_struct_size_byte_(other.data_color_->GetDataSize()),
                                              image_size_(other.image_size_),
                                              pixel_count_(other.pixel_count_),
                                              data_color_(std::unique_ptr<Color>(other.data_color_->clone()))
        {
            size_t byte_count = pixel_count_ * pixel_struct_size_byte_;
            data_ = std::unique_ptr<uint8_t[]>(new uint8_t[byte_count]);
            mempcpy(data_.get(), other.data_.get(), byte_count);
        }

        ~DataPixels(){};

        // The data in iterator is not dereferencable. It is just a 'void *'
        class iterator
        {
        public:
            iterator(uint8_t *ptr, size_t pixel_struct_size_byte) : ptr_(ptr), pixel_struct_size_byte_(pixel_struct_size_byte) {}
            iterator(const iterator &other) : ptr_(other.ptr_), pixel_struct_size_byte_(other.pixel_struct_size_byte_) {}

            void *operator*() { return ptr_; }

            // Pre-increment
            iterator &operator++()
            {
                ptr_ += pixel_struct_size_byte_;
                return *this;
            }

            // Post-increment
            iterator operator++(int)
            {
                iterator copy(*this);
                ++(*this); // ptr_ += pixel_struct_size_byte_;
                return copy;
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

        iterator begin() { return iterator(data_.get(), pixel_struct_size_byte_); }
        iterator end() { return iterator(data_.get() + pixel_count_, pixel_struct_size_byte_); }

        void CopyData(DataPixels &other)
        {
            if ((pixel_struct_size_byte_ != other.pixel_struct_size_byte_) || (pixel_count_ != other.pixel_count_))
                throw error_data_mismatch();

            // std::copy(other.begin(), other.end(), this->begin()); // Iterator needs to be derived from RandomAccessIterator
            std::copy_n(reinterpret_cast<uint8_t *>(other.data_.get()), pixel_struct_size_byte_ * pixel_count_, reinterpret_cast<uint8_t *>(data_.get()));

            // iterator this_it = this->begin();
            // iterator other_it = other.begin();

            // //remove loop!!!
            // for (; this_it != this->end(); this_it++)
            // {
            //     //std::memcpy(*this_it, *other_it, pixel_struct_size_byte_);
            // }
        }

        std::unique_ptr<Color> GetColorType() { return std::unique_ptr<Color>(data_color_->clone()); }

        void TransformToColorType(const std::unique_ptr<Color> &new_color)
        {

            std::unique_ptr<uint8_t[]> new_data(new uint8_t[pixel_count_]);
            iterator it_old = begin();
            iterator it_old_end = end();

            size_t new_pixel_size = new_color->GetDataSize();
            iterator it_new(new_data.get(), new_pixel_size);

            // Saves the pointer to the color data that will change inside for loop
            // There is no need to call Color::GetData() each loop,
            //      because the address does not change, only data inside Color changes
            void *old_color_data_ptr = data_color_->GetData();
            void *new_color_data_ptr = new_color->GetData();

            for (; it_old != it_old_end; it_old++, it_new++)
            {
                // memcpy(old_color_data_ptr, *it_old, pixel_struct_size_byte_);

                std::copy_n(reinterpret_cast<uint8_t *>(*it_old), pixel_struct_size_byte_, reinterpret_cast<uint8_t *>(old_color_data_ptr));
                new_color->SetColor(*data_color_);
                std::copy_n(reinterpret_cast<uint8_t *>(new_color_data_ptr), new_pixel_size, reinterpret_cast<uint8_t *>(*it_new));

                // memcpy(*it_new, new_color_data_ptr, new_pixel_size);
            }

            pixel_struct_size_byte_ = new_color->GetDataSize();
            data_color_ = std::unique_ptr<Color>(new_color->clone());
            data_.swap(new_data);
        }

        Point GetSize() { return image_size_; }

        void SwapData(DataPixels &other)
        {
            std::swap(pixel_struct_size_byte_, other.pixel_struct_size_byte_);
            std::swap(image_size_, other.image_size_);
            std::swap(pixel_count_, other.pixel_count_);
            std::swap(data_color_, other.data_color_);
            std::swap(data_, other.data_);
        }

    private:
        size_t pixel_struct_size_byte_;
        Point image_size_;
        size_t pixel_count_;
        std::unique_ptr<Color> data_color_;
        std::unique_ptr<uint8_t[]> data_;

        friend class Painter;
    };
}

#endif // PAINT_INC_DATA_PIXEL_H