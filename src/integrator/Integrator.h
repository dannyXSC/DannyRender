#ifndef __DANNY__INTEGRATOR__INTEGRATOR__
#define __DANNY__INTEGRATOR__INTEGRATOR__

#include <core/Scene.h>
#include <core/Image.h>

namespace danny
{
    namespace integrator
    {
        class Integrator
        {
        public:
            virtual ~Integrator() {}

            virtual void integrate(const core::Scene &scene, core::Image &output) = 0;
        };
    }
}

#endif