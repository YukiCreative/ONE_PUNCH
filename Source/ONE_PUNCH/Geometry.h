#pragma once

#include "Vector2.h"
#include "Vector3.h"
#include <DxLib.h>

namespace Geometry
{
	constexpr float kPi = 3.14159265f;
	constexpr float kRadToDeg = 180 / kPi;
	constexpr float kDegToRad = kPi / 180;

	// movePos��basePos����]�̒��S��Y��������rad��]�����ʒu��Ԃ�
	Vector3 GetRotatedPosUpVecY(const Vector3& movePos, const Vector3& basePos, const float rad);

	// GetRotatedPosUpVecY�̊֐��̍s���Ԃ�Ver
	MATRIX GetRotateMatrixUpVecY(const Vector3& movePos, const Vector3& basePos, const float rad);

	// GetRotatedPosUpVecY�̔C�Ӊ�]��Ver
	Vector3 GetRotatedPosAnyUpVec(const Vector3& movePos, const Vector3& basePos, const Vector3& axis, const float rad);

	// ���3�����x�N�g���̂Ȃ��p(�x��)
	float Corner(const Vector3& a, const Vector3& b);
}