#include <iostream>
#include <memory>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/vec3.hpp>

#include <core/Scene.h>
#include <core/PinholeCamera.h>
#include <material/Diffuse.hpp>
#include <material/Metal.h>
#include <texture/ConstantTexture.h>
#include <texture/IntervalTexture.h>
#include <texture/Texture.h>
#include <geometry/Mesh.hpp>
#include <geometry/Sphere.hpp>
#include <light/DiffuseArealight.h>
#include <integrator/Pathtracer.h>
#include <material/Cloth.h>

#include <glm/ext.hpp>

danny::material::ClothPara getCloth1()
{
    danny::material::ClothPara cloth_para1;
    cloth_para1.albedo = glm::vec3(0.2, 0.8, 1);
    cloth_para1.gamma_s = 12.f;
    cloth_para1.gamma_v = 24.f;
    cloth_para1.k_d = 0.3f;
    cloth_para1.eta = 1.46f;
    cloth_para1.alpha = 0.33f;
    cloth_para1.tangent_offsets = {-25, 25};
    return cloth_para1;
}

std::pair<danny::material::ClothPara, danny::material::ClothPara> getCloth2()
{
    danny::material::ClothPara cloth_para1;
    // cloth_para1.albedo = glm::vec3(1, 0.95, 0.05) * 0.12;
    cloth_para1.albedo = glm::vec3(1, 0.95, 0.05) * 0.9;
    cloth_para1.gamma_s = 5.f;
    cloth_para1.gamma_v = 10.f;
    cloth_para1.k_d = 0.2f;
    cloth_para1.eta = 1.345f;
    cloth_para1.alpha = 0.75f;
    cloth_para1.tangent_offsets = {-35, -35, 35, 35};

    danny::material::ClothPara cloth_para2;
    // cloth_para2.albedo = glm::vec3(1, 0.95, 0.05) * 0.16;
    cloth_para2.albedo = glm::vec3(1, 0.95, 0.05);
    cloth_para2.gamma_s = 18.f;
    cloth_para2.gamma_v = 32.f;
    cloth_para2.k_d = 0.3f;
    cloth_para2.eta = 1.345f;
    cloth_para2.alpha = 0.25f;
    cloth_para2.tangent_offsets = {0, 0};

    return std::make_pair(cloth_para1, cloth_para2);
}

std::pair<danny::material::ClothPara, danny::material::ClothPara> getCloth3()
{
    danny::material::ClothPara cloth_para1;
    cloth_para1.albedo = glm::vec3(1, 0.37, 0.3) * 0.5;
    cloth_para1.gamma_s = 2.5f;
    cloth_para1.gamma_v = 5.f;
    cloth_para1.k_d = 0.1f;
    cloth_para1.eta = 1.539f;
    cloth_para1.alpha = 0.9f;
    cloth_para1.tangent_offsets = {-32, -32, -18, 0, 0, 18, 32, 32};

    danny::material::ClothPara cloth_para2;
    cloth_para2.albedo = glm::vec3(1, 0.37, 0.3);
    cloth_para2.gamma_s = 30.f;
    cloth_para2.gamma_v = 60.f;
    cloth_para2.k_d = 0.7f;
    cloth_para2.eta = 1.539f;
    cloth_para2.alpha = 0.1f;
    cloth_para2.tangent_offsets = {0, 0};

    return std::make_pair(cloth_para1, cloth_para2);
}

int main()
{
    auto [cloth_para1, cloth_para2] = getCloth2();

    auto red = std::make_shared<danny::texture::ConstantTexture>(glm::vec3(0.63f, 0.065f, 0.05f));
    auto green = std::make_shared<danny::texture::ConstantTexture>(glm::vec3(0.14f, 0.45f, 0.091f));
    auto white = std::make_shared<danny::texture::ConstantTexture>(glm::vec3(0.725f, 0.71f, 0.68f));
    auto blue = std::make_shared<danny::texture::ConstantTexture>(glm::vec3(0.14f, 0.05f, 0.45f));
    auto interval = std::make_shared<danny::texture::IntervalTexture>();

    auto red_diffuse = std::make_shared<danny::material::Diffuse>(red);
    auto green_diffuse = std::make_shared<danny::material::Diffuse>(green);
    auto white_diffuse = std::make_shared<danny::material::Diffuse>(white);
    auto blue_diffuse = std::make_shared<danny::material::Diffuse>(blue);
    auto interval_diffuse = std::make_shared<danny::material::Diffuse>(interval);
    auto cloth_material = std::make_shared<danny::material::Cloth>(cloth_para1, cloth_para2);

    auto roughness_0_15 = std::make_shared<danny::texture::ConstantTexture>(glm::vec3(0.15, 0, 0));
    auto metal_0_15 = std::make_shared<danny::material::Metal>(roughness_0_15, 4.);

    auto roughness_0 = std::make_shared<danny::texture::ConstantTexture>(glm::vec3(0., 0, 0));
    auto metal_0 = std::make_shared<danny::material::Metal>(roughness_0, 4.);

    auto cloth_obj = std::make_unique<danny::geometry::Mesh>(
        "../asset/models/cloth/n16-w400-texture01.obj",
        cloth_material);

    auto floor = std::make_unique<danny::geometry::Mesh>(
        "../asset/models/cornellbox/floor.obj",
        white_diffuse);
    auto shortbox = std::make_unique<danny::geometry::Mesh>(
        "../asset/models/cornellbox/shortbox.obj",
        white_diffuse);
    auto tallbox = std::make_unique<danny::geometry::Mesh>(
        "../asset/models/cornellbox/tallbox.obj",
        white_diffuse);
    auto shortbox_metal_0 = std::make_unique<danny::geometry::Mesh>(
        "../asset/models/cornellbox/shortbox.obj",
        metal_0);
    auto tallbox_metal_0 = std::make_unique<danny::geometry::Mesh>(
        "../asset/models/cornellbox/tallbox.obj",
        metal_0);
    auto left = std::make_unique<danny::geometry::Mesh>(
        "../asset/models/cornellbox/left.obj",
        red_diffuse);
    auto right = std::make_unique<danny::geometry::Mesh>(
        "../asset/models/cornellbox/right.obj",
        green_diffuse);

    auto light_obj = std::make_unique<danny::geometry::Mesh>(
        "../asset/models/cornellbox/light.obj",
        nullptr);

    auto light = std::make_unique<danny::light::DiffuseArealight>((8.0f * glm::vec3(0.747f + 0.058f, 0.747f + 0.258f, 0.747f) +
                                                                   15.6f * glm::vec3(0.740f + 0.287f, 0.740f + 0.160f, 0.740f) +
                                                                   18.4f * glm::vec3(0.737f + 0.642f, 0.737f + 0.159f, 0.737f)),
                                                                  std::move(light_obj));

    auto sphere_0_15 = std::make_unique<danny::geometry::Sphere>(glm::vec3(250, 100, 200), 100, metal_0_15);
    auto sphere_0 = std::make_unique<danny::geometry::Sphere>(glm::vec3(150, 100, 200), 100, metal_0);
    auto sphere_interval = std::make_unique<danny::geometry::Sphere>(glm::vec3(150, 100, 200), 100, interval_diffuse);
    auto sphere_cloth = std::make_unique<danny::geometry::Sphere>(glm::vec3(150, 100, 200), 100, cloth_material);

    auto resolution = glm::ivec2(783, 784);
    float near_distance = 10.f;
    auto fov = glm::vec2(40);
    auto pos = glm::vec3(278, 278, -800);
    auto dir = glm::vec3(0, 0, 1);
    auto up = glm::vec3(0, 1, 0);
    auto camera = std::make_unique<danny::core::PinholeCamera>(resolution,
                                                               near_distance,
                                                               fov,
                                                               pos,
                                                               dir,
                                                               up);
    int spp = 128;
    float cutoff_rate = 0.2f;
    int thread_num = 16;
    auto integrator = std::make_unique<danny::integrator::Pathtracer>(spp, cutoff_rate, thread_num);

    float secondary_ray_epsilon = 0.01;
    danny::core::Scene scene(std::move(integrator), std::move(camera), secondary_ray_epsilon);

    scene.addLight(std::move(light));
    scene.addObject(std::move(floor));
    // scene.addObject(std::move(shortbox));
    // scene.addObject(std::move(tallbox));
    // scene.addObject(std::move(shortbox_metal_0));
    // scene.addObject(std::move(tallbox_metal_0));
    scene.addObject(std::move(left));
    scene.addObject(std::move(right));
    scene.addObject(std::move(cloth_obj));
    scene.addObject(std::move(sphere_0_15));
    // scene.addObject(std::move(sphere_interval));
    // scene.addObject(std::move(sphere_cloth));

    scene.buildBVH();

    std::string other_info = "布料-interval-spp";

    std::ostringstream oss;
    oss << spp << "_" << other_info << spp
        << ".png";

    scene.render("../result/cache/", oss.str());
}
