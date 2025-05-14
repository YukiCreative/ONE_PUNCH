#include "Application.h"
#include "Game.h"
#include "Input.h"
#include "SceneController.h"
#include <DxLib.h>
#include "EffectManager.h"
#include <EffekseerForDXLib.h>
#include "SoundManager.h"

#if _DEBUG
namespace
{
	// �R���\�[���p
	FILE* in = 0;
	FILE* out = 0;
}
#endif

Application::Application() :
	m_isRunning(true)
{
}

Application& Application::GetInstance()
{
	static Application instance;
	return instance;
}

bool Application::Init()
{
	/*�R���\�[��Debug�p*/
#if _DEBUG
	AllocConsole();							// �R���\�[��
	freopen_s(&out, "CON", "w", stdout);	// stdout
	freopen_s(&in, "CON", "r", stdin);		// stdin
#endif

	// �������O�ɕK�v�ȏ��̒�`
	ChangeWindowMode(Game::kInitWindowMode);
	SetGraphMode(Game::kScreenWidth, Game::kScreenHeight, Game::kColorBitDepth);

	// DxLib�̏������ŁA�G�������瑦�ُ�l��Ԃ�
	if (DxLib_Init() == -1) return false;

	// ����ʂɕ`�悵�܂�
	SetDrawScreen(DX_SCREEN_BACK);

	SetUseZBuffer3D(true);
	SetWriteZBuffer3D(true);
	SetUseBackCulling(true);

	return true;
}

void Application::Run() const
{
	SceneController& scenes = SceneController::GetInstance();
	Input& input = Input::GetInstance();
	EffectManager& effect = EffectManager::GetInstance();
	SoundManager& sound = SoundManager::GetInstance();
	scenes.Init();
	input.Init();
	effect.Init();
	sound.Init();

	// �Q�[�����[�v
	while (ProcessMessage() == 0)
	{
		// �J�n���̃R���s���[�^�̎��Ԃ��擾
		long long beforFrameTime = GetNowHiPerformanceCount();

		// ��ʂ����ꂢ��
		ClearDrawScreen();

		input.Update();

		// �����ɃQ�[���̏���������
		scenes.Update();
		scenes.Draw();
		effect.Update();
		effect.Draw();
		sound.Update();
#if _DEBUG
		input.Draw_Debug();
#endif

		// �`�悵������ʂ𔽉f
		ScreenFlip();

		// 60fps�ɂȂ�悤�ɒ������Ă�
		while (GetNowHiPerformanceCount() - beforFrameTime < 16667);

		// �I�����߂��o�Ă����烋�[�v�𔲂���
		if (!m_isRunning) break;
	}
}

void Application::Tarminate() const
{
	// Effekseer�̂��Ƃ��܂�
	Effkseer_End();

	// DxLib�̏I������
	DxLib_End();

#if _DEBUG//�R���\�[��Debug�p
	fclose(out); fclose(in); FreeConsole();//�R���\�[�����
#endif
}

void Application::QuitGame()
{
	m_isRunning = false;
}