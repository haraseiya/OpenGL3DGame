#pragma once


// �C���X�^���X�̃^�C�v
enum class InstanceType
{
	Begin,
	PlayerBullet = 0,	// �v���C���[�̒e
	EnergyCapsule,		// �G�l���M�[�J�v�Z��

	InstanceTypeNum,

	End
};

InstanceType& operator++(InstanceType& rhs);