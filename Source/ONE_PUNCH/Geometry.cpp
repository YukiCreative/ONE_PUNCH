#include "Geometry.h"
#include <cmath>
Vector3 Geometry::GetRotatedPosUpVecY(const Vector3& movePos, const Vector3& basePos, const float rad)
{
	// basePos�����_����������movePos�̈ʒu���o��
	const Vector3 movedPos = movePos - basePos;
	// �o�����ʒu���A���_�𒆐S�ɉ�]������
	const Vector3 rotatedMovedPos = VTransform(movedPos, MGetRotY(rad));
	// targetPos�����_�ɖ߂�
	const Vector3 movedRotatedMovedPos = rotatedMovedPos + basePos;
	// �����Ԃ�
	return movedRotatedMovedPos;
}

MATRIX Geometry::GetRotateMatrixUpVecY(const Vector3& movePos, const Vector3& basePos, const float rad)
{
	// ���炷
	const MATRIX moveMat = MGetTranslate(-basePos);
	// ��
	const MATRIX rotateMat = MGetRotY(rad);
	const MATRIX rotatedMovedMat = MMult(moveMat, rotateMat);
	// �t�ɖ߂��Ă���
	const MATRIX returnMat = MGetTranslate(basePos);
	const MATRIX movedRotatedMovedMat = MMult(rotatedMovedMat, returnMat);

	return movedRotatedMovedMat;
}

Vector3 Geometry::GetRotatedPosAnyUpVec(const Vector3& movePos, const Vector3& basePos, const Vector3& axis, const float rad)
{
	// basePos�����_����������movePos�̈ʒu���o��
	const Vector3 movedPos = movePos - basePos;
	// �o�����ʒu���A���_�𒆐S�ɉ�]������
	const Vector3 rotatedMovedPos = VTransform(movedPos, MGetRotY(rad));
	// targetPos�����_�ɖ߂�
	const Vector3 movedRotatedMovedPos = rotatedMovedPos + basePos;
	// �����Ԃ�
	return movedRotatedMovedPos;
}

float Geometry::Corner(const Vector3& a, const Vector3& b)
{
	// �����ʂ�ɂ��Ƃ����ԏd�������Ɍ����鏈���ɂȂ�����
	return acosf(a.Dot(b) / (a.Magnitude() * b.Magnitude()));
}