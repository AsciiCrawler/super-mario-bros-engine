#pragma once
#include "includes/glm/glm.hpp"

class Entity;

class PhysicsEngine
{
public:
   void update(const float step);

   // True when the entity's hitbox, shifted by offset, overlaps a tile
   static bool collidesAt(const Entity &entity, const glm::vec2 &offset);

   // Move along one axis, stopping flush against the first tile in the way.
   // A blocked move zeroes that velocity component, sets the matching contact flag and returns true
   static bool moveX(Entity &entity, const float dx);
   static bool moveY(Entity &entity, const float dy);

   // Finds the smallest shift along shiftAxis (0 = x, 1 = y), up to maxShift, after which move no longer hits a tile.
   // Tries the sign of preferDir first (positive when 0); onlyPreferred never tries the other direction
   static bool findCornerShift(const Entity &entity, const glm::vec2 &move, const int shiftAxis, const float maxShift, const int preferDir, const bool onlyPreferred, float &shift);
};
