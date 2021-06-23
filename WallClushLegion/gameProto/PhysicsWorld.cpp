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
	printf("PysicsWorld �쐬\n");

	// �����R���|�[�l���g�z��̊m��
	mBGBoxs.reserve(256);

	// �{�b�N�X���_�̏�����
	InitBoxVertices();

	// �f�o�b�O�p���C���̃V�F�[�_�[�ǂݍ���
	mLineShader = new Shader();
	mLineShader->Load("shaders/LineWorld.vert", "shaders/Line.frag");

	// �����蔻��{�b�N�X�̃��C���z��
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
	printf("PysicsWorld �j��\n");
	mBGBoxs.clear();
	delete mLineShader;
}

// �{�b�N�X�R���C�_�[�̒ǉ�
void PhysicsWorld::AddBoxCollider(EnumPhysicsType type, BoxCollider* box)
{
	switch (type)
	{
	// �v���C���[
	case EnumPhysicsType::EnumPlayer:
		mPlayerBoxs.push_back(box);
		break;

	case EnumPhysicsType::EnumPlayerTrigger:
		mPlayerTrigger.push_back(box);
		break;
	// �G
	case EnumPhysicsType::EnumEnemy:
		mEnemyBoxs.push_back(box);
		break;

	// �G�̍U��
	case EnumPhysicsType::EnumEnemyAttackBox:
		mEnemyAttackBoxs.push_back(box);
		break;

	// �G�̍U���g���K�[
	case EnumPhysicsType::EnumEnemyAttackTrigger:
		mEnemyAttackTrigger.push_back(box);
		break;
	// NPC
	case EnumPhysicsType::EnumNPC:
		mNPCBoxs.push_back(box);
		break;

	// NPC�g���K�[
	case EnumPhysicsType::EnumNPCTrigger:
		mNPCTriggers.push_back(box);
		break;

	// �w�i
	case  EnumPhysicsType::EnumBG:
		mBGBoxs.push_back(box);
		break;

	// �w�i�g���K�[
	case EnumPhysicsType::EnumBGTrigger:
		mBGTriggers.push_back(box);
		break;
	}
}

// �{�b�N�X�R���C�_�[�̍폜
void PhysicsWorld::RemoveBoxCollider(BoxCollider* box)
{
	// �v���[���[�̃{�b�N�X���ɂ��邩�ǂ���
	auto iterPlayer = std::find(mPlayerBoxs.begin(), mPlayerBoxs.end(), box);
	if (iterPlayer != mPlayerBoxs.end())
	{
		mPlayerBoxs.erase(iterPlayer);
		return;
	}

	auto iterPlayerTrig = std::find(mPlayerTrigger.begin(), mPlayerTrigger.end(), box);
	if (iterPlayerTrig != mPlayerTrigger.end())
	{
		mPlayerTrigger.erase(iterPlayerTrig);
		return;
	}

	// �G�ɂ̃{�b�N�X���ɂ��邩�ǂ���
	auto iterEnemy = std::find(mEnemyBoxs.begin(), mEnemyBoxs.end(), box);
	if (iterEnemy != mEnemyBoxs.end())
	{
		mEnemyBoxs.erase(iterEnemy);
		return;
	}

	// �G�ɂ̃{�b�N�X���ɂ��邩�ǂ���
	auto iterEnemyAttack = std::find(mEnemyAttackBoxs.begin(), mEnemyAttackBoxs.end(), box);
	if (iterEnemyAttack != mEnemyAttackBoxs.end())
	{
		mEnemyAttackBoxs.erase(iterEnemyAttack);
		return;
	}

	auto iterEnemyAttackTrigger = std::find(mEnemyAttackTrigger.begin(), mEnemyAttackTrigger.end(), box);
	if (iterEnemyAttackTrigger != mEnemyAttackTrigger.end())
	{
		mEnemyAttackTrigger.erase(iterEnemyAttackTrigger);
		return;
	}

	// BackGround���ɂ���H
	auto iterBG = std::find(mBGBoxs.begin(), mBGBoxs.end(), box);
	if (iterBG != mBGBoxs.end())
	{
		mBGBoxs.erase(iterBG);
		return;
	}

	// NPC�̃{�b�N�X���ɂ��邩�ǂ���
	auto iterNPC = std::find(mNPCBoxs.begin(), mNPCBoxs.end(), box);
	if (iterNPC != mNPCBoxs.end())
	{
		mNPCBoxs.erase(iterNPC);
		return;
	}

	// �g���K�[���ɂ���H
	auto iterTrig = std::find(mBGTriggers.begin(), mBGTriggers.end(), box);
	if (iterTrig != mBGTriggers.end())
	{
		mBGTriggers.erase(iterTrig);
		return;
	}
}

// �f�o�b�O�p�@�{�b�N�X���X�g�\��
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
	// �����蔻�茟�o�O�ɂ��ׂẴg���K�[�����Z�b�g
	//for (auto t : mBGTriggers) t->SetHitTriggerFlag(false);

	//PlayerAndBGTest();			// �v���C���[�Ɣw�i�Փ�
	//PlayerAndEnemyTest();		// �v���C���[�ƓG�Փ�
	//PlayerAndNPCTest();			// �v���C���[��NPC�Փ�
	//EnemyAndBGTest();			// �G�Ɣw�i�Փ�
	//EnemyAndNPCTest();			// �G��NPC�̓����蔻��
	//EnemyAttackAndNPCTest();	// �G�̍U���{�b�N�X��NPC�̓����蔻��
	//EnemyTriggerAndNPCTest();	// �G�̃A�^�b�N�g���K�[��NPC�̓����蔻��
	//NPCAndEenmyTest();			// NPC�ƓG�̓����蔻��
	//NPCAndNPCTest();			// NPC���m�̓����蔻��
	//NPCAttackAndEnemyTest();	// NPC�̍U���{�b�N�X��NPC�̓����蔻��

		// �Е��������A�N�V������Ԃ������蔻��e�X�g
	for (auto reactionPair : mOneSideReactions)
	{
		OneReactionMatch(reactionPair);
	}

	// �������A�N�V������Ԃ������蔻��Z�b�g
	for (auto reactionPair : mDualReactions)
	{
		DualReactionMatch(reactionPair);
	}

	// �����Ɠ������X�g�̓����蔻��Z�b�g
	for (auto t : mSelfReactions)
	{
		SelfReactionMatch(t);
	}

	// �g���K�[�Ɣw�i�̃q�b�g���ׂ�
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
	// �^�O���猟�����č폜
	std::vector<ColliderComponent*>::iterator iter = std::find(mColliders[t].begin(), mColliders[t].end(), collider);
	if (iter != mColliders[t].end())
	{
		mColliders[t].erase(iter);
		return;
	}
}

// �����蔻��̉����i�f�o�b�O�p�j
void PhysicsWorld::DebugShowBox()
{
	// �f�o�b�O���[�h���H
	if (!mBoolDebugMode)
	{
		return;
	}

	// AABB�`�揀��
	Matrix4 scale, trans, world, view, proj, viewProj;
	view = RENDERER->GetViewMatrix();
	proj = RENDERER->GetProjectionMatrix();
	viewProj = view * proj;
	mLineShader->SetActive();
	mLineShader->SetMatrixUniform("uViewProj", viewProj);

	// �����蔻��{�b�N�X�`�� tag���ɐF��ς��Ă��ׂẴ��X�g�\��
	int colorCount = 0;
	size_t colorNum = mLineColors.size();
	for (auto t = Tag::Begin; t != Tag::End; ++t)
	{
		DrawCollisions(mColliders[t], mLineColors[colorCount % colorNum]);
		colorCount++;
	}
}

// �����蔻��̕`��
void PhysicsWorld::DrawBoxs(std::vector<BoxCollider*>& boxs, const Vector3& color)
{
	// �s��Q
	Matrix4 scaleMat, posMat, worldMat;
	Vector3 scale, pos;

	mLineShader->SetVectorUniform("uColor", color);
	for (auto item : boxs)
	{
		AABB box;
		Vector3 min, max;
		box = item->GetWorldBox();

		// �{�b�N�X�̃X�P�[���ƈʒu���擾
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

// �Փ˃{�b�N�X�̕`��
void PhysicsWorld::DrawCollisions(std::vector<class ColliderComponent*>& collisions, const Vector3& color)
{
	Matrix4 scaleMat, posMat, rotMat, worldMat, slopeRot;
	Vector3 scale, pos;

	mLineShader->SetVectorUniform("uColor", color);
	for (auto item : collisions)
	{
		// Box�������ꍇ�̕`��
		if (item->GetColliderType() == ColliderTypeEnum::Box)
		{
			AABB box;
			Vector3 min, max;
			box = dynamic_cast<BoxCollider*>(item)->GetWorldBox();

			// �{�b�N�X�̃X�P�[���ƈʒu���擾
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
		// Wall�������ꍇ�̕`�� 
		if (item->GetColliderType() == ColliderTypeEnum::Wall)
		{
			WallCollider* wallcol;
			Wall walldata;

			Vector3 scale; // �`��X�P�[�����O�W��
			Vector3 pos; // �`��ʒu
			Vector3 normal; // �ǖ@��

			// WallCollider�ƕǃf�[�^�擾
			wallcol = dynamic_cast<WallCollider*>(item);
			walldata = wallcol->GetWall();
			normal = walldata.mNormal;
			// 4�_�̒��_�����߂�
			for (int i = 0; i < 4; i++)
			{
				pos += walldata.mWallVertex[i];
			}
			pos = pos * 0.25f;

			// �s��
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

//// �v���C���[�Ɣw�i�̓����蔻��
//void PhysicsWorld::PlayerAndBGTest()
//{
//	//�w�i�ƃv���[���[�̏Փˌ��o
//	for (auto p : mPlayerBoxs)
//	{
//		for (auto b : mBGBoxs)
//		{
//			BoxCollider* player = p;
//			BoxCollider* box = b;
//
//			if (Intersect(player->GetWorldBox(), b->GetWorldBox()))
//			{
//				//�v���[���[�̕ǂ߂荞�ݏC��������
//				dynamic_cast<Player*>(player->GetOwner())->FixCollision(player, box);
//			}
//		}
//	}
//}
//
//// �v���C���[�ƓG�̓����蔻��
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
//// �v���C���[��NPC�̓����蔻��
//void PhysicsWorld::PlayerAndNPCTest()
//{
//	for (auto n : mNPCBoxs)
//	{
//		for (auto p : mPlayerBoxs)
//		{
//			// �Փ˂�����Run��Ԃ�
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
//// �G�Ɣw�i�����蔻��
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
//// �G��NPC�̓����蔻��
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
//// �G�̍U����NPC�̓����蔻��
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
//// �G��NPC�̓����蔻��
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

//// �g���K�[�Ɣw�i�Ƃ̏Փ˔���
//void PhysicsWorld::TriggerAndBGTest()
//{
//	for (auto trigger : mBGTriggers)
//	{
//		for (auto bg : mBGBoxs)
//		{
//			// �Փ˂�����g���K�[�̃t���O��ON�ɂ���
//			if (Intersect(trigger->GetWorldBox(), bg->GetWorldBox()))
//			{
//				trigger->SetHitTriggerFlag(true);
//			}
//		}
//	}
//}

//// NPC�ƓG�̓����蔻��
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
//// NPC���m�̓����蔻��
//void PhysicsWorld::NPCAndNPCTest()
//{
//	for (auto n : mNPCBoxs)
//	{
//		// NPC�̓����蔻��
//		for (int i = 1; i < mNPCBoxs.size() - 1; i++)
//		{
//			// ������ID����Ȃ��ꍇ
//			if (n->GetID() != mNPCBoxs[i]->GetID())
//			{
//				// �Փ˔���
//				if (Intersect(n->GetWorldBox(), mNPCBoxs[i]->GetWorldBox()))
//				{
//					dynamic_cast<NPCActorBase*>(n->GetOwner())->OnCollisionEnter(n, mNPCBoxs[i]);
//				}
//			}
//		}
//	}
//}

// NPC�̍U���ƓG�̓����蔻��
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

// �����蔻��̏�����
void PhysicsWorld::InitBoxVertices()
{
	// �{�b�N�X���_���X�g
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

	// �{�b�N�X�̃��C�����X�g
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

// �Е��������A�N�V�������s�������蔻��g�ݍ��킹�𑍓�����`�F�b�N
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

// �����Ƃ����A�N�V�������s�������蔻��g�ݍ��킹�𑍃`�F�b�N
void PhysicsWorld::DualReactionMatch(collidePairs cp)
{
	for (auto reaction1 : mColliders[cp.pair1])
	{
		for (auto reaction2 : mColliders[cp.pair2])
		{
			if (reaction1->CollisionDetection(reaction2))
			{
				// ���ꂼ�ꃊ�A�N�V�������N�����A��������̏Փˑ����n��
				reaction1->GetOwner()->OnCollisionEnter(reaction2);
				reaction2->GetOwner()->OnCollisionEnter(reaction1);
			}
		}
	}
}

// �����Ɠ��������蔻��O���[�v�̑��g�ݍ��킹���s��
void PhysicsWorld::SelfReactionMatch(Tag type)
{
	size_t size = mColliders[type].size();

	for (int i = 0; i < size; i++)
	{
		//�������g�̓����蔻��͍s�킸�A��������`�F�b�N
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
