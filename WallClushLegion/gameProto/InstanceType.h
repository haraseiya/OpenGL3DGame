#pragma once


// インスタンスのタイプ
enum class InstanceType
{
	Begin,
	PlayerBullet = 0,	// プレイヤーの弾
	EnergyCapsule,		// エネルギーカプセル

	InstanceTypeNum,

	End
};

InstanceType& operator++(InstanceType& rhs);