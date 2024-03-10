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

        void Ldr::save(const Image &image, const std::string &name) const
        {
            std::ostringstream oss;
            oss << m_path << Timer::nowTime() << "_" << name;
            std::cout << oss.str() << std::endl;
            m_tonemapper->tonemap(image).saveLdr(oss.str());
        }
    }
}