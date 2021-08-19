#pragma once

#ifdef IMPORT_UNIVERSITY_CUDA_API
#define UNIVERSITY_CUDA_API 
#else
#define UNIVERSITY_CUDA_API __declspec(dllexport)
#endif

struct UniversityCUDA_API_Planet
{
    struct Vector
    {
        float x, y, z;
    }; 

    Vector Acceleration;
    Vector Location;
    Vector Speed;

    float Mass;
};

extern "C" UNIVERSITY_CUDA_API void UniversityCUDA_API_Init(UniversityCUDA_API_Planet* dataBuffer, int bufferSize);

extern "C" UNIVERSITY_CUDA_API void UniversityCUDA_API_Step(UniversityCUDA_API_Planet* dataBuffer, float DeltaTime);

extern "C" UNIVERSITY_CUDA_API void UniversityCUDA_API_Exit();