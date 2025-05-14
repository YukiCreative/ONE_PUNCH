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
	// ����
	const Vector3 GetDir() const;
	// ���f���̌����̍s��
	const DxLib::tagMATRIX GetMatrix() const;
	// ���g�̈ʒu�����炷
	// �����蔻��A�J������������
	void CommitMove() override;

private:
	std::shared_ptr<Sphere> m_sphere;
	std::weak_ptr<Camera> m_camera;
	std::shared_ptr<AnimationModel> m_animModel;

	int m_stateFrameCount;
	int m_invincibleCount;
	using HitPoint = RangeLimitedValue<int, kPlayerMinHp, kPlayerMaxHp>;
	HitPoint m_hp;
	// �ǂꂮ�炢�`���[�W����Ă��邩
	using PunchCharge = RangeLimitedValue<float, kPunchChargeMin, kPunchChargeMax>;
	PunchCharge m_punchCharge;

	std::weak_ptr<EffekseerEffect> m_chargeEffect;
	std::weak_ptr<Sound> m_chargeSound;

	// ����������
	float m_walkLength;
	bool m_isGotWalkBonus;
	// ����������
	float m_runLength;
	bool m_isGotRunBonus;
	
private:

	using StateFunc_t = void (Player::*)(Input&);
	StateFunc_t m_state;

	// �w���p�[�֐�
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
	// �U������̈ʒu���v���C���[�̌������犄��o��
	Vector3 GetPlayerAttackPos() const;
	// �v���C���[�����͈̔͂ɐ�������
	void LimitPlayer();

	void OnDamage();

	// ���
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

