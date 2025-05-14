#include "PlayerAttackRange.h"
#include <DxLib.h>
#include "SphereCollider.h"
#include "Rigid.h"
#include "Collidable.h"
#include "ScoreManager.h"
#include "Bee.h"

namespace
{
	constexpr int kWeight = 1;
	constexpr bool kIsThrough = true;
	constexpr bool kIsStatic = false;

	constexpr int kBonusEnemyNum = 4;
	// この辺は適当でOK
	const PhysicalMaterial kMaterial =
	{
		0.1f, // 地上摩擦
		0.1f, // 空中摩擦
		0.1f, // 反発係数
	};
}

PlayerAttackRange::PlayerAttackRange() :
	m_damage(0),
	m_lifeTime(0),
	m_hitEnemyNum(0)
{
}

void PlayerAttackRange::Init(const int damage, int lifeTime, const Vector3& initPos, const float radius)
{
	m_kind = ActorKind::kPlayerAttack;
	m_damage = damage;
	m_lifeTime = lifeTime;
	m_pos = initPos;
	auto sphereCol = std::make_shared<SphereCollider>();
	sphereCol->Init(initPos, kWeight, kIsThrough, kIsStatic, radius);
	auto rigid = std::make_shared<Rigid>();
	rigid->Init(kMaterial);
	m_collidable = std::make_shared<Collidable>();
	m_collidable->Init(sphereCol, rigid);
}

void PlayerAttackRange::Update()
{
	--m_lifeTime;

	if (m_lifeTime < 0)
	{
		m_isAlive = false;
	}
}

void PlayerAttackRange::Draw() const
{
#if _DEBUG
	m_collidable->GetCol().Draw();
#endif
}

void PlayerAttackRange::CommitMove()
{
}

void PlayerAttackRange::OnCollision(std::shared_ptr<Actor> other)
{
	// 敵に当たったらカウントを増やす
	if (other->GetKind() != ActorKind::kEnemy) return;

	// そいつがすでに攻撃を受けていたらカウントしない
	if (std::dynamic_pointer_cast<Bee>(other)->IsDamaging()) return;

	++m_hitEnemyNum;

	// もし一定値なら、
	if (m_hitEnemyNum != kBonusEnemyNum) return;

	ScoreManager::GetInstance().SpecialBonusPoints(BonusKind::kMassExtermination);
}