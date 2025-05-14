#pragma once
#include <memory>
#include "Vector3.h"

class Collider3D;
class Rigid;

enum class ColKind3D;

// Collider3D��Rigid�܂Ƃ߂�
class Collidable
{
public:
	Collidable();

	// �O�Ő������Ă���ɂԂ�����
	void Init(std::shared_ptr<Collider3D> col, std::shared_ptr<Rigid> rigid);

	// Collider,Rigid�̕ϐ����������o�R�����Ɏ擾�ł���
	const Vector3   GetVel()     const;
	const ColKind3D GetColKind() const;
	const bool      IsThrough()  const;
	const bool      IsStatic()   const;
	const bool      IsStop()     const;
	const int       GetWeight()  const;
	const Vector3   GetPos()     const;
	const float     GetBounce()  const;

	void SetVel(const Vector3& vel);
	void SetPos(const Vector3& pos);

	bool HasCol() const;
	Collider3D& GetCol() const { return *m_col; }
	bool HasRigid() const;
	Rigid& GetRigid() const { return *m_rigid; }
private:
	std::shared_ptr<Collider3D> m_col;
	std::shared_ptr<Rigid> m_rigid;
};

