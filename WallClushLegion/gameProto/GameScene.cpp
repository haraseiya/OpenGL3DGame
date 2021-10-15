#include "Game.h"
#include "Math.h"
#include "Renderer.h"
#include "GameScene.h"
#include "PlayerBase.h"
#include "Player1.h"
#include "WeakEnemy.h"
#include "BossEnemy.h"
#include "NPCManager.h"
#include "Attacker.h"
#include "Clasher.h"
#include "Healer.h"
#include "DebugGrid.h"
#include "ThirdPersonCamera.h"
#include "LevelManager.h"
#include "BitMapText.h"
#include "DebugLog.h"
#include "Input.h"
#include "ResultScene.h"
#include "Font.h"
#include "LevelActor.h"
#include "EnemyManager.h"
#include "SelectScene.h"
#include "FPSCounter.h"
#include "BulletManager.h"

#pragma warning(disable:4996)

const float GameScene::mMaxFps = 60;

GameScene::GameScene(PlayerBase* player)
	: mPlayer(player)
	, mEnemyManager(nullptr)
	, mFPSCounter(nullptr)
{ 
	printf("////////////////\n");
	printf("//�Q�[���V�[��//\n");
	printf("////////////////\n");

	// �s�񏉊���
	Matrix4 proj;
	proj = Matrix4::CreatePerspectiveFOV(Math::ToRadians(45.0f), GAMEINSTANCE.GetRenderer()->GetScreenWidth(), GAMEINSTANCE.GetRenderer()->GetScreenHeight(), 1.0, 10000.0f);
	GAMEINSTANCE.GetRenderer()->SetProjMatrix(proj);

	// �v���C���[����
	mPlayer->SetPosition(Vector3(0, 0, 500));
	mPlayer->SetPlayerSceneState(PlayerSceneState::PLAYER_GAMESCENE);
	mPlayer->LoadAnimation();

	// �G�l�~�[�}�l�[�W���[����
	mEnemyManager = new EnemyManager(mPlayer);

	// ���C�g
	GAMEINSTANCE.GetRenderer()->SetAmbientLight(Vector3(0.2f, 0.2f, 0.2f));
	DirectionalLight& dir = GAMEINSTANCE.GetRenderer()->GetDirectionalLight();

	// �f�B���N�V�������C�g
	dir.mDirection    = Vector3(0.7f, -0.7f, -0.7f);
	dir.mDiffuseColor = Vector3(0.78f, 0.88f, 1.0f);
	dir.mSpecColor    = Vector3(0.8f, 0.8f, 0.8f);

	Vector3 color(0, 1, 0);
	mGrid = new DebugGrid( 1000.0f, 30, color );

	// �J��������
	mCamera= new ThirdPersonCamera(mPlayer);
	mCamera->Init(Vector3(1000, 0, 1000), Vector3(0, 0, 0), Vector3(0, 0, 1));
	mCamera->SetCameraLength(3000.0f);

	// �}�b�v�ǂݍ���
	LevelActor* level = new LevelActor();
	Vector3 offset(0, 0, 0);
	level->LoadLevel("Assets/Map/stage.gpmesh", "", offset);
	level->SetScale(3.0f);

	// �e�L�X�g1�ǂݍ���
	mFont = new BitMapText;
	mFont->SetFontImage(16, 6, "assets/UI/sci-fismall.png");
	mFont->ReMapText(" !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_\tabcdefghijklmnopqrstuvwxyz{|}~\\");

	// �e�L�X�g2�ǂݍ���
	mFont2 = new BitMapText;
	mFont2->SetFontImage(16, 6, "assets/UI/sci-fiLarge.png");
	mFont2->ReMapText(" !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_\tabcdefghijklmnopqrstuvwxyz{|}~\\");

	SetColliderPair();

	// FPS�v���N���X����
	mFPSCounter = new FPSCounter(mMaxFps);
}

GameScene::~GameScene()
{
	delete mEnemyManager;
	delete mLevel;
	delete mFPSCounter;
	//delete mCamera;

	delete mFont;
	delete mFont2;
}

SceneBase *GameScene::update()
{
	// �L�[�{�^���������ꂽ��f�o�b�O���[�h
	if (INPUT_INSTANCE.IsKeyPushdown(KEY_START)||INPUT_INSTANCE.IsKeyPushdown(SDL_SCANCODE_F3))
	{
		GAMEINSTANCE.GetPhysics()->ToggleDebugMode();  
	}
	Matrix4 view;
	view = Matrix4::CreateLookAt(Vector3(0, -1000, 1000), Vector3(0, 0, 0), Vector3(0, 0, 1));

	// �G�t�F�N�V�A�}�l�[�W���[�̃A�b�v�f�[�g
	RENDERER->GetEffekseerManager()->Update();
	mEnemyManager->Update(GAMEINSTANCE.GetDeltaTime());

	// �G�E�F�[�u�I���A���̓v���C���[�����S�����烊�U���g�V�[���ֈڍs
	const bool isFinishWave = mEnemyManager->GetWaveFinishFlag();
	const bool isPlayerDie = mPlayer->GetDeadAnimFlag();
	const bool isResuleScene=isFinishWave||isPlayerDie;

	// ���U���g�V�[���֍s�����Ԃ̎�
	if (isResuleScene)
	{
		return new ResultScene;
	}

	mFPSCounter->Update();

	return this;
}

void GameScene::draw()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	//��ʃN���A
	GAMEINSTANCE.GetRenderer()->WindowClear();

	//�Q�[���V�X�e���֘A�`��
	GAMEINSTANCE.GetRenderer()->Draw();

	// �X�v���C�g�̃����_�����O�`��J�n
	RENDERER->SpriteDrawBegin();

	DebugLog();

	// �v���C���[�ʒu
	Vector3 playerPos = mPlayer->GetPosition();

	DirectionalLight dirLight = RENDERER->GetDirectionalLight();
	Vector3 lightDir = dirLight.mDirection;
	lightDir.Normalize();

	// �f�v�X�}�b�v���Z�b�g
	RENDERER->SetDepthSetting(playerPos,lightDir,Vector3::UnitZ,5000.0f);
	RENDERER->SpriteDrawEnd();

	//�G�t�F�N�g�֘A�����ɏ���
	RENDERER->GetEffekseerRenderer()->BeginRendering();
	RENDERER->GetEffekseerManager()->Draw();
	RENDERER->GetEffekseerRenderer()->EndRendering();

	//��ʂ̃t���b�v
	GAMEINSTANCE.GetRenderer()->WindowFlip();
}

void GameScene::DebugLog()
{
	static float anim = 0.0f;
	anim += 0.01f;

	char buf1[256];
	char buf2[256];
	char buf3[256];

	sprintf(buf1, "PlayerPosition(x:%.2f)(y:%.2f)(z:%.2f)", mPlayer->GetPosition().x, mPlayer->GetPosition().y, mPlayer->GetPosition().z);
	sprintf(buf2, "FPS:%.2f", mFPSCounter->GetFPS());
	sprintf(buf3, "Wave %d", mEnemyManager->GetWaveCount() + 1);

	// �t�H���g�Z�b�g
	mFont->TextDraw(50, 25, buf1);
	mFont->TextDraw(50, 50, buf2);
	mFont2->TextDraw(700, RENDERER->GetScreenHeight() / 3, buf3);
}

void GameScene::SetColliderPair()
{
	// �����蔻��̑g�ݍ��킹�Z�b�g
	GAMEINSTANCE.GetPhysics()->SetSelfReaction(Tag::ENEMY);
	GAMEINSTANCE.GetPhysics()->SetDualReactionCollisionPair(Tag::PLAYER_BULLET, Tag::ENEMY);
	GAMEINSTANCE.GetPhysics()->SetDualReactionCollisionPair(Tag::ENEMY, Tag::PLAYER_BULLET);
	GAMEINSTANCE.GetPhysics()->SetDualReactionCollisionPair(Tag::PLAYER, Tag::ENEMY_BULLET);
	GAMEINSTANCE.GetPhysics()->SetDualReactionCollisionPair(Tag::ENEMY_BULLET, Tag::PLAYER);
	GAMEINSTANCE.GetPhysics()->SetDualReactionCollisionPair(Tag::ENEMY, Tag::PLAYER_SPECIAL_SHOT);
	GAMEINSTANCE.GetPhysics()->SetDualReactionCollisionPair(Tag::ENEMY, Tag::PLAYER);
}
