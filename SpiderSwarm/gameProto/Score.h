#pragma once

class Score
{
public:
	Score();
	~Score();

	void AddScore(int score);
	void ScoreUpdate();

private:
	int mSumScore;
	int mHighScore;
};

