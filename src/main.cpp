#include <iostream>
#include <memory>
#include <filesystem>

namespace fs = std::filesystem;

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/vec3.hpp>
#include <glm/ext.hpp>

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
#include <xml/Node.h>
#include <core/Timer.h>

using std::cout;
using std::endl;

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::cout << "Please indicate input location as the first argument of the program." << std::endl;
        std::cout << "Do not add any other program arguments." << std::endl;
        return 0;
    }

    try
    {
        using namespace danny;
        core::Timer timer;

        timer.start();
        core::Scene scene(core::Scene::Xml(xml::Node::getRoot(argv[1])));
        std::cout << "BVH build and input read time: " << timer.getTime() << std::endl;

        scene.render();
    }
    catch (const std::runtime_error &e)
    {
        std::cout << e.what() << std::endl;
    }

    return 0;
}
