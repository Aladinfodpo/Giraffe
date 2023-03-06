#include "mesh.h"

#include <QFile>
#include <QTextStream>
#include <array>

Mesh::Mesh()
{

}

void Mesh::addPoint(const Point3D& p){
    data.push_back(p.x);
    data.push_back(p.y);
    data.push_back(p.z);
    data.push_back(p.u);
    data.push_back(p.v);
    data.push_back(p.nx);
    data.push_back(p.ny);
    data.push_back(p.nz);
}

void Mesh::addDefaultPlane(){
    Point3D p0(-1.f, 1.f, 0.f);
    p0.u = 0.f;
    p0.v = 0.f;

    auto p1 = p0;
    p1.y *= -1.f;
    p1.v = 1.f;

    auto p2 = p1;
    p2.y *= -1.f;
    p2.x *= -1.f;
    p2.v = 0.f;
    p2.u = 1.f;

    auto p3 = p2;
    p3.y *= -1.f;
    p3.v = 1.f;
    addPlane(p0, p2, p3, p1);
}

void Mesh::addTriangle(const Point3D &p0, const Point3D &p1, const Point3D &p2){
    addPoint(p0);
    addPoint(p1);
    addPoint(p2);
}

void Mesh::addPlane(const Point3D &p0, const Point3D &p1, const Point3D &p2, const Point3D &p3){
    addTriangle(p0,p1,p2);
    addTriangle(p0,p2,p3);
}

void Mesh::loadObj(QString filename){
    QFile file(filename);
    if(!file.exists() || !file.open(QIODevice::ReadOnly))
        return;

    std::vector<std::array<float, 3>> pos;
    std::vector<std::array<float, 2>> uvs;
    std::vector<std::array<float, 3>> normals;

    QTextStream in(&file);
       while (!in.atEnd())
       {
          QString line = in.readLine();
          auto splited = line.split(" ");
          if(splited[0] == "v")
              pos.push_back({splited[1].toFloat(), splited[2].toFloat(), splited[3].toFloat()});
          if(splited[0] == "vn")
              normals.push_back({splited[1].toFloat(), splited[2].toFloat(), splited[3].toFloat()});
          if(splited[0] == "vt")
              uvs.push_back({splited[1].toFloat(), splited[2].toFloat()});
          if(splited[0] == "f"){
              splited.erase(splited.begin());

              std::vector<Point3D> points;
              for(auto& face : splited){
                  const auto splitFace = face.split("/");
                  const auto indexP = splitFace[0].toInt()-1;
                  Point3D p(pos[indexP][0], pos[indexP][1], pos[indexP][2]);

                  if(splitFace.size() >= 2 && !splitFace[1].isEmpty()){
                      const auto indexU = splitFace[1].toInt()-1;
                      p.u = uvs[indexU][0];
                      p.v = uvs[indexU][1];
                  }

                  if(splitFace.size() == 3 && !splitFace[2].isEmpty()){
                      const auto indexN = splitFace[2].toInt()-1;
                      p.nx = normals[indexN][0];
                      p.ny = normals[indexN][1];
                      p.nz = normals[indexN][2];
                  }

                  points.push_back(p);
              }
              if(splited.size() == 3)
                addTriangle(points[0], points[1], points[2]);
              else
                addPlane(points[0], points[1], points[2], points[3]);
          }

       }
       file.close();
}














