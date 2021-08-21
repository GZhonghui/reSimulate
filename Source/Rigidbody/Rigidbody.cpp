#include"Rigidbody.h"

#include"MathAIO.h"
#include"ToolAIO.h"

std::vector<std::shared_ptr<RigidbodyAPIObject>>* G_ObjBuffer;

extern "C" RIGIDBODY_API void RigidbodyAPI_Init(std::vector<std::shared_ptr<RigidbodyAPIObject>>* objBuffer)
{
    G_ObjBuffer = objBuffer;
}

RIGIDBODY_API void RigidbodyAPI_Step(double DeltaTime)
{
    if (!G_ObjBuffer) return;

}

RIGIDBODY_API void RigidbodyAPI_Exit()
{
    // Nothing
}