#include "memory"
#include "cmath"
#include "includes/glm/glm.hpp"
#include "src/physicsEngine/physicsEngine.hpp"
#include "src/gameManager.hpp"
#include "src/entity.hpp"

// Edges closer than this count as touching, not overlapping
const float contactEpsilon = 1.0e-4f;

// Tiles fill the unit square [x, x + 1] x [y, y + 1]
static int firstCell(const float min) { return (int)std::floor(min + contactEpsilon); }
static int lastCell(const float max) { return (int)std::ceil(max - contactEpsilon) - 1; }

bool PhysicsEngine::collidesAt(const Entity &entity, const glm::vec2 &offset)
{
    const glm::vec2 min = glm::vec2(entity.position) + entity.hitboxOffset + offset;
    const glm::vec2 max = min + entity.hitboxSize;

    for (int y = firstCell(min.y); y <= lastCell(max.y); y++)
        for (int x = firstCell(min.x); x <= lastCell(max.x); x++)
            if (GameManager::tileMap.at(x, y))
                return true;
    return false;
}

// Returns the distance the entity can travel along axis (0 = x, 1 = y) before a tile stops it
static float sweep(const Entity &entity, const int axis, const float delta, bool &blocked)
{
    blocked = false;
    const int other = 1 - axis;
    const glm::vec2 min = glm::vec2(entity.position) + entity.hitboxOffset;
    const glm::vec2 max = min + entity.hitboxSize;

    // Lanes of tiles the hitbox spans on the other axis
    const int laneFirst = firstCell(min[other]);
    const int laneLast = lastCell(max[other]);
    auto solidAt = [&](const int cell, const int lane)
    {
        return axis == 0 ? GameManager::tileMap.at(cell, lane) != nullptr : GameManager::tileMap.at(lane, cell) != nullptr;
    };

    if (delta > 0.0f)
    {
        // Tiles whose near edge lies between the leading edge and where it is going, nearest first
        const float edge = max[axis];
        const int first = (int)std::ceil(edge - contactEpsilon);
        const int last = (int)std::ceil(edge + delta) - 1;
        for (int cell = first; cell <= last; cell++)
            for (int lane = laneFirst; lane <= laneLast; lane++)
                if (solidAt(cell, lane))
                {
                    blocked = true;
                    return (float)cell - edge;
                }
    }
    else if (delta < 0.0f)
    {
        const float edge = min[axis];
        const int first = (int)std::floor(edge + contactEpsilon) - 1;
        const int last = (int)std::floor(edge + delta);
        for (int cell = first; cell >= last; cell--)
            for (int lane = laneFirst; lane <= laneLast; lane++)
                if (solidAt(cell, lane))
                {
                    blocked = true;
                    return (float)(cell + 1) - edge;
                }
    }
    return delta;
}

bool PhysicsEngine::moveX(Entity &entity, const float dx)
{
    bool blocked;
    entity.position.x += sweep(entity, 0, dx, blocked);
    if (blocked)
    {
        entity.velocity.x = 0.0f;
        (dx > 0.0f ? entity.onWallRight : entity.onWallLeft) = true;
    }
    return blocked;
}

bool PhysicsEngine::moveY(Entity &entity, const float dy)
{
    bool blocked;
    entity.position.y += sweep(entity, 1, dy, blocked);
    if (blocked)
    {
        entity.velocity.y = 0.0f;
        (dy > 0.0f ? entity.onCeiling : entity.onGround) = true;
    }
    return blocked;
}

bool PhysicsEngine::findCornerShift(const Entity &entity, const glm::vec2 &move, const int shiftAxis, const float maxShift, const int preferDir, const bool onlyPreferred, float &shift)
{
    const float increment = 1.0f / 16.0f; // One sprite pixel
    const int first = preferDir < 0 ? -1 : 1;
    const int directions = onlyPreferred && preferDir != 0 ? 1 : 2;
    for (int i = 1; i * increment <= maxShift + contactEpsilon; i++)
        for (int d = 0; d < directions; d++)
        {
            glm::vec2 offset = glm::vec2(0.0f, 0.0f);
            offset[shiftAxis] = (d == 0 ? first : -first) * i * increment;
            // The shifted spot must be free, and the move from there must be clear
            if (!collidesAt(entity, offset) && !collidesAt(entity, offset + move))
            {
                shift = offset[shiftAxis];
                return true;
            }
        }
    return false;
}

void PhysicsEngine::update(const float step)
{
    for (auto &&e : GameManager::dynamicEntities)
    {
        e->onGround = e->onCeiling = e->onWallLeft = e->onWallRight = false;
        moveX(*e, e->velocity.x * step);
        moveY(*e, e->velocity.y * step);
    }
}
