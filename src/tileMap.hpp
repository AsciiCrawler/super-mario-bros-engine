#pragma once
#include "memory"
#include "vector"
#include "src/entity.hpp"

// Grid of static tiles indexed by their whole-number position
class TileMap
{
public:
    void build(const std::vector<std::shared_ptr<Entity>> &tiles);
    Entity *at(const int x, const int y) const;

private:
    int originX = 0;
    int originY = 0;
    int width = 0;
    int height = 0;
    std::vector<Entity *> cells;
};
