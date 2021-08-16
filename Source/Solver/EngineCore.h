#pragma once

#include"MathAIO.h"
#include"ToolAIO.h"

#define SIMULATE_CORE SimulateCore()

class Slmulater
{
protected:
    GLFWwindow* m_MainWindow;

public:
    Slmulater() = default;
    ~Slmulater() = default;

public:
    void Init();
    void Loop();
    void Exit();
};

int SimulateCore();