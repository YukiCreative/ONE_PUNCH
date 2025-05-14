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

	// ��
	const std::string kSpawnSound = "EnemySpawn.mp3";
}

EnemyGenerator::EnemyGenerator() :
	m_spawnSpan(0),
	m_enemyHandle(-1)
{
}

void EnemyGenerator::Init(std::weak_ptr<Player> player, std::weak_ptr<Timer> timerRef)
{
	// �Q�ƕێ�
	m_player = player;
	m_timer = timerRef;

	m_enemyHandle = MV1LoadModel(kEnemyModelPath.c_str());

	// �G���o���āA���o�����Ԃ����߂�
	SpawnBee();
}

void EnemyGenerator::Update()
{
	// ��莞�Ԃ��ƂɖI�𐶐�
	// ��,���p�̓����_��
	// �������Ԃ��߂Â��Ă���قǊԊu���Z���Ȃ�
	// �G����̂����Ȃ��Ȃ��������o��

	// �G���X�g�̊m�F
	std::list<std::weak_ptr<Bee>> aliveList;
	for (auto& bee : m_beeList)
	{
		// ���łɑ��݂��Ȃ������菜��
		if (bee.expired()) continue;

		aliveList.emplace_back(bee);
	}
	m_beeList = aliveList;
	// �����ĒN�����Ȃ��Ȃ����Ƃ��A���Ԃ�҂����ɏo��������
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
	// �ȁ[��ɂ����Ȃ�
}

void EnemyGenerator::CommitMove()
{
	// ���݂������Ȃ��̂ňړ����ł��܂���
}

void EnemyGenerator::SpawnBee()
{
	// �����_���Ȉʒu�Ƀ����_���Ȑ��o��
	const Vector3 groupSpawnPos = Vector3{static_cast<float>(GetRand(kGroupSpawnPosRange*2)) - kGroupSpawnPosRange,
		0, static_cast<float>(GetRand(kGroupSpawnPosRange*2) - kGroupSpawnPosRange)};

	// �ǂꂾ������オ���Ă邩�𐔒l�����ēG�̕������ɔ��f
	int heat = ScoreManager::GetInstance().GetKilledEnemy(); // �|�����G�̐�
	heat += std::clamp(kTimerImpact - m_timer.lock()->GetCountSecond() * 0.5f, 0.0f, 9999.0f); // �c�莞��

	const int spawnNum = kSpawnNumMin + GetRand(kSpawnNumRange + heat*0.1f);
	for (int i = 0; i < spawnNum; ++i)
	{
		auto bee = std::make_shared<Bee>();

		// �ŏ��Ɍ��߂��ʒu����ɂ΂������
		const Vector3 thisSpawnPos = { groupSpawnPos.x + static_cast<float>(GetRand(kSingleSpawnPosRange*2) - kSingleSpawnPosRange),
			0, groupSpawnPos.z + static_cast<float>(GetRand(kSingleSpawnPosRange * 2) - kSingleSpawnPosRange) };

		bee->Init(thisSpawnPos, m_player,m_enemyHandle);
		// �������΂������
		bee->RotateUpVecY(GetRand(314) * 0.01f);
		m_spawnActorList.emplace_back(bee);
		// �����ł��L��
		m_beeList.emplace_back(bee);
	}

	// �o�����Ԃ��Đݒ�
	m_spawnSpan = GetNextSpawnTime();
	// ����炷
	SoundManager::GetInstance().Play(kSpawnSound);
}

int EnemyGenerator::GetNextSpawnTime()
{
	// �l���������v�f
	// ���̐����Ă�G�̐��@�����قǎ��𒷂�����
	// ���܂œ|�����G�̐��@�����قǎ���Z������
	// �c�莞�ԁ@�@�@�@�@�@�����قǎ��𒷂�����

	int heat = ScoreManager::GetInstance().GetKilledEnemy();

	int spawnTime = kSpawnTimeMin + std::clamp(GetRand(kSpawnTimeRange - heat*10), 0, 99999);


	return spawnTime;
}
