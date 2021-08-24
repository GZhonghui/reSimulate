#include"Rigidbody.h"

#include"MathAIO.h"
#include"ToolAIO.h"

#include"RigidbodyCore.h"

RigidbodyCore Core;

extern "C" RIGIDBODY_API void RigidbodyAPI_Init(std::vector<std::shared_ptr<RigidbodyAPIObject>>* objBuffer)
{
    Core.Init(objBuffer);
}

RIGIDBODY_API void RigidbodyAPI_Step(double DeltaTime)
{
    Core.UpdateAcceleration(DeltaTime);
    Core.UpdateLocation(DeltaTime);
    Core.UpdateSpeed(DeltaTime);
}

RIGIDBODY_API void RigidbodyAPI_Exit()
{
    Core.Exit();
}