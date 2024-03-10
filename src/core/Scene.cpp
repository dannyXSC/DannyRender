#include "Scene.h"
#include <stdexcept>
#include <iostream>
#include <memory>

#include <core/Timer.h>
#include <core/Output.h>
#include <core/Tonemapper.h>

namespace danny
{
    namespace core
    {
        inline float clamp(const float &lo, const float &hi, const float &v)
        {
            return std::max(lo, std::min(hi, v));
        }

        Scene::Scene(std::unique_ptr<integrator::Integrator> integrator,
                     std::unique_ptr<Camera> camera,
                     const glm::vec3 &background_radiance,
                     float secondary_ray_epsilon)
        {
            this->m_bvh_flag = false;
            this->background_radiance = background_radiance;
            this->secondary_ray_epsilon = secondary_ray_epsilon;
            this->m_integrator = std::move(integrator);
            // not consider output for a moment
            this->camera = std::move(camera);
            this->m_image = std::make_unique<Image>(this->camera->get_resolution().x, this->camera->get_resolution().y);
        }

        Scene::Scene(std::unique_ptr<integrator::Integrator> integrator,
                     std::unique_ptr<Camera> camera,
                     std::vector<std::unique_ptr<geometry::Object>> &obj_list,
                     std::vector<std::unique_ptr<light::Light>> &light_list,
                     const glm::vec3 &background_radiance,
                     float secondary_ray_epsilon)
            : Scene(std::move(integrator), std::move(camera), background_radiance, secondary_ray_epsilon)
        {
            for (auto &obj : obj_list)
                addObject(std::move(obj));

            for (auto &light : light_list)
                addLight(std::move(light));

            buildBVH();
        }

        void Scene::addObject(std::unique_ptr<geometry::Object> obj)
        {
            m_bvh.addObject(std::move(obj));
        }

        void Scene::addLight(std::unique_ptr<light::Light> light)
        {
            auto object = light->getObject();
            if (object)
            {
                // environment light does not have object
                object_to_light[object.get()] = light.get();
                m_bvh.addObject(object);
            }
            lights.push_back(std::move(light));
        }

        void Scene::setEnvironmentLight(std::unique_ptr<light::Light> light)
        {
            if (environment_light)
            {
                throw std::runtime_error("Error: There can be at most one EnvironmentLight");
            }
            addLight(std::move(light));
            environment_light = lights.back();
        }

        void Scene::buildBVH()
        {
            if (m_bvh.get_objects().size() < 1024)
            {
                m_bvh.buildWithMedianSplit();
            }
            else
            {
                m_bvh.buildWithSAHSplit();
            }
            this->m_bvh_flag = true;
        }

        geometry::BBox Scene::getBBox() const
        {
            return m_bvh.get_root()->bbox;
        }

        bool
        Scene::intersect(const geometry::Ray &ray, geometry::Intersection &intersection, float max_distance) const
        {
            auto result = m_bvh.intersect(ray, intersection, max_distance);

            if (intersection.object)
            {
                intersection.object->fillIntersection(ray, intersection);
            }

            return result;
        }

        bool Scene::intersectShadowRay(const geometry::Ray &ray, float max_distance) const
        {
            return m_bvh.intersectShadowRay(ray, max_distance);
        }

        void Scene::render()
        {
            // assert that bvh has been build
            assert(m_bvh_flag == true);

            Timer timer;
            timer.start();
            m_integrator->integrate(*this, *m_image);
            std::cout << "Render time: " << timer.getTime() << std::endl;

            // not use output here
            // just save it
            Ldr output("../result/", std::make_unique<Clamp>(0., 1.));
            output.save(*m_image, ".png");
        }

        glm::vec3 Scene::getBackgroundRadiance(const glm::vec3 &direction, bool light_explicitly_sampled) const
        {
            if (environment_light)
            {
                if (!light_explicitly_sampled)
                {
                    return environment_light->getLe(direction, glm::vec3(0.0f), 0.0f);
                }
                else
                {
                    return glm::vec3(0.0f);
                }
            }
            else
            {
                return background_radiance;
            }
        }
    }
}