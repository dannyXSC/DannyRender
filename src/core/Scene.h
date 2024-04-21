#ifndef __DANNY__CORE__SCENE__
#define __DANNY__CORE__SCENE__

#include <vector>
#include <memory>
#include <unordered_map>

#include <core/forward_decl.h>
#include <xml/Node.h>
#include <integrator/Integrator.h>
#include <integrator/Pathtracer.h>
#include <core/Output.h>
#include <core/Tonemapper.h>
#include <core/Camera.h>
#include <core/PinholeCamera.h>
#include <light/Light.h>
#include <geometry/BVH.h>
#include <core/Image.h>

namespace danny
{
    namespace core
    {
        class Scene
        {
        public:
            // Xml structure of the class.
            struct Xml
            {
                glm::vec3 background_radiance;
                float secondary_ray_epsilon;
                std::unique_ptr<integrator::Integrator::Xml> integrator;
                std::vector<std::unique_ptr<Output::Xml>> outputs;
                std::unique_ptr<Camera::Xml> camera;
                std::vector<std::unique_ptr<geometry::Object::Xml>> objects;
                std::vector<std::unique_ptr<light::Light::Xml>> lights;

                explicit Xml(const xml::Node &node);
            };

        public:
            std::unique_ptr<Camera> camera;
            std::vector<std::shared_ptr<light::Light>> lights;
            std::shared_ptr<light::Light> environment_light;
            std::unordered_map<const geometry::Object *, const light::Light *> object_to_light;
            glm::vec3 background_radiance;
            float secondary_ray_epsilon;

        public:
            explicit Scene(const Scene::Xml &xml);
            explicit Scene(std::unique_ptr<integrator::Integrator> integrator,
                           std::unique_ptr<Camera> camera,
                           std::vector<std::unique_ptr<Output>> &outputs,
                           float secondary_ray_epsilon = 1e-4f,
                           const glm::vec3 &background_radiance = glm::vec3(0., 0., 0.));
            explicit Scene(std::unique_ptr<integrator::Integrator> integrator,
                           std::unique_ptr<Camera> camera,
                           std::vector<std::unique_ptr<Output>> &outputs,
                           std::vector<std::unique_ptr<geometry::Object>> &obj_list,
                           std::vector<std::unique_ptr<light::Light>> &light_list,
                           float secondary_ray_epsilon = 1e-4f,
                           const glm::vec3 &background_radiance = glm::vec3(0., 0., 0.));

            geometry::BBox getBBox() const;
            bool intersect(const geometry::Ray &ray, geometry::Intersection &intersection, float max_distance) const;
            bool intersectShadowRay(const geometry::Ray &ray, float max_distance) const;
            void render();
            glm::vec3 getBackgroundRadiance(const glm::vec3 &direction, bool light_explicitly_sampled) const;

            void addObject(std::unique_ptr<geometry::Object> obj);
            void addLight(std::unique_ptr<light::Light> light);
            void setEnvironmentLight(std::unique_ptr<light::Light> light);
            void buildBVH();

        private:
            bool m_bvh_flag;
            geometry::BVH<std::shared_ptr<geometry::Object>> m_bvh;
            std::unique_ptr<integrator::Integrator> m_integrator;
            std::unique_ptr<Image> m_image;
            std::vector<std::unique_ptr<Output>> m_outputs;
        };
    }
}

#endif
