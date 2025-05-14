#include "ActorController.h"
#include "Bee.h"
#include "Camera.h"
#include "Color.h"
#include "EffectManager.h"
#include "EnemyGenerator.h"
#include "Game.h"
#include "Ground.h"
#include "Input.h"
#include "Player.h"
#include "SceneController.h"
#include "SceneGameOver.h"
#include "SceneResult.h"
#include "SceneTest.h"
#include "SkyBox.h"
#include "Timer.h"
#include "Vector3.h"
#include <DxLib.h>
#include "TimeUI.h"
#include "ScoreManager.h"
#include "KilledEnemyNum.h"
#include "Image.h"
#include "Music.h"

// �e�X�g
#include "Model.h"
#include "SphereActor.h"

#define DEBUG_TIME_LIMIT false

namespace
{
	const Vector3 kInitCameraPos = {0, 200, -500};

	constexpr int kDrawGlidNum = 10;

#if DEBUG_TIME_LIMIT
	constexpr int kTimeLimit = 1 * 60;
#else
	constexpr int kTimeLimit = 60 * 60;
#endif

	// �n��
	const std::string kFieldModelPath = "Data/Model/Field.mv1";
	const Vector3 kFieldPos = {0, -3000, 0};
	const Vector3 kFieldScale = {30, 30, 30};

	// UI
	const Vector2 kInitTimeUIPos = {Game::kScreenHalfWidth, 0};
	const Vector2 kKilledEnemyNumUIPos = {50, 30};
	const std::string kButtonGuide = "ButtonGuide.png";
	constexpr float kButtonGuideSize = 0.5f;
	const Vector2 kButtonGuidePos = { 350, 680 };

	// ��
	const std::string kGameBGM = "Data/Music/Game.wav";
}

SceneTest::SceneTest() :
	m_dirLightHandle(-1),
	m_dirLightHandle2(-1),
	m_update(&SceneTest::FadeInUpdate)
{
}

SceneTest::~SceneTest()
{
	DeleteLightHandle(m_dirLightHandle);
	DeleteLightHandle(m_dirLightHandle2);
}

void SceneTest::Init()
{
	m_camera = std::make_shared<Camera>();
	m_actorController = std::make_shared<ActorController>();

	m_camera->Init();

	m_player = std::make_shared<Player>();
	auto sky = std::make_shared<SkyBox>();
	m_enemyGenerator = std::make_shared<EnemyGenerator>();
	m_player->Init(m_camera);
	sky->Init();
	m_actorController->AddActor(m_player);
	m_actorController->AddActor(sky);

	// �������Ԃ̃^�C�}�[
	m_timer = std::make_shared<Timer>();
	m_timer->Init();
	m_timer->SetCount(kTimeLimit);
	m_timer->SetStateCountDown();
	m_timer->StartCount();

	// Timer���K�v�Ȃ̂ł�����
	m_enemyGenerator->Init(m_player, m_timer);
	m_actorController->AddActor(m_enemyGenerator);

	// ���ł�UI
	m_timeUI = std::make_shared<TimeUI>();
	m_timeUI->Init(kInitTimeUIPos, m_timer);
	m_killedEnemyUI = std::make_shared<KilledEnemyNum>();
	m_killedEnemyUI->Init(kKilledEnemyNumUIPos);
	m_buttonGuide = std::make_shared<Image>();
	m_buttonGuide->Init(kButtonGuide);
	m_buttonGuide->SetExRate(kButtonGuideSize);

	// ���C�g
	// �������֏Ƃ炷
	// ����Əオ�Ƃ炳���B�Ȃ�ŁH
	m_dirLightHandle = CreateDirLightHandle(Vector3::Up());
	m_dirLightHandle2 = CreateDirLightHandle(Vector3::Down());

	// ���f���r���[�A�̐ݒ肪���f����Ȃ��̂ł����Ŕ��f
	m_field = std::make_shared<Model>();
	m_field->Init(kFieldModelPath);
	m_field->SetPos(kFieldPos);
	m_field->SetScale(kFieldScale);
}

void SceneTest::Update()
{
	// �J�����̕��������
	SetLightDirection(m_camera->GetCameraDir());
	(this->*m_update)();
}

void SceneTest::Draw() const
{
	//DrawGrid();

	m_field->Draw();
	m_actorController->Draw();
	// �|�C���^�Ȃ�const����Ȃ��֐����Ăׂ�� ���߂Ēm����
	m_timeUI->Draw();
	m_killedEnemyUI->Draw();
	m_buttonGuide->Draw(kButtonGuidePos);
	ScoreManager::GetInstance().Draw();

	// �S�Ă̏�Ƀt�F�[�h
	m_fadePanel->Draw({ Game::kScreenHalfWidth, Game::kScreenHalfHeight });
}

void SceneTest::Entry()
{
	// �Ȃ��Đ�
	Music::GetInstance().Play(kGameBGM);
	m_fadePanel->SetImageBlendPal(255);
}

void SceneTest::Reave()
{
	// �G�t�F�N�g���폜
	EffectManager::GetInstance().ClearEffects();
}

void SceneTest::GameOver()
{
	SceneController::GetInstance().ChangeScene(std::make_shared<SceneGameOver>());
}

void SceneTest::FadeInUpdate()
{
	m_camera->Update();
	Scene::FadeInUpdate();
	if (m_fadePanel->GetBlendParam() <= 0)
	{
		m_update = &SceneTest::NormalUpdate;
		return;
	}
}

void SceneTest::NormalUpdate()
{
	ScoreManager::GetInstance().Update();
	Input& input = Input::GetInstance();
	m_camera->Update();

	m_actorController->Update();
	m_killedEnemyUI->Update();
	if (!m_actorController->CheckIsAlive(m_player))
	{
		GameOver();
		return;
	}
	// �������Ԃ��o�߂�����
	if (m_timer->Update())
	{
		m_timer->SetCount(0);
		// �G������
		// ���U���g
		m_update = &SceneTest::FadeOutUpate;
		return;
	}
}

void SceneTest::FadeOutUpate()
{
	m_camera->Update();
	Scene::FadeOutUpdate();
	if (m_fadePanel->GetBlendParam() >= 255)
	{
		auto resultScene = std::make_shared<SceneResult>();
		resultScene->Init();
		SceneController::GetInstance().ChangeScene(resultScene);
		return;
	}
}


void SceneTest::DrawGrid() const
{
	Vector3 lineStart{ 0, 0, -50 * kDrawGlidNum };
	Vector3 lineEnd{ 0, 0, 50 * kDrawGlidNum };

	const int kGridLoopNum = static_cast<int>(kDrawGlidNum * 0.5f);

	// ���s(Z)����
	for (int x = -kGridLoopNum; x < kGridLoopNum + 1; ++x)
	{
		lineStart.x = static_cast<float>(x * 100);
		lineEnd.x = static_cast<float>(x * 100);

		DrawLine3D(lineStart, lineEnd, 0x00ff00);
	}

	lineStart = { -50*kDrawGlidNum, 0, 0 };
	lineEnd = { 50*kDrawGlidNum, 0, 0 };

	// ��(X)����
	for (int z = -kGridLoopNum; z < kGridLoopNum + 1; ++z)
	{
		lineStart.z = static_cast<float>(z * 100);
		lineEnd.z = static_cast<float>(z * 100);

		DrawLine3D(lineStart, lineEnd, 0xff0000);
	}

	// Y���̕����Ƀ��C����L�΂�
	DrawLine3D({ 0,0,0 }, { 0,100,0 }, 0x0000ff);
}