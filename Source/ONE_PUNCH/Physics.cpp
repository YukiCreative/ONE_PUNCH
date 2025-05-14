#include "Physics.h"
#include "Actor.h"
#include "Collider3D.h"
#include "SphereCollider.h"
#include "PolygonCollider.h"
#include "CollisionChecker.h"
#include "Collidable.h"
#include "Rigid.h"

namespace
{
	// �d�͂͒萔
	// ��ŕϐ��ɂ��邩���H�H�H
	const Vector3 kGravity = {0, -0.8f, 0};
}

void Physics::Update(std::list<std::shared_ptr<Actor>> actorList)
{
	// �d��
	Gravity(actorList);

	CheckHit(actorList);
}

void Physics::CheckHit(std::list<std::shared_ptr<Actor>>& actorList)
{
	for (auto& actA : actorList)
	{
		for (auto& actB : actorList)
		{
			if (!actA->HasCollidable() || !actB->HasCollidable() ||
				!actA->HasCol       () || !actB->HasCol       () ||
				!actA->HasRigid     () || !actB->HasRigid     ()) continue;

			// ����l���Ȃ�v�Z���Ȃ�
			if (actA == actB) continue;

			Collidable& colA = actA->GetCollidable();
			Collidable& colB = actB->GetCollidable();
			const ColKind3D colKindA = colA.GetColKind();
			const ColKind3D colKindB = colB.GetColKind();

			// �ǂ�������t���[���ɓ����Ă��Ȃ���Γ������Ă��Ȃ�
			if (colA.IsStop() && colB.IsStop()) continue;

			// ���蔲���邩�ǂ���
			const bool skipPushBack = colA.IsThrough() || colB.IsThrough();
			bool hitResult = false;

			// ���Collider�̎�ނɉ����������蔻��֐����Ă�
			if (colKindA == ColKind3D::kSphere && colKindB == ColKind3D::kSphere)
			{
				float time;
				hitResult = CollisionChecker::CheckHitSS(colA, colB, time);

				if (hitResult)
				{
					// �����߂�����
					// ���ߐݒ���m�F���āA�ǂ�����s���߂Ȃ牟���߂�
					if (!skipPushBack)
					{
						// �����߂�
						CollisionChecker::FixMoveSS(colA, colB, time);
					}
				}
			}
			else if (colKindA == ColKind3D::kSphere && colKindB == ColKind3D::kPolygon)
			{
				PolyHitData hitData;
				// ���ꔽ�΂̃P�[�X���񋓂��Ȃ��Ƃ����Ȃ��̃S�~�R�[�h�߂���H
				hitResult = CollisionChecker::CheckHitSP(colA, colB, hitData);

				if (hitResult)
				{
					// �����߂�
					if (!skipPushBack)
					{
						CollisionChecker::FixMoveSP(colA, colB, hitData);
					}
				}
			}
			// ���̓����蔻��𑝂₵�����Ƃ��͂�����else�łȂ���

			if (hitResult)
			{
				actA->OnCollision(actB);
				actB->OnCollision(actA);
			}
		}
	}
}

void Physics::DrawColRange(std::list<std::shared_ptr<Actor>> actorList) const
{
	// Collider�̕`��֐����Ă� ���ꂾ��
	for (auto& actor : actorList)
	{
		if (!actor->HasCollidable() || !actor->HasCol()) continue;
		actor->GetCol().Draw();
	}
}

void Physics::Gravity(std::list<std::shared_ptr<Actor>> actorList)
{
	for (auto& actor : actorList)
	{
		// �����Ă��Ȃ��\��������
		if (!actor->HasCollidable() || !actor->HasRigid()) continue;

		// static�̂�͓������Ȃ�
		if (actor->GetCol().IsStatic()) continue;

		auto& rigid = actor->GetRigid();
		rigid.AddVel(kGravity);
	}
}
