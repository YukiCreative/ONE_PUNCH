#pragma once
#include "Scene.h"
#include <memory>
#include "Button.h"

class ButtonSystem;
class Image;

// スコアとかを表示する
class SceneResult : public Scene, public std::enable_shared_from_this<SceneResult>
{
	class KilledEnemy;
	class BonusPoints;
	class TotalScore;

public:
	SceneResult();
	~SceneResult();

	void Init() override;
	void Update() override;
	void Draw() const override;

	void Entry() override;
	void Reave() override;

	void ReturnToTitle();

private:
	using ResultSequence_t = void (SceneResult::*)();
	ResultSequence_t m_update;

	std::shared_ptr<Image> m_background;

	std::shared_ptr<Image> m_resultImage;
	Vector2 m_resultPos;

	std::shared_ptr<Image> m_rank;
	Vector2 m_rankPos;

	std::shared_ptr<KilledEnemy> m_killedEnemy;
	std::shared_ptr<BonusPoints> m_bonusPoints;
	std::shared_ptr<TotalScore> m_totalScore;

	std::shared_ptr<ButtonSystem> m_buttons;

	int m_fontHandle;
	int m_guideTime;

	// フェード、フラグ管理にした方がいい説
	bool m_isFadeIn;
	bool m_isFadeOut;
private:

	void AppearResult();
	void AppearKilledEnemyNum();
	void CountKilledNum();
	void AppearBonusScore();
	void CountBonusScore();
	void AppearTotalScore();
	void CountTotalScore();
	void AppearRank();
	void NormalUpdate();

	class ButtonReturnToTitle : public Button
	{
	public:
		void Init(const Vector2& initPos, std::weak_ptr<SceneResult> result);

		void OnFocused() override;
		void OnDisfocused() override;
		void OnPressed() override;
		void Draw() const override;
	private:
		void FocusedUpdate() override;
		void NormalUpdate() override;
	private:
		std::shared_ptr<Image> m_image;
		std::weak_ptr<SceneResult> m_result;
	};

	class KilledEnemy
	{
	public:
		KilledEnemy();
		~KilledEnemy();

		void Init(const int fontHandle);

		void Update();
		void Draw() const;
		// 数を0から増やす演出
		void Count() { m_counting = true; }
		void Appear() { m_moving = true; }
		bool Counting() const { return m_counting; }
		bool Moving() const { return m_moving; }

	private:
		float m_count;
		int m_maxCount;
		int m_fontHandle;
		bool m_moving;
		bool m_counting;

		Vector2 m_pos;
		std::shared_ptr<Image> m_image;
	};

	class BonusPoints
	{
	public:
		BonusPoints();
		~BonusPoints();

		void Init(const int fontHandle);
		void Update();
		void Draw() const;

		void Appear() { m_isMoving = true; }
		void Count() { m_isCounting = true; }
		bool IsMoving() const {return m_isMoving;}
		bool IsCounting() const { return m_isCounting; }

	private:
		std::shared_ptr<Image> m_image;
		float m_count;
		int m_maxCount;
		Vector2 m_pos;

		bool m_isMoving;
		bool m_isCounting;

		int m_fontHandle;
	};

	class TotalScore
	{
	public:
		TotalScore();
		~TotalScore();

		void Init(const int fontHandle);
		void Update();
		void Draw() const;

		void Appear() { m_isMoving = true; }
		void Count() { m_isCounting = true; }
		bool IsMoving() const { return m_isMoving; }
		bool IsCounting() const { return m_isCounting; }

	private:
		std::shared_ptr<Image> m_image;
		float m_count;
		int m_maxCount;
		Vector2 m_pos;

		bool m_isMoving;
		bool m_isCounting;

		int m_fontHandle;
	};
};

