#include "Actor.h"
#include "ActorController.h"
#include "Physics.h"
#include <DxLib.h>
#include <list>

void ActorController::Init()
{
	m_physics = std::make_shared<Physics>();
}

void ActorController::AddActor(std::shared_ptr<Actor> addActor)
{
	m_actors.emplace_back(addActor);
}

void ActorController::Update()
{
	ActorList_t aliveActors;
	for (auto& actor : m_actors)
	{
		actor->Update();
		// �����Ă����X�g�����
		if (actor->IsAlive())
		{
			aliveActors.emplace_back(actor);
		}
		// ����Actor�𐶐����Ă�����
		if (actor->IsSpawnedActor())
		{
			// ������E���Ă������ActorList�ŊǗ�����
			aliveActors.sort();
			aliveActors.merge(actor->GetSpawnActor());
			actor->ClearActorList();
		}
	}
	// m_actors�𐶂��Ă������ɂ���
	m_actors = aliveActors;

	// �d�͂Ƃ������蔻��Ƃ�
	m_physics->Update(m_actors);

	// �C����̈ړ��ʔ��f
	for (auto& actor : m_actors)
	{
		actor->CommitMove();
	}
}

void ActorController::Draw() const
{
	for (auto& actor : m_actors)
	{
		actor->Draw();
	}

#if _DEBUG
	// �����蔻��̉���
	m_physics->DrawColRange(m_actors);
#endif
}

bool ActorController::CheckIsAlive(std::shared_ptr<Actor> checkAct)
{
	for (const auto& actor : m_actors)
	{
		if (actor.get() == checkAct.get())
		{
			return true;
		}
	}
	return false;
}