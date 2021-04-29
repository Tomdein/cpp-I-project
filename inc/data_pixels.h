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
     * @brief A class used for storing and access of the pixel data
     * 
     * This class is used for storing and access of the pixel data without any color information, just the size of a single pixel.
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
            std::copy_n(reinterpret_cast<uint8_t *>(other.data_.get()), byte_count, reinterpret_cast<uint8_t *>(data_.get()));
        }

        ~DataPixels(){};

        // The data in iterator is not dereferencable. It is just a 'void *'
        /**
         * @brief A DataPixels iterator.
         * 
         */
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
            uint8_t *ptr_;                  /// Pointer to the current position in pixel data.
            size_t pixel_struct_size_byte_; /// Size of a single pixel in bytes.
        };

        /**
         * @brief A random access operator for pixels in DataPixels.
         * 
         * @param pos the position of pixels (represented as linear array of pixels).
         * @return void* pointer to the pixel at \ref pos.
         */
        void *operator[](size_t pos)
        {
            return &data_[pos * pixel_struct_size_byte_];
        }
        /**
         * @brief A random access method with position as coordinates.
         * 
         * This method does a bounds check and throws if coordinates are out of bounds.
         * 
         * @param x the x coordinate of pixel.
         * @param y the y coordinate of pixel.
         * @return void* pointer to the pixel at (x, y).
         */
        void *at(Unit x, Unit y)
        {
            if (x >= image_size_.x || y >= image_size_.y)
            {
                throw std::out_of_range("Accesssing data pixels out of range.");
            }
            return &data_[(y * image_size_.x + x) * pixel_struct_size_byte_];
        }

        /**
         * @brief Returns the iterator to the beginning of the DataPixels.
         * 
         * @return iterator to the beginning of the DataPixels.
         */
        iterator begin() { return iterator(data_.get(), pixel_struct_size_byte_); }
        /**
         * @brief Returns the iterator to the one pixel after the end of the DataPixels.
         * 
         * @return iterator to the one after the last pixel of the DataPixels.
         */
        iterator end() { return iterator(data_.get() + pixel_count_ * pixel_struct_size_byte_, pixel_struct_size_byte_); }

        /**
         * @brief Coppies data from another DataPixels.
         * 
         * If \ref other does not have the same size of the data or different size of pixel
         * then this method throws \ref error_data_mismatch.
         * 
         * @param other where to copy the data from.
         */
        void CopyData(DataPixels &other)
        {
            // Other does not have the same size of the data or different size of pixel -> throw
            if ((pixel_struct_size_byte_ != other.pixel_struct_size_byte_) || (pixel_count_ != other.pixel_count_))
                throw error_data_mismatch();

            // Copy all the data
            std::copy_n(reinterpret_cast<uint8_t *>(other.data_.get()), pixel_struct_size_byte_ * pixel_count_, reinterpret_cast<uint8_t *>(data_.get()));
        }

        /**
         * @brief Get the Color.
         * 
         * Returns a clone of Color instance associated with this DataPixels.
         * 
         * @return std::unique_ptr<Color> a clone of Color instance.
         */
        std::unique_ptr<Color> GetColorType() { return std::unique_ptr<Color>(data_color_->clone()); }

        /**
         * @brief Transforms DataPixels to new color type.
         * 
         * Allocates space for new pixel data and then transforms each pixel to the new color.
         * 
         * @param new_color \ref Color to tranform to.
         */
        void TransformToColorType(const std::unique_ptr<Color> &new_color)
        {
            size_t new_pixel_size = new_color->GetDataSize();

            // Allocate space for pixel data
            std::unique_ptr<uint8_t[]> new_data(new uint8_t[pixel_count_ * new_pixel_size]);

            // Create iterators to old pixel data
            iterator it_old = begin();
            iterator it_old_end = end();

            // Create iterator to new pixel data
            iterator it_new(new_data.get(), new_pixel_size);

            // Saves the pointer to the color data that will change inside for loop
            // There is no need to call Color::GetData() each loop,
            //      because the address does not change, only data inside Color changes
            void *old_color_data_ptr = data_color_->GetData();
            void *new_color_data_ptr = new_color->GetData();

            // For each pixel: load the color, tranform the color and then save the color
            for (; it_old != it_old_end; it_old++, it_new++)
            {
                // Copy from data_ to old_color
                std::copy_n(reinterpret_cast<uint8_t *>(*it_old), pixel_struct_size_byte_, reinterpret_cast<uint8_t *>(old_color_data_ptr));
                // Set grayscale_color from old_color
                new_color->SetColor(*data_color_);
                // Copy grayscale_color data to new_data
                std::copy_n(reinterpret_cast<uint8_t *>(new_color_data_ptr), new_pixel_size, reinterpret_cast<uint8_t *>(*it_new));
            }

            // Update the DataPixels to new color type
            pixel_struct_size_byte_ = new_color->GetDataSize();
            data_color_ = std::unique_ptr<Color>(new_color->clone());

            // Swap the old data of DataPixels with new data
            data_.swap(new_data);
        }

        /**
         * @brief Get the dimensions of data.
         * 
         * @return Point dimensions of DataPixels.
         */
        Point GetSize() { return image_size_; }

        /**
         * @brief Swaps the DataPixel objects.
         * 
         * @param other the other DataPixel to swap data with.
         */
        void SwapData(DataPixels &other)
        {
            std::swap(pixel_struct_size_byte_, other.pixel_struct_size_byte_);
            std::swap(image_size_, other.image_size_);
            std::swap(pixel_count_, other.pixel_count_);
            std::swap(data_color_, other.data_color_);
            std::swap(data_, other.data_);
        }

    private:
        size_t pixel_struct_size_byte_;     /// Size of single pixel.
        Point image_size_;                  /// Dimensions of pixel data (rows, columns).
        size_t pixel_count_;                /// Number of total pixels.
        std::unique_ptr<Color> data_color_; /// Color type associated with this DataPixels.
        std::unique_ptr<uint8_t[]> data_;   /// Pointer to pixel data.

        // The Painter can edit PixelData
        friend class Painter;
    };
}

#endif // PAINT_INC_DATA_PIXEL_H