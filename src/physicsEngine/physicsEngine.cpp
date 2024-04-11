#include "memory"
#include "iostream"
#include "includes/glm/glm.hpp"
#include "src/physicsEngine/physicsEngine.hpp"
#include "src/gameManager.hpp"
#include "src/entity.hpp"

bool isOverlap(const glm::vec3 a, const glm::vec3 b)
{
    if (a.x + 1 < b.x || a.x > b.x + 1)
        return false;
    if (a.y + 1 < b.y || a.y > b.y + 1)
        return false;
    return true;
}

void moveEntity(const std::shared_ptr<Entity> entity)
{
    glm::vec3 entityDeltaPosition = entity->position + (entity->velocity * GameManager::deltaTime);
    for (auto &&sEntity : GameManager::staticEntities)
    {
        if (isOverlap(entityDeltaPosition, sEntity.second->position))
        {
            const glm::vec3 temp = entityDeltaPosition - sEntity.second->position;

            // Top => Down
            if (entity->velocity.y < 0.01f && entityDeltaPosition.y > sEntity.second->position.y && abs(entityDeltaPosition.x - sEntity.second->position.x) < 0.95f)
                if (abs(temp.y) > abs(temp.x))
                {
                    entityDeltaPosition.y = sEntity.second->position.y + 1.0f;
                    entity->velocity.y = 0.0f;
                }

            // Down => Top
            if (entity->velocity.y > 0.01f && entityDeltaPosition.y < sEntity.second->position.y && abs(entityDeltaPosition.x - sEntity.second->position.x) < 0.95f)
                if (abs(temp.y) > abs(temp.x))
                {
                    std::cout << "Collision from bottom" << std::endl;
                    GameManager::inputSystem->lastJumpMilliseconds = 0;
                    entityDeltaPosition.y = sEntity.second->position.y - 1.0f;
                    entity->velocity.y = 0.0f;
                }

            // Left => Right
            if (entity->velocity.x > 0.01f && entityDeltaPosition.x < sEntity.second->position.x && abs(entityDeltaPosition.y - sEntity.second->position.y) < 0.95f)
                if (abs(temp.x) > abs(temp.y))
                {
                    entityDeltaPosition.x = sEntity.second->position.x - 1.0f;
                    entity->velocity.x = 0.0f;
                }

            // Right => Left
            if (entity->velocity.x < -0.01f && entityDeltaPosition.x > sEntity.second->position.x && abs(entityDeltaPosition.y - sEntity.second->position.y) < 0.95f)
                if (abs(temp.x) > abs(temp.y))
                {
                    entityDeltaPosition.x = sEntity.second->position.x + 1.0f;
                    entity->velocity.x = 0.0f;
                }
        }
    }

    entity->position = entityDeltaPosition;
}

void PhysicsEngine::update()
{
    for (auto &&e : GameManager::dynamicEntities)
    {
        if (GameManager::inputSystem->jumpState == 0)
        {
            e.second->velocity.y -= 100.0f * GameManager::deltaTime;
            if (e.second->velocity.y <= -25.0f)
                e.second->velocity.y = -25.0f;
        }

        // Horizontal Speed
        if (e.second->velocity.x > 0.1f)
            e.second->velocity.x -= 20.0f * GameManager::deltaTime;
        if (e.second->velocity.x < -0.1f)
            e.second->velocity.x += 20.0f * GameManager::deltaTime;
        if (abs(e.second->velocity.x) <= 0.1f)
            e.second->velocity.x = 0.0f;
    }

    for (auto &&dEntity : GameManager::dynamicEntities)
        moveEntity(dEntity.second);
}