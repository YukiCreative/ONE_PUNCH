#include "EffectManager.h"
#include "EffekseerEffect.h"
#include <DxLib.h>
#include <EffekseerForDXLib.h>
#include <filesystem>
#include <string>

namespace
{
	constexpr int     kEffectPerticleLimit = 20000;
	const std::string kSourceFolder = "Data/Effect/";
	const std::string kEffectFileExtension = ".efkefc";
	constexpr float   kDefaultEffectSize = 100.0f;
}

EffectManager::EffectManager()
{
}


EffectManager::~EffectManager()
{
}

EffectManager& EffectManager::GetInstance()
{
	static EffectManager instance;
	return instance;
}

void EffectManager::Init()
{
	// ��A�̂��܂��Ȃ�
	SetUseDirect3DVersion(DX_DIRECT3D_11);
	// �����������Ɏ��s������A�v���I��
	if (Effekseer_Init(kEffectPerticleLimit) == -1) { DxLib_End(); }

	// �t���X�N���[���Ή�
	SetChangeScreenModeGraphicsSystemResetFlag(false);
	Effekseer_SetGraphicsDeviceLostCallbackFunctions();

	// Z�o�b�t�@�̐ݒ肪�K�v�Ȃ̂����A����͂ق��̏ꏊ�Ŏ��s���Ă���̂ł悵

	// �G�t�F�N�g�̓ǂݍ���
	LoadEffectSources();
}

void EffectManager::Update()
{
	// ����ňꊇ�X�V
	UpdateEffekseer3D();

	// �Đ����I�������G�t�F�N�g���폜
	EffectList_t playingEffects;
	for (auto& effect : m_effects)
	{
		// �Đ����ĂȂ��z�͒u���Ă���
		if (!effect->IsPlaying()) continue;

		// �E��
		playingEffects.emplace_back(effect);
	}
	// �Đ����̂�����ɂ���
	m_effects = playingEffects;
}

void EffectManager::Draw() const
{
	// Effekseer�̃G�t�F�N�g�͂���ł��ׂĈꊇ�`�悳���
	// �����h���[�R�[�������炳�Ȃ��ƕ��ׂ���΂����炾�Ǝv��
	DrawEffekseer3D();
}

std::weak_ptr<EffekseerEffect> EffectManager::GenerateEffect(const std::string fileName, const Vector3& initPos)
{
	// �\�[�X����G�t�F�N�g����
	const int sourceHandle = m_effectSources[fileName];
	const int effectHandle = PlayEffekseer3DEffect(sourceHandle);

	// ���O�N���X�̃C���X�^���X��
	auto effect = std::make_shared<EffekseerEffect>();
	effect->Init(effectHandle, initPos);

	// �ۑ�
	m_effects.emplace_back(effect);

	// ���L
	return effect;
}

void EffectManager::ClearEffects()
{
	// �폜�֐�

	m_effects.clear();
}

void EffectManager::LoadEffectSources()
{
	// �G�t�F�N�g�t�@�C�����A����̃t�H���_����ꊇ�œǂݍ���

	namespace fs = std::filesystem;

	// �����ł��̃Q�[���Ŏg���摜���ꊇ�œǂݍ���ł��܂���
	// �w�肵���t�H���_�̒��̂��ׂẲ摜�t�@�C����ǂݍ���
	for (const auto& file : fs::directory_iterator(kSourceFolder))
	{
		std::string path = file.path().string();
		// ���΃p�X�ł͂Ȃ��t�@�C���������Ŏ���悤�ɂ���
		std::string pathCopy = path;
		std::string fileName = pathCopy.erase(0, kSourceFolder.size());

		// �����������t�@�C���g���q�łȂ��Ƃ��A��΂�
		if (file.path().extension().string() != kEffectFileExtension)
		{
#if _DEBUG
			printf("�G�t�F�N�g�t�@�C���łȂ����̂����m\n");
			printf("�@�p�X�F%s\n", path.c_str());
#endif
			continue;
		}

		// �ǂݍ���
		const int handle = LoadEffekseerEffect(path.c_str(), kDefaultEffectSize);
		m_effectSources[fileName] = handle;

#if _DEBUG
		assert(handle != -1 && "�ǂݍ��݂Ɏ��s����");
		printf("�摜�t�@�C����ǂݍ��݂܂���\n");
		printf("�@���΃p�X�@�@�F%s\n", path.c_str());
		printf("�@�A�z�z��L�[�F%s\n", fileName.c_str());
		printf("�@�n���h���@�@�F%d\n", handle);
#endif
	}
}