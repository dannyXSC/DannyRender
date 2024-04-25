#include "Parser.h"

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/string_cast.hpp"
#include <iostream>

#include <OBJ_Loader.hpp>

namespace danny
{
    namespace xml
    {
        const std::unordered_set<std::string> Parser::gSupportedFormatsLoad{"jpg", "png", "tga", "bmp", "psd", "gif", "hdr", "pic"};
        const std::unordered_set<std::string> Parser::gSupportedFormatsSave{"png", "bmp", "tga"};

        void loadTriangle(const objl::Mesh &mesh, std::shared_ptr<geometry::BVH<geometry::Triangle>> &bvh)
        {
            std::array<glm::vec3, 3> face_vertices;
            std::array<glm::vec2, 3> tex_vertices;
            std::array<glm::vec3, 3> n_vertices;

            for (int i = 0; i < mesh.Vertices.size(); i += 3)
            {
                for (int j = 0; j < 3; j++)
                {
                    auto vert = glm::vec3(mesh.Vertices[i + j].Position.X,
                                          mesh.Vertices[i + j].Position.Y,
                                          mesh.Vertices[i + j].Position.Z);
                    face_vertices[j] = vert;

                    auto tex = glm::vec2(mesh.Vertices[i + j].TextureCoordinate.X,
                                         mesh.Vertices[i + j].TextureCoordinate.Y);
                    tex_vertices[j] = tex;

                    auto n = glm::vec3(mesh.Vertices[i + j].Normal.X,
                                       mesh.Vertices[i + j].Normal.Y,
                                       mesh.Vertices[i + j].Normal.Z);
                    n_vertices[j] = n;
                }

                if (mesh.noNormal)
                {
                    bvh->addObject(geometry::Triangle(face_vertices[0], face_vertices[1] - face_vertices[0],
                                                      face_vertices[2] - face_vertices[0], tex_vertices[0],
                                                      tex_vertices[1], tex_vertices[2]));
                }
                else
                {
                    bvh->addObject(geometry::Triangle(face_vertices[0], face_vertices[1] - face_vertices[0],
                                                      face_vertices[2] - face_vertices[0], tex_vertices[0],
                                                      tex_vertices[1], tex_vertices[2],
                                                      n_vertices[0], n_vertices[1], n_vertices[2]));
                }
            }
        }

        std::shared_ptr<geometry::BVH<geometry::Triangle>> Parser::loadModel(const std::string &path)
        {
            static std::unordered_map<std::string, std::shared_ptr<geometry::BVH<geometry::Triangle>>> path_to_bvh;

            if (path_to_bvh.find(path) == path_to_bvh.end())
            {
                objl::Loader loader;
                loader.LoadFile(path);

                auto &bvh = path_to_bvh[path] = std::make_shared<geometry::BVH<geometry::Triangle>>();

                for (int mesh_idx = 0; mesh_idx < loader.LoadedMeshes.size(); mesh_idx++)
                {
                    auto mesh = loader.LoadedMeshes[mesh_idx];
                    loadTriangle(mesh, bvh);
                }

                // TODO: error
                bvh->buildWithMedianSplit();
                // bvh->buildWithSAHSplit();
            }

            return path_to_bvh[path];
        }

        std::shared_ptr<geometry::BVH<geometry::Triangle>> Parser::loadModel(const std::string &path, const std::string &object_name)
        {
            static std::unordered_map<std::string,
                                      std::unordered_map<std::string, std::shared_ptr<geometry::BVH<geometry::Triangle>>>>
                path_to_bvh;
            if (path_to_bvh.find(path) == path_to_bvh.end())
            {
                objl::Loader loader;
                loader.LoadFile(path);

                for (int mesh_idx = 0; mesh_idx < loader.LoadedMeshes.size(); mesh_idx++)
                {
                    auto mesh = loader.LoadedMeshes[mesh_idx];
                    auto mesh_name = mesh.MeshName;
                    auto &bvh = path_to_bvh[path][mesh_name] = std::make_shared<geometry::BVH<geometry::Triangle>>();

                    loadTriangle(mesh, bvh);

                    // TODO: error
                    bvh->buildWithMedianSplit();
                    // bvh->buildWithSAHSplit();
                }
            }

            if (path_to_bvh[path].find(object_name) == path_to_bvh[path].end())
            {
                throw std::runtime_error("obj file: [" + path + "] do not have object: <" + object_name + ">");
            }
            return path_to_bvh[path][object_name];
        }

        std::shared_ptr<std::vector<core::Image>> Parser::loadImage(const std::string &path, bool mipmapping)
        {
            static std::unordered_map<std::string, std::shared_ptr<std::vector<core::Image>>> path_to_image;

            if (path_to_image.find(path) == path_to_image.end())
            {
                core::Image image(path);

                if (mipmapping)
                {
                    path_to_image[path] = std::make_shared<std::vector<core::Image>>(image.generateMipmaps());
                }
                else
                {
                    path_to_image[path] = std::make_shared<std::vector<core::Image>>(1, std::move(image));
                }
            }

            return path_to_image[path];
        }
    }
}