#include "Light.h"

#include <light/DiffuseArealight.h>

namespace danny
{
    namespace light
    {
        std::unique_ptr<Light::Xml> Light::Xml::factory(const xml::Node &node)
        {
            auto light_type = node.attribute("type", true);

            if (light_type == std::string("DiffuseArealight"))
            {
                return std::make_unique<DiffuseArealight::Xml>(node);
            }
            else
            {
                node.throwError("Unknown Light type.");
            }

            return nullptr;
        }
    }
}