#pragma once
#include "Vector2.h"
#include <string>
#include <memory>

// ����
// �J�[�\�����t�H�[�J�X�ł���
// �t�H�[�J�X���ɉ�����
// �����Ɛݒ肳�ꂽ�C�x���g�����s�����
// �C�x���g��Button���p�������N���X�Ŏ���
// �{�^���̃t�H�[�J�X��J�[�\���̏����͕ʂ̃N���X�ł��
class Button abstract : public std::enable_shared_from_this<Button>
{
public:
	Button();

	void Init(const Vector2& initPos);
	// Update�͏����������Ă���̂ŏ㏑�����Ȃ�
	void Update();
	virtual void Draw() const abstract;

	// �t�H�[�J�X���ꂽ��ButtonSystem���ĂԊ֐�
	virtual void OnFocused() abstract;
	virtual void OnDisfocused() abstract;
	// �����ɉ������̌ʏ�����
	virtual void OnPressed() abstract;

	void SetRightButton(std::weak_ptr<Button> right) { m_rightButton = right; }
	void SetLeftButton(std::weak_ptr<Button> left) { m_leftButton = left; }
	void SetUpButton(std::weak_ptr<Button> up) { m_upButton = up; }
	void SetDownButton(std::weak_ptr<Button> down) { m_downButton = down; }

	std::weak_ptr<Button> RightButton() const;
	std::weak_ptr<Button> LeftButton()  const;
	std::weak_ptr<Button> UpButton()    const;
	std::weak_ptr<Button> DownButton()  const;

protected:
	Vector2 m_pos;

	// �t�H�[�J�X�̗L���̐؂�ւ�
	void InvertState();
	virtual void FocusedUpdate() abstract;
	virtual void NormalUpdate() abstract;

private:
	// �㉺���E�̓��͂����ꂽ���̃{�^���̑J�ڐ�
	std::weak_ptr<Button> m_leftButton;
	std::weak_ptr<Button> m_rightButton;
	std::weak_ptr<Button> m_upButton;
	std::weak_ptr<Button> m_downButton;
	// �摜�Ƃ��e�L�X�g�Ƃ��͔h����Ő錾�������������₷���Ǝv���Ēu���Ă��Ȃ�

	using UpdateState_t = void (Button::*)();
	UpdateState_t m_updateState;
};