#pragma once
#include <memory>
#include <list>

class Scene;

class SceneController
{
private:
	SceneController();
	SceneController(const SceneController&) = delete;
	void operator=(const SceneController&) = delete;

	using SceneList_t = std::list<std::shared_ptr<Scene>>;
	SceneList_t m_scenes;

	// �V�[�����ς��������Draw�����s���Ȃ�
	bool m_isChangedSceneInThisFrame;
public:
	static SceneController& GetInstance();

	void Init();
	void Update();
	void Draw() const;

	void ChangeScene(std::shared_ptr<Scene> nextScene);
	void StackScene(std::shared_ptr<Scene> stackScene);
	void RemoveScene();
	// ������V�[����S�ď����āA�V�����V�[��������ɂ���
	void SingleScene(std::shared_ptr<Scene> nextScene);
};