#ifndef __DANNY__GEOMETRY__OBJECT__
#define __DANNY__GEOMETRY__OBJECT__
#include <core/forward_decl.h>

#include <xml/Node.h>
#include <geometry/Ray.hpp>
#include <geometry/Plane.hpp>
#include <geometry/BBox.hpp>

#include <unordered_map>
#include <glm/vec2.hpp>
#include <memory>

namespace danny
{
    namespace geometry
    {
        class Object
        {
        public:
            // Xml structure of the class.
            struct Xml
            {
                std::unordered_map<std::string, std::string> attributes;

                virtual ~Xml() = default;
                virtual std::unique_ptr<Object> create() const = 0;
                static std::unique_ptr<Object::Xml> factory(const xml::Node &node);
            };

        public:
            virtual ~Object() = default;
            virtual Plane sample(std::shared_ptr<core::UniformSampler> sampler) const = 0;
            virtual float getSurfaceArea() const = 0;
            virtual BBox getBBox() const = 0;
            virtual glm::vec2 getBoundsOnAxis(int axis) const = 0;
            virtual bool intersect(const Ray &ray, Intersection &intersection, float max_distance) const = 0;
            virtual bool intersectShadowRay(const Ray &ray, float max_distance) const = 0;
            // 因为triangle并不知道自己的坐标实际在哪（转换矩阵存在mesh里而不是triangle）
            // 所以实际的位置需要通过fillIntersection，由mesh来导入
            virtual void fillIntersection(const Ray &ray, Intersection &intersection) const = 0;
        };
    }
}
#endif