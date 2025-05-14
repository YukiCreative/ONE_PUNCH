#pragma once
#include "Actor.h"
#include "RangeLimitedValue.h"

class AnimationModel;
class Player;
class PlayerAttackRange;
class EffekseerEffect;

namespace
{
	constexpr int kMinHp = 0;
	constexpr int kMaxHp = 3;
}

// 敵
class Bee final : public Actor
{
public:
	Bee();
	~Bee();

	void Init(const Vector3& initPos, std::weak_ptr<Player> player, const int handle);

	void Update() override;
	void Draw() const override;

	void OnCollision(std::shared_ptr<Actor> other) override;
	void CommitMove() override;

	void RotateUpVecY(const float rad);

	// 攻撃状態かどうか
	bool IsAttacking() const;
	bool IsDamaging() const;
private:
	std::shared_ptr<AnimationModel> m_animModel;
	using StateFunc_t = void (Bee::*)();
	StateFunc_t m_state;
	int m_stateFrameCount;
	using HitPoint = RangeLimitedValue<int, kMinHp, kMaxHp>;
	HitPoint m_hp;
	std::weak_ptr<EffekseerEffect> m_afterImage;
	std::weak_ptr<EffekseerEffect> m_stunEffect;
	int m_surroundFrame;
	float m_surroundRotSpeed;
	// 今回の攻撃が当たったかどうか
	// 攻撃モーション中しか使わない
	bool m_isHitAttack;

	std::weak_ptr<Player> m_player;
private:
	// 状態
	void Idle();
	void Chase();
	// プレイヤーの周りを回る
	void Surround();
	void Damage();
	void Death();
	void Attack();
	// プレイヤーが攻撃を避けたとき
	void Stun();
	// 気絶状態から復帰するとき
	void Recovery();
	// 攻撃がプレイヤーに当たった時のモーション
	void Attacked();

	void ChangeState(StateFunc_t nextState);
	void ChangeStateIdle();
	void ChangeStateChase();
	void ChangeStateSurround();
	void ChangeStateDamage();
	void ChangeStateDeath();
	void ChangeStateAttack();
	void ChangeStateStun();
	void ChangeStateRecovery();
	void ChangeStateAttacked();

	void OnDamage(const std::shared_ptr<PlayerAttackRange>& playerAttack);
	void OnDeath();
	// 今の移動速度から、軌道エフェクトの色を計算
	void ChangeAfteImageColorFromVel();
	Vector3 GetDir() const;
};

