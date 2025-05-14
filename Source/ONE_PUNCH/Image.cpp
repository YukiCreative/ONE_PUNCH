#include "Image.h"
#include "Vector2.h"
#include <cassert>
#include <DxLib.h>
#include <string>
#include "ImageStore.h"

Image::Image() :
	m_expandRate(1.0f),
	m_angle     (0.0f),
	m_blendMode (DX_BLENDMODE_NOBLEND),
	m_blendPal  (0),
	m_reverceX  (false),
	m_reverceY  (false),
	m_offset    (),
	m_sourceHandle(-1)
{
}

void Image::Init(const int& handle)
{
	m_sourceHandle = handle;
}

void Image::Init(const std::string& fileName)
{
	SetGraph(fileName);
}

void Image::Draw(const Vector2& pos) const
{
	Vector2 correctedPos = pos + m_offset;
	SetDrawBlendMode(m_blendMode, static_cast<int>(m_blendPal.Value()));
	DrawRotaGraph(static_cast<int>(correctedPos.x), static_cast<int>(correctedPos.y), static_cast<double>(m_expandRate),
		static_cast<double>(m_angle), m_sourceHandle, true, m_reverceX, m_reverceY);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void Image::SetGraph(const std::string& fileName)
{
	// �n���h����Ⴄ
	m_sourceHandle = ImageStore::GetInstance().GetGraph(fileName);
}

void Image::ExpandGraph(const float& exRate)
{
	m_expandRate *= exRate;
}
