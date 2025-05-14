#pragma once
#include <list>
#include <string>
#include <memory>
#include <unordered_map>

class Sound;

class SoundManager
{
private:
	SoundManager();
	SoundManager(const SoundManager&) = delete;
	void operator=(const SoundManager&) = delete;
public:
	static SoundManager& GetInstance();

	void Init();

	// ���t���[���Ăяo��
	void Update();

	// Unity��Instantiate�݂����Ȋ����ŁA����������ɂ��̎Q�Ƃ�Ԃ�
	std::weak_ptr<Sound> Play(const std::string& fileName, const bool isLoop = false);
	// �ꊇ�ŕς�����
	// 0(����)~255(�f�t�H���g)
	void SetAllSoundVolume(int volume);

private:
	using SoundList_t = std::list<std::shared_ptr<Sound>>;
	SoundList_t m_sounds;

	std::unordered_map<std::string, int> m_store;

private:
	void RemoveFinishedSounds();
	void LoadSourceSound();
};

