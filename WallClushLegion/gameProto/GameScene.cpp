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

#pragma warning(disable:4996)

const int GameScene::mAttackerNum = 20;
const int GameScene::mClasherNum = 0;
const int GameScene::mHealerNum = 0;
const int GameScene::mAllNPCNum = mAttackerNum + mClasherNum + mHealerNum;

GameScene::GameScene(PlayerBase* player)
	: mPlayer(nullptr)
	, m_bossEnemy(nullptr)
	, mNPCManager(nullptr)
	, m_survivalAttacker(0)
	, m_survivalClasher(0)
	, m_survivalHealer(0)
	, m_survivalNPC(0)
{ 
	printf("////////////////\n");
	printf("//ゲームシーン//\n");
	printf("////////////////\n");

	// 行列初期化
	Matrix4 proj;
	proj = Matrix4::CreatePerspectiveFOV(Math::ToRadians(45.0f), GAMEINSTANCE.GetRenderer()->GetScreenWidth(), GAMEINSTANCE.GetRenderer()->GetScreenHeight(), 1.0, 10000.0f);

	//GAMEINSTANCE.GetRenderer()->SetViewMatrix(view);
	GAMEINSTANCE.GetRenderer()->SetProjMatrix(proj);

	// プレーヤー生成
	mPlayer = player;
	mPlayer->SetPosition(Vector3(0, 0, 500));

	// エネミーマネージャー生成
	mEnemyManager = new EnemyManager(mPlayer);

	// ライト
	GAMEINSTANCE.GetRenderer()->SetAmbientLight(Vector3(0.2f, 0.2f, 0.2f));
	DirectionalLight& dir = GAMEINSTANCE.GetRenderer()->GetDirectionalLight();

	dir.mDirection    = Vector3(0.7f, -0.7f, -0.7f);
	dir.mDiffuseColor = Vector3(0.78f, 0.88f, 1.0f);
	dir.mSpecColor    = Vector3(0.8f, 0.8f, 0.8f);

	Vector3 color(0, 1, 0);
	mGrid = new DebugGrid( 1000.0f, 30, color );

	// カメラ生成
	ThirdPersonCamera* c = new ThirdPersonCamera(mPlayer);
	c->Init(Vector3(1000, 0, 1000), Vector3(0, 0, 0), Vector3(0, 0, 1));
	c->SetCameraLength(1500.0f);

	// マップ読み込み
	LevelActor* level = new LevelActor();
	Vector3 offset(0, 0, 0);
	level->LoadLevel("assets/Mesh/stage.gpmesh", "", offset);
	level->SetScale(3.0f);

	// テキスト読み込みインスタンス生成
	mFont = new BitMapText;
	mFont->SetFontImage(16, 6, "Assets/font.png");
	mFont->ReMapText(" !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_\tabcdefghijklmnopqrstuvwxyz{|}~\\");

	//GAMEINSTANCE.GetPhysics()->SetOneSideReactionCollisionPair(Tag::Enemy,Tag::Enemy);
	// 当たり判定の組み合わせセット
	GAMEINSTANCE.GetPhysics()->SetSelfReaction(Tag::Enemy);
	GAMEINSTANCE.GetPhysics()->SetDualReactionCollisionPair(Tag::PlayerBullet,Tag::Enemy);
	GAMEINSTANCE.GetPhysics()->SetDualReactionCollisionPair(Tag::Enemy, Tag::PlayerBullet);

	mFPSCounter = new FPSCounter();
	//GAMEINSTANCE.GetPhysics()->SetSelfReaction(Tag::Enemy);
}

GameScene::~GameScene()
{
	delete mPlayer;
	delete m_bossEnemy;
	delete mLevel;
}

SceneBase *GameScene::update()
{
	if (INPUT_INSTANCE.IsKeyPushdown(KEY_START)||INPUT_INSTANCE.IsKeyPushdown(SDL_SCANCODE_F3))
	{
		GAMEINSTANCE.GetPhysics()->ToggleDebugMode();  
	}
	
	GAMEINSTANCE.GetDeltaTime();
	Matrix4 view;
	view = Matrix4::CreateLookAt(Vector3(0, -1000, 1000), Vector3(0, 0, 0), Vector3(0, 0, 1));

	RENDERER->GetEffekseerManager()->Update();
	mEnemyManager->Update(GAMEINSTANCE.GetDeltaTime());

	// 終了フラグが立ったらシーン遷移
	if (mEnemyManager->GetWaveFinishFlag())
	{
		return new ResultScene;
	}

	mFPSCounter->Update();

	return this;
}

void GameScene::draw()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	//画面クリア
	GAMEINSTANCE.GetRenderer()->WindowClear();

	//ゲームシステム関連描画
	GAMEINSTANCE.GetRenderer()->Draw();

	RENDERER->SpriteDrawBegin();

	DebugLog();

	Vector3 playerPos = mPlayer->GetPosition();

	DirectionalLight dirLight = RENDERER->GetDirectionalLight();
	Vector3 lightDir = dirLight.mDirection;
	lightDir.Normalize();

	RENDERER->SetDepthSetting(playerPos,lightDir,Vector3::UnitZ,5000.0f);
	RENDERER->SpriteDrawEnd();

	//エフェクト関連ここに書く
	RENDERER->GetEffekseerRenderer()->BeginRendering();
	RENDERER->GetEffekseerManager()->Draw();
	RENDERER->GetEffekseerRenderer()->EndRendering();

	//画面のフリップ
	GAMEINSTANCE.GetRenderer()->WindowFlip();
}

void GameScene::DebugLog()
{
	static float anim = 0.0f;
	anim += 0.01f;

	char buf1[256];
	char buf2[256];

	sprintf(buf1, "PlayerPosition(x:%.2f)(y:%.2f)(z:%.2f)", mPlayer->GetPosition().x, mPlayer->GetPosition().y, mPlayer->GetPosition().z);
	sprintf(buf2, "FPS:%.2f", mFPSCounter->GetFPS());

	mFont->TextDraw(50, 25, buf1);
	mFont->TextDraw(50, 50, buf2);
}

