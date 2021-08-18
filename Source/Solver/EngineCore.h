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

    uint32_t m_GroundTextureID;
    uint32_t m_GroundShaderProgramID;
    uint32_t m_GroundVAOID;
    uint32_t m_GroundVBOID;

public:
    Slmulator() = default;
    ~Slmulator() = default;

protected:
    void InitGLFW();
    void DestroyGLFW();

    void LoadSkybox();
    void RenderSkybox();
    void DestroySkybox();

    void LoadScene();
    void RenderScene();
    void DestroyScene();

protected:
    void Render();

public:
    void Init();
    void Loop();
    void Exit();
};

int SimulateCore();