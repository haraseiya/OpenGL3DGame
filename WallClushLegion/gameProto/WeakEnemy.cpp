#include "WeakEnemy.h"
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

WeakEnemy::WeakEnemy(GameObject* target)
	: mCoolTime(0.0f)
	, mTarget(target)
{
	// パラメーター初期化
	mScale = 0.5f;
	mWalkSpeed = 500.0f;
	mRunSpeed = 500.0f;
	mTurnSpeed = Math::Pi;
	mHitPoint = 3;
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

WeakEnemy::~WeakEnemy()
{
	std::cout << "ボス敵破棄" << std::endl;
}

void WeakEnemy::UpdateActor(float _deltaTime)
{
	// 前方方向に何かいたら
	//if (IsHitTrigger(EnemyTriggerEnum::ForwardBox))
	//{
	//	std::cout << "ForwardBoxHit!!" << std::endl;
	//}

	if (mHitPoint <= 0)
	{
		mState=STATE_DEAD;
	}
	mCoolTime += _deltaTime;
}

void WeakEnemy::OnCollisionEnter(ColliderComponent* own,ColliderComponent* other)
{
	Tag colliderTag = other->GetTag();

	// 当たり判定で帰ってきた結果がmHitBox、背景との衝突だった場合
	if (colliderTag == Tag::BackGround)
	{
		Vector3 fix;

		//壁とぶつかったとき
		AABB enemyBox = mBoxCollider->GetWorldBox();
		AABB bgBox = dynamic_cast<BoxCollider*>(other)->GetWorldBox();

		// めり込みを修正
		calcCollisionFixVec(enemyBox, bgBox, fix);
	}

	// 衝突情報
	CollisionInfo info;

	// 敵と衝突したら
	if (colliderTag==Tag::Enemy)
	{
		Vector3 fix;

		//壁とぶつかったとき
		AABB otherEnemyBox = dynamic_cast<BoxCollider*>(other)->GetWorldBox();
		AABB enemyBox = mHitBox->GetWorldBox();

		// めり込みを修正
		calcCollisionFixVec(enemyBox, otherEnemyBox, fix);

		// 補正ベクトル分戻す
		mPosition += fix;

		// 位置再計算
		ComputeWorldTransform();
	}

	// プレイヤー弾と衝突したら
	if (colliderTag == Tag::PlayerBullet)
	{
		mSkelMeshComponent->SetHitColor(Color::Red);
		mHitPoint--;
	}

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

	//if (colliderTag == Tag::Enemy)
	//{
	//	Vector3 fix;

	//	//壁とぶつかったとき
	//	AABB enemyBox = mEnemyBox;
	//	AABB otherEnemyBox = dynamic_cast<BoxCollider*>(other)->GetWorldBox();

	//	// めり込みを修正
	//	calcCollisionFixVec(enemyBox, otherEnemyBox, fix);

	//	// 補正ベクトル分戻す
	//	mPosition += fix;

	//	// 位置再計算
	//	ComputeWorldTransform();
	//}
}

void WeakEnemy::FixCollision(BoxCollider* enemy, BoxCollider* player)
{
	//// 直したときの位置
	//Vector3 fix;

	//// 壁とぶつかったとき
	//AABB playerBox = hitPlayerBox->GetWorldBox();
	//AABB enemyBox = mHitBox->GetWorldBox();

	//// めり込みを修正
	//calcCollisionFixVec(playerBox, enemyBox, fix);

	//// 補正ベクトル分戻す
	//mPosition += fix;

	//// 位置が変わったのでボックス再計算
	//mHitBox->OnUpdateWorldTransform();
}

void WeakEnemy::SetAttackHitBox(float scale)
{
	// 攻撃判定用ボックスの生成
	mAttackBox = new BoxCollider(this);

	// 敵前方方向の当たり判定
	AABB box = mEnemyBox;
	box.mMin *= 1.5;
	box.mMax *= 1.5;
	mAttackBox->SetObjectBox(box);
}

void WeakEnemy::RemoveAttackHitBox()
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

void WeakEnemy::LoadModel()
{
	mSkelMeshComponent = new SkeletalMeshComponent(this);
	mMesh = RENDERER->GetMesh("Assets/Mesh/SK_Greater_Spider.gpmesh");
}

void WeakEnemy::LoadSkeleton()
{
	mSkelMeshComponent->SetMesh(mMesh);
	mSkelMeshComponent->SetSkeleton(RENDERER->GetSkeleton("Assets/Mesh/SK_Greater_Spider.gpskel"));
}

void WeakEnemy::LoadAnimation()
{
	//mAnimations.emplace(EnemyStateEnum::Idle, RENDERER->GetAnimation("Assets/Animation/ExoGame_Bears_Idle.gpanim", true));
	mAnimations.emplace(EnemyStateEnum::Walk, RENDERER->GetAnimation("Assets/Animation/Greater_Spider_Walk.gpanim", true));
	mAnimations.emplace(EnemyStateEnum::Run, RENDERER->GetAnimation("Assets/Animation/Greater_Spider_Walk.gpanim", true));
	//mAnimations.emplace(EnemyStateEnum::Attack1, RENDERER->GetAnimation("Assets/Animation/ExoGame_Bears_Attack_Melee.gpanim", false));
	//mAnimations.emplace(EnemyStateEnum::Die, RENDERER->GetAnimation("Assets/Animation/ExoGame_Bears_Attack_Death.gpanim", false));
}

void WeakEnemy::BehaviorResister()
{
	m_enemyBehaviorComponent = new EnemyBehaviorComponent(this);
	m_enemyBehaviorComponent->RegisterState(new EnemyIdle(m_enemyBehaviorComponent, mTarget));
	m_enemyBehaviorComponent->RegisterState(new EnemyPatrol(m_enemyBehaviorComponent));
	m_enemyBehaviorComponent->RegisterState(new EnemyLookAround(m_enemyBehaviorComponent));
	m_enemyBehaviorComponent->RegisterState(new EnemyChase(m_enemyBehaviorComponent, mTarget));
	m_enemyBehaviorComponent->RegisterState(new EnemyAttack(m_enemyBehaviorComponent));
	m_enemyBehaviorComponent->SetFirstState(EnemyStateEnum::Idle);
}

void WeakEnemy::SetCollider()
{
	mEnemyBox = mMesh->GetCollisionBox();
	mEnemyBox.mMin.y *= 0.5f;
	mEnemyBox.mMax.y *= 0.5f;
	mHitBox = new BoxCollider(this);
	mHitBox->SetObjectBox(mEnemyBox);
	mHitBox->SetArrowRotate(true);

	AABB box = mMesh->GetCollisionBox();
	mBoxCollider = new BoxCollider(this);
	mBoxCollider->SetObjectBox(box);
}

void WeakEnemy::SetAttackTrigger()
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
