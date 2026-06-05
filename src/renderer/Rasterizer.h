#pragma once

#include"renderer/FrameBuffer.h"
#include"core/MathTypes.h"
#include"geometry/HalfEdgeMesh.h"
#include"Shader.h" 
class Rasterizer {
public:
    explicit Rasterizer(FrameBuffer &buffer);
    void drawTriangle2D(const Vec2d &p0,const Vec2d &p1,const Vec2d &p2,const Vec3d &color);
    
    void drawTriangle(const Vec3d &p0,const Vec3d &p1,const Vec3d &p2,const Vec3d &color);
    void drawTriangle3D(const Vec3d &p0,const Vec3d &p1,const Vec3d &p2,const Mat4d &MVP,const Vec3d &color);
    void drawMesh(const HalfEdgeMesh &mesh, const Mat4d &MVP, const Vec3d &color);

    void drawTriangle(const VertexOutput &o0,const VertexOutput &o1,const VertexOutput &o2,const Shader &shader);
    void drawTriangle3D(const VertexInput &p0,const VertexInput &p1,const VertexInput &p2, const Shader &shader);
    void drawMesh(const HalfEdgeMesh &mesh, const Shader &shader);

private:
    FrameBuffer& buffer;
    bool insideClipSpace(const Vec4d& clip) const;
    bool clipToNDC(const Vec4d &clip, Vec3d &ndc) const;
    Vec3d NDCToScreen(const Vec3d &pos) const;
    bool transPoint(const Vec3d &p, const Mat4d &MVP, Vec3d &screenPos) const;
};