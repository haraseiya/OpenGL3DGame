#pragma once
#include "Game.h"
#include "BoxCollider.h"

class BoxCollider;
class Shader;
class NPCState;

enum class EnumPhysicsType
{
	EnumEnemy,			// 敵キャラ
	EnumPlayer,			// プレーヤー
	EnumNPC,			// NPC
	EnumBG,				// 背景ブロック
	EnumHitCheck,		// ヒットチェック用（押し返しなし)
	EnumEnemyAttack,	// 敵の攻撃用
	EnumBGTrigger,      // 背景との接触トリガー
	EnumNPCTrigger		// NPCとの接触トリガー
};

class PhysicsWorld
{
public:
	PhysicsWorld();
	~PhysicsWorld();

	// BoxCollider(AABB)追加・削除
	void AddBoxCollider(EnumPhysicsType type,BoxCollider* box); 
	void RemoveBoxCollider(BoxCollider* box);          

	// コリジョン
	void Collision();

	// デバッグ用
	void DebugShowBox();
	void DebugShowBoxLists();                                 
	void ToggleDebugMode() { mBoolDebugMode = !mBoolDebugMode; }

private:
	std::vector<BoxCollider*> mBGBoxs;			// 背景あたりデータ
	std::vector<BoxCollider*> mPlayerBoxs;      // プレーヤーあたりデータ
	std::vector<BoxCollider*> mEnemyBoxs;       // 敵キャラ   
	std::vector<BoxCollider*> mEnemyAttackBoxs;
	std::vector<BoxCollider*> mBGTriggers;      // 背景との接触判定用トリガー  
	std::vector<BoxCollider*> mNPCTriggers;		// NPCとの接触トリガー
	std::vector<BoxCollider*> mPlayerAttackBox;	// プレイヤー攻撃判定用ボックス
	std::vector<BoxCollider*> mNPCBoxs;			// NPCの当たり判定ボックス

	void PlayerAndBGTest();				// プレーヤーと壁とのあたり判定
	void PlayerAndEnemyTest();			// プレイヤーと敵の当たり判定
	void PlayerAndNPCTest();			// プレイヤーとNPCの当たり判定
	void EnemyAndBGTest();				// 敵と壁との当たり判定
	void EnemyAndNPCTest();				// 敵とNPCの当たり判定
	void TriggerAndBGTest();			// 背景トリガーと背景の判定テスト
	void NPCAndEenmyTest();				// NPCと敵の当たり判定
	void NPCAndNPCTest();				// NPCとNPCの当たり判定
	void InitBoxVertices();				// ボックス頂点の初期化
	void DrawBoxs(std::vector<BoxCollider*>& boxs, const Vector3& color);

	bool mBoolDebugMode;	// デバッグモードを使用するか
	unsigned int m_boxVAO;	// ボックス描画用のVAO  
	unsigned int m_VBO;
	unsigned int m_EBO;

	Shader* mLineShader;	// ライン描画用シェーダー
	NPCState* m_npcState;	// NPCの状態
};