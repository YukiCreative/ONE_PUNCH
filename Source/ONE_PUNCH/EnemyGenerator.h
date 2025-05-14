#pragma once
#include "Actor.h"

class Player;
class Timer;
class Bee;

// �G�����������ɐ�������
class EnemyGenerator : public Actor
{
public:
	EnemyGenerator();

	void Init(std::weak_ptr<Player> player, std::weak_ptr<Timer> timerRef);

	void Update() override;
	void Draw() const override;

	void CommitMove();
private:
	// ���ɓG���o���܂ł̎���(�t���[��)
	int m_spawnSpan;
	std::weak_ptr<Player> m_player;
	std::weak_ptr<Timer> m_timer;
	int m_enemyHandle;

	// �X�|�[���������I���X�g
	// �����ɖI����̂����Ȃ��Ȃ�Α��X�|�[���A��񂱂���
	std::list<std::weak_ptr<Bee>> m_beeList;

private:
	void SpawnBee();
	// ���݂̏󋵂��玟�ɓG�𕦂����鎞�Ԃ�Ԃ�
	int  GetNextSpawnTime();
};

