#include "SceneResult.h"
#include "ButtonSystem.h"
#include "SceneController.h"
#include "SceneTitle.h"
#include <DxLib.h>
#include "Game.h"
#include "ScoreManager.h"
#include "Music.h"
#include "SoundManager.h"

namespace
{
	const Vector2 kButtonInitPos = { Game::kScreenHalfWidth, Game::kScreenHalfHeight };

	// 画像
	const std::string kResultImage = "Result.png";
	constexpr float kResultSize = 0.4f;
	const Vector2 kResultInitPos = {-500, 100};
	const Vector2 kResultTargetPos = {300, 100};
	constexpr float kResultSpeed = 0.1f;
	const  std::string kBackground = "Result_Background.png";

	const std::string kRankD = "Rank_D.png";
	const std::string kRankC = "Rank_C.png";
	const std::string kRankB = "Rank_B.png";
	const std::string kRankA = "Rank_A.png";
	const std::string kRankS = "Rank_S.png";

	constexpr int kDRank = 0;
	constexpr int kCRank = 100000;
	constexpr int kBRank = 150000;
	constexpr int kARank = 200000;
	constexpr int kSRank = 230000;
	//constexpr int kSSRank = 260000;
	//constexpr int kSSSRank = 290000;

	const Vector2 kRankPos = {900, 350};
	constexpr float kRankImageExpandSpeed = 0.01f;

	// フォント
	const std::string kFontName = "Bernard MT Condensed";
	constexpr int kFontSize = 50;
	constexpr int kFontThick = 1;

	const Vector2 kGuidePos = {1000, 700};
	constexpr int kGuideAnimInterval = 20;

	// 音
	const std::string kResultBGM = "Data/Music/Result.wav";
	const std::string kSubmitSound = "Submit.mp3";
}

SceneResult::SceneResult() :
	m_update(&SceneResult::AppearResult),
	m_fontHandle(-1),
	m_guideTime(0),
	m_isFadeIn(true),
	m_isFadeOut(false)
{
}

SceneResult::~SceneResult()
{
	DeleteFontToHandle(m_fontHandle);
}

void SceneResult::Init()
{
	m_fontHandle = CreateFontToHandle(kFontName.c_str(), kFontSize, kFontThick, DX_FONTTYPE_EDGE);

	m_buttons = std::make_shared<ButtonSystem>();
	auto buttonReturnTitle = std::make_shared<ButtonReturnToTitle>();
	buttonReturnTitle->Init(kButtonInitPos, weak_from_this());
	m_buttons->AddButton(buttonReturnTitle);
	m_buttons->SetButtonFocus(buttonReturnTitle);

	m_resultImage = std::make_shared<Image>();
	m_resultImage->Init(kResultImage);
	m_resultImage->SetExRate(kResultSize);
	m_resultPos = kResultInitPos;

	m_killedEnemy = std::make_shared<KilledEnemy>();
	m_killedEnemy->Init(m_fontHandle);
	m_bonusPoints = std::make_shared<BonusPoints>();
	m_bonusPoints->Init(m_fontHandle);
	m_totalScore = std::make_shared<TotalScore>();
	m_totalScore->Init(m_fontHandle);

	// スコアに応じて、画像を変える
	m_rank = std::make_shared<Image>();
	const int totalScore = ScoreManager::GetInstance().GetTotalScore();
	std::string image;
	if (totalScore >= kSRank) image = kRankS;
	else if (totalScore >= kARank) image = kRankA;
	else if (totalScore >= kBRank) image = kRankB;
	else if (totalScore >= kCRank) image = kRankC;
	else image = kRankD;
	m_rank->Init(image);
	// とりあえず隠しておく
	m_rank->SetExRate(0);

	// 背景
	m_background = std::make_shared<Image>();
	m_background->Init(kBackground);
}

void SceneResult::Update()
{
	++m_guideTime;
	if (m_guideTime > kGuideAnimInterval * 4)
	{
		m_guideTime = 0;
	}

	// フェード
	if (m_isFadeIn)
	{
		Scene::FadeInUpdate();
		if (m_fadePanel->GetBlendParam() <= 0)
		{
			m_isFadeIn = false;
		}
	}
	if (m_isFadeOut)
	{
		Scene::FadeOutUpdate();
		if (m_fadePanel->GetBlendParam() >= 255)
		{
			auto title = std::make_shared<SceneTitle>();
			title->Init();
			SceneController::GetInstance().ChangeScene(title);
			return;
		}
	}

	(this->*m_update)();
	// シーンが切り替わった可能性があるので、この後にはコードを書かない！
}

void SceneResult::Draw() const
{
	m_background->Draw({Game::kScreenHalfWidth, Game::kScreenHalfHeight});
	m_buttons->Draw();
	m_resultImage->Draw(m_resultPos);
	m_killedEnemy->Draw();
	m_bonusPoints->Draw();
	m_totalScore->Draw();
	m_rank->Draw(kRankPos);

	// ちょっと雑だけど
	if (m_update == &SceneResult::NormalUpdate)
	{
		if (m_guideTime < kGuideAnimInterval)
			DrawString(kGuidePos.x, kGuidePos.y, "Aボタンでタイトルへ戻ります", 0xffffff);
		else if (m_guideTime < kGuideAnimInterval*2)
			DrawString(kGuidePos.x, kGuidePos.y, "Aボタンでタイトルへ戻ります.", 0xffffff);
		else if (m_guideTime < kGuideAnimInterval*3)
			DrawString(kGuidePos.x, kGuidePos.y, "Aボタンでタイトルへ戻ります..", 0xffffff);
		else
			DrawString(kGuidePos.x, kGuidePos.y, "Aボタンでタイトルへ戻ります...", 0xffffff);
	}

	m_fadePanel->Draw({Game::kScreenHalfWidth, Game::kScreenHalfHeight});
}

void SceneResult::Entry()
{
	Music::GetInstance().Play(kResultBGM);
	m_fadePanel->SetImageBlendPal(255);
}

void SceneResult::Reave()
{
}

void SceneResult::ReturnToTitle()
{
	m_isFadeOut = true;
}

void SceneResult::AppearResult()
{
	// 「リザルト」の画像を出す
	m_resultPos.LerpMyself(kResultTargetPos, kResultSpeed);

	// もし指定の位置に来たなら
	if ((m_resultPos - kResultTargetPos).SqrMagnitude() < 0.1f)
	{
		m_resultPos = kResultTargetPos;
		// 次は敵を倒した数を表示する
		m_update = &SceneResult::AppearKilledEnemyNum;
		m_killedEnemy->Appear();
		return;
	}
}

void SceneResult::AppearKilledEnemyNum()
{
	m_killedEnemy->Update();

	if (!m_killedEnemy->Moving())
	{
		m_update = &SceneResult::CountKilledNum;
		m_killedEnemy->Count();
		return;
	}
}

void SceneResult::CountKilledNum()
{
	m_killedEnemy->Update();

	if (!m_killedEnemy->Counting())
	{
		m_update = &SceneResult::AppearBonusScore;
		m_bonusPoints->Appear();
		return;
	}
}

void SceneResult::AppearBonusScore()
{
	m_bonusPoints->Update();

	if (m_bonusPoints->IsMoving()) return;

	m_update = &SceneResult::CountBonusScore;
	m_bonusPoints->Count();
	return;
}

void SceneResult::CountBonusScore()
{
	m_bonusPoints->Update();

	if (m_bonusPoints->IsCounting()) return;

	m_update = &SceneResult::AppearTotalScore;
	m_totalScore->Appear();
}

void SceneResult::AppearTotalScore()
{
	m_totalScore->Update();

	if (m_totalScore->IsMoving()) return;

	m_update = &SceneResult::CountTotalScore;
	m_totalScore->Count();
	return;
}

void SceneResult::CountTotalScore()
{
	m_totalScore->Update();

	if (m_totalScore->IsCounting()) return;

	m_update = &SceneResult::AppearRank;
}

void SceneResult::AppearRank()
{
	// 等速で拡大
	m_rank->SetExRate(m_rank->ExRate() + kRankImageExpandSpeed);

	// 終わったらNormalUpdateへ
	if (m_rank->ExRate() > 1.5f)
	{
		m_update = &SceneResult::NormalUpdate;
		return;
	}
}

void SceneResult::NormalUpdate()
{
	// いつも通り
	m_buttons->Update();
}

// 以下、ボタンの定義
// ========================================

void SceneResult::ButtonReturnToTitle::Init(const Vector2& initPos, std::weak_ptr<SceneResult> result)
{
	Button::Init(initPos);
	m_result = result;
}

void SceneResult::ButtonReturnToTitle::Draw() const
{
}

void SceneResult::ButtonReturnToTitle::OnFocused()
{
}

void SceneResult::ButtonReturnToTitle::OnDisfocused()
{
}

void SceneResult::ButtonReturnToTitle::OnPressed()
{
	// 音を鳴らす
	SoundManager::GetInstance().Play(kSubmitSound);
	m_result.lock()->ReturnToTitle();
}

void SceneResult::ButtonReturnToTitle::FocusedUpdate()
{
}

void SceneResult::ButtonReturnToTitle::NormalUpdate()
{
}

// ==================================================

namespace
{
	constexpr float kCountSpeed = 0.1f;

	const Vector2 kKilledEnemyInitPos = {-300, 250};
	const Vector2 kKilledEnemyTargetPos = {250, 250};
	const Vector2 kKilledEnemyTextOffset = {75, 0};
	constexpr float kMoveSpeed = 0.1f;

	// 画像
	const std::string kEnemyImage = "BeeImage.png";
	constexpr float kEnemyExRate = 0.2f;
}

SceneResult::KilledEnemy::KilledEnemy() : 
	m_count(0),
	m_maxCount(0),
	m_counting(false),
	m_moving(false),
	m_fontHandle(-1)
{
}

SceneResult::KilledEnemy::~KilledEnemy()
{
}

void SceneResult::KilledEnemy::Init(const int fontHandle)
{
	m_fontHandle = fontHandle;
	m_image = std::make_shared<Image>();
	m_image->Init(kEnemyImage);
	m_image->SetExRate(kEnemyExRate);
	m_maxCount = ScoreManager::GetInstance().GetKilledEnemy();
	m_pos = kKilledEnemyInitPos;
}

void SceneResult::KilledEnemy::Update()
{
	// これくらいならステート要らないかな
	if (m_moving)
	{
		// 特定の位置までlerp
		// lerp便利
		m_pos.LerpMyself(kKilledEnemyTargetPos, kMoveSpeed);

		// 到達したら
		if ((m_pos - kKilledEnemyTargetPos).SqrMagnitude() < 0.1f)
		{
			// これ以上頑張らない
			m_moving = false;
			m_pos = kKilledEnemyTargetPos;
		}
	}
	if (m_counting)
	{
		m_count = std::lerp(m_count, static_cast<float>(m_maxCount), kCountSpeed);

		// 到達したら
		if (m_maxCount - m_count < 0.1f)
		{
			// 止まる
			m_counting = false;
			m_count = m_maxCount;
		}
	}
}

void SceneResult::KilledEnemy::Draw() const
{
	m_image->Draw(m_pos);
	DrawFormatStringToHandle(m_pos.x + kKilledEnemyTextOffset.x, m_pos.y + kKilledEnemyTextOffset.y, 0xffffff, m_fontHandle, "x %d", static_cast<int>(m_count));
}

// =================================================

namespace
{
	constexpr float kBonusMoveSpeed = 0.1f;
	constexpr float kBonusCountSpeed = 0.1f;

	const Vector2 kBonusInitPos = { -500, 400 };
	const Vector2 kBonusTargetPos = { 250, 400 };
	const Vector2 kBonusTextOffset = { 50, 40 };

	const std::string kBonusImage = "Bonus.png";
}

SceneResult::BonusPoints::BonusPoints() :
	m_count(0),
	m_fontHandle(-1),
	m_maxCount(0),
	m_isCounting(false),
	m_isMoving(false)
{
}

SceneResult::BonusPoints::~BonusPoints()
{
}

void SceneResult::BonusPoints::Init(const int fontHandle)
{
	m_pos = kBonusInitPos;
	m_maxCount = ScoreManager::GetInstance().GetBonusScore();
	m_fontHandle = fontHandle;
	m_image = std::make_shared<Image>();
	m_image->Init(kBonusImage);
}

void SceneResult::BonusPoints::Update()
{
	if (m_isMoving)
	{
		m_pos.LerpMyself(kBonusTargetPos, kBonusMoveSpeed);
		if ((m_pos - kBonusTargetPos).SqrMagnitude() < 0.1f)
		{
			m_pos = kBonusTargetPos;
			m_isMoving = false;
		}
	}
	if (m_isCounting)
	{
		m_count = std::lerp(m_count, static_cast<float>(m_maxCount), kBonusCountSpeed);
		if (m_maxCount - m_count < 0.5f)
		{
			m_count = m_maxCount;
			m_isCounting = false;
		}
	}
}

void SceneResult::BonusPoints::Draw() const
{
	m_image->Draw(m_pos);
	DrawFormatStringToHandle(m_pos.x + kBonusTextOffset.x, m_pos.y + kBonusTextOffset.y, 0xffffff, m_fontHandle, "%d pts", static_cast<int>(m_count));
}

// =====================================

namespace
{
	const std::string kTotalImage = "Total.png";
	const Vector2 kTotalInitPos = {-500,550};
	const Vector2 kTotalTargetPos = { 250, 550 };
	const Vector2 kTotalTextOffset = { 50, 40 };
	constexpr float kTotalCountSpeed = 0.1f;
}

SceneResult::TotalScore::TotalScore() :
	m_fontHandle(-1),
	m_isCounting(false),
	m_isMoving(false),
	m_count(0),
	m_maxCount(0)
{
}

SceneResult::TotalScore::~TotalScore()
{
}

void SceneResult::TotalScore::Init(const int fontHandle)
{
	m_pos = kTotalInitPos;
	m_maxCount = ScoreManager::GetInstance().GetTotalScore();
	m_image = std::make_shared<Image>();
	m_image->Init(kTotalImage);
	m_fontHandle = fontHandle;
}

void SceneResult::TotalScore::Update()
{
	// 同じことするだけ
	if (m_isMoving)
	{
		m_pos.LerpMyself(kTotalTargetPos, kBonusMoveSpeed);
		if ((m_pos - kTotalTargetPos).SqrMagnitude() < 0.1f)
		{
			m_pos = kTotalTargetPos;
			m_isMoving = false;
		}
	}
	if (m_isCounting)
	{
		m_count = std::lerp(m_count, static_cast<float>(m_maxCount), kTotalCountSpeed);
		if (m_maxCount - m_count < 0.5f)
		{
			m_count = m_maxCount;
			m_isCounting = false;
		}
	}
}

void SceneResult::TotalScore::Draw() const
{
	m_image->Draw(m_pos);
	DrawFormatStringToHandle(m_pos.x + kTotalTextOffset.x, m_pos.y + kTotalTextOffset.y, 0xffffff, m_fontHandle, "%d pts", static_cast<int>(m_count));
}
