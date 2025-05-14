#include "Input.h"
#include <cassert>
#include <DxLib.h>

namespace
{
	constexpr int kPadA      = PAD_INPUT_1;
	constexpr int kPadB      = PAD_INPUT_2;
	constexpr int kPadX      = PAD_INPUT_3;
	constexpr int kPadY      = PAD_INPUT_4;
	constexpr int kPadL      = PAD_INPUT_5;
	constexpr int kPadR      = PAD_INPUT_6;
	constexpr int kPadSelect = PAD_INPUT_7;
	constexpr int kPadStart  = PAD_INPUT_8;
	constexpr int kPadLStick = PAD_INPUT_9;
	constexpr int kPadRStick = PAD_INPUT_10;
	constexpr int kPadUp     = PAD_INPUT_UP;
	constexpr int kPadDown   = PAD_INPUT_DOWN;
	constexpr int kPadRight  = PAD_INPUT_RIGHT;
	constexpr int kPadLeft   = PAD_INPUT_LEFT;
}

Input::Input() :
	m_padInput(0),
	m_beforePadInput(0),
	m_keyInput(),
	m_beforeKeyInput(),
	m_leftInputAxis(),
	m_beforeLeftInputAxis(),
	m_rightInputAxis(),
	m_beforeRightInputAxis(),
	m_inputEvent()
{
}

void Input::GetAxis()
{
	// �X�e�B�b�N�̓��͂��擾
	// ���X�e�B�b�N
	int leftAxisX, leftAxisY;
	GetJoypadAnalogInput(&leftAxisX, &leftAxisY, DX_INPUT_PAD1);
	// �L�[�{�[�h���擾
	if (IsPressed("KeyMoveUp"))
	{
		leftAxisY = -1000;
	}
	if (IsPressed("KeyMoveDown"))
	{
		leftAxisY = 1000;
	}
	if (IsPressed("KeyMoveRight")) 
	{
		leftAxisX = 1000;
	}
	if (IsPressed("KeyMoveLeft")) 
	{
		leftAxisX = -1000;
	}

	// �E�X�e�B�b�N
	int rightAxisX, rightAxisY;
	//Get
	// �g�p�񐄏��Ə����Ă��邪���v���낤��
	GetJoypadAnalogInputRight(&rightAxisX, &rightAxisY, DX_INPUT_PAD1);

	// �A�i���O���͂̑傫����1000�܂łɐ�������
	Vector2 leftInputAxis(static_cast<float>(leftAxisX), static_cast<float>(leftAxisY));
	if (leftInputAxis.SqrMagnitude() > 1000 * 1000)
	{
		leftInputAxis.Normalize();
		leftInputAxis *= 1000;
	}
	Vector2 rightInputAxis(static_cast<float>(rightAxisX), static_cast<float>(rightAxisY));
	if (rightInputAxis.SqrMagnitude() > 1000 * 1000)
	{
		rightInputAxis.Normalize();
		rightInputAxis *= 1000;
	}

	// �����őO�t���[���̓��͂��o���Ă���
	m_beforeLeftInputAxis = m_leftInputAxis;
	// �����čX�V
	m_leftInputAxis = leftInputAxis;
	m_beforeRightInputAxis = m_rightInputAxis;
	m_rightInputAxis = rightInputAxis;
}

Input& Input::GetInstance()
{
	static Input instance;
	return instance;
}

void Input::SetKeyDefault()
{
	m_inputEvent["Submit"] =
	{
		{PeripheralType::kKeyboard, KEY_INPUT_RETURN},
		{PeripheralType::kPad, kPadA},
	};
	m_inputEvent["Up"] =
	{
		{PeripheralType::kKeyboard, KEY_INPUT_UP},
		{PeripheralType::kPad, PAD_INPUT_UP},
	};
	m_inputEvent["Down"] =
	{
		{PeripheralType::kKeyboard, KEY_INPUT_DOWN},
		{PeripheralType::kPad, PAD_INPUT_DOWN},
	};
	m_inputEvent["Right"] =
	{
		{PeripheralType::kKeyboard, KEY_INPUT_RIGHT},
		{PeripheralType::kPad, PAD_INPUT_RIGHT},
	};
	m_inputEvent["Left"] =
	{
		{PeripheralType::kKeyboard, KEY_INPUT_LEFT},
		{PeripheralType::kPad, PAD_INPUT_LEFT},
	};
	m_inputEvent["Rise"] =
	{
		{PeripheralType::kKeyboard, KEY_INPUT_RSHIFT}
	};
	m_inputEvent["Descend"] =
	{
		{PeripheralType::kKeyboard, KEY_INPUT_RCONTROL}
	};
	m_inputEvent["CameraRotateRight"] =
	{
		{PeripheralType::kKeyboard, KEY_INPUT_D}
	};
	m_inputEvent["CameraRotateLeft"] =
	{
		{PeripheralType::kKeyboard, KEY_INPUT_A}
	};
	m_inputEvent["CameraRotateVPlus"] =
	{
		{PeripheralType::kKeyboard, KEY_INPUT_W}
	};
	m_inputEvent["CameraRotateVMinus"] =
	{
		{PeripheralType::kKeyboard, KEY_INPUT_S}
	};
	m_inputEvent["KeyMoveUp"] =
	{
		{PeripheralType::kKeyboard, KEY_INPUT_UP},
	};
	m_inputEvent["KeyMoveDown"] =
	{
		{PeripheralType::kKeyboard, KEY_INPUT_DOWN},
	};
	m_inputEvent["KeyMoveRight"] =
	{
		{PeripheralType::kKeyboard, KEY_INPUT_RIGHT},
	};
	m_inputEvent["KeyMoveLeft"] =
	{
		{PeripheralType::kKeyboard, KEY_INPUT_LEFT},
	};
	m_inputEvent["Jump"] =
	{
		{PeripheralType::kKeyboard, KEY_INPUT_SPACE},
		{PeripheralType::kPad, kPadX},
	};
	m_inputEvent["Dash"] =
	{
		{PeripheralType::kKeyboard, KEY_INPUT_LSHIFT},
		{PeripheralType::kPad, kPadB},
	};
	m_inputEvent["Reset"] =
	{
		{PeripheralType::kKeyboard, KEY_INPUT_P},
	};
	m_inputEvent["Attack"] =
	{
		{PeripheralType::kKeyboard, KEY_INPUT_Z},
		{PeripheralType::kPad, kPadA},
	};
}

void Input::Init()
{
	SetKeyDefault();

	m_padDInput = std::make_shared<DxLib::tagDINPUT_JOYSTATE>();
	m_beforePadDInput = std::make_shared<DxLib::tagDINPUT_JOYSTATE>();
}

void Input::Update()
{
	// PAD�̑O�̃t���[���̂��L��
	m_beforePadInput = m_padInput;
	m_padInput = GetJoypadInputState(DX_INPUT_PAD1);

	// �L�[
	m_beforeKeyInput = m_keyInput;
	GetHitKeyStateAll(m_keyInput.data());

	GetAxis();
}

void Input::Draw_Debug() const
{
}

bool Input::IsPressed(const std::string& key) const
{
	bool isPressed = false;
	// �^����ꂽ�L�[�ɑΉ�����S�Ă̎��Ӌ@��̓��̓R�[�h�𒲂ׂ�
	for (const auto& inputEvent : m_inputEvent.at(key))
	{
		switch (inputEvent.type)
		{
			// or�Ŏ���Ă���͎̂d�l�ł�
		case PeripheralType::kKeyboard:
			// �L�[�{�[�h�̂����œ��͂��擾
			isPressed = isPressed || m_keyInput[inputEvent.inputCode];
			break;
		case PeripheralType::kPad:
			// �p�b�h
			isPressed = isPressed || m_padInput & inputEvent.inputCode;
			break;
		default:
			assert(false && "�񋓑̗̂v�f�ɑ΂��ď�������������Ă��Ȃ�");
			break;
		}
	}
	return isPressed;
}

bool Input::IsTrigger(const std::string& key) const
{
	bool isTrigger = false;
	// �^����ꂽ�L�[�ɑΉ�����S�Ă̎��Ӌ@��̓��̓R�[�h�𒲂ׂ�
	for (const auto& inputEvent : m_inputEvent.at(key))
	{
		switch (inputEvent.type)
		{
		case PeripheralType::kKeyboard:
			// �L�[�{�[�h�̂����œ��͂��擾
			isTrigger = isTrigger || m_keyInput[inputEvent.inputCode] && !m_beforeKeyInput[inputEvent.inputCode];
			break;
		case PeripheralType::kPad:
			// �p�b�h
			isTrigger = isTrigger || (m_padInput & inputEvent.inputCode) && !(m_beforePadInput & inputEvent.inputCode);
			break;
		default:
			assert(false && "�񋓑̗̂v�f�ɑ΂��ď�������������Ă��Ȃ�");
			break;
		}
	}
	return isTrigger;
}

const Vector2& Input::GetLeftInputAxis() const
{
	return m_leftInputAxis;
}

const Vector2& Input::GetBeforeLeftInputAxis() const
{
	return m_beforeLeftInputAxis;
}

const Vector2& Input::GetRightInputAxis() const
{
	return m_rightInputAxis;
}

const Vector2& Input::GetBeforeRightInputAxis() const
{
	return m_beforeRightInputAxis;
}
