#pragma once
#include "Actor.h"
#include "Color.h"
#include "RangeLimitedValue.h"

class Sphere;
class SphereCollider;
class Camera;
class AnimationModel;
class Input;
class EffekseerEffect;
class Sound;

namespace DxLib
{
	struct tagMATRIX;
}

namespace
{
	constexpr int kPlayerMinHp = 0;
	constexpr int kPlayerMaxHp = 10;
	constexpr float kPunchChargeMin = 1.0f;
	constexpr float kPunchChargeMax = 3.0f;
}

class Player : public Actor
{
public:
	Player();
	~Player();

	void Init(std::weak_ptr<Camera> camera);

	void Update() override;
	void Draw() const override;
	void OnCollision(std::shared_ptr<Actor> other) override;
	// 向き
	const Vector3 GetDir() const;
	// モデルの向きの行列
	const DxLib::tagMATRIX GetMatrix() const;
	// 自身の位置をずらす
	// 当たり判定、カメラも動かす
	void CommitMove() override;

private:
	std::shared_ptr<Sphere> m_sphere;
	std::weak_ptr<Camera> m_camera;
	std::shared_ptr<AnimationModel> m_animModel;

	int m_stateFrameCount;
	int m_invincibleCount;
	using HitPoint = RangeLimitedValue<int, kPlayerMinHp, kPlayerMaxHp>;
	HitPoint m_hp;
	// どれぐらいチャージされているか
	using PunchCharge = RangeLimitedValue<float, kPunchChargeMin, kPunchChargeMax>;
	PunchCharge m_punchCharge;

	std::weak_ptr<EffekseerEffect> m_chargeEffect;
	std::weak_ptr<Sound> m_chargeSound;

	// 歩いた距離
	float m_walkLength;
	bool m_isGotWalkBonus;
	// 走った距離
	float m_runLength;
	bool m_isGotRunBonus;
	
private:

	using StateFunc_t = void (Player::*)(Input&);
	StateFunc_t m_state;

	// ヘルパー関数
	void ChangeState(StateFunc_t nextState);
	void ChangeStateIdle();
	void ChangeStateWalk();
	void ChangeStateDash();
	void ChangeStateJump();
	void ChangeStateAir();
	void ChangeStateLanding();
	void ChangeStateDeath();
	void ChangeStateDamage();
	void ChangeStateAttack();
	void ChangeStateCharge();
	void ChangeStateMaxCharge();
	void ChangeStateSharpTurn();

	void CameraRotate(Input& input);
	Vector3 GetMoveValueFromLeftAxis(Input& input, const float moveMagnitude);
	// 攻撃判定の位置をプレイヤーの向きから割り出す
	Vector3 GetPlayerAttackPos() const;
	// プレイヤーを特定の範囲に制限する
	void LimitPlayer();

	void OnDamage();

	// 状態
	void Idle(Input& input);
	void Walk(Input& input);
	void Dash(Input& input);
	void Jump(Input& input);
	void Air(Input& input);
	void Landing(Input& input);
	void Death(Input& input);
	void Damage(Input& input);
	void Attack(Input& input);
	void Charge(Input& input);
	void MaxCharge(Input& input);
	void SharpTurn(Input& input);
};

