#pragma once
#include "Actor.h"

class SphereCollider;

// �v���C���[�̍U������N���X
// ���ȋ��̑O��ł�
class PlayerAttackRange : public Actor
{
public:
	PlayerAttackRange();
	void Init(const int damage, int lifeTime, const Vector3& initPos, const float radius);

	void SetPos(const Vector3& setPos) { m_pos = setPos; }
	const int GetDamage() const { return m_damage; }

	void Update() override;
	void Draw() const override;

	void CommitMove() override;

	void OnCollision(std::shared_ptr<Actor>) override;

private:
	int m_damage;
	int m_lifeTime;
	int m_hitEnemyNum;
};

