#define STB_IMAGE_IMPLEMENTATION

#include"EngineCore.h"

void Simulator::InitGLFW()
{
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    m_MainWindow = glfwCreateWindow(G_WINDOW_WIDTH, G_WINDOW_HEIGHT, "Live Window", nullptr, nullptr);

    if (!m_MainWindow)
    {
        Out::Log(pType::ERROR, "init GLFW Window Failed");

        return;
    }

    glfwSetWindowSizeLimits(m_MainWindow, G_WINDOW_WIDTH, G_WINDOW_HEIGHT, GLFW_DONT_CARE, GLFW_DONT_CARE);

    glfwMakeContextCurrent(m_MainWindow);
    glfwSwapInterval(0);

    gladLoadGL();
}

void Simulator::DestroyGLFW()
{
    glfwDestroyWindow(m_MainWindow);
    glfwTerminate();
}

void Simulator::InitUI()
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGui::StyleColorsClassic();

    ImGui_ImplGlfw_InitForOpenGL(m_MainWindow, true);
    ImGui_ImplOpenGL3_Init("#version 330 core");
}

void Simulator::RenderUI()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    auto LogWinFlag =
        ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse |
        ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoBringToFrontOnFocus |
        ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove;

    ImGui::SetNextWindowPos(ImVec2(8, 8), ImGuiCond_Always);

    if (ImGui::Begin("reSimulate",nullptr, LogWinFlag))
    {
        int FPS = -1;
        try
        {
            FPS = static_cast<int>(1.0 / m_FrameTime);
        }
        catch (...) {}

        ImGui::Text("FPS : %d (%lfs)", FPS, m_FrameTime);

        if (false)
        {
            ImGui::Separator();
            ImGui::Text("Particles Number : %d", m_FluidSpriteCount);
        }

        if (true)
        {
            ImGui::Separator();
            for (int i = 0; i < m_RigidbodyObjects.size(); ++i)
            {
                ImGui::Text("%.4lf,%.4lf,%.4lf",
                    m_RigidbodyObjects[i]->getLocation().x(),
                    m_RigidbodyObjects[i]->getLocation().y(),
                    m_RigidbodyObjects[i]->getLocation().z());
            }
        }

        ImGui::End();
    }

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Simulator::DestroyUI()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void Simulator::LoadSkybox()
{
    glGenTextures(1, &m_SkyboxTextureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_SkyboxTextureID);

    int textureWidth, textureHeight, textureChannels;
    unsigned char* textureData;

    const std::string SkyboxPath("../Asset/Skybox/Creek/");

    textureData = stbi_load((SkyboxPath + "posX.jpg").c_str(), &textureWidth, &textureHeight, &textureChannels, 3);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, textureData);
    stbi_image_free(textureData);

    textureData = stbi_load((SkyboxPath + "negX.jpg").c_str(), &textureWidth, &textureHeight, &textureChannels, 3);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, textureData);
    stbi_image_free(textureData);

    textureData = stbi_load((SkyboxPath + "posZ.jpg").c_str(), &textureWidth, &textureHeight, &textureChannels, 3);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, textureData);
    stbi_image_free(textureData);

    textureData = stbi_load((SkyboxPath + "negZ.jpg").c_str(), &textureWidth, &textureHeight, &textureChannels, 3);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, textureData);
    stbi_image_free(textureData);

    textureData = stbi_load((SkyboxPath + "posY.jpg").c_str(), &textureWidth, &textureHeight, &textureChannels, 3);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, textureData);
    stbi_image_free(textureData);

    textureData = stbi_load((SkyboxPath + "negY.jpg").c_str(), &textureWidth, &textureHeight, &textureChannels, 3);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, textureData);
    stbi_image_free(textureData);

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    Shader skyboxVertShader("./Shader/Skybox.vert");
    Shader skyboxFragShader("./Shader/Skybox.frag");

    const char* skyboxVertShaderSource = skyboxVertShader.m_ShaderCode.data();
    const char* skyboxFragShaderSource = skyboxFragShader.m_ShaderCode.data();

    uint32_t skyboxVertShaderID = glCreateShader(GL_VERTEX_SHADER);
    uint32_t skyboxFragShaderID = glCreateShader(GL_FRAGMENT_SHADER);

    int  compileSuccessfully;
    char compileInfo[512];

    glShaderSource(skyboxVertShaderID, 1, &skyboxVertShaderSource, nullptr);
    glCompileShader(skyboxVertShaderID);

    glGetShaderiv(skyboxVertShaderID, GL_COMPILE_STATUS, &compileSuccessfully);
    if (!compileSuccessfully)
    {
        glGetShaderInfoLog(skyboxVertShaderID, 512, NULL, compileInfo);
        Out::Log(pType::WARNING, "Skybox Vert Shader Compile Failed : %s", compileInfo);
    }

    glShaderSource(skyboxFragShaderID, 1, &skyboxFragShaderSource, nullptr);
    glCompileShader(skyboxFragShaderID);

    glGetShaderiv(skyboxFragShaderID, GL_COMPILE_STATUS, &compileSuccessfully);
    if (!compileSuccessfully)
    {
        glGetShaderInfoLog(skyboxFragShaderID, 512, NULL, compileInfo);
        Out::Log(pType::WARNING, "Skybox Frag Shader Compile Failed : %s", compileInfo);
    }

    m_SkyboxShaderProgramID = glCreateProgram();
    glAttachShader(m_SkyboxShaderProgramID, skyboxVertShaderID);
    glAttachShader(m_SkyboxShaderProgramID, skyboxFragShaderID);
    glLinkProgram(m_SkyboxShaderProgramID);

    glDeleteShader(skyboxVertShaderID);
    glDeleteShader(skyboxFragShaderID);

    float skyboxVertices[] =
    {
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
    };

    glGenVertexArrays(1, &m_SkyboxVAOID);
    glGenBuffers(1, &m_SkyboxVBOID);

    glBindVertexArray(m_SkyboxVAOID);
    glBindBuffer(GL_ARRAY_BUFFER, m_SkyboxVBOID);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), skyboxVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
}

void Simulator::RenderSkybox()
{
    glDepthMask(GL_FALSE);

    glUseProgram(m_SkyboxShaderProgramID);

    auto projectionLocation = glGetUniformLocation(m_SkyboxShaderProgramID, "projection");
    auto viewLocation = glGetUniformLocation(m_SkyboxShaderProgramID, "view");

    int screenWidth, screenHeight;
    glfwGetFramebufferSize(m_MainWindow, &screenWidth, &screenHeight);

    glm::vec3 cameraLocation(G_CAMERA_X, G_CAMERA_Y, G_CAMERA_Z);
    glm::vec3 cameraTarget(G_TARGET_X, G_TARGET_Y, G_TARGET_Z);

    auto projection = glm::perspective(glm::radians(60.0f), (float)screenWidth / screenHeight, 0.1f, 100.0f);
    auto view = glm::mat4(glm::mat3(glm::lookAt(cameraLocation, cameraTarget, glm::vec3(0, 1, 0))));

    glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(projection));
    glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(view));

    glUniform1i(glGetUniformLocation(m_SkyboxShaderProgramID, "skybox"), 0);

    glBindVertexArray(m_SkyboxVAOID);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_SkyboxTextureID);

    glDrawArrays(GL_TRIANGLES, 0, 36);

    glDepthMask(GL_TRUE);
}

void Simulator::DestroySkybox()
{
    glDeleteTextures(1, &m_SkyboxTextureID);
    glDeleteProgram(m_SkyboxShaderProgramID);

    glDeleteVertexArrays(1, &m_SkyboxVAOID);
    glDeleteBuffers(1, &m_SkyboxVBOID);
}

void Simulator::LoadScene()
{
    glGenTextures(1, &m_GroundTextureID);
    glBindTexture(GL_TEXTURE_2D, m_GroundTextureID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    const char* texturePath = "../Asset/Texture/Ground_02.jpg";
    int textureWidth, textureHeight, textureChannels;
    unsigned char* groundTextureData = stbi_load(texturePath, &textureWidth, &textureHeight, &textureChannels, 3);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, groundTextureData);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(groundTextureData);

    Shader groundVertShader("./Shader/Ground.vert");
    Shader groundFragShader("./Shader/Ground.frag");

    const char* groundVertShaderSource = groundVertShader.m_ShaderCode.data();
    const char* groundFragShaderSource = groundFragShader.m_ShaderCode.data();

    uint32_t groundVertShaderID = glCreateShader(GL_VERTEX_SHADER);
    uint32_t groundFragShaderID = glCreateShader(GL_FRAGMENT_SHADER);

    int  compileSuccessfully;
    char compileInfo[512];

    glShaderSource(groundVertShaderID, 1, &groundVertShaderSource, nullptr);
    glCompileShader(groundVertShaderID);

    glGetShaderiv(groundVertShaderID, GL_COMPILE_STATUS, &compileSuccessfully);
    if (!compileSuccessfully)
    {
        glGetShaderInfoLog(groundVertShaderID, 512, NULL, compileInfo);
        Out::Log(pType::WARNING, "Ground Vert Shader Compile Failed : %s", compileInfo);
    }

    glShaderSource(groundFragShaderID, 1, &groundFragShaderSource, nullptr);
    glCompileShader(groundFragShaderID);

    glGetShaderiv(groundFragShaderID, GL_COMPILE_STATUS, &compileSuccessfully);
    if (!compileSuccessfully)
    {
        glGetShaderInfoLog(groundFragShaderID, 512, NULL, compileInfo);
        Out::Log(pType::WARNING, "Ground Frag Shader Compile Failed : %s", compileInfo);
    }

    m_GroundShaderProgramID = glCreateProgram();
    glAttachShader(m_GroundShaderProgramID, groundVertShaderID);
    glAttachShader(m_GroundShaderProgramID, groundFragShaderID);
    glLinkProgram(m_GroundShaderProgramID);

    glDeleteShader(groundVertShaderID);
    glDeleteShader(groundFragShaderID);

    int repeatCount = 8;

    float groundVertices[] =
    {
         -8.0f, 0.0f, 8.0f, 0.0f, 0.0f,
         -8.0f, 0.0f,-8.0f, 0.0f, repeatCount,
          8.0f, 0.0f,-8.0f, repeatCount, repeatCount,

         -8.0f, 0.0f, 8.0f, 0.0f, 0.0f,
          8.0f, 0.0f,-8.0f, repeatCount, repeatCount,
          8.0f, 0.0f, 8.0f, repeatCount, 0.0f
    };

    glGenVertexArrays(1, &m_GroundVAOID);
    glGenBuffers(1, &m_GroundVBOID);

    glBindVertexArray(m_GroundVAOID);
    glBindBuffer(GL_ARRAY_BUFFER, m_GroundVBOID);
    glBufferData(GL_ARRAY_BUFFER, sizeof(groundVertices), groundVertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
}

void Simulator::RenderScene()
{
    glUseProgram(m_GroundShaderProgramID);

    auto projectionLocation = glGetUniformLocation(m_GroundShaderProgramID, "projection");
    auto viewLocation = glGetUniformLocation(m_GroundShaderProgramID, "view");

    int screenWidth, screenHeight;
    glfwGetFramebufferSize(m_MainWindow, &screenWidth, &screenHeight);

    glm::vec3 cameraLocation(G_CAMERA_X, G_CAMERA_Y, G_CAMERA_Z);
    glm::vec3 cameraTarget(G_TARGET_X, G_TARGET_Y, G_TARGET_Z);

    auto projection = glm::perspective(glm::radians(60.0f), (float)screenWidth / screenHeight, 0.1f, 100.0f);
    auto view = glm::lookAt(cameraLocation, cameraTarget, glm::vec3(0, 1, 0));

    glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(projection));
    glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(view));

    glUniform1i(glGetUniformLocation(m_SkyboxShaderProgramID, "groundTexture"), 0);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_GroundTextureID);

    glBindVertexArray(m_GroundVAOID);

    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void Simulator::DestroyScene()
{
    glDeleteTextures(1, &m_GroundTextureID);
    glDeleteProgram(m_GroundShaderProgramID);

    glDeleteVertexArrays(1, &m_GroundVAOID);
    glDeleteBuffers(1, &m_GroundVBOID);
}

void Simulator::LoadFluidRenderable()
{
    glGenTextures(1, &m_SpriteTextureID);
    glBindTexture(GL_TEXTURE_2D, m_SpriteTextureID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    const char* texturePath = "../Asset/Texture/Sprite.png";
    int textureWidth, textureHeight, textureChannels;
    unsigned char* spriteTextureData = stbi_load(texturePath, &textureWidth, &textureHeight, &textureChannels, 3);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, spriteTextureData);

    stbi_image_free(spriteTextureData);

    Shader spriteVertShader("./Shader/Sprite.vert");
    Shader spriteFragShader("./Shader/Sprite.frag");

    const char* spriteVertShaderSource = spriteVertShader.m_ShaderCode.data();
    const char* spriteFragShaderSource = spriteFragShader.m_ShaderCode.data();

    uint32_t spriteVertShaderID = glCreateShader(GL_VERTEX_SHADER);
    uint32_t spriteFragShaderID = glCreateShader(GL_FRAGMENT_SHADER);

    int  compileSuccessfully;
    char compileInfo[512];

    glShaderSource(spriteVertShaderID, 1, &spriteVertShaderSource, nullptr);
    glCompileShader(spriteVertShaderID);

    glGetShaderiv(spriteVertShaderID, GL_COMPILE_STATUS, &compileSuccessfully);
    if (!compileSuccessfully)
    {
        glGetShaderInfoLog(spriteVertShaderID, 512, NULL, compileInfo);
        Out::Log(pType::WARNING, "Sprite Vert Shader Compile Failed : %s", compileInfo);
    }

    glShaderSource(spriteFragShaderID, 1, &spriteFragShaderSource, nullptr);
    glCompileShader(spriteFragShaderID);

    glGetShaderiv(spriteFragShaderID, GL_COMPILE_STATUS, &compileSuccessfully);
    if (!compileSuccessfully)
    {
        glGetShaderInfoLog(spriteFragShaderID, 512, NULL, compileInfo);
        Out::Log(pType::WARNING, "Sprite Frag Shader Compile Failed : %s", compileInfo);
    }

    m_SpriteShaderProgramID = glCreateProgram();
    glAttachShader(m_SpriteShaderProgramID, spriteVertShaderID);
    glAttachShader(m_SpriteShaderProgramID, spriteFragShaderID);
    glLinkProgram(m_SpriteShaderProgramID);

    glDeleteShader(spriteVertShaderID);
    glDeleteShader(spriteFragShaderID);

    glGenVertexArrays(1, &m_SpriteVAOID);
    glGenBuffers(1, &m_SpriteVBOID);

    uint32_t floatIndex = 0;

    for (auto i = m_FluidParticles->begin(); i != m_FluidParticles->end(); ++i)
    {
        m_FluidParticlesBuffer[floatIndex++] = static_cast<float>(i->x());
        m_FluidParticlesBuffer[floatIndex++] = static_cast<float>(i->y());
        m_FluidParticlesBuffer[floatIndex++] = static_cast<float>(i->z());
    }

    glBindVertexArray(m_SpriteVAOID);
    glBindBuffer(GL_ARRAY_BUFFER, m_SpriteVBOID);
    glBufferData(GL_ARRAY_BUFFER, m_FluidSpriteCount * 3 * sizeof(float), m_FluidParticlesBuffer.get(), GL_STREAM_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
}

void Simulator::RenderFluidRenderable()
{
    glEnable(GL_POINT_SPRITE);
    glEnable(GL_PROGRAM_POINT_SIZE);
    // glEnable(GL_BLEND);
    // glBlendFunc(GL_SRC_ALPHA, GL_ONE);

    glUseProgram(m_SpriteShaderProgramID);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, m_SpriteTextureID);

    auto projectionLocation = glGetUniformLocation(m_SpriteShaderProgramID, "projection");
    auto viewLocation = glGetUniformLocation(m_SpriteShaderProgramID, "view");

    int screenWidth, screenHeight;
    glfwGetFramebufferSize(m_MainWindow, &screenWidth, &screenHeight);

    glm::vec3 cameraLocation(G_CAMERA_X, G_CAMERA_Y, G_CAMERA_Z);
    glm::vec3 cameraTarget(G_TARGET_X, G_TARGET_Y, G_TARGET_Z);

    auto projection = glm::perspective(glm::radians(60.0f), (float)screenWidth / screenHeight, 0.1f, 100.0f);
    auto view = glm::lookAt(cameraLocation, cameraTarget, glm::vec3(0, 1, 0));

    glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(projection));
    glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(view));

    glUniform3f(glGetUniformLocation(m_SpriteShaderProgramID, "cameraPos"), G_CAMERA_X, G_CAMERA_Y, G_CAMERA_Z);

    glUniform1i(glGetUniformLocation(m_SpriteShaderProgramID, "spriteTexture"), 1);

    uint32_t floatIndex = 0;

    for (auto i = m_FluidParticles->begin(); i != m_FluidParticles->end(); ++i)
    {
        m_FluidParticlesBuffer[floatIndex++] = static_cast<float>(i->x());
        m_FluidParticlesBuffer[floatIndex++] = static_cast<float>(i->y());
        m_FluidParticlesBuffer[floatIndex++] = static_cast<float>(i->z());
    }

    glBindVertexArray(m_SpriteVAOID);

    glBindBuffer(GL_ARRAY_BUFFER, m_SpriteVBOID);
    glBufferData(GL_ARRAY_BUFFER, m_FluidSpriteCount * 3 * sizeof(float), m_FluidParticlesBuffer.get(), GL_STREAM_DRAW);

    glDrawArrays(GL_POINTS, 0, m_FluidSpriteCount);

    // glDisable(GL_BLEND);
}

void Simulator::DestroyFluidRenderable()
{
    glDeleteTextures(1, &m_SpriteTextureID);
    glDeleteProgram(m_SpriteShaderProgramID);

    glDeleteVertexArrays(1, &m_SpriteVAOID);
    glDeleteBuffers(1, &m_SpriteVBOID);
}

void Simulator::LoadFluidObjects()
{
    m_FluidParticles = std::make_shared<std::vector<Point>>();

    for (float i = -3.5; i <= -0.5; i += 0.15)
    {
        for (float j = -3.5; j <= -0.5; j += 0.15)
        {
            for (float k = 0.0; k <= 3.5; k += 0.15)
            {
                m_FluidParticles->push_back(Point(i, 2.5 + k, j));
            }
        }
    }

    Out::Log(pType::MESSAGE, "Sphere Count : %d", m_FluidParticles->size());

    m_FluidSpriteCount = m_FluidParticles->size();

    m_FluidParticlesBuffer = std::make_unique<float[]>(m_FluidSpriteCount * 3);

    FluidAPI_SetParticleBuffer(m_FluidParticles);
}

void Simulator::DestroyFluidObjects()
{
    m_FluidParticles->clear();
}

void Simulator::LoadUniversityCUDAPlanetRenderable()
{
    Shader planetVertShader("./Shader/Planet.vert");
    Shader planetFragShader("./Shader/Planet.frag");

    const char* planetVertShaderSource = planetVertShader.m_ShaderCode.data();
    const char* planetFragShaderSource = planetFragShader.m_ShaderCode.data();

    uint32_t planetVertShaderID = glCreateShader(GL_VERTEX_SHADER);
    uint32_t planetFragShaderID = glCreateShader(GL_FRAGMENT_SHADER);

    int  compileSuccessfully;
    char compileInfo[512];

    glShaderSource(planetVertShaderID, 1, &planetVertShaderSource, nullptr);
    glCompileShader(planetVertShaderID);

    glGetShaderiv(planetVertShaderID, GL_COMPILE_STATUS, &compileSuccessfully);
    if (!compileSuccessfully)
    {
        glGetShaderInfoLog(planetVertShaderID, 512, NULL, compileInfo);
        Out::Log(pType::WARNING, "Planet Vert Shader Compile Failed : %s", compileInfo);
    }

    glShaderSource(planetFragShaderID, 1, &planetFragShaderSource, nullptr);
    glCompileShader(planetFragShaderID);

    glGetShaderiv(planetFragShaderID, GL_COMPILE_STATUS, &compileSuccessfully);
    if (!compileSuccessfully)
    {
        glGetShaderInfoLog(planetFragShaderID, 512, NULL, compileInfo);
        Out::Log(pType::WARNING, "Planet Frag Shader Compile Failed : %s", compileInfo);
    }

    m_PlanetShaderProgramID = glCreateProgram();
    glAttachShader(m_PlanetShaderProgramID, planetVertShaderID);
    glAttachShader(m_PlanetShaderProgramID, planetFragShaderID);
    glLinkProgram(m_PlanetShaderProgramID);

    glDeleteShader(planetVertShaderID);
    glDeleteShader(planetFragShaderID);

    glGenVertexArrays(1, &m_PlanetVAOID);
    glGenBuffers(1, &m_PlanetVBOID);

    glBindVertexArray(m_PlanetVAOID);
    glBindBuffer(GL_ARRAY_BUFFER, m_PlanetVBOID);

    glBufferData(GL_ARRAY_BUFFER, m_UniversityCUDAPlanetCount * 10 * sizeof(float), m_UniversityCUDAPlanets->data(), GL_STREAM_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(0);
}

void Simulator::RenderUniversityCUDAPlanetRenderable()
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);

    glEnable(GL_POINT_SPRITE);
    glEnable(GL_PROGRAM_POINT_SIZE);

    glUseProgram(m_PlanetShaderProgramID);

    glBindVertexArray(m_PlanetVAOID);

    glBindBuffer(GL_ARRAY_BUFFER, m_PlanetVBOID);

    // Call every frame?
    glBufferData(GL_ARRAY_BUFFER, m_UniversityCUDAPlanetCount * 10 * sizeof(float), m_UniversityCUDAPlanets->data(), GL_STREAM_DRAW);

    glDrawArrays(GL_POINTS, 0, m_UniversityCUDAPlanetCount);

    glDisable(GL_BLEND);
}

void Simulator::DestroyUniversityCUDAPlanetRenderable()
{
    glDeleteProgram(m_PlanetShaderProgramID);

    glDeleteVertexArrays(1, &m_PlanetVAOID);
    glDeleteBuffers(1, &m_PlanetVBOID);
}

void Simulator::LoadUniversityCUDAPlanetObjects()
{
    m_UniversityCUDAPlanets = std::make_shared<std::vector<UniversityCUDA_API_Planet>>();

    UniversityCUDA_API_Planet newPlanet;

    for (int i = 0; i < 20480; i += 1)
    {
        newPlanet.Acceleration = { 0,0,0 };

        newPlanet.Mass = Uniform(10, 64);

        if (i < 5120)
        {
            newPlanet.Location.x = static_cast<float>(Uniform(-300, -200));
            newPlanet.Location.y = static_cast<float>(Uniform(-300, -200));
            newPlanet.Location.z = static_cast<float>(Uniform(-300, -200));
        }
        else
        {
            newPlanet.Location.x = static_cast<float>(Uniform(500, 600));
            newPlanet.Location.y = static_cast<float>(Uniform(500, 600));
            newPlanet.Location.z = static_cast<float>(Uniform(500, 600));
        }

        newPlanet.Speed.x = static_cast<float>(Uniform(-100, 100));
        newPlanet.Speed.y = static_cast<float>(Uniform(-100, 100));
        newPlanet.Speed.z = static_cast<float>(Uniform(-100, 100));

        m_UniversityCUDAPlanets->push_back(newPlanet);
    }

    m_UniversityCUDAPlanetCount = m_UniversityCUDAPlanets->size();

    UniversityCUDA_API_Init(m_UniversityCUDAPlanets->data(), m_UniversityCUDAPlanets->size());
}

void Simulator::DestroyUniversityCUDAPlanetObjects()
{
    UniversityCUDA_API_Exit();

    m_UniversityCUDAPlanets->clear();
}

void Simulator::LoadRigidbody()
{
    m_RigidbodyObjects.push_back(std::make_shared<RigidbodyAPISphere>(Point(0, 3, 0), 2, 2048, 1));
    m_RigidbodyRenderableObjects.push_back(std::make_shared<RenderableSphere>(Point(0, 3, 0), 1, 64, 36));

    m_RigidbodyObjects.push_back(std::make_shared<RigidbodyAPISphere>(Point(1.5, 5, 0), 2, 1024, 1));
    m_RigidbodyRenderableObjects.push_back(std::make_shared<RenderableSphere>(Point(1.5, 5, 0), 1, 64, 36));

    m_RigidbodyObjects.push_back(std::make_shared<RigidbodyAPISphere>(Point(0, 5, 2), 2, 1024, 2));
    m_RigidbodyRenderableObjects.push_back(std::make_shared<RenderableSphere>(Point(0, 5, 2), 2, 64, 36));

    m_RigidbodyObjects.push_back(std::make_shared<RigidbodyAPISphere>(Point(-1.5, 5, 2), 2, 1024, 2));
    m_RigidbodyRenderableObjects.push_back(std::make_shared<RenderableSphere>(Point(-1.5, 5, 2), 2, 64, 36));

    m_RigidbodyObjects.push_back(std::make_shared<RigidbodyAPISphere>(Point(1, 8, 0), 5, 1024, 3));
    m_RigidbodyRenderableObjects.push_back(std::make_shared<RenderableSphere>(Point(1, 8, 0), 3, 64, 36));

    RigidbodyAPI_Init(&m_RigidbodyObjects);
}

void Simulator::RenderRigidbody()
{
    int sceneWidth, sceneHeight;
    glfwGetFramebufferSize(m_MainWindow, &sceneWidth, &sceneHeight);

    float R = 1.0f * sceneWidth / sceneHeight;

    for (int i = 0; i < m_RigidbodyRenderableObjects.size(); i += 1)
    {
        m_RigidbodyRenderableObjects[i]->MoveTo(m_RigidbodyObjects[i]->getLocation());

        m_RigidbodyRenderableObjects[i]->Render(R);
    }
}

void Simulator::DestroyRigidbody()
{
    RigidbodyAPI_Exit();
}

void Simulator::Update()
{
    static bool FirstUpdate = true;
    static double LastTime = glfwGetTime();

    double NowTime = glfwGetTime();
    double DeltaTime = NowTime - LastTime;
    LastTime = NowTime;

    if (FirstUpdate)
    {
        FirstUpdate = false;
        return;
    }

    // Code Here

    m_FrameTime = DeltaTime;

    // FluidAPI_Step(DeltaTime);

    // UniversityCUDA_API_Step(m_UniversityCUDAPlanets->data(), DeltaTime);

    RigidbodyAPI_Step(DeltaTime);
}

void Simulator::Render()
{
    int sceneWidth, sceneHeight;
    glfwGetFramebufferSize(m_MainWindow, &sceneWidth, &sceneHeight);
    
    float R = 1.0f * sceneWidth / sceneHeight;

    glViewport(0, 0, sceneWidth, sceneHeight);

    glEnable(GL_DEPTH_TEST);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    RenderSkybox();
    RenderScene();

    // RenderFluidRenderable();

    // RenderUniversityCUDAPlanetRenderable();

    RenderRigidbody();

    RenderUI();
}

void Simulator::Init()
{
    InitGLFW();

    InitUI();

    LoadSkybox();
    LoadScene();

    // LoadFluidObjects();
    // LoadFluidRenderable();

    // LoadUniversityCUDAPlanetObjects();
    // LoadUniversityCUDAPlanetRenderable();
    
    LoadRigidbody();
}

void Simulator::Loop()
{
    while (!glfwWindowShouldClose(m_MainWindow))
    {
        glfwPollEvents();

        Update();
        Render();

        glfwSwapBuffers(m_MainWindow);
    }
}

void Simulator::Exit()
{
    DestroySkybox();
    DestroyScene();

    // DestroyFluidObjects();
    // DestroyFluidRenderable();

    // DestroyUniversityCUDAPlanetObjects();
    // DestroyUniversityCUDAPlanetRenderable();

    DestroyRigidbody();

    DestroyUI();

    DestroyGLFW();
}

int SimulateCore()
{
    srand((unsigned)time(nullptr));

    auto MainApp = std::make_unique<Simulator>();

    Out::Log(pType::MESSAGE, "Initing...");
    MainApp->Init();

    Out::Log(pType::MESSAGE, "Looping...");
    MainApp->Loop();

    Out::Log(pType::MESSAGE, "Exiting...");
    MainApp->Exit();

    return 0;
}