#pragma once
#include<utility>
#include<algorithm>
#include<memory>

#include<string>
#include<filesystem>

#include "dataStructure/math/Vec2.h"
#include "dataStructure/math/Vec3.h"
#include "dataStructure/vertex/texturedVertex.h"

#include "graphics/Graphics.h"
#include "pubeToScreenTransformer.h"

#include "graphics/Surface.h"

class Game;

template<typename effect>
class pipeline
{
public:
	using vertex =typename effect::vertex;

	pipeline(effect obj,Graphics* gfx)
		: m_effectFunctor(std::move(obj)), gfx(gfx)
	{}
	
	void bindRotationMatrix(Mat3 rotationMat) { m_rotationMatrix = rotationMat; }
	void bindTranslation(Vec3 translateby) { m_translation = translateby; }

	void draw(std::vector<uint32_t> indexBuffer, std::vector<vertex> vertexBuffer)
	{
		vertexTransformer(vertexBuffer);
		triangleRasterizer(indexBuffer, vertexBuffer);
	}

private:
	void vertexTransformer(std::vector<vertex>& vertexBuffer)
	{
		for (auto& vertex : vertexBuffer)
		{
			vertex.m_position = vertex.m_position * m_rotationMatrix;
			vertex.m_position += m_translation;
		}
	}

	void triangleRasterizer(std::vector<uint32_t> indexBuffer, std::vector<vertex> vertexBuffer)
	{
		for (int i = 0 ; i < indexBuffer.size(); i= i +3)
		{
			Vec3 v0 = vertexBuffer[indexBuffer[i]].m_position;
			Vec3 v1 = vertexBuffer[indexBuffer[i + 1]].m_position;
			Vec3 v2 = vertexBuffer[indexBuffer[i + 2]].m_position;

			Vec3 normal = ((v1 - v0).cross(v2 - v0));
			if (!(normal.dot(v0) > 0.0f))
			{
				//donot cull this particular triangle..
				vertex v0 = vertexBuffer[indexBuffer[i]];
				vertex v1 = vertexBuffer[indexBuffer[i + 1]];
				vertex v2 = vertexBuffer[indexBuffer[i + 2]];
				
				screenSpaceTransformer(v0, v1, v2);
				triangleRasterizer(v0, v1, v2);
			}
		}
	}

	void screenSpaceTransformer(vertex& v0, vertex& v1, vertex& v2)
	{
		v0.m_position = pubeToScreenTransformer::getCoordinatesInScreenSpace(v0.m_position, gfx->ScreenWidth, gfx->ScreenHeight);
		v1.m_position = pubeToScreenTransformer::getCoordinatesInScreenSpace(v1.m_position, gfx->ScreenWidth, gfx->ScreenHeight);
		v2.m_position = pubeToScreenTransformer::getCoordinatesInScreenSpace(v2.m_position, gfx->ScreenWidth, gfx->ScreenHeight);
	}
	void screenSpaceTransformer(vertex& v0)
	{
		v0.m_position = pubeToScreenTransformer::getCoordinatesInScreenSpace(v0.m_position, gfx->ScreenWidth, gfx->ScreenHeight);
	}

	void triangleRasterizer(vertex v0, vertex v1, vertex v2)
	{
		const vertex* p0 = &v0;
		const vertex* p1 = &v1;
		const vertex* p2 = &v2;

		// sorting vertices by y
		if (p0->m_position.y > p1->m_position.y) std::swap(p0, p1);
		if (p1->m_position.y > p2->m_position.y) std::swap(p1, p2);
		if (p0->m_position.y > p1->m_position.y) std::swap(p0, p1);

		///case for p1 and p2 y value equal not handled.. basically.. no natural flat bottom is being handled..
		if (p1->m_position.y == p2->m_position.y) //natural flat bottom
		{
			if (p1->m_position.x > p2->m_position.x)
				std::swap(p1, p2);
			drawFlatBottomTriangle(*p0, *p1, *p2);
		}
		else if (p0->m_position.y == p1->m_position.y) //natural flat top
		{
			if (p0->m_position.x > p1->m_position.x)
				std::swap(p0, p1);
			drawFlatTopTriangle(*p0, *p1, *p2);
		}
		else
		{
			//normal triangle which needs to be split into natural flat bottom and natural flat top
			float alphaSplit = (p1->m_position.y - p0->m_position.y) / (p2->m_position.y - p0->m_position.y);
			vertex splitPoint = p0->interpolateTo(*p2, alphaSplit);

			if (splitPoint.m_position.x < p1->m_position.x)
			{
				//majorLeft triangle
				drawFlatBottomTriangle	(*p0, splitPoint, *p1);
				drawFlatTopTriangle		(splitPoint, *p1, *p2);
			}
			else
			{
				//majorRight triangle
				drawFlatBottomTriangle(*p0, *p1, splitPoint);
				drawFlatTopTriangle(*p1, splitPoint, *p2);
			}
		}
	}


private:
	void drawFlatBottomTriangle(const vertex& vertex0, const vertex& vertex1, const vertex& vertex2)
	{
		const vertex* v0 = &vertex0;
		const vertex* v1 = &vertex1;
		const vertex* v2 = &vertex2;

		vertex leftEdge		= *v0;
		vertex rightEdge	= *v0;
		vertex leftEdgeEnd	= *v1;
		vertex rightEdgeEnd	= *v2;

		vertex leftEdgeDelta = (leftEdgeEnd - leftEdge) / (leftEdgeEnd.m_position - leftEdge.m_position).y;
		vertex rightEdgeDelta = (rightEdgeEnd - rightEdge) / (rightEdgeEnd.m_position - rightEdge.m_position).y;

		drawFlatTriangle(*v0,*v1,*v2,  leftEdgeDelta, rightEdgeDelta, *v0);
	}
	void drawFlatTopTriangle(const vertex& vertex0, const vertex& vertex1, const vertex& vertex2)
	{
		const vertex* v0 = &vertex0;
		const vertex* v1 = &vertex1;
		const vertex* v2 = &vertex2;

		vertex leftEdge = *v0;
		vertex rightEdge = *v1;
		vertex leftEdgeEnd = *v2;
		vertex rightEdgeEnd = *v2;

		vertex leftEdgeDelta = (leftEdgeEnd - leftEdge) / (leftEdgeEnd.m_position - leftEdge.m_position).y;
		vertex rightEdgeDelta = (rightEdgeEnd - rightEdge) / (rightEdgeEnd.m_position - rightEdge.m_position).y;

		drawFlatTriangle(*v0, *v1, *v2, leftEdgeDelta, rightEdgeDelta, *v1);
	}
	void drawFlatTriangle(const vertex& v0, const vertex& v1, const vertex& v2, const vertex& dv0 , const vertex& dv1, vertex itEdge1)
	{
		auto itEdge0 = v0;

		const int yStart = (int)std::ceil(v0.m_position.y - 0.5f);
		const int yEnd   = (int)std::ceil(v2.m_position.y - 0.5f);

		itEdge0 += dv0 * (float(yStart) + 0.5f - v0.m_position.y); //pre step something like less than a pixel ?
		itEdge1 += dv1 * (float(yStart) + 0.5f - v0.m_position.y); //pre step something like less than a pixel ?

		for (int y = yStart; y < yEnd; y++, itEdge0 += dv0, itEdge1 += dv1)
		{
			const int xStart = std::ceil(itEdge0.m_position.x - 0.5f);
			const int xEnd	 = std::ceil(itEdge1.m_position.x - 0.5f);

			auto iLine = itEdge0;

			const float dx = itEdge1.m_position.x - itEdge0.m_position.x;
			const auto diLine = (itEdge1 - itEdge0) / dx;

			//pre stepping
			iLine += diLine * (float(xStart) + 0.5 - itEdge0.m_position.x);

			for (int x = xStart; x < xEnd; x++, iLine += diLine)
			{
				gfx->PutPixel(x, y, m_effectFunctor.ps(iLine));
			}

		}
	}

private:
	Mat3 m_rotationMatrix = Mat3::Identity(); //the rotation is on model centre
	Vec3 m_translation = Vec3(0.0f,0.0f,0.0f); // the translation is with respect to the model centre
	Graphics* gfx = nullptr;

	effect m_effectFunctor;

	friend class Game;
};