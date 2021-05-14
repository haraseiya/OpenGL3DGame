﻿
#ifndef __EFFEKSEER_RECTF_H__
#define __EFFEKSEER_RECTF_H__

//----------------------------------------------------------------------------------
// Include
//----------------------------------------------------------------------------------
#include "Effekseer.Base.h"

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace Effekseer
{
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
/**
	@brief	四角形
*/
struct RectF
{
private:
public:
	float X;

	float Y;

	float Width;

	float Height;

	RectF();

	RectF(float x, float y, float width, float height);

	Vector2D Position() const;

	Vector2D Size() const;
};

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
} // namespace Effekseer
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#endif // __EFFEKSEER_RECTF_H__
