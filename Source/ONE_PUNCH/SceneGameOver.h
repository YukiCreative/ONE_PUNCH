#pragma once
#include "Scene.h"
#include <memory>
#include "Button.h"

class ButtonSystem;

class SceneGameOver : public Scene
{
public:
	SceneGameOver();

	void Init() override;
	void Update() override;
	void Draw() const override;

	void Entry() override;
	void Reave() override;
private:
	std::shared_ptr<ButtonSystem> m_buttons;
private:

	class ButtonReturnToTitle : public Button
	{
	public:
		void Init(const Vector2& initPos);

		void Draw() const override;
		void OnFocused() override;
		void OnDisfocused() override;
		void OnPressed() override;
	private:
		void FocusedUpdate() override;
		void NormalUpdate() override;
	private:
	};
};

