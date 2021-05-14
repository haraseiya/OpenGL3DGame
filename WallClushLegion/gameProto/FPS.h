#pragma once

// 毎フレームにかかった時間を計測し上限を設けるクラス
class FPS
{
public:
	FPS();
	~FPS();

	/*
	@brief	フレーム毎の処理
	*/
	void Update();

private:
	/*
	@brief	FPSが設定値より大きくならないための制御
	*/
	void Wait();

	//フレームレートの最高値
	const Uint32 setFps;

	//１フレームにかける時間
	const Uint32 oneFrameTickCount;

	//フレームのカウンター
	Uint32 fpsCount;

	//FPSの計算を始めた時の時間
	Uint32 frameStartTickTime;

	//計算したフレームレート
	Uint32 fps;

	//前のフレームのかかった時間
	int beforetickCount;

	//現在のフレームのかかった時間
	float deltaTime;
public://ゲッターセッター
	/*
	@return 現在のフレームにかかった時間
	*/
	float GetDeltaTime() { return deltaTime; }
};

