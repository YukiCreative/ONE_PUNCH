#include "Scene.h"
#include "Image.h"
#include <string>

namespace
{
	// ただの黒い板
	const std::string kFadeName = "Fade.png";
	// 時間ないし　定数にしよ
	constexpr float kFadeSpeed = 4.0f;
}

Scene::Scene()
{
	m_fadePanel = std::make_shared<Image>();
	m_fadePanel->Init(kFadeName);
	m_fadePanel->SetImageBlendMode(DX_BLENDMODE_ALPHA);
}

void Scene::FadeInUpdate()
{          
	m_fadePanel->SetImageBlendPal(m_fadePanel->GetBlendParam() - kFadeSpeed);
}

void Scene::FadeOutUpdate()
{
	m_fadePanel->SetImageBlendPal(m_fadePanel->GetBlendParam() + kFadeSpeed);
}
