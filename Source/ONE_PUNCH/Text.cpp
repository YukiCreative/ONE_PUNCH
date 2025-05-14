#include "Text.h"
#include <DxLib.h>
#include "Color.h"

Text::Text() :
	m_color(Color::White),
	m_fontHandle(-1)
{
}

void Text::Init(const std::string text)
{
	m_text = text;
}

void Text::Draw(const Vector2& drawPos)
{

}
