#ifndef __DANNY__GEOMETRY__INTERSECTION__
#define __DANNY__GEOMETRY__INTERSECTION__
#include <core/forward_decl.h>

#include <glm/vec3.hpp>
#include <glm/vec2.hpp>

#include <geometry/Object.hpp>
#include <geometry/Plane.hpp>
#include <geometry/Triangle.hpp>
#include <material/BsdfMaterial.hpp>

namespace danny
{
    namespace geometry
    {
        struct Intersection
        {
            // mainly copy from https://github.com/isikmustafa/glue with a little modification
            Plane plane;
            // texture 上的坐标
            glm::vec2 uv;
            // texture坐标轴u在世界坐标系上的的方向
            glm::vec3 dpdu;
            // texture坐标轴v在世界坐标系上的的方向
            glm::vec3 dpdv;
            // texture坐标轴u在局部系上的的方向
            glm::vec3 dpdu_local;
            // texture坐标轴v在局部系上的的方向
            glm::vec3 dpdv_local;
            float distance{-1.0f};
            int bsdf_choice{-1};
            const Object *object{nullptr};
            const Triangle *triangle{nullptr};
            const material::BsdfMaterial *bsdf_material{nullptr};
            bool radiance_transport{true};
        };
    }
}

#endif