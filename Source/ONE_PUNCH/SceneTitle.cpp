#include "ButtonSystem.h"
#include "Game.h"
#include "SceneController.h"
#include "SceneTitle.h"
#include "SceneTest.h"
#include <DxLib.h>
#include "SkyBox.h"
#include "ActorController.h"
#include "Image.h"
#include "ScoreManager.h"
#include "Music.h"
#include "SoundManager.h"

namespace
{
	const std::string kTitleLogo = "ONEPUNCH.png";
	const Vector2 kTitleLogoPos = { Game::kScreenHalfWidth, Game::kScreenHalfHeight -100 };
	const Vector2 kStartButtonInitPos = {Game::kScreenHalfWidth-300,Game::kScreenHalfHeight+200};
	const Vector2 kQuitButtonInitPos = {Game::kScreenHalfWidth + 300,Game::kScreenHalfHeight+200};

	// ��
	const std::string kTitleBGM = "Data/Music/Title.wav";
	const std::string kSubmitSound = "Submit.mp3";
	const std::string kSelectSound = "Select.mp3";
}

SceneTitle::SceneTitle() :
	m_update(&SceneTitle::FadeInUpdate)
{
}

void SceneTitle::Init()
{
	m_actors = std::make_shared<ActorController>();
	m_actors->Init();

	auto sky = std::make_shared<SkyBox>();
	sky->Init();
	m_actors->AddActor(sky);

	m_titleLogo = std::make_shared<Image>();
	m_titleLogo->Init(kTitleLogo);
	m_titleLogo->SetExRate(0.8f);

	// �{�^���̐ݒ�
	m_buttons = std::make_shared<ButtonSystem>();
	auto buttonStartGame = std::make_shared<ButtonStartGame>();
	auto buttonQuitGame = std::make_shared<ButtonQuitGame>();
	buttonStartGame->Init(kStartButtonInitPos, weak_from_this());
	buttonQuitGame->Init(kQuitButtonInitPos);
	buttonStartGame->SetRightButton(buttonQuitGame);
	buttonStartGame->SetLeftButton(buttonQuitGame);
	buttonQuitGame->SetRightButton(buttonStartGame);
	buttonQuitGame->SetLeftButton(buttonStartGame);
	m_buttons->AddButton(buttonStartGame);
	m_buttons->AddButton(buttonQuitGame);
	m_buttons->SetButtonFocus(buttonStartGame);
}

void SceneTitle::Update()
{
	(this->*m_update)();
}

void SceneTitle::Draw() const
{
	m_actors->Draw();
	m_buttons->Draw();
	m_titleLogo->Draw(kTitleLogoPos);
#if _DEBUG
	DrawString(0, 0, "TitleScene", 0xffffff);
#endif

	// �S�Ă̏�Ƀt�F�[�h
	m_fadePanel->Draw({Game::kScreenHalfWidth, Game::kScreenHalfHeight});
}

void SceneTitle::Entry()
{
	// �Ȃ��Đ�
	Music::GetInstance().Play(kTitleBGM);
	// �^������
	m_fadePanel->SetImageBlendPal(255);
}

void SceneTitle::Reave()
{
}

void SceneTitle::GameStart()
{
	m_update = &SceneTitle::FadeOutUpate;
}

void SceneTitle::FadeInUpdate()
{
	Scene::FadeInUpdate();
	if (m_fadePanel->GetBlendParam() <= 0)
	{
		m_update = &SceneTitle::NormalUpdate;
		return;
	}
}

void SceneTitle::NormalUpdate()
{
	m_actors->Update();
	m_buttons->Update();
}

void SceneTitle::FadeOutUpate()
{
	Scene::FadeOutUpdate();
	// ���S�ɍ����Ȃ�����
	if (m_fadePanel->GetBlendParam() >= 255)
	{
		// �ǂ����Œ肾��������
		SceneController::GetInstance().ChangeScene(std::make_shared<SceneTest>());
		return;
	}
}

// �V�[���Ŏg���{�^���̏������V�[����cpp�ɓ���Ă݂�
// ================================================

namespace
{
	const std::string kForcusedStartButton = "Start.png";
	const std::string kDisforcusedStartButton = "Start_NoF.png";
}

void SceneTitle::ButtonStartGame::Init(const Vector2& initPos, std::weak_ptr<SceneTitle> title)
{
	Button::Init(initPos);
	m_image = std::make_shared<Image>();
	m_image->Init(kDisforcusedStartButton);
	m_title = title;
}

void SceneTitle::ButtonStartGame::OnFocused()
{
	// ���Ȃ炷
	SoundManager::GetInstance().Play(kSelectSound);
	m_image->SetGraph(kForcusedStartButton);
	// �ł�������
	m_image->SetExRate(1.2f);
}

void SceneTitle::ButtonStartGame::OnDisfocused()
{
	m_image->SetGraph(kDisforcusedStartButton);
	// ����������
	m_image->SetExRate(1.0f);
}

void SceneTitle::ButtonStartGame::OnPressed()
{
	// �X�R�A�̏�����
	ScoreManager::GetInstance().Reset();
	// ����炷
	SoundManager::GetInstance().Play(kSubmitSound);

	// �Q�[���X�^�[�g
	m_title.lock()->GameStart();
}

void SceneTitle::ButtonStartGame::Draw() const
{
	m_image->Draw(m_pos);
}

void SceneTitle::ButtonStartGame::FocusedUpdate()
{
}

void SceneTitle::ButtonStartGame::NormalUpdate()
{
}

// ====================================

#include "Application.h"

namespace
{
	const std::string kFocusedQuitButton = "Exit.png";
	const std::string kDisfocusedQuitButton = "Exit_NoF.png";
}

void SceneTitle::ButtonQuitGame::Init(const Vector2& initPos)
{
	Button::Init(initPos);
	m_image = std::make_shared<Image>();
	m_image->Init(kDisfocusedQuitButton);
}

void SceneTitle::ButtonQuitGame::OnFocused()
{
	// ���Ȃ炷
	SoundManager::GetInstance().Play(kSelectSound);
	m_image->SetGraph(kFocusedQuitButton);
	// �ł�������
	m_image->SetExRate(1.2f);
}

void SceneTitle::ButtonQuitGame::OnDisfocused()
{
	m_image->SetGraph(kDisfocusedQuitButton);
	// ����������
	m_image->SetExRate(1.0f);
}

void SceneTitle::ButtonQuitGame::OnPressed()
{
	// ����炷
	SoundManager::GetInstance().Play(kSubmitSound);
	Application::GetInstance().QuitGame();
}

void SceneTitle::ButtonQuitGame::Draw() const
{
	m_image->Draw(m_pos);
}

void SceneTitle::ButtonQuitGame::FocusedUpdate()
{
}

void SceneTitle::ButtonQuitGame::NormalUpdate()
{
}
