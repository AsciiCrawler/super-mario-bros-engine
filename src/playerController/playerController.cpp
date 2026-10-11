#include "src/playerController/playerController.hpp"
#include "src/gameManager.hpp"
#include "algorithm"
#include "cmath"

// Moves value toward target by at most maxDelta
static float approach(const float value, const float target, const float maxDelta)
{
    return value > target ? std::max(value - maxDelta, target) : std::min(value + maxDelta, target);
}

static int sign(const float value)
{
    return (value > 0.0f) - (value < 0.0f);
}

void PlayerController::update(const float step)
{
    Entity &player = *GameManager::playerEntity;
    const InputSystem &input = *GameManager::inputSystem;
    const MovementTuning &t = this->tuning;

    int moveX = input.isHeld(InputSystem::ActionRight) - input.isHeld(InputSystem::ActionLeft);
    if (this->forceMoveXTimer > 0.0f)
    {
        this->forceMoveXTimer -= step;
        moveX = this->forceMoveX;
    }
    const int moveY = input.isHeld(InputSystem::ActionUp) - input.isHeld(InputSystem::ActionDown);
    if (moveX != 0)
        this->facing = moveX;

    // Contacts come from the previous physics step
    if (player.onGround)
        this->coyoteTimer = t.coyoteTime;
    else
        this->coyoteTimer -= step;

    // Dash: refilled on the ground, and takes over movement while active
    if (player.onGround && this->state == StateNormal)
        this->canDash = true;
    if (this->state == StateNormal && this->canDash && input.pressedWithin(InputSystem::ActionDash, t.dashBuffer * 1000.0f))
    {
        GameManager::inputSystem->consume(InputSystem::ActionDash);
        this->startDash(moveX, moveY);
    }
    if (this->state == StateDash)
    {
        this->updateDash(step);
        return;
    }

    const float maxRun = input.isHeld(InputSystem::ActionRun) ? t.runSpeed : t.walkSpeed;

    // Horizontal: accelerate toward the target speed, shedding excess speed gently so momentum carries
    {
        const float mult = player.onGround ? 1.0f : t.airMult;
        if (std::abs(player.velocity.x) > maxRun && sign(player.velocity.x) == moveX)
            player.velocity.x = approach(player.velocity.x, maxRun * moveX, t.runReduce * mult * step);
        else
            player.velocity.x = approach(player.velocity.x, maxRun * moveX, t.runAccel * mult * step);
    }

    // Gravity, halved near the apex while jump is held
    {
        const bool wallSlide = !player.onGround && player.velocity.y <= 0.0f && ((moveX > 0 && player.onWallRight) || (moveX < 0 && player.onWallLeft));
        const float maxFall = wallSlide ? t.wallSlideMax : moveY < 0 ? t.fastMaxFall : t.maxFall;
        const bool nearApex = std::abs(player.velocity.y) < t.halfGravThreshold && input.isHeld(InputSystem::ActionJump);
        player.velocity.y = approach(player.velocity.y, -maxFall, t.gravity * (nearApex ? 0.5f : 1.0f) * step);
    }

    // Variable jump: holding jump keeps the launch speed; releasing it or bumping a ceiling ends the window
    if (this->varJumpTimer > 0.0f)
    {
        this->varJumpTimer -= step;
        if (input.isHeld(InputSystem::ActionJump) && !player.onCeiling)
            player.velocity.y = std::max(player.velocity.y, this->varJumpSpeed);
        else
            this->varJumpTimer = 0.0f;
    }

    if (input.pressedWithin(InputSystem::ActionJump, t.jumpBuffer * 1000.0f))
    {
        const bool wallRight = PhysicsEngine::collidesAt(player, glm::vec2(t.wallJumpCheckDist, 0.0f));
        const bool wallLeft = PhysicsEngine::collidesAt(player, glm::vec2(-t.wallJumpCheckDist, 0.0f));
        if (this->coyoteTimer > 0.0f)
        {
            GameManager::inputSystem->consume(InputSystem::ActionJump);
            this->jump(moveX);
        }
        else if (wallRight || wallLeft)
        {
            // Push away from the wall; between two walls, away from the one being faced
            const int direction = wallRight && (this->facing > 0 || !wallLeft) ? -1 : 1;
            GameManager::inputSystem->consume(InputSystem::ActionJump);
            this->wallJump(direction, maxRun);
        }
    }

    this->correctCorners(step);
}

// Slides the player around corners it would otherwise clip by a few pixels
void PlayerController::correctCorners(const float step)
{
    Entity &player = *GameManager::playerEntity;
    const glm::vec2 move = glm::vec2(player.velocity) * step;
    const glm::vec2 moveX = glm::vec2(move.x, 0.0f);
    float shift;

    // Rising into the edge of a ceiling: shift sideways, keeping the upward speed
    if (move.y > 0.0f && !PhysicsEngine::collidesAt(player, moveX) && PhysicsEngine::collidesAt(player, move))
        if (PhysicsEngine::findCornerShift(player, move, 0, this->tuning.cornerCorrection, sign(move.x), false, shift))
            player.position.x += shift;

    // Airborne and clipping the top of a ledge: lift onto it
    if (!player.onGround && move.x != 0.0f && PhysicsEngine::collidesAt(player, moveX))
        if (PhysicsEngine::findCornerShift(player, moveX, 1, this->tuning.cornerCorrection, 1, true, shift))
            player.position.y += shift;
}

void PlayerController::startDash(const int moveX, const int moveY)
{
    // No direction held dashes the way the player faces
    const glm::vec2 direction = moveX == 0 && moveY == 0 ? glm::vec2((float)this->facing, 0.0f) : glm::vec2((float)moveX, (float)moveY);
    this->dashDirection = glm::normalize(direction);
    if (direction.x != 0.0f)
        this->facing = sign(direction.x);

    this->state = StateDash;
    this->canDash = false;
    this->dashFreezeTimer = this->tuning.dashFreeze;
    this->dashTimer = this->tuning.dashTime;
    this->varJumpTimer = 0.0f;
    this->forceMoveXTimer = 0.0f;
    GameManager::playerEntity->velocity = glm::vec3(0.0f, 0.0f, 0.0f);
}

void PlayerController::updateDash(const float step)
{
    Entity &player = *GameManager::playerEntity;

    if (this->dashFreezeTimer > 0.0f)
    {
        this->dashFreezeTimer -= step;
        player.velocity = glm::vec3(0.0f, 0.0f, 0.0f);
        if (this->dashFreezeTimer > 0.0f)
            return;
        // Velocity is set once; a tile in the way zeroes that component for the rest of the dash
        player.velocity = glm::vec3(this->dashDirection * this->tuning.dashSpeed, 0.0f);
    }
    else
    {
        this->dashTimer -= step;
        if (this->dashTimer <= 0.0f || (this->dashDirection.y > 0.0f && player.onCeiling))
        {
            this->endDash();
            return;
        }
    }

    this->correctCorners(step);
}

void PlayerController::endDash()
{
    Entity &player = *GameManager::playerEntity;
    this->state = StateNormal;
    const float speed = glm::length(glm::vec2(player.velocity));
    if (speed > this->tuning.endDashSpeed)
        player.velocity *= this->tuning.endDashSpeed / speed;
}

void PlayerController::wallJump(const int direction, const float maxRun)
{
    Entity &player = *GameManager::playerEntity;
    this->coyoteTimer = 0.0f;
    this->varJumpTimer = this->tuning.varJumpTime;
    this->varJumpSpeed = this->tuning.jumpSpeed;
    this->forceMoveX = direction;
    this->forceMoveXTimer = this->tuning.wallJumpForceTime;
    this->facing = direction;
    player.velocity.x = direction * (maxRun + this->tuning.jumpHBoost);
    player.velocity.y = this->tuning.jumpSpeed;
}

void PlayerController::jump(const int moveX)
{
    Entity &player = *GameManager::playerEntity;
    this->coyoteTimer = 0.0f;
    this->varJumpTimer = this->tuning.varJumpTime;
    this->varJumpSpeed = this->tuning.jumpSpeed;
    player.velocity.x += this->tuning.jumpHBoost * moveX;
    player.velocity.y = this->tuning.jumpSpeed;
}
