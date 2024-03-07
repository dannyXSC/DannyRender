#include "Sphere.hpp"

#include <glm/gtc/constants.hpp>

#include <geometry/Mapper.hpp>
#include <core/Math.hpp>
namespace danny
{
    namespace geometry
    {
        Sphere::Sphere(const glm::vec3 &p, float r, std::shared_ptr<material::BsdfMaterial> material, Transformation::Info &&t)
            : m_transformation(t), m_bsdf_material(material)
        {
            // convert to object space whose sphere radius is 1
            // and center at 0,0,0
            auto t_radius = r * t.scaling.x;
            auto t_center = p + t.translation;

            auto transformation = t;
            transformation.scaling = glm::vec3(t_radius);
            transformation.translation = t_center;

            m_transformation = Transformation(transformation);
            m_bbox = geometry::BBox(t_center - transformation.scaling, t_center + transformation.scaling);
            m_area = 4.0f * glm::pi<float>() * t_radius * t_radius;
        }

        Plane Sphere::sample(std::shared_ptr<core::UniformSampler> sampler) const
        {
            auto point = core::math::sampleHemisphereUniform(sampler->sample(), sampler->sample()).toCartesianCoordinate();
            return m_transformation.planeToWorldSpace(Plane(point, point));
        }

        float Sphere::getSurfaceArea() const
        {
            return m_area;
        }

        BBox Sphere::getBBox() const
        {
            return m_bbox;
        }

        glm::vec2 Sphere::getBoundsOnAxis(int axis) const
        {
            return glm::vec2(m_bbox.get_min()[axis], m_bbox.get_max()[axis]);
        }

        bool Sphere::intersect(const Ray &ray, Intersection &intersection, float max_distance) const
        {
            // Analytic solution.
            auto transformed_ray = m_transformation.rayToObjectSpace(ray);
            auto a = glm::dot(transformed_ray.direction, transformed_ray.direction);
            auto b = 2.0f * glm::dot(transformed_ray.direction, transformed_ray.origin);
            auto c = glm::dot(transformed_ray.origin, transformed_ray.origin) - 1.0f;

            auto distance = 0.0f;
            auto discriminant = b * b - 4.0f * a * c;
            if (discriminant >= 0.0f)
            {
                auto delta = glm::sqrt(discriminant);
                auto div = 0.5f / a; // To prevent cancelling out.
                distance = -div * b - div * delta;

                if (distance < 0.0f)
                {
                    distance = -div * b + div * delta;
                }
            }

            if (distance > 0.0f && distance < max_distance)
            {
                intersection.distance = distance;
                intersection.object = this;

                return true;
            }

            return false;
        }

        bool Sphere::intersectShadowRay(const Ray &ray, float max_distance) const
        {
            Intersection s;
            return intersect(ray, s, max_distance);
        }

        void Sphere::fillIntersection(const Ray &ray, Intersection &intersection) const
        {
            auto transformed_ray = m_transformation.rayToObjectSpace(ray);

            intersection.plane.point = ray(intersection.distance);
            auto local_point = transformed_ray(intersection.distance);
            intersection.plane.normal = glm::normalize(m_transformation.normalToWorldSpace(local_point));

            auto values = SphericalMapper().map(local_point, glm::vec3(0.0f));
            intersection.uv = values.uv;
            intersection.dpdu = m_transformation.vectorToWorldSpace(values.dpdu);
            intersection.dpdv = m_transformation.vectorToWorldSpace(values.dpdv);
            intersection.bsdf_material = m_bsdf_material.get();
        }
    }
}