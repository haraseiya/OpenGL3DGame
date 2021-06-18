// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#pragma once
#include <vector>
#include <cstdint>
#include "Math.h"
#include "Tag.h"

class GameObject
{
public:
	enum State
	{
		EActive,
		EPaused,
		EDead
	};

	GameObject(Tag actorTag);
	virtual ~GameObject();

	void              Update(float deltaTime);                                                     // Update関数　Gameからコールされる（オーバーライド不可）
	void              UpdateComponents(float deltaTime);                                           // アタッチされてる全てのコンポーネントたちの更新（オーバーライド不可）
	virtual void      UpdateActor(float deltaTime);                                                // アクター専用のアップデート　（オーバーライド可能）
	void              ProcessInput();                                                              // ゲームから呼ばれる入力関連（オーバーライド不可）
	virtual void      OnCollisionEnter(class ColliderComponent* otherBox);                         // 他の物体と当たった時

	// Getters/setters　ゲッター・セッター
	const Vector3& GetPosition() const { return mPosition; }                                                            // 位置のゲット
	void              SetPosition(const Vector3& pos) { mPosition = pos; mRecomputeWorldTransform = true; }                // 位置のセット
	float             GetScale() const { return mScale; }                                                                  // スケーリングのゲット
	void              SetScale(float scale) { mScale = scale;  mRecomputeWorldTransform = true; }                          // スケーリングのセット
	const Quaternion& GetRotation() const { return mRotation; }                                                            // 回転（クォータニオン）のゲット
	void              SetRotation(const Quaternion& rotation) { mRotation = rotation;  mRecomputeWorldTransform = true; }  // 回転（クォータニオン）のセット
	Tag               GetTag() { return mTag; }

	void              ComputeWorldTransform();                                                     // ワールド変換行列の計算
	void              SetComputeWorldTransform() { mRecomputeWorldTransform = true; }              // 移動など変換行列の再計算が必要な時
	const Matrix4& GetWorldTransform() const { return mWorldTransform; }                        // ワールド変換行列のゲット
	Vector3           GetForward() const { return Vector3::Transform(Vector3::UnitX, mRotation); } // 前進ベクトルのゲット
	Vector3           GetRight() const { return Vector3::Transform(Vector3::UnitY, mRotation); }   // 右方向ベクトルのゲット
	State             GetState() const { return mState; }                                          // アクターの状態ゲット
	float             GetSpeed() const { return mSpeed; }                                          // アクターの現在のスピード
	void              SetSpeed(float speed) { mSpeed = speed; }
	void              SetState(State state) { mState = state; }                                    // アクターの状態セット
	int               GetID() { return mID; };                                                     // アクターの管理IDのゲット
	void              AddComponent(class Component* component);                                    // コンポーネントの追加 
	void              RemoveComponent(class Component* component);                                 // コンポーネントの削除 
	void              RotateToNewForward();                                                        // 前進ベクトルから回転を計算

protected:

	Tag               mTag;                                                                        // アクター種別
	State             mState;                                                                      // アクターの状態
	Matrix4           mWorldTransform;                                                             // ワールド変換行列
	Vector3           mPosition;                                                                   // 位置
	Vector3           mDirection;																
	Quaternion        mRotation;                                                                   // 回転
	float             mScale;                                                                      // スケーリング
	float             mSpeed;                                                                      // 現在のスピード
	bool              mRecomputeWorldTransform;                                                    // ワールド変換の再計算が必要か？
	int               mID;                                                                         // 管理ID
	static int        mGlobalActorNo;                                                              // 管理ID用の静的カウンタ

	std::vector<class Component*> mDeleteComponents;                                               // 削除コンポーネント
	std::vector<class Component*> mComponents;                                                     // コンポーネント配列                                          
};
