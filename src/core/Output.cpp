#include "Output.h"

#include <sstream>
#include <iostream>
#include <core/Timer.h>

namespace danny
{
    namespace core
    {
        Ldr::Ldr(const std::string &path,
                 std::unique_ptr<Tonemapper> tonemapper)
            : m_path(path), m_tonemapper(std::move(tonemapper))
        {
        }

        void Ldr::save(const Image &image) const
        {
            std::ostringstream oss;
            m_tonemapper->tonemap(image).saveLdr(m_path);
        }
    }
}