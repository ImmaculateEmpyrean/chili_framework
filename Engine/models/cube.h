#pragma once
#include <vector>
#include "effects/textureEffect.h"

class cube
{
public:
	static std::vector<textureEffect::vertex> getSkinnedCubeVertices(float side = 0.5f);
	static std::vector<std::uint32_t> getSkinnedCubeIndices();
};
