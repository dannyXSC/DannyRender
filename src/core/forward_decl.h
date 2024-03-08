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
        class PinholeCamera;
        class RealSampler;
        class UniformSampler;
        class Scene;
        class Timer;
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
    }

    namespace utils
    {
        class Parser;
    }
}

#endif