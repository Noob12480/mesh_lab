#include"Material.h"
void Material::setBaseColor(const Vec3d& color){
    this->baseColor=color;
}
void Material::setTexture(const Texture* texture){
    this->texture=texture;
}

Vec3d Material::baseColorAt(const Vec2d& uv) const{
    if(hasTexture()){
        return texture->sampleBilinear(uv.x(),uv.y());
    }
    return baseColor;
}

bool Material::hasTexture() const{
    return texture!=nullptr;
}