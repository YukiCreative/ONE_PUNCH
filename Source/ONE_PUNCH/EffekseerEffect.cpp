#include "EffekseerEffect.h"
#include <EffekseerForDXLib.h>

EffekseerEffect::EffekseerEffect() :
	m_handle(-1)
{
}

EffekseerEffect::~EffekseerEffect()
{
	// ���̃C���X�^���X�����񂾂Ƃ��A�G�t�F�N�g�����ʂ悤�ɂ���
	Kill();
}

void EffekseerEffect::Init(const int handle, const Vector3& initPos)
{
	m_handle = handle;

	SetPos(initPos);
}

void EffekseerEffect::SetPos(const Vector3& pos)
{
	SetPosPlayingEffekseer3DEffect(m_handle, pos.x, pos.y, pos.z);
}

void EffekseerEffect::SetRotate(const Vector3& rad)
{
	SetRotationPlayingEffekseer3DEffect(m_handle, rad.x, rad.y, rad.z);
}

void EffekseerEffect::SetScale(const Vector3& scale)
{
	SetScalePlayingEffekseer3DEffect(m_handle, scale.x, scale.y, scale.z);
}

void EffekseerEffect::SetPlaySpeed(const float speed)
{
	SetSpeedPlayingEffekseer3DEffect(m_handle, speed);
}

const bool EffekseerEffect::IsPlaying() const
{
	// �Đ��� = 0,��~��or�Đ��I�� = -1
	return IsEffekseer3DEffectPlaying(m_handle) == 0;
}

void EffekseerEffect::SetDynamicParam(const float x, const float y, const float z, const float w)
{
	SetDynamicInput3DEffect(m_handle, 0, x);
	SetDynamicInput3DEffect(m_handle, 1, y);
	SetDynamicInput3DEffect(m_handle, 2, z);
	SetDynamicInput3DEffect(m_handle, 3, w);
}

void EffekseerEffect::SetColor(const int r, const int g, const int b, const int a)
{
	SetColorPlayingEffekseer3DEffect(m_handle, r, g, b, a);
}

void EffekseerEffect::Kill()
{
	// �Ƃ肠�����Đ����~�߂��Manager�������Ă����
	// ���̎d�l�͂̂��ɓs���������ĕύX���邩��
	StopEffekseer3DEffect(m_handle);
}