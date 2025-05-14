#include "ButtonSystem.h"
#include "Input.h"
#include "Button.h"
#include "ButtonNoFocus.h"
#include <cassert>

namespace
{
	constexpr int kButtonPosMoveThreshold = 300;
	constexpr int kCursorMoveInterval = 20;
	constexpr int kAxisDirThreshold = 45;

	const std::string kSerectSound = "ButtonSelectSound.mp3";
}

void ButtonSystem::MoveFocus(Vector2 inputAxis)
{
	if (!CanMoveFocus(inputAxis))
	{
		//printf("�����Ȃ�����\n");
		return;
	}

	Vector2 inputDir(0,0);
	std::weak_ptr<Button> beforeButton = m_nowFocusedButton;
	// axis������ăt�H�[�J�X�̈ʒu�����炷
	if (inputAxis.x > kButtonPosMoveThreshold)
	{
		m_nowFocusedButton = m_nowFocusedButton.lock()->RightButton();
		++inputDir.x;
	}
	else if (inputAxis.x < -kButtonPosMoveThreshold)
	{
		m_nowFocusedButton = m_nowFocusedButton.lock()->LeftButton();
		--inputDir.x;
	}
	if (inputAxis.y > kButtonPosMoveThreshold)
	{
		m_nowFocusedButton = m_nowFocusedButton.lock()->DownButton();
		++inputDir.y;
	}
	else if (inputAxis.y < -kButtonPosMoveThreshold)
	{
		m_nowFocusedButton = m_nowFocusedButton.lock()->UpButton();
		--inputDir.y;
	}

	// �����ŉ����ς���Ă��Ȃ������珈���I���
	if (m_nowFocusedButton.lock() == beforeButton.lock()) return;

	// ���̓��͂��L��
	m_inputDirLastMove = inputDir;

	// �V�����I�����ꂽ�{�^���Ɂu���O�I�΂�Ă邼�v�ƘA��
	m_nowFocusedButton.lock()->OnFocused();
	// ���̃{�^���͒ʏ��Ԃɖ߂�悤�ɘA��
	beforeButton.lock()->OnDisfocused();
	// �^�C�}�[��ݒ�
	m_cursorMoveTimer = kCursorMoveInterval;
	// ���łɉ�
	//SoundManager::GetInstance().Play(kSerectSound);
}

bool ButtonSystem::CanMoveFocus(const Vector2& inputAxis) const
{
	// ����
	// �@�O�̗L���ȓ��͕����Ɠ��������i���͂Ȃ��ȊO�j�ɓ��͂��Ă�����A
	//�@ ���̕����ɓ��͂����߂Ĉ�莞�Ԃ��o�߂��Ă��Ȃ��Ƃ����Ȃ�
	// �O�̃t���[���ƕ������Ⴄ�Ȃ炻�̂܂ܓ��͂�ʂ�
	// ���͕�����360�x���㉺���E�ŕ��������l��
	
	Vector2 beforeInputAxis = Input::GetInstance().GetBeforeLeftInputAxis();

	// �O����͂��Ă��Ȃ�����True�ł�����
	if (beforeInputAxis.x == 0 && beforeInputAxis.y == 0) return true;

	if (abs(inputAxis.RerativeAngle(m_inputDirLastMove)) > 30) return true;

	if (m_cursorMoveTimer <= 0) return true;

	return false;
}

ButtonSystem::ButtonSystem() :
	m_cursorMoveTimer(0),
	m_inputDirLastMove(0,0)
{
	m_noFocus = std::make_shared<ButtonNoFocus>();
	m_nowFocusedButton = m_noFocus;
}

ButtonSystem::~ButtonSystem()
{
}

void ButtonSystem::Update()
{
	--m_cursorMoveTimer;
	Vector2 inputAxis = Input::GetInstance().GetLeftInputAxis();

	// �I���{�^���̈ړ�����
	MoveFocus(inputAxis);

	// �o�^����Ă���{�^���̍X�V����������
	for (auto& button : m_buttons)
	{
		button->Update();
	}

	// �{�^���������Ƃ��̏���
	if (Input::GetInstance().IsTrigger("Submit"))
	{
		m_nowFocusedButton.lock()->OnPressed();
		// �����ɃV�[���J�ڂ��邩������Ȃ��̂�return���Ƃ�
		return;
	}

	//printf("m_cursorMoveTimer=%d\n", m_cursorMoveTimer);
}

void ButtonSystem::Draw() const
{
	// �{�^���̕`��֐������s
	for (const auto& button : m_buttons)
	{
		button->Draw();
		//printf("�Q�Ɛ�:%d", button.use_count());
	}
}

void ButtonSystem::AddButton(std::shared_ptr<Button> buttonInstance)
{
	m_buttons.push_back(buttonInstance);
}

void ButtonSystem::SetButtonFocus(std::shared_ptr<Button> setButton)
{
	assert(std::find(m_buttons.begin(), m_buttons.end(), setButton) != m_buttons.end() && "�o�^����Ă��Ȃ��{�^���ł�");
	m_nowFocusedButton.lock()->OnDisfocused();
	m_nowFocusedButton = setButton;
	m_nowFocusedButton.lock()->OnFocused();
}

void ButtonSystem::ExitFocus()
{
	m_nowFocusedButton = std::static_pointer_cast<Button>(m_noFocus);
}
