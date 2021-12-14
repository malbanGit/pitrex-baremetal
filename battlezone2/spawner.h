#pragma once

class spawner
{
public:
	spawner();
	~spawner();

	void run();
	void reset();

	void addKill();

protected:
	int mKillCount;

	int mMissileVolleyCount;
};

