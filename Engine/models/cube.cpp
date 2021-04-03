#include "cube.h"

std::vector<textureEffect::vertex> cube::getSkinnedCubeVertices(float side)
{
	static std::vector<textureEffect::vertex> m_vertexBuffer{
		{Vec3(side, side, side),Vec2(0.0f, 0.5f)},		//0
		{Vec3(-side, side, side),Vec2(0.25f, 0.5f)},	//1
		{Vec3(-side, side, -side),Vec2(0.25f, 0.75f)},  //2
		{Vec3(side, side, -side),Vec2(0.0f, 0.75f)},	//3

		{Vec3(-side, -side, side),Vec2(0.5f, 0.5f)},	//4
		{Vec3(-side, -side, -side),Vec2(0.5f, 0.75f)},	//5
		{Vec3(-side, side, side),Vec2(0.5f, 0.25f)},	//6
		{Vec3(side, side, side),Vec2(0.75f, 0.25f)},	//7

		{Vec3(side, -side, side),Vec2(0.75f, 0.5f)},	//8
		{Vec3(side, -side, -side),Vec2(0.75f, 0.75f)},	//9
		{Vec3(side, side, -side),Vec2(0.75f, 1.0f)},	//10
		{Vec3(-side, side, -side),Vec2(0.5f, 1.0f)},	//11

		{Vec3(side, side, -side),Vec2(1.0f, 0.75f)},	//12
		{Vec3(side, side, side), Vec2(1.0f, 0.5f)},	//13
	};
	
	return m_vertexBuffer;
}
std::vector<std::uint32_t> cube::getSkinnedCubeIndices()
{
	static auto m_indexBuffer = std::vector<std::uint32_t>{
					1,0,3	, 3,2,1,
					4,1,2	, 2,5,4,
					8,4,5	, 5,9,8,
					11,10,9 , 9,5,11,
					12,13,8 , 8,9,12,
					8,7,6	, 6,4,8
	};

	return m_indexBuffer;
}


std::vector<colorEffect::vertex> cube::getColorEffectCubeVertices(float side)
{
	static std::vector<colorEffect::vertex> m_vertexBuffer{
		{Vec3(side, side, side),Vec3(0.0f, 0.0f,1.0f)},		//0
		{Vec3(-side, side, side),Vec3(1.0f, 0.0f,0.0f)},	//1
		{Vec3(-side, side, -side),Vec3(0.0f, 1.0f,0.0f)},  //2
		{Vec3(side, side, -side),Vec3(0.0f, 0.5f , 0.0f)},	//3

		{Vec3(-side, -side, side),Vec3(0.0f, 0.0f , 1.0f)},	//4
		{Vec3(-side, -side, -side),Vec3(0.0f, 1.0f , 0.0f)},	//5
		{Vec3(-side, side, side),Vec3(1.0f, 0.0f , 0.0f)},	//6
		{Vec3(side, side, side),Vec3(1.0f, 0.0f,1.0f)},	//7

		{Vec3(side, -side, side),Vec3(0.0f, 0.0f , 1.0f)},	//8
		{Vec3(side, -side, -side),Vec3(1.0f, 0.0f , 0.0f)},	//9
		{Vec3(side, side, -side),Vec3(0.0f, 1.0f , 0.0f)},	//10
		{Vec3(-side, side, -side),Vec3(0.5f, 1.0f,0.0f)},	//11

		{Vec3(side, side, -side),Vec3(1.0f, 0.0f,1.0f)},	//12
		{Vec3(side, side, side), Vec3(1.0f, 0.5f,1.0f)},	//13
	};

	return m_vertexBuffer;
}
std::vector<std::uint32_t> cube::getColorEffectCubeIndices()
{
	return getSkinnedCubeIndices();
}
