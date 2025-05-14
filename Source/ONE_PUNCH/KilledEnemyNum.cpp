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

	// �t�H���g
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
	// �G�����񂾂��𖈃t���[���m�F����
	const int killedEnemy = ScoreManager::GetInstance().GetKilledEnemy();
	// ���������Ă���΁A���̃t���[���ɓ|����
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
	// �摜���k��
	m_image->ExpandGraph(1.0f - static_cast<float>(m_stateFrameCount) * kExpandDecreaseRate);

	// �G�����񂾂��𖈃t���[���m�F����
	const int killedEnemy = ScoreManager::GetInstance().GetKilledEnemy();
	// ���������Ă���΁A���̃t���[���ɓ|����
	const int increase = killedEnemy - m_killedEnemyNum;
	if (increase)
	{
		OnIncrease();
		m_killedEnemyNum = killedEnemy;
		return;
	}

	// ���ɖ߂�����Normal��
	if (m_image->ExRate() < kInitImageExRate)
	{
		m_state = &KilledEnemyNum::NormalUpdate;
		m_stateFrameCount = 0;
		return;
	}
}

void KilledEnemyNum::OnIncrease()
{
	// ��Ԃ�ς���
	m_state = &KilledEnemyNum::IncreaseUpdate;
	m_stateFrameCount = 0;
	// �T�C�Y�����Z�b�g
	m_image->SetExRate(kHighlightStartExRate);
}
