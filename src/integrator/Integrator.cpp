#include "Integrator.h"

#include <integrator/Pathtracer.h>

namespace danny
{
    namespace integrator
    {
        std::unique_ptr<Integrator::Xml> Integrator::Xml::factory(const xml::Node &node)
        {
            auto integrator_type = node.attribute("type", true);

            if (integrator_type == std::string("Pathtracer"))
            {
                return std::make_unique<Pathtracer::Xml>(node);
            }
            else
            {
                node.throwError("Unknown Integrator type.");
            }

            return nullptr;
        }
    }
}