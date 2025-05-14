#pragma once
#include "Actor.h"

class PolygonCollider;

// �n�ʃN���X
// �e�X�g����
class Ground : public Actor
{
public:
	Ground();
	~Ground();
	
	void Init(const Vector3& initPos);

	void Update() override;
	void Draw() const override;

	void OnCollision(std::shared_ptr<Actor> other) override;

	void CommitMove() override;

private:
	int m_model;
};