#define GLM_ENABLE_EXPERIMENTAL
#include <iostream>
#include <glm/geometric.hpp>
#include <glm/vec3.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/ext.hpp>

#include <geometry/Sphere.hpp>
#include <geometry/Transformation.hpp>

class A
{
public:
    int t;
    A() : t(0) { t = 1; }
};

int main()
{
    auto result1 = glm::transpose(glm::inverse(glm::transpose(glm::mat2(glm::vec2(1, 1), glm::vec2(1, -1)))) *
                                  glm::transpose(glm::mat2x3(glm::vec3(1, 2, 3), glm::vec3(3, 2, 1))));
    auto result2 = glm::mat2x3(glm::vec3(1, 2, 3), glm::vec3(3, 2, 1)) *
                   glm::inverse(glm::mat2(glm::vec2(1, 1), glm::vec2(1, -1)));
    // auto result = glm::mat2x3(glm::vec3(1, 2, 3), glm::vec3(3, 2, 1));
    std::cout << glm::to_string(result1) << std::endl;
    std::cout << glm::to_string(result2) << std::endl;
}
