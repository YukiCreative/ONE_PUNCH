#include "SoundManager.h"
#include "Sound.h"
#include <filesystem>
#include <DxLib.h>
#include <cassert>

namespace
{
	const std::string kFolderPath = "Data/Sound/";

	const std::vector<std::string> kSoundFileExtensions =
	{
		".mp3",
		".wav",
	};
}

SoundManager::SoundManager()
{
}

void SoundManager::SetAllSoundVolume(int volume)
{
	for (auto& sound : m_sounds)
	{
		// �������Ńo���f�[�V�����`�F�b�N���Ă���邩����v
		sound->SetVolume(volume);
	}
}

SoundManager& SoundManager::GetInstance()
{
	static SoundManager instance;
	return instance;
}

void SoundManager::Init()
{
	LoadSourceSound();
}

void SoundManager::Update()
{
	RemoveFinishedSounds();
}

void SoundManager::RemoveFinishedSounds()
{
	SoundList_t playingSounds;
	for (const auto& sound : m_sounds)
	{
		if (sound->IsPlaying())
		{
			playingSounds.push_back(sound);
		}
	}
	// �Đ����̂����c���Č�͎Q�Ƃ��Ȃ������
	m_sounds = playingSounds;

	//printf("���̉����C���X�^���X�F%d��\n", m_sounds.size());
}

std::weak_ptr<Sound> SoundManager::Play(const std::string& fileName, const bool isLoop)
{
	auto sound = std::make_shared<Sound>();
	sound->Init(m_store[fileName], isLoop);
	m_sounds.emplace_back(sound);
	return sound;
}

void SoundManager::LoadSourceSound()
{
	// �w�肵���t�H���_���ɂ��鉹���t�@�C����S�ǂݍ���
	namespace fs = std::filesystem;

	for (const auto& file : fs::directory_iterator(kFolderPath))
	{
		std::string path = file.path().string();

		// �摜�Ƃ��ēǂ߂�g���q���ǂ������m���߂Ă�
		bool isValidExtention = std::find(kSoundFileExtensions.begin(),
			kSoundFileExtensions.end(), file.path().extension().string()) != kSoundFileExtensions.end();
		if (!isValidExtention)
		{
#if _DEBUG
			printf("�g���q���Ⴄ�t�@�C�����X�L�b�v\n");
			printf("�@�p�X�F%s\n", path.c_str());
#endif
			continue;
		}

		// �t�@�C���������o��
		std::string pathCopy = path;
		std::string fileName = pathCopy.erase(0, kFolderPath.size());
		m_store[fileName] = LoadSoundMem(path.c_str());

#if _DEBUG
		assert(m_store[fileName] != -1 && "�����t�@�C���ǂݍ��݂Ɏ��s����");
		printf("�����t�@�C����ǂݍ��݂܂����B\n");
		printf("�@�t�@�C�����F%s\n", fileName.c_str());
		printf("�@�n���h���F%d\n", m_store[fileName]);
#endif
	}
}
