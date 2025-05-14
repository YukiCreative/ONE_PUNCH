#pragma once
#include <memory>

class Image;

class Scene abstract
{
public:
	// ��������R���X�g���N�^�ŏ����������Ⴈ
	Scene();

	virtual void Init() abstract;
	virtual void Update() abstract;
	virtual void Draw() const abstract;

	virtual void Entry() abstract;
	virtual void Reave() abstract;
protected:
	std::shared_ptr<Image> m_fadePanel;
	// ��������s�����珟��Ƀp�l�������S�ɓ����ɂȂ�܂Ńt�F�[�h�C�����Ă����
	void FadeInUpdate();
	// �p�l�������S�ɕs�����ɂȂ�܂Ńt�F�[�h�A�E�g
	void FadeOutUpdate();
};

