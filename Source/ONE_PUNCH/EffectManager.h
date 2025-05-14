#pragma once
#include "Vector3.h"
#include <memory>
#include <unordered_map>
#include <list>
#include <string>

class EffekseerEffect;

// Effekseer���g����
// �G�t�F�N�g���o��
// �G�t�F�N�g�𓮂���
// �G�t�F�N�g��`��
// �V���O���g���ɂ���
class EffectManager
{
private:
	EffectManager();
	EffectManager(const EffectManager&) = delete;
	void operator=(const EffectManager&) = delete;

public:
	~EffectManager();

	static EffectManager& GetInstance();

	void Init();

	void Update();
	void Draw() const;

	// �|�C���^��Ԃ��ČĂяo�����N���X������ł���悤�ɂ���
	std::weak_ptr<EffekseerEffect> GenerateEffect(const std::string fileName, const Vector3& initPos);
	// ���o�Ă���G�t�F�N�g�����ׂč폜
	void ClearEffects();
private:
	using EffectSourceMap_t = std::unordered_map<std::string, int>;
	EffectSourceMap_t m_effectSources;

	using EffectList_t = std::list<std::shared_ptr<EffekseerEffect>>;
	EffectList_t m_effects;

private:

	void LoadEffectSources();
};