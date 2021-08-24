#include "PlayerBase.h"
#include "Game.h"
#include "Input.h"
#include "Mesh.h"
#include "Renderer.h"
#include "MeshComponent.h"
#include "SkeletalMeshComponent.h"
#include "Skeleton.h"
#include "Animation.h"
#include "PhysicsWorld.h"
#include "BoxCollider.h"
#include "Collision.h"
#include "AttachMeshComponent.h"
#include "EffectComponent.h"
#include "ChantEffect.h"
#include "Bullet.h"

#include "PlayerStateBase.h"
#include "PlayerStateRun.h"
#include "PlayerStateIdle.h"

PlayerBase::PlayerBase()
	: GameObject(Tag::Player)
{	// 大きさを100分の1に
	mScale = 0.01f;

	//メッシュのロード
	Mesh* mesh = RENDERER->GetMesh("Assets/Mesh/Player.gpmesh");
	mMeshComp = new SkeletalMeshComponent(this);
	mMeshComp->SetMesh(mesh);

	// スケルトン
	mMeshComp->SetSkeleton(RENDERER->GetSkeleton("Assets/Skelton/Player.gpskel"));

	// アニメーションの取得 & アニメーション配列にセット
	mAnimTypes.resize(static_cast<unsigned int>(PlayerState::PLAYER_STATE_NUM));
	mAnimTypes[static_cast<unsigned int>(PlayerState::PLAYER_STATE_IDLE)] = RENDERER->GetAnimation("Assets/Animation/Player_Idle.gpanim", true);
	mAnimTypes[static_cast<unsigned int>(PlayerState::PLAYER_STATE_RUN)] = RENDERER->GetAnimation("Assets/Animation/Player_Running.gpanim", true);
	mAnimTypes[static_cast<unsigned int>(PlayerState::PLAYER_STATE_REVIVE)] = RENDERER->GetAnimation("Assets/Animation/Player_Revive.gpanim", true);

	// アイドル状態アニメーションをセット
	mMeshComp->PlayAnimation(mAnimTypes[static_cast<unsigned int>(PlayerState::PLAYER_STATE_IDLE)], cAnimationSpeed);

	// プレイヤーステートプールの初期化
	mStatePools.push_back(new PlayerStateIdle);			// 待機状態
	mStatePools.push_back(new PlayerStateRun);			// 走る状態	

	// あたり判定セット
	AABB playerBox = mesh->GetCollisionBox();
	mHitBox = new BoxCollider(this);
	playerBox.mMin.x *= 1.2f;
	playerBox.mMin.y *= 1.2f;
	playerBox.mMax.x *= 1.2f;
	playerBox.mMax.y *= 1.2f;
	mHitBox->SetObjectBox(playerBox);

	// プレーヤーの足元を調べるボックスを作成　ボックス高1/4, ボックス上面が原点に来るようにする
	AABB groundBox;
	groundBox = playerBox;
	groundBox.mMin.x *= 0.8f;
	groundBox.mMin.y *= 0.8f;
	groundBox.mMax.x *= 0.8f;
	groundBox.mMax.y *= 0.8f;
	groundBox.mMin.z = -2.0f;  //ジャンプ時に引っかからない高さ
	groundBox.mMax.z *= 0.0f;
	mHitGroundBox = new BoxCollider(this);
	mHitGroundBox->SetObjectBox(groundBox);

	// プレーヤーの頭上を調べるボックスを作成 ボックス底面が頭上に来るようにする
	AABB headBox;
	headBox = groundBox;
	headBox.mMin.z = playerBox.mMax.z;
	headBox.mMax.z = headBox.mMin.z + 2.0f;
	mHitHeadBox = new BoxCollider(this);
	mHitHeadBox->SetObjectBox(headBox);

	printf("PlayerActor作成 id:[%5d] this : (0x%p)\n", mID, this);

}

PlayerBase::~PlayerBase()
{
	mAnimTypes.clear(); //アニメーション本体の消去はレンダラー側で行われる
	printf("PlayerActor破棄 id:[%5d] this : (0x%p)\n", mID, this);
}

void PlayerBase::UpdateActor(float deltaTime)
{
	const bool canChangeState = mNowState != mNextState;

	// ステート外部からステート変更があったか？
	if (canChangeState)
	{
		mStatePools[static_cast<unsigned int>(mNowState)]->Exit(this, deltaTime);
		mStatePools[static_cast<unsigned int>(mNextState)]->Enter(this, deltaTime);
		mNowState = mNextState;
		return;
	}

	// ステート実行
	mNextState = mStatePools[static_cast<unsigned int>(mNowState)]->Update(this, deltaTime);

	// ステート内部からステート変更あったか？
	if (mNowState != mNextState)
	{
		mStatePools[static_cast<unsigned int>(mNowState)]->Exit(this, deltaTime);
		mStatePools[static_cast<unsigned int>(mNextState)]->Enter(this, deltaTime);
		mNowState = mNextState;
	}

	// 敵が存在しないならAimモード停止
	if (!GAMEINSTANCE.IsExistActorType(Tag::Enemy))
	{
		mAimMode = false;
		return;
	}
	if (!mAimMode)
	{
		mTarget = GAMEINSTANCE.GetEnemyActor();
	}

	// ターゲットを指定
	Vector3 aimPos, aimDir;
	aimPos = mTarget->GetPosition();

	//自身から敵に向かう向きベクトルを計算
	aimDir = aimPos - mPosition;
	aimDir.z = 0.0f;

	// プレーヤーと十分距離があるなら向きを変更
	if (aimDir.LengthSq() > 0.5f)
	{
		aimDir.Normalize();
		//mDirection = aimDir;
	}

	// 弾が撃てるのであれば
	mShootTimer += deltaTime;
	const bool isShot = mShootTimer > mInterval && INPUT_INSTANCE.GetInput(KEY_R) == KEY_STATE_PRESSED;
	if (isShot)
	{
		mShootTimer = 0.0f;
		Bullet* ba = new Bullet(mPosition, this->GetForward(), Tag::PlayerBullet);
	}
}

// 背景AABBとのヒットめり込み解消 ( 当たった際にPhysicsWorldから呼ばれる ）
void PlayerBase::FixCollision(BoxCollider* hitPlayerBox, BoxCollider* hitBox)
{
}

SkeletalMeshComponent* PlayerBase::GetSkeletalMeshComp()
{
	return mMeshComp;
}

const Animation* PlayerBase::GetAnim(PlayerState state)
{
	return mAnimTypes[static_cast<unsigned int>(state)];
}

void PlayerBase::OnCollisionEnter(ColliderComponent* own, ColliderComponent* other)
{
	// タグ追加
	Tag colliderTag = other->GetTag();

	// 衝突した物体のタグが背景の場合
	if (colliderTag == Tag::BackGround)
	{
		if (other->GetColliderType() == ColliderTypeEnum::Box)
		{
			Vector3 fix;

			// 壁とぶつかったとき
			AABB playerBox = mHitBox->GetWorldBox();
			AABB bgBox = dynamic_cast<BoxCollider*>(other)->GetWorldBox();

			// めり込みを修正
			calcCollisionFixVec(playerBox, bgBox, fix);

			// 補正ベクトル分戻す
			mPosition += fix;

			// 位置が変わったのでボックス再計算
			ComputeWorldTransform();
		}
	}

	// 衝突した物体のタグが敵の場合
	if (colliderTag == Tag::Enemy)
	{
		if (other->GetColliderType() == ColliderTypeEnum::Box)
		{
			Vector3 fix;

			// 壁とぶつかったとき
			AABB playerBox = mHitBox->GetWorldBox();
			AABB enemyBox = dynamic_cast<BoxCollider*>(other)->GetWorldBox();

			// めり込みを修正
			calcCollisionFixVec(playerBox, enemyBox, fix);

			// 補正ベクトル分戻す
			mPosition += fix;

			// 位置が変わったのでボックス再計算
			ComputeWorldTransform();
		}
	}
}

