#pragma once 
 
#define _USE_MATH_DEFINES 
#include <cmath> 
#include <vector> 
#include <iostream> 
#include "Vector.hpp" 
#include "Object3D.hpp" 
#include "BoundBox.hpp" 
using namespace std; 
 
namespace potato { 
 
    struct Vert { 
        Vec3f pos {}; 
        Vec4f color {}; // [0,1] 
        Vec3f normal {};
        Vec4f viewPos {}; 
 
        Vert operator+(const Vert &other) const { 
            return { 
                pos + other.pos, 
                color + other.color,
                normal + other.normal,
                viewPos + other.viewPos
            }; 
        }; 
 
        Vert operator-(const Vert &other) const { 
            return { 
                pos - other.pos, 
                color - other.color,
                normal - other.normal,
                viewPos - other.viewPos
            }; 
        }; 
 
        Vert operator*(const Vert &other) const { 
            return { 
                pos * other.pos, 
                color * other.color,
                normal * other.normal,
                viewPos * other.viewPos
            }; 
        }; 
 
        Vert operator*(const float &w) const { 
            return { 
                pos * w, 
                color * w,
                normal * w,
                viewPos * w
            }; 
        }; 
    }; 
 
    struct Face { 
        vector<unsigned int> indices {}; 
    }; 
 
    class PolyMesh : public Object3D { 
    protected: 
        vector<Vert> vertices {}; 
        vector<Face> faces {}; 
    public: 
        PolyMesh() : Object3D() {}; 
        virtual ~PolyMesh() {}; 
 
        vector<Vert>& getVertices() { return vertices; }; 
        vector<Face>& getFaces() { return faces; }; 

        void recomputeNormals() {
            // Zero out normals first
            for(int i = 0; i < vertices.size(); i++) {
                vertices[i].normal = Vec3f(0,0,0);
            }
            // For each face...
            for(Face &f : faces) {
                Vert A = vertices[f.indices[0]];
                Vert B = vertices[f.indices[1]];
                Vert C = vertices[f.indices[2]];
                Vec3f v1 = B.pos - A.pos;
                Vec3f v2 = C.pos - A.pos;
                Vec3f n = v1.cross(v2).normalize();
                vertices[f.indices[0]].normal = vertices[f.indices[0]].normal + n;
                vertices[f.indices[1]].normal = vertices[f.indices[1]].normal + n;
                vertices[f.indices[2]].normal = vertices[f.indices[2]].normal + n;
            }

            for(int i = 0; i < vertices.size(); i++) {
                vertices[i].normal = vertices[i].normal.normalize();
            }
        };
    }; 
 
    // Compute bounds for single face 
    void computeBounds(vector<Vert> &vertices, Face &face, BoundBoxf &box, bool startBox=true); 
 
    // Compute bounds for ENTIRE mesh 
    void computeBounds(PolyMesh *mesh, BoundBoxf &box, bool startBox=true); 
}; 
