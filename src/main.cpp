#include <iostream>
#include <memory>

#include <glm/vec3.hpp>

#include <core/Scene.h>
#include <material/Diffuse.hpp>
#include <texture/ConstantTexture.h>
#include <texture/Texture.h>
#include <geometry/Mesh.hpp>
#include <light/DiffuseArealight.h>
#include <core/PinholeCamera.h>
#include <integrator/Pathtracer.h>
#define GLM_ENABLE_EXPERIMENTAL

#include <glm/ext.hpp>
int main()
{
    auto red = std::make_shared<danny::texture::ConstantTexture>(glm::vec3(0.63f, 0.065f, 0.05f));
    auto green = std::make_shared<danny::texture::ConstantTexture>(glm::vec3(0.14f, 0.45f, 0.091f));
    auto white = std::make_shared<danny::texture::ConstantTexture>(glm::vec3(0.725f, 0.71f, 0.68f));

    auto red_diffuse = std::make_shared<danny::material::Diffuse>(red);
    auto green_diffuse = std::make_shared<danny::material::Diffuse>(green);
    auto white_diffuse = std::make_shared<danny::material::Diffuse>(white);

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
    auto light = std::make_unique<danny::light::DiffuseArealight>((8.0f * glm::vec3(0.747f + 0.058f, 0.747f + 0.258f, 0.747f) +
                                                                   15.6f * glm::vec3(0.740f + 0.287f, 0.740f + 0.160f, 0.740f) +
                                                                   18.4f * glm::vec3(0.737f + 0.642f, 0.737f + 0.159f, 0.737f)),
                                                                  std::move(light_obj));

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
    auto integrator = std::make_unique<danny::integrator::Pathtracer>(16, 0.2f);

    danny::core::Scene scene(std::move(integrator), std::move(camera));

    scene.addLight(std::move(light));
    scene.addObject(std::move(floor));
    scene.addObject(std::move(shortbox));
    scene.addObject(std::move(tallbox));
    scene.addObject(std::move(left));
    scene.addObject(std::move(right));

    scene.buildBVH();

    scene.render();
}
