#pragma once
#include "Actor.h"
#include <memory>

class Model;

// ‚¨‹ó
class SkyBox : public Actor
{
public:
	SkyBox();

	void Init();

	void Update() override;
	void Draw() const override;

	void CommitMove() override;
private:
	std::shared_ptr<Model> m_model;
};

