#include "Attacker.h"
#include "Game.h"
#include "Renderer.h"
#include "Mesh.h"
#include "Skeleton.h"
#include "SkeletalMeshComponent.h"
#include "AttachMeshComponent.h"
#include "Collision.h"
#include "BoxCollider.h"
#include "NPCBehaviorComponent.h"
#include "NPCRun.h"
#include "NPCPatrol.h"
#include "NPCLookAround.h"
#include "NPCAttack.h"
#include "Input.h"
#include "EffectComponent.h"
#include "NPCDie.h"

#include <iostream>

const int Attacker::m_maxHP = 100;
const float Attacker::m_maxSpeed = 500.0f;

Attacker::Attacker(Player* player,EnemyBase* enemy)
	: mNPCBehaviorComponent(nullptr)
	, mFrontTriggerBox(nullptr)
	, mHitBox(nullptr)
{
	printf("Create : [Actor] Attacker 0x%p\n", this);

	// ‰Šúƒpƒ‰ƒ[ƒ^Ý’è
	mWalkSpeed = 100.0f;
	mRunSpeed = m_maxSpeed;
	mTurnSpeed = Math::Pi;
	mHitPoint = m_maxHP;
	mIsOnGround = true;
	mScale = Vector3(0.01f,0.01f,0.01f);

	// ƒ‚ƒfƒ‹“Ç‚Ýž‚Ý
	mSkelMeshComponent = new SkeletalMeshComponent(this);
	Mesh* mesh = RENDERER->GetMesh("Assets/Mesh/Attacker1.gpmesh");
	mSkelMeshComponent->SetMesh(mesh);
	mSkelMeshComponent->SetSkeleton(RENDERER->GetSkeleton("Assets/Skelton/Attacker1.gpskel"));

	// ƒAƒjƒ[ƒVƒ‡ƒ““Ç‚Ýž‚Ý
	mAnimations.emplace(NPCStateEnum::Idle, RENDERER->GetAnimation("Assets/Animation/NPCIdle.gpanim", true));
	mAnimations.emplace(NPCStateEnum::Walk, RENDERER->GetAnimation("Assets/Animation/Attacker_Walking.gpanim", true));
	mAnimations.emplace(NPCStateEnum::Run, RENDERER->GetAnimation("Assets/Animation/NPCRun.gpanim", true));
	mAnimations.emplace(NPCStateEnum::Attack1, RENDERER->GetAnimation("Assets/Animation/NPCDeath.gpanim", false));
	mAnimations.emplace(NPCStateEnum::Die, RENDERER->GetAnimation("Assets/Animation/NPCDeath.gpanim", false));

	//// EnemyBehaviorComponent ‚É ‚Ó‚é‚Ü‚¢‚ð’Ç‰Á
	mNPCBehaviorComponent = new NPCBehaviorComponent(this);
	mNPCBehaviorComponent->RegisterState(new NPCPatrol(mNPCBehaviorComponent));
	mNPCBehaviorComponent->RegisterState(new NPCLookAround(mNPCBehaviorComponent));
	mNPCBehaviorComponent->RegisterState(new NPCRun(mNPCBehaviorComponent,player,enemy));
	mNPCBehaviorComponent->RegisterState(new NPCAttack(mNPCBehaviorComponent, enemy));
	mNPCBehaviorComponent->RegisterState(new NPCDie(mNPCBehaviorComponent));
	mNPCBehaviorComponent->SetFirstState(NPCStateEnum::Die);

	// NPC‚Ì“–‚½‚è”»’è‚ð’Ç‰Á
	AABB npcBox = mesh->GetCollisionBox();
	npcBox.mMin.x *= 0.5f;
	npcBox.mMax.x *= 0.5f;
	mHitBox = new BoxCollider(this, EnumPhysicsType::EnumNPC);
	mHitBox->SetObjectBox(npcBox);
	mHitBox->SetArrowRotate(false);

	// NPC‚Ì‘O•ûƒgƒŠƒK[‚ð’Ç‰Á
	AABB npcForward;
	npcForward.mMin.x = npcBox.mMax.x;
	npcForward.mMin.y = npcBox.mMin.y;
	npcForward.mMin.z = npcBox.mMin.z + 100;
	npcForward.mMax.x = npcForward.mMin.x + 100.0f;
	npcForward.mMax.y = npcForward.mMin.y + 100.0f;
	npcForward.mMax.z = npcForward.mMin.z + 100.0f;
	SetTriggerBox(NPCTriggerEnum::ForwardBox, npcForward);

	//EffectComponent* ec = new EffectComponent(this, true, false);
	//ec->LoadEffect(u"assets/Effect/distortion.efk");
	//Vector3 pos(0, 0, 100);
	//ec->SetRelativePosition(pos);
	//Matrix4 rot = Matrix4::CreateRotationZ(Math::ToRadians(0.0f));
	//ec->SetRelativeRotate(rot);
}

Attacker::~Attacker()
{
	printf("Remove : [Actor] BlackKnight 0x%p\n", this);
}

void Attacker::UpdateActor(float deltaTime)
{
	// ‘Ì—Í‚ª‚È‚­‚È‚Á‚½‚ç
	if (mHitPoint <= 0)
	{
		this->EDead;
	}
}

void Attacker::OnCollision(BoxCollider* hitThisBox, BoxCollider* hitOtherBox)
{
	// “–‚½‚è”»’è‚Å‹A‚Á‚Ä‚«‚½Œ‹‰Ê‚ªmHitBoxA”wŒi‚Æ‚ÌÕ“Ë‚¾‚Á‚½ê‡
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

	// “–‚½‚è”»’è‚Å‹A‚Á‚Ä‚«‚½Œ‹‰Ê‚ªmHitBoxA”wŒi‚Æ‚ÌÕ“Ë‚¾‚Á‚½ê‡
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

	// “–‚½‚è”»’è‚Å‹A‚Á‚Ä‚«‚½Œ‹‰Ê‚ªŽ©g‚Æ“G‚Æ‚ÌÕ“Ë‚¾‚Á‚½ê‡
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

	// ƒgƒŠƒK[‚Éƒqƒbƒg‚µ‚½‚Ì‚ª“G‚Ìê‡
	if (mFrontTriggerBox == hitThisBox &&
		hitOtherBox->GetType() == EnumPhysicsType::EnumEnemy)
	{
		mNPCBehaviorComponent->ChangeState(NPCStateEnum::Attack1);
	}

	// ƒqƒbƒg‚µ‚½‚Ì‚ªƒvƒŒƒCƒ„[‚Ìê‡
	if (mHitBox == hitThisBox &&
		hitOtherBox->GetType() == EnumPhysicsType::EnumPlayer)
	{
		mNPCBehaviorComponent->ChangeState(NPCStateEnum::Run);

		AABB playerBox = hitOtherBox->GetWorldBox();
		AABB thisBox = hitThisBox->GetWorldBox();
		Vector3 fixVec;

		calcCollisionFixVec(thisBox, playerBox, fixVec);
		mPosition += fixVec;
		mHitBox->OnUpdateWorldTransform();
	}

	// “G‚ÌUŒ‚‚ªƒqƒbƒg‚µ‚½ê‡
	if (mHitBox == hitThisBox &&
		hitOtherBox->GetType() == EnumPhysicsType::EnumEnemyAttackBox)
	{
		mHitPoint -= 100;
	}
}

void Attacker::GetDamage(const int& power)
{
	// ‘Ì—Í‚ðŒ¸‚ç‚·
	mHitPoint -= power;
}

bool Attacker::IsFrontHit()
{
	return mFrontTriggerBox->IsTrigerHit();
}
