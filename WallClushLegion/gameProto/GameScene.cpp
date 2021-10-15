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
	printf("//ゲームシーン//\n");
	printf("////////////////\n");

	// 行列初期化
	Matrix4 proj;
	proj = Matrix4::CreatePerspectiveFOV(Math::ToRadians(45.0f), GAMEINSTANCE.GetRenderer()->GetScreenWidth(), GAMEINSTANCE.GetRenderer()->GetScreenHeight(), 1.0, 10000.0f);
	GAMEINSTANCE.GetRenderer()->SetProjMatrix(proj);

	// プレイヤー生成
	mPlayer->SetPosition(Vector3(0, 0, 500));
	mPlayer->SetPlayerSceneState(PlayerSceneState::PLAYER_GAMESCENE);
	mPlayer->LoadAnimation();

	// エネミーマネージャー生成
	mEnemyManager = new EnemyManager(mPlayer);

	// ライト
	GAMEINSTANCE.GetRenderer()->SetAmbientLight(Vector3(0.2f, 0.2f, 0.2f));
	DirectionalLight& dir = GAMEINSTANCE.GetRenderer()->GetDirectionalLight();

	// ディレクションライト
	dir.mDirection    = Vector3(0.7f, -0.7f, -0.7f);
	dir.mDiffuseColor = Vector3(0.78f, 0.88f, 1.0f);
	dir.mSpecColor    = Vector3(0.8f, 0.8f, 0.8f);

	Vector3 color(0, 1, 0);
	mGrid = new DebugGrid( 1000.0f, 30, color );

	// カメラ生成
	mCamera= new ThirdPersonCamera(mPlayer);
	mCamera->Init(Vector3(1000, 0, 1000), Vector3(0, 0, 0), Vector3(0, 0, 1));
	mCamera->SetCameraLength(3000.0f);

	// マップ読み込み
	LevelActor* level = new LevelActor();
	Vector3 offset(0, 0, 0);
	level->LoadLevel("Assets/Map/stage.gpmesh", "", offset);
	level->SetScale(3.0f);

	// テキスト1読み込み
	mFont = new BitMapText;
	mFont->SetFontImage(16, 6, "assets/UI/sci-fismall.png");
	mFont->ReMapText(" !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_\tabcdefghijklmnopqrstuvwxyz{|}~\\");

	// テキスト2読み込み
	mFont2 = new BitMapText;
	mFont2->SetFontImage(16, 6, "assets/UI/sci-fiLarge.png");
	mFont2->ReMapText(" !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_\tabcdefghijklmnopqrstuvwxyz{|}~\\");

	SetColliderPair();

	// FPS計測クラス生成
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
	// キーボタンが押されたらデバッグモード
	if (INPUT_INSTANCE.IsKeyPushdown(KEY_START)||INPUT_INSTANCE.IsKeyPushdown(SDL_SCANCODE_F3))
	{
		GAMEINSTANCE.GetPhysics()->ToggleDebugMode();  
	}
	Matrix4 view;
	view = Matrix4::CreateLookAt(Vector3(0, -1000, 1000), Vector3(0, 0, 0), Vector3(0, 0, 1));

	// エフェクシアマネージャーのアップデート
	RENDERER->GetEffekseerManager()->Update();
	mEnemyManager->Update(GAMEINSTANCE.GetDeltaTime());

	// 敵ウェーブ終了、又はプレイヤーが死亡したらリザルトシーンへ移行
	const bool isFinishWave = mEnemyManager->GetWaveFinishFlag();
	const bool isPlayerDie = mPlayer->GetDeadAnimFlag();
	const bool isResuleScene=isFinishWave||isPlayerDie;

	// リザルトシーンへ行ける状態の時
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

	//画面クリア
	GAMEINSTANCE.GetRenderer()->WindowClear();

	//ゲームシステム関連描画
	GAMEINSTANCE.GetRenderer()->Draw();

	// スプライトのレンダリング描画開始
	RENDERER->SpriteDrawBegin();

	DebugLog();

	// プレイヤー位置
	Vector3 playerPos = mPlayer->GetPosition();

	DirectionalLight dirLight = RENDERER->GetDirectionalLight();
	Vector3 lightDir = dirLight.mDirection;
	lightDir.Normalize();

	// デプスマップをセット
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
	char buf3[256];

	sprintf(buf1, "PlayerPosition(x:%.2f)(y:%.2f)(z:%.2f)", mPlayer->GetPosition().x, mPlayer->GetPosition().y, mPlayer->GetPosition().z);
	sprintf(buf2, "FPS:%.2f", mFPSCounter->GetFPS());
	sprintf(buf3, "Wave %d", mEnemyManager->GetWaveCount() + 1);

	// フォントセット
	mFont->TextDraw(50, 25, buf1);
	mFont->TextDraw(50, 50, buf2);
	mFont2->TextDraw(700, RENDERER->GetScreenHeight() / 3, buf3);
}

void GameScene::SetColliderPair()
{
	// 当たり判定の組み合わせセット
	GAMEINSTANCE.GetPhysics()->SetSelfReaction(Tag::ENEMY);
	GAMEINSTANCE.GetPhysics()->SetDualReactionCollisionPair(Tag::PLAYER_BULLET, Tag::ENEMY);
	GAMEINSTANCE.GetPhysics()->SetDualReactionCollisionPair(Tag::ENEMY, Tag::PLAYER_BULLET);
	GAMEINSTANCE.GetPhysics()->SetDualReactionCollisionPair(Tag::PLAYER, Tag::ENEMY_BULLET);
	GAMEINSTANCE.GetPhysics()->SetDualReactionCollisionPair(Tag::ENEMY_BULLET, Tag::PLAYER);
	GAMEINSTANCE.GetPhysics()->SetDualReactionCollisionPair(Tag::ENEMY, Tag::PLAYER_SPECIAL_SHOT);
	GAMEINSTANCE.GetPhysics()->SetDualReactionCollisionPair(Tag::ENEMY, Tag::PLAYER);
}
