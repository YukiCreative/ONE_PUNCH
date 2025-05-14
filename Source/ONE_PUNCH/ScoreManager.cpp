#include "ScoreManager.h"
#include <DxLib.h>
#include "BonusUI.h"
#include "ImageStore.h"

namespace
{
	constexpr int kEnemyScore = 5000;
	// 画像
	const std::string kImageName = "BonusBoard.png";
	// フォント
	const std::string kFontName = "HGP明朝E 標準";
	constexpr int kFontSize = 20;
	constexpr int kFontThick = 1;

	constexpr size_t kMaxUINum = 10;
}

ScoreManager::ScoreManager() :
	m_score(0),
	m_killedEnemy(0),
	m_fontHandle(-1),
	m_imageHandle(-1),
	m_bonusScore(0)
{
	// ハンドルの取得
	m_fontHandle = CreateFontToHandle(kFontName.c_str(), kFontSize, kFontThick, DX_FONTTYPE_EDGE);
	m_imageHandle = ImageStore::GetInstance().GetGraph(kImageName);

	// Mapの設定
	m_bonusMap[BonusKind::kAvoid] =
	{
		std::make_shared<BonusStatus>("攻撃を回避", 250)
	};
	m_bonusMap[BonusKind::kCounterAttack] =
	{
		std::make_shared<BonusStatus>("カウンターアタック", 250)
	};
	m_bonusMap[BonusKind::kGapAttack] =
	{
		std::make_shared<BonusStatus>("ボディががら空きだぜ", 250)
	};
	m_bonusMap[BonusKind::kMassExtermination] =
	{
		std::make_shared<BonusStatus>("一騎当千", 1000)
	};
	m_bonusMap[BonusKind::kNoDamage] =
	{
		std::make_shared<BonusStatus>("ノーダメージ", 10000)
	};
	m_bonusMap[BonusKind::kNoRunning] =
	{
		std::make_shared<BonusStatus>("座して待つ", 500)
	};
	m_bonusMap[BonusKind::kPunchingBag] =
	{
		std::make_shared<BonusStatus>("サンドバッグ", 3000)
	};
	m_bonusMap[BonusKind::kSmallPunch] =
	{
		std::make_shared<BonusStatus>("小パン連打", 250)
	};
	m_bonusMap[BonusKind::kWalkBonus] =
	{
		std::make_shared<BonusStatus>("てくてく", 1000)
	};
	m_bonusMap[BonusKind::kDashBonus] =
	{
		std::make_shared<BonusStatus>("エッホエッホ", 1000)
	};
	m_bonusMap[BonusKind::kMaxChargeAttack] =
	{
		std::make_shared<BonusStatus>("ONE PUNCH", 500)
	};
}

ScoreManager::~ScoreManager()
{
	DeleteFontToHandle(m_fontHandle);
	DeleteGraph(m_imageHandle);
}

ScoreManager& ScoreManager::GetInstance()
{
	static ScoreManager instance;
	return instance;
}

void ScoreManager::Update()
{
	UIList_t appearUI;
	for (auto& UI : m_bonusUI)
	{
		UI->Update();
		if (UI->IsDeleted()) continue;
		appearUI.emplace_back(UI);
	}
	m_bonusUI = appearUI;
}

void ScoreManager::Draw() const
{
	for (auto& UI : m_bonusUI)
	{
		UI->Draw();
	}
}

void ScoreManager::SpecialBonusPoints(const BonusKind bonusKind)
{
	// 今あるやつを一段下に
	for (auto& UI : m_bonusUI)
	{
		UI->ShiftDown();
	}

	auto& bonusStatus = m_bonusMap[bonusKind];
	// 得点を加算
	m_score += bonusStatus->bonusPoints;
	m_bonusScore += bonusStatus->bonusPoints;
	// UIを生成
	auto addUI = std::make_shared<BonusUI>();
	addUI->Init(bonusStatus, m_fontHandle, m_imageHandle);
	m_bonusUI.emplace_back(addUI);

	// もし表示されているUIの数が最大数を超えたら、
	if (m_bonusUI.size() > kMaxUINum)
	{
		// 退場していない一番古いUIを退場させる
		for (auto it = m_bonusUI.begin(); it != m_bonusUI.end(); ++it)
		{
			auto& instance = (*it);
			if (instance->IsDisappeared()) continue;

			instance->Disappear();
			break;
		}
	}
}

void ScoreManager::DefeatedEnemy()
{
	++m_killedEnemy;
	m_score += kEnemyScore;
}

void ScoreManager::Reset()
{
	m_bonusUI.clear();
	m_killedEnemy = 0;
	m_score = 0;
	m_bonusScore = 0;
}
