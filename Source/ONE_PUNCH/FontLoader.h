#pragma once
#include <string>
#include <unordered_map>

// �t�H���g�t�@�C����windows�Ɉꎞ�I�Ɏ�荞�ނ���
// �V���O���g���������Ă���
class FontLoader
{
private:
	FontLoader();
	FontLoader(const FontLoader&) = delete;
	void operator=(const FontLoader&) = delete;

	using FontMap_t = std::unordered_map<std::string, int>;
	FontMap_t m_fontHandles;
public:
	// ������s���Ă���x�������s����Ȃ�
	static void LoadFontFile();

	~FontLoader();
};

