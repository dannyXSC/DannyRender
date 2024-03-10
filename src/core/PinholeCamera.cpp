#include "PinholeCamera.h"

#include <glm/trigonometric.hpp>
namespace danny
{
    namespace core
    {
        PinholeCamera::PinholeCamera(const glm::ivec2 &resolution, float near_distance,
                                     const glm::vec2 &fov_xy, const glm::vec3 &pos,
                                     const glm::vec3 &dir, const glm::vec3 &up)
            : m_camera_space(pos, -dir, up), m_resolution(resolution), m_near_distance(near_distance)
        {
            auto s_right = glm::tan(glm::radians(fov_xy.x) * 0.5f) * near_distance;
            auto s_up = glm::tan(glm::radians(fov_xy.y) * 0.5f) * near_distance;

            m_screen_coordinates = glm::vec4(-s_right, s_right, -s_up, s_up);
            m_pixel_length = glm::vec2((m_screen_coordinates.y - m_screen_coordinates.x) / m_resolution.x, (m_screen_coordinates.w - m_screen_coordinates.z) / m_resolution.y);
        }

        geometry::Ray PinholeCamera::castRay(int x, int y, float offset_x, float offset_y) const
        {
            glm::vec3 pixel_coordinate(m_screen_coordinates.x + m_pixel_length.x * (static_cast<float>(x) + offset_x),
                                       m_screen_coordinates.w - m_pixel_length.y * (static_cast<float>(y) + offset_y), -m_near_distance);

            return geometry::Ray(m_camera_space.get_origin(), glm::normalize(m_camera_space.vectorToWorldSpace(pixel_coordinate)));
        }
    }
}