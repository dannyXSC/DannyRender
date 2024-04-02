#include <iostream>
#include <memory>
#include <filesystem>

namespace fs = std::filesystem;

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
#include <core/Output.h>

#include <glm/ext.hpp>

std::pair<danny::material::ClothPara, danny::material::ClothPara> getCloth1()
{
    danny::material::ClothPara cloth_para1;
    cloth_para1.albedo = glm::vec3(0.2, 0.8, 1) * 0.3;
    cloth_para1.gamma_s = 12.f;
    cloth_para1.gamma_v = 24.f;
    cloth_para1.k_d = 0.3f;
    cloth_para1.eta = 1.46f;
    cloth_para1.alpha = 0.33f;
    cloth_para1.tangent_offsets = {-25, 25};
    return std::make_pair(cloth_para1, cloth_para1);
}

std::pair<danny::material::ClothPara, danny::material::ClothPara> getCloth2()
{
    danny::material::ClothPara cloth_para1;
    cloth_para1.albedo = glm::vec3(1, 0.95, 0.05) * 0.12;
    cloth_para1.gamma_s = 5.f;
    cloth_para1.gamma_v = 10.f;
    cloth_para1.k_d = 0.2f;
    cloth_para1.eta = 1.345f;
    cloth_para1.alpha = 0.75f;
    cloth_para1.tangent_offsets = {-35, -35, 35, 35};

    danny::material::ClothPara cloth_para2;
    cloth_para2.albedo = glm::vec3(1, 0.95, 0.05) * 0.16;
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
    cloth_para1.albedo = glm::vec3(1, 0.37, 0.3) * 0.035;
    cloth_para1.gamma_s = 2.5f;
    cloth_para1.gamma_v = 5.f;
    cloth_para1.k_d = 0.1f;
    cloth_para1.eta = 1.539f;
    cloth_para1.alpha = 0.9f;
    cloth_para1.tangent_offsets = {-32, -32, -18, 0, 0, 18, 32, 32};

    danny::material::ClothPara cloth_para2;
    cloth_para2.albedo = glm::vec3(1, 0.37, 0.3) * 0.2;
    cloth_para2.gamma_s = 30.f;
    cloth_para2.gamma_v = 60.f;
    cloth_para2.k_d = 0.7f;
    cloth_para2.eta = 1.539f;
    cloth_para2.alpha = 0.1f;
    cloth_para2.tangent_offsets = {0, 0};

    return std::make_pair(cloth_para1, cloth_para2);
}

std::pair<danny::material::ClothPara, danny::material::ClothPara> getCloth4()
{
    danny::material::ClothPara cloth_para1;
    cloth_para1.albedo = glm::vec3(1, 0.37, 0.3) * 0.035;
    cloth_para1.gamma_s = 2.5f;
    cloth_para1.gamma_v = 5.f;
    cloth_para1.k_d = 0.1f;
    cloth_para1.eta = 1.539f;
    cloth_para1.alpha = 0.67f;
    cloth_para1.tangent_offsets = {-30, -30, 30, 30, -5, -5, 5, 5};

    danny::material::ClothPara cloth_para2;
    cloth_para2.albedo = glm::vec3(1, 0.37, 0.3) * 0.2;
    cloth_para2.gamma_s = 30.f;
    cloth_para2.gamma_v = 60.f;
    cloth_para2.k_d = 0.7f;
    cloth_para2.eta = 1.539f;
    cloth_para2.alpha = 0.33f;
    cloth_para2.tangent_offsets = {0, 0};

    return std::make_pair(cloth_para1, cloth_para2);
}

std::pair<danny::material::ClothPara, danny::material::ClothPara> getCloth5()
{
    danny::material::ClothPara cloth_para1;
    cloth_para1.albedo = glm::vec3(0.1, 1, 0.4) * 0.2;
    cloth_para1.gamma_s = 4.f;
    cloth_para1.gamma_v = 8.f;
    cloth_para1.k_d = 0.1f;
    cloth_para1.eta = 1.345f;
    cloth_para1.alpha = 0.86f;
    cloth_para1.tangent_offsets = {-25, -25, 25, 25};

    danny::material::ClothPara cloth_para2;
    cloth_para2.albedo = glm::vec3(1, 0, 0.1) * 0.6;
    cloth_para2.gamma_s = 5.f;
    cloth_para2.gamma_v = 10.f;
    cloth_para2.k_d = 0.1f;
    cloth_para2.eta = 1.345f;
    cloth_para2.alpha = 0.14f;
    cloth_para2.tangent_offsets = {0, 0};

    return std::make_pair(cloth_para1, cloth_para2);
}

std::pair<danny::material::ClothPara, danny::material::ClothPara> getCloth6()
{
    danny::material::ClothPara cloth_para1;
    cloth_para1.albedo = glm::vec3(0.05, 0.02, 0) * 0.3;
    cloth_para1.gamma_s = 6.f;
    cloth_para1.gamma_v = 12.f;
    cloth_para1.k_d = 0.1f;
    cloth_para1.eta = 1.46f;
    cloth_para1.alpha = 0.5f;
    cloth_para1.tangent_offsets = {-90, -50};

    danny::material::ClothPara cloth_para2;
    cloth_para2.albedo = glm::vec3(0.05, 0.02, 0) * 0.3;
    cloth_para2.gamma_s = 6.f;
    cloth_para2.gamma_v = 12.f;
    cloth_para2.k_d = 0.1f;
    cloth_para2.eta = 1.46f;
    cloth_para2.alpha = 0.5f;
    cloth_para2.tangent_offsets = {-90, -55, 55, 90};

    return std::make_pair(cloth_para1, cloth_para2);
}

void generateScene(const std::string &name)
{
    auto red = std::make_shared<danny::texture::ConstantTexture>(glm::vec3(0.63f, 0.065f, 0.05f));
    auto green = std::make_shared<danny::texture::ConstantTexture>(glm::vec3(0.14f, 0.45f, 0.091f));
    auto white = std::make_shared<danny::texture::ConstantTexture>(glm::vec3(0.725f, 0.71f, 0.68f));
    auto blue = std::make_shared<danny::texture::ConstantTexture>(glm::vec3(0.14f, 0.05f, 0.45f));
    auto test = std::make_shared<danny::texture::ConstantTexture>(glm::vec3(0.14f, 0.05f, 0.45f));
    auto interval = std::make_shared<danny::texture::IntervalTexture>();

    auto red_diffuse = std::make_shared<danny::material::Diffuse>(red);
    auto green_diffuse = std::make_shared<danny::material::Diffuse>(green);
    auto white_diffuse = std::make_shared<danny::material::Diffuse>(white);
    auto blue_diffuse = std::make_shared<danny::material::Diffuse>(blue);
    auto interval_diffuse = std::make_shared<danny::material::Diffuse>(interval);
    auto test_material = std::make_shared<danny::material::Diffuse>(test);

    auto roughness_0_15 = std::make_shared<danny::texture::ConstantTexture>(glm::vec3(0.15, 0, 0));
    auto metal_0_15 = std::make_shared<danny::material::Metal>(roughness_0_15, 4.);

    auto roughness_0 = std::make_shared<danny::texture::ConstantTexture>(glm::vec3(0., 0, 0));
    auto metal_0 = std::make_shared<danny::material::Metal>(roughness_0, 4.);

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

    auto light = std::make_unique<danny::light::DiffuseArealight>(glm::vec3(500000, 500000, 500000),
                                                                  std::move(light_obj));
    auto sphere_0_15 = std::make_unique<danny::geometry::Sphere>(glm::vec3(250, 100, 200), 100, metal_0_15);
    auto sphere_0 = std::make_unique<danny::geometry::Sphere>(glm::vec3(150, 100, 200), 100, metal_0);
    auto sphere_interval = std::make_unique<danny::geometry::Sphere>(glm::vec3(150, 100, 200), 100, interval_diffuse);
    auto sphere_test = std::make_unique<danny::geometry::Sphere>(glm::vec3(150, 100, 200), 100, test_material);

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
    int spp = 1024;
    float cutoff_rate = 0.2f;
    int thread_num = 16;
    auto integrator = std::make_unique<danny::integrator::Pathtracer>(spp, cutoff_rate, thread_num);

    std::vector<std::unique_ptr<danny::core::Output>> outputs;
    outputs.emplace_back(std::make_unique<danny::core::Ldr>(name, std::make_unique<danny::core::GlobalReinhard>(0.18, 1.0)));
    // danny::core::Ldr output(root_path, std::make_unique<danny::core::Clamp>(0.f, 1.0f));

    float secondary_ray_epsilon = 0.01;
    danny::core::Scene scene(std::move(integrator), std::move(camera), outputs, secondary_ray_epsilon);

    scene.addLight(std::move(light));
    scene.addObject(std::move(floor));
    // scene.addObject(std::move(shortbox));
    // scene.addObject(std::move(tallbox));
    // scene.addObject(std::move(shortbox_metal_0));
    // scene.addObject(std::move(tallbox_metal_0));
    scene.addObject(std::move(left));
    scene.addObject(std::move(right));
    scene.addObject(std::move(sphere_0_15));
    // scene.addObject(std::move(sphere_interval));
    // scene.addObject(std::move(sphere_cloth));
    // scene.addObject(std::move(sphere_test));

    scene.buildBVH();

    scene.render();
}

void generateScene1(const danny::material::ClothPara &cloth_para1, const danny::material::ClothPara &cloth_para2, const std::string &name)
{
    auto red = std::make_shared<danny::texture::ConstantTexture>(glm::vec3(0.63f, 0.065f, 0.05f));
    auto green = std::make_shared<danny::texture::ConstantTexture>(glm::vec3(0.14f, 0.45f, 0.091f));
    auto white = std::make_shared<danny::texture::ConstantTexture>(glm::vec3(0.725f, 0.71f, 0.68f));
    auto blue = std::make_shared<danny::texture::ConstantTexture>(glm::vec3(0.14f, 0.05f, 0.45f));
    auto test = std::make_shared<danny::texture::ConstantTexture>(glm::vec3(0.14f, 0.05f, 0.45f));
    auto interval = std::make_shared<danny::texture::IntervalTexture>();

    auto red_diffuse = std::make_shared<danny::material::Diffuse>(red);
    auto green_diffuse = std::make_shared<danny::material::Diffuse>(green);
    auto white_diffuse = std::make_shared<danny::material::Diffuse>(white);
    auto blue_diffuse = std::make_shared<danny::material::Diffuse>(blue);
    auto interval_diffuse = std::make_shared<danny::material::Diffuse>(interval);
    auto cloth_material = std::make_shared<danny::material::Cloth>(cloth_para1, cloth_para2);
    auto test_material = std::make_shared<danny::material::Diffuse>(test);

    auto roughness_0_15 = std::make_shared<danny::texture::ConstantTexture>(glm::vec3(0.15, 0, 0));
    auto metal_0_15 = std::make_shared<danny::material::Metal>(roughness_0_15, 4.);

    auto roughness_0 = std::make_shared<danny::texture::ConstantTexture>(glm::vec3(0., 0, 0));
    auto metal_0 = std::make_shared<danny::material::Metal>(roughness_0, 4.);

    auto cloth_obj = std::make_unique<danny::geometry::Mesh>(
        "../asset/models/cloth/000026.obj",
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

    // auto light = std::make_unique<danny::light::DiffuseArealight>((8.0f * glm::vec3(0.747f + 0.058f, 0.747f + 0.258f, 0.747f) +
    //                                                                15.6f * glm::vec3(0.740f + 0.287f, 0.740f + 0.160f, 0.740f) +
    //                                                                18.4f * glm::vec3(0.737f + 0.642f, 0.737f + 0.159f, 0.737f)),
    //                                                               std::move(light_obj));
    auto light = std::make_unique<danny::light::DiffuseArealight>(glm::vec3(500000, 500000, 500000),
                                                                  std::move(light_obj));
    auto sphere_0_15 = std::make_unique<danny::geometry::Sphere>(glm::vec3(250, 100, 200), 100, metal_0_15);
    auto sphere_0 = std::make_unique<danny::geometry::Sphere>(glm::vec3(150, 100, 200), 100, metal_0);
    auto sphere_interval = std::make_unique<danny::geometry::Sphere>(glm::vec3(150, 100, 200), 100, interval_diffuse);
    auto sphere_cloth = std::make_unique<danny::geometry::Sphere>(glm::vec3(150, 100, 200), 100, cloth_material);
    auto sphere_test = std::make_unique<danny::geometry::Sphere>(glm::vec3(150, 100, 200), 100, test_material);

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
    int spp = 16;
    float cutoff_rate = 0.2f;
    int thread_num = 16;
    auto integrator = std::make_unique<danny::integrator::Pathtracer>(spp, cutoff_rate, thread_num);

    std::vector<std::unique_ptr<danny::core::Output>> outputs;
    outputs.emplace_back(std::make_unique<danny::core::Ldr>(name, std::make_unique<danny::core::GlobalReinhard>(0.18, 1.0)));
    // danny::core::Ldr output(root_path, std::make_unique<danny::core::Clamp>(0.f, 1.0f));

    float secondary_ray_epsilon = 0.01;
    danny::core::Scene scene(std::move(integrator), std::move(camera), outputs, secondary_ray_epsilon);

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
    // scene.addObject(std::move(sphere_test));

    scene.buildBVH();

    scene.render();
}

void generateScene2()
{
    auto [cloth_para1, cloth_para2] = getCloth1();

    auto white = std::make_shared<danny::texture::ConstantTexture>(glm::vec3(0.708f, 0.721f, 0.758f));
    auto white_diffuse = std::make_shared<danny::material::Diffuse>(white);
    auto cloth_material = std::make_shared<danny::material::Cloth>(cloth_para1, cloth_para2);

    auto wall0 = std::make_unique<danny::geometry::Mesh>(
        "../asset/models/mc_cloth/wall-0.obj",
        white_diffuse);
    auto wall1 = std::make_unique<danny::geometry::Mesh>(
        "../asset/models/mc_cloth/wall-1.obj",
        white_diffuse);
    auto wall2 = std::make_unique<danny::geometry::Mesh>(
        "../asset/models/mc_cloth/wall-2.obj",
        white_diffuse);
    auto wall3 = std::make_unique<danny::geometry::Mesh>(
        "../asset/models/mc_cloth/wall-3.obj",
        white_diffuse);
    auto wall4 = std::make_unique<danny::geometry::Mesh>(
        "../asset/models/mc_cloth/wall-4.obj",
        white_diffuse);
    auto cloth_obj = std::make_unique<danny::geometry::Mesh>(
        "../asset/models/mc_cloth/cloth.obj",
        cloth_material);

    auto light_obj = std::make_unique<danny::geometry::Mesh>(
        "../asset/models/mc_cloth/light.obj",
        nullptr);

    auto light = std::make_unique<danny::light::DiffuseArealight>(glm::vec3(1.0) * 20.f,
                                                                  std::move(light_obj));

    auto resolution = glm::ivec2(512, 512);
    float near_distance = 1.f;
    auto fov = glm::vec2(90);
    auto pos = glm::vec3(0, 10, 20);
    auto dir = glm::vec3(0, 0, -1);
    auto up = glm::vec3(0, 1, 0);
    auto camera = std::make_unique<danny::core::PinholeCamera>(resolution,
                                                               near_distance,
                                                               fov,
                                                               pos,
                                                               dir,
                                                               up);

    int spp = 16;
    float cutoff_rate = 0.2f;
    int thread_num = 16;
    auto integrator = std::make_unique<danny::integrator::Pathtracer>(spp, cutoff_rate, thread_num);

    std::string other_info = "mc_cloth1";

    std::ostringstream oss;
    oss << "../result/cache" << spp << "_" << other_info
        << ".png";

    std::vector<std::unique_ptr<danny::core::Output>> outputs;
    outputs.emplace_back(std::make_unique<danny::core::Ldr>(other_info, std::make_unique<danny::core::GlobalReinhard>(0.18, 1.0)));

    float secondary_ray_epsilon = 0.01;
    danny::core::Scene scene(std::move(integrator), std::move(camera), outputs, secondary_ray_epsilon);

    scene.addLight(std::move(light));
    scene.addObject(std::move(wall0));
    scene.addObject(std::move(wall1));
    scene.addObject(std::move(wall2));
    scene.addObject(std::move(wall3));
    scene.addObject(std::move(wall4));
    scene.addObject(std::move(cloth_obj));

    scene.buildBVH();

    scene.render();
}

void generateSceneCloth(const std::string &obj_path,
                        const std::string &name,
                        const danny::material::ClothPara &cloth_para1,
                        const danny::material::ClothPara &cloth_para2)
{
    auto red = std::make_shared<danny::texture::ConstantTexture>(glm::vec3(0.63f, 0.065f, 0.05f));
    auto green = std::make_shared<danny::texture::ConstantTexture>(glm::vec3(0.14f, 0.45f, 0.091f));
    auto white = std::make_shared<danny::texture::ConstantTexture>(glm::vec3(0.725f, 0.71f, 0.68f));
    auto roughness_0_15 = std::make_shared<danny::texture::ConstantTexture>(glm::vec3(0.15, 0, 0));

    auto red_diffuse = std::make_shared<danny::material::Diffuse>(red);
    auto green_diffuse = std::make_shared<danny::material::Diffuse>(green);
    auto white_diffuse = std::make_shared<danny::material::Diffuse>(white);
    auto cloth_material = std::make_shared<danny::material::Cloth>(cloth_para1, cloth_para2);
    auto metal_0_15 = std::make_shared<danny::material::Metal>(roughness_0_15, 4.);

    auto cloth_obj = std::make_unique<danny::geometry::Mesh>(
        obj_path,
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
    auto left = std::make_unique<danny::geometry::Mesh>(
        "../asset/models/cornellbox/left.obj",
        red_diffuse);
    auto right = std::make_unique<danny::geometry::Mesh>(
        "../asset/models/cornellbox/right.obj",
        green_diffuse);

    auto light_obj = std::make_unique<danny::geometry::Mesh>(
        "../asset/models/cornellbox/light.obj",
        nullptr);

    auto light = std::make_unique<danny::light::DiffuseArealight>(glm::vec3(500000, 500000, 500000),
                                                                  std::move(light_obj));
    auto sphere_0_15 = std::make_unique<danny::geometry::Sphere>(glm::vec3(250, 100, 200), 100, metal_0_15);

    auto resolution = glm::ivec2(512, 512);
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
    int spp = 1024;
    float cutoff_rate = 0.2f;
    int thread_num = 16;
    auto integrator = std::make_unique<danny::integrator::Pathtracer>(spp, cutoff_rate, thread_num);

    std::vector<std::unique_ptr<danny::core::Output>> outputs;
    outputs.emplace_back(std::make_unique<danny::core::Ldr>(name, std::make_unique<danny::core::GlobalReinhard>(0.18, 1.0)));

    float secondary_ray_epsilon = 0.01;
    danny::core::Scene scene(std::move(integrator), std::move(camera), outputs, secondary_ray_epsilon);

    scene.addLight(std::move(light));
    scene.addObject(std::move(floor));
    scene.addObject(std::move(left));
    scene.addObject(std::move(right));
    scene.addObject(std::move(cloth_obj));
    scene.addObject(std::move(sphere_0_15));

    scene.buildBVH();

    scene.render();
}

// void runGenerateCloth()
// {
//     // std::string obj_save_path = "/share/test/scxie/cloth_simulater";
//     // std::vector<std::string> sub_name = {"GSSimulator-n128-w400-texture",
//     //                                      "JacobiSimulator-n128-w400-texture",
//     //                                      "CGSimulator-n128-w400-texture"};
//     std::string obj_save_path = "/mnt/e/数据/cloth_simulator";
//     std::vector<std::string> sub_name = {"JacobiSimulator-n128-w400-texture"};

//     std::string image_save_path = "/mnt/e/数据/cloth_render";
//     fs::create_directory(image_save_path);

//     std::vector<danny::material::ClothPara> paras1, paras2;

//     {
//         auto [para1, para2] = getCloth1();
//         paras1.push_back(para1);
//         paras2.push_back(para2);
//     }
//     {
//         auto [para1, para2] = getCloth2();
//         paras1.push_back(para1);
//         paras2.push_back(para2);
//     }
//     {
//         auto [para1, para2] = getCloth3();
//         paras1.push_back(para1);
//         paras2.push_back(para2);
//     }
//     {
//         auto [para1, para2] = getCloth4();
//         paras1.push_back(para1);
//         paras2.push_back(para2);
//     }
//     {
//         auto [para1, para2] = getCloth5();
//         paras1.push_back(para1);
//         paras2.push_back(para2);
//     }
//     {
//         auto [para1, para2] = getCloth6();
//         paras1.push_back(para1);
//         paras2.push_back(para2);
//     }

//     for (auto sub_dir : sub_name)
//     {
//         auto image_sub = image_save_path + "/" + sub_dir;
//         fs::create_directory(image_sub);

//         for (int j = 0; j < 6; j++)
//         {
//             std::ostringstream cloth_oss;
//             cloth_oss << image_sub << "/Cloth" << j;
//             auto cloth_dir = cloth_oss.str();
//             fs::create_directory(cloth_dir);

//             for (int i = 20; i < 140; i++)
//             {
//                 std::ostringstream oss;
//                 oss << obj_save_path << "/"
//                     << sub_dir << "/"
//                     << "mesh/"
//                     << std::setw(6) << std::setfill('0') << i << ".obj";
//                 auto obj_path = oss.str();

//                 std::ostringstream cur_oss;
//                 cur_oss << cloth_dir << "/"
//                         << std::setw(6) << std::setfill('0') << i
//                         << ".png";
//                 auto save_path = cur_oss.str();

//                 std::cout << std::endl
//                           << save_path << " start" << std::endl;

//                 generateSceneCloth(obj_path, save_path, paras1[j], paras2[j]);
//             }
//         }
//     }
// }

int main()
{
    generateScene("./test.png");
    // auto [cloth1, cloth2] = getCloth1();
    // generateScene1(cloth1, cloth2, "./test.png");
    // runGenerateCloth();
}