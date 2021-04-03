#pragma once
#include <vector>

#include "effects/textureEffect.h"
#include "effects/colorEffect.h"

class cube
{
public:
	static std::vector<textureEffect::vertex> getSkinnedCubeVertices(float side = 0.5f);
	static std::vector<std::uint32_t> getSkinnedCubeIndices();

	static std::vector<colorEffect::vertex> getColorEffectCubeVertices(float side = 0.5f);
	static std::vector<std::uint32_t> getColorEffectCubeIndices();
};
