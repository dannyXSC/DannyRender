#include "Pathtracer.h"

#define GLM_ENABLE_EXPERIMENTAL

#include <glm/ext.hpp>

#include <integrator/Integrator.h>
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
        Pathtracer::Xml::Xml(const xml::Node &node)
        {
            // filter = core::Filter::Xml::factory(node.child("Filter", true));
            node.parseChildText("SampleCount", &sample_count);
            node.parseChildText("CutoffProbability", &cutoff_probability, 0.5f);
            // node.parseChildText("RRThreshold", &rr_threshold);
        }

        std::unique_ptr<Integrator> Pathtracer::Xml::create() const
        {
            return std::make_unique<Pathtracer>(*this);
        }

        Pathtracer::Pathtracer(int spp, float cutoff_probability, int thread_num)
            : m_spp(spp), m_cutoff_probability(cutoff_probability), m_thread_num(thread_num), m_progress(0) {}

        void Pathtracer::integrate(const core::Scene &scene, core::Image &output)
        {
            auto resolution = scene.camera->get_resolution();
            int x, y;

#pragma omp parallel num_threads(std::thread::hardware_concurrency())
            {
#pragma omp for schedule(dynamic) collapse(2)
                for (x = 0; x < resolution.x; x += cPTPatchSize)
                {
                    for (y = 0; y < resolution.y; y += cPTPatchSize)
                    {
                        integratePatch(scene, output, x, y, omp_get_thread_num());
                    }
                }
            }
            utils::UpdateProgress(1.0f);
            std::cout << std::endl;
        }

        void Pathtracer::integratePatch(const core::Scene &scene, core::Image &output, int x, int y, int id)
        {
            auto resolution = scene.camera->get_resolution();
            auto bound_x = glm::min(cPTPatchSize, resolution.x - x);
            auto bound_y = glm::min(cPTPatchSize, resolution.y - y);

            std::shared_ptr<core::UniformSampler> sampler = std::make_shared<core::UniformSampler>();
            float weight = 1.0f / m_spp;
            int total_task = resolution.x * resolution.y;
            for (int i = 0; i < bound_x; ++i)
            {
                for (int j = 0; j < bound_y; ++j)
                {
                    glm::vec3 result(0.0);
                    for (int k = 0; k < m_spp; ++k)
                    {
                        geometry::Ray ray = scene.camera->castRay(x + i, y + j, sampler->sample(), sampler->sample());

                        auto color = estimatePixel(scene, ray, sampler);
                        result += color * weight;
                    }
                    output.set(x + i, y + j, result);
                }
                m_mtx.lock();
                m_progress += bound_y;
                utils::UpdateProgress(1.0f * m_progress / total_task);
                m_mtx.unlock();
            }
        }

        const glm::vec3 Pathtracer::estimatePixel(const core::Scene &scene,
                                                  const geometry::Ray &ray,
                                                  std::shared_ptr<core::UniformSampler> sampler,
                                                  bool light_explicitly_sampled)
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
                // 如果已经采样过了，就不贡献能量了
                if (!light_explicitly_sampled)
                    return iter->second->getLe(ray.direction, inter_obj.plane.normal, inter_obj.distance);
                else
                    return glm::vec3(0);
            }

            // 定义击中点的切空间
            core::CoordinateSpace tangent_space(inter_obj.plane.point, inter_obj.plane.normal);
            auto wo_tangent = tangent_space.vectorToLocalSpace(-ray.direction);
            inter_obj.dpdu_local = tangent_space.vectorToLocalSpace(glm::normalize(inter_obj.dpdu));
            inter_obj.dpdv_local = tangent_space.vectorToLocalSpace(glm::normalize(inter_obj.dpdv));

            // 对光源进行重要性采样
            glm::vec3 L_dir(0);
            light_explicitly_sampled = false;
            for (int i = 0; i < scene.lights.size(); i++)
            {
                auto &light = scene.lights[i];
                auto light_sample = light->sample(sampler, inter_obj);

                auto ray_dot_n = glm::dot(ray.direction, inter_obj.plane.normal);
                auto offset = (ray_dot_n > 0
                                   ? -inter_obj.plane.normal * scene.secondary_ray_epsilon
                                   : inter_obj.plane.normal * scene.secondary_ray_epsilon);
                // geometry::Ray ray_to_light(inter_obj.plane.point + scene.secondary_ray_epsilon * light_sample.wi_world, light_sample.wi_world);
                geometry::Ray ray_to_light(inter_obj.plane.point + offset, light_sample.wi_world);

                // 光线原点向normal方向移动一下
                // geometry::Ray ray_to_light(p_o_deviation, light_sample.wi_world);
                auto wi_tangent = tangent_space.vectorToLocalSpace(light_sample.wi_world);
                auto bsdf = inter_obj.bsdf_material->getBsdf(wi_tangent, wo_tangent, inter_obj);

                light_explicitly_sampled = true;

                if (!scene.intersectShadowRay(ray_to_light,
                                              light_sample.distance +
                                                  5.f *
                                                      scene.secondary_ray_epsilon *
                                                      (ray_dot_n * glm::dot(light_sample.wi_world, inter_obj.plane.normal) > 0
                                                           ? 1
                                                           : -1)))
                {
                    // 与光源之间没有遮挡
                    auto le = light_sample.le;
                    auto cos = glm::abs(core::math::cosTheta(wi_tangent));
                    auto pdf = light_sample.pdf_w;

                    auto f = le * bsdf * cos / pdf;
                    // 判断是不是低概率打到的
                    auto f_sum = f.x + f.y + f.z;
                    if (f_sum > 0.f && !glm::isinf(f_sum))
                    {
                        L_dir = L_dir + f;
                    }
                    // 若没有概率从这个方向射出去
                    // 就说明这束光所在的立体角，应该不需要被重要性采样
                    if (glm::l2Norm(bsdf) < 1e-5)
                        light_explicitly_sampled = false;
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

            // 轮盘赌
            f = f / (1.f - m_cutoff_probability);
            geometry::Ray ray_to_next(inter_obj.plane.point + scene.secondary_ray_epsilon * wi_world, wi_world);
            // 判断是不是光源，由light_explicitly_sampled 携带信息了
            // 判断击中物体了没，由下一次迭代计算
            f = f * estimatePixel(scene, ray_to_next, sampler, light_explicitly_sampled);
            auto f_sum = f.x + f.y + f.z;

            if (f_sum > 0.f && !glm::isinf(f_sum))
            {
                L_indir = f;
            }

            return L_dir + L_indir;
        }
    }
}