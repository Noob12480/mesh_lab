#pragma once

#include"core/MathTypes.h"
#include<string>
#include<vector>

class Texture {
public:
    Texture() = default;

    bool loadPPM(const std::string& filename);

    Vec3d sample(double u, double v) const;
    Vec3d sampleNearest(double u, double v) const;
    Vec3d sampleBilinear(double u, double v) const;

    int getWidth() const;
    int getHeight() const;
    bool isValid() const;

private:
    int width=0;
    int height=0;

    std::vector<Vec3d> data;
    Vec3d getPixel(int x, int y) const;
};