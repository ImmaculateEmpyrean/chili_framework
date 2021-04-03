#pragma once
#include<iostream>
#include<vector>
#include<string>
#include<filesystem>
#include<memory>

#include<utility>
#include<algorithm>

#include "dataStructure/math/Vec2.h"
#include "dataStructure/math/Vec3.h"
#include "dataStructure/math/Mat3.h"

#include "graphics/Surface.h"
#include "graphics/Colors.h"

class colorEffect
{
public:
	colorEffect() = default;

	class vertex
	{
	public:
		vertex() = default;
		vertex(Vec3 position, Vec3 color)
			:
			m_position(position),
			m_color(color)
		{}

	public:
		vertex interpolateTo(const vertex& rhs, float alpha) const
		{
			vertex v;
			v.m_position = (m_position * (1 - alpha)) + (rhs.m_position * (alpha));
			v.m_color = (m_color * (1 - alpha)) + (rhs.m_color * (alpha));

			return v;
		}


		vertex operator+(const vertex& rhs) const
		{
			vertex vert;
			vert.m_position = m_position + rhs.m_position;
			vert.m_color = m_color + rhs.m_color;

			return vert;
		}
		vertex& operator+=(const vertex& rhs)
		{
			m_position = m_position + rhs.m_position;
			m_color = m_color + rhs.m_color;
			return *this;
		}

		vertex operator-(const vertex& rhs) const
		{
			vertex vert;
			vert.m_position = m_position - rhs.m_position;
			vert.m_color = m_color - rhs.m_color;
			return vert;
		}
		vertex operator-=(const vertex& rhs)
		{
			m_position = m_position - rhs.m_position;
			m_color = m_color - rhs.m_color;
			return *this;
		}


		vertex operator/(const float rhs) const
		{
			vertex vert;
			vert.m_position = m_position / rhs;
			vert.m_color = m_color / rhs;
			return vert;
		}
		vertex operator*(const float rhs) const
		{
			vertex vert;
			vert.m_position = m_position * rhs;
			vert.m_color = m_color * rhs;
			return vert;
		}

	public:
		Vec3 m_position = Vec3(-99.0f, -99.0f, -99.0f); //this variable with the very same name is mandatory and the pipeline expects said variable
		Vec3 m_color = Vec3(-100.0f, -100.0f,-100.0f);
	};


	class pixelShader
	{
	public:
		Color operator()(colorEffect::vertex vert)
		{
			return Color(vert.m_color.x * 255.0f,vert.m_color.y * 255.0f,vert.m_color.z * 255.0f,255.0f);
		}
	};

public:
	pixelShader ps;
};