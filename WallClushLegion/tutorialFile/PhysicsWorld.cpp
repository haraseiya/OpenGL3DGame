#include <typeinfo>
#include <algorithm>

#include "PhysicsWorld.h"
#include "Actor.h"
#include "playerActor.h"
#include "BoxCollider.h"
#include "Collision.h"

// PhysicsWorldコンストラクタ
PhysicsWorld::PhysicsWorld()
	: mBoolDebugMode(false)
{
	// 物理コンポーネント配列の確保
	mBGBoxs.reserve(256);

	printf("PysicsWorld 作成\n");

}

// デストラクタ
PhysicsWorld::~PhysicsWorld()
{
	printf("PysicsWorld 破棄\n");
	mBGBoxs.clear();
}

// BoxColliderの追加
void PhysicsWorld::AddBoxCollider(EnumPhysicsType type, BoxCollider *box)
{
	// プレーヤーあたり判定追加
	if (type == EnumPlayer)
	{
		mPlayerBoxs.push_back(box);
	}

	// 背景あたり判定追加
	if (type == EnumBG)
	{
		mBGBoxs.push_back(box);
	}

}

// boxColliderの削除
void PhysicsWorld::RemoveBoxCollider(BoxCollider* box)
{
	// BackGround内にいる？
	auto iterBG = std::find(mBGBoxs.begin(), mBGBoxs.end(), box);
	if (iterBG != mBGBoxs.end())
	{
		mBGBoxs.erase(iterBG);
		return;
	}
}

// デバッグ用　ボックスリスト表示
void PhysicsWorld::DebugShowBoxLists()
{

	if (mBGBoxs.size())
	{
		printf("\n-----------------PhysicsList--BGLists---------------\n");
		for (auto m : mBGBoxs)
		{
			Vector3 pos = m->GetOwner()->GetPosition();
			printf("%6d ", m->mGlobalID);
			printf("(% 7.2f,% 7.2f % 7.2f)-", pos.x, pos.y, pos.z);
			printf("[%p]\n", m->GetOwner());
		}
	}
}

void PhysicsWorld::Collision()
{
	// プレーヤーと背景衝突
	PlayerAndBGTest();

}

void PhysicsWorld::DebugShowBox()
{

}

void PhysicsWorld::PlayerAndBGTest()
{
	//背景とプレーヤーの衝突検出
	for (auto p : mPlayerBoxs)
	{
		for (auto b : mBGBoxs)
		{
			BoxCollider *player = p;
			BoxCollider *box = b;
			if (Intersect(player->GetWorldBox(), b->GetWorldBox()))
			{
				//プレーヤーの壁めり込み修正処理へ
				dynamic_cast<PlayerActor *>(player->GetOwner())->FixCollision( player, box );
			}
		}
	}

}

