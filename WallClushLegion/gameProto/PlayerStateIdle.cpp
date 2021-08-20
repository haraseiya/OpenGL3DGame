#include "PlayerStateIdle.h"
#include "Input.h"
#include "InputController.h"
#include "SkeletalMeshComponent.h"

PlayerStateIdle::PlayerStateIdle()
{
}

PlayerStateIdle::~PlayerStateIdle()
{
}

// �A�C�h����Ԃ��瑼�̏�Ԃւ̈ڍs
PlayerState PlayerStateIdle::Update(Player* owner, float deltaTime)
{
	// �X�e�B�b�N���͂���������
	mIsControllerInputOff = !(INPUT_INSTANCE.IsLStickMove());

	// �����L�[�����͂��ꂽ��
	mIsIdle = INPUT_INSTANCE.IsKeyOff(KEY_UP) &
		INPUT_INSTANCE.IsKeyOff(KEY_RIGHT) &
		INPUT_INSTANCE.IsKeyOff(KEY_DOWN) &
		INPUT_INSTANCE.IsKeyOff(KEY_LEFT) &
		mIsControllerInputOff;

	// �J��������݂��O�i�������擾
	Vector3 TargetPos = GAMEINSTANCE.GetViewTarget();
	Vector3 ViewPos = GAMEINSTANCE.GetViewPos();
	Vector3 forwardVec = TargetPos - ViewPos;
	forwardVec.z = 0.0; // ���������𖳎�

	// �J�����O���x�N�g���ƉE�����x�N�g���Z�o
	forwardVec = Vector3::Normalize(forwardVec);
	Vector3 rightVec = Vector3::Cross(Vector3::UnitZ, forwardVec);

	float forwardAngle = 0.0f;
	forwardAngle = acosf(Vector3::Dot(Vector3::UnitX, rightVec));
	Vector2 stickR = INPUT_INSTANCE.GetRStick();
	Matrix3 rot = Matrix3::CreateRotation(forwardAngle);
	stickR = Vector2::Transform(stickR, rot);

	Vector3 DirVecR(0.0f, 0.0f, 0.0f);
	DirVecR.x += stickR.x;
	DirVecR.y += stickR.y;

	Vector3 charaForwardVec = owner->GetForward(); // �L�����̑O�i�x�N�g��

	if (DirVecR.LengthSq() > 0.5f)
	{
		// �����L�[����
		charaForwardVec = DirVecR;

		// �i�s�����Ɍ����ĉ�]
		charaForwardVec.Normalize();
		owner->RotateToNewForward(charaForwardVec);
	}

	// �A�C�h����Ԃł͂Ȃ��ꍇ
	if (!mIsIdle)
	{
		return PlayerState::PLAYER_STATE_RUN;
	}

	return PlayerState::PLAYER_STATE_IDLE;
}

// �A�C�h����Ԃւ̈ڍs����
void PlayerStateIdle::Enter(class Player* owner, float deltaTime)
{
	// �A�C�h����Ԃ̃A�j���[�V�����Đ�
	mMeshComp = owner->GetSkeletalMeshComp();
	mMeshComp->PlayAnimation(owner->GetAnim(PlayerState::PLAYER_STATE_IDLE));
}
