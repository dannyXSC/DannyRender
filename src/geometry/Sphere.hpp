#ifndef __DANNY__GEOMETRY__Sphere__
#define __DANNY__GEOMETRY__Sphere__
#include <core/forward_decl.h>

#include <memory>

#include <geometry/Object.hpp>
#include <geometry/Transformation.hpp>
#include <geometry/BBox.hpp>
#include <core/Sampler.hpp>
#include <geometry/Intersection.hpp>

namespace danny
{
    namespace geometry
    {
        class Sphere : Object
        {
        public:
            Sphere(const glm::vec3 &p, float r, std::shared_ptr<material::BsdfMaterial> m)
                : Sphere(p, r, m, Transformation::Info()) {}

            Sphere(const glm::vec3 &p, float r, std::shared_ptr<material::BsdfMaterial>, Transformation::Info &&t);

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