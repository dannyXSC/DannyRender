#include "Triangle.hpp"

#include <iostream>
#include <glm/geometric.hpp>

#include <core/Sampler.hpp>
#include <geometry/Intersection.hpp>

namespace danny
{
    namespace geometry
    {
        Triangle::Triangle(const glm::vec3 &v0, const glm::vec3 &edge1, const glm::vec3 &edge2,
                           const glm::vec2 &uv0, const glm::vec2 &uv1, const glm::vec2 &uv2)
            : m_v0(v0), m_edge1(edge1), m_edge2(edge2), m_mapper(nullptr)
        {
            m_normal0 = m_normal1 = m_normal2 = glm::normalize(glm::cross(edge1, edge2));
            if (uv0.x == 0.0f && uv1.x == 0.0f && uv2.x == 0.0f)
            {
                m_mapper = std::make_unique<SphericalMapper>();
            }
            else
            {
                m_mapper = std::make_unique<UVMapper>(edge1, edge2, uv0, uv1, uv2);
            }
        }

        Triangle::Triangle(const glm::vec3 &v0, const glm::vec3 &edge1, const glm::vec3 &edge2,
                           const glm::vec2 &uv0, const glm::vec2 &uv1, const glm::vec2 &uv2,
                           const glm::vec3 &n0, const glm::vec3 &n1, const glm::vec3 &n2)
            : m_v0(v0), m_edge1(edge1), m_edge2(edge2), m_normal0(glm::normalize(n0)), m_normal1(glm::normalize(n1)), m_normal2(glm::normalize(n2)), m_mapper(nullptr)
        {
            if (uv0.x == 0.0f && uv1.x == 0.0f && uv2.x == 0.0f)
            {
                m_mapper = std::make_unique<SphericalMapper>();
            }
            else
            {
                m_mapper = std::make_unique<UVMapper>(edge1, edge2, uv0, uv1, uv2);
            }
        }

        geometry::Plane Triangle::samplePlane(std::shared_ptr<core::UniformSampler> sampler) const
        {
            auto v1 = m_edge1 + m_v0;
            auto v2 = m_edge2 + m_v0;

            auto sqrt_rand = glm::sqrt(sampler->sample());
            auto u = 1.0f - sqrt_rand;
            auto v = sampler->sample() * sqrt_rand;

            auto normal = m_normal0 * u + m_normal1 * v + m_normal2 * (1.0f - u - v);

            return geometry::Plane(m_v0 * u + v1 * v + v2 * (1.0f - u - v), normal);
        }

        float Triangle::getSurfaceArea() const
        {
            return glm::length(glm::cross(m_edge1, m_edge2)) * 0.5f;
        }

        BBox Triangle::getBBox() const
        {
            auto v1 = m_edge1 + m_v0;
            auto v2 = m_edge2 + m_v0;

            return BBox(glm::min(glm::min(v1, v2), m_v0), glm::max(glm::max(v1, v2), m_v0));
        }

        glm::vec2 Triangle::getBoundsOnAxis(int axis) const
        {
            auto v1 = m_edge1 + m_v0;
            auto v2 = m_edge2 + m_v0;

            return glm::vec2(glm::min(glm::min(v1[axis], v2[axis]), m_v0[axis]), glm::max(glm::max(v1[axis], v2[axis]), m_v0[axis]));
        }

        std::array<glm::vec3, 3> Triangle::getVertices() const
        {
            return {m_v0, m_edge1 + m_v0, m_edge2 + m_v0};
        }

        bool Triangle::intersect(const Ray &ray, Intersection &intersection, float max_distance) const
        {
            auto pvec = glm::cross(ray.direction, m_edge2);
            auto inv_det = 1.0f / glm::dot(m_edge1, pvec);

            auto tvec = ray.origin - m_v0;
            auto w1 = glm::dot(tvec, pvec) * inv_det;

            auto qvec = glm::cross(tvec, m_edge1);
            auto w2 = glm::dot(ray.direction, qvec) * inv_det;

            if (w1 < 0.0f || w2 < 0.0f || (w1 + w2) > 1.0f)
            {
                return false;
            }

            auto distance = glm::dot(m_edge2, qvec) * inv_det;
            if (distance > 0.0f && distance < max_distance)
            {
                intersection.distance = distance;
                intersection.triangle = this;

                return true;
            }
            return false;
        }

        bool Triangle::intersectShadowRay(const Ray &ray, float max_distance) const
        {
            Intersection s;
            return intersect(ray, s, max_distance);
        }

        void Triangle::fillIntersection(const Ray &ray, Intersection &intersection) const
        {
            auto pvec = glm::cross(ray.direction, m_edge2);
            auto inv_det = 1.0f / glm::dot(m_edge1, pvec);

            auto tvec = ray.origin - m_v0;
            auto w1 = glm::dot(tvec, pvec) * inv_det;

            auto qvec = glm::cross(tvec, m_edge1);
            auto w2 = glm::dot(ray.direction, qvec) * inv_det;

            intersection.plane.normal = (1.0f - w1 - w2) * m_normal0 + w1 * m_normal1 + w2 * m_normal2;
            auto values = m_mapper->map(ray(intersection.distance), glm::vec3(1.0f - w1 - w2, w1, w2));
            intersection.uv = values.uv;
            intersection.dpdu = values.dpdu;
            intersection.dpdv = values.dpdv;
        }
    }
}