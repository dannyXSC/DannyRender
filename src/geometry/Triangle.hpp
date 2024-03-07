#ifndef __DANNY__GEOMETRY__TRIANGLE__
#define __DANNY__GEOMETRY__TRIANGLE__

#include <geometry/BBox.hpp>
#include <geometry/Ray.hpp>
#include <geometry/Plane.hpp>
#include <geometry/Mapper.hpp>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <array>
#include <memory>

namespace danny
{
    namespace geometry
    {
        // Although having almost the same member functions, this class do not inherit from Object base class.
        // The reason is that Triangle is not designed to have its own material and transformation.
        // Triangles exist only for building a mesh.
        class Triangle
        {
        public:
            Triangle(const glm::vec3 &v0, const glm::vec3 &edge1, const glm::vec3 &edge2,
                     const glm::vec2 &uv0, const glm::vec2 &uv1, const glm::vec2 &uv2);

            geometry::Plane samplePlane(std::shared_ptr<core::UniformSampler> sampler) const;
            float getSurfaceArea() const;
            BBox getBBox() const;
            glm::vec2 getBoundsOnAxis(int axis) const;
            std::array<glm::vec3, 3> getVertices() const;
            bool intersect(const Ray &ray, Intersection &intersection, float max_distance) const;
            bool intersectShadowRay(const Ray &ray, float max_distance) const;
            void fillIntersection(const Ray &ray, Intersection &intersection) const;

        private:
            glm::vec3 m_v0;
            // 0 -> 2
            glm::vec3 m_edge1;
            // 0 -> 1
            glm::vec3 m_edge2;
            glm::vec3 m_normal;
            std::unique_ptr<Mapper> m_mapper;
        };
    }
}

#endif