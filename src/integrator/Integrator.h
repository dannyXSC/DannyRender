#ifndef __DANNY__INTEGRATOR__INTEGRATOR__
#define __DANNY__INTEGRATOR__INTEGRATOR__

#include <core/forward_decl.h>

#include <memory>

#include <xml/Node.h>

namespace danny
{
    namespace integrator
    {
        class Integrator
        {
        public:
            // Xml structure of the class.
            struct Xml
            {
                virtual ~Xml() {}
                virtual std::unique_ptr<Integrator> create() const = 0;
                static std::unique_ptr<Integrator::Xml> factory(const xml::Node &node);
            };

        public:
            virtual ~Integrator() {}

            virtual void integrate(const core::Scene &scene, core::Image &output) = 0;
        };
    }
}

#endif