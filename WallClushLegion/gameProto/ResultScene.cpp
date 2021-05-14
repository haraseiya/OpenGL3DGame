#include "ResultScene.h"
#include "Renderer.h"
#include "Input.h"
#include "TitleScene.h"

ResultScene::ResultScene()
{
	m_texture = RENDERER->GetTexture("Assets/Image/Result.png");
}

ResultScene::~ResultScene()
{

}

SceneBase* ResultScene::update()
{
	if (INPUT_INSTANCE.IsKeyPullup(KEY_A))
	{
		return new TitleScene();
	}

	return this;
}

void ResultScene::draw()
{
	// 背景色を設定
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	// 画面クリア
	GAMEINSTANCE.GetRenderer()->WindowClear();


	RENDERER->SpriteDrawBegin();
	RENDERER->DrawTexture(m_texture, Vector2(640, 370));
	RENDERER->SpriteDrawEnd();

	// 画面のフリップ
	GAMEINSTANCE.GetRenderer()->WindowFlip();
}
