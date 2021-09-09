#pragma once

#ifdef IMPORT_RIGIDBODY_API
#define RIGIDBODY_API 
#else
#define RIGIDBODY_API __declspec(dllexport)
#endif

#include<Eigen/Eigen>

#include<vector>
#include<memory>

enum class RigidbodyAPIObjectType
{
    SPHERE
};

class RigidbodyAPIObject
{
protected:
    RigidbodyAPIObjectType m_Type;

protected:
    Eigen::Vector3d m_Center;
    Eigen::Vector3d m_Speed;
    Eigen::Vector3d m_Acceleration;

    double m_Mass;
    double m_K;

public:
    RigidbodyAPIObject(RigidbodyAPIObjectType Type, const Eigen::Vector3d& Center, double Mass, double K) :
        m_Type(Type), m_Center(Center), m_Mass(Mass), m_K(K), m_Speed(0), m_Acceleration(0) {}
    virtual ~RigidbodyAPIObject() = default;

public:
    RigidbodyAPIObjectType getType() const noexcept { return m_Type; }

public:
    Eigen::Vector3d getLocation() const { return m_Center; }
    void setLocation(const Eigen::Vector3d& Location) { m_Center = Location; }
    void Move(const Eigen::Vector3d& Offset) { m_Center += Offset; }

public:
    virtual Eigen::Vector3d getForce(std::shared_ptr<RigidbodyAPIObject> otherObj) = 0;
    virtual Eigen::Vector3d getForceWithPlane() = 0;
    virtual Eigen::Vector3d getForceWithWall() = 0;

public:
    friend class RigidbodyCore;
};

class RigidbodyAPISphere : public RigidbodyAPIObject
{
protected:
    double m_Radius;

public:
    RigidbodyAPISphere(const Eigen::Vector3d& Center, double Mass, double K, double Radius) :
        RigidbodyAPIObject(RigidbodyAPIObjectType::SPHERE, Center, Mass, K),
        m_Radius(Radius) {}
    virtual ~RigidbodyAPISphere() = default;

public:
    virtual Eigen::Vector3d getForce(std::shared_ptr<RigidbodyAPIObject> otherObj)
    {
        if (!otherObj) return Eigen::Vector3d(0);

        switch (otherObj->getType())
        {
        case RigidbodyAPIObjectType::SPHERE:
        {
            RigidbodyAPISphere* otherSphere = (RigidbodyAPISphere*)(otherObj.get());
            double Distance = (m_Center - otherSphere->m_Center).norm();

            if (Distance < m_Radius + otherSphere->m_Radius)
            {
                double overL = m_Radius + otherSphere->m_Radius - Distance;
                double thisL = overL * otherSphere->m_K / (m_K + otherSphere->m_K);

                Eigen::Vector3d ForceDir = (m_Center - otherSphere->m_Center).normalized();
                double ForceValue = thisL * m_K;

                return ForceDir * ForceValue;
            }
        }break;
        }

        return Eigen::Vector3d(0);
    }

    virtual Eigen::Vector3d getForceWithPlane()
    {
        if (m_Center.y() > m_Radius) return Eigen::Vector3d(0);

        // Upon the Plane

        double overL = m_Radius - m_Center.y();
        double ForceValue = m_K * overL;

        return Eigen::Vector3d(0, 1, 0) * ForceValue;
    }

    virtual Eigen::Vector3d getForceWithWall()
    {
        auto SumForce = Eigen::Vector3d(0, 0, 0);

        const double Limit = 8.0;
        
        if (m_Center.x() + m_Radius > Limit)
        {
            SumForce += (m_Center.x() + m_Radius - Limit) * m_K * Eigen::Vector3d(-1, 0, 0);
        }
        else if (m_Center.x() - m_Radius < -Limit)
        {
            SumForce += (-Limit - (m_Center.x() - m_Radius)) * m_K * Eigen::Vector3d(1, 0, 0);
        }

        if (m_Center.z() + m_Radius > Limit)
        {
            SumForce += (m_Center.z() + m_Radius - Limit) * m_K * Eigen::Vector3d(0, 0, -1);
        }
        else if (m_Center.z() - m_Radius < -Limit)
        {
            SumForce += (-Limit - (m_Center.z() - m_Radius)) * m_K * Eigen::Vector3d(0, 0, 1);
        }

        return SumForce;
    }

public:
    friend class RigidbodyCore;
};

extern "C" RIGIDBODY_API void RigidbodyAPI_Init(std::vector<std::shared_ptr<RigidbodyAPIObject>>* objBuffer);

extern "C" RIGIDBODY_API void RigidbodyAPI_Step(double DeltaTime);

extern "C" RIGIDBODY_API void RigidbodyAPI_Exit();