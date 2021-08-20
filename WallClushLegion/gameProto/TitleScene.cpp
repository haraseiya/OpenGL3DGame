#include "TitleScene.h"
#include "Renderer.h"
#include "Texture.h"
#include "DebugGrid.h"
#include "CameraActor.h"
#include "Player.h"
#include "Attacker.h"
#include "Clasher.h"
#include "Healer.h"
#include "Input.h"
#include "GameScene.h"

TitleScene::TitleScene()
	: m_player(nullptr)
{
	printf("//////////////////\n");
	printf("//�^�C�g���V�[��//\n");
	printf("//////////////////\n");

	// �v���C���[�ǉ�
	m_player = new Player();
	m_player->SetPosition(Vector3(0, 0, 0));

	// ���C�g
	GAMEINSTANCE.GetRenderer()->SetAmbientLight(Vector3(0.2f, 0.2f, 0.2f));
	DirectionalLight& dir = GAMEINSTANCE.GetRenderer()->GetDirectionalLight();
	dir.mDirection = Vector3(0.7f, -0.7f, -0.7f);
	dir.mDiffuseColor = Vector3(0.78f, 0.88f, 1.0f);
	dir.mSpecColor = Vector3(0.8f, 0.8f, 0.8f);

	Vector3 color(0, 1, 0);
	mGrid = new DebugGrid(1000.0f, 30, color);

	// �J�����ǉ�
	CameraActor* c = new CameraActor(m_player);
	c->Init(Vector3(150, 0, 150), Vector3(0, 0, 100), Vector3(0, 0, 0));

	m_texture = RENDERER->GetTexture("Assets/Image/Title.png");
}

TitleScene::~TitleScene()
{
	delete m_player;
	printf("�^�C�g���V�[���I��");
	//manager->Destroy();
}

SceneBase* TitleScene::update()
{
	if (INPUT_INSTANCE.IsKeyPullup(KEY_A))
	{
		return new GameScene();
	}

	float radius = 160.0f;
	static float anim = 0.0f;
	anim += 0.01f;
	Matrix4 view;
	view = GAMEINSTANCE.GetViewMatrix();
	GAMEINSTANCE.GetRenderer()->SetViewMatrix(view);

	return this;
}

void TitleScene::draw()
{
	// �w�i�F��ݒ�
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	// ��ʃN���A
	GAMEINSTANCE.GetRenderer()->WindowClear();


	GAMEINSTANCE.GetRenderer()->Draw();

	RENDERER->SpriteDrawBegin();
	RENDERER->DrawTexture(m_texture, Vector2(640, 370));
	RENDERER->SpriteDrawEnd();

	// ��ʂ̃t���b�v
	GAMEINSTANCE.GetRenderer()->WindowFlip();
}
