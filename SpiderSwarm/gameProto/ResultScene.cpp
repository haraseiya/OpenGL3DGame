#include "ResultScene.h"
#include "Renderer.h"
#include "Input.h"
#include "TitleScene.h"
#include "ScoreManager.h"
#include "Player1.h"
#include "BitMapText.h"
#include "CameraActor.h"
#include "LevelActor.h"

#pragma warning(disable:4996)

ResultScene::ResultScene()
	: mScore(0)
	, mCamera(nullptr)
{
	mSceneState = SceneState::SCENE_RESULT;
	//m_texture = RENDERER->GetTexture("Assets/Image/Result.png");
	mPlayer = new Player1();
	mPlayer->SetPlayerSceneState(PlayerSceneState::PLAYER_RESULT);
	mPlayer->SetPosition(Vector3(0, 0, 800));
	mPlayer->LoadAnimation();
	mPlayer->RotateToNewForward(Vector3::NegUnitX);
 
	// �s�񏉊���
	Matrix4 proj;
	proj = Matrix4::CreatePerspectiveFOV(Math::ToRadians(45.0f), GAMEINSTANCE.GetRenderer()->GetScreenWidth(), GAMEINSTANCE.GetRenderer()->GetScreenHeight(), 1.0, 10000.0f);
	GAMEINSTANCE.GetRenderer()->SetProjMatrix(proj);

	// ���C�g
	GAMEINSTANCE.GetRenderer()->SetAmbientLight(Vector3(0.2f, 0.2f, 0.2f));
	DirectionalLight& dir = GAMEINSTANCE.GetRenderer()->GetDirectionalLight();

	// �f�B���N�V�������C�g
	dir.mDirection = Vector3(0.7f, -0.7f, -0.7f);
	dir.mDiffuseColor = Vector3(0.78f, 0.88f, 1.0f);
	dir.mSpecColor = Vector3(0.8f, 0.8f, 0.8f);

	// �}�b�v�ǂݍ���
	mLevelActor = new LevelActor();
	Vector3 offset(0, 0, 0);
	mLevelActor->LoadLevel("Assets/Map/Stage.gpmesh", "", offset);
	mLevelActor->SetScale(4.5f);

	// �J�����̒ǉ�
	mCamera = new CameraActor(nullptr);
	mCamera->Init(Vector3(-300, 0, 1000), Vector3(0, 0, 1000), Vector3(0, 0, 0));

	// �e�L�X�g1�ǂݍ���
	mFont = new BitMapText;
	mFont->SetFontImage(16, 6, "assets/UI/sci-fiLarge.png");
	mFont->ReMapText(" !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_\tabcdefghijklmnopqrstuvwxyz{|}~\\");
}

ResultScene::~ResultScene()
{
	delete mCamera;
	delete mPlayer;
	delete mFont;

	mCamera = nullptr;
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

	//�Q�[���V�X�e���֘A�`��
	GAMEINSTANCE.GetRenderer()->Draw();

	RENDERER->SpriteDrawBegin();
	char buf1[256];

	
	mScore += 100;
	const int sumScore = ScoreManager::GetInstance()->GetSumScore();

	if (mScore > sumScore)
	{
		mScore = sumScore;
	}

	sprintf(buf1, "%d", mScore);
	mFont->TextDraw(150, 50, buf1);
	//RENDERER->DrawTexture(m_texture, Vector2(640, 370));
	RENDERER->SpriteDrawEnd();

	// ��ʂ̃t���b�v
	GAMEINSTANCE.GetRenderer()->WindowFlip();
}
