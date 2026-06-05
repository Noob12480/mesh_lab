#include"Rasterizer.h"
#include"iostream"
Rasterizer::Rasterizer(FrameBuffer &buffer) : buffer(buffer){}

//叉乘
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
            
            double c0=cross(p0p,p0p1);
            double c1=cross(p1p,p1p2);
            double c2=cross(p2p,p2p0);
            if((c0>=0&&c1>=0&&c2>=0)||(c0<=0&&c1<=0&&c2<=0)){
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
    maxX=std::min(maxX,buffer.getWidth()-1);
    maxY=std::min(maxY,buffer.getHeight()-1);

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

            double c0=cross(p0p,p0p1);
            double c1=cross(p1p,p1p2);
            double c2=cross(p2p,p2p0);
            if((c0>=0&&c1>=0&&c2>=0)||(c0<=0&&c1<=0&&c2<=0)){
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

void Rasterizer::drawTriangle3D(const Vec3d& p0,const Vec3d& p1,const Vec3d& p2,const Mat4d& MVP,const Vec3d& color){
    //3d tri->2d tri
    Vec3d sp0,sp1,sp2;
    if(!transPoint(p0,MVP,sp0)||!transPoint(p1,MVP,sp1)||!transPoint(p2,MVP,sp2))return;
    //std::cout<<sp0<<' '<<sp1<<' '<<sp2<<' '<<'\n';
    drawTriangle(sp0,sp1,sp2,color);
}

void Rasterizer::drawMesh(const HalfEdgeMesh &mesh, const Mat4d &MVP, const Vec3d &color){
    for(int i=0;i<mesh.getFaces().size();i++){
        std::vector<int> vi=mesh.faceVertices(i);

        if(vi.size()<3) continue;
        int o=vi[0];
        const HEVert &v0=mesh.getVertices().at(o);
        Vec3d p0(v0.x,v0.y,v0.z);

        for(int j=1;j+1<vi.size();j++){
            int vertId1=vi[j];
            int vertId2=vi[j+1];

            const HEVert &v1=mesh.getVertices().at(vertId1);
            const HEVert &v2=mesh.getVertices().at(vertId2);
            
            Vec3d p1(v1.x,v1.y,v1.z);
            Vec3d p2(v2.x,v2.y,v2.z);

            drawTriangle3D(p0,p1,p2,MVP,color);
        }
    }
}

void Rasterizer::drawTriangle(const VertexOutput& o0,const VertexOutput& o1,const VertexOutput& o2,const Shader &shader){
    //AABB
    Vec3d p0,p1,p2;
    p0=NDCToScreen(clipToNDC(o0.clipPosition));
    p1=NDCToScreen(clipToNDC(o1.clipPosition));
    p2=NDCToScreen(clipToNDC(o2.clipPosition));

    int minX,minY,maxX,maxY;
    minX=std::min(std::floor(p0.x()),std::min(std::floor(p1.x()),std::floor(p2.x())));
    minY=std::min(std::floor(p0.y()),std::min(std::floor(p1.y()),std::floor(p2.y())));
    maxX=std::max(std::ceil(p0.x()),std::max(std::ceil(p1.x()),std::ceil(p2.x())));
    maxY=std::max(std::ceil(p0.y()),std::max(std::ceil(p1.y()),std::ceil(p2.y())));

    //收缩到窗口内
    minX=std::max(minX,0);
    minY=std::max(minY,0);
    maxX=std::min(maxX,buffer.getWidth()-1);
    maxY=std::min(maxY,buffer.getHeight()-1);

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

            double c0=cross(p0p,p0p1);
            double c1=cross(p1p,p1p2);
            double c2=cross(p2p,p2p0);
            if((c0>=0&&c1>=0&&c2>=0)||(c0<=0&&c1<=0&&c2<=0)){
                //深度插值
                double ap2=cross(p0p,p1p);
                double ap0=cross(p1p,p2p);
                double ap1=cross(p2p,p0p);
                double a=ap0+ap1+ap2;
                if (std::abs(a)<1e-8) continue;

                double depth=(ap0*p0.z()+ap1*p1.z()+ap2*p2.z())/a;

                //插值
                VertexOutput o;
                o.worldPosition=(ap0*o0.worldPosition+ap1*o1.worldPosition+ap2*o2.worldPosition)/a;
                o.normal=(ap0*o0.normal+ap1*o1.normal+ap2*o2.normal)/a;
                o.color=(ap0*o0.color+ap1*o1.color+ap2*o2.color)/a;
                
                Vec3d color=shader.fragment(o);

                if(buffer.depthTest(x,y,depth)){
                    buffer.setDepth(x,y,depth);
                    buffer.setPixel(x,y,color);
                    //std::cout<<"1111"<<'\n';
                }
            }
        }
    }
}

void Rasterizer::drawTriangle3D(const Vec3d &p0,const Vec3d &p1,const Vec3d &p2,const Shader &shader){
    Vec3d n=(p1-p0).cross(p2-p1).normalized();

    const VertexInput v0={p0,n},v1={p1,n},v2={p2,n};

    VertexOutput o0,o1,o2;
    o0=shader.vertex(v0);
    o1=shader.vertex(v1);
    o2=shader.vertex(v2);

    if(o0.clipPosition.w()>0||o1.clipPosition.w()>0||o2.clipPosition.w()>0){
        return;
    }

    drawTriangle(o0,o1,o2,shader);
}

void Rasterizer::drawMesh(const HalfEdgeMesh &mesh, const Shader &shader){
    for(int i=0;i<mesh.getFaces().size();i++){
        std::vector<int> vi=mesh.faceVertices(i);

        if(vi.size()<3) continue;
        int o=vi[0];
        const HEVert &v0=mesh.getVertices().at(o);
        Vec3d p0(v0.x,v0.y,v0.z);

        for(int j=1;j+1<vi.size();j++){
            int vertId1=vi[j];
            int vertId2=vi[j+1];

            const HEVert &v1=mesh.getVertices().at(vertId1);
            const HEVert &v2=mesh.getVertices().at(vertId2);
            
            Vec3d p1(v1.x,v1.y,v1.z);
            Vec3d p2(v2.x,v2.y,v2.z);

            drawTriangle3D(p0,p1,p2,shader);
        }
    }
}

Vec3d Rasterizer::clipToNDC(const Vec4d& clip) const{
    if(std::abs(clip.w())<1e-8) {
        //std::cerr<<"w值为0"<<'\n';
        return Vec3d(0,0,0);
    }
    return Vec3d(clip.x()/clip.w(), clip.y()/clip.w(), clip.z()/clip.w());
}

Vec3d Rasterizer::NDCToScreen(const Vec3d &pos) const{
    return Vec3d((pos.x()+1.0)*0.5*(buffer.getWidth()-1), (-pos.y()+1.0)*0.5*(buffer.getHeight()-1), (-pos.z()+1.0)*0.5);
}

bool Rasterizer::transPoint(const Vec3d &p, const Mat4d &MVP, Vec3d &screenPos) const{
    Vec4d pp(p.x(),p.y(),p.z(),1);

    Vec4d clip=MVP*pp;

    if (clip.w() >= 0) {
        // std::cout<<"存在距离为负数的平面: "<<clip.w()<<'\n';
        return false;
    }

    Vec3d ndc=clipToNDC(clip);
    screenPos=NDCToScreen(ndc);
    return true;
}
