#pragma once
#include "Game.h"
#include "Tag.h"

#include <vector>
#include <unordered_map>

class BoxCollider;
class Shader;
class NPCState;

enum class EnumPhysicsType
{
	EnumPlayer,				// �v���C���[
	EnumPlayerTrigger,		// �v���C���[�̉r���͈͗p
	EnumEnemy,				// �G�L����
	EnumEnemyAttackBox,		// �G�̍U���p
	EnumEnemyAttackTrigger,	// �G�̍U���p�g���K�[
	EnumNPC,				// NPC
	EnumNPCAttackBox,		// NPC�̍U���p
	EnumNPCAttackTrigger,	// NPC�̍U���p�g���K�[
	EnumBG,					// �w�i�u���b�N
	EnumHitCheck,			// �q�b�g�`�F�b�N�p�i�����Ԃ��Ȃ�)
	EnumBGTrigger,			// �w�i�Ƃ̐ڐG�g���K�[
	EnumNPCTrigger			// NPC�Ƃ̐ڐG�g���K�[
};

class PhysicsWorld
{
public:
	struct collidePairs
	{
		Tag pair1;
		Tag pair2;
	};

	PhysicsWorld();
	~PhysicsWorld();

	// BoxCollider(AABB)�ǉ��E�폜
	void AddBoxCollider(EnumPhysicsType type,BoxCollider* box); 
	void RemoveBoxCollider(BoxCollider* box);          

	// SphereCollider�ǉ��E�폜
	//void AddSphereCollider(EnumPhysicsType type, Sphere* sphere);
	//void RemoveSphereCollider(Sphere* sphere);
	
	// �R���W����
	void Collision();

	void AddCollider(class ColliderComponent* collider);               // Collider�ǉ�
	void RemoveCollider(class ColliderComponent* collider);            // Collider�폜

	// �f�o�b�O�p
	void DebugShowBox();
	void DebugShowBoxLists();                                 
	void ToggleDebugMode() { mBoolDebugMode = !mBoolDebugMode; }

private:
	std::vector<BoxCollider*> mBGBoxs;			// �w�i������f�[�^
	std::vector<BoxCollider*> mPlayerBoxs;      // �v���[���[������f�[�^
	std::vector<BoxCollider*> mPlayerTrigger;
	std::vector<BoxCollider*> mEnemyBoxs;       // �G�L����   
	std::vector<BoxCollider*> mEnemyAttackBoxs;	// �G�̍U���p
	std::vector<BoxCollider*> mEnemyAttackTrigger;
	std::vector<BoxCollider*> mBGTriggers;      // �w�i�Ƃ̐ڐG����p�g���K�[  
	std::vector<BoxCollider*> mNPCTriggers;		// NPC�Ƃ̐ڐG�g���K�[
	std::vector<BoxCollider*> mPlayerAttackBox;	// �v���C���[�U������p�{�b�N�X
	std::vector<BoxCollider*> mNPCBoxs;			// NPC�̓����蔻��{�b�N�X
	std::vector<BoxCollider*> mNPCAttackBoxs;	// NPC�̍U���p

	std::unordered_map<Tag, std::vector<class ColliderComponent*>> mColliders;    // �����蔻��{�b�N�X���X�g

	void PlayerAndBGTest();				// �v���[���[�ƕǂƂ̂����蔻��
	void PlayerAndEnemyTest();			// �v���C���[�ƓG�̓����蔻��
	void PlayerAndNPCTest();			// �v���C���[��NPC�̓����蔻��
	void EnemyAndBGTest();				// �G�ƕǂƂ̓����蔻��
	void EnemyAndNPCTest();				// �G��NPC�̓����蔻��
	void EnemyAttackAndNPCTest();		// �G�̍U����NPC�̓����蔻��
	void EnemyTriggerAndNPCTest();
	void TriggerAndBGTest();			// �w�i�g���K�[�Ɣw�i�̔���e�X�g
	void NPCAndEenmyTest();				// NPC�ƓG�̓����蔻��
	void NPCAndNPCTest();				// NPC��NPC�̓����蔻��
	void NPCAttackAndEnemyTest();		// NPC�̍U���ƓG�̓����蔻��
	void InitBoxVertices();				// �{�b�N�X���_�̏�����
	void DrawBoxs(std::vector<BoxCollider*>& boxs, const Vector3& color);

	bool mBoolDebugMode;	// �f�o�b�O���[�h���g�p���邩
	unsigned int m_boxVAO;	// �{�b�N�X�`��p��VAO  
	unsigned int m_VBO;
	unsigned int m_EBO;

	Shader* mLineShader;	// ���C���`��p�V�F�[�_�[
	NPCState* m_npcState;	// NPC�̏��
};