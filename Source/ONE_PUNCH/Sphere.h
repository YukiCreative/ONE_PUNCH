#pragma once
#include "Vector3.h"
#include "Color.h"

// DrawSphere3D���N���X������
class Sphere
{
private:
	float m_radius;
	int m_divNum;
	Color::ColorCode_t m_difColor;
	Color::ColorCode_t m_spcColor;
	bool m_isFill;
public:
	void Init(const float radius, const int divNum, const Color::ColorCode_t difColor, const Color::ColorCode_t spcColor, bool fillFlag);

	// ���W�����͂��̃N���X�������Ă���N���X����Ⴄ
	void Draw(const Vector3& pos) const;

	void SetDifColor(const Color::ColorCode_t difColor) { m_difColor = difColor; }
};