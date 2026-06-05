#include"Shader.h"
#include"cassert"
#include"iostream"
PhongShader::PhongShader(const Mat4d& MVP, const Mat4d& Model, const Vec3d& baseColor, const Vec3d& lightPos, const Vec3d& cameraPos)
: MVP(MVP),Model(Model),baseColor(baseColor),lightPos(lightPos),cameraPos(cameraPos){
    Mat3d M3 = Model.block<3, 3>(0, 0);
    normalMatrix = M3.inverse().transpose();
}

VertexOutput PhongShader::vertex(const VertexInput& input) const{
    VertexOutput o;
    Vec4d local(input.position.x(),input.position.y(),input.position.z(),1.0);

    o.clipPosition = MVP*local;
    Vec4d worldPos=Model*local;

    o.worldPosition=Vec3d(worldPos.x(),worldPos.y(),worldPos.z());
    o.normal=(normalMatrix*input.vertNormal).normalized();
    o.color=input.color;
    return o;
}

Vec3d PhongShader::fragment(const VertexOutput& input) const{
    Vec3d n=input.normal.normalized();

    Vec3d l=(lightPos-input.worldPosition).normalized();
    Vec3d v=(cameraPos-input.worldPosition).normalized();
    Vec3d h=(l+v).normalized();

    double ambient=ka;
    double diffuse=kd*std::max(0.0,n.dot(l));
    // assert(n.dot(l)<=0);
    //std::cout<<n.dot(l)<<'\n';
    double specular=ks*pow(std::max(0.0,n.dot(h)),shininess);

    Vec3d color=baseColor*(ambient+diffuse)+lightColor*specular;
    return color;
}