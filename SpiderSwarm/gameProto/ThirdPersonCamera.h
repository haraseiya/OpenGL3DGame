#pragma once
#include "CameraActor.h"

class GameObject;

class ThirdPersonCamera : public CameraActor
{
public:
	ThirdPersonCamera(GameObject* targetActor);
	~ThirdPersonCamera();
	void UpdateActor(float deltaTime) override;

	void SetRotateZAngle(float angle) { mRotateZAngle = angle; }
	void SetLookDownAngle(float angle) { mLookDownAngle = angle; }
	void SetCameraLength(float length) { mCameraLength = length; }
	void SetCameraPos(Vector3& vec) { mCameraPos = vec; }

	Vector3& GetCameraForwardVec() { return mCameraForwardVec; }
	Vector3& GetCameraPos() { return mCameraPos; }

private:
	float mCameraLength;         // カメラ距離
	float mRotateZAngle;         // カメラ周囲回転方向回転角
	float mLookDownAngle;        // カメラ見降ろし方向回転角

	Vector3 mCameraForwardVec;   // 前方方向
	Vector3 mCameraPos;          // カメラ位置
};