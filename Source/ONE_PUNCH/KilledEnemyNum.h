#pragma once
#include "Vector2.h"
#include <memory>

class ScoreManager;
class Image;

// 倒した敵の数を表示します
class KilledEnemyNum
{
public:
	KilledEnemyNum();
	void Init(const Vector2& initPos);

	// 演出を行うので
	void Update();
	void Draw() const;
private:
	Vector2 m_pos;
	int m_killedEnemyNum;
	std::shared_ptr<Image> m_image;
	int m_fontHandle;

private:
	using State_t = void (KilledEnemyNum::*)();
	State_t m_state;
	int m_stateFrameCount;

	// 何もしないともいう
	void NormalUpdate();
	// カウントが増えたとき、強調する
	void IncreaseUpdate();

	void OnIncrease();
};