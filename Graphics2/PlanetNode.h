#pragma once
#include "NoiseManager.h"
#include "BasicNoise.h"
#include "MeshNode.h"
#include "Material.h"
#include "Mesh.h"

/**
 * Toggles between in which way the terrain should be sampled.
 *
 */
enum class TerrainMode
{
    TextureSample,
    Procedural,
    Flat
};

/**
 * Represents a graph for procedurally generated or
 * sampled terrain data.
 *
 * The terrain is displayed as a grid matrix with a Y offset on
 * each vertex that depends on:
 *  A: The sampled color from a texture
 *  B: The sampled color from a perline noise
 *  C: A constant value
 * Depending on which mode has been selected.
 */
class PlanetNode : public MeshNode
{
public:
                                    PlanetNode() : MeshNode(L"Terrain")                { }
                                    PlanetNode(wstring name) : MeshNode(name)          { }

    virtual                        ~PlanetNode()                                       { }

    virtual  bool                   Initialise() override;

     inline  FLOAT                  GetRadius() const                               { return _radius; }
     inline  void                   SetRadius(const FLOAT& value)                   { _radius = value; }

     inline  FLOAT                  GetConstantValue() const                        { return _constantValue; }
     inline  void                   SetConstantValue(const FLOAT& value)            { _constantValue = value; }

     inline  void                   SetDrawMode(const MeshMode& value)              { _draw = value; }
     inline  void                   SetResolution(const UINT& value)                { _resolution = value; }
     inline  void                   SetResolution(const UINT&& value)               { _resolution = value; }

     inline  TerrainMode            GetMode()                                       { return _mode; }
     inline  void                   SetMode(TerrainMode value)                      { _mode = value; }

     inline  NoiseManager&          GetNoiseManager()                               { return _noises; }

             bool                   Generate();

protected:
             void                   GenerateVertices(Mesh* target);
             void                   GenerateIndices(Mesh* target, size_t verticesLength);

             int                    GenerateTopFace(vector<int>& indices, int t, int ring);
             int                    GenerateBottomFace(vector<int>& indices, int t, int ring, size_t verticesLength);

             void                   MakeSphere(vector<Vector3>& vertices);

     static  int                    CreateQuad(vector<int>& indices, int i, int v00, int v10, int v01, int v11);
   constexpr FLOAT                  GetNormalizedValue(const UINT& value, const UINT& range) const;

             void                   SetVertex(vector<Vector3>& vertices, int i, const float& x, const float& y, const float& z);

private:
    NoiseManager                    _noises;

    TerrainMode                     _mode{ TerrainMode::Flat };
    MeshMode                        _draw{ MeshMode::TriangleList };

    FLOAT                           _radius{ 250 };

    FLOAT                           _constantValue{ 0 };
    UINT                            _resolution{ 512 };
};

