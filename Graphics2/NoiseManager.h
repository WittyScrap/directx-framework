#pragma once
#include "DirectXCore.h"
#include <vector>
#include <memory>

#define clamp(a, x, y) { a = ((a) > (x) ? ((a) < (y) ? (a) : (y)) : (x)); }

class BasicNoise;
using namespace std;

/**
 * Manages multiple noise layers.
 *
 */
class NoiseManager
{
public:
     inline  FLOAT                  GetMinimumHeight() const                        { return _minHeight; }
     inline  void                   SetMinimumHeight(const FLOAT& seaLevel)         { _minHeight = seaLevel; }
     inline  void                   SetMinimumHeight(const FLOAT&& seaLevel)        { _minHeight = seaLevel; }

     inline  FLOAT                  GetMaximumHeight() const                        { return _maxHeight; }
     inline  void                   SetMaximumHeight(FLOAT value)                   { _maxHeight = value; }

     template<typename TNoiseType>
     inline  TNoiseType*            CreateNoise();

             FLOAT                  GetNoiseValue(FLOAT radius, FLOAT x, FLOAT y, FLOAT z) const;

private:
	vector<shared_ptr<BasicNoise>>	_noises;

    FLOAT                           _minHeight{ 0 };
    FLOAT                           _maxHeight{ 5.f };
};

template<typename TNoiseType>
inline TNoiseType* NoiseManager::CreateNoise()
{
	shared_ptr<TNoiseType> noise = make_shared<TNoiseType>();
	_noises.push_back(noise);
	return noise.get();
}