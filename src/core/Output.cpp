#include "Output.h"

#include <sstream>
#include <iostream>
#include <core/Timer.h>
#include <xml/Parser.h>

namespace danny
{
    namespace core
    {
        std::unique_ptr<Output::Xml> Output::Xml::factory(const xml::Node &node)
        {
            auto output_type = node.attribute("type", true);

            if (output_type == std::string("Ldr"))
            {
                return std::make_unique<Ldr::Xml>(node);
            }
            else
            {
                node.throwError("Unknown Output type.");
            }

            return nullptr;
        }

        Ldr::Xml::Xml(const xml::Node &node)
        {
            node.parseChildText("Path", &path);
            node.parseChildText("Format", &format);
            tonemapper = Tonemapper::Xml::factory(node.child("Tonemapper", true));

            if (xml::Parser::gSupportedFormatsSave.find(format) == xml::Parser::gSupportedFormatsSave.end())
            {
                node.child("Format").throwError("Unsupported Format.");
            }
        }

        std::unique_ptr<Output> Ldr::Xml::create() const
        {
            return std::make_unique<Ldr>(*this);
        }

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