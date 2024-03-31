#include "Parser.h"

#include <OBJ_Loader.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/string_cast.hpp"
#include <iostream>

namespace danny
{
    namespace utils
    {
        std::shared_ptr<geometry::BVH<geometry::Triangle>> Parser::loadModel(const std::string &path)
        {
            static std::unordered_map<std::string, std::shared_ptr<geometry::BVH<geometry::Triangle>>> path_to_bvh;

            if (path_to_bvh.find(path) == path_to_bvh.end())
            {
                objl::Loader loader;
                loader.LoadFile(path);

                assert(loader.LoadedMeshes.size() == 1);
                auto mesh = loader.LoadedMeshes[0];

                auto &bvh = path_to_bvh[path] = std::make_shared<geometry::BVH<geometry::Triangle>>();

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

                // TODO: error
                bvh->buildWithMedianSplit();
            }

            return path_to_bvh[path];
        }
    }
}