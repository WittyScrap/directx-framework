#pragma once
#include "NoiseManager.h"
#include "BasicNoise.h"
#include "MeshNode.h"
#include "Material.h"
#include "Mesh.h"

#include <thread> // Oh no

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

#define MESH shared_ptr<Mesh>

struct LOD
{
    UINT planetLOD, atmosphereLOD;
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
                                    PlanetNode() : MeshNode(L"Terrain")             { }
                                    PlanetNode(wstring name) : MeshNode(name)       { }
                                                                                    // Abort planet generation thread (if it is still running)
    virtual                        ~PlanetNode()                                    { b_abortBuild = true; _planetBuildingThread.join(); }

    virtual  bool                   Initialise() override;

     inline  FLOAT                  GetRadius() const                               { return _radius; }
     inline  void                   SetRadius(const FLOAT& value)                   { _radius = value; }

     inline  FLOAT                  GetAtmosphereThickness() const                  { return _atmosphereThickness; }
     inline  void                   SetAtmosphereThickness(const FLOAT& value)      { _atmosphereThickness = value; }

     inline  FLOAT                  GetConstantValue() const                        { return _constantValue; }
     inline  void                   SetConstantValue(const FLOAT& value)            { _constantValue = value; }

     inline  void                   SetMinimumDistance(const FLOAT& value)          { _minimumDistance = value; }
     inline  void                   SetMaximumDistance(const FLOAT& value)          { _maximumDistance = value; }

     inline  void                   SetDrawMode(const MeshMode& value)              { _draw = value; }
             void                   CreateLOD(const UINT& resolution);

     inline  TerrainMode            GetMode()                                       { return _mode; }
     inline  void                   SetMode(TerrainMode value)                      { _mode = value; }

     inline  NoiseManager&          GetNoiseManager()                               { return _noises; }

     inline  void                   SetHasAtmosphere(const bool& value)             { b_hasAtmosphere = value; }
     inline  bool                   GetHasAtmosphere() const                        { return b_hasAtmosphere; }

             bool                   Generate();
    virtual  void                   OnPreRender() override;

protected:
             void                   GenerateAllLODs();

             MESH                   GenerateLOD(const LOD& resolution);
             void                   MakeSphere(vector<Vector3>& vertices, const UINT& resolution, float radius, bool deform);

             void                   PopulateGroundMaterial(shared_ptr<Material>& mat);
             void                   PopulateAtmosphereMaterial(shared_ptr<Material>& mat);

private:
             void                   SetLOD(size_t lod);

private:
    NoiseManager                    _noises;

    TerrainMode                     _mode{ TerrainMode::Flat };
    MeshMode                        _draw{ MeshMode::TriangleList };

    FLOAT                           _radius{ 250 };
    FLOAT                           _atmosphereThickness{ 50.f };

    FLOAT                           _constantValue{ 0 };
    const UINT                      _atmosphereMaxResolution{ 32 };

    shared_ptr<Material>            _atmosphereMaterial;
    shared_ptr<Material>            _planetMaterial;

    vector<MESH>                    _meshLODs;
    vector<LOD>                     _requestedLODs;

    FLOAT                           _minimumDistance{ 200.f };
    FLOAT                           _maximumDistance{ 4000.f };

    int                             _currentLOD{ -1 };
    bool                            b_hasAtmosphere{ true };

    thread                          _planetBuildingThread;
    bool                            b_abortBuild{ false };
};

#undef MESH