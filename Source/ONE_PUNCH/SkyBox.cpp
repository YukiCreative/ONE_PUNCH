#include "SkyBox.h"
#include "Model.h"
#include <DxLib.h>

namespace
{
	const std::string kModelPath = "Data/Model/Sky_Daylight.pmx";
	constexpr float kRotateSpeed = 0.0001f;
	const Vector3 kScale = {2.0f, 2.0f, 2.0f};
	const Vector3 kPos = { 0, 1000, 0 };
}

SkyBox::SkyBox()
{
}

void SkyBox::Init()
{
	m_model = std::make_shared<Model>();
	m_model->Init(kModelPath);
	m_collidable = nullptr;
	m_model->SetScale(kScale);
	m_model->SetPos(kPos);
}

void SkyBox::Update()
{
	// ƒ‚ƒfƒ‹‚ð‰ñ“]‚³‚¹‚é
	m_model->RotateUpVecY(kRotateSpeed);
}

void SkyBox::Draw() const
{
	m_model->Draw();
}

void SkyBox::CommitMove()
{
}
