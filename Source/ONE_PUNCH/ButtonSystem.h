#pragma once
#include <list>
#include <memory>
#include "Vector2.h"

class Button;
class ButtonNoFocus;

// ����
// �{�^�����������񎝂�
// ���͂��{�^���ɍS���A����
// �ǂ̃{�^�����I�΂�Ă��邩�����Ă�����
// �{�^���������ꂽ�Ƃ��A���̃{�^���ɋ����Ă�����@
class ButtonSystem
{
private:
	using ButtonList_t = std::list<std::shared_ptr<Button>>;
	ButtonList_t m_buttons;
	std::weak_ptr<Button> m_nowFocusedButton;
	// ��t�H�[�J�X����\���{�^�������
	std::shared_ptr<ButtonNoFocus> m_noFocus;
	// ���������ɓ��͂��������瓮���d�l�̃^�C�}�[
	int m_cursorMoveTimer;
	Vector2 m_inputDirLastMove;

	void MoveFocus(Vector2 inputAxis);

	bool CanMoveFocus(const Vector2& inputAxis) const;
public:
	ButtonSystem();
	~ButtonSystem();

	// ���͂���ǂ̃{�^����I�����Ă��邩��ς����肷��
	void Update();
	
	void Draw() const;

	// �{�^����ǉ�
	// �V�[�������p����z��
	void AddButton(std::shared_ptr<Button> buttonInstance);

	void SetButtonFocus(std::shared_ptr<Button> setButton);
	void ExitFocus();
};