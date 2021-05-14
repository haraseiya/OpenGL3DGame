#pragma once
#include "GameObject.h"

class GameObject;

class CameraActor
{
public:
	CameraActor(GameObject* targetActor);
	virtual ~CameraActor();
	virtual void UpdateActor(float deltaTime);

	void     Update(float deltaTime);      //オーバーライド不可
	void     SetActive();
	void     Init(const Vector3& cameraPos, const Vector3& targetPos, const Vector3& upVec);
	Matrix4& GetViewMatrix() { return mViewMatrix; }
	Vector3& GetViewTarget() { return mViewTarget; }
	Vector3& GetViewPos() { return mPosition; }

protected:
	GameObject* mTargetActor;  // カメラが基準とするアクター
	Matrix4  mViewMatrix;   // ビュー行列
	Vector3  mPosition;
	Vector3  mViewTarget;   // カメラ注目点
	Vector3  mViewVector;   // 視線ベクトル
};