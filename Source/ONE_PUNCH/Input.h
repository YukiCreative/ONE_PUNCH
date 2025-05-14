#pragma once
#include "Vector2.h"
#include <array>
#include <vector>
#include <string>
#include <unordered_map>
#include <memory>

namespace DxLib
{
	struct tagDINPUT_JOYSTATE;
}

class Input
{
public:
	static Input& GetInstance();

	// キーコンフィグを初期値に
	void SetKeyDefault();

	void Init();
	void Update();
	void Draw_Debug() const;

	// 押されている
	bool IsPressed(const std::string& key) const;
	// 今まさに押された
	bool IsTrigger(const std::string& key) const;

	// アナログコントローラ＆十字キーの入力
	const Vector2& GetLeftInputAxis() const;
	const Vector2& GetBeforeLeftInputAxis() const;
	// 右も
	const Vector2& GetRightInputAxis() const;
	const Vector2& GetBeforeRightInputAxis() const;
private:
	enum class PeripheralType
	{
		kKeyboard,
		kPad,
	};

	struct Peripheral
	{
		PeripheralType type;
		int inputCode;
	};

private:
	// いつものごとくシングルトン
	Input();
	Input(const Input&) = delete;
	void operator=(const Input&) = delete;

	std::shared_ptr<DxLib::tagDINPUT_JOYSTATE> m_padDInput;
	std::shared_ptr<DxLib::tagDINPUT_JOYSTATE> m_beforePadDInput;

	int m_padInput;
	int m_beforePadInput;

	std::array<char, 256> m_keyInput;
	std::array<char, 256> m_beforeKeyInput;

	Vector2 m_leftInputAxis;
	Vector2 m_beforeLeftInputAxis;
	Vector2 m_rightInputAxis;
	Vector2 m_beforeRightInputAxis;

	std::unordered_map<std::string, std::vector<Peripheral>> m_inputEvent;
private:

	// スティック入力を取得する処理を分けた
	void GetAxis();
};

