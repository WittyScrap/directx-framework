#pragma once
#include "SimplexNoise.h"
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

     inline  FLOAT                  GetPeakHeight() const                           { return _peakHeight; }
     inline  void                   SetPeakHeight(FLOAT value)                      { _peakHeight = value; }

     inline  FLOAT                  GetPerlinStartX() const                         { return _noiseOffsetX; }
     inline  FLOAT                  GetPerlinStartY() const                         { return _noiseOffsetY; }

     inline  void                   SetNoiseOffsetX(const FLOAT& value)             { _noiseOffsetX = value; }
     inline  void                   SetNoiseOffsetY(const FLOAT& value)             { _noiseOffsetY = value; }

     inline  void                   SetNoiseOctaves(const size_t& value)            { _octaves = value; }
     inline  void                   SetNoise(SimplexNoise& noise)                   { _noise = noise; }
     inline  void                   SetNoiseScale(const FLOAT& scale)               { _noiseScale = scale; }

     inline  FLOAT                  GetSeaLevel() const                             { return _seaLevel; }
     inline  void                   SetSeaLevel(const FLOAT& seaLevel)              { _seaLevel = seaLevel; }
     inline  void                   SetSeaLevel(const FLOAT&& seaLevel)             { _seaLevel = seaLevel; }

     inline  void                   SetDrawMode(const MeshMode& value)              { _draw = value; }
     inline  void                   SetResolution(const UINT& value)                { _resolution = value; }
     inline  void                   SetResolution(const UINT&& value)               { _resolution = value; }

     inline  TerrainMode            GetMode()                                       { return _mode; }
     inline  void                   SetMode(TerrainMode value)                      { _mode = value; }

             bool                   Generate();

protected:
             FLOAT                  GetNoiseValue(FLOAT x, FLOAT y, FLOAT z) const;

             void                   GenerateVertices(Mesh* target);
             void                   GenerateIndices(Mesh* target, size_t verticesLength);

             int                    GenerateTopFace(vector<int>& indices, int t, int ring);
             int                    GenerateBottomFace(vector<int>& indices, int t, int ring, size_t verticesLength);

             void                   MakeSphere(vector<Vector3>& vertices);

     static  int                    CreateQuad(vector<int>& indices, int i, int v00, int v10, int v01, int v11);

   constexpr FLOAT                  GetNormalizedValue(const UINT& value, const UINT& range) const;

private:
    SimplexNoise                    _noise;

    TerrainMode                     _mode{ TerrainMode::Flat };
    MeshMode                        _draw{ MeshMode::TriangleList };

    FLOAT                           _radius{ 250 };

    FLOAT                           _constantValue{ 0 };
    FLOAT                           _peakHeight{ 256 };
    UINT                            _resolution{ 128 };

    FLOAT                           _noiseOffsetX{ 0 };
    FLOAT                           _noiseOffsetY{ 0 };
    FLOAT                           _noiseOffsetZ{ 0 };

    FLOAT                           _noiseScale{ 1 };
    SIZE_T                          _octaves{ 1 };
    FLOAT                           _seaLevel{ 0 };
};

