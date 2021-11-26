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
 
	// 行列初期化
	Matrix4 proj;
	proj = Matrix4::CreatePerspectiveFOV(Math::ToRadians(45.0f), GAMEINSTANCE.GetRenderer()->GetScreenWidth(), GAMEINSTANCE.GetRenderer()->GetScreenHeight(), 1.0, 10000.0f);
	GAMEINSTANCE.GetRenderer()->SetProjMatrix(proj);

	// ライト
	GAMEINSTANCE.GetRenderer()->SetAmbientLight(Vector3(0.2f, 0.2f, 0.2f));
	DirectionalLight& dir = GAMEINSTANCE.GetRenderer()->GetDirectionalLight();

	// ディレクションライト
	dir.mDirection = Vector3(0.7f, -0.7f, -0.7f);
	dir.mDiffuseColor = Vector3(0.78f, 0.88f, 1.0f);
	dir.mSpecColor = Vector3(0.8f, 0.8f, 0.8f);

	// マップ読み込み
	mLevelActor = new LevelActor();
	Vector3 offset(0, 0, 0);
	mLevelActor->LoadLevel("Assets/Map/Stage.gpmesh", "", offset);
	mLevelActor->SetScale(4.5f);

	// カメラの追加
	mCamera = new CameraActor(nullptr);
	mCamera->Init(Vector3(-300, 0, 1000), Vector3(0, 0, 1000), Vector3(0, 0, 0));

	// テキスト1読み込み
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
	// 背景色を設定
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	// 画面クリア
	GAMEINSTANCE.GetRenderer()->WindowClear();

	//ゲームシステム関連描画
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

	// 画面のフリップ
	GAMEINSTANCE.GetRenderer()->WindowFlip();
}
