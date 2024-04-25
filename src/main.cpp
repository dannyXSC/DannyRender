#include <iostream>
#include <memory>
#include <filesystem>

namespace fs = std::filesystem;

#include <sstream>
#include <fstream>

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

#include <texture/ImageTexture.h>

int run_batch()
{
    std::string xml_info1 = R"(<Scene>
    <SecondaryRayEpsilon>0.1</SecondaryRayEpsilon>
    <Integrator type='Pathtracer'>
        <SampleCount>2048</SampleCount>
        <CutoffProbability>0.2</CutoffProbability>
    </Integrator>

    <Output type='Ldr'>
        <Path>../result/cache/metal-2048-0_)";
    std::string xml_info2 = R"(</Path>
        <Format>png</Format>
        <Tonemapper type='GlobalReinhard'>
            <Key>0.18</Key>
            <MaxLuminance>1.0</MaxLuminance>
        </Tonemapper>
    </Output>

    <Camera type='Pinhole'>
        <Position>278 273 -800</Position>
        <Direction>0 0 1</Direction>
        <Up>0 1 0</Up>
        <FovXY>39.3077 39.3077</FovXY>
        <Resolution>783 784</Resolution>
        <NearDistance>10</NearDistance>
    </Camera>

    <Object type='Mesh'>
        <Datapath>../asset/models/cornellbox/floor.obj</Datapath>
        <BsdfMaterial type='Diffuse'>
            <Kd textureType='Constant'>
                <Value>0.708 0.721 0.758</Value>
            </Kd>
        </BsdfMaterial>
    </Object>

    <Object type='Mesh'>
        <Datapath>../asset/models/cornellbox/right.obj</Datapath>
        <BsdfMaterial type='Diffuse'>
            <Kd textureType='Constant'>
                <Value>0.708 0.721 0.758</Value>
            </Kd>
        </BsdfMaterial>
    </Object>

    <Object type='Mesh'>
        <Datapath>../asset/models/cornellbox/left.obj</Datapath>
        <BsdfMaterial type='Diffuse'>
            <Kd textureType='Constant'>
                <Value>0.708 0.721 0.758</Value>
            </Kd>
        </BsdfMaterial>
    </Object>

    <Object type="Sphere">
        <Radius>100</Radius>
        <Center>150 100 200</Center>
        <BsdfMaterial type='Metal'>
            <Fresnel type='Conductor'>
        )";
    std::string xml_info3 = R"(
            </Fresnel>
            <PrecomputedE textureType='Image'>
                <Datapath>../asset/precomputed/GGX_E_LUT.png</Datapath>
            </PrecomputedE>
            <PrecomputedEAvg textureType='Image'>
                <Datapath>../asset/precomputed/GGX_Eavg_LUT.png</Datapath>
            </PrecomputedEAvg>

            <Roughness textureType='Constant'>
                <Value>)";
    std::string xml_info4 = R"( 0 0</Value>
            </Roughness>
        </BsdfMaterial>
    </Object>

    <Light type='DiffuseArealight'>
        <Flux>500000 500000 500000</Flux>
        <Object type='Mesh'>
            <Datapath>../asset/models/cornellbox/light.obj</Datapath>
        </Object>
    </Light>

</Scene>)";

    glm::vec3 N(0.131, 0.44715, 1.4215), K(4.0624, 2.4212, 1.9342);
    for (float i = 0.5; i < 6.0; i += 0.5)
    {
        std::stringstream ss;
        ss << xml_info1 << i * 10 << xml_info2
           << "<N>" << N.x << " " << N.y << " " << N.z << "</N>\n"
           << "<K>" << K.x << " " << K.y << " " << K.z << "</K>\n"
           << xml_info3 << i / 10.f << xml_info4;
        auto xml_info = ss.str();
        std::ofstream myfile;
        myfile.open("./tmp.xml");
        myfile << xml_info;
        myfile.close();

        using namespace danny;
        core::Timer timer;

        timer.start();
        core::Scene scene(core::Scene::Xml(xml::Node::getRoot("./tmp.xml")));
        std::cout << "BVH build and input read time: " << timer.getTime() << std::endl;

        scene.render();
    }
    return 0;
}

int run(int argc, char *argv[])
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

#include <texture/ImageTexture.h>
void test()
{
    using namespace danny;
    auto texture = std::make_unique<texture::ImageTexture>("../asset/precomputed/GGX_E_LUT.png");
    geometry::Intersection inter;
    inter.uv = glm::vec2(0.9999, 0.9999);
    auto result = texture->fetch(inter);
    cout << glm::to_string(result) << endl;
}

int main(int argc, char *argv[])
{
    run(argc, argv);
    // run_batch();
    // test();
    return 0;
}
