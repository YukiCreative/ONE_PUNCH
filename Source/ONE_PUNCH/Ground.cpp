#include "Ground.h"
#include "PolygonCollider.h"
#include <Dxlib.h>
#include <string>
#include "Rigid.h"
#include "Collidable.h"

namespace
{
	// static�Ȃ̂ŏd���͈Ӗ��Ȃ�
	constexpr int kWeight = 99;
	constexpr bool kIsThrough = false;
	constexpr bool kIsStatic = true;
	// �Q�Ɨp���b�V���̂��߂̒萔
	constexpr bool kUseTransform = true;
	const std::string kModelPath = "Data/Model/Plane.mv1";
	const PhysicalMaterial kPhysicalMaterial =
	{
		1.0f, // �n�㖀�C
		1.0f, // �󒆖��C
		0.1f, // �����W��
	};
}

Ground::Ground() :
	m_model(-1)
{
}

Ground::~Ground()
{
	MV1DeleteModel(m_model);
}

void Ground::Init(const Vector3& initPos)
{
	m_kind = ActorKind::kFloor;
	m_model = MV1LoadModel(kModelPath.c_str());
	m_pos = initPos;
	auto polyCol = std::make_shared<PolygonCollider>();
	polyCol->Init(initPos, kWeight, kIsThrough, kIsStatic, m_model, kUseTransform);
	auto rigid = std::make_shared<Rigid>();
	rigid->Init(kPhysicalMaterial);
	m_collidable = std::make_shared<Collidable>();
	m_collidable->Init(polyCol, rigid);
}

void Ground::Update()
{
	MV1SetPosition(m_model, m_pos);
}

void Ground::Draw() const
{
	MV1DrawModel(m_model);
}

void Ground::OnCollision(std::shared_ptr<Actor> other)
{
	printfDx("�|���S������������\n");
}

void Ground::CommitMove()
{
	m_pos += m_collidable->GetRigid().Update();
	
}
