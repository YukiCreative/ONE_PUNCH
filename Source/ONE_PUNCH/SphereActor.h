#pragma once
#include "Actor.h"
#include <memory>
#include "Color.h"

class Sphere;
class SphereCollider;

// Sphere‚ðActor‚É‚µ‚½
// ƒeƒXƒg
class SphereActor final : public Actor
{
public:

	void Init(const Vector3& initPos, const float radius, const int divNum, const Color::ColorCode_t difColor, const Color::ColorCode_t spcColor, bool isFill);
	void Update() override;
	void Draw() const override;

	void CommitMove() override;

	void OnCollision(std::shared_ptr<Actor> other) override;
private:
	std::shared_ptr<Sphere> m_sphere;
};

