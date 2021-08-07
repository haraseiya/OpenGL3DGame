#include "StrongEnemy.h"
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
#include "EnemyIdle.h"
#include "EnemyPatrol.h"
#include "EnemyLookAround.h"
#include "EnemyChase.h"
#include "EnemyAttack.h"

#include <iostream>

StrongEnemy::StrongEnemy(GameObject* target)
	: mCoolTime(0.0f)
	, mTarget(target)
{
	// パラメーター初期化
	mScale = 1.0f;
	mWalkSpeed = 500.0f;
	mRunSpeed = 500.0f;
	mTurnSpeed = Math::Pi;
	mHitPoint = 100;
	mIsOnGround = true;

	// モデル読み込み
	LoadModel();

	// スケルトン読み込み
	LoadSkeleton();

	// アニメーション読み込み
	LoadAnimation();

	// EnemyBehaviorにふるまいを登録
	BehaviorResister();

	// 当たり判定を追加
	SetCollider();

	// 攻撃用トリガー追加
	SetAttackTrigger();
}

StrongEnemy::~StrongEnemy()
{
	std::cout << "ボス敵破棄" << std::endl;
}

void StrongEnemy::UpdateActor(float _deltaTime)
{
	// 前方方向に何かいたら
	//if (IsHitTrigger(EnemyTriggerEnum::ForwardBox))
	//{
	//	std::cout << "ForwardBoxHit!!" << std::endl;
	//}

	if (mHitPoint <= 0)
	{
		this->STATE_DEAD;
	}
	mCoolTime += _deltaTime;
}

void StrongEnemy::OnCollisionEnter(ColliderComponent* own,ColliderComponent* other)
{
	// 当たり判定で帰ってきた結果がmHitBox、背景との衝突だった場合
	//if (other->GetTag()==Tag::BackGround)
	//{
	//	AABB bgBox = hitOtherBox->GetWorldBox();
	//	AABB thisBox = hitThisBox->GetWorldBox();
	//	Vector3 fixVec;

	//	calcCollisionFixVec(thisBox, bgBox, fixVec);
	//	mPosition += fixVec;
	//	mHitBox->OnUpdateWorldTransform();
	//}

	//// アタックトリガーにヒットしたら
	//if (other->GetTag() == Tag::NPC)
	//{
	//	if (mCoolTime > 3.0f)
	//	{
	//		mCoolTime = 0.0f;
	//		// 攻撃アニメーションにステートチェンジ
	//		m_enemyBehaviorComponent->ChangeState(EnemyStateEnum::Attack1);
	//	}
	//}

	//if (other->GetTag()==Tag::NPC)
	//{
	//	mHitPoint -= 10;
	//}
}

void StrongEnemy::FixCollision(BoxCollider* hitEnemyBox, BoxCollider* hitPlayerBox)
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

void StrongEnemy::SetAttackHitBox(float scale)
{
	// 攻撃判定用ボックスの生成
	mAttackBox = new BoxCollider(this);

	// 敵前方方向の当たり判定
	AABB box = mEnemyBox;
	box.mMin *= 1.5;
	box.mMax *= 1.5;
	mAttackBox->SetObjectBox(box);
}

void StrongEnemy::RemoveAttackHitBox()
{
	if (mAttackBox)
	{
		delete mAttackBox;
		mAttackBox = nullptr;
	}
}

//bool BossEnemy::IsFrontHit()
//{
//	//return mAttackBox->IsTrigerHit();
//}

void StrongEnemy::LoadModel()
{
	mSkelMeshComponent = new SkeletalMeshComponent(this);
	mMesh = RENDERER->GetMesh("Assets/Mesh/SK_Greater_Spider_Boss.gpmesh");
}

void StrongEnemy::LoadSkeleton()
{
	mSkelMeshComponent->SetMesh(mMesh);
	mSkelMeshComponent->SetSkeleton(RENDERER->GetSkeleton("Assets/Mesh/SK_Greater_Spider_Boss.gpskel"));
}

void StrongEnemy::LoadAnimation()
{
	//mAnimations.emplace(EnemyStateEnum::Idle, RENDERER->GetAnimation("Assets/Animation/ExoGame_Bears_Idle.gpanim", true));
	mAnimations.emplace(EnemyStateEnum::Walk, RENDERER->GetAnimation("Assets/Animation/Greater_Spider_Walk.gpanim", true));
	mAnimations.emplace(EnemyStateEnum::Run, RENDERER->GetAnimation("Assets/Animation/Greater_Spider_Walk.gpanim", true));
	//mAnimations.emplace(EnemyStateEnum::Attack1, RENDERER->GetAnimation("Assets/Animation/ExoGame_Bears_Attack_Melee.gpanim", false));
	//mAnimations.emplace(EnemyStateEnum::Die, RENDERER->GetAnimation("Assets/Animation/ExoGame_Bears_Attack_Death.gpanim", false));
}

void StrongEnemy::BehaviorResister()
{
	m_enemyBehaviorComponent = new EnemyBehaviorComponent(this);
	m_enemyBehaviorComponent->RegisterState(new EnemyIdle(m_enemyBehaviorComponent, mTarget));
	m_enemyBehaviorComponent->RegisterState(new EnemyPatrol(m_enemyBehaviorComponent));
	m_enemyBehaviorComponent->RegisterState(new EnemyLookAround(m_enemyBehaviorComponent));
	m_enemyBehaviorComponent->RegisterState(new EnemyChase(m_enemyBehaviorComponent, mTarget));
	m_enemyBehaviorComponent->RegisterState(new EnemyAttack(m_enemyBehaviorComponent));
	m_enemyBehaviorComponent->SetFirstState(EnemyStateEnum::Idle);
}

void StrongEnemy::SetCollider()
{
	mEnemyBox = mMesh->GetCollisionBox();
	mEnemyBox.mMin.y *= 0.5f;
	mEnemyBox.mMax.y *= 0.5f;
	mHitBox = new BoxCollider(this);
	mHitBox->SetObjectBox(mEnemyBox);
	mHitBox->SetArrowRotate(true);
}

void StrongEnemy::SetAttackTrigger()
{
	mEnemyForward.mMin.x = mEnemyBox.mMax.x;
	mEnemyForward.mMin.y = mEnemyBox.mMin.y;
	mEnemyForward.mMin.z = mEnemyBox.mMin.z;
	mEnemyForward.mMax.x = mEnemyForward.mMin.x + 100.0f;
	mEnemyForward.mMax.y = mEnemyForward.mMin.y + 100.0f;
	mEnemyForward.mMax.z = mEnemyForward.mMin.z + 100.0f;
	mAttackTrigger = new BoxCollider(this);
	mAttackTrigger->SetObjectBox(mEnemyForward);
}
