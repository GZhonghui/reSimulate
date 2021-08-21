#pragma once

#pragma warning(disable:26495)
#pragma warning(disable:26451)

#include"Config.h"

#include<unordered_map>
#include<unordered_set>
#include<algorithm>
#include<cstdarg>
#include<cstdint>
#include<cstring>
#include<string>
#include<chrono>
#include<vector>
#include<cstdio>
#include<ctime>
#include<cmath>

#include<imgui.h>
#include<imgui_impl_glfw.h>
#include<imgui_impl_opengl3.h>

#include<glad/glad.h>

#include<GLFW/glfw3.h>

#include<STB/stb_image.h>
#include<STB/stb_image_write.h>

#define IMPORT_FLUID_API
#include"../Fluid/Fluid.h"

#define IMPORT_UNIVERSITY_CUDA_API
#include"../UniversityCUDA/UniversityCUDA.h"

#define IMPORT_RIGIDBODY_API
#include"../Rigidbody/Rigidbody.h"

enum class pType
{
    MESSAGE, WARNING, ERROR
};

class Out
{
public:
    Out() = default;
    ~Out() = default;

private:
    static void printTime()
    {
        time_t nowTime = time(nullptr);
        tm* ltm = localtime(&nowTime);

        printf("[%02d:%02d:%02d]", ltm->tm_hour, ltm->tm_min, ltm->tm_sec);
    }

public:
    static void Log(pType Type, const char* Format, ...)
    {
        switch (Type)
        {
        case pType::MESSAGE:
            printf("[MESSAGE]");
            break;
        case pType::WARNING:
            printf("[WARNING]");
            break;
        case pType::ERROR:
            printf("[ ERROR ]");
            break;
        }
        printf(" "); printTime(); printf(" >>");

        va_list args;

        va_start(args, Format);
        vprintf(Format, args);
        va_end(args);

        puts("");
    }
};