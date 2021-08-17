#pragma once

#include"MathAIO.h"
#include"ToolAIO.h"

#include"Shader.h"

#define SIMULATE_CORE SimulateCore()

class Slmulator
{
protected:
    GLFWwindow* m_MainWindow;

    uint32_t m_SkyboxTextureID;
    uint32_t m_SkyboxShaderProgramID;
    uint32_t m_SkyboxVAOID;
    uint32_t m_SkyboxVBOID;

    uint32_t m_MainShaderProgramID;
    uint32_t m_MainVAOID;
    uint32_t m_MainVBOID;

public:
    Slmulator() = default;
    ~Slmulator() = default;

protected:
    void InitGLFW();
    void DestroyGLFW();

    void LoadSkybox();
    void RenderSkybox();
    void DestroySkybox();

    void InitOpenGL();
    void DestroyOpenGL();

protected:
    void Render();

public:
    void Init();
    void Loop();
    void Exit();
};

int SimulateCore();