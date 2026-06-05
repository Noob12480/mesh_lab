#include"Shader.h"

FlatShader::FlatShader(const Mat4d& MVP, const Vec3d& baseColor, const Vec3d& lightDir)
     : MVP(MVP), baseColor(baseColor), lightDir(lightDir.normalized()) {}

VertexOutput FlatShader::vertex(const VertexInput& input) const{
    VertexOutput o;
    o.clipPosition = MVP*Vec4d(input.position.x(),input.position.y(),input.position.z(),1.0);

    o.worldPosition=input.position;
    o.normal=input.normal;
    o.color=input.color;
    return o;
}

Vec3d FlatShader::fragment(const VertexOutput& input) const{
    Vec3d n=input.normal.normalized();
    double ambient=0.2;
    double diffuse=std::max(0.0,n.dot(lightDir));
    double intensity=ambient+0.8*diffuse;
    return baseColor*intensity;
}