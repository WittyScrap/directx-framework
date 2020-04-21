#pragma once
#include "ILight.h"

/**
 * Directional light.
 *
 */
class DirectionalLight : public ILight
{
public:
                                    DirectionalLight() : _col{ 1, 1, 1, 1 }, _direction{ 0, 1, 0, 1 }                       {}

                                    DirectionalLight(const XMFLOAT4& col) : _col{ col }, _direction{ 0, 1, 0, 1 }           {}
                                    DirectionalLight(const DirectionalLight& d) : _col{ d._col }, _direction{ 0, 1, 0, 1 }  {}

    virtual                         ~DirectionalLight()                                                                     {}

    XMFLOAT4&                       GetColor() override                                                                     { return _col; }
    void                            SetColor(const XMFLOAT4& c) override                                                    { _col = c; }

    XMVECTOR&                       GetDirection()                                                                          { return _direction; }
    void                            SetDirection(const XMVECTOR& d)                                                         { _direction = d; }

private:
    XMFLOAT4                        _col;
    XMVECTOR                        _direction;
};

