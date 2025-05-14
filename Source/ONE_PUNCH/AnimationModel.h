#pragma once
#include <memory>
#include <string>
#include "Vector3.h"

class Model;

namespace DxLib
{
	struct tagMATRIX;
}

// 3D���f�������������Ɏg���₷���A�j���[�V����������
// Model���p�����Ă��܂����l����
class AnimationModel
{
public:
	AnimationModel();
	~AnimationModel();

	void Init(const std::string& path, const float animSpeed);
	// ���łɂ���n���h�����Q�Ƃ���p�^�[��
	// �f�X�g���N�^�Ń��f�����폜���Ȃ�
	void Init(const int handle, const float animSpeed);

	void Update();
	void Draw(const Vector3& drawPos) const;
	void Draw() const;

	void Stop() { m_isPlaying = false; }
	void Play() { m_isPlaying = true; }
	const bool IsPlaying() const { return m_isPlaying; }
	// �A�j���[�V�������I����Ă��邩�ǂ���
	const bool IsEnd() const { return !m_isPlaying; }
	// �A�j���[�V�����̃u�����h�͂ł��Ȃ�
	void ChangeAnimation(const int animId, bool isLoop = true);
	void ChangeAnimation(const std::string& animName, bool isLoop = true);

	// ���̕�Model�N���X�̒��p
	// Y������ɉ�]
	void RotateUpVecY(const float rad);
	void RotateUpVecZ(const float rad);
	void SetPos(const Vector3& pos);
	DxLib::tagMATRIX GetMatrix() const;
	void SetMatrix(const DxLib::tagMATRIX& mat);
	// ���̃A�j���[�V�������m���߂�Ƃ��Ɏg��
	const bool CheckAnimName(const std::string name);
private:
	std::shared_ptr<Model> m_model;

	int m_animHandle;
	float m_playTime;
	float m_playSpeed;
	bool m_isPlaying;
	float m_animTotalTime;
	bool m_isLoop;
};

