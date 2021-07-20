#include <typeinfo>
#include <algorithm>

#include "PhysicsWorld.h"
#include "GameObject.h"
#include "Renderer.h"
#include "Shader.h"
#include "Player.h"
#include "EnemyBase.h"
#include "NPCActorBase.h"
#include "NPCState.h"
#include "Collision.h"
#include "BoxCollider.h"
#include "WallCollider.h"

PhysicsWorld::PhysicsWorld()
	: mBoolDebugMode(false)
{
	printf("PysicsWorld 作成\n");

	// 物理コンポーネント配列の確保
	mBGBoxs.reserve(256);

	// ボックス頂点の初期化
	InitBoxVertices();

	// デバッグ用ラインのシェーダー読み込み
	mLineShader = new Shader();
	mLineShader->Load("shaders/LineWorld.vert", "shaders/Line.frag");

	// 当たり判定ボックスのライン配列
	mLineColors.emplace_back(Color::White);
	mLineColors.emplace_back(Color::Red);
	mLineColors.emplace_back(Color::Green);
	mLineColors.emplace_back(Color::Blue);
	mLineColors.emplace_back(Color::Yellow);
	mLineColors.emplace_back(Color::LightYellow);
	mLineColors.emplace_back(Color::LightBlue);
	mLineColors.emplace_back(Color::LightPink);
	mLineColors.emplace_back(Color::LightGreen);
}

PhysicsWorld::~PhysicsWorld()
{
	printf("PysicsWorld 破棄\n");
	mBGBoxs.clear();
	delete mLineShader;
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
	// 片方だけリアクションを返す当たり判定テスト
	for (auto reactionPair : mOneSideReactions)
	{
		OneReactionMatch(reactionPair);
	}

	// 両方リアクションを返す当たり判定セット
	for (auto reactionPair : mDualReactions)
	{
		DualReactionMatch(reactionPair);
	}

	// 自分と同じリストの当たり判定セット
	for (auto t : mSelfReactions)
	{
		SelfReactionMatch(t);
	}

	// トリガーと背景のヒット調べる
	//TriggerAndBGTest();
}

void PhysicsWorld::AddCollider(ColliderComponent* collider)
{
	Tag t = collider->GetTag();
	mColliders[t].emplace_back(collider);
}

void PhysicsWorld::RemoveCollider(ColliderComponent* collider)
{
	Tag t = collider->GetOwner()->GetTag();
	// タグから検索して削除
	std::vector<ColliderComponent*>::iterator iter = std::find(mColliders[t].begin(), mColliders[t].end(), collider);
	if (iter != mColliders[t].end())
	{
		mColliders[t].erase(iter);
		return;
	}
}

// 当たり判定の可視化（デバッグ用）
void PhysicsWorld::DebugShowBox()
{
	// デバッグモードか？
	if (!mBoolDebugMode)
	{
		return;
	}

	// AABB描画準備
	Matrix4 scale, trans, world, view, proj, viewProj;
	view = RENDERER->GetViewMatrix();
	proj = RENDERER->GetProjectionMatrix();
	viewProj = view * proj;
	mLineShader->SetActive();
	mLineShader->SetMatrixUniform("uViewProj", viewProj);

	// 当たり判定ボックス描画 tag毎に色を変えてすべてのリスト表示
	int colorCount = 0;
	size_t colorNum = mLineColors.size();
	for (auto t = Tag::Begin; t != Tag::End; ++t)
	{
		DrawCollisions(mColliders[t], mLineColors[colorCount % colorNum]);
		colorCount++;
	}
}

// 当たり判定の描画
void PhysicsWorld::DrawBoxs(std::vector<BoxCollider*>& boxs, const Vector3& color)
{
	// 行列群
	Matrix4 scaleMat, posMat, worldMat;
	Vector3 scale, pos;

	mLineShader->SetVectorUniform("uColor", color);
	for (auto item : boxs)
	{
		AABB box;
		Vector3 min, max;
		box = item->GetWorldBox();

		// ボックスのスケールと位置を取得
		min = box.mMin;
		max = box.mMax;
		scale = max - min;
		pos = min;

		scaleMat = Matrix4::CreateScale(scale);
		posMat = Matrix4::CreateTranslation(pos);

		worldMat = scaleMat * posMat;
		mLineShader->SetMatrixUniform("uWorld", worldMat);

		glBindVertexArray(m_boxVAO);
		glDrawElements(GL_LINES, 24, GL_UNSIGNED_INT, 0);
	}
}

// 衝突ボックスの描画
void PhysicsWorld::DrawCollisions(std::vector<class ColliderComponent*>& collisions, const Vector3& color)
{
	Matrix4 scaleMat, posMat, rotMat, worldMat, slopeRot;
	Vector3 scale, pos;

	mLineShader->SetVectorUniform("uColor", color);
	for (auto item : collisions)
	{
		// Boxだった場合の描画
		if (item->GetColliderType() == ColliderTypeEnum::Box)
		{
			AABB box;
			Vector3 min, max;
			box = dynamic_cast<BoxCollider*>(item)->GetWorldBox();

			// ボックスのスケールと位置を取得
			min = box.mMin;
			max = box.mMax;
			scale = max - min;
			pos = min;

			scaleMat = Matrix4::CreateScale(scale);
			posMat = Matrix4::CreateTranslation(pos);

			worldMat = scaleMat * posMat;
			mLineShader->SetMatrixUniform("uWorld", worldMat);

			glBindVertexArray(mBoxVAO);
			glDrawElements(GL_LINES, 24, GL_UNSIGNED_INT, 0);
		}
		// Wallだった場合の描画 
		if (item->GetColliderType() == ColliderTypeEnum::Wall)
		{
			WallCollider* wallcol;
			Wall walldata;

			Vector3 scale; // 描画スケーリング係数
			Vector3 pos; // 描画位置
			Vector3 normal; // 壁法線

			// WallColliderと壁データ取得
			wallcol = dynamic_cast<WallCollider*>(item);
			walldata = wallcol->GetWall();
			normal = walldata.mNormal;
			// 4点の中点を求める
			for (int i = 0; i < 4; i++)
			{
				pos += walldata.mWallVertex[i];
			}
			pos = pos * 0.25f;

			// 行列
			scaleMat = Matrix4::CreateScale(walldata.mScale);
			rotMat = Matrix4::CreateRotationZ(walldata.mZRotate);
			posMat = Matrix4::CreateTranslation(pos);
			slopeRot = Matrix4::CreateRotationY(walldata.mSlopeAngle);

			worldMat = scaleMat * slopeRot * rotMat * posMat;
			mLineShader->SetMatrixUniform("uWorld", worldMat);

			glBindVertexArray(mSquareVAO);
			glDrawElements(GL_LINES, 10, GL_UNSIGNED_INT, 0);

		}
	}
}

//// プレイヤーと背景の当たり判定
//void PhysicsWorld::PlayerAndBGTest()
//{
//	//背景とプレーヤーの衝突検出
//	for (auto p : mPlayerBoxs)
//	{
//		for (auto b : mBGBoxs)
//		{
//			BoxCollider* player = p;
//			BoxCollider* box = b;
//
//			if (Intersect(player->GetWorldBox(), b->GetWorldBox()))
//			{
//				//プレーヤーの壁めり込み修正処理へ
//				dynamic_cast<Player*>(player->GetOwner())->FixCollision(player, box);
//			}
//		}
//	}
//}
//
//// プレイヤーと敵の当たり判定
//void PhysicsWorld::PlayerAndEnemyTest()
//{
//	for (auto p : mPlayerBoxs)
//	{
//		for (auto e : mEnemyBoxs)
//		{
//			if (Intersect(p->GetWorldBox(), e->GetWorldBox()))
//			{
//				dynamic_cast<Player*>(p->GetOwner())->FixCollision(p, e);
//			}
//		}
//	}
//}
//
//// プレイヤーとNPCの当たり判定
//void PhysicsWorld::PlayerAndNPCTest()
//{
//	for (auto n : mNPCBoxs)
//	{
//		for (auto p : mPlayerBoxs)
//		{
//			// 衝突したらRun状態に
//			if (Intersect(n->GetWorldBox(),p->GetWorldBox()))
//			{
//				dynamic_cast<NPCActorBase*>(n->GetOwner())->OnCollisionEnter(n,p);
//			}
//		}
//	}
//
//	for (auto n : mNPCBoxs)
//	{
//		for (auto pt : mPlayerTrigger)
//		{
//			if (Intersect(n->GetWorldBox(), pt->GetWorldBox()))
//			{
//				dynamic_cast<NPCActorBase*>(n->GetOwner())->OnCollisionEnter(n, pt);
//			}
//		}
//	}
//}
//
//// 敵と背景当たり判定
//void PhysicsWorld::EnemyAndBGTest()
//{
//	for (auto e : mEnemyBoxs)
//	{
//		for (auto bg : mBGBoxs)
//		{
//			if (Intersect(e->GetWorldBox(), bg->GetWorldBox()))
//			{
//				dynamic_cast<EnemyBase*>(e->GetOwner())->OnCollisionEnter(e, bg);
//			}
//		}
//	} 
//}
//
//// 敵とNPCの当たり判定
//void PhysicsWorld::EnemyAndNPCTest()
//{
//	for (auto e : mEnemyBoxs)
//	{
//		for (auto n : mNPCBoxs)
//		{
//			if (Intersect(e->GetWorldBox(), n->GetWorldBox()))
//			{
//				dynamic_cast<EnemyBase*>(e->GetOwner())->OnCollisionEnter(e, n);
//			}
//		}
//	}
//}
//
//// 敵の攻撃とNPCの当たり判定
//void PhysicsWorld::EnemyAttackAndNPCTest()
//{
//	for (auto ea : mEnemyAttackBoxs)
//	{
//		for (auto n : mNPCBoxs)
//		{
//			if (Intersect(ea->GetWorldBox(), n->GetWorldBox()))
//			{
//				dynamic_cast<NPCActorBase*>(n->GetOwner())->OnCollisionEnter(n,ea);
//			}
//		}
//	}
//}
//
//// 敵とNPCの当たり判定
//void PhysicsWorld::EnemyTriggerAndNPCTest()
//{
//	for (auto et : mEnemyAttackTrigger)
//	{
//		for (auto n : mNPCBoxs)
//		{
//			if (Intersect(et->GetWorldBox(), n->GetWorldBox()))
//			{
//				//dynamic_cast<EnemyBase*>(et->GetOwner())->OnCollision(et,n);
//			}
//		}
//	}
//}

//// トリガーと背景との衝突判定
//void PhysicsWorld::TriggerAndBGTest()
//{
//	for (auto trigger : mBGTriggers)
//	{
//		for (auto bg : mBGBoxs)
//		{
//			// 衝突したらトリガーのフラグをONにする
//			if (Intersect(trigger->GetWorldBox(), bg->GetWorldBox()))
//			{
//				trigger->SetHitTriggerFlag(true);
//			}
//		}
//	}
//}

//// NPCと敵の当たり判定
//void PhysicsWorld::NPCAndEenmyTest()
//{
//	for (auto n : mNPCBoxs)
//	{
//		for (auto e : mEnemyBoxs)
//		{
//			if (Intersect(n->GetWorldBox(),e->GetWorldBox()))
//			{
//				dynamic_cast<NPCActorBase*>(n->GetOwner())->OnCollisionEnter(n,e);
//			}
//		}
//	}
//}
//
//// NPC同士の当たり判定
//void PhysicsWorld::NPCAndNPCTest()
//{
//	for (auto n : mNPCBoxs)
//	{
//		// NPCの当たり判定
//		for (int i = 1; i < mNPCBoxs.size() - 1; i++)
//		{
//			// 自分のIDじゃない場合
//			if (n->GetID() != mNPCBoxs[i]->GetID())
//			{
//				// 衝突判定
//				if (Intersect(n->GetWorldBox(), mNPCBoxs[i]->GetWorldBox()))
//				{
//					dynamic_cast<NPCActorBase*>(n->GetOwner())->OnCollisionEnter(n, mNPCBoxs[i]);
//				}
//			}
//		}
//	}
//}

// NPCの攻撃と敵の当たり判定
//void PhysicsWorld::NPCAttackAndEnemyTest()
//{
//	for (auto na : mNPCAttackBoxs)
//	{
//		for (auto e : mEnemyBoxs)
//		{
//			if (Intersect(na->GetWorldBox(), e->GetWorldBox()))
//			{
//				dynamic_cast<EnemyBase*>(na->GetOwner())->OnCollisionEnter(e, na);
//			}
//		}
//	}
//}

// 当たり判定の初期化
void PhysicsWorld::InitBoxVertices()
{
	// ボックス頂点リスト
	float vertices[] = {
		0.0f, 0.0f, 0.0f,  // min
		1.0f, 0.0f, 0.0f,
		1.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 1.0f,  // max
		0.0f, 1.0f, 1.0f,
	};
	// ボックスのラインリスト
	unsigned int lineList[] = {
		0,1,
		1,2,
		2,3,
		3,0,
		4,5,
		5,6,
		6,7,
		7,4,
		0,4,
		1,5,
		2,6,
		3,7,
	};
	unsigned int vbo, ebo;
	glGenVertexArrays(1, &mBoxVAO);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ebo);
	glBindVertexArray(mBoxVAO);
	{
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(lineList), lineList, GL_STATIC_DRAW);
	}
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
}

void PhysicsWorld::InitSquareVertices()
{
	// ボックス頂点リスト
	float vertices[] = {  // 奥下→奥上→手前上→手前下
		0.0f, 0.5f,-0.5f,
		0.0f, 0.5f, 0.5f,
		0.0f,-0.5f, 0.5f,
		0.0f,-0.5f,-0.5f,
		0.0f, 0.0f, 0.0f, // 法線表示
	   -1.0f, 0.0f, 0.0f,
	};
	// ボックスのラインリスト
	unsigned int lineList[] = {
		0,1,
		1,2,
		2,3,
		3,0,
		4,5
	};

	// 頂点配列 mSquareVAOの設定
	unsigned int vbo, ebo;
	glGenVertexArrays(1, &mSquareVAO);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ebo);
	glBindVertexArray(mSquareVAO);
	{
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(lineList), lineList, GL_STATIC_DRAW);
	}
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
}

void PhysicsWorld::SetOneSideReactionCollisionPair(Tag noReactionType, Tag reactionType)
{
	collidePairs cp;
	cp.pair1 = noReactionType;
	cp.pair2 = reactionType;

	mOneSideReactions.emplace_back(cp);
}

void PhysicsWorld::SetDualReactionCollisionPair(Tag reaction1, Tag reaction2)
{
	collidePairs cp;
	cp.pair1 = reaction1;
	cp.pair2 = reaction2;
	mDualReactions.emplace_back(cp);
}

void PhysicsWorld::SetSelfReaction(Tag selfreaction)
{
	mSelfReactions.emplace_back(selfreaction);
}

void PhysicsWorld::ClearOneSidePair()
{
	mOneSideReactions.clear();
}

void PhysicsWorld::ClearDualPair()
{
	mDualReactions.clear();
}

void PhysicsWorld::ClearSelfPair()
{
	mSelfReactions.clear();
}

void PhysicsWorld::ClearAllPair()
{
	ClearOneSidePair();
	ClearDualPair();
	ClearSelfPair();
}

// 片方だけリアクションを行う当たり判定組み合わせを総当たりチェック
void PhysicsWorld::OneReactionMatch(collidePairs cp)
{
	for (auto obj : mColliders[cp.pair1])
	{
		for (auto reactionObj : mColliders[cp.pair2])
		{
			if (obj->CollisionDetection(reactionObj))
			{
				reactionObj->GetOwner()->OnCollisionEnter(obj);
			}
		}
	}
}

// 両方ともリアクションを行う当たり判定組み合わせを総チェック
void PhysicsWorld::DualReactionMatch(collidePairs cp)
{
	for (auto reaction1 : mColliders[cp.pair1])
	{
		for (auto reaction2 : mColliders[cp.pair2])
		{
			if (reaction1->CollisionDetection(reaction2))
			{
				// それぞれリアクションを起こし、もう一方の衝突相手を渡す
				reaction1->GetOwner()->OnCollisionEnter(reaction2);
				reaction2->GetOwner()->OnCollisionEnter(reaction1);
			}
		}
	}
}

// 自分と同じ当たり判定グループの総組み合わせを行う
void PhysicsWorld::SelfReactionMatch(Tag type)
{
	size_t size = mColliders[type].size();

	for (int i = 0; i < size; i++)
	{
		//自分自身の当たり判定は行わず、総当たりチェック
		for (int j = i + 1; j < size; j++)
		{
			if (mColliders[type][i]->CollisionDetection(mColliders[type][j]))
			{
				GameObject* obj1 = mColliders[type][i]->GetOwner();
				GameObject* obj2 = mColliders[type][j]->GetOwner();
				obj1->OnCollisionEnter(mColliders[type][i]);
				obj2->OnCollisionEnter(mColliders[type][j]);
			}
		}
	}
}
