#pragma once
#include "memory"
#include "unordered_map"
#include <cstdint>

class InputSystem
{
public:
    void update();
    int jumpState = 0; // 0 Idle - 1 Jump - 2 PostJump
    uint64_t lastJumpMilliseconds = 0;

private:
    std::unordered_map<int, bool> keys;
};