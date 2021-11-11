#pragma once

#include <unordered_map>
#include <string>
#include <vector>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_types.h>
#include "math.h"
#include "Tag.h"

class GameObject;
class SceneBase;
class Renderer;
class CameraActor;
class PhysicsWorld;
class AudioManager;
class FPSCounter;

class Game
{
private:
	Game();				// シングルトン

	void Input();		// 入力処理
	void Draw();        // 描画
	int  Update();      // フレーム更新処理
	void ActorUpdate(); // アクターアップデート
	void ShowActor();   // アクターリスト表示（デバッグ用）

	Renderer* mRenderer;			// レンダリングエンジン
	SceneBase* mNowScene;           // ベースシーンクラス
	SceneBase* tmpScene;			// シーン入れ替え用
	CameraActor* mActiveCamera;     // システムが描画に使うカメラ 
	PhysicsWorld* mPhysicsWorld;    // あたり判定システム
	AudioManager* mAudio;           // サウンド関連

	bool  mIsRunning;               // ゲームループ回すか？
	bool  mIsPauseMode;             // ポーズモード

	std::unordered_map<Tag,std::vector<GameObject*>> mActors;	// アクター配列
	std::vector<GameObject*> mPendingActors;					// アクター追加準備用配列

	float    mDeltaTime;	// 1フレームの経過時間（秒単位）
	Uint32   mTicksCount;	// 経過時間（ミリ秒単位）
	Matrix4  mViewMatrix;	// ビュー行列

	GameObject* mPlayerActor;	// プレイヤーアクター
	GameObject* mEnemyActor;	// 敵アクター
	FPSCounter* mFPSCounter;

	int mScore;
public:
	~Game();
	static Game& GetInstance()
	{
		static Game instance;
		return instance;
	}

	void Run();                                                                    // ゲーム本体実行
	void Shutdown();                                                               // 終了時
	bool Initialize(int screenWidth, int screenHeight, bool fullScreen = false);   // 画面初期化設定

	void setFirstScene(SceneBase* pScene) { mNowScene = pScene; }                  // 開始シーンのセット
	void setShutDown() { mIsRunning = false; }                                     // シャットダウン

	Renderer* GetRenderer() { return mRenderer; }									// レンダラーシステムの取得
	SDL_Renderer* GetSDLRenderer();                                                // 2DのSDLレンダラーの取得

	void AddActor(GameObject* actor);                                             // アクターの追加
	void RemoveActor(GameObject* actor);                                          // アクターの削除

	void SetCameraActor(CameraActor* camera);      // カメラアクターをシステムに登録
	void InActiveCamera(CameraActor* inActiveCam); // カメラの登録を解除

	void SetPlayerActor(GameObject* player);              // プレイヤーアクターの登録
	GameObject* GetPlayerActor(); // プレイヤーアクターの取得
	GameObject* GetEnemyActor();

	const Matrix4& GetViewMatrix();                                                      // 現在セットされているカメラのビュー行列を返す
	const Vector3& GetViewTarget();
	const Vector3& GetViewPos();
	const float    GetDeltaTime() { return mDeltaTime; }
	PhysicsWorld* const GetPhysics() { return mPhysicsWorld; }

	AudioManager* const GetAudio() { return mAudio; }

	std::vector<class GameObject*>const& GetActors(Tag type);
	class GameObject* GetFirstActor(Tag type);

	bool IsExistActorType(Tag type);                                               // そのアクタータイプは存在するか？
	class GameObject* FindActorFromID(int searchActorID);                                      // アクターIDからアクターへのポインタを検索する

	void SetScore(int& score) { mScore = score; }
	int GetScore() { return mScore; }
};

#define GAMEINSTANCE Game::GetInstance()
#define RENDERER     Game::GetInstance().GetRenderer()
#define AUDIO        Game::GetInstance().GetAudio()

