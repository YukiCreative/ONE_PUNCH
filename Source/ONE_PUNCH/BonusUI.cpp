#include "BonusUI.h"
#include "Image.h"
#include "ScoreManager.h"
#include <DxLib.h>

namespace
{
	const Vector2 kInitPos = {1800, 300};
	const Vector2 kAppearPos = {1170, 300};
	const Vector2 kShiftDown = {0, 20};
	const Vector2 kDisappear = {800, 0};
	constexpr float kLerpStrength = 0.1f;
	constexpr float kImageSize = 0.5f;
	constexpr float kDeleteThreshold = 1900.0f;
	constexpr float kTextOffsetY = -20.0f;
	constexpr float kTextOffsetX = 15.0f;
	constexpr float kTextLineOffset = 25.0f;
	constexpr float kTextLengthOffset = 0.4f;
}

BonusUI::BonusUI() :
	m_fontHandle(-1),
	m_stateFrameCount(0),
	m_isDisappeared(false),
	m_isDeleted(false),
	m_textLength(0)
{
}

void BonusUI::Init(const std::weak_ptr<BonusStatus> status, const int fontHandle, const int imageHandle)
{
	m_fontHandle = fontHandle;
	m_image = std::make_shared<Image>();
	m_image->Init(imageHandle);
	m_image->SetExRate(kImageSize);
	m_bonusStatus = status;
	m_pos = kInitPos;
	m_targetPos = kAppearPos;
	const std::string text = m_bonusStatus.lock()->flavorText;
	m_textLength = GetDrawStringWidthToHandle(text.c_str(), text.length(), m_fontHandle);
}

void BonusUI::Update()
{
	// 常にtargetPosに向かってlerpで移動
	m_pos.LerpMyself(m_targetPos, kLerpStrength);

	// 画面外に出たら消える
	if (m_pos.x > kDeleteThreshold)
	{
		m_isDeleted = true;
	}
}

void BonusUI::Draw() const
{
	m_image->Draw(m_pos);
	const std::string text = m_bonusStatus.lock()->flavorText.c_str();
	float offsetY = kTextOffsetY;
	int char_size = 0;
	int charNum = 0;
	// 一文字ずつ描画
	for (auto it = text.begin(); it != text.end(); it += char_size)
	{
		unsigned char data = *it;
		if (data < 0x80)      char_size = 1;
		else if (data < 0xE0) char_size = 2;
		else if (data < 0xF0) char_size = 3;
		else                  char_size = 4;

		auto moji = text.substr(std::distance(text.begin(), it), char_size);

		DrawStringToHandle(m_pos.x - m_textLength* kTextLengthOffset + charNum*kTextOffsetX, m_pos.y + offsetY, moji.c_str(), 0xffffff, m_fontHandle);
		++charNum;
	}
	// 下に得点を表示
	const std::string pointsText = std::to_string(m_bonusStatus.lock()->bonusPoints);
	const int pointsLength = GetDrawStringWidthToHandle(pointsText.c_str(), pointsText.length(), m_fontHandle);
	DrawFormatStringToHandle(m_pos.x-pointsLength+25, m_pos.y+5, 0xffffff, m_fontHandle, pointsText.c_str());
	DrawStringToHandle(m_pos.x+30, m_pos.y+5, "points", 0xffffff, m_fontHandle);
}

void BonusUI::ShiftDown()
{
	m_targetPos += kShiftDown;
}

void BonusUI::Disappear()
{
	m_isDisappeared = true;
	m_targetPos += kDisappear;
}