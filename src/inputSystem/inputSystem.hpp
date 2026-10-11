#pragma once
#include "memory"

class InputSystem
{
public:
    enum Action
    {
        ActionLeft,
        ActionRight,
        ActionUp,
        ActionDown,
        ActionRun,
        ActionJump,
        ActionDash,
        ActionCount
    };

    InputSystem();
    void update(); // Polls events, once per frame

    bool isHeld(const Action action) const;
    // True when the action was pressed within the last milliseconds of simulation time and not consumed since
    bool pressedWithin(const Action action, const double milliseconds) const;
    void consume(const Action action); // Spends the latest press so it triggers only once

private:
    bool actions[ActionCount] = {};
    double lastPressMilliseconds[ActionCount] = {}; // Simulation time of each action's latest press
    bool pressConsumed[ActionCount] = {};
    static int actionForKey(const int key); // -1 when the key is not bound
};
