#pragma once
#include "includes/glm/glm.hpp"

// Every movement number lives here, in tiles and seconds
struct MovementTuning
{
    float gravity = 112.5f;
    float maxFall = 20.0f;
    float fastMaxFall = 30.0f;     // Holding down
    float halfGravThreshold = 5.0f; // Gravity halves near the apex while jump is held

    float jumpSpeed = 15.0f;
    float jumpHBoost = 5.0f;   // Extra horizontal speed added in the held direction
    float varJumpTime = 0.23f; // Holding jump keeps the launch speed for up to this long
    float coyoteTime = 0.1f;   // Jumping still works this long after leaving the ground
    float jumpBuffer = 0.1f;   // A jump pressed this long before landing still fires

    float walkSpeed = 7.5f;
    float runSpeed = 10.5f;
    float runAccel = 125.0f;
    float runReduce = 50.0f;   // Slowdown while above top speed in the held direction
    float airMult = 0.65f;     // Horizontal control in the air

    float cornerCorrection = 0.25f; // How far a blocked move may slide around a tile corner

    float wallSlideMax = 2.5f;       // Fall speed while pressing into a wall
    float wallJumpCheckDist = 0.2f;  // A wall this close allows a wall jump
    float wallJumpForceTime = 0.16f; // Input toward the wall is ignored this long after a wall jump

    float dashSpeed = 30.0f;
    float dashTime = 0.15f;     // Gravity is off while dashing
    float dashFreeze = 0.05f;   // Pause before the dash launches
    float endDashSpeed = 20.0f; // Speed is capped to this when the dash ends
    float dashBuffer = 0.1f;    // A dash pressed this long before it is available still fires
};

// Turns input into the player's velocity, once per simulation step, before physics moves the player
class PlayerController
{
public:
    void update(const float step);

    MovementTuning tuning;

private:
    void jump(const int moveX);
    void wallJump(const int direction, const float maxRun);
    void startDash(const int moveX, const int moveY);
    void updateDash(const float step);
    void endDash();

    enum State
    {
        StateNormal,
        StateDash
    };
    State state = StateNormal;
    void correctCorners(const float step);

    int facing = 1;           // -1 left, 1 right
    float coyoteTimer = 0.0f;
    float varJumpTimer = 0.0f;
    float varJumpSpeed = 0.0f;
    int forceMoveX = 0;
    float forceMoveXTimer = 0.0f;

    bool canDash = true; // Refilled on landing
    glm::vec2 dashDirection = glm::vec2(1.0f, 0.0f);
    float dashTimer = 0.0f;
    float dashFreezeTimer = 0.0f;
};
