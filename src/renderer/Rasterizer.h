#pragma once

#include "renderer/FrameBuffer.h"
#include "core/MathTypes.h"

class Rasterizer {
public:
    explicit Rasterizer(FrameBuffer &buffer);
    void drawTriangle2D(const Vec2d& p0,const Vec2d& p1,const Vec2d& p2,const Vec3d& color);
    void drawTriangle(const Vec3d& p0,const Vec3d& p1,const Vec3d& p2,const Vec3d& color);

private:
    FrameBuffer& buffer;
};