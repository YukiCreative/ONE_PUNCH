#include "FontLoader.h"
#include <DxLib.h>
#include <filesystem>

namespace
{
	const std::string kFontFolder = "Data/Font/";
}

FontLoader::FontLoader()
{
	// �����ŁA����̃t�H���_�̃t�@�C����S���[�h
	// �t�H���_�Ȃ̂��f�B���N�g���Ȃ̂��ǂ������ɂ��Ăق���
	std::filesystem::directory_iterator directory = std::filesystem::directory_iterator(kFontFolder);
	for (const auto& file : directory)
	{
		const std::string path = file.path().string();
		AddFontResourceExA(path.c_str(), FR_PRIVATE, NULL);
#if _DEBUG
		printf("�t�H���g��ǂݍ��݂܂���\n");
		printf("�p�X�F%s\n", path.c_str());
#endif
	}
}

void FontLoader::LoadFontFile()
{
	static FontLoader instance;
}

FontLoader::~FontLoader()
{
	RemoveFontResourceExA("", FR_PRIVATE, NULL);
}