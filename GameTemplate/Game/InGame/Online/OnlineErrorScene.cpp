/// <summary>
/// オンライン通信時の通信エラークラス
/// </summary>
#include "stdafx.h"
#include "OnlineErrorScene.h"
#include "../GameScene.h"
#include "../../Title/TitleScene.h"
#include "../Pause.h"
#include "../../Graphics/SpriteRenderSub.h"
#include "Online.h"

namespace nsKabutoubatu
{
	namespace nsOnlineErrorScene
	{
		//オンライン時じゃないときの通常fps値
		const int NORMAL_FPS_VALUE = 60;
		const int IMAGE_WIDTH = 1280;
		const int IMAGE_HEIGHT = 720;
		const int PAD_NO = 0;
	}

	bool OnlineErrorScene::Start()
	{
		m_pause = FindGO<Pause>(nsStdafx::PAUSE_NAME);

		//ポーズ機能を利用する
		m_pause->SetPause(true);
		//ポーズ機能は使えないようにする
		m_pause->SetCanPause(false);

		//背景を暗くするための画像
		m_grayBack = NewGO<SpriteRenderSub>(nsStdafx::PRIORITY_11);
		m_grayBack->Init("OnlineErrorGrayBack", nsOnlineErrorScene::IMAGE_WIDTH, nsOnlineErrorScene::IMAGE_HEIGHT);
		//エラーメッセージ画像
		m_errorMessageBox = NewGO<SpriteRenderSub>(nsStdafx::PRIORITY_12);
		m_errorMessageBox->Init("ErrorMessageBox", nsOnlineErrorScene::IMAGE_WIDTH, nsOnlineErrorScene::IMAGE_HEIGHT);

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
		////TODO:↓機能していない
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
		if (g_pad[nsOnlineErrorScene::PAD_NO]->IsTrigger(enButtonA))
		{
			//オンラインのインスタンスを削除する
			m_online = FindGO<Online>(nsStdafx::ONLINE_NAME);
			DeleteGO(m_online);
			//ヌルポインタを入れておく。
			m_online = nullptr;

			//ゲームシーンのインスタンスを検索
			m_gameScene = FindGO<GameScene>(nsStdafx::GAMESCENE_NAME);
			//ゲームシーンを削除し、タイトルシーンを生成
			m_gameScene->DeleteGameSceneClass();
			NewGO<TitleScene>(nsStdafx::PRIORITY_0, nsStdafx::TITLESCENE_NAME);
			//ポーズ機能を利用しないようにする
			m_pause->SetPause(false);
			//ポーズ機能を使えるようにする
			m_pause->SetCanPause(true);
			//クラスを破棄
			DeleteGO(this);

			//オンラインは通常60fpsからオンライン用に30fpsまで下げていたので、
			//下げていた分を戻しておく。
			g_engine->SetFrameRateMode(TkEngine::enFrameRateMode_Fix, nsOnlineErrorScene::NORMAL_FPS_VALUE);
		}
	}
}