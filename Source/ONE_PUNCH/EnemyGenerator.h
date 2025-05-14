#pragma once
#include "Actor.h"

class Player;
class Timer;
class Bee;

// 敵をいい感じに生成する
class EnemyGenerator : public Actor
{
public:
	EnemyGenerator();

	void Init(std::weak_ptr<Player> player, std::weak_ptr<Timer> timerRef);

	void Update() override;
	void Draw() const override;

	void CommitMove();
private:
	// 次に敵を出すまでの時間(フレーム)
	int m_spawnSpan;
	std::weak_ptr<Player> m_player;
	std::weak_ptr<Timer> m_timer;
	int m_enemyHandle;

	// スポーンさせた蜂リスト
	// ここに蜂が一体もいなくなれば即スポーン、わんこそば
	std::list<std::weak_ptr<Bee>> m_beeList;

private:
	void SpawnBee();
	// 現在の状況から次に敵を沸かせる時間を返す
	int  GetNextSpawnTime();
};

