#pragma once

#ifdef IMPORT_FLUID_API
#define FLUID_API 
#else
#define FLUID_API __declspec(dllexport)
#endif

#include<Eigen/Eigen>

#include<vector>
#include<memory>

extern "C" FLUID_API void FluidAPI_SetParticleBuffer(std::shared_ptr<std::vector<Eigen::Vector3d>> ParticleBuffer);

extern "C" FLUID_API void FluidAPI_Step(double DeltaTime);