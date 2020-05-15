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
#define G 0.0000000000674f

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
                                    PlanetNode() : MeshNode(L"Terrain")              { _allPlanets.push_back(this); }
                                    PlanetNode(wstring name) : MeshNode(name)        { _allPlanets.push_back(this); }
                                                                                     // Abort planet generation thread (if it is still running), remove this planet from the list of existing planets
    virtual                        ~PlanetNode()                                     { b_abortBuild = true; _planetBuildingThread.join(); RemovePlanet(this); }

    virtual  bool                   Initialise()                                     override;
    virtual  void                   Update()                                         override;

     inline  FLOAT                  GetRadius() const                                { return _radius; }
     inline  void                   SetRadius(const FLOAT& value)                    { _radius = value; }

             FLOAT                  GetMass() const;

     inline  FLOAT                  GetAtmosphereThickness() const                   { return _atmosphereThickness; }
     inline  void                   SetAtmosphereThickness(const FLOAT& value)       { _atmosphereThickness = value; }

     inline  FLOAT                  GetConstantValue() const                         { return _constantValue; }
     inline  void                   SetConstantValue(const FLOAT& value)             { _constantValue = value; }

     inline  void                   SetMinimumDistance(const FLOAT& value)           { _minimumDistance = value; }
     inline  void                   SetMaximumDistance(const FLOAT& value)           { _maximumDistance = value; }
     inline  void                   SetScatterDistance(const FLOAT& value)           { _scatterMinimumDistance = value; }

     inline  void                   SetDrawMode(const MeshMode& value)               { _draw = value; }
             void                   CreateLOD(const UINT& resolution);

     inline  TerrainMode            GetMode()                                        { return _mode; }
     inline  void                   SetMode(TerrainMode value)                       { _mode = value; }

     inline  NoiseManager&          GetNoiseManager()                                { return _noises; }

     inline  void                   SetHasAtmosphere(const bool& value)              { b_hasAtmosphere = value; }
     inline  bool                   GetHasAtmosphere() const                         { return b_hasAtmosphere; }

     inline  const FLOAT&           GetGravity() const                               { return _gravity; }
     inline  void                   SetGravity(const FLOAT& gravity)                 { _gravity = gravity; }

     inline  void                   SetGrassColor(FLOAT r, FLOAT g, FLOAT b)         { _grassColor = XMFLOAT4(r, g, b, 1.f); }
     inline  void                   SetSandColor(FLOAT r, FLOAT g, FLOAT b)          { _sandColor = XMFLOAT4(r, g, b, 1.f); }

             bool                   Generate();
    virtual  void                   OnPreRender() override;
    virtual  void                   OnPostRender() override;

             const FLOAT            GetHeightAtPoint(const FLOAT& radius, const Vector3& unitPos) const;
             const FLOAT            GetHeightAtPoint(const Vector3& unitPos) const;

     inline  FLOAT                  GetHeightFromSurface(const Vector3& point) const { return (point - GetWorldPosition()).Length() - _radius; }
     inline  FLOAT                  GetOrbitalVelocity(const FLOAT& height) const    { return sqrt(G * GetMass() / height); }

     inline  void                   SetSOI(const FLOAT& height)                      { _SOIHeight = height; }
     inline  const FLOAT&           GetSOI() const                                   { return _SOIHeight; }
     inline  BOOL                   PointInSOI(const Vector3& p)                     { return (p - GetWorldPosition()).SqrLength() < ((_radius + _SOIHeight) * (_radius + _SOIHeight)); }

             void                   Orbit(const PlanetNode* const planet);
     inline  const Vector3&         GetLinearVelocity() const                        { return _linearVelocity; }

     inline  void                   SetScatterMesh(const shared_ptr<Mesh>& mesh)     { _scatterMesh = mesh; }
     inline  void                   ClearScatterMesh()                               { _scatterMesh = nullptr; }

     static  shared_ptr<PlanetNode> GenerateRandom(const FLOAT noiseScale = 1.f);
     static  Vector3                CalculateTotalGravity(Vector3 sourcePoint, FLOAT sourceMass, initializer_list<PlanetNode*> exclude = {});

     static  vector<PlanetNode*>&   GetAllPlanets()                                  { return _allPlanets; }

protected:
             void                   GenerateAllLODs();

             MESH                   GenerateLOD(const LOD& resolution);
             void                   MakeSphere(vector<Vector3>& vertices, const UINT& resolution, float radius, bool deform);

             void                   PopulateGroundMaterial(shared_ptr<Material>& mat);
             void                   PopulateAtmosphereMaterial(shared_ptr<Material>& mat);

             void                   RealizeScatter(Vector3& cubePosition) const;
             void                   RenderScatter(XMMATRIX& location) const;

private:
             void                   SetLOD(size_t lod);
     static  void                   RemovePlanet(PlanetNode* target);

private:
    NoiseManager                    _noises;

    TerrainMode                     _mode{ TerrainMode::Flat };
    MeshMode                        _draw{ MeshMode::TriangleList };

    FLOAT                           _radius{ 250 };
    FLOAT                           _atmosphereThickness{ 35.f };
    FLOAT                           _SOIHeight{ 50.f };

    XMFLOAT4                        _grassColor{ 1, 1, 1, 1 };
    XMFLOAT4                        _sandColor{ 1, 1, 1, 1 };

    FLOAT                           _constantValue{ 0 };
    const UINT                      _atmosphereMaxResolution{ 32 };

    shared_ptr<Material>            _atmosphereMaterial;
    shared_ptr<Material>            _planetMaterial;

    vector<MESH>                    _meshLODs;
    vector<LOD>                     _requestedLODs;

    FLOAT                           _minimumDistance{ 200.f };
    FLOAT                           _maximumDistance{ 10000.f };

    shared_ptr<Mesh>                _scatterMesh{ nullptr };
    FLOAT                           _scatterMinimumDistance{ 1500.f };

    int                             _currentLOD{ -1 };
    bool                            b_hasAtmosphere{ true };

    thread                          _planetBuildingThread;
    bool                            b_abortBuild{ false };

    Vector3                         _linearVelocity{ 0, 0, 0 };

    FLOAT                           _gravity{ 9.85f };
    static vector<PlanetNode*>      _allPlanets;
};

#undef MESH