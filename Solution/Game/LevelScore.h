#pragma once
struct LevelScore
{
	LevelScore() : myTotalEnemies(0), myKilledEnemies(0), myTotalOptional(0), myCompletedOptional(0)
		, myTotalShotsFired(0), myShotsHit(0){}
	int myTotalEnemies;
	int myKilledEnemies;
	int myTotalOptional;
	int myCompletedOptional;
	int myTotalShotsFired;
	int myShotsHit;
};
