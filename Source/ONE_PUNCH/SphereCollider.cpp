#include "Color.h"
#include "SphereCollider.h"
#include <DxLib.h>

namespace
{
	// DrawSphere3D�Ɏg�p����萔���`
	const int kDivNum = 16;
	const Color::ColorCode_t kDifColor = Color::Red;
	const Color::ColorCode_t kSpcColor = Color::White;
	const bool kIsFill = false;
}

SphereCollider::SphereCollider() :
	m_radius(0)
{
	// colKind���㏑��
	m_colKind = ColKind3D::kSphere;
}

void SphereCollider::Init(const Vector3& initPos, const int weight, const bool isThrough, const bool isStatic, const float radius)
{
	Collider3D::Init(initPos, weight, isThrough, isStatic);
	m_radius = radius;
}

void SphereCollider::Draw() const
{
	// �����蔻��̗L���͈͂��t���[���ŕ`��
	DrawSphere3D(m_pos, m_radius, kDivNum, kDifColor, kSpcColor, kIsFill);
}