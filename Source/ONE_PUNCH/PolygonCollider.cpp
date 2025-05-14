#include "PolygonCollider.h"
#include <DxLib.h>
#include <string>
#include <cassert>

namespace
{
	// �|���S���̓����蔻������������Ƃ��͕K�����̖��O�̃t���[����p�ӂ��邱�ƁB
	const std::string kCollisionFrameName = "CollisionFrame";
}

PolygonCollider::PolygonCollider() :
	Collider3D(),
	m_model(-1),
	m_frameIndex(-1),
	m_useTransform(false)
{
}

PolygonCollider::~PolygonCollider()
{
	// �Q�Ɨp���b�V���̌�n��
	MV1TerminateReferenceMesh(m_model, m_frameIndex, m_useTransform);
}

void PolygonCollider::Init(const Vector3& initPos, const int weight, const bool isThrough, const bool isStatic, const int handle, const bool useTransform)
{
	Collider3D::Init(initPos, weight, isThrough, isStatic);
	m_colKind = ColKind3D::kPolygon;
	m_model = handle;
	m_useTransform = useTransform;
	// ����͂��Ȃ��Ƃ����Ȃ��炵��
	MV1SetupCollInfo(m_model);

	// �����蔻��p�̃t���[�����Z�b�g�A�b�v
	m_frameIndex = MV1SearchFrame(m_model, kCollisionFrameName.c_str());
	assert(m_frameIndex != -1 && "�����������f�����ǂݍ��߂ĂȂ�����");
	assert(m_frameIndex != -2 && "�w�薼�̃t���[��������܂���");
	MV1SetupReferenceMesh(m_model, m_frameIndex, m_useTransform);
}

void PolygonCollider::RefreshRefMesh()
{
	// �ړ����Ȃ��Ȃ��΂���
	if (!m_useTransform) return;

	MV1RefreshReferenceMesh(m_model, m_frameIndex, m_useTransform);
}

void PolygonCollider::Draw() const
{
	MV1DrawModel(m_model);
}

const DxLib::tagMV1_REF_POLYGONLIST PolygonCollider::GetRefMesh()
{
	return MV1GetReferenceMesh(m_model, m_frameIndex, m_useTransform);
}
