#include "StrongEnemy.h"
#include "PlayerBase.h"
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
#include "ExplosionEffect.h"
#include "EnemyBullet.h"

// 強敵ステート関連
#include "EnemyBehaviorComponent.h"
#include "EnemyIdle.h"
#include "EnemyChase.h"
#include "EnemyAttack.h"
#include "EnemySpawn.h"
#include "EnemyDeath.h"

#include <iostream>

const float StrongEnemy::mInterval = 5.0f;	// 弾の発射間隔

StrongEnemy::StrongEnemy(GameObject* target)
	: mTarget(target)
{
	// パラメーター初期化
	mScale = 1.0f;
	mHitPoint = 30;
	mWalkSpeed = 500.0f;
	mRunSpeed = 500.0f;
	mTurnSpeed = Math::Pi;
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

void StrongEnemy::UpdateActor(float deltaTime)
{
	// 5秒おきにプレイヤーに向かって弾を発射
	mShootTimer += deltaTime;
	const bool isShot = mShootTimer > mInterval;
	if (isShot)
	{
		mShootTimer = 0.0f;

		Vector3 firePos;
		firePos = mDirection;
		firePos.z = 550.0f;

		// 敵弾のインスタンス生成
		mEnemyBullet = new EnemyBullet(this,2.0f,300.0f);
	}
}

void StrongEnemy::OnCollisionEnter(ColliderComponent* own,ColliderComponent* other)
{
	// 入ってきたタグを取得
	Tag otherTag = other->GetTag();

	// プレイヤーバレットに衝突した場合
	if (otherTag == Tag::PLAYER_BULLET)
	{
		mHitPoint--;
	}

	// プレイヤーに衝突時補完しながら位置を修正
	if (otherTag == Tag::ENEMY || otherTag == Tag::PLAYER)
	{
		// 修正分の位置が入る
		Vector3 fix;

		//壁とぶつかったとき
		AABB otherEnemyBox = dynamic_cast<BoxCollider*>(other)->GetWorldBox();
		AABB enemyBox = mHitBox->GetWorldBox();

		// めり込みを修正
		calcCollisionFixVec(enemyBox, otherEnemyBox, fix);

		// ベクトルを補正しながら戻す
		mPosition = Vector3::Lerp(mPosition, mPosition + fix, 0.1f);
		mPosition.z = 500.0f;

		// 位置再計算
		ComputeWorldTransform();
	}

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

// 攻撃用当たり判定をセット
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

// 攻撃用当たり判定を破棄
void StrongEnemy::RemoveAttackHitBox()
{
	if (mAttackBox)
	{
		delete mAttackBox;
		mAttackBox = nullptr;
	}
}

// 自身の当たり判定を破棄
void StrongEnemy::RemoveHitBox()
{
	if (mHitBox)
	{
		delete mHitBox;
		mHitBox = nullptr;
	}
}

void StrongEnemy::LoadModel()
{
	mSkelMeshComponent = new SkeletalMeshComponent(this);
	mMesh = RENDERER->GetMesh("Assets/Mesh/SK_Greater_Spider_Boss.gpmesh");
}

// スケルトンの読み込み
void StrongEnemy::LoadSkeleton()
{
	mSkelMeshComponent->SetMesh(mMesh);
	mSkelMeshComponent->SetSkeleton(RENDERER->GetSkeleton("Assets/Mesh/SK_Greater_Spider_Boss.gpskel"));
}

// アニメーションの読み込み
void StrongEnemy::LoadAnimation()
{
	mAnimations.emplace(EnemyStateEnum::Walk, RENDERER->GetAnimation("Assets/Animation/Greater_Spider_Walk.gpanim", true));
	mAnimations.emplace(EnemyStateEnum::Run, RENDERER->GetAnimation("Assets/Animation/Greater_Spider_Walk.gpanim", true));
	mAnimations.emplace(EnemyStateEnum::Attack1, RENDERER->GetAnimation("Assets/Animation/ExoGame_Bears_Attack_Melee.gpanim", false));
	mAnimations.emplace(EnemyStateEnum::Spawn, RENDERER->GetAnimation("Assets/Animation/ExoGame_Greater_Spider_Spawn.gpanim", false));
	mAnimations.emplace(EnemyStateEnum::Death, RENDERER->GetAnimation("Assets/Animation/ExoGame_Greater_Spider_Death.gpanim", false));
}

// 強敵ふるまいの登録
void StrongEnemy::BehaviorResister()
{
	// アニメーション配列にふるまいを登録
	mEnemyBehaviorComponent = new EnemyBehaviorComponent(this);
	mEnemyBehaviorComponent->RegisterState(new EnemyIdle(mEnemyBehaviorComponent, mTarget));
	mEnemyBehaviorComponent->RegisterState(new EnemyChase(mEnemyBehaviorComponent, mTarget));
	mEnemyBehaviorComponent->RegisterState(new EnemyAttack(mEnemyBehaviorComponent));
	mEnemyBehaviorComponent->RegisterState(new EnemySpawn(mEnemyBehaviorComponent));
	mEnemyBehaviorComponent->RegisterState(new EnemyDeath(mEnemyBehaviorComponent));

	// 最初のアニメーションをセット
	mEnemyBehaviorComponent->SetFirstState(EnemyStateEnum::Spawn);
}

// 当たり判定のセット
void StrongEnemy::SetCollider()
{
	mEnemyBox = mMesh->GetCollisionBox();
	mEnemyBox.mMin.y *= 0.5f;
	mEnemyBox.mMax.y *= 0.5f;
	mHitBox = new BoxCollider(this);
	mHitBox->SetObjectBox(mEnemyBox);
	mHitBox->SetArrowRotate(true);
}

// 攻撃トリガーのセット
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
