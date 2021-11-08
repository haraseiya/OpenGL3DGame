#pragma once
#include "Singleton.h"
#include <vector>
#include "Math.h"

class GameObject;
class EnemyBase;
class EnemySpawner;
class WeakEnemy;
class StrongEnemy;
class BossEnemy;

class EnemyManager /*: public Singleton<EnemyManager>*/
{
public:
	//friend class Singleton<EnemyManager>;	// Singleton�ł̃C���X�^���X�쐬�͋���

	EnemyManager(GameObject* target);
	virtual ~EnemyManager();

	void CreateFirstWave();			// �ŏ��̃E�F�[�u���쐬
	void CreateWave(int waveCount);	// �E�F�[�u�̍쐬
	void RemoveDeadEnemy();			// ���񂾓G��j��
	void Update(float deltaTime);	// �X�V����

	bool GetWaveFinishFlag() { return mIsLastWave; }	// �I���t���O�̎擾
	bool GetEnemyExtinction();							// �G����ł������ǂ���

	int GetWaveCount() { return mWaveCount; }
	EnemyBase* GetNearestEnemy(std::vector<EnemyBase*> enemys);	// ��ԋ߂��G

private:
	GameObject* mTarget;				// �^�[�Q�b�g����I�u�W�F�N�g

	EnemyBase* mWeakEnemyPrototype;		
	EnemySpawner* mWeakEnemySpawner;
	EnemyBase* mStrongEnemyPrototype;

	EnemySpawner* mStrongEnemySpawner;
	EnemyBase* mBossEnemyPrototype;
	EnemySpawner* mBossEnemySpawner;

	std::vector<std::vector<EnemyBase*>> mEnemyWaveList;	// �G�E�F�[�u���X�g
	std::vector<EnemyBase*> mEnemyWave0;					// �`���[�g���A���p�E�F�[�u
	std::vector<EnemyBase*> mEnemyWave1;					// �E�F�[�u1
	std::vector<EnemyBase*> mEnemyWave2;					// �E�F�[�u2
	std::vector<EnemyBase*> mEnemyWave3;					// �E�F�[�u3
	std::vector<EnemyBase*> mEnemyWave4;					// �E�F�[�u4

	Vector3 mOffset;					// �G�ʒu�̃I�t�Z�b�g

	static const int mMaxEnemyWave;
	static const int mMaxEnemyNum;		// �G�̍ő吔
	static const int mMaxWeakEnemy;		// �G���G�ő吔
	static const int mMaxStrongEnemy;	// ���G�ő吔
	static const int mMaxBossEnemy;		// �{�X�G�ő吔

	static const float mRandomRangeMinX;
	static const float mRandomRangeMaxX;
	static const float mRandomRangeMinY;
	static const float mRandomRangeMaxY;

	float mTime;						// �^�C�}�[
	int mWaveCount;						// �E�F�[�u�̃J�E���g
	bool mIsLastWave;					// ���X�g�E�F�[�u��
	int mEnemyNum;						// ���݂̓G�̐�
	bool mIsNext;						// ���̃E�F�[�u�ɍs���邩
	bool mIsExtinction;					// ���݂̃E�F�[�u�̓G����ł�����
	float mDistance;
};

