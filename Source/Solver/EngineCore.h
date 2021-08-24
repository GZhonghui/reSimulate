#pragma once

#include"MathAIO.h"
#include"ToolAIO.h"

#include"Shader.h"
#include"Renderable.h"

#define SIMULATE_CORE SimulateCore()

class Simulator
{
    // Render
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

    // Fluid
    uint32_t m_SpriteTextureID;
    uint32_t m_SpriteShaderProgramID;
    uint32_t m_SpriteVAOID;
    uint32_t m_SpriteVBOID;

    // University
    uint32_t m_PlanetShaderProgramID;
    uint32_t m_PlanetVAOID;
    uint32_t m_PlanetVBOID;

    // UI
protected:
    double m_FrameTime;

    // Import Libs
protected:
    // For Fluid
    uint32_t m_FluidSpriteCount;
    std::shared_ptr<std::vector<Point>> m_FluidParticles;
    std::unique_ptr<float[]> m_FluidParticlesBuffer;

    // For Rigidbody
    std::vector<std::shared_ptr<RigidbodyAPIObject>> m_RigidbodyObjects;
    std::vector<std::shared_ptr<RenderableSphere>> m_RigidbodyRenderableObjects;

    // For University
    uint32_t m_UniversityCUDAPlanetCount;
    std::shared_ptr<std::vector<UniversityCUDA_API_Planet>> m_UniversityCUDAPlanets;

public:
    Simulator() = default;
    ~Simulator() = default;

protected:
    void InitGLFW();
    void DestroyGLFW();

    void InitUI();
    void RenderUI();
    void DestroyUI();

    void LoadSkybox();
    void RenderSkybox();
    void DestroySkybox();

    void LoadScene();
    void RenderScene();
    void DestroyScene();

    void LoadFluidRenderable();
    void RenderFluidRenderable();
    void DestroyFluidRenderable();

    void LoadFluidObjects();
    void DestroyFluidObjects();

    void LoadUniversityCUDAPlanetRenderable();
    void RenderUniversityCUDAPlanetRenderable();
    void DestroyUniversityCUDAPlanetRenderable();

    void LoadUniversityCUDAPlanetObjects();
    void DestroyUniversityCUDAPlanetObjects();

    void LoadRigidbody();
    void RenderRigidbody();
    void DestroyRigidbody();

protected:
    void Update();
    void Render();

public:
    void Init();
    void Loop();
    void Exit();
};

int SimulateCore();