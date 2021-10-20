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
	printf("//タイトルシーン//\n");
	printf("//////////////////\n");

	// プレイヤー生成
	mPlayer = new Player1();
	mPlayer->SetPosition(Vector3(500.0, 0.0f, 0.0f));
	mPlayer->RotateToNewForward(Vector3::NegUnitX);
	mPlayer->SetPlayerSceneState(PlayerSceneState::PLAYER_TITLESCENE);

	// 雑魚敵生成
	mEnemy = new WeakEnemy(mPlayer);
	mEnemy->SetPosition(Vector3(-500.0f, 0.0f, 0.0f));
	mEnemy->RotateToNewForward(Vector3::UnitX);

	// 武器生成
	mWeapon = new Weapon(mPlayer);
	mWeapon->SetPosition(Vector3(0.0f, 0.0f, 100.0f));
	mWeapon->SetScale(1.0f);

	// ライティング設定
	GAMEINSTANCE.GetRenderer()->SetAmbientLight(Vector3(0.2f, 0.2f, 0.2f));
	DirectionalLight& dir = GAMEINSTANCE.GetRenderer()->GetDirectionalLight();
	dir.mDirection = Vector3(0.7f, -0.7f, -0.7f);
	dir.mDiffuseColor = Vector3(0.78f, 0.88f, 1.0f);
	dir.mSpecColor = Vector3(0.8f, 0.8f, 0.8f);

	// 三人称カメラ追加
	CameraActor* c = new CameraActor(mPlayer);
	Vector3 cameraLerpPos = Vector3::Lerp(mPlayer->GetPosition(), mEnemy->GetPosition(), 0.01f);
	c->Init(Vector3(-1000, 0, 1000), cameraLerpPos, Vector3(0, 0, 0));

	// テクスチャ追加
	mTexture = RENDERER->GetTexture("Assets/Image/Title.png");
}

TitleScene::~TitleScene()
{
	delete mPlayer;
	delete mEnemy;

	printf("タイトルシーン終了");
}

SceneBase* TitleScene::update()
{
	// Aキーが押されたら
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
	// 背景色を設定
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	// 画面クリア
	GAMEINSTANCE.GetRenderer()->WindowClear();

	// スプライト描画
	RENDERER->SpriteDrawBegin();
	RENDERER->DrawTexture(mTexture, Vector2(640, 370));
	RENDERER->SpriteDrawEnd();

	// レンダー描画
	GAMEINSTANCE.GetRenderer()->Draw();

	// 画面のフリップ
	GAMEINSTANCE.GetRenderer()->WindowFlip();
}
