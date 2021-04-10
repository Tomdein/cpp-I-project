#ifndef PAINT_INC_DATA_PIXEL_H
#define PAINT_INC_DATA_PIXEL_H

#include <exception>
#include <cstring>

#include "unit.h"

namespace paint
{
    class error_data_mismatch : public std::exception
    {
    public:
        error_data_mismatch() = default;
        virtual ~error_data_mismatch() {}

        virtual const char *what() const noexcept override { return "Copying data with different underlying structure"; }
    };

    class DataPixel
    {
    public:
        DataPixel(Point size, size_t pixel_struct_size_byte) : pixel_struct_size_byte_(pixel_struct_size_byte)
        {
            size_ = size.x * size.y;
            data_ = new uint8_t[size_ * pixel_struct_size_byte_];
        }
        ~DataPixel() { delete static_cast<uint8_t *>(data_); }

        // The data iterator is not dereferencable. It is just a 'void *'
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

        iterator begin() { return iterator(data_, pixel_struct_size_byte_); };
        iterator end() { return iterator(&data_[size_], pixel_struct_size_byte_); };

        iterator GetIterator() const { return iterator(data_, pixel_struct_size_byte_); }

        void CopyData(const DataPixel &other)
        {
            if ((pixel_struct_size_byte_ != other.pixel_struct_size_byte_) || (size_ != other.size_))
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
        size_t size_;
        uint8_t *data_;
    };
}

#endif // PAINT_INC_DATA_PIXEL_H