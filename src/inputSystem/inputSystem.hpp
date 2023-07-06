#pragma once
#include "memory"
#include "unordered_map"

class InputSystem
{
public:
    void update();
    int jumpState = 0; // 0 Idle - 1 Jump - 2 PostJump

private:
    std::unordered_map<int, bool> keys;
    int lastJumpMilliseconds;
};