#ifndef __DANNY__CORE__SAMPLER__
#define __DANNY__CORE__SAMPLER__
#include <core/forward_decl.h>

#include <random>
#include <map>
#include <utility>
#include <memory>
namespace danny
{
    namespace core
    {
        class RealSampler
        {
        public:
            virtual ~RealSampler() = default;

            virtual float sample() = 0;
        };

        class UniformSampler : public RealSampler
        {
        public:
            static std::shared_ptr<UniformSampler> getInstance(float min = 0.0f, float max = 1.0f)
            {
                static std::map<std::pair<float, float>, std::shared_ptr<UniformSampler>> instance_pool;

                std::pair<float, float> p = std::make_pair(min, max);
                if (instance_pool.count(p) > 0)
                {
                    return instance_pool[p];
                }
                std::shared_ptr<UniformSampler> s(new UniformSampler(min, max));
                instance_pool.emplace(p, s);
                return s;
            }

        public:
            float sample() override;

        private:
            UniformSampler(float min = 0.0f, float max = 1.0f);

            UniformSampler(UniformSampler const &);
            void operator=(UniformSampler const &);

        private:
            std::mt19937 m_generator;
            std::uniform_real_distribution<float> m_distribution;
        };
    }
}
#endif