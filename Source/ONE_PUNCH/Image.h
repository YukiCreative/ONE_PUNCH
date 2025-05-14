#pragma once
#include <string>
#include "Vector2.h"
#include "Geometry.h"
#include "RangeLimitedValue.h"

using BlendParam = RangeLimitedValue<float, 0.0f, 255.0f>;

// �摜��\������R���|�[�l���g���ۂ����
class Image
{
private:
	int     m_sourceHandle;
	float   m_expandRate;
	float   m_angle;
	int     m_blendMode;
	BlendParam m_blendPal;
	bool    m_reverceX;
	bool    m_reverceY;
	Vector2 m_offset;
public:
	Image();

	void Init(const int& handle);
	void Init(const std::string& fileName);

	void Draw(const Vector2& pos) const;

	void SetGraph(const std::string& fileName);

	// ���݂̉��{�̑傫���ɂ��邩
	void ExpandGraph(const float& exRate);
	// ���
	void SetExRate(const float& value)    { m_expandRate = value; }
	// ���󂩂牽�x��]�����邩
	void RotateGraph(const float& deg)    { m_angle += deg * Geometry::kDegToRad; }
	// ���
	void SetAngle(const float& angleDeg)  { m_angle = angleDeg * Geometry::kDegToRad; }
	// SetDrawBlendMode�����̂܂ܕ\�Ɍ����Ă���
	void SetImageBlendMode(const int& blendmode, const float& pal)
	{
		m_blendMode = blendmode;
		m_blendPal = pal;
	}
	void SetImageBlendMode(const int& blendMode) { m_blendMode = blendMode; }
	void SetImageBlendPal (const float& pal)       { m_blendPal = pal; }
	void SetReverceFlagX  (const bool& value)	 { m_reverceX = value; }
	void SetReverceFlagY  (const bool& value)	 { m_reverceY = value; }
	void SetOffset		  (const Vector2& value) { m_offset = value; }
	int GraphHandle() const { return m_sourceHandle; }
	
	const float& ExRate() const { return m_expandRate; }
	const float GetBlendParam() const { return m_blendPal.Value(); }
};

