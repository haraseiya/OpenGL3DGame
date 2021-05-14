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
	printf("//タイトルシーン//\n");
	printf("//////////////////\n");

	// プレイヤー追加
	m_player = new Player();
	m_player->SetPosition(Vector3(0, 0, 0));

	// ライト
	GAMEINSTANCE.GetRenderer()->SetAmbientLight(Vector3(0.2f, 0.2f, 0.2f));
	DirectionalLight& dir = GAMEINSTANCE.GetRenderer()->GetDirectionalLight();
	dir.mDirection = Vector3(0.7f, -0.7f, -0.7f);
	dir.mDiffuseColor = Vector3(0.78f, 0.88f, 1.0f);
	dir.mSpecColor = Vector3(0.8f, 0.8f, 0.8f);

	Vector3 color(0, 1, 0);
	mGrid = new DebugGrid(1000.0f, 30, color);

	// カメラ追加
	CameraActor* c = new CameraActor(m_player);
	c->Init(Vector3(150, 0, 150), Vector3(0, 0, 100), Vector3(0, 0, 0));

	m_texture = RENDERER->GetTexture("Assets/Image/Title.png");
	//// FBXマネージャー作成
	//manager = FbxManager::Create();

	//// 入出力設定を作成
	//ioSettings = FbxIOSettings::Create(manager, IOSROOT);

	//// マネージャーに入出力設定をセットする
	//manager->SetIOSettings(ioSettings);

	//// インポーターの作成
	//FbxImporter* importer = FbxImporter::Create(manager, "");

	//// インポーターの初期化
	//const char* fileName;
	//importer->Initialize(fileName);

	//// 初期化が失敗したらエラーログを出力
	//if (importer->GetStatus().GetErrorString())return;

	//// シーンの作成
	//FbxScene* scene = FbxScene::Create(manager, "TestScene");

	//// シーンに読み込んだFBXデータをインポートする
	//importer->Import(scene);

	//// インポーターは用済みなので破棄
	//importer->Destroy();

	//// 三角ポリゴンのみに変換するコンバーターを作成
	//FbxGeometryConverter geometryConverter(manager);

	//// シーンに含まれるすべてのポリゴンを三角形に変換
	//geometryConverter.Triangulate(scene, true);

	//// ノード数の取得
	//int nodeCount = scene->GetNodeCount();

	//// シーンからノードの番号を指定して取得
	//int index;
	//FbxNode* node = scene->GetNode(index);

	//// シーンからルートノードを取得
	//FbxNode* rootNode = scene->GetRootNode();

	//// 子ノードの数を取得
	//int childCount = node->GetChildCount();

	//// 子ノードを取得する
	//FbxNode* child = node->GetChild(index);

	//// ノードの名前を取得
	//const char* nodeName = node->GetName();

	//// ノードの位置、回転、スケールを取得
	//FbxDouble3 translation = node->LclTranslation.Get();
	//FbxDouble3 rotation = node->LclRotation.Get();
	//FbxDouble3 scaling = node->LclScaling.Get();
}

TitleScene::~TitleScene()
{
	delete m_player;
	printf("タイトルシーン終了");
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
	// 背景色を設定
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	// 画面クリア
	GAMEINSTANCE.GetRenderer()->WindowClear();


	GAMEINSTANCE.GetRenderer()->Draw();

	//RENDERER->SpriteDrawBegin();
	//RENDERER->DrawTexture(m_texture, Vector2(640, 370));
	//RENDERER->SpriteDrawEnd();

	// 画面のフリップ
	GAMEINSTANCE.GetRenderer()->WindowFlip();
}
