#include "CollisionChecker.h"
#include <DxLib.h>
#include "Actor.h"
#include "Collidable.h"
#include "Rigid.h"
//#include "Collider3D.h"
#include <array>

bool CollisionChecker::CheckHitSS(const Collidable& colA, const Collidable& colB, float& time)
{
	auto& sphereColA = static_cast<SphereCollider&>(colA.GetCol());
	auto& sphereColB = static_cast<SphereCollider&>(colB.GetCol());

	const Vector3 initColDist  = colB.GetPos() - colA.GetPos(); // �ړ��O���΋���
	const Vector3 afterColA    = colA.GetPos() + colA.GetVel(); // �ړ���A
	const Vector3 afterColB    = colB.GetPos() + colB.GetVel(); // �ړ���B
	const Vector3 afterColDist = afterColB     - afterColA;     // �ړ��㑊�΋���
	const Vector3 colDistDiff  = afterColDist  - initColDist;   // ���΋����̍���
	const float sumRad = sphereColA.GetRadius() + sphereColB.GetRadius();

	// �ŏ�����Փ˂��Ă��邩�ǂ���
	if (initColDist.SqrMagnitude() <= sumRad * sumRad)
	{
		time = 0;
		return true;
	}

	// ���s�ړ����Ă��邩
	if (colDistDiff.SqrMagnitude() == 0)
	{
		// ���͂��łɏ�ŕ��s�ړ����Փ˂̏ꍇ���L���b�`�ł��Ă���̂ŁA�����ɗ���Ƃ������Ƃ͓������Ă��Ȃ�
		return false;
	}

	const float dot = initColDist.Dot(colDistDiff);	// �������O�������Ȃ�
	const float colDistLength = initColDist.SqrMagnitude(); // �ړ��O���΋����̒���(�������Ŋ����Ă��Ȃ�)
	const float colDistDiffLength = colDistDiff.SqrMagnitude();

	// �Փ˂̔��ʎ�
	const float discriminant = dot*dot-colDistDiffLength*(colDistLength-sumRad*sumRad);
	// ��̒������̐��Ȃ���Ȃ�
	if (discriminant < 0)
	{
		return false;
	}

	time = (-dot - sqrtf(discriminant)) / colDistDiffLength;
	// ���̃t���[���ł͓�����Ȃ��p�^�[��
	if (time < 0 || time > 1)
	{
		return false;
	}

	// �����܂ŗ����瓖�����Ă���@���߂łƂ�
	return true;
}

void CollisionChecker::FixMoveSS(Collidable& colA, Collidable& colB, const float time)
{
	const Vector3 velA = colA.GetVel();
	const Vector3 velB = colB.GetVel();

	// ��̋��̂��A�ŏ��ɐڐG����ʒu���o��
	const Vector3 fixedColAPos = colA.GetPos() + velA * time;
	const Vector3 fixedColBPos = colB.GetPos() + velB * time;
	// A����B�̃x�N�g��
	// ���ꂪ���̂̏Փ˂̖@��
	const Vector3 diffN = (fixedColBPos - fixedColAPos).GetNormalize();
	// A��B�̃p�����[�^����A�����������߂�
	const float reboundRate = 1 + colA.GetBounce() * colB.GetBounce();
	// �d�ʂ��������
	const float totalWeight = static_cast<float>(colA.GetWeight() + colB.GetWeight());
	// ���𕪊����Ă邾��
	const float dot = (velA - velB).Dot(diffN);
	float weightFactA = -colB.GetWeight() / totalWeight;
	float weightFactB =  colA.GetWeight() / totalWeight;
	const Vector3 constVec = diffN * (reboundRate * dot);

	// ����A��B�ǂ��炩��static�Ȃ�A�����͓�������
	// �������ɂ��ׂĂ������t����
	if (colA.IsStatic())
	{
		weightFactA = 0;
		weightFactB = 1;
	}
	else if (colB.IsStatic())
	{
		weightFactA = 1;
		weightFactB = 0;
	}
	// �ǂ�����static�Ȃ����������
	if (colA.IsStatic() && colB.IsStatic())
	{
		weightFactA = 0;
		weightFactB = 0;
	}

	// �Փˌ�x�N�g���̎Z�o
	const Vector3 fixedVelA = constVec * weightFactA + velA;
	const Vector3 fixedVelB = constVec * weightFactB + velB;

	// �Z�o�����l�𔽉f
	colA.SetVel(fixedVelA);
	colB.SetVel(fixedVelB);
	colA.SetPos(fixedColAPos);
	colB.SetPos(fixedColBPos);
}

void CollisionChecker::ComparePolyHit(PolyHitData& a, const PolyHitData& b)
{
	if (a.time < b.time) return;

	a = b;
}

bool CollisionChecker::CheckHitSP(const Collidable& sCol, const Collidable& pCol, PolyHitData& hit)
{
	auto& sphereCol = static_cast<SphereCollider&>(sCol.GetCol());
	auto& polygonCol = static_cast<PolygonCollider&>(pCol.GetCol());

	bool isHit = false;

	const Vector3 sumVel = sCol.GetVel() - pCol.GetVel(); // ���̂��猩�����Α��x�ōl����

	// �܂��A�|���S���̃��b�V���̏�Ԃ��擾����
	// ���߂̍X�V����
	polygonCol.RefreshRefMesh();
	auto& refMesh = polygonCol.GetRefMesh();

	// �擾�����|���S�����ׂĂɑ΂��Ď��s(��΂���)
	for (int i = 0; i < refMesh.PolygonNum; ++i)
	{
		// �Ƃ肠�����K�v�ȏ����擾
		const auto& polygon = refMesh.Polygons[i];
		const auto& vertices = refMesh.Vertexs;
		// ���_�̍��W
		const std::array<Vector3, 3> polygonVertices =
		{
			vertices[polygon.VIndex[0]].Position,
			vertices[polygon.VIndex[1]].Position,
			vertices[polygon.VIndex[2]].Position,
		};
		DrawSphere3D(polygonVertices[0], 100, 10, 0xff0000, 0xff0000, true);
		DrawSphere3D(polygonVertices[1], 100, 10, 0xff0000, 0xff0000, true);
		DrawSphere3D(polygonVertices[2], 100, 10, 0xff0000, 0xff0000, true);
		// ��
		const std::array<Vector3, 3> polygonLines =
		{
			polygonVertices[1] - polygonVertices[0], // 0~1�Ɍ������x�N�g��
			polygonVertices[2] - polygonVertices[1], // 1~2
			polygonVertices[0] - polygonVertices[2], // 2~0
		};
		// (�ʂ�)�@��
		const Vector3 normal = polygonLines[2].Cross(polygonLines[0]).GetNormalize();
		DrawLine3D(pCol.GetPos(), pCol.GetPos() + normal * 500, 0xffffff);
		const float dot = sumVel.Dot(normal); // ���܂̈ړ������Ɩʂ̌����̊֌W���ǂꂮ�炢�����킩��
		const Vector3 c0 = sphereCol.GetPos() - polygonVertices[0]; // �ǂ����K���ȃ|���S���̒��_���狅�̌��݈ʒu�܂�
		const float dotC0 = c0.Dot(normal);
		const float distPlaneToPoint = fabsf(dotC0); // �ʂ���_�ւ̋���

		const float collideTime = (sphereCol.GetRadius() - dotC0) / dot; // ���ꂪ�Փˎ���

		if (dot >= 0) continue; // �ړ��������ʂɑ΂��ė����
		if (collideTime > 1 || collideTime < 0) continue; // ����̈ړ��ŏՓ˂��Ȃ��ꍇ

		// �Փ˂���
		// ���߂��Փ˂������ꍇ�̂ݍX�V
		ComparePolyHit(hit, PolyHitData(collideTime, polygonVertices, normal));
		//isHit = true;
	}

	return isHit;
}

void CollisionChecker::FixMoveSP(Collidable& _sphereCol, Collidable& _polygonCol, const PolyHitData& hitData)
{
	auto& sphereCol = static_cast<SphereCollider&>(_sphereCol.GetCol());
	auto& polygonCol = static_cast<PolygonCollider&>(_polygonCol.GetCol());

	// �ꏊ���Œ肵�Ă݂�
	_sphereCol.SetPos(_sphereCol.GetPos() + _sphereCol.GetVel() * hitData.time);
	_polygonCol.SetPos(_polygonCol.GetPos() + _polygonCol.GetVel() * hitData.time);

	// �o�E���h
	_sphereCol.GetRigid().SetVel(_sphereCol.GetVel() * (hitData.time - 1));
}

// ===============================================

PolyHitData::PolyHitData() :
	time(kBigNum),
	vertices(),
	normal()
{
}

PolyHitData::PolyHitData(const float _time, const Vertices& _vertices, const Vector3& _normal)
{
	time = _time;
	vertices = _vertices;
	normal = _normal;
}

void PolyHitData::operator=(const PolyHitData& other)
{
	time = other.time;
	vertices = other.vertices;
	normal = other.normal;
}