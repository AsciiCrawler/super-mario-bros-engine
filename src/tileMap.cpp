#include "src/tileMap.hpp"
#include "algorithm"
#include "climits"
#include "cmath"

void TileMap::build(const std::vector<std::shared_ptr<Entity>> &tiles)
{
    this->cells.clear();
    this->width = 0;
    this->height = 0;
    if (tiles.empty())
        return;

    int minX = INT_MAX, minY = INT_MAX, maxX = INT_MIN, maxY = INT_MIN;
    for (auto &&tile : tiles)
    {
        const int x = (int)std::lround(tile->position.x);
        const int y = (int)std::lround(tile->position.y);
        minX = std::min(minX, x);
        minY = std::min(minY, y);
        maxX = std::max(maxX, x);
        maxY = std::max(maxY, y);
    }

    this->originX = minX;
    this->originY = minY;
    this->width = maxX - minX + 1;
    this->height = maxY - minY + 1;
    this->cells.assign(this->width * this->height, nullptr);

    for (auto &&tile : tiles)
    {
        const int x = (int)std::lround(tile->position.x) - this->originX;
        const int y = (int)std::lround(tile->position.y) - this->originY;
        this->cells[y * this->width + x] = tile.get();
    }
}

Entity *TileMap::at(const int x, const int y) const
{
    const int localX = x - this->originX;
    const int localY = y - this->originY;
    if (localX < 0 || localY < 0 || localX >= this->width || localY >= this->height)
        return nullptr;
    return this->cells[localY * this->width + localX];
}
