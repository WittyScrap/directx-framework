#pragma once
#include "SceneNode.h"
#include "DirectXCore.h"
#include "Material.h"
#include "Mesh.h"
#include <vector>
#include <map>

#define RENDER_QUEUE_GEOMETRY       2000
#define RENDER_QUEUE_TRANSPARENT    3000

// Typedef
class MeshNode;

/** Defines a render queue as a map of indices to collections of items */
typedef map<size_t, vector<MeshNode*>> RenderQueue;

/**
 * Represents a node that can retain any type
 * of geometry.
 *
 */
class MeshNode : public SceneNode
{
public:
                                                /* --- Constructors --- */
                                                MeshNode()                     : SceneNode(L"Mesh Node")                    {}
                                                MeshNode(const wstring& name)  : SceneNode(name)                            {}

    virtual                                     ~MeshNode()                                                                 {}

    virtual bool                                Initialise()                                                                override { return true; }
    virtual void                                Update(FXMMATRIX& currentWorldTransformation)                               override;
    virtual void                                OnPreRender()                                                               {}
    virtual void                                Render()                                                                    override;
    virtual void                                Shutdown()                                                                  override;

            void                                RenderImmediately();

            void                                SetMaterial(const shared_ptr<Material>& material);
            void                                SetMesh(const shared_ptr<Mesh>& mesh);

            const shared_ptr<Mesh>              GetMesh() const;
            const shared_ptr<Material>          GetMaterial() const;

            void                                SetMaterial(const size_t location, const shared_ptr<Material>& material);
            const shared_ptr<Material>          GetMaterial(const size_t location) const;

            void                                Build(shared_ptr<Mesh> mesh);

            void                                SetRenderQueue(const size_t& queue)                                         { _queueIndex = queue; }
     static void                                RenderQueuedNodes();

protected:

    static  ComPtr<ID3D11Device>                GetDevice();
    static  ComPtr<ID3D11DeviceContext>         GetDeviceContext();

            void                                InternalRender(shared_ptr<Mesh> mesh, shared_ptr<Material> material);

private:
            shared_ptr<Mesh>                    _mesh = nullptr;
            shared_ptr<Material>                _material = nullptr;
            vector<shared_ptr<Material>>        _materials;

            size_t                              _queueIndex{ RENDER_QUEUE_GEOMETRY };
    static  RenderQueue                         _renderQueue;
};

