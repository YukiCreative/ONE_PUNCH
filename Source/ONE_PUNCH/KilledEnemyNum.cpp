#include "KilledEnemyNum.h"
#include "Image.h"
#include "ScoreManager.h"
#include <string>

namespace
{
	const std::string kBeeImage = "BeeImage.png";
	constexpr float kInitImageExRate = 0.1f;
	constexpr float kHighlightStartExRate = 0.3f;
	constexpr float kExpandDecreaseRate = 0.001f;
	constexpr int kNumOffsetX = 50;
	constexpr int kNumOffsetY = -20;

	// フォント
	const std::string kFontName = "Bernard MT Condensed";
	constexpr int kFontSize = 50;
	constexpr int kFontThick = 1;
}

KilledEnemyNum::KilledEnemyNum() :
	m_killedEnemyNum(0),
	m_stateFrameCount(0),
	m_state(&KilledEnemyNum::NormalUpdate),
	m_fontHandle(-1)
{
}

void KilledEnemyNum::Init(const Vector2& initPos)
{
	m_pos = initPos;
	m_image = std::make_shared<Image>();
	m_image->Init(kBeeImage);
	m_image->SetExRate(kInitImageExRate);
	m_fontHandle = CreateFontToHandle(kFontName.c_str(), kFontSize, kFontThick, DX_FONTTYPE_EDGE);
}

void KilledEnemyNum::Update()
{
	(this->*m_state)();
	++m_stateFrameCount;
}

void KilledEnemyNum::Draw() const
{
	m_image->Draw(m_pos);
	DrawFormatStringToHandle(static_cast<int>(m_pos.x + kNumOffsetX), static_cast<int>(m_pos.y+kNumOffsetY), 0xffffff, m_fontHandle, "x %d", m_killedEnemyNum);
}

void KilledEnemyNum::NormalUpdate()
{
	// 敵が死んだかを毎フレーム確認する
	const int killedEnemy = ScoreManager::GetInstance().GetKilledEnemy();
	// 数が増えていれば、このフレームに倒した
	const int increase = killedEnemy - m_killedEnemyNum;
	if (increase)
	{
		OnIncrease();
		m_killedEnemyNum = killedEnemy;
		return;
	}
}

void KilledEnemyNum::IncreaseUpdate()
{
	// 画像を縮小
	m_image->ExpandGraph(1.0f - static_cast<float>(m_stateFrameCount) * kExpandDecreaseRate);

	// 敵が死んだかを毎フレーム確認する
	const int killedEnemy = ScoreManager::GetInstance().GetKilledEnemy();
	// 数が増えていれば、このフレームに倒した
	const int increase = killedEnemy - m_killedEnemyNum;
	if (increase)
	{
		OnIncrease();
		m_killedEnemyNum = killedEnemy;
		return;
	}

	// 元に戻ったらNormalへ
	if (m_image->ExRate() < kInitImageExRate)
	{
		m_state = &KilledEnemyNum::NormalUpdate;
		m_stateFrameCount = 0;
		return;
	}
}

void KilledEnemyNum::OnIncrease()
{
	// 状態を変える
	m_state = &KilledEnemyNum::IncreaseUpdate;
	m_stateFrameCount = 0;
	// サイズをリセット
	m_image->SetExRate(kHighlightStartExRate);
}
