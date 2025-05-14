#include "SceneGameOver.h"
#include "ButtonSystem.h"
#include "SceneController.h"
#include "SceneTitle.h"
#include "Game.h"
#include <DxLib.h>

namespace
{
	const Vector2 kButtonInitPos = {0,0};
}

SceneGameOver::SceneGameOver()
{
}

void SceneGameOver::Init()
{
	m_buttons = std::make_shared<ButtonSystem>();
	auto buttonReturnToTitle = std::make_shared<ButtonReturnToTitle>();
	buttonReturnToTitle->Init(kButtonInitPos);
	m_buttons->AddButton(buttonReturnToTitle);
	m_buttons->SetButtonFocus(buttonReturnToTitle);
}

void SceneGameOver::Update()
{
	m_buttons->Update();
}

void SceneGameOver::Draw() const
{
	m_buttons->Draw();

	DrawString(0, 0, "がめおヴぇｒ", 0xffffff);
	DrawString(Game::kScreenHalfWidth, Game::kScreenHalfHeight, "死んだぜ", 0xffffff);
	DrawString(Game::kScreenHalfWidth, Game::kScreenHalfHeight + 15, "ＡかEnterでタイトル", 0xffffff);
}

void SceneGameOver::Entry()
{
}

void SceneGameOver::Reave()
{
}

// 以下、ボタンの定義
// ========================================

void SceneGameOver::ButtonReturnToTitle::Init(const Vector2& initPos)
{
	Button::Init(initPos);
}

void SceneGameOver::ButtonReturnToTitle::Draw() const
{
}

void SceneGameOver::ButtonReturnToTitle::OnFocused()
{
}

void SceneGameOver::ButtonReturnToTitle::OnDisfocused()
{
}

void SceneGameOver::ButtonReturnToTitle::OnPressed()
{
	SceneController::GetInstance().ChangeScene(std::make_shared<SceneTitle>());
}

void SceneGameOver::ButtonReturnToTitle::FocusedUpdate()
{
}

void SceneGameOver::ButtonReturnToTitle::NormalUpdate()
{
}
