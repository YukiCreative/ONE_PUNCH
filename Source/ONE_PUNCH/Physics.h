#pragma once
#include <memory>
#include <list>

class Actor;
class Collider3D;

// �R���C�_�[�𓮂����A�����蔻����s���A�����߂����v�Z������
// ActorController������
class Physics
{
public:

	// ActorController����ړ��Ɠ����蔻��𕪗�����
	void Update(std::list<std::shared_ptr<Actor>> actorList);

	// �����蔻��͈̔͂�`��
	// �f�o�b�O�p
	void DrawColRange(std::list <std::shared_ptr<Actor>>) const;
private:
	// �d�͂�����
	void Gravity(std::list<std::shared_ptr<Actor>> actorList);
	// �����蔻��
	void CheckHit(std::list<std::shared_ptr<Actor>>& actorList);
};

