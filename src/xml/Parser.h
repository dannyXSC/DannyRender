#ifndef __DANNY__XML__PARSER__
#define __DANNY__XML__PARSER__
#include <core/forward_decl.h>
#include <geometry/BVH.h>
#include <geometry/Triangle.hpp>

#include <memory>
#include <unordered_set>

#include <core/Image.h>

namespace danny
{
    namespace xml
    {
        class Parser
        {
        public:
            static const std::unordered_set<std::string> gSupportedFormatsLoad;
            static const std::unordered_set<std::string> gSupportedFormatsSave;

        public:
            static std::shared_ptr<geometry::BVH<geometry::Triangle>> loadModel(const std::string &path);
            static std::shared_ptr<geometry::BVH<geometry::Triangle>> loadModel(const std::string &path, const std::string &object_name);
            static std::shared_ptr<std::vector<core::Image>> loadImage(const std::string &path, bool mipmapping = false);
        };
    }
}
#endif