#pragma once
#include <vector>

class GameObject;
class WeakEnemy;
class StrongEnemy;
class BossEnemy;

class EnemyManager
{
public:
	EnemyManager(GameObject* target);
	~EnemyManager();

	void CreateWave();
	void Update(float deltaTime);

	bool GetWaveFinishFlag() { return mIsLastWave; }

private:
	GameObject* mTarget;									// �^�[�Q�b�g����I�u�W�F�N�g

	std::vector<std::vector<GameObject*>> mEnemyWaveList;	// �G�E�F�[�u���X�g
	std::vector<GameObject*> mFirstWave;					// ���w
	std::vector<GameObject*> mSecondWave;					// ���w
	std::vector<GameObject*> mThirdWave;					// ��O�w

	static const int mMaxEnemyNum;
	float mTime;
	int mWaveCount;
	bool mIsLastWave;
	int mEnemyNum;
	bool mIsNext;
};

