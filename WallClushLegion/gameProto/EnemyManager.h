#pragma once
#include <vector>

class GameObject;
class EnemyBase;
class WeakEnemy;
class StrongEnemy;
class BossEnemy;

class EnemyManager
{
public:
	EnemyManager(GameObject* target);
	~EnemyManager();

	void CreateFirstWave();			// �ŏ��̃E�F�[�u���쐬
	void CreateWave(int waveCount);				// �E�F�[�u�̍쐬
	void RemoveDeadEnemy();			// ���񂾓G��j��
	void Update(float deltaTime);	// �X�V����

	bool GetWaveFinishFlag() { return mIsLastWave; }	// �I���t���O�̎擾
	bool GetEnemyExtinction();							// 

private:
	GameObject* mTarget;	// �^�[�Q�b�g����I�u�W�F�N�g

	std::vector<std::vector<EnemyBase*>> mEnemyWaveList;	// �G�E�F�[�u���X�g
	std::vector<EnemyBase*> mEnemyWave1;
	std::vector<EnemyBase*> mEnemyWave2;
	std::vector<EnemyBase*> mEnemyWave3;

	static const int mMaxEnemyWave;
	static const int mMaxEnemyNum;		// �G�̍ő吔
	float mTime;						// �^�C�}�[
	int mWaveCount;						// �E�F�[�u�̃J�E���g
	bool mIsLastWave;					// ���X�g�E�F�[�u��
	int mEnemyNum;						// ���݂̓G�̐�
	bool mIsNext;						// ���̃E�F�[�u�ɍs���邩
	bool mIsExtinction;					// ���݂̃E�F�[�u�̓G����ł�����
};

