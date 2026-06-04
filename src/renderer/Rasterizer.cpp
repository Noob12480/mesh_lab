#include"Rasterizer.h"
#include"iostream"
Rasterizer::Rasterizer(FrameBuffer &buffer) : buffer(buffer){}

//叉乘的正负
bool getCrossDir(const Vec2d &v1, const Vec2d &v2){
    return v1.x()*v2.y()-v2.x()*v1.y()>0;
}
double cross(const Vec2d &v1, const Vec2d &v2){
    return v1.x()*v2.y()-v2.x()*v1.y();
}
void Rasterizer::drawTriangle2D(const Vec2d& p0,const Vec2d& p1,const Vec2d& p2,const Vec3d& color){
    //AABB
    int minX,minY,maxX,maxY;
    minX=std::min(std::floor(p0.x()),std::min(std::floor(p1.x()),std::floor(p2.x())));
    minY=std::min(std::floor(p0.y()),std::min(std::floor(p1.y()),std::floor(p2.y())));
    maxX=std::max(std::ceil(p0.x()),std::max(std::ceil(p1.x()),std::ceil(p2.x())));
    maxY=std::max(std::ceil(p0.y()),std::max(std::ceil(p1.y()),std::ceil(p2.y())));

    //收缩到窗口内
    minX=std::max(minX,0);
    minY=std::max(minY,0);
    maxX=std::min(maxX,buffer.getWidth());
    maxY=std::min(maxY,buffer.getHeight());

    for(int y=minY;y<=maxY;y++){
        for(int x=minX;x<=maxX;x++){
            //叉乘
            Vec2d p(x+0.5,y+0.5);
            
            Vec2d p0p=p0-p;
            Vec2d p1p=p1-p;
            Vec2d p2p=p2-p;
            Vec2d p0p1=p1-p0;
            Vec2d p1p2=p2-p1;
            Vec2d p2p0=p0-p2;

            if(getCrossDir(p0p,p0p1)==getCrossDir(p1p,p1p2)&&getCrossDir(p1p,p1p2)==getCrossDir(p2p,p2p0)){
                buffer.setPixel(x,y,color);
            }
        }
    }
}

void Rasterizer::drawTriangle(const Vec3d& p0,const Vec3d& p1,const Vec3d& p2,const Vec3d& color){
    //AABB
    int minX,minY,maxX,maxY;
    minX=std::min(std::floor(p0.x()),std::min(std::floor(p1.x()),std::floor(p2.x())));
    minY=std::min(std::floor(p0.y()),std::min(std::floor(p1.y()),std::floor(p2.y())));
    maxX=std::max(std::ceil(p0.x()),std::max(std::ceil(p1.x()),std::ceil(p2.x())));
    maxY=std::max(std::ceil(p0.y()),std::max(std::ceil(p1.y()),std::ceil(p2.y())));

    //收缩到窗口内
    minX=std::max(minX,0);
    minY=std::max(minY,0);
    maxX=std::min(maxX,buffer.getWidth());
    maxY=std::min(maxY,buffer.getHeight());

    for(int y=minY;y<=maxY;y++){
        for(int x=minX;x<=maxX;x++){
            //叉乘
            Vec2d p(x+0.5,y+0.5);
            
            Vec2d p0p(p.x()-p0.x(),p.y()-p0.y());
            Vec2d p1p(p.x()-p1.x(),p.y()-p1.y());
            Vec2d p2p(p.x()-p2.x(),p.y()-p2.y());
            Vec2d p0p1(p1.x()-p0.x(),p1.y()-p0.y());
            Vec2d p1p2(p2.x()-p1.x(),p2.y()-p1.y());
            Vec2d p2p0(p0.x()-p2.x(),p0.y()-p2.y());

            if(getCrossDir(p0p,p0p1)==getCrossDir(p1p,p1p2)&&getCrossDir(p1p,p1p2)==getCrossDir(p2p,p2p0)){
                //重心坐标插值
                double ap2=cross(p0p,p1p);
                double ap0=cross(p1p,p2p);
                double ap1=cross(p2p,p0p);
                double a=ap0+ap1+ap2;
                if (std::abs(a)<1e-8) continue;

                double depth=(ap0*p0.z()+ap1*p1.z()+ap2*p2.z())/a;
                
                if(buffer.depthTest(x,y,depth)){
                    buffer.setDepth(x,y,depth);
                    buffer.setPixel(x,y,color);
                    //std::cout<<"1111"<<'\n';
                }
            }
        }
    }
}