#pragma once
#include "Game.h"

class PhysicsWorld
{
public:
	typedef enum {
		EnumEnemy,                // 敵キャラ
		EnumPlayer,				  // プレーヤー
		EnumBG,					  // 背景ブロック
		EnumCoin,				  // コイン
		EnumHitCheck			  // ヒットチェック用（押し返しなし)
	}EnumPhysicsType;

	PhysicsWorld();
	~PhysicsWorld();
	void AddBoxCollider(EnumPhysicsType type, class BoxCollider* box); // BoxCollider(AABB)追加
	void RemoveBoxCollider(class BoxCollider* box);                    // BoxCollider削除
	void DebugShowBoxLists();                                          // ボックスリスト表示（デバッグ用)
	void Collision();                                                  // コリジョン
	void DebugShowBox();                                               // デバッグ用ボックス表示（未実装）
	void ToggleDebugMode() { mBoolDebugMode = !mBoolDebugMode; }       // デバッグモード

private:
	bool                              mBoolDebugMode ;                 // デバッグモード
	std::vector<class BoxCollider*>   mBGBoxs        ;                 // 背景あたりデータ
	std::vector<class BoxCollider*>   mPlayerBoxs    ;                 // プレーヤーあたりデータ
	std::vector<class BoxCollider*>   mCoins         ;                 // コインあたりデータ

	void PlayerAndBGTest(); // プレーヤーと壁とのあたり判定

};