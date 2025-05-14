#pragma once
#include "Collider3D.h"
#include <memory>
#include <list>

namespace DxLib
{
	struct tagMV1_COLL_RESULT_POLY_DIM;
	struct tagMV1_REF_POLYGONLIST;
}

class PolygonCollider : public Collider3D
{
public:
	PolygonCollider();
	~PolygonCollider();

	void Init(const Vector3& initPos, const int weight, const bool isThrough, const bool isStatic, const int handle, const bool useTransform);

	// DxLib�̎Q�ƃ��b�V���̍X�V
	// �����蔻�肷�鎞�ɂł��Ă�ł���
	void RefreshRefMesh();
	void Draw() const override;

	const int GetModelHandle() const { return m_model; }
	void SetModelHandle(const int handle) { m_model = handle; }
	// �Q�Ɨp���b�V���̍\���̂�Ԃ��܂�
	const DxLib::tagMV1_REF_POLYGONLIST GetRefMesh();
private:
	int m_model;
	// "CllisionFrame"�Ƃ����t���[���̔ԍ�
	int m_frameIndex;
	// ���̃��f�����ړ�������A�j���[�V���������肷�邩�ǂ���
	// �ړ����Ȃ��ꍇ�͂��̕��y���Ȃ�Ɨ\�z
	bool m_useTransform;
};