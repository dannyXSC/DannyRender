#ifndef __DANNY__CORE__FORWARDDECL__
#define __DANNY__CORE__FORWARDDECL__

namespace danny
{
    namespace core
    {
        class Camera;
        class CoordinateSpace;
        class Discrete1DSampler;
        class PinholeCamera;
        class RealSampler;
        class UniformSampler;
        class Scene;
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