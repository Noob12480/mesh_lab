#pragma once

#include "core/MathTypes.h"
#include "renderer/Texture.h"

class Material {
public:
    Material() = default;

    void setBaseColor(const Vec3d& color);
    void setTexture(const Texture* texture);

    Vec3d baseColorAt(const Vec2d& uv) const;

    bool hasTexture() const;

private:
    Vec3d baseColor=Vec3d(1.0, 1.0, 1.0);
    const Texture* texture=nullptr;
};