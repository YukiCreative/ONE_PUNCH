#include "AnimationModel.h"
#include "Bee.h"
#include "Camera.h"
#include "Collidable.h"
#include "Color.h"
#include "EffectManager.h"
#include "EffekseerEffect.h"
#include "Game.h"
#include "Geometry.h"
#include "Input.h"
#include "Player.h"
#include "PlayerAttackRange.h"
#include "Rigid.h"
#include "ScoreManager.h"
#include "Sound.h"
#include "SoundManager.h"
#include "Sphere.h"
#include "SphereCollider.h"
#include "Vector2.h"
#include <DxLib.h>

namespace
{
	// テストで球体を出す際の定数
	constexpr int   kDivNum = 10;
	constexpr int   kDifColor = Color::Red;
	constexpr int   kSpcColor = Color::White;
	constexpr bool  kIsFill = true;

	// 衝突、物理
	const Vector3 kInitPos = { 0, 0, 0 };
	constexpr float kRadius = 50;
	constexpr int kWeight = 100;
	constexpr bool kIsThrough = false;
	constexpr bool kIsStatic = false;
	const PhysicalMaterial kPhysicalMaterial =
	{
		0.1f,  // 地上摩擦
		0.01f, // 空中摩擦
		0.1f,  // 反発係数
	};

	// ゲーム定数
	constexpr float kWalkSpeed = 1.0f;
	constexpr float kDashSpeed = 2.0f;
	constexpr float kJumpSpeed = 0.5f;
	constexpr float kAnimSpeed = 30.0f;
	constexpr float kCameraRotateMult = 0.0001f;
	constexpr float kJumpForce = 30.0f;
	constexpr float kKnockBack = 10.0f;
	constexpr float kFallVel = 2.0f;
	constexpr float kChargeIncrease = 0.01f; // 1フレームに増加するチャージ量
	constexpr int   kAttackLifeTime = 30; // 攻撃持続時間
	const Vector3   kAttackPosOffset = {0, 0, -100};
	constexpr float kAttackPosOffsetY = 100;
	constexpr float kAttackRadiusBase = 100.0f;
	constexpr int   kInvincibleFrame = 30;
	constexpr float kModelRotateSpeed = 0.05f;
	const Vector3   kEffectOffset = { 0, 0, -100 };
	const Vector3   kAttackEffectSizeBase = {0.5f,0.5f,0.5f};
	constexpr float kPlayerPosLimit = 2500.0f;
	constexpr float kPlayerWalkBonusLength = 50000;
	constexpr float kPlayerDashBonusLength = 100000;

	// エフェクト
	const Vector3 kChargeEffectOffset = {-100, 150, 50};
	const std::string kChargeEffectName = "PlayerCharge.efkefc";

	// これはプレイヤーの向きによって回す
	const Vector3 kTargetPosOffset = {0, 200, -100};
	// アニメーションの名前
	const std::string kModelPath         = "Data/Model/Player.mv1";
	const std::string kIdleAnimName      = "CharacterArmature|Idle";
	const std::string kWalkAnimName      = "CharacterArmature|Walk";
	const std::string kDashAnimName      = "CharacterArmature|Run";
	const std::string kJumpAnimName      = "CharacterArmature|Jump";
	const std::string kAirAnimName       = "CharacterArmature|Jump_Idle";
	const std::string kLandingAnimName   = "CharacterArmature|Jump_Land";
	const std::string kDeathAnimName     = "CharacterArmature|Death";
	const std::string kDamageAnimName    = "CharacterArmature|Damage";
	const std::string kAttackAnimName    = "CharacterArmature|Punch";
	// アニメーションが粉々になったので仕方なく対応
	const std::string kStartPunchAnim    = "Punch.005";
	const std::string kChargeAnimName    = "Punch.006";
	const std::string kAttackMomentAnim  = "Punch.002";
	const std::string kEndPunchAnim      = "Punch.004";
	const std::string kMaxChargeAnimName = "Punch.008";
	const std::string kSharpTurnAnimName = "SharpTurn";

	// 音
	const std::string kChargingSound = "Charge.wav";
	const std::string kMaxChargeSound = "MaxCharge.wav";
	const std::string kJumpSound = "Jump.mp3";
	const std::string kBigPunchSound = "BigPunch.mp3";
	const std::string kMiddlePunchSound = "MiddlePunch.mp3";
	const std::string kWindSound = "MiddlePunch.mp3";
}

Player::Player() :
	m_state(&Player::Idle),
	m_stateFrameCount(0),
	m_invincibleCount(0),
	m_walkLength(0),
	m_runLength(0),
	m_isGotRunBonus(false),
	m_isGotWalkBonus(false)
{
}

Player::~Player()
{
	// もしチャージ音とかがなっていたら止める
	if (!m_chargeSound.expired())
	{
		m_chargeSound.lock()->Stop();
	}
}

void Player::Init(std::weak_ptr<Camera> camera)
{
	m_hp = kPlayerMaxHp;
	m_kind = ActorKind::kPlayer;
	m_pos = kInitPos;
	m_camera = camera;
	m_camera.lock()->SetTargetPos(m_pos + kTargetPosOffset);
	m_sphere = std::make_shared<Sphere>();
	m_sphere->Init(kRadius, kDivNum, kDifColor, kSpcColor, kIsFill);
	auto sphereCol = std::make_shared<SphereCollider>();
	sphereCol->Init(m_pos, kWeight, kIsThrough, kIsStatic, kRadius);
	auto rigid = std::make_shared<Rigid>();
	rigid->Init(kPhysicalMaterial);
	m_collidable = std::make_shared<Collidable>();
	m_collidable->Init(sphereCol, rigid);
	m_animModel = std::make_shared<AnimationModel>();
	m_animModel->Init(kModelPath, kAnimSpeed);
	m_animModel->ChangeAnimation(kIdleAnimName);
	m_animModel->Play();
}

void Player::Update()
{
	Input& input = Input::GetInstance();

	(this->*m_state)(input);
	++m_stateFrameCount;

	if (input.IsTrigger("Reset"))
	{
		m_pos = Vector3::Zero();
		m_collidable->GetCol().SetPos(Vector3::Zero());
	}

	m_sphere->SetDifColor(Color::Red);
	// アニメーションを進める
	m_animModel->Update();
	// 無敵時間を経過させる
	--m_invincibleCount;
}

void Player::Draw() const
{
	m_animModel->Draw(m_pos);

#if _DEBUG
	m_sphere->Draw(m_pos);
	const Vector3 correctedTargetOffset = VTransformSR(kTargetPosOffset, m_animModel->GetMatrix());
	DrawSphere3D(m_pos + correctedTargetOffset, 10, 10, 0xffffff, 0xffffff, true);

	// プレイヤーの位置を表示
	DrawFormatString(Game::kScreenWidth - 300, 0, 0xffffff, "X:%f,Y:%f,Z:%f", m_pos.x, m_pos.y, m_pos.z);
	auto mat = m_animModel->GetMatrix();
	DrawLine3D(m_pos, m_pos + Vector3{mat.m[0][0], mat.m[0][1], mat.m[0][2]} * 100, 0xff0000);
	DrawLine3D(m_pos, m_pos + Vector3{mat.m[1][0], mat.m[1][1], mat.m[1][2]} * 100, 0x00ff00);
	DrawLine3D(m_pos, m_pos + Vector3{mat.m[2][0], mat.m[2][1], mat.m[2][2]} * 100, 0x0000ff);

	DrawFormatString(Game::kScreenWidth - 300, 15, 0xffffff, "チャージ:%f", m_punchCharge.Value());
	DrawLine3D(m_pos, m_pos + m_collidable->GetVel()*100, 0xff0000);
#endif
}

void Player::OnCollision(std::shared_ptr<Actor> other)
{
	m_sphere->SetDifColor(Color::Blue);
	ActorKind kind = other->GetKind();

	if (kind == ActorKind::kEnemy)
	{
		// もし敵が攻撃状態で接触したら
		auto enemy = std::static_pointer_cast<Bee>(other);
		if (enemy->IsAttacking())
		{
			printf("当たった\n");
			// 吹っ飛ぶ
			const Vector3 force = (m_pos - other->GetPos().XZ()).GetNormalize() * kKnockBack;
			m_collidable->GetRigid().AddVel(force);
			OnDamage();
			return;
		}
	}
	if (kind == ActorKind::kFloor)
	{
		printf("プレイヤー地面に当たった!");
		if (m_state == &Player::Air)
		{
			ChangeStateLanding();
			return;
		}
	}
}

const Vector3 Player::GetDir() const
{
	auto mat = m_animModel->GetMatrix();
	return Vector3{-mat.m[2][0], -mat.m[2][1], -mat.m[2][2]}.GetNormalize();
}

const DxLib::tagMATRIX Player::GetMatrix() const
{
	return m_animModel->GetMatrix();
}

void Player::CommitMove()
{
	auto& rigid = m_collidable->GetRigid();
	auto& col = m_collidable->GetCol();

	// Colliderの位置が正しい位置なのでそれを反映
	m_pos = col.GetPos();

	// 移動
	// 動いていなければ何もしない
	if (rigid.IsStop()) return;

	// 移動量を確定
	const Vector3 speed = rigid.Update();

	// 移動前位置を取っておく
	const Vector3 posBeforeMove = m_pos;
	// 移動量を反映
	m_pos += speed;
	col.SetPos(m_pos);

	// プレイヤーの行動範囲を制限
	LimitPlayer();

	// 最終的な移動距離を出す
	const Vector3 moveValue = m_pos - posBeforeMove;
	// 歩いた距離or走った距離に加算
	if (m_state == &Player::Walk)
	{
		m_walkLength += moveValue.SqrMagnitude();
		// 距離が一定以上に達したら実績
		if (m_walkLength > kPlayerWalkBonusLength &&
			!m_isGotWalkBonus)
		{
			ScoreManager::GetInstance().SpecialBonusPoints(BonusKind::kWalkBonus);
			m_isGotWalkBonus = true;
		}
	}
	else if (m_state == &Player::Dash)
	{
		m_runLength += moveValue.SqrMagnitude();
		// 距離が一定以上に達したら実績
		if (m_runLength > kPlayerDashBonusLength &&
			!m_isGotRunBonus)
		{
			ScoreManager::GetInstance().SpecialBonusPoints(BonusKind::kDashBonus);
			m_isGotRunBonus = true;
		}
	}

	// カメラを動かす
	if (!m_camera.expired())
	{
		// カメラの注視点がプレイヤーの前に来るようにする
		const Vector3 correctedTargetOffset = VTransformSR(kTargetPosOffset, m_animModel->GetMatrix());
		m_camera.lock()->SetTargetPos(m_pos + correctedTargetOffset);
	}

	const Vector3 speedDir = Vector3(speed.x, 0, speed.z).GetNormalize();
	const Vector3 modelDir = GetDir();
	const Vector3 cross = speedDir.Cross(modelDir);

	// 移動の向きにモデルの向きを変える
	float rotate = -cross.y * 0.5f;
	m_animModel->RotateUpVecY(rotate);

	const float   dot = speedDir.Dot(modelDir);
	// 真反対に移動しているときに振り向かない問題を解決
	if (fabsf(rotate) < 0.01f && dot < -0.999999f)
	{
		// 特定の状態なら
		if (m_state == &Player::Dash)
		{
			// 急旋回状態へ
			ChangeStateSharpTurn();
			return;
		}
		// また、歩き状態なら
		if (m_state == &Player::Walk)
		{
			m_animModel->RotateUpVecY(0.5f);
		}
	}
}

void Player::OnDamage()
{
	// もし無敵時間なら食らわない
	if (m_invincibleCount > 0) return;

	// HPを減らす とりあえず1
	m_hp -= 1;

#if _DEBUG
	printf("現在HP%d\n", m_hp.Value());
#endif

	// もしチャージ中だったなら、そのエフェクトを消す
	if (!m_chargeEffect.expired())
	{
		m_chargeEffect.lock()->Kill();
	}
	// 音も消す
	if (!m_chargeSound.expired())
	{
		m_chargeSound.lock()->Stop();
		m_chargeSound.reset();
	}

	// HPが0なら死亡
	if (m_hp.IsMin())
	{
		ChangeStateDeath();
		return;
	}

	// ダメージ状態へ移行
	ChangeStateDamage();
}

void Player::CameraRotate(Input& input)
{
	// 右スティックでカメラ
	Vector2 rightAxis = input.GetRightInputAxis();
	if (input.IsPressed("CameraRotateRight"))
	{
		rightAxis.x = 1000;
	}
	if (input.IsPressed("CameraRotateLeft"))
	{
		rightAxis.x = -1000;
	}
	if (input.IsPressed("CameraRotateVPlus"))
	{
		rightAxis.y = 1000;
	}
	if (input.IsPressed("CameraRotateVMinus"))
	{
		rightAxis.y = -1000;
	}
	if (!m_camera.expired())
	{
		m_camera.lock()->RotateCameraV(rightAxis.y * kCameraRotateMult);
		m_camera.lock()->RotateCameraUpVecY(rightAxis.x * kCameraRotateMult);
	}
}

void Player::ChangeState(StateFunc_t nextState)
{
	m_state = nextState;
	m_stateFrameCount = 0;
}

void Player::ChangeStateIdle()
{
	ChangeState(&Player::Idle);
	m_animModel->ChangeAnimation(kIdleAnimName);
}

void Player::ChangeStateWalk()
{
	ChangeState(&Player::Walk);
	m_animModel->ChangeAnimation(kWalkAnimName);
}

void Player::ChangeStateDash()
{
	ChangeState(&Player::Dash);
	m_animModel->ChangeAnimation(kDashAnimName);
}

void Player::ChangeStateJump()
{
	ChangeState(&Player::Jump);
	m_animModel->ChangeAnimation(kJumpAnimName, false);
	auto& rigid = m_collidable->GetRigid();
	m_collidable->GetRigid().AddVel({0, kJumpForce, 0});
	rigid.ChangeStateAir();
	// 効果音
	SoundManager::GetInstance().Play(kJumpSound);
}

void Player::ChangeStateAir()
{
	ChangeState(&Player::Air);
	m_animModel->ChangeAnimation(kAirAnimName);
}

void Player::ChangeStateLanding()
{
	auto& rigid = m_collidable->GetRigid();
	rigid.StopY();
	ChangeState(&Player::Landing);
	m_animModel->ChangeAnimation(kLandingAnimName, false);
	rigid.ChangeStateGround();
}

void Player::ChangeStateDeath()
{
	ChangeState(&Player::Death);
	m_animModel->ChangeAnimation(kDeathAnimName, false);
}

void Player::ChangeStateDamage()
{
	ChangeState(&Player::Damage);
	m_animModel->ChangeAnimation(kDamageAnimName, false);
	// 無敵時間を設定
	m_invincibleCount = kInvincibleFrame;
}

void Player::ChangeStateAttack()
{
	// エフェクト消す
	m_chargeEffect.lock()->Kill();
	// 音消す
	m_chargeSound.lock()->Stop();
	// パンチ音を出す
	// ため具合によって変える
	if (m_punchCharge.IsMax())
	{
		SoundManager::GetInstance().Play(kBigPunchSound);
		SoundManager::GetInstance().Play(kWindSound);
	}
	else
	{
		SoundManager::GetInstance().Play(kMiddlePunchSound);
	}


	ChangeState(&Player::Attack);
	m_animModel->ChangeAnimation(kAttackMomentAnim, false);
	const float charge = m_punchCharge.Value();

	// 攻撃判定を生成
	auto punch = std::make_shared<PlayerAttackRange>();
	// ためが長ければ長いほど攻撃力が高くなり、判定が大きくなる
	punch->Init(static_cast<int>(charge), kAttackLifeTime, GetPlayerAttackPos(), kAttackRadiusBase * charge * charge);
	SpawnActor(punch);

	// エフェクト
	Vector3 effectOffset = VTransformSR(kEffectOffset, m_animModel->GetMatrix());
	effectOffset *= charge;
	auto sonicBoom = EffectManager::GetInstance().GenerateEffect("SonicBoom.efkefc", m_pos + effectOffset);
	auto ballWind  = EffectManager::GetInstance().GenerateEffect("BallWind.efkefc", m_pos + effectOffset*2);
	// 回転
	const Vector2 dir = { GetDir().x, GetDir().z };
	const float   rot = dir.RerativeAngle(Vector2::Up()) * Geometry::kDegToRad;
	sonicBoom.lock()->SetRotate({0,-rot,0});
	ballWind .lock()->SetRotate({0,-rot,0});
	// 拡大縮小
	sonicBoom.lock()->SetScale(kAttackEffectSizeBase * charge);
	ballWind .lock()->SetScale(kAttackEffectSizeBase * charge);

	m_punchCharge.SetMin();
}

void Player::ChangeStateCharge()
{
	ChangeState(&Player::Charge);
	m_animModel->ChangeAnimation(kStartPunchAnim, false);
	// チャージを初期値に
	m_punchCharge.SetMin();
	// チャージエフェクトを出す
	const Vector3 chargePos = m_pos + VTransformSR(kChargeEffectOffset, GetMatrix());
	m_chargeEffect = EffectManager::GetInstance().GenerateEffect(kChargeEffectName, chargePos);
	// チャージ音を出す
	m_chargeSound = SoundManager::GetInstance().Play(kChargingSound);
}

void Player::ChangeStateMaxCharge()
{
	ChangeState(&Player::MaxCharge);
	// TODO:該当アニメーションの作成
	m_animModel->ChangeAnimation(kMaxChargeAnimName, true);
	// 無限チャージの音に切り替え
	m_chargeSound.lock()->Stop();
	m_chargeSound = SoundManager::GetInstance().Play(kMaxChargeSound, true);
}

void Player::ChangeStateSharpTurn()
{
	ChangeState(&Player::SharpTurn);
	// いい感じにならないので、すべる方向に力を加えてしまう
	m_collidable->GetRigid().AddVel(GetDir() * 50);
	m_animModel->ChangeAnimation(kSharpTurnAnimName, false);
}

Vector3 Player::GetMoveValueFromLeftAxis(Input& input, const float moveMagnitude)
{
	Vector3 moveDir = {};
	// 左スティックをとる
	const Vector2 leftAxis = input.GetLeftInputAxis();
	moveDir.x = leftAxis.x;
	moveDir.z = -leftAxis.y;

	// √2移動を阻止
	Vector3 moveValue = moveDir * moveMagnitude;
	if (moveDir.SqrMagnitude() > 1 * 1)
	{
		moveValue = moveDir.GetNormalize() * moveMagnitude;
	}

	return moveValue;
}

Vector3 Player::GetPlayerAttackPos() const
{
	Vector3 result = m_pos + VTransformSR(kAttackPosOffset * m_punchCharge.Value() * m_punchCharge.Value(), m_animModel->GetMatrix());
	result.y += kAttackPosOffsetY;
	return result;
}

void Player::LimitPlayer()
{
	// 地面はy=0。常識です。
	if (m_pos.y < 0) m_pos.y = 0;
	// posが一定範囲を超えないようにclamp
	m_pos.x = std::clamp(m_pos.x, -kPlayerPosLimit, kPlayerPosLimit);
	m_pos.z = std::clamp(m_pos.z, -kPlayerPosLimit, kPlayerPosLimit);

	m_collidable->SetPos(m_pos);
}

void Player::Idle(Input& input)
{
	CameraRotate(input);

	// 待機アニメーション
	// 入力があったらその状態へ
	if (input.GetLeftInputAxis().SqrMagnitude())
	{
		ChangeStateWalk();
		return;
	}
	// スペースキーでジャンプ
	if (input.IsTrigger("Jump"))
	{
		ChangeStateJump();
		return;
	}
	// Aボタンでチャージ開始
	if (input.IsTrigger("Attack"))
	{
		ChangeStateCharge();
		return;
	}
}

void Player::Walk(Input& input)
{
	CameraRotate(input);

	// 入力がなくなったら待機
	if (!input.GetLeftInputAxis().SqrMagnitude())
	{
		ChangeStateIdle();
		return;
	}
	if (input.IsPressed("Dash"))
	{
		ChangeStateDash();
		return;
	}
	// スペースキーでジャンプ
	if (input.IsTrigger("Jump"))
	{
		ChangeStateJump();
		return;
	}
	// Aボタンでチャージ開始
	if (input.IsTrigger("Attack"))
	{
		ChangeStateCharge();
		return;
	}

	Vector3 moveValue = GetMoveValueFromLeftAxis(input, kWalkSpeed);

	// 移動量をカメラの向きに修正
	Vector3 correctedMove = m_camera.lock()->RotateVecToCameraDirXZ(moveValue, {0,0,1});
	
	// 加速
	m_collidable->GetRigid().AddVel(correctedMove);
}

void Player::Dash(Input& input)
{
	CameraRotate(input);

	// 入力がなくなったら待機
	if (!input.GetLeftInputAxis().SqrMagnitude())
	{
		ChangeStateIdle();
		return;
	}
	// 動いてるけど、走ってもいない→歩いている
	if (!input.IsPressed("Dash"))
	{
		ChangeStateWalk();
		return;
	}
	// スペースキーでジャンプ
	if (input.IsTrigger("Jump"))
	{
		ChangeStateJump();
		return;
	}
	// Aボタンでチャージ開始
	if (input.IsTrigger("Attack"))
	{
		ChangeStateCharge();
		return;
	}

	Vector3 moveValue = GetMoveValueFromLeftAxis(input, kDashSpeed);

	// 移動量をカメラの向きに修正
	Vector3 correctedMove = m_camera.lock()->RotateVecToCameraDirXZ(moveValue, { 0,0,1 });

	m_collidable->GetRigid().AddVel(correctedMove);
}

void Player::Jump(Input& input)
{
	CameraRotate(input);

	// Jumpアニメーションが終わったらAirへ
	if (m_animModel->IsEnd())
	{
		ChangeStateAir();
		return;
	}

	Vector3 moveValue = GetMoveValueFromLeftAxis(input, kJumpSpeed);

	// 移動量をカメラの向きに修正
	Vector3 correctedMove = m_camera.lock()->RotateVecToCameraDirXZ(moveValue, { 0,0,1 });

	m_collidable->GetRigid().AddVel(correctedMove);
}

void Player::Air(Input& input)
{
	CameraRotate(input);

	// 接地したら次の状態へ
	if (m_pos.y <= 0)
	{
		ChangeStateLanding();
		return;
	}

	Vector3 moveValue = GetMoveValueFromLeftAxis(input, kJumpSpeed);

	// 移動量をカメラの向きに修正
	Vector3 correctedMove = m_camera.lock()->RotateVecToCameraDirXZ(moveValue, { 0,0,1 });

	m_collidable->GetRigid().AddVel(correctedMove);
}

void Player::Landing(Input& input)
{
	CameraRotate(input);

	// 着地モーションが終わるか、何か入力があったら別の状態へ
	if (m_animModel->IsEnd())
	{
		ChangeStateIdle();
		return;
	}
	// Aボタンでチャージ開始
	if (input.IsTrigger("Attack"))
	{
		ChangeStateCharge();
		return;
	}
}

void Player::Death(Input& input)
{
	CameraRotate(input);

	// 死亡アニメーションが終了するまで待機
	if (m_animModel->IsEnd())
	{

		return;
	}
}

void Player::Damage(Input& input)
{
	CameraRotate(input);

	// 移動
	Vector3 moveValue = GetMoveValueFromLeftAxis(input, kWalkSpeed);

	// 移動量をカメラの向きに修正
	Vector3 correctedMove = m_camera.lock()->RotateVecToCameraDirXZ(moveValue, { 0,0,1 });

	// 加速
	m_collidable->GetRigid().AddVel(correctedMove);

	// スペースキーでジャンプ
	if (input.IsTrigger("Jump"))
	{
		ChangeStateJump();
		return;
	}
	// アニメーションが終わればIdleへ
	if (m_animModel->IsEnd())
	{
		ChangeStateIdle();
		return;
	}
	// 接地したら次の状態へ
	if (m_pos.y <= 0)
	{
		ChangeStateLanding();
		return;
	}
}

void Player::Attack(Input& input)
{
	CameraRotate(input);
	// 遷移した瞬間が状態関数内にかけないのがつらい
	// クラス化しよかな

	// 細切れのアニメーションをつなげる
	if (m_animModel->CheckAnimName(kAttackMomentAnim) && m_animModel->IsEnd())
	{
		m_animModel->ChangeAnimation(kEndPunchAnim, false);
	}

	// 殴り終わったら待機
	if (m_animModel->CheckAnimName(kEndPunchAnim) && m_animModel->IsEnd())
	{
		ChangeStateIdle();
		return;
	}

	// 殴った後、余韻は移動状態で消せる
	if (m_animModel->CheckAnimName(kEndPunchAnim) && input.GetLeftInputAxis().SqrMagnitude())
	{
		ChangeStateWalk();
		return;
	}

	// 殴った後Aボタンでチャージ開始
	if (m_animModel->CheckAnimName(kEndPunchAnim) && input.IsTrigger("Attack"))
	{
		ChangeStateCharge();
		return;
	}
}

void Player::Charge(Input& input)
{
	// カメラは動かせる
	CameraRotate(input);
	// 移動はできない

	// チャージ
	m_punchCharge += kChargeIncrease;

	// もしStartPunchAnimが終わったら、次にChargeを再生
	if (m_animModel->CheckAnimName(kStartPunchAnim) && m_animModel->IsEnd())
	{
		m_animModel->ChangeAnimation(kChargeAnimName, false);
	}

	// 攻撃を離すまで実行
	if (!input.IsPressed("Attack"))
	{
		ChangeStateAttack();
		return;
	}
	// 最大までチャージしたらループモーションへ
	if (m_punchCharge.IsMax())
	{
		ChangeStateMaxCharge();
		return;
	}
	// ジャンプしたらキャンセルできる
	if (input.IsTrigger("Jump"))
	{
		ChangeStateJump();
		// パンチのチャージをリセット
		m_punchCharge.SetMin();
		return;
	}
}

void Player::MaxCharge(Input& input)
{
	CameraRotate(input);

	// 攻撃ボタンを離すまでアニメーションを実行
	if (!input.IsPressed("Attack"))
	{
		ChangeStateAttack();
		return;
	}
}

void Player::SharpTurn(Input& input)
{
	CameraRotate(input);

	// モデルの反対方向に力を加え続ける
	m_collidable->GetRigid().AddVel(-GetDir());

	if (m_animModel->IsEnd())
	{
		ChangeStateIdle();
		// 向きを反転
		m_animModel->RotateUpVecY(DX_PI_F);
		return;
	}
}
