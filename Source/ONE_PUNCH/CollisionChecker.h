#pragma once

#include "SphereCollider.h"
#include "PolygonCollider.h"
#include <array>

class Actor;
class Collidable;
class Physics;

struct PolyHitData
{
	using Vertices = std::array<Vector3, 3>;
	const float kBigNum =3.4e38f;
	float time;
	// �|���S�����`�����钸�_3��
	// ���v���ɒ��_������
	Vertices vertices;
	Vector3 normal; // �@���B���_���狁�܂邯�ǁA����̓r���Ōv�Z����̂œ���Ƃ�

	PolyHitData();
	PolyHitData(const float _time, const Vertices& _vertices, const Vector3& _normal);
	void operator=(const PolyHitData& other);
};

// �Փ˔���̊֐����܂Ƃ߂܂���
// Physics�ɏ����ƒ����̂�
class CollisionChecker
{
	friend Physics;
private:
	
	// CheckHit�Z�Z�n
	// Collider�̎�ނɉ����������蔻��̏���
	// �����������ǂ�����Ԃ��@���̂ق��ɕԂ������f�[�^����������Q�Ƃ�v������
	// ����炷�ׂĂ̊֐��ŁA���̃t���[���̈ړ��ʂ��������Ĕ��肷��

	// FixMove�Z�Z�n
	// ���ۂɓ�����u��(������Ȃ��Ȃ�ʒu)�܂ňړ������A
	// ����ɂ��̌�̈ړ����x��������

	// ������
	/// <param name="colA">���̂`</param>
	/// <param name="colB">���̂a</param>
	/// <param name="time">�Փˎ��Ԃ��i�[����|�C���^(�Ԃ��l�͈̔�0~1)</param>
	/// <returns>�����������ǂ���</returns>
	static bool CheckHitSS(const Collidable& colA, const Collidable& colB, float& time);
	/// <param name="colA">����A</param>
	/// <param name="colB">����B</param>
	/// <param name="time">�Փˎ���</param>
	static void FixMoveSS(Collidable& colA, Collidable& colB, const float time);

	// ����x�|���S��

	// A��B���ׂāA���Փˎ��Ԃ���������A�ɑ�����܂��B
	static void ComparePolyHit(PolyHitData& a, const PolyHitData& b);

	/// <param name="sCol">����</param>
	/// <param name="pCol">�|���S��</param>
	/// <param name="hit">��ԋ߂��Փˎ��ԁ��|���S�����Ԃ��Ă���</param>
	/// <returns>�����������ǂ���</returns>
	static bool CheckHitSP(const Collidable& sCol, const Collidable& pCol, PolyHitData& hit);
	static void FixMoveSP(Collidable& sphereCol, Collidable& polygonCol, const PolyHitData& hitData);
};