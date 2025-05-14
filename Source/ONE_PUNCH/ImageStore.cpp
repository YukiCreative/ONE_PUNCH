#include "ImageStore.h"
#include <DxLib.h>
#include <filesystem>
#include <vector>
#include <cassert>

namespace
{
	const std::string kFolderPath = "Data/Image/";

	const std::vector<std::string> kImageFileExtensions =
	{
		".png",
		".jpg",
		".bmp",
	};
}

ImageStore::ImageStore()
{
	LoadImages();
}

ImageStore::~ImageStore()
{
	// �摜�̉��
	// �S��������Ⴆ
	InitGraph();
}

ImageStore& ImageStore::GetInstance()
{
	static ImageStore instance;
	return instance;
}

int ImageStore::GetGraph(const std::string fileName)
{
	assert(m_handles.contains(fileName) && "�t�@�C�����̉摜������܂���");
	return m_handles.at(fileName);
}

void ImageStore::LoadImages()
{
	namespace fs = std::filesystem;

	for (const auto& file : fs::directory_iterator(kFolderPath))
	{
		const auto& path = file.path();

		// �摜�Ƃ��ēǂ߂�g���q���ǂ������m���߂Ă�
		bool isValidExtention = std::find(kImageFileExtensions.begin(),
			kImageFileExtensions.end(), path.extension().string()) != kImageFileExtensions.end();
		if (!isValidExtention)
		{
#if _DEBUG
			printf("�g���q���Ⴄ�t�@�C�����X�L�b�v\n");
			printf("�@�p�X�F%s\n", path.string().c_str());
#endif
			continue;
		}

		// �ǂݍ���
		std::string pathCopy = path.string();
		const std::string fileName = pathCopy.erase(0, kFolderPath.size());
		const int handle  = LoadGraph(path.string().c_str());
		m_handles[fileName] = handle;

#if _DEBUG
		assert(handle != -1 && "�ǂݍ��݂Ɏ��s����");
		printf("�摜�t�@�C����ǂݍ��݂܂���\n");
		printf("�@�t�@�C�����F%s\n", fileName.c_str());
		printf("�@�n���h���F%d\n", handle);
#endif
	}
}
