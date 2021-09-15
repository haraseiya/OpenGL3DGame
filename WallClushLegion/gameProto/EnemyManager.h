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

	void CreateFirstWave();
	void CreateWave();
	void Update(float deltaTime);

	bool GetWaveFinishFlag() { return mIsLastWave; }
	bool GetEnemyExtinction();

private:
	GameObject* mTarget;									// �^�[�Q�b�g����I�u�W�F�N�g

	std::vector<std::vector<EnemyBase*>> mEnemyWaveList;	// �G�E�F�[�u���X�g
	std::vector<EnemyBase*> mFirstWave;					// ���w
	std::vector<EnemyBase*> mSecondWave;					// ���w
	std::vector<EnemyBase*> mThirdWave;					// ��O�w

	static const int mMaxEnemyNum;				// �G�̍ő吔
	float mTime;								// �^�C�}�[
	int mWaveCount;								// �E�F�[�u�̃J�E���g
	bool mIsLastWave;							// ���X�g�E�F�[�u��
	int mEnemyNum;								// ���݂̓G�̐�
	bool mIsNext;								// ���̃E�F�[�u�ɍs���邩
	bool mIsExtinction;							// ���݂̃E�F�[�u�̓G����ł�����
};

