#ifndef __DANNY__CORE__OUTPUT__
#define __DANNY__CORE__OUTPUT__

#include <core/Image.h>
#include <core/Tonemapper.h>

namespace danny
{
    namespace core
    {
        class Output
        {
        public:
            virtual ~Output() = default;

            virtual void save(const Image &image) const = 0;
        };

        class Ldr : public Output
        {
        public:
            explicit Ldr(const std::string &path,
                         std::unique_ptr<Tonemapper> tonemapper);

            void save(const Image &image) const override;

        private:
            std::string m_path;
            std::unique_ptr<Tonemapper> m_tonemapper;
        };
    }
}

#endif
