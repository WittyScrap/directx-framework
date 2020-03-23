#pragma once
#include "SceneGraph.h"

/**
 * A node that represents the Point of View of the scene
 * and holds all relevant view and projection matrices.
 *
 */
class CameraNode : public SceneGraph
{
public:
                                        CameraNode() : SceneGraph(L"Camera"), _viewTransformation(), _projectionTransformation()            {}
                                        CameraNode(wstring name) : SceneGraph(name), _viewTransformation(), _projectionTransformation()     {}

    virtual                             ~CameraNode()                                                                                       {}

    virtual bool                        Initialise()                        override;
    virtual void                        Update(FXMMATRIX& m)                override;

            XMMATRIX					GetViewTransformation() const;
            XMMATRIX					GetProjectionTransformation() const;

            void                        UpdateMatrices();

     static CameraNode* const           GetMain();
            void                        SetMain();

     inline FLOAT                       GetNearClipPlane() const                        { return _nearClipPlane; }
     inline FLOAT                       GetFarClipPlane() const                         { return _farClipPlane; }
     inline FLOAT                       GetFieldOfView() const                          { return _fieldOfView; }

            void                        SetNearClipPlane(FLOAT ncp);
            void                        SetFarClipPlane(FLOAT fcp);
            void                        SetFieldOfView(FLOAT fov);

private:
    XMFLOAT4X4							_viewTransformation;
    XMFLOAT4X4							_projectionTransformation;

    static CameraNode*                  _mainCamera;

    FLOAT                               _fieldOfView{ 90.f };

    FLOAT                               _nearClipPlane{ .3f };
    FLOAT                               _farClipPlane{ 10000.f };
};

#define MAIN_CAMERA     CameraNode::GetMain()
#define NO_MAIN_CAMERA  CameraNode::GetMain() == nullptr
#define HAS_MAIN_CAMERA CameraNode::GetMain() != nullptr