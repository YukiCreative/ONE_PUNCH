#pragma once
#include "Vector2.h"
#include <array>
#include <vector>
#include <string>
#include <unordered_map>
#include <memory>

namespace DxLib
{
	struct tagDINPUT_JOYSTATE;
}

class Input
{
public:
	static Input& GetInstance();

	// �L�[�R���t�B�O�������l��
	void SetKeyDefault();

	void Init();
	void Update();
	void Draw_Debug() const;

	// ������Ă���
	bool IsPressed(const std::string& key) const;
	// ���܂��ɉ����ꂽ
	bool IsTrigger(const std::string& key) const;

	// �A�i���O�R���g���[�����\���L�[�̓���
	const Vector2& GetLeftInputAxis() const;
	const Vector2& GetBeforeLeftInputAxis() const;
	// �E��
	const Vector2& GetRightInputAxis() const;
	const Vector2& GetBeforeRightInputAxis() const;
private:
	enum class PeripheralType
	{
		kKeyboard,
		kPad,
	};

	struct Peripheral
	{
		PeripheralType type;
		int inputCode;
	};

private:
	// �����̂��Ƃ��V���O���g��
	Input();
	Input(const Input&) = delete;
	void operator=(const Input&) = delete;

	std::shared_ptr<DxLib::tagDINPUT_JOYSTATE> m_padDInput;
	std::shared_ptr<DxLib::tagDINPUT_JOYSTATE> m_beforePadDInput;

	int m_padInput;
	int m_beforePadInput;

	std::array<char, 256> m_keyInput;
	std::array<char, 256> m_beforeKeyInput;

	Vector2 m_leftInputAxis;
	Vector2 m_beforeLeftInputAxis;
	Vector2 m_rightInputAxis;
	Vector2 m_beforeRightInputAxis;

	std::unordered_map<std::string, std::vector<Peripheral>> m_inputEvent;
private:

	// �X�e�B�b�N���͂��擾���鏈���𕪂���
	void GetAxis();
};

