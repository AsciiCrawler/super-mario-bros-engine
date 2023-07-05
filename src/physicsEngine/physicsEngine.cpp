#include "memory"
#include "iostream"
#include "includes/glm/glm.hpp"
#include "src/physicsEngine/physicsEngine.hpp"
#include "src/gameManager.hpp"
#include "src/entity.hpp"
#include "src/physicsEngine/rigidbody.hpp"

bool isOverlap(const Rigidbody a, const Rigidbody b)
{
    if (a.deltaPosition.x + 1 < b.deltaPosition.x || a.deltaPosition.x > b.deltaPosition.x + 1)
        return false;
    if (a.deltaPosition.y + 1 < b.deltaPosition.y || a.deltaPosition.y > b.deltaPosition.y + 1)
        return false;
    return true;
}

void moveEntity(const std::shared_ptr<Entity> entity)
{
    Rigidbody entityRb = Rigidbody(entity->position, entity->physicsDirection);
    for (auto &&sEntity : GameManager::staticEntities)
    {
        const Rigidbody staticRb = Rigidbody(sEntity.second->position, glm::vec3(0.0f));
        if (isOverlap(entityRb, staticRb))
        {
            const glm::vec3 temp = entityRb.deltaPosition - staticRb.deltaPosition;
            if (entity->physicsDirection.y < 0.01f && entityRb.deltaPosition.y > staticRb.deltaPosition.y && abs(entityRb.deltaPosition.x - staticRb.deltaPosition.x) < 0.95f)
                if (abs(temp.y) > abs(temp.x))
                {
                    entityRb.deltaPosition.y = staticRb.deltaPosition.y + 1.0f;
                    entity->physicsDirection.y = 0.0f;
                }

            if (entity->physicsDirection.y > 0.01f && entityRb.deltaPosition.y < staticRb.deltaPosition.y && abs(entityRb.deltaPosition.x - staticRb.deltaPosition.x) < 0.95f)
                if (abs(temp.y) > abs(temp.x))
                {
                    entityRb.deltaPosition.y = staticRb.deltaPosition.y - 1.0f;
                    entity->physicsDirection.y = 0.0f;
                }

            if (entity->physicsDirection.x > 0.01f && entityRb.deltaPosition.x < staticRb.deltaPosition.x && abs(entityRb.deltaPosition.y - staticRb.deltaPosition.y) < 0.95f)
                if (abs(temp.x) > abs(temp.y))
                {
                    entityRb.deltaPosition.x = staticRb.deltaPosition.x - 1.0f;
                    entity->physicsDirection.x = 0.0f;
                }

            if (entity->physicsDirection.x < -0.01f && entityRb.deltaPosition.x > staticRb.deltaPosition.x && abs(entityRb.deltaPosition.y - staticRb.deltaPosition.y) < 0.95f)
                if (abs(temp.x) > abs(temp.y))
                {
                    entityRb.deltaPosition.x = staticRb.deltaPosition.x + 1.0f;
                    entity->physicsDirection.x = 0.0f;
                }
        }
    }

    entity->position = entityRb.deltaPosition;
}

void PhysicsEngine::update()
{
    for (auto &&e : GameManager::dynamicEntities)
        e.second->physicsDirection.y -= 30.0f * GameManager::deltaTime;

    for (auto &&dEntity : GameManager::dynamicEntities)
        moveEntity(dEntity.second);
}