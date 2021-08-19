#include"Backend.cuh"

__global__ void UpdateAccelerationKernel(UniversityCUDA_API_Planet* Src, UniversityCUDA_API_Planet* Dst, int Size, float Delta)
{
    int ID = threadIdx.x + blockDim.x * blockIdx.x;

    if(ID < Size)
    {
        Dst[ID].Acceleration.x = 0.0f;
        Dst[ID].Acceleration.y = 0.0f;
        Dst[ID].Acceleration.z = 0.0f;

        int otherIdx = 0;

        UniversityCUDA_API_Planet Me = Src[ID];

        while(otherIdx < Size)
        {
            // if(otherIdx != ID)
            {
                float dX = Src[otherIdx].Location.x - Me.Location.x;
                float dY = Src[otherIdx].Location.y - Me.Location.y;
                float dZ = Src[otherIdx].Location.z - Me.Location.z;

                float distanceSquare = dX * dX + dY * dY + dZ * dZ;

                float Mom = std::powf(distanceSquare + 0.0001, 1.5);
                float Left = Src[otherIdx].Mass / Mom;

                Dst[otherIdx].Acceleration.x += Left * dX;
                Dst[otherIdx].Acceleration.y += Left * dY;
                Dst[otherIdx].Acceleration.z += Left * dZ;
            }
            otherIdx += 1;
        }
    }
}

void UpdateAcceleration(UniversityCUDA_API_Planet* Src, UniversityCUDA_API_Planet* Dst, int Size, float Delta)
{
    UpdateAccelerationKernel<<<20, 1024>>>(Src,Dst,Size,Delta);
}

__global__ void UpdateLocationKernel(UniversityCUDA_API_Planet* Src, UniversityCUDA_API_Planet* Dst, int Size, float Delta)
{
    int ID = threadIdx.x + blockDim.x * blockIdx.x;

    if(ID < Size)
    {
        Dst[ID].Location.x = Src[ID].Location.x + Src[ID].Speed.x * Delta;
        Dst[ID].Location.y = Src[ID].Location.y + Src[ID].Speed.y * Delta;
        Dst[ID].Location.z = Src[ID].Location.z + Src[ID].Speed.z * Delta;
    }
}

void UpdateLocation(UniversityCUDA_API_Planet* Src, UniversityCUDA_API_Planet* Dst, int Size, float Delta)
{
    UpdateLocationKernel<<<20, 1024>>>(Src,Dst,Size,Delta);
}

__global__ void UpdateSpeedKernel(UniversityCUDA_API_Planet* Src, UniversityCUDA_API_Planet* Dst, int Size, float Delta)
{
    int ID = threadIdx.x + blockDim.x * blockIdx.x;

    if(ID < Size)
    {
        Dst[ID].Speed.x = Src[ID].Speed.x + Src[ID].Acceleration.x * Delta;
        Dst[ID].Speed.y = Src[ID].Speed.y + Src[ID].Acceleration.y * Delta;
        Dst[ID].Speed.z = Src[ID].Speed.z + Src[ID].Acceleration.z * Delta;
    }
}

void UpdateSpeed(UniversityCUDA_API_Planet* Src, UniversityCUDA_API_Planet* Dst, int Size, float Delta)
{
    UpdateSpeedKernel<<<20, 1024>>>(Src,Dst,Size,Delta);
}
