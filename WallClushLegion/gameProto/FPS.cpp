#include "FPS.h"
#include "SDL/SDL.h"
#include "stdio.h"

FPS::FPS()
	: setFps(60)
	, oneFrameTickCount(1000 / setFps)
	, fpsCount(0)
	, frameStartTickTime(0)
	, fps(0)
	, beforetickCount(0)
	, deltaTime(0)
{

}

FPS::~FPS()
{
}

/*
@brief	フレーム毎の処理
*/
void FPS::Update()
{
	Wait();
	deltaTime = (SDL_GetTicks() - beforetickCount) / 1000.0f;
	if (deltaTime >= 0.10f)
	{
		deltaTime = 0.10f;
	}
	beforetickCount = SDL_GetTicks();
	//1フレーム目の時刻を保存
	if (fpsCount == 0)
	{
		frameStartTickTime = SDL_GetTicks();
	}
	//設定したフレーム数が経過したら
	if (fpsCount == setFps)
	{
		int nowTickTime = SDL_GetTicks();
		fps = 1000 / ((nowTickTime - frameStartTickTime) / setFps);
		fpsCount = 0;
	}
	else
	{
		fpsCount++;
	}
}

/*
@brief	FPSが設定値より大きくならないための制御
*/
void FPS::Wait()
{
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), beforetickCount + oneFrameTickCount));
}
