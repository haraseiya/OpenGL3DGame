#pragma once
#include "Game.h"
#include "Tag.h"

#include <vector>
#include <unordered_map>

class BoxCollider;
class Shader;
class NPCState;

enum class EnumPhysicsType
{
	EnumPlayer,				// プレイヤー
	EnumPlayerTrigger,		// プレイヤーの詠唱範囲用
	EnumEnemy,				// 敵キャラ
	EnumEnemyAttackBox,		// 敵の攻撃用
	EnumEnemyAttackTrigger,	// 敵の攻撃用トリガー
	EnumNPC,				// NPC
	EnumNPCAttackBox,		// NPCの攻撃用
	EnumNPCAttackTrigger,	// NPCの攻撃用トリガー
	EnumBG,					// 背景ブロック
	EnumHitCheck,			// ヒットチェック用（押し返しなし)
	EnumBGTrigger,			// 背景との接触トリガー
	EnumNPCTrigger			// NPCとの接触トリガー
};

class PhysicsWorld
{
public:
	struct collidePairs
	{
		Tag pair1;
		Tag pair2;
	};

	PhysicsWorld();
	~PhysicsWorld();

	// BoxCollider(AABB)追加・削除
	void AddBoxCollider(EnumPhysicsType type,BoxCollider* box); 
	void RemoveBoxCollider(BoxCollider* box);          

	// SphereCollider追加・削除
	//void AddSphereCollider(EnumPhysicsType type, Sphere* sphere);
	//void RemoveSphereCollider(Sphere* sphere);
	
	// コリジョン
	void Collision();

	void AddCollider(class ColliderComponent* collider);               // Collider追加
	void RemoveCollider(class ColliderComponent* collider);            // Collider削除

	// デバッグ用
	void DebugShowBox();
	void DebugShowBoxLists();                                 
	void ToggleDebugMode() { mBoolDebugMode = !mBoolDebugMode; }

private:
	std::vector<BoxCollider*> mBGBoxs;			// 背景あたりデータ
	std::vector<BoxCollider*> mPlayerBoxs;      // プレーヤーあたりデータ
	std::vector<BoxCollider*> mPlayerTrigger;
	std::vector<BoxCollider*> mEnemyBoxs;       // 敵キャラ   
	std::vector<BoxCollider*> mEnemyAttackBoxs;	// 敵の攻撃用
	std::vector<BoxCollider*> mEnemyAttackTrigger;
	std::vector<BoxCollider*> mBGTriggers;      // 背景との接触判定用トリガー  
	std::vector<BoxCollider*> mNPCTriggers;		// NPCとの接触トリガー
	std::vector<BoxCollider*> mPlayerAttackBox;	// プレイヤー攻撃判定用ボックス
	std::vector<BoxCollider*> mNPCBoxs;			// NPCの当たり判定ボックス
	std::vector<BoxCollider*> mNPCAttackBoxs;	// NPCの攻撃用

	std::unordered_map<Tag, std::vector<class ColliderComponent*>> mColliders;    // 当たり判定ボックスリスト

	void PlayerAndBGTest();				// プレーヤーと壁とのあたり判定
	void PlayerAndEnemyTest();			// プレイヤーと敵の当たり判定
	void PlayerAndNPCTest();			// プレイヤーとNPCの当たり判定
	void EnemyAndBGTest();				// 敵と壁との当たり判定
	void EnemyAndNPCTest();				// 敵とNPCの当たり判定
	void EnemyAttackAndNPCTest();		// 敵の攻撃とNPCの当たり判定
	void EnemyTriggerAndNPCTest();
	void TriggerAndBGTest();			// 背景トリガーと背景の判定テスト
	void NPCAndEenmyTest();				// NPCと敵の当たり判定
	void NPCAndNPCTest();				// NPCとNPCの当たり判定
	void NPCAttackAndEnemyTest();		// NPCの攻撃と敵の当たり判定
	void InitBoxVertices();				// ボックス頂点の初期化
	void DrawBoxs(std::vector<BoxCollider*>& boxs, const Vector3& color);

	bool mBoolDebugMode;	// デバッグモードを使用するか
	unsigned int m_boxVAO;	// ボックス描画用のVAO  
	unsigned int m_VBO;
	unsigned int m_EBO;

	Shader* mLineShader;	// ライン描画用シェーダー
	NPCState* m_npcState;	// NPCの状態
};