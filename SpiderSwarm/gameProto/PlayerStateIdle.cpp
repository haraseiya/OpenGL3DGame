#include "PlayerStateIdle.h"
#include "PlayerBase.h"
#include "Input.h"
#include "InputController.h"
#include "SkeletalMeshComponent.h"

PlayerStateIdle::PlayerStateIdle(PlayerBehaviorComponent* owner)
	: PlayerStateBase(owner)
{
	mStateType = PlayerStateEnum::Idle;
}

PlayerStateIdle::~PlayerStateIdle()
{
}

// �A�C�h����Ԃ��瑼�̏�Ԃւ̈ڍs
PlayerStateEnum PlayerStateIdle::Update(float deltaTime)
{
	const bool isDead = mOwner->GetHitPoint() <= 0;
	if (isDead)
	{
		return PlayerStateEnum::Die;
	}

	// �X�e�B�b�N���͂���������
	mIsControllerInputOff = !(INPUT_INSTANCE.IsLStickMove());
	INPUT_INSTANCE.GetLStick();

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

	Vector3 charaForwardVec = mOwner->GetForward(); // �L�����̑O�i�x�N�g��

	if (DirVecR.LengthSq() > 0.5f)
	{
		// �i�s�����Ɍ����ĉ�]
		charaForwardVec = Vector3::Lerp(mOwner->GetForward(), DirVecR, 0.2f);
		charaForwardVec.Normalize();
		mOwner->RotateToNewForward(charaForwardVec);
	}

	// �A�C�h����Ԃł͂Ȃ��ꍇ
	if (!mIsIdle)
	{
		return PlayerStateEnum::Forward;
	}

	return PlayerStateEnum::Idle;
}

// �A�C�h����Ԃւ̈ڍs����
void PlayerStateIdle::OnEnter()
{
	// �A�C�h����Ԃ̃A�j���[�V�����Đ�
	mMeshComp = mOwner->GetSkeletalMeshComp();
	mMeshComp->PlayAnimation(mOwner->GetAnim(PlayerState::PLAYER_STATE_IDLE));
}

void PlayerStateIdle::OnExit()
{
}
