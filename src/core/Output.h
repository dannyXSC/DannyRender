#ifndef __DANNY__CORE__OUTPUT__
#define __DANNY__CORE__OUTPUT__

#include <xml/Node.h>
#include <core/Image.h>
#include <core/Tonemapper.h>

namespace danny
{
    namespace core
    {
        class Output
        {
        public:
            // Xml structure of the class.
            struct Xml
            {
                virtual ~Xml() = default;
                virtual std::unique_ptr<Output> create() const = 0;
                static std::unique_ptr<Output::Xml> factory(const xml::Node &node);
            };

        public:
            virtual ~Output() = default;

            virtual void save(const Image &image) const = 0;
        };

        class Ldr : public Output
        {
        public:
            // Xml structure of the class.
            struct Xml : public Output::Xml
            {
                std::string path;
                std::string format;
                std::unique_ptr<Tonemapper::Xml> tonemapper;

                explicit Xml(const xml::Node &node);
                std::unique_ptr<Output> create() const override;
            };

        public:
            explicit Ldr(const Ldr::Xml &xml) : Ldr(xml.path + "." + xml.format, xml.tonemapper->create()){};
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
