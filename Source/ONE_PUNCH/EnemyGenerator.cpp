#include "EnemyGenerator.h"
#include "Bee.h"
#include <DxLib.h>
#include <string>
#include "ScoreManager.h"
#include "Timer.h"
#include "SoundManager.h"

namespace
{
	constexpr int kSpawnTimeRange = 5 * 60;
	constexpr int kSpawnTimeMin   = 4 * 60;

	constexpr int kSpawnNumMin = 1;
	constexpr int kSpawnNumRange = 3;

	constexpr int kGroupSpawnPosRange = 2000;
	constexpr int kSingleSpawnPosRange = 500;

	const std::string kEnemyModelPath = "Data/Model/Bee.mv1";

	constexpr int kTimerImpact = 30;

	// 音
	const std::string kSpawnSound = "EnemySpawn.mp3";
}

EnemyGenerator::EnemyGenerator() :
	m_spawnSpan(0),
	m_enemyHandle(-1)
{
}

void EnemyGenerator::Init(std::weak_ptr<Player> player, std::weak_ptr<Timer> timerRef)
{
	// 参照保持
	m_player = player;
	m_timer = timerRef;

	m_enemyHandle = MV1LoadModel(kEnemyModelPath.c_str());

	// 敵を出して、次出す時間を決める
	SpawnBee();
}

void EnemyGenerator::Update()
{
	// 一定時間ごとに蜂を生成
	// 数,方角はランダム
	// 制限時間が近づいてくるほど間隔が短くなる
	// 敵が一体も居なくなった時も出す

	// 敵リストの確認
	std::list<std::weak_ptr<Bee>> aliveList;
	for (auto& bee : m_beeList)
	{
		// すでに存在しないやつを取り除く
		if (bee.expired()) continue;

		aliveList.emplace_back(bee);
	}
	m_beeList = aliveList;
	// そして誰もいなくなったとき、時間を待たずに出現させる
	if (m_beeList.empty())
	{
		SpawnBee();
		return;
	}

	--m_spawnSpan;
	if (m_spawnSpan <= 0)
	{
		SpawnBee();
	}
}

void EnemyGenerator::Draw() const
{
	// なーんにもしない
}

void EnemyGenerator::CommitMove()
{
	// 存在を持たないので移動もできません
}

void EnemyGenerator::SpawnBee()
{
	// ランダムな位置にランダムな数出す
	const Vector3 groupSpawnPos = Vector3{static_cast<float>(GetRand(kGroupSpawnPosRange*2)) - kGroupSpawnPosRange,
		0, static_cast<float>(GetRand(kGroupSpawnPosRange*2) - kGroupSpawnPosRange)};

	// どれだけ盛り上がってるかを数値化して敵の沸く数に反映
	int heat = ScoreManager::GetInstance().GetKilledEnemy(); // 倒した敵の数
	heat += std::clamp(kTimerImpact - m_timer.lock()->GetCountSecond() * 0.5f, 0.0f, 9999.0f); // 残り時間

	const int spawnNum = kSpawnNumMin + GetRand(kSpawnNumRange + heat*0.1f);
	for (int i = 0; i < spawnNum; ++i)
	{
		auto bee = std::make_shared<Bee>();

		// 最初に決めた位置を基準にばらつかせる
		const Vector3 thisSpawnPos = { groupSpawnPos.x + static_cast<float>(GetRand(kSingleSpawnPosRange*2) - kSingleSpawnPosRange),
			0, groupSpawnPos.z + static_cast<float>(GetRand(kSingleSpawnPosRange * 2) - kSingleSpawnPosRange) };

		bee->Init(thisSpawnPos, m_player,m_enemyHandle);
		// 向きをばらつかせる
		bee->RotateUpVecY(GetRand(314) * 0.01f);
		m_spawnActorList.emplace_back(bee);
		// 自分でも記憶
		m_beeList.emplace_back(bee);
	}

	// 出現時間を再設定
	m_spawnSpan = GetNextSpawnTime();
	// 音を鳴らす
	SoundManager::GetInstance().Play(kSpawnSound);
}

int EnemyGenerator::GetNextSpawnTime()
{
	// 考慮したい要素
	// 今の生きてる敵の数　多いほど次を長くする
	// 今まで倒した敵の数　多いほど次を短くする
	// 残り時間　　　　　　多いほど次を長くする

	int heat = ScoreManager::GetInstance().GetKilledEnemy();

	int spawnTime = kSpawnTimeMin + std::clamp(GetRand(kSpawnTimeRange - heat*10), 0, 99999);


	return spawnTime;
}
