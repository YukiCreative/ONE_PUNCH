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

	// 前方宣言でunique_ptrを使うのはめっちゃめんどい　らしい
	// やり方分からない
	std::shared_ptr<ActorController> m_actorController;
	// ActorControllerに世話を任せるが、自分でも手綱を持っておくクラス
	// もしくは、Sceneからいじれないようにハンドルみたいなのを渡せるようにするのもアリ
	std::shared_ptr<Player> m_player;
	std::shared_ptr<EnemyGenerator> m_enemyGenerator;

	// ステージ
	// 時間がないのでモデルをポン置き
	std::shared_ptr<Model> m_field;

	// Actorではないやつ
	std::shared_ptr<Timer> m_timer;
	std::shared_ptr<Camera> m_camera;
	std::shared_ptr<TimeUI> m_timeUI;
	std::shared_ptr<KilledEnemyNum> m_killedEnemyUI;
	std::shared_ptr<Image> m_buttonGuide;

	// ライト
	int m_dirLightHandle;
	int m_dirLightHandle2;

	void (SceneTest::* m_update)();
private:
	void DrawGrid() const;
	// ゲームオーバーシーンへの遷移
	void GameOver();

	void FadeInUpdate();
	void NormalUpdate();
	void FadeOutUpate();
};

