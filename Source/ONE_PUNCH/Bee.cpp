#include "AnimationModel.h"
#include "Bee.h"
#include "Collidable.h"
#include "Geometry.h"
#include "Player.h"
#include "PlayerAttackRange.h"
#include "Rigid.h"
#include "SphereCollider.h"
#include <DxLib.h>
#include <string>
#include "ScoreManager.h"
#include "EffectManager.h"
#include "EffekseerEffect.h"
#include <EffekseerForDXLib.h>
#include "SoundManager.h"

namespace
{
	// コライダー定数
	const Vector3 kColOffset = { 0,50,0 };
	constexpr float kColRadius = 50.0f;
	constexpr int kColWeight = 10;
	constexpr bool kColIsThrough = false;
	constexpr bool kColIsStatic = false;

	// アニメーション定数
	const std::string kModelPath      = "Data/Model/Bee.mv1";
	const std::string kIdleAnimName   = "MonsterArmature|Flying";
	const std::string kAttackAnimName = "MonsterArmature|Attack";
	const std::string kDeathAnimName  = "MonsterArmature|Death";
	const std::string kDamageAnimName = "MonsterArmature|HitRecieve";
	const std::string kRecoveryAnimName = "MonsterArmature|Recovery";
	const std::string kStunAnimName = "MonsterArmature|Stun";
	const std::string kPreAttackAnimName = "MonsterArmature|AttackPreliminaryAction";
	constexpr float kAnimSpeed = 30.0f;
	// ゲーム定数
	constexpr float kShiftChaseLength = 2000.0f;
	constexpr float kShiftSurroundLength = 500.0f;
	constexpr int   kSurroundFrameRange = 360;
	constexpr float kSurroundRotateSpeed = 0.003f;
	constexpr float kChaseSpeed       = 0.1f;
	const Vector3   kAttackVec        = { 0, 40, -13 };
	constexpr int   kStunInterval = 600;
	const Vector3   kBlownVec         = {0, 10.0f, 10.0f};
	constexpr int   kDeathScore       = 5000;

	// スポーンエフェクト
	const std::string kSpawnEffectName = "EnemySpawn.efkefc";

	// 吹っ飛ぶ軌道エフェクト定数
	const std::string kAfterImage     = "AfterImage.efkefc";
	constexpr int kAfterImageDefaultR = 255;
	constexpr int kAfterImageDefaultG = 255;
	constexpr int kAfterImageDefaultB = 0;
	constexpr int kAfterImageDefaultA = 255;
	constexpr float kIncreaseColorMult = 3.0f;
	constexpr int kAttackedInterval = 30;

	// 気絶エフェクト定数
	const std::string kStunEffect = "Stun.efkefc";

	// 退場エフェクト
	const std::string kDeathEffect = "EnemyDeath.efkefc";

	const PhysicalMaterial kPhysicalMaterial =
	{
		0.1f,  // 地上摩擦
		0.03f, // 空中摩擦
		0.1f,  // 反発係数
	};

	// 音
	const std::string kDeathSound = "EnemyDeath.mp3";
}

Bee::Bee() :
	m_state(&Bee::Idle),
	m_stateFrameCount(0),
	m_isHitAttack(false),
	m_surroundFrame(0),
	m_surroundRotSpeed(0)
{
}

Bee::~Bee()
{
}

void Bee::Init(const Vector3& initPos, std::weak_ptr<Player> player, const int handle)
{
	m_hp.SetMax();
	m_pos = initPos;
	m_kind = ActorKind::kEnemy;
	m_player = player;
	auto sphereCol = std::make_shared<SphereCollider>();
	sphereCol->Init(m_pos + kColOffset, kColWeight, kColIsThrough, kColIsStatic, kColRadius);

	auto rigid = std::make_shared<Rigid>();
	rigid->Init(kPhysicalMaterial);
	rigid->ChangeStateAir();

	m_collidable = std::make_shared<Collidable>();
	m_collidable->Init(sphereCol, rigid);

	m_animModel = std::make_shared<AnimationModel>();
	m_animModel->Init(handle, kAnimSpeed);
	m_animModel->ChangeAnimation(kIdleAnimName, true);
	m_animModel->Play();

	// エフェクト出す
	EffectManager::GetInstance().GenerateEffect(kSpawnEffectName, m_pos);
}

void Bee::Update()
{
	(this->*m_state)();
	++m_stateFrameCount;

	if (m_pos.y < 0 && m_collidable->GetVel().y < 0)
	{
		m_pos.y = 0;
		m_collidable->SetPos(m_pos);
		m_collidable->GetRigid().StopY();
	}

	m_animModel->Update();
}

void Bee::Draw() const
{
	m_animModel->Draw();

#if _DEBUG
	m_collidable->GetCol().Draw();
#endif
}

void Bee::OnCollision(std::shared_ptr<Actor> other)
{
	if (other->GetKind() == ActorKind::kPlayerAttack)
	{
		auto attack = std::dynamic_pointer_cast<PlayerAttackRange>(other);
		// ダメージを受ける
		OnDamage(attack);
		return;
	}

	// 攻撃中にプレイヤーに接触したなら
	if (other->GetKind() == ActorKind::kPlayer && IsAttacking())
	{
		m_isHitAttack = true;
	}
}

void Bee::CommitMove()
{
	// 移動を確定
	// 物理運動を計算
	m_pos += m_collidable->GetRigid().Update();

	m_animModel->SetPos(m_pos);
	m_collidable->GetCol().SetPos(m_pos + kColOffset);
}

void Bee::Idle()
{
	if (m_player.expired()) return;

	// プレイヤーを探す
	auto player = m_player.lock();
	const Vector3 dist = player->GetPos() - m_pos; // 自分→プレイヤー
	const float distLength = dist.SqrMagnitude();

	// プレイヤーが一定距離に入っていたら追尾
	if (distLength < kShiftChaseLength * kShiftChaseLength)
	{
		ChangeStateChase();
	}
}

void Bee::Chase()
{
	// プレイヤーを追いかける
	// プレイヤーが今自分の右半分にいるかか左半分にいるかで回転方向を変える

	if (m_player.expired())
	{
		ChangeStateIdle();
		return;
	}

	// プレイヤーの方向に向かう
	const Vector3 dir = GetDir();
	const Vector3 thisToPlayer = (m_player.lock()->GetPos() - m_pos);
	const Vector3 unitThisToPlayer = thisToPlayer.GetNormalize();
	const Vector3 cross = dir.Cross(unitThisToPlayer);

	m_animModel->RotateUpVecY(cross.y);
	m_collidable->GetRigid().AddVel(unitThisToPlayer * kChaseSpeed);

	// もし一定距離内に接近したら
	const float distLength = thisToPlayer.SqrMagnitude();
	if (distLength < kShiftSurroundLength * kShiftSurroundLength)
	{
		ChangeStateSurround();
		return;
	}
	// もし一定範囲内から出たら
	// 追尾をやめる
	if (distLength > kShiftChaseLength * kShiftChaseLength)
	{
		ChangeStateIdle();
		return;
	}
}

void Bee::Surround()
{
	// 指定時間たったら攻撃状態に
	if (m_stateFrameCount >= m_surroundFrame)
	{
		ChangeStateAttack();
		return;
	}

	if (m_player.expired()) return;

	// ランダム秒　様子見のような動きをしてほしい
	// プレイヤーの周りをぐるぐる回る
	m_pos = Geometry::GetRotatedPosUpVecY(m_pos, m_player.lock()->GetPos(), m_surroundRotSpeed);
	// プレイヤーを凝視
	const Vector3 dir = GetDir();
	const Vector3 thisToPlayer = (m_player.lock()->GetPos() - m_pos);
	const Vector3 unitThisToPlayer = thisToPlayer.GetNormalize();
	const Vector3 cross = dir.Cross(unitThisToPlayer);

	m_animModel->RotateUpVecY(cross.y);

	// もしプレイヤーとの距離が一定以上離れていれば
	// 再び追尾する
	if (thisToPlayer.SqrMagnitude() > kShiftSurroundLength * kShiftSurroundLength)
	{
		ChangeStateChase();
		return;
	}
}

void Bee::Damage()
{
	// エフェクトを追従
	if (!m_afterImage.expired())
	{
		m_afterImage.lock()->SetPos(m_pos);
		// 色
		ChangeAfteImageColorFromVel();
	}

	// ダメージモーション
	if (m_animModel->IsEnd())
	{
		ChangeStateIdle();
		return;
	}
}

void Bee::Death()
{
	// エフェクトを追従
	if (!m_afterImage.expired())
	{
		m_afterImage.lock()->SetPos(m_pos);
		ChangeAfteImageColorFromVel();
	}

	// アニメーションを再生したのち自身を削除
	if (m_stateFrameCount > 120)
	{
		// エフェクト削除
		if (!m_afterImage.expired())
		{
			m_afterImage.lock()->Kill();
		}

		// 死亡エフェクト出す
		EffectManager::GetInstance().GenerateEffect(kDeathEffect, m_pos);
		// 音を鳴らす
		SoundManager::GetInstance().Play(kDeathSound);

		m_isAlive = false;
		return;
	}
}

void Bee::Attack()
{
	// 体当たりしたい
	// この時にプレイヤーと当たるとダメージを与える

	// 落下し始めたら顔面アタック
	if (m_collidable->GetVel().y < 0 && !m_animModel->CheckAnimName(kAttackAnimName))
	{
		m_animModel->ChangeAnimation(kAttackAnimName, false);
		return;
	}

	// 着地したら
	if (m_pos.y < 0)
	{
		// もしヒットしてたなら
		if (m_isHitAttack)
		{
			// やったった
			ChangeStateAttacked();
			return;
		}
		else
		{
			// やっちまった
			ChangeStateStun();
			return;
		}
	}
}

void Bee::Stun()
{
	// 気絶する

	// 震えてもいい
	m_pos.x += GetRand(10) - 5;
	m_pos.z += GetRand(10) - 5;
	m_collidable->SetPos(m_pos);
	m_stunEffect.lock()->SetPos(m_pos);

	// 一定時間待つ
	if (m_stateFrameCount > kStunInterval)
	{
		ChangeStateRecovery();
		return;
	}
}

void Bee::Recovery()
{
	// モーションが終わるまで待つ

	if (m_animModel->IsEnd())
	{
		ChangeStateIdle();
		return;
	}
}

void Bee::Attacked()
{
	// 一定時間待つ

	if (m_stateFrameCount > kAttackedInterval)
	{
		ChangeStateRecovery();
		return;
	}
}

void Bee::ChangeState(StateFunc_t nextState)
{
	m_state = nextState;
	m_stateFrameCount = 0;
}

void Bee::ChangeStateIdle()
{
	ChangeState(&Bee::Idle);
	m_animModel->ChangeAnimation(kIdleAnimName, true);
}

void Bee::ChangeStateChase()
{
	ChangeState(&Bee::Chase);
	m_animModel->ChangeAnimation(kIdleAnimName, true);
}

void Bee::ChangeStateSurround()
{
	ChangeState(&Bee::Surround);
	// 今回の様子見時間を決める
	m_surroundFrame = GetRand(kSurroundFrameRange);
	m_surroundRotSpeed = GetRand(1) ? kSurroundRotateSpeed : -kSurroundRotateSpeed;
}

void Bee::ChangeStateDamage()
{
	ChangeState(&Bee::Damage);
	m_animModel->ChangeAnimation(kDamageAnimName, false);
	// もし気絶エフェクトが残ってたら消す
	if (!m_stunEffect.expired()) m_stunEffect.lock()->Kill();
}

void Bee::ChangeStateDeath()
{
	ChangeState(&Bee::Death);
	m_animModel->ChangeAnimation(kDeathAnimName, false);
	// もし気絶エフェクトが残ってたら消す
	if (!m_stunEffect.expired()) m_stunEffect.lock()->Kill();
}

void Bee::ChangeStateAttack()
{
	ChangeState(&Bee::Attack);
	m_animModel->ChangeAnimation(kPreAttackAnimName, false);
	const Vector3 attackVel = VTransformSR(kAttackVec, m_animModel->GetMatrix());
	m_collidable->GetRigid().AddVel(attackVel);
	m_isHitAttack = false;
}

void Bee::ChangeStateAttacked()
{
	ChangeState(&Bee::Attacked);
}

void Bee::ChangeStateStun()
{
	ChangeState(&Bee::Stun);
	m_animModel->ChangeAnimation(kStunAnimName,false);
	// エフェクト出す
	m_stunEffect = EffectManager::GetInstance().GenerateEffect(kStunEffect, m_pos);
	// ボーナススコア出す
	ScoreManager::GetInstance().SpecialBonusPoints(BonusKind::kAvoid);
}

void Bee::ChangeStateRecovery()
{
	ChangeState(&Bee::Recovery);
	m_animModel->ChangeAnimation(kRecoveryAnimName, false);
	// もし気絶エフェクトが残ってたら消す
	if (!m_stunEffect.expired()) m_stunEffect.lock()->Kill();
}

void Bee::OnDamage(const std::shared_ptr<PlayerAttackRange>& playerAttack)
{
	if (m_state == &Bee::Damage || m_state == &Bee::Death) return;

	// ダメージを受ける
	const int damage = playerAttack->GetDamage();
	m_hp -= damage;

	// もし自分が攻撃中なら
	// カウンターとしてボーナス
	if (IsAttacking())
	{
		ScoreManager::GetInstance().SpecialBonusPoints(BonusKind::kCounterAttack);
	}
	// さらに自分が一撃で死んだならボーナス
	if (damage >= kMaxHp)
	{
		ScoreManager::GetInstance().SpecialBonusPoints(BonusKind::kMaxChargeAttack);
	}

	auto& rigid = m_collidable->GetRigid();
	const auto mat = m_animModel->GetMatrix();
	const Vector3 dir = { mat.m[2][0],mat.m[2][1],mat.m[2][2] };
	// ダメージに応じて吹っ飛ぶ力を変える
	// 吹っ飛ぶ向きはプレイヤーの向き
	Vector3 blownAway = VTransformSR(kBlownVec * static_cast<float>(damage*damage), m_player.lock()->GetMatrix());
	// モデルの向きが行列の向きと逆なのでそれを修正する
	blownAway.x *= -1;
	blownAway.z *= -1;
	rigid.AddVel(blownAway);

	// 軌道エフェクトを出す
	m_afterImage = EffectManager::GetInstance().GenerateEffect(kAfterImage, m_pos);
	// 色を変える
	ChangeAfteImageColorFromVel();

	if (m_hp.IsMin())
	{
		// 体力がなくなったら死ぬ
		ChangeStateDeath();
		OnDeath();
		return;
	}

	ChangeStateDamage();
}

void Bee::OnDeath()
{
	// スコア加算
	ScoreManager::GetInstance().DefeatedEnemy();
}

void Bee::ChangeAfteImageColorFromVel()
{
	if (m_afterImage.expired()) return;

	auto effect = m_afterImage.lock();

	// 速度の大きさをとる
	const float mag = m_collidable->GetVel().Magnitude()* kIncreaseColorMult;
	// 速度に応じて色を変える
	m_afterImage.lock()->SetColor(kAfterImageDefaultR, std::clamp(kAfterImageDefaultG - static_cast<int>(mag), 0, 255), kAfterImageDefaultB, kAfterImageDefaultA);
}

Vector3 Bee::GetDir() const
{
	auto mat = m_animModel->GetMatrix();
	return {-mat.m[2][0],-mat.m[2][1] ,-mat.m[2][2] };
}

void Bee::RotateUpVecY(const float rad)
{
	m_animModel->RotateUpVecY(rad);
}

bool Bee::IsAttacking() const
{
	return m_state == &Bee::Attack;
}

bool Bee::IsDamaging() const
{
	return m_state == &Bee::Damage || m_state == &Bee::Death;
}
