#pragma once
#include <unordered_map>
#include <string>
#include <memory>

class BonusUI;

struct BonusStatus
{
	std::string flavorText;
	int bonusPoints;
};

// ボーナス行動一覧
enum class BonusKind
{
	kMaxChargeAttack,
	kCounterAttack,
	kPunchingBag,
	kWalkBonus,
	kDashBonus,
	kNoRunning,
	kSmallPunch,
	kNoDamage,
	kAvoid,
	kGapAttack,
	kMassExtermination,
};

// スコアを記録します
// シングルトン
// 外部ファイル保存させてもあり
// あとUIの管理もやっちゃってます
class ScoreManager
{
private:
	ScoreManager();
	ScoreManager(const ScoreManager&) = delete;
	void operator=(const ScoreManager&) = delete;

public:
	~ScoreManager();

	static ScoreManager& GetInstance();

	// 使うときは毎フレーム呼んでくれ
	void Update();
	void Draw() const;

	// 加点する
	// 加点したやつは画面右に表示される
	// 加点量が多いボーナスは金色で表示されたり
	void SpecialBonusPoints(const BonusKind bonusKind);

	// 敵(蜂)を倒したとき
	// 専用の演出を実行する
	// 死んだ敵から呼ぶのが確実
	void DefeatedEnemy();

	int GetTotalScore() const { return m_score; }
	int GetKilledEnemy() const { return m_killedEnemy; }
	int GetBonusScore() const { return m_bonusScore; }

	// ゲームを開始する時にこれ実行して
	void Reset();
private:
	std::unordered_map < BonusKind, std::shared_ptr<BonusStatus>> m_bonusMap;
	int m_score;
	// 倒した敵の数
	int m_killedEnemy;
	// 追加得点だけ
	int m_bonusScore;

	using UIList_t = std::list<std::shared_ptr<BonusUI>>;
	UIList_t m_bonusUI;

	// UIにあげるハンドル
	int m_fontHandle;
	int m_imageHandle;
};