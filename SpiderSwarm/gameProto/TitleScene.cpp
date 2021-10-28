#include "TitleScene.h"
#include "Renderer.h"
#include "Texture.h"
#include "DebugGrid.h"
#include "CameraActor.h"
#include "PlayerBase.h"
#include "Player1.h"
#include "SelectScene.h"
#include "Input.h"
#include "GameScene.h"
#include "WeakEnemy.h"
#include "Weapon.h"
#include "ThirdPersonCamera.h"

TitleScene::TitleScene()
	: mPlayer(nullptr)
	, mWeapon(nullptr)
{
	printf("//////////////////\n");
	printf("//�^�C�g���V�[��//\n");
	printf("//////////////////\n");

	// �v���C���[����
	mPlayer = new Player1();
	mPlayer->SetPosition(Vector3(500.0, 0.0f, 0.0f));
	mPlayer->RotateToNewForward(Vector3::NegUnitX);
	mPlayer->SetPlayerSceneState(PlayerSceneState::PLAYER_TITLESCENE);

	// �G���G����
	mEnemy = new WeakEnemy(mPlayer);
	mEnemy->SetPosition(Vector3(-500.0f, 0.0f, 0.0f));
	mEnemy->RotateToNewForward(Vector3::UnitX);

	// ���퐶��
	mWeapon = new Weapon(mPlayer);
	mWeapon->SetPosition(Vector3(0.0f, 0.0f, 100.0f));
	mWeapon->SetScale(1.0f);

	// ���C�e�B���O�ݒ�
	GAMEINSTANCE.GetRenderer()->SetAmbientLight(Vector3(0.2f, 0.2f, 0.2f));
	DirectionalLight& dir = GAMEINSTANCE.GetRenderer()->GetDirectionalLight();
	dir.mDirection = Vector3(0.7f, -0.7f, -0.7f);
	dir.mDiffuseColor = Vector3(0.78f, 0.88f, 1.0f);
	dir.mSpecColor = Vector3(0.8f, 0.8f, 0.8f);

	mPlayerCameraOffset = Vector3(100.0f, 0.0f, 200.0f);
	mEnemyCameraOffset = Vector3(100.0f, 0.0f, 100.0f);

	// �J�����̒ǉ�
	mCamera = new CameraActor(mPlayer);
	mCamera->Init(Vector3(0, -100, 200), mPlayer->GetPosition()+mPlayerCameraOffset, Vector3(0,0,0));

	// �e�N�X�`���ǉ�
	mTexture = RENDERER->GetTexture("Assets/Image/Title.png");
}

TitleScene::~TitleScene()
{
	delete mPlayer;
	delete mEnemy;

	printf("�^�C�g���V�[���I��");
}

SceneBase* TitleScene::update()
{
	// A�L�[�������ꂽ��Q�[����ʂ֑J��
	if (INPUT_INSTANCE.IsKeyPushdown(KEY_A))
	{
		return new GameScene();
	}

	mTimer += GAMEINSTANCE.GetDeltaTime();

	// �^�C�g����ʃJ�������[�N
	Vector3 cameraPos;
	if (mTimer <= 5.0f)
	{
		cameraPos = Vector3::Lerp(mCamera->GetViewPos(), mPlayer->GetPosition() + mEnemyCameraOffset, 0.01f);
		mCamera->SetPosition(cameraPos);
	}
	else if (mTimer > 5.0f)
	{
		mCamera->SetTarget(mEnemy->GetPosition());
		cameraPos = Vector3::Lerp(mCamera->GetViewPos(), mEnemy->GetPosition() + mPlayerCameraOffset, 0.01f);
		mCamera->SetPosition(cameraPos);
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

	// �X�v���C�g�`��
	RENDERER->SpriteDrawBegin();
	RENDERER->DrawTexture(mTexture, Vector2(640, 370));
	RENDERER->SpriteDrawEnd();

	// �����_�[�`��
	GAMEINSTANCE.GetRenderer()->Draw();

	// ��ʂ̃t���b�v
	GAMEINSTANCE.GetRenderer()->WindowFlip();
}
