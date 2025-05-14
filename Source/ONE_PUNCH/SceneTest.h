#pragma once
#include "Scene.h"
#include <memory>

class Camera;
class Model;
class ActorController;
class Player;
class Ground;
class Timer;
class EnemyGenerator;
class TimeUI;
class KilledEnemyNum;

class SceneTest final : public Scene
{
public:
	SceneTest();
	~SceneTest();

	void Init() override;
	void Update() override;
	void Draw() const override;

	void Entry() override;
	void Reave() override;

private:

	// �O���錾��unique_ptr���g���̂͂߂�����߂�ǂ��@�炵��
	// ����������Ȃ�
	std::shared_ptr<ActorController> m_actorController;
	// ActorController�ɐ��b��C���邪�A�����ł���j�������Ă����N���X
	// �������́AScene���炢����Ȃ��悤�Ƀn���h���݂����Ȃ̂�n����悤�ɂ���̂��A��
	std::shared_ptr<Player> m_player;
	std::shared_ptr<EnemyGenerator> m_enemyGenerator;

	// �X�e�[�W
	// ���Ԃ��Ȃ��̂Ń��f�����|���u��
	std::shared_ptr<Model> m_field;

	// Actor�ł͂Ȃ����
	std::shared_ptr<Timer> m_timer;
	std::shared_ptr<Camera> m_camera;
	std::shared_ptr<TimeUI> m_timeUI;
	std::shared_ptr<KilledEnemyNum> m_killedEnemyUI;
	std::shared_ptr<Image> m_buttonGuide;

	// ���C�g
	int m_dirLightHandle;
	int m_dirLightHandle2;

	void (SceneTest::* m_update)();
private:
	void DrawGrid() const;
	// �Q�[���I�[�o�[�V�[���ւ̑J��
	void GameOver();

	void FadeInUpdate();
	void NormalUpdate();
	void FadeOutUpate();
};

