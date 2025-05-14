#include "Collider3D.h"

Collider3D::Collider3D() :
	m_colKind(ColKind3D::kNone), // None�̓G���[��f�����߂̑���
	m_pos(),
	m_isMove(false),
	m_isStatic(false),
	m_weight(1)
{
}

Collider3D::~Collider3D()
{
}

void Collider3D::Init(const Vector3& initPos, const int weight, const bool isThrough, const bool isStatic)
{
	m_pos = initPos;
	m_weight = weight;
	m_isThrough = isThrough;
	m_isStatic = isStatic;
}

void Collider3D::SetPos(const Vector3& pos)
{
	// �ʒu���ς���Ă�Γ����Ă���
	// ���łɍ��t���[���œ����Ă���Ƃ��͂�������true���ێ�
	m_isMove = m_isMove || m_pos != pos;
	m_pos = pos;
}