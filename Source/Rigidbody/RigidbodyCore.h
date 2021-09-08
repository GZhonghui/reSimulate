#pragma once

#include"MathAIO.h"
#include"ToolAIO.h"

#include"Rigidbody.h"

class RigidbodyCore
{
protected:
    std::vector<std::shared_ptr<RigidbodyAPIObject>>* m_ObjBuffer;

public:
    RigidbodyCore() :m_ObjBuffer(nullptr) {}
    virtual ~RigidbodyCore() = default;

public:
    void Init(std::vector<std::shared_ptr<RigidbodyAPIObject>>* ObjBuffer)
    {
        m_ObjBuffer = ObjBuffer;
    }

    void Exit() {}

    void UpdateAcceleration(double DeltaTime)
    {
        if (!m_ObjBuffer) return;

        for (int nowObj = 0; nowObj < m_ObjBuffer->size(); nowObj += 1)
        {
            Eigen::Vector3d SumForce(0);

            for (int targetObj = 0; targetObj < m_ObjBuffer->size(); targetObj += 1)
            {
                if (nowObj == targetObj) continue;

                SumForce += (*m_ObjBuffer)[nowObj]->getForce((*m_ObjBuffer)[targetObj]);
            }

            SumForce += (*m_ObjBuffer)[nowObj]->getForceWithPlane();

            // Slow Down
            SumForce -= (*m_ObjBuffer)[nowObj]->m_Speed * 0.9;

            (*m_ObjBuffer)[nowObj]->m_Acceleration = SumForce / (*m_ObjBuffer)[nowObj]->m_Mass;
            (*m_ObjBuffer)[nowObj]->m_Acceleration += Eigen::Vector3d(0, -1, 0) * G;
        }
    }

    void UpdateLocation(double DeltaTime)
    {
        if (!m_ObjBuffer) return;

        for (int nowObj = 0; nowObj < m_ObjBuffer->size(); nowObj += 1)
        {
            (*m_ObjBuffer)[nowObj]->Move((*m_ObjBuffer)[nowObj]->m_Speed * DeltaTime);
        }
    }

    void UpdateSpeed(double DeltaTime)
    {
        if (!m_ObjBuffer) return;

        for (int nowObj = 0; nowObj < m_ObjBuffer->size(); nowObj += 1)
        {
            (*m_ObjBuffer)[nowObj]->m_Speed += (*m_ObjBuffer)[nowObj]->m_Acceleration * DeltaTime;
        }
    }
};