#include "BossEnemy.h"
#include "Player.h"
#include "NPCActorBase.h"
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
#include "EnemyAttack.h"

#include <iostream>

const int BossEnemy::m_power = 10;

BossEnemy::BossEnemy(Player* player)
{
	// パラメーター初期化
	mScale = Vector3(3.0f,3.0f,3.0f);
	mWalkSpeed = 500.0f;
	mRunSpeed = 500.0f;
	mTurnSpeed = Math::Pi;
	mHitPoint = 100;
	mIsOnGround = true;

	// モデル読み込み
	mSkelMeshComponent = new SkeletalMeshComponent(this);
	mMesh = RENDERER->GetMesh("Assets/Mesh/Enemy_Bear.gpmesh");

	// スケルトン読み込み
	mSkelMeshComponent->SetMesh(mMesh);
	mSkelMeshComponent->SetSkeleton(RENDERER->GetSkeleton("Assets/Skelton/Enemy_Bear.gpskel"));

	// アニメーション読み込み
	mAnimations.emplace(EnemyStateEnum::Idle, RENDERER->GetAnimation("Assets/Animation/ExoGame_Bears_Idle.gpanim", true));
	mAnimations.emplace(EnemyStateEnum::Walk, RENDERER->GetAnimation("Assets/Animation/ExoGame_Bears_Walk.gpanim", true));
	mAnimations.emplace(EnemyStateEnum::Run, RENDERER->GetAnimation("Assets/Animation/ExoGame_Bears_Walk.gpanim", true));
	mAnimations.emplace(EnemyStateEnum::Attack1, RENDERER->GetAnimation("Assets/Animation/ExoGame_Bears_Attack_Melee.gpanim", false));
	mAnimations.emplace(EnemyStateEnum::Die, RENDERER->GetAnimation("Assets/Animation/ExoGame_Bears_Attack_Death.gpanim", false));

	// EnemyBehaviorにふるまいを登録
	m_enemyBehaviorComponent = new EnemyBehaviorComponent(this);
	m_enemyBehaviorComponent->RegisterState(new EnemyPatrol(m_enemyBehaviorComponent));
	m_enemyBehaviorComponent->RegisterState(new EnemyLookAround(m_enemyBehaviorComponent));
	m_enemyBehaviorComponent->RegisterState(new EnemyChase(m_enemyBehaviorComponent,player));
	m_enemyBehaviorComponent->RegisterState(new EnemyAttack(m_enemyBehaviorComponent));
	m_enemyBehaviorComponent->SetFirstState(EnemyStateEnum::Run);

	// 敵キャラの当たり判定を追加
	m_enemyBox = mMesh->GetCollisionBox();
	m_enemyBox.mMin.y *= 0.5f;
	m_enemyBox.mMax.y *= 0.5f;
	mHitBox = new BoxCollider(this, EnumPhysicsType::EnumEnemy);
	mHitBox->SetObjectBox(m_enemyBox);
	mHitBox->SetArrowRotate(true);
}

BossEnemy::~BossEnemy()
{
	std::cout << "ボス敵破棄" << std::endl;
}

void BossEnemy::UpdateActor(float _deltaTime)
{
	// 前方方向に何かいたら
	if (IsHitTrigger(EnemyTriggerEnum::ForwardBox))
	{
		std::cout << "ForwardBoxHit!!" << std::endl;
	}
}

void BossEnemy::OnCollision(BoxCollider* hitThisBox, BoxCollider* hitOtherBox)
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

	// NPCとの当たり判定
	if (mHitBox == hitThisBox &&
		hitOtherBox->GetType() == EnumPhysicsType::EnumNPC)
	{
		m_enemyBehaviorComponent->ChangeState(EnemyStateEnum::Attack1);
	}

	if (hitThisBox->GetType()==EnumPhysicsType::EnumEnemyAttack&&
		hitOtherBox->GetType()==EnumPhysicsType::EnumNPC)
	{
		
	}
}

void BossEnemy::FixCollision(BoxCollider* hitEnemyBox, BoxCollider* hitPlayerBox)
{
	// 直したときの位置
	Vector3 fix;

	// 壁とぶつかったとき
	AABB playerBox = hitPlayerBox->GetWorldBox();
	AABB enemyBox = mHitBox->GetWorldBox();

	// めり込みを修正
	calcCollisionFixVec(playerBox, enemyBox, fix);

	// 補正ベクトル分戻す
	mPosition += fix;

	// 位置が変わったのでボックス再計算
	mHitBox->OnUpdateWorldTransform();
}

void BossEnemy::SetAttackHitBox(float scale)
{
	m_attackBox = new BoxCollider(this, EnumPhysicsType::EnumEnemyAttack);

	// 敵前方方向の当たり判定
	AABB enemyForward;
	enemyForward.mMin.x = m_enemyBox.mMax.x;
	enemyForward.mMin.y = m_enemyBox.mMin.y;
	enemyForward.mMin.z = m_enemyBox.mMin.z;
	enemyForward.mMax.x = enemyForward.mMin.x + 100.0f;
	enemyForward.mMax.y = enemyForward.mMin.y + 100.0f;
	enemyForward.mMax.z = enemyForward.mMin.z + 100.0f;
	m_attackBox->SetObjectBox(enemyForward);
}

void BossEnemy::RemoveAttackHitBox()
{
	if (m_attackBox)
	{
		delete m_attackBox;
		m_attackBox = nullptr;
	}
}

bool BossEnemy::IsFrontHit()
{
	return m_attackBox->IsTrigerHit();
}
