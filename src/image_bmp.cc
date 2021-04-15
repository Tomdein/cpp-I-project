#include "image_bmp.h"

namespace paint
{
    namespace image_bmp
    {
        bool ImageBMP::CheckHeader()
        {
            if (header_bmp_.bf_type != kBfType)
            {
                return false;
            }

            if (header_bmp_.bf_reserved1 != 0x0)
            {
                return false;
            }

            if (header_bmp_.bf_reserved2 != 0x0)
            {
                return false;
            }

            if (header_bmp_.bf_offBits < 0x36)
            {
                return false;
            }

            return true;
        }

        bool ImageBMP::CheckHeaderInfo()
        {
            if (header_bmp_info_.bi_planes != kPlanes)
            {
                return false;
            }

            if (header_bmp_info_.bi_bitCount != BiBitCount::k1bpPX ||
                header_bmp_info_.bi_bitCount != BiBitCount::k4bpPX ||
                header_bmp_info_.bi_bitCount != BiBitCount::k8bpPX ||
                header_bmp_info_.bi_bitCount != BiBitCount::k24bpPX)
            {
                return false;
            }

            if (header_bmp_info_.bi_compression != BiCompression::kBiRGB ||
                header_bmp_info_.bi_compression != BiCompression::kBiRLE8 ||
                header_bmp_info_.bi_compression != BiCompression::kBiRLE4)
            {
                return false;
            }

            return true;
        }
    }
}