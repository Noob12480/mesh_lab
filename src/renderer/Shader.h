#pragma once

#include "core/MathTypes.h"

struct VertexInput {
    Vec3d position;
    Vec3d normal;
    Vec3d color;
};

struct VertexOutput {
    Vec4d clipPosition;
    Vec3d worldPosition;
    Vec3d normal;
    Vec3d color;
};

class Shader {
public:
    virtual ~Shader() = default;

    virtual VertexOutput vertex(const VertexInput& input) const=0;
    virtual Vec3d fragment(const VertexOutput& input) const=0;
};

class FlatShader : public Shader {
public:
    FlatShader(const Mat4d& MVP, const Vec3d& baseColor, const Vec3d& lightDir);
    VertexOutput vertex(const VertexInput& input) const;
    Vec3d fragment(const VertexOutput& input) const;

private:
    Mat4d MVP;
    Vec3d baseColor;
    Vec3d lightDir;
};

class PhoneShader : public Shader {
public:
    PhoneShader(const Mat4d& MVP, const Vec3d& baseColor, const Vec3d& lightDir);
    VertexOutput vertex(const VertexInput& input) const;
    Vec3d fragment(const VertexOutput& input) const;

private:
    Mat4d MVP;
    Vec3d baseColor;
    Vec3d lightDir;
};