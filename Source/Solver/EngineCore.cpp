#define STB_IMAGE_IMPLEMENTATION

#include"EngineCore.h"

void Slmulator::InitGLFW()
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
    glfwSwapInterval(1);

    gladLoadGL();
}

void Slmulator::DestroyGLFW()
{
    glfwDestroyWindow(m_MainWindow);
    glfwTerminate();
}

void Slmulator::LoadSkybox()
{
    glGenTextures(1, &m_SkyboxTextureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_SkyboxTextureID);

    int textureWidth, textureHeight, textureChannels;
    unsigned char* textureData;

    stbi_set_flip_vertically_on_load(false);

    const std::string SkyboxPath("../Asset/Creek/");

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

void Slmulator::RenderSkybox()
{
    double SumTime = glfwGetTime() * 0.3;

    glDepthMask(GL_FALSE);

    glUseProgram(m_SkyboxShaderProgramID);

    auto projectionLocation = glGetUniformLocation(m_SkyboxShaderProgramID, "projection");
    auto viewLocation = glGetUniformLocation(m_SkyboxShaderProgramID, "view");

    auto projection = glm::perspective(glm::radians(45.0f), 16.0f / 9, 0.1f, 100.0f);
    auto view = glm::translate(glm::mat4(1.0), glm::vec3(0.0f, 0.0f, (float)SumTime));

    //view = glm::lookAt(glm::vec3(0, 0, 0), glm::vec3(std::cos(SumTime), 0, std::sin(SumTime)), glm::vec3(0, 1, 0));
    view = glm::lookAt(glm::vec3(0, 0, 0), glm::vec3(0, 0, -1), glm::vec3(0, 1, 0));
    view = glm::mat4(glm::mat3(view));

    glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(projection));
    glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(view));

    glUniform1i(glGetUniformLocation(m_SkyboxShaderProgramID, "skybox"), 0);

    glBindVertexArray(m_SkyboxVAOID);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_SkyboxTextureID);

    glDrawArrays(GL_TRIANGLES, 0, 36);

    glDepthMask(GL_TRUE);
}

void Slmulator::DestroySkybox()
{
    glDeleteTextures(1, &m_SkyboxTextureID);
    glDeleteProgram(m_SkyboxShaderProgramID);

    glDeleteVertexArrays(1, &m_SkyboxVAOID);
    glDeleteBuffers(1, &m_SkyboxVBOID);
}

void Slmulator::LoadScene()
{

}

void Slmulator::RenderScene()
{

}

void Slmulator::DestroyScene()
{

}

void Slmulator::InitOpenGL()
{
    Shader mainVertShader("./Shader/Main.vert");
    Shader mainFragShader("./Shader/Main.frag");

    const char* mainVertShaderSource = mainVertShader.m_ShaderCode.data();
    const char* mainFragShaderSource = mainFragShader.m_ShaderCode.data();

    uint32_t mainVertShaderID = glCreateShader(GL_VERTEX_SHADER);
    uint32_t mainFragShaderID = glCreateShader(GL_FRAGMENT_SHADER);

    int  compileSuccessfully;
    char compileInfo[512];

    glShaderSource(mainVertShaderID, 1, &mainVertShaderSource, nullptr);
    glCompileShader(mainVertShaderID);

    glGetShaderiv(mainVertShaderID, GL_COMPILE_STATUS, &compileSuccessfully);
    if (!compileSuccessfully)
    {
        glGetShaderInfoLog(mainVertShaderID, 512, NULL, compileInfo);
        Out::Log(pType::WARNING, "Compile Failed : %s", compileInfo);
    }

    glShaderSource(mainFragShaderID, 1, &mainFragShaderSource, nullptr);
    glCompileShader(mainFragShaderID);

    glGetShaderiv(mainFragShaderID, GL_COMPILE_STATUS, &compileSuccessfully);
    if (!compileSuccessfully)
    {
        glGetShaderInfoLog(mainFragShaderID, 512, NULL, compileInfo);
        Out::Log(pType::WARNING, "Compile Failed : %s", compileInfo);
    }

    m_MainShaderProgramID = glCreateProgram();
    glAttachShader(m_MainShaderProgramID, mainVertShaderID);
    glAttachShader(m_MainShaderProgramID, mainFragShaderID);
    glLinkProgram(m_MainShaderProgramID);

    glDeleteShader(mainVertShaderID);
    glDeleteShader(mainFragShaderID);

    float vertices[] =
    {
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
         0.0f,  0.5f, 0.0f
    };

    glGenVertexArrays(1, &m_MainVAOID);
    glGenBuffers(1, &m_MainVBOID);

    glBindVertexArray(m_MainVAOID);
    glBindBuffer(GL_ARRAY_BUFFER, m_MainVBOID);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
}

void Slmulator::DestroyOpenGL()
{
    glDeleteProgram(m_MainShaderProgramID);

    glDeleteVertexArrays(1, &m_MainVAOID);
    glDeleteBuffers(1, &m_MainVBOID);
}

void Slmulator::Render()
{
    int sceneWidth, sceneHeight;
    glfwGetFramebufferSize(m_MainWindow, &sceneWidth, &sceneHeight);

    glViewport(0, 0, sceneWidth, sceneHeight);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    RenderSkybox();

    glUseProgram(m_MainShaderProgramID);

    glBindVertexArray(m_MainVAOID);
    //glDrawArrays(GL_TRIANGLES, 0, 3);
}

void Slmulator::Init()
{
    InitGLFW();

    InitOpenGL();

    LoadSkybox();
    LoadScene();
}

void Slmulator::Loop()
{
    while (!glfwWindowShouldClose(m_MainWindow))
    {
        glfwPollEvents();

        Render();

        glfwSwapBuffers(m_MainWindow);
    }
}

void Slmulator::Exit()
{
    DestroySkybox();
    DestroyScene();

    DestroyOpenGL();

    DestroyGLFW();
}

int SimulateCore()
{
    auto MainApp = std::make_unique<Slmulator>();

    Out::Log(pType::MESSAGE, "Initing...");
    MainApp->Init();

    Out::Log(pType::MESSAGE, "Looping...");
    MainApp->Loop();

    Out::Log(pType::MESSAGE, "Exiting...");
    MainApp->Exit();

    return 0;
}