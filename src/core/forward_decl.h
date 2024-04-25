#ifndef __DANNY__CORE__FORWARDDECL__
#define __DANNY__CORE__FORWARDDECL__

namespace danny
{
    namespace core
    {
        class Camera;
        class CoordinateSpace;
        class Discrete1DSampler;
        class Image;
        class Output;
        class PinholeCamera;
        class RealSampler;
        class UniformSampler;
        class Scene;
        class Timer;
        class Tonemapper;
    }

    namespace geometry
    {
        class BBox;
        struct Intersection;
        class Mapper;
        class UVMapper;
        class SphericalMapper;
        class Mesh;
        class Object;
        class Plane;
        class Ray;
        class Sphere;
        struct SphericalCoordinate;
        class Transformation;
        class Triangle;
    }

    namespace integrator
    {
        class Integrator;
        class Pathtracer;

    }

    namespace light
    {
        struct LightSample;
        class Light;
        class DiffuseArealight;
    }

    namespace material
    {
        class BsdfMaterial;
        class Metal;
        class Diffuse;
        class Cloth;
        struct ClothPara;
    }

    namespace microfacet
    {
        class GGXDistribution;
        class Fresnel;
        class MicrocylinderCloth;
        class Microfacet;
        class MicrofacetReflection;
        class KullaConty;
    }

    namespace xml
    {
        class Parser;
    }

    namespace texture
    {
        class Texture;
        class ConstantTexture;
    }
}

#endif