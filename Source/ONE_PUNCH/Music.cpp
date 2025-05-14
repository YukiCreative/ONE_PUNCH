#include "Music.h"
#include <algorithm>
#include <DxLib.h>

Music::Music() :
	m_playType(DX_PLAYTYPE_LOOP),
	m_nowPlayingFilePath()
{
}

Music& Music::GetInstance()
{
	static Music instance;
	return instance;
}

void Music::Init()
{
}

void Music::Play(const std::string& path)
{
	// �������Ƃ��Ă��鉹�y�������z�Ȃ�A���̂܂܂ɂ���������
	if (m_nowPlayingFilePath == path) return;

	m_nowPlayingFilePath = path;
	PlayMusic(path.c_str(), m_playType);
}

void Music::Loop(const bool& loopOrNot)
{
	m_playType = loopOrNot ? DX_PLAYTYPE_LOOP : DX_PLAYTYPE_BACK;
}

bool Music::IsPlaying() const
{
	return CheckMusic();
}

void Music::Stop() const
{
	StopMusic();
}
