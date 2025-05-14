#include "SphereActor.h"
#include "Sphere.h"
#include "SphereCollider.h"
#include "Rigid.h"
#include "Collidable.h"

namespace
{
	// コライダー
	constexpr int kWeight = 1;
	constexpr bool kIsThrough = false;
	constexpr bool kIsStatic = true;
	const PhysicalMaterial kPhysicalMaterial =
	{
		0.1f, // 地上摩擦
		0.1f, // 空中摩擦
		0.1f, // 反発係数
	};
}

void SphereActor::Init(const Vector3& initPos, const float radius, const int divNum, const Color::ColorCode_t difColor, const Color::ColorCode_t spcColor, bool isFill)
{
	m_pos = initPos;
	m_sphere = std::make_shared<Sphere>();
	m_sphere->Init(radius, divNum, difColor, spcColor, isFill);

	// ひとまず球体と同じ大きさ、位置で初期化
	auto sphereCol = std::make_shared<SphereCollider>();
	sphereCol->Init(m_pos, kWeight, kIsThrough, kIsStatic, radius);

	auto rigid = std::make_shared<Rigid>();
	rigid->Init(kPhysicalMaterial);
	m_collidable = std::make_shared<Collidable>();
	m_collidable->Init(sphereCol, rigid);
}

void SphereActor::Update()
{
	m_sphere->SetDifColor(Color::Red);
}

void SphereActor::Draw() const
{
	m_sphere->Draw(m_pos);
}

void SphereActor::CommitMove()
{
	auto& rigid = m_collidable->GetRigid();
	m_pos += rigid.GetVel();
	m_collidable->GetCol().SetPos(m_pos);
	rigid.Update();
}

void SphereActor::OnCollision(std::shared_ptr<Actor> other)
{
	// 色を変える
	m_sphere->SetDifColor(Color::Blue);
	printf("当たった！");
}