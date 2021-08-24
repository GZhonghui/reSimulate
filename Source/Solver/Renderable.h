#pragma once

#include"MathAIO.h"
#include"ToolAIO.h"

#include"Shader.h"

class Renderable
{
public:
    Renderable() = default;
    virtual ~Renderable() = default;

public:
    virtual void Render(float R) = 0;
};

class RenderableSprite : public Renderable { /* Nothing */ };

class RenderableSphere : Renderable
{
protected:
    Point m_Center;

protected:
    double m_Radius;
    uint32_t m_HeightSegments;
    uint32_t m_RandSegments;
    uint32_t m_VerticesCount;

protected:
    uint32_t m_ShaderProgramID;
    uint32_t m_VAOID;
    uint32_t m_VBOID;
    uint32_t m_EBOID;

public:
    RenderableSphere(const Point& Center, double Radius, uint32_t HeightSegments, uint32_t RandSegments) :
        m_Center(Center), m_Radius(Radius), m_HeightSegments(HeightSegments), m_RandSegments(RandSegments)
    {
        Shader vertShader("./Shader/Sphere.vert");
        Shader fragShader("./Shader/Sphere.frag");

        const char* vertShaderSource = vertShader.m_ShaderCode.data();
        const char* fragShaderSource = fragShader.m_ShaderCode.data();

        uint32_t vertShaderID = glCreateShader(GL_VERTEX_SHADER);
        uint32_t fragShaderID = glCreateShader(GL_FRAGMENT_SHADER);

        glShaderSource(vertShaderID, 1, &vertShaderSource, nullptr);
        glCompileShader(vertShaderID);

        glShaderSource(fragShaderID, 1, &fragShaderSource, nullptr);
        glCompileShader(fragShaderID);

        m_ShaderProgramID = glCreateProgram();
        glAttachShader(m_ShaderProgramID, vertShaderID);
        glAttachShader(m_ShaderProgramID, fragShaderID);
        glLinkProgram(m_ShaderProgramID);

        glDeleteShader(vertShaderID);
        glDeleteShader(fragShaderID);

        glGenVertexArrays(1, &m_VAOID);
        glGenBuffers(1, &m_VBOID);
        glGenBuffers(1, &m_EBOID);

        glBindVertexArray(m_VAOID);
        glBindBuffer(GL_ARRAY_BUFFER, m_VBOID);

        std::vector<float> verticesData;

        double nowHeight = -m_Radius;
        double heightStep = 2 * m_Radius / m_HeightSegments;
        
        double nowRand = 0;
        double nowRadius = 0;
        double randStep = 2 * pi / m_RandSegments;

        // Buttom Vertex
        verticesData.push_back(0);
        verticesData.push_back(nowHeight);
        verticesData.push_back(0);

        nowHeight += heightStep;

        for (int hIndex = 1; hIndex < m_HeightSegments; hIndex += 1)
        {
            nowRand = 0;
            nowRadius = std::sqrt(m_Radius * m_Radius - nowHeight * nowHeight);

            for (int rIndex = 1; rIndex <= m_RandSegments; rIndex += 1)
            {
                double nowX = std::cos(nowRand) * nowRadius;
                double nowY = nowHeight;
                double nowZ = -std::sin(nowRand) * nowRadius;

                verticesData.push_back(nowX);
                verticesData.push_back(nowY);
                verticesData.push_back(nowZ);

                nowRand += randStep;
            }

            nowHeight += heightStep;
        }

        // Top Vertex
        verticesData.push_back(0);
        verticesData.push_back(nowHeight);
        verticesData.push_back(0);

        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * verticesData.size(), verticesData.data(), GL_STATIC_DRAW);

        std::vector<unsigned int> indicesData;

        auto getIndex = [&](int Level, int subIndex) -> unsigned int
        {
            subIndex = subIndex % m_RandSegments;

            if (Level == 0) return 0;
            if (Level == m_HeightSegments) return (HeightSegments - 1) * m_RandSegments + 1;
            return (Level - 1) * m_RandSegments + subIndex + 1;
        };

        for (int rIndex = 0; rIndex < m_RandSegments; rIndex += 1)
        {
            indicesData.push_back(getIndex(0, 0));
            indicesData.push_back(getIndex(1, rIndex));
            indicesData.push_back(getIndex(1, rIndex + 1));
        }

        for (int rIndex = 0; rIndex < m_RandSegments; rIndex += 1)
        {
            indicesData.push_back(getIndex(m_HeightSegments, 0));
            indicesData.push_back(getIndex(m_HeightSegments - 1, rIndex));
            indicesData.push_back(getIndex(m_HeightSegments - 1, rIndex + 1));
        }

        for (int hIndex = 1; hIndex <= m_HeightSegments - 2; hIndex += 1)
        {
            for (int rIndex = 0; rIndex < m_RandSegments; rIndex += 1)
            {
                indicesData.push_back(getIndex(hIndex, rIndex));
                indicesData.push_back(getIndex(hIndex + 1, rIndex));
                indicesData.push_back(getIndex(hIndex + 1, rIndex + 1));

                indicesData.push_back(getIndex(hIndex, rIndex));
                indicesData.push_back(getIndex(hIndex, rIndex - 1));
                indicesData.push_back(getIndex(hIndex + 1, rIndex));
            }
        }

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBOID);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indicesData.size(), indicesData.data(), GL_STATIC_DRAW);

        m_VerticesCount = indicesData.size();

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
        glEnableVertexAttribArray(0);

        glBindVertexArray(0);
    }

    virtual ~RenderableSphere()
    {
        glDeleteProgram(m_ShaderProgramID);

        glDeleteVertexArrays(1, &m_VAOID);
        glDeleteBuffers(1, &m_VBOID);
        glDeleteBuffers(1, &m_EBOID);
    }

public:
    virtual void Render(float R)
    {
        glUseProgram(m_ShaderProgramID);

        glm::vec3 cameraLocation(G_CAMERA_X, G_CAMERA_Y, G_CAMERA_Z);
        glm::vec3 cameraTarget(G_TARGET_X, G_TARGET_Y, G_TARGET_Z);

        auto projection = glm::perspective(glm::radians(60.0f), R, 0.1f, 100.0f);
        auto view = glm::lookAt(cameraLocation, cameraTarget, glm::vec3(0, 1, 0));

        auto model = glm::mat4(1.0);
        model = glm::translate(model, Convert(m_Center));

        glUniformMatrix4fv(glGetUniformLocation(m_ShaderProgramID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(m_ShaderProgramID, "view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(m_ShaderProgramID, "model"), 1, GL_FALSE, glm::value_ptr(model));

        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

        glBindVertexArray(m_VAOID);
        glDrawElements(GL_TRIANGLES, m_VerticesCount, GL_UNSIGNED_INT, 0);

        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

public:
    void MoveTo(const Point& newLocation)
    {
        m_Center = newLocation;
    }

    void Move(const Direction& Offset)
    {
        m_Center += Offset;
    }
};