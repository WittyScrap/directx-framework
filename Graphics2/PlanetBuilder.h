#pragma once
#include "Mesh.h"


/**
 * Generator for planetoid meshes and atmospheres.
 * This generator does not yield materials.
 *
 */
class PlanetBuilder
{
public:
                                PlanetBuilder(const UINT& resolution) : _resolution { resolution } {}
    virtual                    ~PlanetBuilder()                                                    {}

    virtual void                Generate();
            void                Map(Mesh* target);
     inline void                SetStopFlag(bool* const flagAddr)                                  { p_b_StopFlag = flagAddr; }

     inline vector<Vector3>&    GetVertices()                                                      { return _vertices; }

protected:
            void                GenerateVertices();
            void                GenerateIndices();

            int                 GenerateTopFace(int t, int ring);
            int                 GenerateBottomFace(int t, int ring);

            int                 CreateQuad(int i, int v00, int v10, int v01, int v11);
            void                SetVertex(int i, const float& x, const float& y, const float& z);

     inline bool                CanProceed() const                                                 { return !p_b_StopFlag || !(*p_b_StopFlag); }

private:
    const UINT             _resolution;

    vector<int>            _indices;
    vector<XMFLOAT2>       _texCoord;
    vector<Vector3>        _vertices;

    bool*                  p_b_StopFlag{ nullptr };
};

