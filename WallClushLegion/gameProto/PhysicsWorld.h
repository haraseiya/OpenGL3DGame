#pragma once
#include "Game.h"
#include "BoxCollider.h"

class BoxCollider;
class Shader;
class NPCState;

enum class EnumPhysicsType
{
	EnumEnemy,			// �G�L����
	EnumPlayer,			// �v���[���[
	EnumNPC,			// NPC
	EnumBG,				// �w�i�u���b�N
	EnumHitCheck,		// �q�b�g�`�F�b�N�p�i�����Ԃ��Ȃ�)
	EnumEnemyAttack,	// �G�̍U���p
	EnumBGTrigger,      // �w�i�Ƃ̐ڐG�g���K�[
	EnumNPCTrigger		// NPC�Ƃ̐ڐG�g���K�[
};

class PhysicsWorld
{
public:
	PhysicsWorld();
	~PhysicsWorld();

	// BoxCollider(AABB)�ǉ��E�폜
	void AddBoxCollider(EnumPhysicsType type,BoxCollider* box); 
	void RemoveBoxCollider(BoxCollider* box);          

	// �R���W����
	void Collision();

	// �f�o�b�O�p
	void DebugShowBox();
	void DebugShowBoxLists();                                 
	void ToggleDebugMode() { mBoolDebugMode = !mBoolDebugMode; }

private:
	std::vector<BoxCollider*> mBGBoxs;			// �w�i������f�[�^
	std::vector<BoxCollider*> mPlayerBoxs;      // �v���[���[������f�[�^
	std::vector<BoxCollider*> mEnemyBoxs;       // �G�L����   
	std::vector<BoxCollider*> mEnemyAttackBoxs;
	std::vector<BoxCollider*> mBGTriggers;      // �w�i�Ƃ̐ڐG����p�g���K�[  
	std::vector<BoxCollider*> mNPCTriggers;		// NPC�Ƃ̐ڐG�g���K�[
	std::vector<BoxCollider*> mPlayerAttackBox;	// �v���C���[�U������p�{�b�N�X
	std::vector<BoxCollider*> mNPCBoxs;			// NPC�̓����蔻��{�b�N�X

	void PlayerAndBGTest();				// �v���[���[�ƕǂƂ̂����蔻��
	void PlayerAndEnemyTest();			// �v���C���[�ƓG�̓����蔻��
	void PlayerAndNPCTest();			// �v���C���[��NPC�̓����蔻��
	void EnemyAndBGTest();				// �G�ƕǂƂ̓����蔻��
	void EnemyAndNPCTest();				// �G��NPC�̓����蔻��
	void TriggerAndBGTest();			// �w�i�g���K�[�Ɣw�i�̔���e�X�g
	void NPCAndEenmyTest();				// NPC�ƓG�̓����蔻��
	void NPCAndNPCTest();				// NPC��NPC�̓����蔻��
	void InitBoxVertices();				// �{�b�N�X���_�̏�����
	void DrawBoxs(std::vector<BoxCollider*>& boxs, const Vector3& color);

	bool mBoolDebugMode;	// �f�o�b�O���[�h���g�p���邩
	unsigned int m_boxVAO;	// �{�b�N�X�`��p��VAO  
	unsigned int m_VBO;
	unsigned int m_EBO;

	Shader* mLineShader;	// ���C���`��p�V�F�[�_�[
	NPCState* m_npcState;	// NPC�̏��
};