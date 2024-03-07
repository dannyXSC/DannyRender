#ifndef __DANNY__GEOMETRY__BVH__
#define __DANNY__GEOMETRY__BVH__
#include <core/forward_decl.h>

#include <vector>
#include <memory>

#include <geometry/BBox.hpp>

namespace danny
{
    namespace geometry
    {
        struct BVHNode
        {
            BBox bbox;
            std::unique_ptr<BVHNode> left{nullptr};
            std::unique_ptr<BVHNode> right{nullptr};
            int start;
            int end;

            BVHNode() = default;
            BVHNode(int p_start, int p_end)
                : start(p_start), end(p_end)
            {
            }
            BVHNode(const BBox &p_bbox, int p_start, int p_end)
                : bbox(p_bbox), start(p_start), end(p_end)
            {
            }
        };

        template <typename Primitive>
        class BVH
        {
        public:
            void addObject(Primitive primitive);
            void buildWithMedianSplit();
            void buildWithSAHSplit();
            bool intersect(const Ray &ray, Intersection &intersection, float max_distance) const;
            bool intersectShadowRay(const Ray &ray, float max_distance) const;

            const std::vector<Primitive> &get_objects() const { return m_objects; }
            const std::unique_ptr<BVHNode> &get_root() const { return m_root; }

        private:
            std::vector<Primitive> m_objects;
            std::unique_ptr<BVHNode> m_root;

        private:
            // 多线程加载
            void buildWithMedianSplitWork(std::unique_ptr<BVHNode> *node, int work = 1);
            void buildWithSAHSplitWork(std::unique_ptr<BVHNode> *node, float work = 1);
        };
    }
}

#include <geometry/BVH.inl>
#endif