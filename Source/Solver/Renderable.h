#pragma once

#include"MathAIO.h"
#include"ToolAIO.h"

class Renderable
{
public:
    Renderable() = default;
    virtual ~Renderable() = default;

public:
    virtual void Render() = 0;
};

class RenderableSprite : public Renderable { /* Nothing */ };

class RenderableSphere : Renderable
{
protected:
    double m_Radius;
    int m_HeightSegment;
    int m_RandSegment;

public:
    RenderableSphere() = default;
    virtual ~RenderableSphere() = default;

public:
    virtual void Render()
    {

    }
};