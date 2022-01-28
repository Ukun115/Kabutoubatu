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
		DeleteGO(m_grayBack);
		DeleteGO(m_errorMessageBox);
	}

	void OnlineErrorScene::PauseUpdate()
	{
		//TODO:↓機能していないので機能させる
		if (m_errorMessageBoxXscale < 1.0f)
		{
			m_errorMessageBoxXscale += 0.01f;
			m_errorMessageBox->SetScale({ m_errorMessageBoxXscale,1.0f,1.0f });
		}

		BackTitle(m_padNo);
	}

	//Aボタンでタイトル画面に戻るメソッド
	void OnlineErrorScene::BackTitle(const int padNo)
	{
		if (g_pad[m_padNo]->IsTrigger(enButtonA))
		{
			//TODO:タイトル画面に戻るようにする。

			////ゲームシーンを削除し、タイトルシーンを生成
			//m_gameScene->SetOnlineError(true);
			//NewGO<TitleScene>(nsStdafx::PRIORITY_0, nsStdafx::TITLESCENE_NAME);

			////クラスを破棄
			//DeleteGO(this);

			//exeを閉じてゲーム終了
			exit(EXIT_SUCCESS);
		}
	}
}