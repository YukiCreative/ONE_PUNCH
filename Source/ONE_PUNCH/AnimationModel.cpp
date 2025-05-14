#include "AnimationModel.h"
#include <DxLib.h>
#include "Model.h"
#include <cassert>
#include "Game.h"

void AnimationModel::ChangeAnimation(const int animId, bool isLoop)
{
	m_isLoop = isLoop;
	m_isPlaying = true;

	// �܂��f�^�b�`
	MV1DetachAnim(m_model->m_handle, m_animHandle);

	m_animHandle = MV1AttachAnim(m_model->m_handle, animId);
	assert(m_animHandle != -1 && "�A�j���[�V�����̃A�^�b�`�Ɏ��s����");

	m_playTime = 0;

	// �A�j���[�V�����̎��Ԃ�c�����Ă���
	m_animTotalTime = MV1GetAttachAnimTotalTime(m_model->m_handle, m_animHandle);
}

void AnimationModel::ChangeAnimation(const std::string& animName, bool isLoop)
{
	const int animId = MV1GetAnimIndex(m_model->m_handle, animName.c_str());
	ChangeAnimation(animId, isLoop);
}

void AnimationModel::RotateUpVecY(const float rad)
{
	m_model->RotateUpVecY(rad);
}

void AnimationModel::RotateUpVecZ(const float rad)
{
	m_model->RotateUpVecZ(rad);
}

void AnimationModel::SetPos(const Vector3& pos)
{
	m_model->SetPos(pos);
}

DxLib::tagMATRIX AnimationModel::GetMatrix() const
{
	return m_model->GetMatrix();
}

void AnimationModel::SetMatrix(const DxLib::tagMATRIX& mat)
{
	m_model->SetMatrix(mat);
}

const bool AnimationModel::CheckAnimName(const std::string name)
{
	int a = MV1GetAnimIndex(m_model->m_handle, name.c_str());
	int b = MV1GetAttachAnim(m_model->m_handle, m_animHandle);
	return  a == b;
}

AnimationModel::AnimationModel() :
	m_animHandle(-1),
	m_isPlaying(false),
	m_playTime(0),
	m_animTotalTime(0),
	m_isLoop(true),
	m_playSpeed(1.0f)
{
}

AnimationModel::~AnimationModel()
{
}

void AnimationModel::Init(const std::string& path, const float animSpeed)
{
	m_model = std::make_shared<Model>();
	m_model->Init(path);
	m_playSpeed = animSpeed;
}

void AnimationModel::Init(const int handle, const float animSpeed)
{
	m_model = std::make_shared<Model>();
	m_model->Init(handle);
	m_playSpeed = animSpeed;
}

void AnimationModel::Update()
{
	if (!m_isPlaying) return;

	// �A�j���[�V�����̎��Ԃ�i�߂�
	// �����P�ʂ̓~���b
	m_playTime += m_playSpeed / Game::kFrameRate;

	if (m_playTime > m_animTotalTime)
	{
		// ���[�v���Ă����Ȃ烋�[�v
		if (m_isLoop)
		{
			m_playTime -= m_animTotalTime;
		}
		else
		{
			m_isPlaying = false;
		}
	}

	MV1SetAttachAnimTime(m_model->m_handle, m_animHandle, m_playTime);
}

void AnimationModel::Draw(const Vector3& drawPos) const
{
	// model��Draw�������Ɉʒu������`�ł�������
	m_model->SetPos(drawPos);
	m_model->Draw();
}

void AnimationModel::Draw() const
{
	m_model->Draw();
}
