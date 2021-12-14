#pragma once

class highscore
{
public:
	highscore(void);
	~highscore(void);

	void init();
	void run();

	bool isHighScore(int score);

	void drawEnterScore();
	void drawTable();

private:

	void addHighScore(char* name, int score);

	typedef struct
	{
		char name[4];
		int score;
	}ENTRY;
	
	ENTRY mEntries[10];

	int mEditCurrentPos;
	int mEditCurrentLetter[3];
	int mDebounceTimer;
	int mLastLetter;
	char mEditName[4];
	int mScore;
};


