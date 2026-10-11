#include "memory"
#include "cmath"
#include "includes/glm/glm.hpp"
#include "src/physicsEngine/physicsEngine.hpp"
#include "src/gameManager.hpp"
#include "src/entity.hpp"

bool isOverlap(const glm::vec3 &a, const glm::vec3 &b)
{
    if (a.x + 1 < b.x || a.x > b.x + 1)
        return false;
    if (a.y + 1 < b.y || a.y > b.y + 1)
        return false;
    return true;
}

void resolveCollision(Entity &entity, glm::vec3 &entityDeltaPosition, const Entity &tile)
{
    if (isOverlap(entityDeltaPosition, tile.position))
    {
        const glm::vec3 temp = entityDeltaPosition - tile.position;

        // Top => Down
        if (entity.velocity.y < 0.01f && entityDeltaPosition.y > tile.position.y && std::abs(entityDeltaPosition.x - tile.position.x) < 0.95f)
            if (std::abs(temp.y) > std::abs(temp.x))
            {
                entityDeltaPosition.y = tile.position.y + 1.0f;
                entity.velocity.y = 0.0f;
            }

        // Down => Top
        if (entity.velocity.y > 0.01f && entityDeltaPosition.y < tile.position.y && std::abs(entityDeltaPosition.x - tile.position.x) < 0.95f)
            if (std::abs(temp.y) > std::abs(temp.x))
            {
                GameManager::inputSystem->cancelJump();
                entityDeltaPosition.y = tile.position.y - 1.0f;
                entity.velocity.y = 0.0f;
            }

        // Left => Right
        if (entity.velocity.x > 0.01f && entityDeltaPosition.x < tile.position.x && std::abs(entityDeltaPosition.y - tile.position.y) < 0.95f)
            if (std::abs(temp.x) > std::abs(temp.y))
            {
                entityDeltaPosition.x = tile.position.x - 1.0f;
                entity.velocity.x = 0.0f;
            }

        // Right => Left
        if (entity.velocity.x < -0.01f && entityDeltaPosition.x > tile.position.x && std::abs(entityDeltaPosition.y - tile.position.y) < 0.95f)
            if (std::abs(temp.x) > std::abs(temp.y))
            {
                entityDeltaPosition.x = tile.position.x + 1.0f;
                entity.velocity.x = 0.0f;
            }
    }
}

void moveEntity(Entity &entity, const float step)
{
    glm::vec3 entityDeltaPosition = entity.position + (entity.velocity * step);

    // Only tiles near the entity can overlap it; the extra cell of margin covers
    // positions reached after a collision snaps the entity by up to one tile
    const int minX = (int)std::ceil(entityDeltaPosition.x - 1.0f) - 1;
    const int maxX = (int)std::floor(entityDeltaPosition.x + 1.0f) + 1;
    const int minY = (int)std::ceil(entityDeltaPosition.y - 1.0f) - 1;
    const int maxY = (int)std::floor(entityDeltaPosition.y + 1.0f) + 1;
    for (int y = minY; y <= maxY; y++)
        for (int x = minX; x <= maxX; x++)
        {
            const Entity *tile = GameManager::tileMap.at(x, y);
            if (tile)
                resolveCollision(entity, entityDeltaPosition, *tile);
        }

    entity.position = entityDeltaPosition;
}

void PhysicsEngine::update(const float step)
{
    for (auto &&e : GameManager::dynamicEntities)
    {
        if (GameManager::inputSystem->jumpState == 0)
        {
            e->velocity.y -= 100.0f * step;
            if (e->velocity.y <= -25.0f)
                e->velocity.y = -25.0f;
        }

        // Horizontal Speed
        if (e->velocity.x > 0.1f)
            e->velocity.x -= 20.0f * step;
        if (e->velocity.x < -0.1f)
            e->velocity.x += 20.0f * step;
        if (std::abs(e->velocity.x) <= 0.1f)
            e->velocity.x = 0.0f;
    }

    for (auto &&dEntity : GameManager::dynamicEntities)
        moveEntity(*dEntity, step);
}
