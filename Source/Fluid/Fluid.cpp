#include"Fluid.h"

#include"Config.h"
#include"PBF.h"

std::shared_ptr<std::vector<Eigen::Vector3d>> G_ParticleBuffer;

FLUID_API void FluidAPI_SetParticleBuffer(std::shared_ptr<std::vector<Eigen::Vector3d>> ParticleBuffer)
{
    G_ParticleBuffer = ParticleBuffer;
}

FLUID_API void FluidAPI_Step(double DeltaTime)
{
    if (!G_ParticleBuffer) return;

    for (auto i = G_ParticleBuffer->begin(); i != G_ParticleBuffer->end(); ++i)
    {
        i->y() = i->y() - DeltaTime * 0.8;
    }
}