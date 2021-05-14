// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "GameObject.h"
#include "Game.h"
#include "Component.h"
#include <algorithm>
#include <typeinfo>

int GameObject::mGlobalActorNo = 0;

// Actorコンストラクタ  
GameObject::GameObject()
	: mState(EActive)
	, mPosition(Vector3::Zero)
	, mRotation(Quaternion::Identity)
	, mRecomputeWorldTransform(true)
	, mID(mGlobalActorNo)
	, mSpeed(0.0f)
{
	// 大きさを設定
	mScale = Vector3(1, 1, 1);

	//ゲームシステム本体に アクターを追加
	GAMEINSTANCE.AddActor(this);
	mGlobalActorNo++;
}

GameObject::~GameObject()
{
	// アクターが持っているコンポーネントの削除
	while (!mComponents.empty())
	{
		delete mComponents.back();
	}

	// ゲームシステム本体にこのアクターの削除を依頼
	GAMEINSTANCE.RemoveActor(this);
}

// Update関数　Gameからコールされる
// 引数 in : deltaTime  1フレーム分の経過時間
void GameObject::Update(float deltaTime)
{
	// アクターが生きているときのみ処理
	if (mState == EActive)
	{
		ComputeWorldTransform();

		UpdateComponents(deltaTime);
		UpdateActor(deltaTime);

		ComputeWorldTransform();
	}
}

// 全てのコンポーネントの更新処理
// 引数 in : deltaTime  1フレーム分の経過時間
void GameObject::UpdateComponents(float deltaTime)
{
	for (auto comp : mComponents)
	{
		comp->Update(deltaTime);
	}
}

// このアクター独自の更新処理 （必要ならオーバーライド）
void GameObject::UpdateActor(float deltaTime)
{
}

// このアクターが持っているコンポーネントの入力処理
void GameObject::ProcessInput()
{
	if (mState == GameObject::EActive)
	{
		// 入力処理を受け取るコンポーネントを優先して実行
		for (auto comp : mComponents)
		{
			comp->ProcessInput();
		}
	}
}

// forwardベクトルの向きに回転する
// in forward : 向かせたい前方方向ベクトル
void GameObject::RotateToNewForward(const Vector3& forward)
{
	// X軸ベクトル(1,0,0)とforwardの間の角度を求める
	float dot = Vector3::Dot(Vector3::UnitX, forward);
	float angle = Math::Acos(dot);

	// 下向きだった場合
	if (dot > 0.9999f)
	{
		SetRotation(Quaternion::Identity);
	}
	// 上向きだった場合
	else if (dot < -0.9999f)
	{
		SetRotation(Quaternion(Vector3::UnitZ, Math::Pi));
	}
	else
	{
		// 軸ベクトルとforwardとの外積から回転軸をもとめ、回転させる
		Vector3 axis = Vector3::Cross(Vector3::UnitX, forward);
		axis.Normalize();
		SetRotation(Quaternion(axis, angle));
	}
}

void GameObject::SetScale(float scale)
{
	mScale = Vector3(scale, scale, scale);
	mRecomputeWorldTransform = true;
}

void GameObject::SetScale(const Vector3& scale)
{
	mScale = scale;
	mRecomputeWorldTransform = true;
}

void GameObject::SetRotation(const Vector3& rotation)
{
	Matrix4 mat;
	mat = Matrix4::CreateRotationX(rotation.x);
	mat *= Matrix4::CreateRotationY(rotation.y);
	mat *= Matrix4::CreateRotationZ(rotation.z);

	mRotation = Quaternion::QuaternionFromMatrix(mat);
	mRecomputeWorldTransform = true;
}

// ワールド変換行列を計算
// 引数 なし
void GameObject::ComputeWorldTransform()
{
	//ワールド変換の再計算が必要なら実行
	if (mRecomputeWorldTransform)
	{
		mRecomputeWorldTransform = false;

		// スケーリング→回転→平行移動となるように変換行列を作成
		mWorldTransform = Matrix4::CreateScale(mScale);
		mWorldTransform *= Matrix4::CreateFromQuaternion(mRotation);
		mWorldTransform *= Matrix4::CreateTranslation(mPosition);

		// アクターが持っている全コンポーネントの変換を促す
		for (auto comp : mComponents)
		{
			comp->OnUpdateWorldTransform();
		}
	}
}

// コンポーネントの追加
// 引数　in : component 追加コンポーネントへのポインタ
void GameObject::AddComponent(Component* component)
{
	// コンポーネントをソートして追加
	// 自分のオーダー番号よりも大きい挿入点を見つける
	int myOrder = component->GetUpdateOrder();
	auto iter = mComponents.begin();
	for (;
		iter != mComponents.end();
		++iter)
	{
		if (myOrder < (*iter)->GetUpdateOrder())
		{
			break;
		}
	}
	// 要素を見つけたポイントの手前に挿入する
	mComponents.insert(iter, component);
}

// コンポーネントの削除
// 引数　in : component  削除コンポーネントへのポインタ 
void GameObject::RemoveComponent(Component* component)
{
	auto iter = std::find(mComponents.begin(), mComponents.end(), component);
	if (iter != mComponents.end())
	{
		mComponents.erase(iter);
	}
}
