#pragma once
#include <string>
#include <stdexcept>
#include <iterator>

// �Q�[�����A�N�^�[�E�����蔻��̎�ޕʃ^�O
// �K�v�Ȃ��̂�Begin��End�̊Ԃ�255�܂Œ�`�ł���

enum class Tag : unsigned char
{
	Begin,               // Tag�̊J�n�}�[�J�[�@�iBegin����O�ɒ�`���Ă͂Ȃ�Ȃ��j

	Player = Begin,
	PlayerBullet,
	Enemy,
	EnemyBullet,
	NPC,
	BackGround,
	Camera,
	Effect,

	End                  // Tag�̏I���}�[�J�[�@�iEnd����ɒ�`���Ă͂Ȃ�Ȃ��j
};

// �O�u�C���N�������g
Tag& operator ++(Tag& rhs);


