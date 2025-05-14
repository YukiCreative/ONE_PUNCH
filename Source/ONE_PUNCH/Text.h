#pragma once
#include <string>
#include "Vector2.h"

class Text
{
private:
	std::string m_text;
	unsigned int m_color;
	int m_fontHandle;
public:
	Text();

	void Init(const std::string text);

	void Draw(const Vector2& drawPos);

	void SetText(const std::string& text) { m_text = text; }
};

