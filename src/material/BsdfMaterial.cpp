#include "BsdfMaterial.hpp"

#include "Diffuse.hpp"
#include "Metal.h"

namespace danny
{
    namespace material
    {
        std::unique_ptr<BsdfMaterial::Xml> BsdfMaterial::Xml::factory(const xml::Node &node)
        {
            auto bsdf_type = node.attribute("type", true);

            if (bsdf_type == std::string("Diffuse"))
            {
                return std::make_unique<material::Diffuse::Xml>(node);
            }
            else if (bsdf_type == std::string("Metal"))
            {
                return std::make_unique<material::Metal::Xml>(node);
            }
            else
            {
                node.throwError("Unknown BsdfMaterial type.");
            }

            return nullptr;
        }
    }
}