#ifndef __DANNY__GEOMETRY__Sphere__
#define __DANNY__GEOMETRY__Sphere__
#include <core/forward_decl.h>

#include <memory>

#include <xml/Node.h>
#include <geometry/Object.hpp>
#include <geometry/Transformation.hpp>
#include <geometry/BBox.hpp>
#include <core/Sampler.hpp>
#include <geometry/Intersection.hpp>

namespace danny
{
    namespace geometry
    {
        class Sphere : public Object
        {
        public:
            // Xml structure of the class.
            struct Xml : public Object::Xml
            {
                float radius;
                glm::vec3 center;
                Transformation::Xml transformation;
                std::unique_ptr<material::BsdfMaterial::Xml> bsdf_material;

                explicit Xml(const xml::Node &node);
                Xml(float p_radius, glm::vec3 p_center, const Transformation::Xml &p_transformation, std::unique_ptr<material::BsdfMaterial::Xml> p_bsdf_material);
                std::unique_ptr<Object> create() const override;
            };

        public:
            Sphere(const Sphere::Xml &xml)
                : Sphere(xml.center, xml.radius, xml.bsdf_material->create(), xml.transformation.info) {}
            Sphere(const glm::vec3 &p, float r, std::shared_ptr<material::BsdfMaterial> m)
                : Sphere(p, r, m, Transformation::Info()) {}

            Sphere(const glm::vec3 &p, float r, std::shared_ptr<material::BsdfMaterial>, const Transformation::Info &t);

            Plane sample(std::shared_ptr<core::UniformSampler> sampler) const override;
            float getSurfaceArea() const override;
            BBox getBBox() const override;
            glm::vec2 getBoundsOnAxis(int axis) const override;
            bool intersect(const Ray &ray, Intersection &intersection, float max_distance) const override;
            bool intersectShadowRay(const Ray &ray, float max_distance) const override;
            void fillIntersection(const Ray &ray, Intersection &intersection) const override;

        private:
            Transformation m_transformation;
            geometry::BBox m_bbox;
            float m_area;
            std::shared_ptr<material::BsdfMaterial> m_bsdf_material;
        };
    }
}

#endif