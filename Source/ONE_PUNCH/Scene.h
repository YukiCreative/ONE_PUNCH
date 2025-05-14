#pragma once
#include <memory>

class Image;

class Scene abstract
{
public:
	// こっそりコンストラクタで初期化しちゃお
	Scene();

	virtual void Init() abstract;
	virtual void Update() abstract;
	virtual void Draw() const abstract;

	virtual void Entry() abstract;
	virtual void Reave() abstract;
protected:
	std::shared_ptr<Image> m_fadePanel;
	// これを実行したら勝手にパネルが完全に透明になるまでフェードインしてくれる
	void FadeInUpdate();
	// パネルが完全に不透明になるまでフェードアウト
	void FadeOutUpdate();
};

