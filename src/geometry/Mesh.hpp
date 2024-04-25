#ifndef __DANNY__GEOMETRY__MESH__
#define __DANNY__GEOMETRY__MESH__

#include <memory>

#include <geometry/Object.hpp>
#include <geometry/Plane.hpp>
#include <geometry/Transformation.hpp>
#include <geometry/BVH.h>
#include <core/Discrete1dSampler.h>
#include <light/Light.h>

namespace danny
{
    namespace geometry
    {
        class Mesh : public Object
        {
        public:
            // Xml structure of the class.
            struct Xml : public Object::Xml
            {
                std::string datapath;
                std::string object_name;
                Transformation::Xml transformation;
                std::unique_ptr<material::BsdfMaterial::Xml> bsdf_material;

                explicit Xml(const xml::Node &node);
                Xml(const std::string &p_datapath, const Transformation::Xml &p_transformation, std::unique_ptr<material::BsdfMaterial::Xml> p_bsdf_material);
                std::unique_ptr<Object> create() const override;
            };

        public:
            explicit Mesh(const Mesh::Xml &xml)
                : Mesh(xml.datapath, xml.object_name, xml.bsdf_material ? xml.bsdf_material->create() : nullptr, xml.transformation.info){};
            explicit Mesh(const std::string path_bvh, const std::string object_name, std::shared_ptr<material::BsdfMaterial> m, const Transformation::Info &t = Transformation::Info());
            explicit Mesh(std::shared_ptr<BVH<Triangle>> bvh, std::shared_ptr<material::BsdfMaterial> m, const Transformation::Info &t = Transformation::Info());

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