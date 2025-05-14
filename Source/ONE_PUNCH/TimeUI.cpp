#include "TimeUI.h"
#include <DxLib.h>
#include "Timer.h"
#include <string>

namespace
{
	const std::string kFontName = "Bernard MT Condensed";
	constexpr int kFontSize = 50;
	constexpr int kFontThick = 1;
	// ���ɕ`�悷�锖���̕`��Ɏg���萔
	constexpr int kBackBoradWidth = 140;
	constexpr int kBackBoradHeight = 50;

	constexpr float kRedTime = 10.0f;
	// ���F�ł͂Ȃ��@���������̐�
	constexpr unsigned int kNiceRed = 0xee3311;
	const std::string kFormatText = "Limit:%.1fs";
}

TimeUI::TimeUI() :
	m_color(0),
	m_fontHandle(-1),
	m_second(0)
{
}

void TimeUI::Init(const Vector2& initPos, std::weak_ptr<Timer> timerRef)
{
	m_pos = initPos;
	m_timerRef = timerRef;
	m_fontHandle = CreateFontToHandle(kFontName.c_str(), kFontSize, kFontThick, DX_FONTTYPE_EDGE);
	m_color = Color::White;
}

TimeUI::~TimeUI()
{
	DeleteFontToHandle(m_fontHandle);
}

void TimeUI::Draw()
{
	GetTime();
	const int length = GetDrawStringWidthToHandle(kFormatText.c_str(), static_cast<int>(kFormatText.length()), m_fontHandle, false);
	SetDrawBlendMode(DX_BLENDMODE_MULA, 100);
	DrawBox(static_cast<int>(m_pos.x - kBackBoradWidth), 
		static_cast<int>(m_pos.y), static_cast<int>(m_pos.x + kBackBoradWidth),
		static_cast<int>(m_pos.y + kBackBoradHeight), Color::Black, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	DrawFormatStringToHandle(static_cast<int>(m_pos.x - length*0.5f), static_cast<int>(m_pos.y), m_color, m_fontHandle, kFormatText.c_str(), m_second);
}

void TimeUI::GetTime()
{
	if (m_timerRef.expired()) return;
	m_second = m_timerRef.lock()->GetCountSecond();

	// ���Ԃ����Ȃ��Ȃ�����F��ς�����
	if (m_second < kRedTime)
	{
		m_color = kNiceRed;
	}
}
