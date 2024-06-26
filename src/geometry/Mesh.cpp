#include "Mesh.hpp"

#include <iostream>

#include <xml/Parser.h>

namespace danny
{
    namespace geometry
    {
        Mesh::Xml::Xml(const xml::Node &node)
        {
            attributes = node.attributes();
            node.parseChildText("Datapath", &datapath);
            // for a .obj file contains multiple objects
            // default ""
            node.parseChildText("ObjectName", &object_name, object_name);
            transformation = node.child("Transformation") ? Transformation::Xml(node.child("Transformation")) : Transformation::Xml();
            bsdf_material = node.parent().value() == std::string("Light") ? nullptr : material::BsdfMaterial::Xml::factory(node.child("BsdfMaterial", true));
        }

        Mesh::Xml::Xml(const std::string &p_datapath, const Transformation::Xml &p_transformation, std::unique_ptr<material::BsdfMaterial::Xml> p_bsdf_material)
            : datapath(p_datapath), transformation(p_transformation), bsdf_material(std::move(p_bsdf_material))
        {
        }

        std::unique_ptr<Object> Mesh::Xml::create() const
        {
            return std::make_unique<Mesh>(*this);
        }

        Mesh::Mesh(const std::string path_bvh, const std::string object_name, std::shared_ptr<material::BsdfMaterial> m, const Transformation::Info &t)
            : Mesh(object_name.size() == 0 ? xml::Parser::loadModel(path_bvh) : xml::Parser::loadModel(path_bvh, object_name),
                   m, std::move(t))
        {
        }

        Mesh::Mesh(std::shared_ptr<BVH<Triangle>> bvh, std::shared_ptr<material::BsdfMaterial> m, const Transformation::Info &t)
            : m_transformation(t),
              m_bvh(bvh),
              m_area(0.0f),
              m_bsdf_material(m)
        {
            std::vector<float> triangle_areas;
            for (const auto &triangle : m_bvh->get_objects())
            {
                auto vertices = triangle.getVertices();
                auto v0 = m_transformation.pointToWorldSpace(vertices[0]);
                auto v1 = m_transformation.pointToWorldSpace(vertices[1]);
                auto v2 = m_transformation.pointToWorldSpace(vertices[2]);
                m_bbox.extend(v0);
                m_bbox.extend(v1);
                m_bbox.extend(v2);
                auto area = glm::length(glm::cross(v1 - v0, v2 - v0)) * 0.5f;
                triangle_areas.push_back(area);
                m_area += area;
            }
            m_triangle_sampler = core::Discrete1DSampler(triangle_areas);
        }

        geometry::Plane Mesh::sample(std::shared_ptr<core::UniformSampler> sampler) const
        {
            return m_transformation.planeToWorldSpace(m_bvh->get_objects()[m_triangle_sampler.sample(sampler)].samplePlane(sampler));
        }

        float Mesh::getSurfaceArea() const
        {
            return m_area;
        }

        BBox Mesh::getBBox() const
        {
            return m_bbox;
        }

        glm::vec2 Mesh::getBoundsOnAxis(int axis) const
        {
            return glm::vec2(m_bbox.get_min()[axis], m_bbox.get_max()[axis]);
        }

        bool Mesh::intersect(const Ray &ray, Intersection &intersection, float max_distance) const
        {
            if (m_bvh->intersect(m_transformation.rayToObjectSpace(ray), intersection, max_distance))
            {
                intersection.object = this;
                return true;
            }
            return false;
        }

        bool Mesh::intersectShadowRay(const Ray &ray, float max_distance) const
        {
            return m_bvh->intersectShadowRay(m_transformation.rayToObjectSpace(ray), max_distance);
        }

        void Mesh::fillIntersection(const Ray &ray, Intersection &intersection) const
        {
            intersection.triangle->fillIntersection(m_transformation.rayToObjectSpace(ray), intersection);

            intersection.plane.point = ray(intersection.distance);
            intersection.plane.normal = glm::normalize(m_transformation.normalToWorldSpace(intersection.plane.normal));
            intersection.dpdu = m_transformation.vectorToWorldSpace(intersection.dpdu);
            intersection.dpdv = m_transformation.vectorToWorldSpace(intersection.dpdv);
            intersection.bsdf_material = m_bsdf_material.get();
        }
    }
}