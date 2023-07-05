#include "src/physicsEngine/physicsEngine.hpp"
#include "src/gameManager.hpp"
#include "memory"
#include "src/entity.hpp"
#include "iostream"

bool isCollition(std::shared_ptr<Entity> a, std::shared_ptr<Entity> b)
{
    if (a->position.x + 1 < b->position.x or a->position.x > b->position.x + 1)
        return false;
    if (a->position.y + 1 < b->position.y or a->position.y > b->position.y + 1)
        return false;
    return true;
}

void PhysicsEngine::update()
{
    for (auto &&e : GameManager::dynamicEntities)
    {
        e.second->physicsDirection.y -= 9.0f * GameManager::deltaTime;
    }

    for (auto &&dEntity : GameManager::dynamicEntities)
    {
        for (auto &&sEntity : GameManager::staticEntities)
        {
            if (isCollition(dEntity.second, sEntity.second))
            {
                if (dEntity.second->physicsDirection.y <= 0)
                {
                    dEntity.second->physicsDirection.y = 0;
                    dEntity.second->position.y = sEntity.second->position.y + 1;
                }
            }
        }
    }

    for (auto &&e : GameManager::dynamicEntities)
    {
        e.second->position += e.second->physicsDirection * GameManager::deltaTime;
    }
}