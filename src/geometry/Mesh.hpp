#ifndef __DANNY__GEOMETRY__MESH__
#define __DANNY__GEOMETRY__MESH__

#include <memory>

#include <geometry/Object.hpp>
#include <geometry/Plane.hpp>
#include <geometry/Transformation.hpp>
#include <geometry/BVH.h>
#include <core/Discrete1dSampler.h>

namespace danny
{
    namespace geometry
    {
        class Mesh : public Object
        {
        public:
            explicit Mesh(const std::string path_bvh, std::shared_ptr<material::BsdfMaterial> m, Transformation::Info &&t = Transformation::Info());
            explicit Mesh(std::shared_ptr<BVH<Triangle>> bvh, std::shared_ptr<material::BsdfMaterial> m, Transformation::Info &&t = Transformation::Info());

            Plane sample(std::shared_ptr<core::UniformSampler> sampler) const override;
            float getSurfaceArea() const override;
            BBox getBBox() const override;
            glm::vec2 getBoundsOnAxis(int axis) const override;
            bool intersect(const Ray &ray, Intersection &intersection, float max_distance) const override;
            bool intersectShadowRay(const Ray &ray, float max_distance) const override;
            void fillIntersection(const Ray &ray, Intersection &intersection) const override;

        private:
            Transformation m_transformation;
            BBox m_bbox;
            core::Discrete1DSampler m_triangle_sampler;
            float m_area;
            std::shared_ptr<BVH<Triangle>> m_bvh;
            std::shared_ptr<material::BsdfMaterial> m_bsdf_material;
        };
    }
}

#endif