#pragma once
#include <glm/glm.hpp>

namespace Physics 
{
    inline static bool CheckCollision(const glm::vec2& center1, const glm::vec2& size1, const glm::vec2& center2, const glm::vec2& size2)
    {
        // Calculate the distance between centers
        float deltaX = glm::abs(center1.x - center2.x);
        float deltaY = glm::abs(center1.y - center2.y);

        // Collision occurs if distance is less than half the combined sizes
        bool collideX = deltaX <= (size1.x / 2.0f + size2.x / 2.0f);
        bool collideY = deltaY <= (size1.y / 2.0f + size2.y / 2.0f);

        return collideX && collideY;
    }
}