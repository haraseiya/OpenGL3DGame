#include "Attacker.h"
#include "Game.h"
#include "Renderer.h"
#include "Mesh.h"
#include "Skeleton.h"
#include "SkeletalMeshComponent.h"
#include "AttachMeshComponent.h"
#include "Collision.h"
#include "BoxCollider.h"
#include "Input.h"
#include "EffectComponent.h"

#include "NPCBehaviorComponent.h"
#include "NPCIdle.h"
#include "NPCDie.h"
#include "NPCRun.h"
#include "NPCPatrol.h"
#include "NPCLookAround.h"
#include "NPCAttack.h"

#include <iostream>

const int Attacker::m_maxHP = 100;
const float Attacker::m_maxSpeed = 500.0f;

Attacker::Attacker(Player* player,EnemyBase* enemy)
	: mNPCBehaviorComponent(nullptr)
	, mAttackTriggerBox(nullptr)
	, mHitBox(nullptr)
	, mCoolTime(0.0f)
{
	printf("Create : [Actor] Attacker 0x%p\n", this);

	// 初期パラメータ設定
	mWalkSpeed = 100.0f;
	mRunSpeed = m_maxSpeed;
	mTurnSpeed = Math::Pi;
	mHitPoint = m_maxHP;
	mIsOnGround = true;
	mScale = Vector3(0.01f,0.01f,0.01f);

	// モデル読み込み
	mSkelMeshComponent = new SkeletalMeshComponent(this);
	Mesh* mesh = RENDERER->GetMesh("Assets/Mesh/Attacker1.gpmesh");
	mSkelMeshComponent->SetMesh(mesh);
	mSkelMeshComponent->SetSkeleton(RENDERER->GetSkeleton("Assets/Skelton/Attacker1.gpskel"));

	// アニメーション読み込み
	mAnimations.emplace(NPCStateEnum::Idle, RENDERER->GetAnimation("Assets/Animation/NPCIdle_Anim.gpanim", true));
	mAnimations.emplace(NPCStateEnum::Walk, RENDERER->GetAnimation("Assets/Animation/Attacker_Walking.gpanim", true));
	mAnimations.emplace(NPCStateEnum::Run, RENDERER->GetAnimation("Assets/Animation/NPCRun.gpanim", true));
	mAnimations.emplace(NPCStateEnum::Attack1, RENDERER->GetAnimation("Assets/Animation/NPCDeath.gpanim", false));
	mAnimations.emplace(NPCStateEnum::Die, RENDERER->GetAnimation("Assets/Animation/NPCDeath.gpanim", false));

	//// EnemyBehaviorComponent に ふるまいを追加
	mNPCBehaviorComponent = new NPCBehaviorComponent(this);
	mNPCBehaviorComponent->RegisterState(new NPCIdle(mNPCBehaviorComponent,player,enemy));
	mNPCBehaviorComponent->RegisterState(new NPCPatrol(mNPCBehaviorComponent));
	mNPCBehaviorComponent->RegisterState(new NPCLookAround(mNPCBehaviorComponent));
	mNPCBehaviorComponent->RegisterState(new NPCRun(mNPCBehaviorComponent,player,enemy));
	mNPCBehaviorComponent->RegisterState(new NPCAttack(mNPCBehaviorComponent, enemy));
	mNPCBehaviorComponent->RegisterState(new NPCDie(mNPCBehaviorComponent));
	mNPCBehaviorComponent->SetFirstState(NPCStateEnum::Idle);

	// NPCの当たり判定を追加
	AABB npcBox = mesh->GetCollisionBox();
	npcBox.mMin.x *= 0.5f;
	npcBox.mMax.x *= 0.5f;
	mHitBox = new BoxCollider(this, EnumPhysicsType::EnumNPC);
	mHitBox->SetObjectBox(npcBox);
	mHitBox->SetArrowRotate(false);

	// NPCの前方トリガーを追加
	AABB npcForward;
	npcForward.mMin.x = npcBox.mMax.x;
	npcForward.mMin.y = npcBox.mMin.y;
	npcForward.mMin.z = npcBox.mMin.z + 100;
	npcForward.mMax.x = npcForward.mMin.x + 100.0f;
	npcForward.mMax.y = npcForward.mMin.y + 100.0f;
	npcForward.mMax.z = npcForward.mMin.z + 100.0f;
	mAttackTriggerBox = new BoxCollider(this, EnumPhysicsType::EnumEnemyAttackTrigger);
	mAttackTriggerBox->SetObjectBox(npcForward);
}

Attacker::~Attacker()
{
	printf("Remove : [Actor] BlackKnight 0x%p\n", this);
}

void Attacker::UpdateActor(float deltaTime)
{
	// 体力がなくなったら
	if (mHitPoint <= 0)
	{
		printf("NPC死亡");
	}

	mCoolTime += deltaTime;
}

void Attacker::OnCollision(BoxCollider* hitThisBox, BoxCollider* hitOtherBox)
{
	// 当たり判定で帰ってきた結果がmHitBox、背景との衝突だった場合
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

	// 別のNPCとの当たり判定
	if (mHitBox == hitThisBox &&
		hitOtherBox->GetType() == EnumPhysicsType::EnumNPC)
	{
		AABB npcBox = hitOtherBox->GetWorldBox();
		AABB thisBox = hitThisBox->GetWorldBox();
		Vector3 fixVec;

		calcCollisionFixVec(thisBox, npcBox, fixVec);
		mPosition += fixVec;
		mHitBox->OnUpdateWorldTransform();
	}

	// 当たり判定で帰ってきた結果が自身と敵との衝突だった場合
	if (mHitBox == hitThisBox &&
		hitOtherBox->GetType() == EnumPhysicsType::EnumEnemy)
	{
		AABB enemyBox = hitOtherBox->GetWorldBox();
		AABB thisBox = hitThisBox->GetWorldBox();
		Vector3 fixVec;

		calcCollisionFixVec(thisBox, enemyBox, fixVec);
		mPosition += fixVec;
		mHitBox->OnUpdateWorldTransform();
	}

	// ヒットしたのがプレイヤーの場合
	if (mHitBox == hitThisBox &&
		hitOtherBox->GetType() == EnumPhysicsType::EnumPlayer)
	{
		AABB playerBox = hitOtherBox->GetWorldBox();
		AABB thisBox = hitThisBox->GetWorldBox();
		Vector3 fixVec;

		calcCollisionFixVec(thisBox, playerBox, fixVec);
		mPosition += fixVec;
		mHitBox->OnUpdateWorldTransform();
	}

	// NPCとプレイヤートリガーがヒットした場合
	if (mHitBox == hitThisBox &&
		hitOtherBox->GetType() == EnumPhysicsType::EnumPlayerTrigger)
	{
		// トリガーに3秒入っていたら
		if (mCoolTime >= 3.0f)
		{
			printf("NPC復活\n");
			mNPCBehaviorComponent->ChangeState(NPCStateEnum::Run);
			mCoolTime = 0.0f;
		}
	}

	// 敵の攻撃がヒットした場合
	if (mHitBox == hitThisBox &&
		hitOtherBox->GetType() == EnumPhysicsType::EnumEnemyAttackBox)
	{
		mHitPoint -= 100;
	}
}

void Attacker::GetDamage(const int& power)
{
	// 体力を減らす
	mHitPoint -= power;
}

bool Attacker::IsFrontHit()
{
	return mAttackTriggerBox->IsTrigerHit();
}
