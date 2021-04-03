#pragma once
#pragma once
#include<iostream>
#include<vector>
#include<string>
#include<memory>

#include "scene/scene.h"

#include "models/cube.h"
#include "pipeline/pipeline.h"

#include "effects/colorEffect.h"

class colorInterpolationScene : public scene
{
public:
	colorInterpolationScene(MainWindow* wnd, Graphics* gfx)
		: scene(wnd, gfx),
		pip(colorEffect(),gfx)
	{}

	virtual void composeFrame() override
	{
		if (wnd->kbd.KeyIsPressed(0x5A)) { rotation = rotation * Mat3::RotateZ(0.0174533); }
		if (wnd->kbd.KeyIsPressed(0x59)) { rotation = rotation * Mat3::RotateY(0.0174533); }
		if (wnd->kbd.KeyIsPressed(0x58)) { rotation = rotation * Mat3::RotateX(0.0174533); }
	}
	virtual void drawFrame() override
	{
		pip.bindTranslation(Vec3(0.0f, 0.0f, 2.0f)); //push the cube model by two in the z
		pip.bindRotationMatrix(rotation);
		pip.draw(cube::getColorEffectCubeIndices(), cube::getColorEffectCubeVertices());
	}

private:
	Mat3 rotation = Mat3::Identity();
	pipeline<colorEffect> pip;
};