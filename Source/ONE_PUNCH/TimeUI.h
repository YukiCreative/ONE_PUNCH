#pragma once
#include "Vector2.h"
#include <memory>
#include "Color.h"

class Timer;

// �������Ԃ�\�����邾��
class TimeUI
{
public:
	TimeUI();
	void Init(const Vector2& initPos, std::weak_ptr<Timer> timerRef);

	~TimeUI();

	// �ꉞ���Ƃ���ł��ʒu��ύX�ł���
	void SetPos(const Vector2& pos) { m_pos = pos; }
	void Draw();
private:
	Vector2 m_pos;
	// ��.�����b
	float m_second;
	std::weak_ptr<Timer> m_timerRef;
	int m_fontHandle;
	Color::ColorCode_t m_color;

private:
	// �^�C�}�[���玞�Ԃ��擾(�b��)
	void GetTime();
};

