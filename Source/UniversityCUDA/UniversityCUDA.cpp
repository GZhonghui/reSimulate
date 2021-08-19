#include"UniversityCUDA.h"

#include"Backend.cuh"

int G_PLANET_SIZE;

UniversityCUDA_API_Planet* G_DeviceReadBuffer;
UniversityCUDA_API_Planet* G_DeviceWriteBuffer;

UNIVERSITY_CUDA_API void UniversityCUDA_API_Init(UniversityCUDA_API_Planet* dataBuffer, int bufferSize)
{
    G_PLANET_SIZE = bufferSize;

    cudaSetDevice(0);

    cudaMalloc((void**)&G_DeviceReadBuffer, sizeof(UniversityCUDA_API_Planet) * G_PLANET_SIZE);
    cudaMalloc((void**)&G_DeviceWriteBuffer, sizeof(UniversityCUDA_API_Planet) * G_PLANET_SIZE);

    cudaMemcpy(G_DeviceReadBuffer, dataBuffer, sizeof(UniversityCUDA_API_Planet) * G_PLANET_SIZE, cudaMemcpyHostToDevice);

    Out::Log(pType::MESSAGE, "Initing CUDA");

    Out::Log(pType::MESSAGE, "Planets Count : %d", G_PLANET_SIZE);
}

UNIVERSITY_CUDA_API void UniversityCUDA_API_Step(UniversityCUDA_API_Planet* dataBuffer, float DeltaTime)
{
    UpdateAcceleration(G_DeviceReadBuffer, G_DeviceWriteBuffer, G_PLANET_SIZE, DeltaTime);
    cudaDeviceSynchronize();

    UpdateLocation(G_DeviceReadBuffer, G_DeviceWriteBuffer, G_PLANET_SIZE, DeltaTime);
    cudaDeviceSynchronize();

    UpdateSpeed(G_DeviceReadBuffer, G_DeviceWriteBuffer, G_PLANET_SIZE, DeltaTime);
    cudaDeviceSynchronize();

    cudaMemcpy(dataBuffer, G_DeviceWriteBuffer, sizeof(UniversityCUDA_API_Planet) * G_PLANET_SIZE, cudaMemcpyDeviceToHost);

    std::swap(G_DeviceReadBuffer, G_DeviceWriteBuffer);
}

UNIVERSITY_CUDA_API void UniversityCUDA_API_Exit()
{
    cudaFree(G_DeviceReadBuffer);
    cudaFree(G_DeviceWriteBuffer);

    cudaDeviceReset();

    Out::Log(pType::MESSAGE, "Cleaning CUDA");
}