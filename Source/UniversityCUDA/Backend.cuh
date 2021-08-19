#ifndef CUDA_BACKEND_CUH
#define CUDA_BACKEND_CUH

#include"MathAIO.h"
#include"ToolAIO.h"

#include"UniversityCUDA.h"

void UpdateAcceleration(UniversityCUDA_API_Planet* Src, UniversityCUDA_API_Planet* Dst, int Size, float Delta);

void UpdateLocation(UniversityCUDA_API_Planet* Src, UniversityCUDA_API_Planet* Dst, int Size, float Delta);

void UpdateSpeed(UniversityCUDA_API_Planet* Src, UniversityCUDA_API_Planet* Dst, int Size, float Delta);

#endif