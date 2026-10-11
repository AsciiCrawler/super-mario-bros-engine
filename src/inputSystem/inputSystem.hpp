#pragma once
#include "memory"

class InputSystem
{
public:
    void update();                        // Polls events, once per frame
    void applyToPlayer(const float step); // Applies held actions, once per simulation step
    void cancelJump();
    int jumpState = 0; // 0 Idle - 1 Jump - 2 PostJump

private:
    enum Action
    {
        ActionLeft,
        ActionRight,
        ActionRun,
        ActionJump,
        ActionCount
    };

    bool actions[ActionCount] = {};
    double lastJumpMilliseconds = 0.0; // Simulation time
    static int actionForKey(const int key); // -1 when the key is not bound
};
