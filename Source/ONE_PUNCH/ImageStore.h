#pragma once
#include <string>
#include <unordered_map>

// ���͂₨�����
class ImageStore
{
private:
	ImageStore();
	ImageStore(const ImageStore&) = delete;
	void operator=(const ImageStore&) = delete;

public:
	~ImageStore();
	static ImageStore& GetInstance();

	int GetGraph(const std::string fileName);

private:
	std::unordered_map<std::string, int> m_handles;

private:
	void LoadImages();
};

