#include "renderer/Texture.h"

#include <fstream>
#include <iostream>
#include <string>
#include <algorithm>

Vec3d Texture::sample(double u, double v) const{
    return sampleBilinear(u, v);
}

Vec3d Texture::sampleNearest(double u, double v) const{
    if (!isValid()) return Vec3d(1, 1, 1);

    u=std::clamp(u, 0.0, 1.0);
    v=std::clamp(v, 0.0, 1.0);

    int x=(int)(u*(width-1));
    int y=(int)((1.0-v)*(height-1));

    return getPixel(x, y);
}

Vec3d Texture::sampleBilinear(double u, double v) const{
    if (!isValid()) {
        return Vec3d(1.0, 1.0, 1.0);
    }

    u=std::clamp(u, 0.0, 1.0);
    v=std::clamp(v, 0.0, 1.0);

    double x=u*(width-1);
    double y=(1.0-v)*(height-1);

    int x0=(int)(std::floor(x));
    int y0=(int)(std::floor(y));
    int x1=std::min(x0+1, width-1);
    int y1=std::min(y0+1, height-1);

    double tx=x-x0;
    double ty=y-y0;

    Vec3d c00=getPixel(x0, y0);
    Vec3d c10=getPixel(x1, y0);
    Vec3d c01=getPixel(x0, y1);
    Vec3d c11=getPixel(x1, y1);

    Vec3d c0=c00*(1.0-tx)+c10*tx;
    Vec3d c1=c01*(1.0-tx)+c11*tx;
    Vec3d c=c0*(1.0-ty)+c1*ty;

    return c;
}

int Texture::getWidth() const{
    return width;
}

int Texture::getHeight() const{
    return height;
}

bool Texture::isValid() const{
    return width>0&&height>0&&!data.empty();
}

Vec3d Texture::getPixel(int x, int y) const{
    return data[y*width+x];
}

static bool readPPMToken(std::istream& in, std::string& token) {
    token.clear();
    while (in >> token) {
        if (!token.empty() && token[0] == '#') {
            std::string dummy;
            std::getline(in, dummy);
            continue;
        }
        return true;
    }
    return false;
}

bool Texture::loadPPM(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);

    if (!file.is_open()) {
        std::cerr << "无法打开纹理文件: " << filename << '\n';
        return false;
    }

    std::string magic;
    if (!readPPMToken(file, magic)) return false;

    if (magic != "P3" && magic != "P6") {
        std::cerr << "不支持的 PPM 格式: " << magic << '\n';
        return false;
    }

    std::string token;

    if (!readPPMToken(file, token)) return false;
    width = std::stoi(token);

    if (!readPPMToken(file, token)) return false;
    height = std::stoi(token);

    if (!readPPMToken(file, token)) return false;
    int maxValue = std::stoi(token);

    if (width <= 0 || height <= 0 || maxValue <= 0) {
        std::cerr << "PPM 尺寸或 maxValue 非法\n";
        return false;
    }

    if (maxValue > 255) {
        std::cerr << "暂不支持 maxValue > 255 的 PPM\n";
        return false;
    }

    data.clear();
    data.resize(width * height);

    if (magic == "P3") {
        // 文本格式
        for (int i = 0; i < width * height; ++i) {
            std::string rs, gs, bs;

            if (!readPPMToken(file, rs)) return false;
            if (!readPPMToken(file, gs)) return false;
            if (!readPPMToken(file, bs)) return false;

            double r = std::stoi(rs) / static_cast<double>(maxValue);
            double g = std::stoi(gs) / static_cast<double>(maxValue);
            double b = std::stoi(bs) / static_cast<double>(maxValue);

            data[i] = Vec3d(r, g, b);
        }
    } else {
        // P6 二进制格式
        file.get();

        for (int i = 0; i < width * height; ++i) {
            unsigned char rgb[3];

            if (!file.read(reinterpret_cast<char*>(rgb), 3)) {
                std::cerr << "PPM 像素数据不足\n";
                return false;
            }

            double r = rgb[0] / static_cast<double>(maxValue);
            double g = rgb[1] / static_cast<double>(maxValue);
            double b = rgb[2] / static_cast<double>(maxValue);

            data[i] = Vec3d(r, g, b);
        }
    }
    return true;
}