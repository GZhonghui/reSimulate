#pragma once

#include"MathAIO.h"
#include"ToolAIO.h"

class Particle
{
public:
    Particle() = default;
    ~Particle() = default;

public:
    bool m_fluids;
    double m_mass;
    double m_lambda;
    double m_density;
    Eigen::Vector3d m_deltaPos;
    Eigen::Vector3d m_position;
    Eigen::Vector3d m_lastPosition;
    Eigen::Vector3d m_velocity;
    std::vector<unsigned int> m_neighborhood;
};