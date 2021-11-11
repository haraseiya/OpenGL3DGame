#pragma once

#define SCORE_MANAGER ScoreManager::GetInstance();

class ScoreManager
{
public:
	static ScoreManager* GetInstance() { return mScoreManager; }

	void AddScore(int score);
	void ScoreUpdate();
	int GetSumScore() { return mSumScore; }

	static void CreateInstance();
	static void RemoveInstance();

private:
	ScoreManager() {}
	~ScoreManager(){}

	static ScoreManager* mScoreManager;
	int mSumScore;
	int mHighScore;
};

