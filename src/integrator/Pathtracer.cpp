#include "Pathtracer.h"

#include <iostream>
#include <thread>
#include <mutex>

#define GLM_ENABLE_EXPERIMENTAL

#include <glm/ext.hpp>

#include <geometry/Intersection.hpp>
#include <core/CoordinateSpace.h>
#include <light/Light.h>
#include <core/Math.hpp>
#include <utils/Global.hpp>

#include <omp.h>

namespace danny
{
    namespace integrator
    {
        Pathtracer::Pathtracer(int spp, float cutoff_probability, int thread_num)
            : m_spp(spp), m_cutoff_probability(cutoff_probability), m_thread_num(thread_num) {}

        void Pathtracer::integrate(const core::Scene &scene, core::Image &output)
        {
            int width = output.get_width(), height = output.get_height();

            std::thread t[m_thread_num];
            std::mutex mtx;
            int progress = 0;

            int height_per_thread = height / m_thread_num;
            int spp_per = (int)std::sqrt(m_spp);
            float width_per_spp = 1.0 / spp_per;

            for (int t_n = 0; t_n < m_thread_num; t_n++)
            {
                t[t_n] = std::thread([&](int h_start, int h_end)
                                     {
                                        std::shared_ptr<core::UniformSampler> sampler = std::make_shared<core::UniformSampler>();

                                        for (uint32_t j = h_start; j < h_end; ++j)
                                        {
                                            for (uint32_t i = 0; i < width; ++i)
                                            {
                                                int cur_ssp_r = 0, cur_ssp_c = 0;
                                                for (int k = 0; k < m_spp; k++)
                                                {
                                                    // generate primary ray direction
                                                    // MSAA
                                                    float offset_x = width_per_spp / 2+ cur_ssp_c * width_per_spp;
                                                    float offset_y = width_per_spp / 2 + cur_ssp_r * width_per_spp;

                                                    geometry::Ray ray = scene.camera->castRay(i, j, offset_x, offset_y);
                                                    auto color = estimatePixel(scene, ray, sampler);
                                                    output.set(i, j, output.get(i, j) + color / m_spp);

                                                    cur_ssp_c++;
                                                    cur_ssp_r = (cur_ssp_r + cur_ssp_c / spp_per) % spp_per;
                                                    cur_ssp_c = cur_ssp_c % spp_per;
                                                }
                                            }
                                            mtx.lock();
                                            progress += 1;
                                            utils::UpdateProgress(1.0f * progress / height);
                                            mtx.unlock();
                                        } },
                                     t_n * height_per_thread, (t_n + 1) * height_per_thread);
            }
            for (int t_n = 0; t_n < m_thread_num; t_n++)
            {
                t[t_n].join();
            }

            utils::UpdateProgress(1.f);
            std::cout << std::endl;
        }

        const glm::vec3 Pathtracer::estimatePixel(const core::Scene &scene, const geometry::Ray &ray, std::shared_ptr<core::UniformSampler> sampler)
        {
            geometry::Intersection inter_obj;
            scene.intersect(ray, inter_obj, std::numeric_limits<float>::max());

            if (!inter_obj.object)
            {
                // TODO: 环境光处理
                return scene.getBackgroundRadiance(ray.direction, false);
            }

            // 如果是光源
            auto iter = scene.object_to_light.find(inter_obj.object);
            if (iter != scene.object_to_light.end())
            {
                // TODO: 判断多次对光源采样问题
                return iter->second->getLe(ray.direction, inter_obj.plane.normal, inter_obj.distance);
            }

            // 不是光源
            core::CoordinateSpace tangent_space(inter_obj.plane.point, inter_obj.plane.normal);
            auto wo_tangent = tangent_space.vectorToLocalSpace(-ray.direction);

            // glm::vec3 p_o_deviation = (core::math::cosTheta(wo_tangent) > 0)
            //                               ? inter_obj.plane.point + inter_obj.plane.normal * scene.secondary_ray_epsilon
            //                               : inter_obj.plane.point - inter_obj.plane.normal * scene.secondary_ray_epsilon;

            // 对光源进行重要性采样
            glm::vec3 L_dir(0);
            int size = scene.lights.size();
            for (int i = 0; i < size; i++)
            {
                auto &light = scene.lights[i];
                auto light_sample = light->sample(sampler, inter_obj);

                geometry::Ray ray_to_light(inter_obj.plane.point + scene.secondary_ray_epsilon * light_sample.wi_world, light_sample.wi_world);

                // 光线原点向normal方向移动一下
                // geometry::Ray ray_to_light(p_o_deviation, light_sample.wi_world);

                if (!scene.intersectShadowRay(ray_to_light,
                                              light_sample.distance - 1.1f * scene.secondary_ray_epsilon))
                {
                    // 与光源之间没有遮挡
                    auto wi_tangent = tangent_space.vectorToLocalSpace(light_sample.wi_world);

                    auto bsdf = inter_obj.bsdf_material->getBsdf(wi_tangent, wo_tangent, inter_obj);
                    auto le = light_sample.le;
                    auto cos = glm::abs(core::math::cosTheta(wi_tangent));
                    auto pdf = light_sample.pdf_w;

                    L_dir = L_dir + le * bsdf * cos / pdf;
                    // std::cout << "le: " << le.x << " " << le.y << " " << le.z << std::endl;
                    // std::cout << "bsdf: " << bsdf.x << " " << bsdf.y << " " << bsdf.z << std::endl;
                    // std::cout << "cos: " << cos << std::endl;
                    // std::cout << "pdf: " << pdf << std::endl;
                    // std::cout << "L_dir: " << L_dir.x << " " << L_dir.y << " " << L_dir.z << std::endl;
                }
            }

            // 俄罗斯轮盘赌
            if (sampler->sample() < m_cutoff_probability)
            {
                return L_dir;
            }
            // Indirect 采样
            glm::vec3 L_indir(0);
            auto [wi_tangent, f] = inter_obj.bsdf_material->sampleWi(wo_tangent, sampler, inter_obj);
            glm::vec3 wi_world = tangent_space.vectorToWorldSpace(wi_tangent);
            wi_world = glm::normalize(wi_world);

            geometry::Ray ray_to_next(inter_obj.plane.point + scene.secondary_ray_epsilon * wi_world, wi_world);

            // geometry::Ray ray_to_next(p_o_deviation, wi_world);
            geometry::Intersection inter_next;
            scene.intersect(ray_to_next, inter_next, std::numeric_limits<float>::max());

            if (inter_next.object &&
                scene.object_to_light.find(inter_next.object) == scene.object_to_light.end())
            {
                // 有物体，并且不是光源
                f = f / (1.f - m_cutoff_probability);
                L_indir = f * estimatePixel(scene, ray_to_next, sampler);
            }

            return L_dir + L_indir;
        }
    }
}
