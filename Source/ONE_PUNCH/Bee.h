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

// �G
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

	// �U����Ԃ��ǂ���
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
	// ����̍U���������������ǂ���
	// �U�����[�V�����������g��Ȃ�
	bool m_isHitAttack;

	std::weak_ptr<Player> m_player;
private:
	// ���
	void Idle();
	void Chase();
	// �v���C���[�̎�������
	void Surround();
	void Damage();
	void Death();
	void Attack();
	// �v���C���[���U����������Ƃ�
	void Stun();
	// �C���Ԃ��畜�A����Ƃ�
	void Recovery();
	// �U�����v���C���[�ɓ����������̃��[�V����
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
	// ���̈ړ����x����A�O���G�t�F�N�g�̐F���v�Z
	void ChangeAfteImageColorFromVel();
	Vector3 GetDir() const;
};

