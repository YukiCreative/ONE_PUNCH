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

// �{�[�i�X�s���ꗗ
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

// �X�R�A���L�^���܂�
// �V���O���g��
// �O���t�@�C���ۑ������Ă�����
// ����UI�̊Ǘ������������Ă܂�
class ScoreManager
{
private:
	ScoreManager();
	ScoreManager(const ScoreManager&) = delete;
	void operator=(const ScoreManager&) = delete;

public:
	~ScoreManager();

	static ScoreManager& GetInstance();

	// �g���Ƃ��͖��t���[���Ă�ł���
	void Update();
	void Draw() const;

	// ���_����
	// ���_������͉�ʉE�ɕ\�������
	// ���_�ʂ������{�[�i�X�͋��F�ŕ\�����ꂽ��
	void SpecialBonusPoints(const BonusKind bonusKind);

	// �G(�I)��|�����Ƃ�
	// ��p�̉��o�����s����
	// ���񂾓G����ĂԂ̂��m��
	void DefeatedEnemy();

	int GetTotalScore() const { return m_score; }
	int GetKilledEnemy() const { return m_killedEnemy; }
	int GetBonusScore() const { return m_bonusScore; }

	// �Q�[�����J�n���鎞�ɂ�����s����
	void Reset();
private:
	std::unordered_map < BonusKind, std::shared_ptr<BonusStatus>> m_bonusMap;
	int m_score;
	// �|�����G�̐�
	int m_killedEnemy;
	// �ǉ����_����
	int m_bonusScore;

	using UIList_t = std::list<std::shared_ptr<BonusUI>>;
	UIList_t m_bonusUI;

	// UI�ɂ�����n���h��
	int m_fontHandle;
	int m_imageHandle;
};