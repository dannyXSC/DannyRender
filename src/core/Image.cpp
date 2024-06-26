#include "Image.h"
#include <core/Math.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb/stb_image_write.h>
#define STB_IMAGE_RESIZE_IMPLEMENTATION
#define STBIR_DEFAULT_FILTER_UPSAMPLE STBIR_FILTER_TRIANGLE   // Avoids ringing.
#define STBIR_DEFAULT_FILTER_DOWNSAMPLE STBIR_FILTER_TRIANGLE // Avoids ringing.
#include <stb/stb_image_resize.h>

namespace danny
{
    namespace core
    {
        template <typename T, int tMax, ImageReprBase::Type tType>
        ImageRepr<T, tMax, tType>::ImageRepr(int width, int height)
            : m_pixels(width, std::vector<RGB>(height))
        {
        }

        template <typename T, int tMax, ImageReprBase::Type tType>
        ImageRepr<T, tMax, tType>::ImageRepr(const std::string &filename)
        {
            // If given image is LDR, stbi_loadf applies an sRGB->Linear conversion.
            int channel;
            int width;
            int height;
            auto data = stbi_loadf(filename.c_str(), &width, &height, &channel, 0);

            if (!data)
            {
                throw std::runtime_error("Error: Image cannot be loaded");
            }

            m_pixels.resize(width, std::vector<RGB>(height));
            for (int j = 0, index = 0; j < height; ++j)
            {
                for (int i = 0; i < width; ++i, index += channel)
                {
                    set(i, j, glm::vec3(data[index], data[index + 1], data[index + 2]));
                }
            }

            stbi_image_free(data);
        }

        template <typename T, int tMax, ImageReprBase::Type tType>
        void ImageRepr<T, tMax, tType>::set(int x, int y, const glm::vec3 &value)
        {
            if constexpr (tMax == 1)
            {
                m_pixels[x][y] = RGB(static_cast<T>(value.x), static_cast<T>(value.y), static_cast<T>(value.z));
            }
            else
            {
                constexpr float max = tMax;
                m_pixels[x][y] = RGB(static_cast<T>(value.x * max), static_cast<T>(value.y * max), static_cast<T>(value.z * max));
            }
        }

        template <typename T, int tMax, ImageReprBase::Type tType>
        glm::vec3 ImageRepr<T, tMax, tType>::get(int x, int y) const
        {
            if constexpr (tMax == 1)
            {
                const auto &value = m_pixels[x][y];
                return glm::vec3(value.r, value.g, value.b);
            }
            else
            {
                constexpr float inv_max = 1.0f / tMax;
                const auto &value = m_pixels[x][y];
                return glm::vec3(value.r, value.g, value.b) * inv_max;
            }
        }

        template <typename T, int tMax, ImageReprBase::Type tType>
        ImageReprBase::Type ImageRepr<T, tMax, tType>::type() const
        {
            return tType;
        }

        template <typename T, int tMax, ImageReprBase::Type tType>
        std::unique_ptr<ImageReprBase> ImageRepr<T, tMax, tType>::clone() const
        {
            return std::make_unique<ImageRepr<T, tMax, tType>>(*this);
        }

        Image::Image(int width, int height, ImageReprBase::Type type)
            : m_image_repr(nullptr), m_width(width), m_height(height)
        {
            switch (type)
            {
            case ImageReprBase::Type::BYTE:
                m_image_repr = std::make_unique<ByteImage>(width, height);
                break;

            case ImageReprBase::Type::FLOAT:
                m_image_repr = std::make_unique<FloatImage>(width, height);
                break;
            }
        }

        Image::Image(const std::string &filename)
            : m_image_repr(nullptr)
        {
            if (stbi_is_hdr(filename.c_str()))
            {
                m_image_repr = std::make_unique<FloatImage>(filename);
            }
            else
            {
                m_image_repr = std::make_unique<ByteImage>(filename);
            }

            int channel;
            stbi_info(filename.c_str(), &m_width, &m_height, &channel);
        }

        Image::Image(const Image &image)
            : m_image_repr(image.m_image_repr->clone()), m_width(image.m_width), m_height(image.m_height)
        {
        }

        Image &Image::operator=(const Image &image)
        {
            m_image_repr = image.m_image_repr->clone();
            m_width = image.m_width;
            m_height = image.m_height;

            return *this;
        }

        void Image::set(int x, int y, const glm::vec3 &value)
        {
            m_image_repr->set(x, y, value);
        }

        glm::vec3 Image::get(int x, int y) const
        {
            return m_image_repr->get(x, y);
        }

        std::vector<Image> Image::generateMipmaps() const
        {
            constexpr int channel = 3; // RGB
            int src_width = m_width;
            int src_height = m_height;
            int dest_width = core::math::closestPowTwo(m_width);
            int dest_height = core::math::closestPowTwo(m_height);
            int stride_src = channel * src_width * sizeof(float);
            int stride_dest = channel * dest_width * sizeof(float);
            std::unique_ptr<float[]> src(new float[channel * src_width * src_height]);
            std::unique_ptr<float[]> dest(new float[channel * dest_width * dest_height]);
            auto src_ptr = src.get();

            for (int j = 0, index = 0; j < m_height; ++j)
            {
                for (int i = 0; i < m_width; ++i, index += channel)
                {
                    auto pixel = get(i, j);

                    src_ptr[index] = pixel.x;
                    src_ptr[index + 1] = pixel.y;
                    src_ptr[index + 2] = pixel.z;
                }
            }

            std::vector<Image> mipmaps;
            while (dest_width >= 1 && dest_height >= 1)
            {
                stbir_resize_float(src.get(), src_width, src_height, stride_src, dest.get(), dest_width, dest_height, stride_dest, channel);

                src_width = dest_width;
                src_height = dest_height;
                dest_width >>= 1;
                dest_height >>= 1;
                stride_src = channel * src_width * sizeof(float);
                stride_dest = channel * dest_width * sizeof(float);
                src = std::move(dest);
                dest = std::unique_ptr<float[]>(new float[channel * dest_width * dest_height]);
                src_ptr = src.get();

                Image mipmap(src_width, src_height, m_image_repr->type());
                for (int j = 0, index = 0; j < src_height; ++j)
                {
                    for (int i = 0; i < src_width; ++i, index += channel)
                    {
                        mipmap.set(i, j, glm::vec3(src_ptr[index], src_ptr[index + 1], src_ptr[index + 2]));
                    }
                }

                mipmaps.push_back(std::move(mipmap));
            }

            return mipmaps;
        }

        void Image::saveLdr(const std::string &filename) const
        {
            constexpr int channel = 3; // RGB
            int stride = channel * m_width;
            std::unique_ptr<unsigned char[]> data(new unsigned char[stride * m_height]);
            auto data_ptr = data.get();

            for (int j = 0, index = 0; j < m_height; ++j)
            {
                for (int i = 0; i < m_width; ++i, index += channel)
                {
                    auto pixel = get(i, j);

                    // Linear->sRGB
                    pixel.x = glm::pow(pixel.x, 0.45454545f);
                    pixel.y = glm::pow(pixel.y, 0.45454545f);
                    pixel.z = glm::pow(pixel.z, 0.45454545f);

                    pixel *= 255.0f;

                    data_ptr[index] = static_cast<unsigned char>(pixel.x);
                    data_ptr[index + 1] = static_cast<unsigned char>(pixel.y);
                    data_ptr[index + 2] = static_cast<unsigned char>(pixel.z);
                }
            }

            auto image_format = filename.substr(filename.find_last_of('.') + 1);

            if (image_format == "png")
            {
                stbi_write_png(filename.c_str(), m_width, m_height, channel, data_ptr, stride);
            }
            else if (image_format == "bmp")
            {
                stbi_write_bmp(filename.c_str(), m_width, m_height, channel, data_ptr);
            }
            else if (image_format == "tga")
            {
                stbi_write_tga(filename.c_str(), m_width, m_height, channel, data_ptr);
            }
        }
    }
}