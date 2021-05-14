#pragma once
#include "Game.h"
#include <functional>

// シーンベースクラス
class SceneBase
{
public:
	SceneBase() {};
	virtual      ~SceneBase() {};

	virtual       SceneBase*  update() = 0;    // シーンの更新処理（オーバーライド必須）
	virtual void  draw() = 0;                  // シーンの描画処理（オーバーライド必須）
};