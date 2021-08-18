#pragma once

#ifdef IMPORT_UNIVERSITY_CUDA_API
#define UNIVERSITY_CUDA_API 
#else
#define UNIVERSITY_CUDA_API __declspec(dllexport)
#endif

#include<Eigen/Eigen>

#include<vector>
#include<memory>

extern "C" UNIVERSITY_CUDA_API void Test();