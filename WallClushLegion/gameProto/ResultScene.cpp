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
	// �w�i�F��ݒ�
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	// ��ʃN���A
	GAMEINSTANCE.GetRenderer()->WindowClear();


	RENDERER->SpriteDrawBegin();
	RENDERER->DrawTexture(m_texture, Vector2(640, 370));
	RENDERER->SpriteDrawEnd();

	// ��ʂ̃t���b�v
	GAMEINSTANCE.GetRenderer()->WindowFlip();
}
