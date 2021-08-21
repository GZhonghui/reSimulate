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

public:
    RigidbodyAPIObject(RigidbodyAPIObjectType Type) :m_Type(Type) {}
    virtual ~RigidbodyAPIObject() = default;

public:
    RigidbodyAPIObjectType getType() const noexcept { return m_Type; }
};

class RigidbodyAPISphere : public RigidbodyAPIObject
{
protected:
    Eigen::Vector3d m_Center;
    double m_Radius;
    double m_K;

public:
    RigidbodyAPISphere(const Eigen::Vector3d& Center, double Radius, double K) :
        RigidbodyAPIObject(RigidbodyAPIObjectType::SPHERE),
        m_Center(Center), m_Radius(Radius), m_K(K) {}
    virtual ~RigidbodyAPISphere() = default;
};

extern "C" RIGIDBODY_API void RigidbodyAPI_Init(std::vector<std::shared_ptr<RigidbodyAPIObject>>* objBuffer);

extern "C" RIGIDBODY_API void RigidbodyAPI_Step(double DeltaTime);

extern "C" RIGIDBODY_API void RigidbodyAPI_Exit();