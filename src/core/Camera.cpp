#include "Camera.h"

#include <core/PinholeCamera.h>

namespace danny
{
    namespace core
    {
        std::unique_ptr<Camera::Xml> Camera::Xml::factory(const xml::Node &node)
        {
            auto camera_type = node.attribute("type", true);

            if (camera_type == std::string("Pinhole"))
            {
                return std::make_unique<PinholeCamera::Xml>(node);
            }
            else
            {
                node.throwError("Unknown Output type.");
            }

            return nullptr;
        }
    }
}