#pragma once
#include "Vector3.h"

// �G�t�F�N�g�N���X
// ����Ȃɂ�点�邱�ƂȂ���
// �n���h��������Ȃ�Ɋi�����������Ă邾��
class EffekseerEffect
{
public:
	EffekseerEffect();
	~EffekseerEffect();

	void Init(const int handle, const Vector3& initPos);
	
	// �ʒu�ݒ�
	void SetPos   (const Vector3& pos);
	// ��]
	void SetRotate(const Vector3& rad);
	// �g�k
	void SetScale (const Vector3& scale);
	// �Đ����x
	void SetPlaySpeed(const float speed);
	// ���I�p�����[�^��ݒ肷��
	void SetDynamicParam(const float x, const float y, const float z, const float w);
	// �F
	void SetColor(const int r, const int g, const int b, const int a);

	const int  GetHandle() const { return m_handle; }
	const bool IsPlaying() const;
	// ���̃G�t�F�N�g���폜
	void Kill();
protected:
	int m_handle;
};

