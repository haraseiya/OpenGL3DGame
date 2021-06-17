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
	printf("PysicsWorld �쐬\n");

	// �����R���|�[�l���g�z��̊m��
	mBGBoxs.reserve(256);

	// �{�b�N�X���_�̏�����
	InitBoxVertices();

	// �f�o�b�O�p���C���̃V�F�[�_�[�ǂݍ���
	mLineShader = new Shader();
	mLineShader->Load("shaders/LineWorld.vert", "shaders/Line.frag");
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
	for (auto t : mBGTriggers) t->SetHitTriggerFlag(false);

	PlayerAndBGTest();			// �v���C���[�Ɣw�i�Փ�
	PlayerAndEnemyTest();		// �v���C���[�ƓG�Փ�
	PlayerAndNPCTest();			// �v���C���[��NPC�Փ�
	EnemyAndBGTest();			// �G�Ɣw�i�Փ�
	EnemyAndNPCTest();			// �G��NPC�̓����蔻��
	EnemyAttackAndNPCTest();	// �G�̍U���{�b�N�X��NPC�̓����蔻��
	EnemyTriggerAndNPCTest();	// �G�̃A�^�b�N�g���K�[��NPC�̓����蔻��
	NPCAndEenmyTest();			// NPC�ƓG�̓����蔻��
	NPCAndNPCTest();			// NPC���m�̓����蔻��
	NPCAttackAndEnemyTest();	// NPC�̍U���{�b�N�X��NPC�̓����蔻��

	// �g���K�[�Ɣw�i�̃q�b�g���ׂ�
	TriggerAndBGTest();
}

// �����蔻��̉����i�f�o�b�O�p�j
void PhysicsWorld::DebugShowBox()
{
	// �f�o�b�O���[�h���H
	if (!mBoolDebugMode)return;

	// AABB�`�揀��
	Matrix4 view, proj, viewProj;
	view = RENDERER->GetViewMatrix();
	proj = RENDERER->GetProjectionMatrix();
	viewProj = view * proj;
	mLineShader->SetActive();
	mLineShader->SetMatrixUniform("uViewProj", viewProj);

	// �����蔻��{�b�N�X�`��
	DrawBoxs(mBGBoxs, Color::Red);
	DrawBoxs(mPlayerBoxs, Color::Blue);
	DrawBoxs(mPlayerTrigger, Color::Yellow);
	DrawBoxs(mEnemyBoxs, Color::White);
	DrawBoxs(mEnemyAttackBoxs, Color::Green);
	DrawBoxs(mEnemyAttackTrigger, Color::Red);
	DrawBoxs(mBGTriggers, Color::LightGreen);
	DrawBoxs(mNPCBoxs, Color::LightPink);
	DrawBoxs(mNPCAttackBoxs, Color::Red);
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

// �v���C���[�Ɣw�i�̓����蔻��
void PhysicsWorld::PlayerAndBGTest()
{
	//�w�i�ƃv���[���[�̏Փˌ��o
	for (auto p : mPlayerBoxs)
	{
		for (auto b : mBGBoxs)
		{
			BoxCollider* player = p;
			BoxCollider* box = b;

			if (Intersect(player->GetWorldBox(), b->GetWorldBox()))
			{
				//�v���[���[�̕ǂ߂荞�ݏC��������
				dynamic_cast<Player*>(player->GetOwner())->FixCollision(player, box);
			}
		}
	}
}

// �v���C���[�ƓG�̓����蔻��
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

// �v���C���[��NPC�̓����蔻��
void PhysicsWorld::PlayerAndNPCTest()
{
	for (auto n : mNPCBoxs)
	{
		for (auto p : mPlayerBoxs)
		{
			// �Փ˂�����Run��Ԃ�
			if (Intersect(n->GetWorldBox(),p->GetWorldBox()))
			{
				dynamic_cast<NPCActorBase*>(n->GetOwner())->OnCollision(n,p);
			}
		}
	}

	for (auto n : mNPCBoxs)
	{
		for (auto pt : mPlayerTrigger)
		{
			if (Intersect(n->GetWorldBox(), pt->GetWorldBox()))
			{
				dynamic_cast<NPCActorBase*>(n->GetOwner())->OnCollision(n, pt);
			}
		}
	}
}

// �G�Ɣw�i�����蔻��
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

// �G��NPC�̓����蔻��
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
}

// �G�̍U����NPC�̓����蔻��
void PhysicsWorld::EnemyAttackAndNPCTest()
{
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

// �G��NPC�̓����蔻��
void PhysicsWorld::EnemyTriggerAndNPCTest()
{
	for (auto et : mEnemyAttackTrigger)
	{
		for (auto n : mNPCBoxs)
		{
			if (Intersect(et->GetWorldBox(), n->GetWorldBox()))
			{
				//dynamic_cast<EnemyBase*>(et->GetOwner())->OnCollision(et,n);
			}
		}
	}
}

// �g���K�[�Ɣw�i�Ƃ̏Փ˔���
void PhysicsWorld::TriggerAndBGTest()
{
	for (auto trigger : mBGTriggers)
	{
		for (auto bg : mBGBoxs)
		{
			// �Փ˂�����g���K�[�̃t���O��ON�ɂ���
			if (Intersect(trigger->GetWorldBox(), bg->GetWorldBox()))
			{
				trigger->SetHitTriggerFlag(true);
			}
		}
	}
}

// NPC�ƓG�̓����蔻��
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

// NPC���m�̓����蔻��
void PhysicsWorld::NPCAndNPCTest()
{
	for (auto n : mNPCBoxs)
	{
		// NPC�̓����蔻��
		for (int i = 1; i < mNPCBoxs.size() - 1; i++)
		{
			// ������ID����Ȃ��ꍇ
			if (n->GetID() != mNPCBoxs[i]->GetID())
			{
				// �Փ˔���
				if (Intersect(n->GetWorldBox(), mNPCBoxs[i]->GetWorldBox()))
				{
					dynamic_cast<NPCActorBase*>(n->GetOwner())->OnCollision(n, mNPCBoxs[i]);
				}
			}
		}
	}
}

// NPC�̍U���ƓG�̓����蔻��
void PhysicsWorld::NPCAttackAndEnemyTest()
{
	for (auto na : mNPCAttackBoxs)
	{
		for (auto e : mEnemyBoxs)
		{
			if (Intersect(na->GetWorldBox(), e->GetWorldBox()))
			{
				dynamic_cast<EnemyBase*>(na->GetOwner())->OnCollision(e, na);
			}
		}
	}
}

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


