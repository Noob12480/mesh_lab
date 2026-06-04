#include<iostream>
#include<windows.h>
#include<Eigen/Core>
#include<Eigen/Geometry>
#include"ObjIO.h"
#include"string"
#include"HalfEdgeMesh.h"
#include"renderer/Rasterizer.h"
void testHalfEdgeMash(){
    std::string filename="assets/coca-cola.obj";
    ObjIO io;
    Mesh mesh;
    io.load(filename,mesh);
    
    // int target = 64;
    // int count = 0;

    // for (int fi = 0; fi < mesh.faces.size(); ++fi) {
    //     for (auto idx : mesh.faces[fi].indices) {
    //         if (idx.v == target) {
    //             std::cout << "internal vertex " << target
    //                     << " used in face " << fi << '\n';
    //             count++;
    //         }
    //     }
    // }

    // std::cout << "used count = " << count << '\n';
    
    HalfEdgeMesh halfEdgeMesh;
    halfEdgeMesh.buildFromMesh(mesh);
    std::cout<<"网格合法性验证:\n"<<halfEdgeMesh.validate()<<'\n';

    std::vector<int> v1=halfEdgeMesh.faceVertices(0);
    std::cout<<"面0的顶点:"<<'\n';
    for(auto v:v1){
        std::cout<<v<<' ';
    }
    std::cout<<'\n';

    std::vector<int> e1=halfEdgeMesh.faceEdges(0);
    std::cout<<"面0的边:"<<'\n';
    for(auto e:e1){
        std::cout<<e<<' ';
    }
    std::cout<<'\n';

    int prevVert=halfEdgeMesh.edgeOrigin(0);
    int targetVert=halfEdgeMesh.edgeTarget(0);
    std::cout<<"边0: "<<prevVert<<' '<<targetVert<<'\n';

    std::vector<int> v2=halfEdgeMesh.vertexNeighbors(290);
    std::cout<<"顶点290的邻接顶点:"<<'\n';
    for(auto v:v2){
        std::cout<<v<<' ';
    }
    std::cout<<'\n'; 

    std::vector<int> e2=halfEdgeMesh.vertexEdges(290);
    std::cout<<"顶点290的邻接边:"<<'\n';
    for(auto e:e2){
        std::cout<<e<<' ';
    }
    std::cout<<'\n'; 

    std::vector<int> f1=halfEdgeMesh.vertexFaces(290);
    std::cout<<"顶点290的邻接面:"<<'\n';
    for(auto f:f1){
        std::cout<<f<<' ';
    }
    std::cout<<'\n'; 
}
void testRenderer(){
    FrameBuffer buffer(512,512);
    buffer.clearDepth(1.0);
    Rasterizer rasterizer(buffer);

    Vec3d p0(20,40,0.99);
    Vec3d p1(300,400,00.2);
    Vec3d p2(500,200,0.1);
    Vec3d color(0,1,0);
    rasterizer.drawTriangle(p0,p1,p2,color);

    Vec3d p3(0,0,0.3);
    Vec3d p4(480,300,0.2);
    Vec3d p5(200,500,0.05);
    Vec3d color1(1,1,0);
    rasterizer.drawTriangle(p3,p4,p5,color1);

    buffer.savePPM("test.ppm");
}
int main() {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    testRenderer();
    //testHalfEdgeMash();

    return 0;
}