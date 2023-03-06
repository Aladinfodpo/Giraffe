#pragma once

#include <vector>
#include <QString>

struct Point3D{
    Point3D(float in_x, float in_y, float in_z, float in_u = 0.f, float in_v = 0.f, float in_nx = 0.f, float in_ny = 0.f, float in_nz = 1.f)
        : x(in_x), y(in_y), z(in_z), u(in_u), v(in_v), nx(in_nx), ny(in_ny), nz(in_nz){}
    float x, y, z;
    float u, v;
    float nx, ny, nz;
    constexpr static int nbField = 8;
    constexpr static int sizePoint = nbField*sizeof(float);
};

class Mesh
{
public:
    Mesh();

    void addDefaultPlane();
    void addTriangle(const Point3D& p0, const Point3D& p1, const Point3D& p2);
    void addPlane(const Point3D& p0, const Point3D& p1, const Point3D& p2, const Point3D& p3);

    float* getData(){return data.data();}
    size_t getNbPoint(){return data.size()/8;}
    size_t getSizeData(){return data.size()*sizeof(float);}

    void loadObj(QString filename);

    void clear(){data.clear();}

private:
    void addPoint(const Point3D& p);

    std::vector<float> data;
};
