#pragma once
#include <memory>
#include "Vector2.h"
#include <unordered_map>

class Image;

struct BonusStatus;

// �{�[�i�X�������������Ƃ�`����
class BonusUI
{
public:
	BonusUI();
	void Init(const std::weak_ptr<BonusStatus> status, const int fontHandle, const int imageHandle);

	void Update();
	void Draw() const;

	// ����ɂ����
	void ShiftDown();
	// �ޏ�A�j���[�V����
	void Disappear();

	bool IsDisappeared() const { return m_isDisappeared; }
	bool IsDeleted() const { return m_isDeleted; }

private:
	std::shared_ptr<Image> m_image;
	int m_fontHandle;
	int m_textLength;
	Vector2 m_pos;
	Vector2 m_targetPos;
	int m_stateFrameCount;
	bool m_isDisappeared;
	bool m_isDeleted;
	std::weak_ptr<BonusStatus> m_bonusStatus;
};

