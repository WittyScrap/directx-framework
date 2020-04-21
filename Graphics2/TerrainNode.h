#pragma once
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
    PerlinNoise,
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
class TerrainNode : public MeshNode
{
public:
                                    TerrainNode() : MeshNode(L"Terrain")                {}
                                    TerrainNode(wstring name) : MeshNode(name)          {}

    virtual                        ~TerrainNode()                                       {}

    virtual  bool                   Initialise() override;

     inline  UINT                   GetWidth() const                                { return _width; }
     inline  UINT                   GetHeight() const                               { return _height; }

     inline  void                   SetWidth(const UINT& value)                     { _width = value; }
     inline  void                   SetHeight(const UINT& value)                    { _height = value; }

     inline  FLOAT                  GetConstantValue() const                        { return _constantValue; }
     inline  void                   SetConstantValue(const FLOAT& value)            { _constantValue = value; }

     inline  FLOAT                  GetPeakHeight() const                           { return _peakHeight; }
     inline  void                   SetPeakHeight(FLOAT value)                      { _peakHeight = value; }

     inline  FLOAT                  GetPerlinStartX() const                         { return _perlinStartX; }
     inline  FLOAT                  GetPerlinStartY() const                         { return _perlinStartY; }

     inline  void                   SetPerlinStartX(const FLOAT& value)             { _perlinStartX = value; }
     inline  void                   SetPerlinStartY(const FLOAT& value)             { _perlinStartY = value; }

             bool                   LoadHeightMap(wstring heightMapFilename);

     inline  void                   SetDrawMode(const MeshMode& value)              { _draw = value; }

     inline  TerrainMode            GetMode()                                       { return _mode; }
     inline  void                   SetMode(TerrainMode value)                       { _mode = value; }

             bool                   Generate();

protected:
             FLOAT                  GenerateFromTexture(size_t it_x, size_t it_y);
             FLOAT                  GenerateFromPerlin(size_t it_x, size_t it_y);
             FLOAT                  GenerateFlat(size_t it_x, size_t it_y);

private:
    TerrainMode                     _mode{ TerrainMode::Flat };
    MeshMode                        _draw{ MeshMode::TriangleList };

    UINT                            _width{ 1024 };
    UINT                            _height{ 1024 };

    FLOAT                           _constantValue{ 0 };
    FLOAT                           _peakHeight{ 512 };

    FLOAT                           _perlinStartX{ 0 };
    FLOAT                           _perlinStartY{ 0 };

    vector<float>                   _heightMap;
};

