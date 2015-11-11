#pragma once
struct LevelScore
{
	LevelScore() : myTotalEnemies(0), myKilledEnemies(0), myTotalOptional(0), myCompletedOptional(0)
		, myTotalShotsFired(0), myShotsHit(0), myLevel(-1){}
	int myTotalEnemies;
	int myKilledEnemies;
	int myTotalOptional;
	int myCompletedOptional;
	int myTotalShotsFired;
	int myShotsHit;
	int myLevel;
};

struct SaveScore
{
	SaveScore() : myStars(0), myCompletedOptional(0), myTotalOptional(0) {}
	int myStars;
	int myCompletedOptional;
	int myTotalOptional;
};