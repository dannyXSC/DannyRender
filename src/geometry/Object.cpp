#include "Object.hpp"
#include <xml/Node.h>
#include <geometry/Mesh.hpp>
#include <geometry/Sphere.hpp>

namespace danny
{
    namespace geometry
    {
        std::unique_ptr<Object::Xml> Object::Xml::factory(const xml::Node &node)
        {
            auto object_type = node.attribute("type", true);

            if (object_type == std::string("Mesh"))
            {
                return std::make_unique<Mesh::Xml>(node);
            }
            else if (object_type == std::string("Sphere"))
            {
                return std::make_unique<Sphere::Xml>(node);
            }
            else
            {
                node.throwError("Unknown Object type.");
            }

            return nullptr;
        }
    }
}