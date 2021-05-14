#include "WeakEnemy.h"
#include "Player.h"
#include "Mesh.h"
#include "Renderer.h"
#include "Skeleton.h"
#include "SkeletalMeshComponent.h"
#include "Texture.h"
#include "BoxCollider.h"
#include "Collision.h"
#include "Animation.h"
#include "MeshComponent.h"
#include "SpriteComponent.h"
#include "PhysicsWorld.h"
#include "EnemyBehaviorComponent.h"
#include "EnemyPatrol.h"
#include "EnemyLookAround.h"
#include "EnemyChase.h"

#include <iostream>

WeakEnemy::WeakEnemy(Player* player)
{
	// �p�����[�^�[������
	mWalkSpeed = 100.0f;
	mRunSpeed = 200.0f;
	mTurnSpeed = Math::Pi;
	mHitPoint = 100;
	mIsOnGround = true;

	// ���f���ǂݍ���
	mSkelMeshComponent = new SkeletalMeshComponent(this);
	mMesh = RENDERER->GetMesh("Assets/Mesh/BeholderSK.gpmesh");
	mSkelMeshComponent->SetMesh(mMesh);
	mSkelMeshComponent->SetSkeleton(RENDERER->GetSkeleton("Assets/Skelton/BeholderSK.gpskel"));

	// �A�j���[�V�����ǂݍ���
	mAnimations.emplace(EnemyStateEnum::Idle, RENDERER->GetAnimation("Assets/Animation/Beholder_IdleNormalAnim.gpanim",true));
	mAnimations.emplace(EnemyStateEnum::Walk, RENDERER->GetAnimation("Assets/Animation/Beholder_WalkFWDAnim.gpanim", true));
	mAnimations.emplace(EnemyStateEnum::Run, RENDERER->GetAnimation("Assets/Animation/Beholder_WalkFWDAnim.gpanim", true));
	mAnimations.emplace(EnemyStateEnum::Attack1, RENDERER->GetAnimation("Assets/Animation/Beholder_Attack03Anim.gpanim", false));
	mAnimations.emplace(EnemyStateEnum::Die, RENDERER->GetAnimation("Assets/Animation/Beholder_DieAnim.gpanim", false));

	// EnemyBehavior�ɂӂ�܂���o�^
	NPCBehaviorComponent* ebc = new NPCBehaviorComponent(this);
	ebc->RegisterState(new EnemyPatrol(ebc));
	ebc->RegisterState(new EnemyLookAround(ebc));
	ebc->RegisterState(new EnemyChase(ebc,player));
	ebc->SetFirstState(EnemyStateEnum::Run);

	// �G�L�����̓����蔻���ǉ�
	AABB enemyBox = mMesh->GetCollisionBox();
	enemyBox.mMin.x *= 0.5f;
	enemyBox.mMax.x *= 0.5f;
	mHitBox = new BoxCollider(this, EnumPhysicsType::EnumEnemy);
	mHitBox->SetObjectBox(enemyBox);
	mHitBox->SetArrowRotate(false);

	// �G�l�~�[�O�������̓����蔻��
	AABB enemyForward;
	enemyForward.mMin.x = enemyBox.mMax.x;
	enemyForward.mMin.y = enemyBox.mMin.y;
	enemyForward.mMin.z = enemyBox.mMin.z + 100;
	enemyForward.mMax.x = enemyForward.mMin.x + 100.0f;
	enemyForward.mMax.y = enemyForward.mMin.y + 100.0f;
	enemyForward.mMax.z = enemyForward.mMin.z + 100.0f;
	SetTriggerBox(EnemyTriggerEnum::ForwardBox, enemyForward);
}

WeakEnemy::~WeakEnemy()
{
	std::cout << "�G���G�j��" << std::endl;
}

void WeakEnemy::UpdateActor(float _deltaTime)
{
	if (IsHitTrigger(EnemyTriggerEnum::ForwardBox))
	{
		std::cout << "ForwardBoxHit!!" << std::endl;
	}
}

void WeakEnemy::OnCollision(BoxCollider* hitThisBox, BoxCollider* hitOtherBox)
{
	// �����蔻��ŋA���Ă������ʂ�mHitBox�A�w�i�Ƃ̏Փ˂������ꍇ
	if (mHitBox == hitThisBox &&
		hitOtherBox->GetType() == EnumPhysicsType::EnumBG)
	{
		AABB bgBox = hitOtherBox->GetWorldBox();
		AABB thisBox = hitThisBox->GetWorldBox();
		Vector3 fixVec;

		calcCollisionFixVec(thisBox, bgBox, fixVec);
		mPosition += fixVec;
		mHitBox->OnUpdateWorldTransform();
	}
}

// �w�iAABB�Ƃ̃q�b�g�߂荞�݉��� ( ���������ۂ�PhysicsWorld����Ă΂�� �j
void WeakEnemy::FixCollision(BoxCollider* hitPlayerBox, BoxCollider* hitBox)
{
	Vector3 fix;

	// �ǂƂԂ������Ƃ�
	AABB bgBox = hitBox->GetWorldBox();
	AABB enemyBox = mHitBox->GetWorldBox();

	// �߂荞�݂��C��
	calcCollisionFixVec(enemyBox, bgBox, fix);

	// �␳�x�N�g�����߂�
	mPosition += fix;

	// �ʒu���ς�����̂Ń{�b�N�X�Čv�Z
	mHitBox->OnUpdateWorldTransform();
}

bool WeakEnemy::IsFrontHit()
{
	return mFrontTriggerBox->IsTrigerHit();
}
