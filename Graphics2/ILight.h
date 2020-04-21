#pragma once
#include "DirectXCore.h"

/**
 * Default light interafece.
 *
 */
class ILight
{
public:
                        ILight()                        {}
                        ILight(const XMFLOAT4& color)   {}

    virtual             ~ILight()                       {}

    /* ----- Interface begins HERE ----- */

    virtual XMFLOAT4&   GetColor()                      = 0;
    virtual void        SetColor(const XMFLOAT4& c)     = 0;
};

