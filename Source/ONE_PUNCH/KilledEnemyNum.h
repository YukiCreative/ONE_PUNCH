#pragma once
#include "Vector2.h"
#include <memory>

class ScoreManager;
class Image;

// �|�����G�̐���\�����܂�
class KilledEnemyNum
{
public:
	KilledEnemyNum();
	void Init(const Vector2& initPos);

	// ���o���s���̂�
	void Update();
	void Draw() const;
private:
	Vector2 m_pos;
	int m_killedEnemyNum;
	std::shared_ptr<Image> m_image;
	int m_fontHandle;

private:
	using State_t = void (KilledEnemyNum::*)();
	State_t m_state;
	int m_stateFrameCount;

	// �������Ȃ��Ƃ�����
	void NormalUpdate();
	// �J�E���g���������Ƃ��A��������
	void IncreaseUpdate();

	void OnIncrease();
};