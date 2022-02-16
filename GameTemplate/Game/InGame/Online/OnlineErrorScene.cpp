/// <summary>
/// オンライン通信時の通信エラークラス
/// </summary>
#include "stdafx.h"
#include "OnlineErrorScene.h"
#include "../GameScene.h"
#include "../../Title/TitleScene.h"
#include "../Pause.h"
#include "../../Graphics/SpriteRenderSub.h"

namespace nsKabutoubatu
{
	bool OnlineErrorScene::Start()
	{
		//ゲームシーンのインスタンスを検索
		m_gameScene = FindGO<GameScene>(nsStdafx::GAMESCENE_NAME);
		m_pause = FindGO<Pause>(nsStdafx::PAUSE_NAME);

		//ポーズ機能を利用する
		m_pause->SetPause(true);
		//ポーズ機能は使えないようにする
		m_pause->SetCanPause(false);

		//背景を暗くするための画像
		m_grayBack = NewGO<SpriteRenderSub>(nsStdafx::PRIORITY_11);
		m_grayBack->Init("OnlineErrorGrayBack", 1280, 720);
		//エラーメッセージ画像
		m_errorMessageBox = NewGO<SpriteRenderSub>(nsStdafx::PRIORITY_12);
		m_errorMessageBox->Init("ErrorMessageBox", 1280, 720);

		//パッドナンバーを取得
		m_padNo = m_gameScene->GetPlayerNo();

		return true;
	}

	OnlineErrorScene::~OnlineErrorScene()
	{
		//背景を暗くする画像を削除
		DeleteGO(m_grayBack);
		//エラーメッセージ画像を削除
		DeleteGO(m_errorMessageBox);
	}

	void OnlineErrorScene::PauseUpdate()
	{
		////TODO:↓機能していないので機能させる
		//if (m_errorMessageBoxXscale < 1.0f)
		//{
		//	m_errorMessageBoxXscale += 0.01f;
		//	m_errorMessageBox->SetScale({ m_errorMessageBoxXscale,1.0f,1.0f });
		//}

		//Aボタンでタイトル画面に戻る
		BackTitle();
	}

	//Aボタンでタイトル画面に戻るメソッド
	void OnlineErrorScene::BackTitle()
	{
		//Aボタンを押したら、
		//if (m_playerGamePad->IsTrigger(enButtonA))
		//{
			//ゲームシーンを削除し、タイトルシーンを生成
			m_gameScene->DeleteGameSceneClass();
			NewGO<TitleScene>(nsStdafx::PRIORITY_0, nsStdafx::TITLESCENE_NAME);
			//ポーズ機能を利用しないようにする
			m_pause->SetPause(false);
			//ポーズ機能を使えるようにする
			m_pause->SetCanPause(true);
			//クラスを破棄
			DeleteGO(this);
		//}
	}
}