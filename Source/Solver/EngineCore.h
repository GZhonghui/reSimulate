#pragma once

#include"MathAIO.h"
#include"ToolAIO.h"

#define SIMULATE_CORE SimulateCore()

class Slmulator
{
protected:
    GLFWwindow* m_MainWindow;

public:
    Slmulator() = default;
    ~Slmulator() = default;

public:
    void Init();
    void Loop();
    void Exit();
};

int SimulateCore();