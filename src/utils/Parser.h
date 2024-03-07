#ifndef __DANNY__XML__PARSER__
#define __DANNY__XML__PARSER__
#include <core/forward_decl.h>
#include <geometry/BVH.h>
#include <geometry/Triangle.hpp>

#include <memory>

namespace danny
{
    namespace utils
    {
        class Parser
        {
        public:
            static std::shared_ptr<geometry::BVH<geometry::Triangle>> loadModel(const std::string &path);
        };
    }
}
#endif