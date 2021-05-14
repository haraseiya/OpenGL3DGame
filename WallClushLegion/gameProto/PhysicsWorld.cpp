#include <typeinfo>
#include <algorithm>

#include "Renderer.h"
#include "Shader.h"
#include "PhysicsWorld.h"
#include "GameObject.h"
#include "Player.h"
#include "EnemyBase.h"
#include "NPCActorBase.h"
#include "NPCState.h"
#include "BoxCollider.h"
#include "Collision.h"

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
}

PhysicsWorld::~PhysicsWorld()
{
	printf("PysicsWorld 破棄\n");
	mBGBoxs.clear();
	delete mLineShader;
}

// ボックスコライダーの追加
void PhysicsWorld::AddBoxCollider(EnumPhysicsType type, BoxCollider* box)
{
	switch (type)
	{
	// プレイヤー
	case EnumPhysicsType::EnumPlayer:
		mPlayerBoxs.push_back(box);
		break;

	// 敵
	case EnumPhysicsType::EnumEnemy:
		mEnemyBoxs.push_back(box);
		break;

	// 敵の攻撃
	case EnumPhysicsType::EnumEnemyAttack:
		mEnemyAttackBoxs.push_back(box);
		break;

	// NPC
	case EnumPhysicsType::EnumNPC:
		mNPCBoxs.push_back(box);
		break;

	// NPCトリガー
	case EnumPhysicsType::EnumNPCTrigger:
		mNPCTriggers.push_back(box);
		break;

	// 背景
	case  EnumPhysicsType::EnumBG:
		mBGBoxs.push_back(box);
		break;

	// 背景トリガー
	case EnumPhysicsType::EnumBGTrigger:
		mBGTriggers.push_back(box);
		break;
	}
}

// ボックスコライダーの削除
void PhysicsWorld::RemoveBoxCollider(BoxCollider* box)
{
	// プレーヤーのボックス内にいるかどうか
	auto iterPlayer = std::find(mPlayerBoxs.begin(), mPlayerBoxs.end(), box);
	if (iterPlayer != mPlayerBoxs.end())
	{
		mPlayerBoxs.erase(iterPlayer);
		return;
	}

	// プレーヤーの攻撃判定内にいるかどうか
	auto iterPlayerAttack = std::find(mPlayerAttackBox.begin(), mPlayerAttackBox.end(), box);
	if (iterPlayerAttack != mPlayerAttackBox.end())
	{
		mPlayerAttackBox.erase(iterPlayerAttack);
		return;
	}

	// 敵にのボックス内にいるかどうか
	auto iterEnemy = std::find(mEnemyBoxs.begin(), mEnemyBoxs.end(), box);
	if (iterEnemy != mEnemyBoxs.end())
	{
		mEnemyBoxs.erase(iterEnemy);
		return;
	}

	// 敵にのボックス内にいるかどうか
	auto iterEnemyAttack = std::find(mEnemyAttackBoxs.begin(), mEnemyAttackBoxs.end(), box);
	if (iterEnemyAttack != mEnemyAttackBoxs.end())
	{
		mEnemyAttackBoxs.erase(iterEnemyAttack);
		return;
	}

	// BackGround内にいる？
	auto iterBG = std::find(mBGBoxs.begin(), mBGBoxs.end(), box);
	if (iterBG != mBGBoxs.end())
	{
		mBGBoxs.erase(iterBG);
		return;
	}

	// NPCのボックス内にいるかどうか
	auto iterNPC = std::find(mNPCBoxs.begin(), mNPCBoxs.end(), box);
	if (iterNPC != mNPCBoxs.end())
	{
		mNPCBoxs.erase(iterNPC);
		return;
	}

	// トリガー内にいる？
	auto iterTrig = std::find(mBGTriggers.begin(), mBGTriggers.end(), box);
	if (iterTrig != mBGTriggers.end())
	{
		mBGTriggers.erase(iterTrig);
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
	// 当たり判定検出前にすべてのトリガーをリセット
	for (auto t : mBGTriggers) t->SetHitTriggerFlag(false);

	PlayerAndBGTest();		// プレイヤーと背景衝突
	PlayerAndEnemyTest();	// プレイヤーと敵衝突
	PlayerAndNPCTest();		// プレイヤーとNPC衝突
	EnemyAndBGTest();		// 敵と背景衝突
	EnemyAndNPCTest();		// 敵とNPCの当たり判定
	NPCAndEenmyTest();		// NPCと敵の当たり判定
	NPCAndNPCTest();		// NPC同士の当たり判定

	// トリガーと背景のヒット調べる
	TriggerAndBGTest();
}

void PhysicsWorld::DebugShowBox()
{
	// デバッグモードか？
	if (!mBoolDebugMode)return;

	// AABB描画準備
	Matrix4 view, proj, viewProj;
	view = RENDERER->GetViewMatrix();
	proj = RENDERER->GetProjectionMatrix();
	viewProj = view * proj;
	mLineShader->SetActive();
	mLineShader->SetMatrixUniform("uViewProj", viewProj);

	// 当たり判定ボックス描画
	DrawBoxs(mBGBoxs, Color::Red);
	DrawBoxs(mPlayerBoxs, Color::Blue);
	DrawBoxs(mEnemyBoxs, Color::White);
	DrawBoxs(mBGTriggers, Color::LightGreen);
	DrawBoxs(mNPCBoxs, Color::LightPink);
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

// プレイヤーと背景の当たり判定
void PhysicsWorld::PlayerAndBGTest()
{
	//背景とプレーヤーの衝突検出
	for (auto p : mPlayerBoxs)
	{
		for (auto b : mBGBoxs)
		{
			BoxCollider* player = p;
			BoxCollider* box = b;

			if (Intersect(player->GetWorldBox(), b->GetWorldBox()))
			{
				//プレーヤーの壁めり込み修正処理へ
				dynamic_cast<Player*>(player->GetOwner())->FixCollision(player, box);
			}
		}
	}
}

// プレイヤーと敵の当たり判定
void PhysicsWorld::PlayerAndEnemyTest()
{
	for (auto p : mPlayerBoxs)
	{
		for (auto e : mEnemyBoxs)
		{
			if (Intersect(p->GetWorldBox(), e->GetWorldBox()))
			{
				dynamic_cast<Player*>(p->GetOwner())->FixCollision(p, e);
			}
		}
	}
}

// プレイヤーとNPCの当たり判定
void PhysicsWorld::PlayerAndNPCTest()
{
	for (auto n : mNPCBoxs)
	{
		for (auto p : mPlayerBoxs)
		{
			// 衝突したらRun状態に
			if (Intersect(n->GetWorldBox(),p->GetWorldBox()))
			{
				dynamic_cast<NPCActorBase*>(n->GetOwner())->OnCollision(n,p);
			}
		}
	}
}

// 敵と背景当たり判定
void PhysicsWorld::EnemyAndBGTest()
{
	for (auto e : mEnemyBoxs)
	{
		for (auto bg : mBGBoxs)
		{
			if (Intersect(e->GetWorldBox(), bg->GetWorldBox()))
			{
				dynamic_cast<EnemyBase*>(e->GetOwner())->OnCollision(e, bg);
			}
		}
	} 
}

// 敵とNPCの当たり判定
void PhysicsWorld::EnemyAndNPCTest()
{
	for (auto e : mEnemyBoxs)
	{
		for (auto n : mNPCBoxs)
		{
			if (Intersect(e->GetWorldBox(), n->GetWorldBox()))
			{
				dynamic_cast<EnemyBase*>(e->GetOwner())->OnCollision(e, n);
			}
		}
	}

	for (auto ea : mEnemyAttackBoxs)
	{
		for (auto n : mNPCBoxs)
		{
			if (Intersect(ea->GetWorldBox(), n->GetWorldBox()))
			{
				dynamic_cast<NPCActorBase*>(n->GetOwner())->OnCollision(n,ea);
			}
		}
	}
}

// トリガーと背景との衝突判定
void PhysicsWorld::TriggerAndBGTest()
{
	for (auto trigger : mBGTriggers)
	{
		for (auto bg : mBGBoxs)
		{
			// 衝突したらトリガーのフラグをONにする
			if (Intersect(trigger->GetWorldBox(), bg->GetWorldBox()))
			{
				trigger->SetHitTriggerFlag(true);
			}
		}
	}
}

// NPCと敵の当たり判定
void PhysicsWorld::NPCAndEenmyTest()
{
	for (auto n : mNPCBoxs)
	{
		for (auto e : mEnemyBoxs)
		{
			if (Intersect(n->GetWorldBox(),e->GetWorldBox()))
			{
				dynamic_cast<NPCActorBase*>(n->GetOwner())->OnCollision(n,e);
			}
		}
	}
}

// NPC同士の当たり判定
void PhysicsWorld::NPCAndNPCTest()
{
	for (auto n1 : mNPCBoxs)
	{
		for (int i=1;i<mNPCBoxs.size()-1;i++)
		{
			// 自分のIDじゃない場合
			if (n1->GetID() != mNPCBoxs[i]->GetID())
			{
				if (Intersect(n1->GetWorldBox(), mNPCBoxs[i]->GetWorldBox()))
				{
					dynamic_cast<NPCActorBase*>(n1->GetOwner())->OnCollision(n1, mNPCBoxs[i]);
				}
			}
		}
	}
}

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

	glGenVertexArrays(1, &m_boxVAO);
	glGenBuffers(1, &m_VBO);
	glGenBuffers(1, &m_EBO);
	glBindVertexArray(m_boxVAO);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(lineList), lineList, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
}


