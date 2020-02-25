#pragma once
#include "Core.h"
#include <vector>

/**
 * Represents a simple container for vertices,
 * triangles, normals, and other information related
 * to a mesh.
 */
class Mesh
{
public:
    /* Construcotrs */
    Mesh();
    Mesh(const Mesh& copy);
    Mesh(const vector<Vector3> vertices, const vector<Vector3> normals, const vector<int> triangles);

    virtual ~Mesh();

private:
    vector<Vector3> _vertices;
    vector<Vector3> _normals;
    vector<int> _triangles;
};

