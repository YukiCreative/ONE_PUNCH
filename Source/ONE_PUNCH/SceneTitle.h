#pragma once
#include "Scene.h"
#include "Button.h"
#include <memory>
#include "Image.h"

class UI;
class ButtonSystem;
class ActorController;
class Image;

class SceneTitle final : public Scene, public std::enable_shared_from_this<SceneTitle>
{
public:
	SceneTitle();

	void Init() override;

	void Update() override;
	void Draw() const override;

	void Entry() override;
	void Reave() override;

	void GameStart();
private:
	std::shared_ptr<ButtonSystem> m_buttons;
	std::shared_ptr<ActorController> m_actors;
	std::shared_ptr<Image> m_titleLogo;

	using UpdateState_t = void (SceneTitle::*)();
	UpdateState_t m_update;

	// これをシーンごとに個別に実装する
	void FadeInUpdate();
	void NormalUpdate();
	void FadeOutUpate();
private:

	// 押したらゲームスタート
	class ButtonStartGame : public Button
	{
	public:
		void Init(const Vector2& initPos, std::weak_ptr<SceneTitle> title);

		void OnFocused() override;
		void OnDisfocused() override;
		void OnPressed() override;
		void Draw() const override;
	private:
		void FocusedUpdate() override;
		void NormalUpdate() override;
	private:
		std::shared_ptr<Image> m_image;
		std::weak_ptr<SceneTitle> m_title;
	};

	// ゲームを終了する
	class ButtonQuitGame : public Button
	{
	public:
		void Init(const Vector2& initPos);

		void OnFocused() override;
		void OnDisfocused() override;
		void OnPressed() override;
		void Draw() const override;
	private:
		void FocusedUpdate() override;
		void NormalUpdate() override;
	private:
		std::shared_ptr<Image> m_image;
	};
};