#pragma once
#include "memory"
#include "unordered_map"

class InputSystem
{
public:
    void update();

private:
    std::unordered_map<int, bool> keys;
};