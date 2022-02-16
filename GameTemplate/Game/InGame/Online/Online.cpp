/// <summary>
/// オンライン通信クラス
/// </summary>
#include "stdafx.h"
#include "Online.h"
#include "SyncOnlineTwoPlayerMatchEngine.h"
#include "OnlineErrorScene.h"

namespace nsKabutoubatu
{
	void Online::Update()
	{
		//オンライン処理
		if (m_onlineTwoPlayerMatchEngine)
		{
			m_onlineTwoPlayerMatchEngine->Update();
		}
	}

	//オンライン初期化処理
	void Online::OnlineInit()
	{
		if (!m_onlineTwoPlayerMatchEngine)
		{
			// キャラが選ばれたのオンラインマッチをスタートする。
			m_onlineTwoPlayerMatchEngine = new SyncOnlineTwoPlayerMatchEngine;
			m_onlineTwoPlayerMatchEngine->Init
			(
				L"c9354541-9e42-4465-b556-6c707bb22d85",	//appID
				L"1.0",										//appVer.
				&m_playerNo,
				sizeof(int),
				[&](void* pData, int size) { OnAllPlayerJoined(pData, size); },
				[&]() { OnAllPlayerStartGame(); },
				[&]() { OnError(); }
			);

			OutputDebugStringA("オンライン初期化\n");
		}
	}

	//すべてのプレイヤーがルームにジョインしたときに呼ばれる処理。
	void Online::OnAllPlayerJoined(void* pData, int size)
	{
		m_isJoined = true;

		OutputDebugStringA("全てのプレイヤーがルームに参加\n");
	}

	//すべてのプレイヤーがゲームスタートした時に呼ばれる処理。
	void Online::OnAllPlayerStartGame()
	{
		m_step = enStep_InGame;

		OutputDebugStringA("全てのプレイヤーがゲームをスタート\n");
	}

	//通信エラーが起きた。
	void Online::OnError()
	{
		OutputDebugStringA("通信エラーが起きました。\n");

		m_step = enStep_Error;

		//通信エラーシーンに遷移
		m_onlineErrorScene = NewGO<OnlineErrorScene>();
		m_onlineErrorScene->SetPlayerGamePad(m_onlineTwoPlayerMatchEngine->GetGamePad(m_playerNo));
	}

	void Online::DeleteData()
	{
		//オンラインマッチングしてしまわないように削除しておく
		if (m_playerNo != -1)
		{
			m_playerNo = -1;
			//削除
			delete m_onlineTwoPlayerMatchEngine;
			//nullポインタにしておく。
			m_onlineTwoPlayerMatchEngine = nullptr;
			//ステップをキャラクターセレクトに戻す
			m_step = enStep_CharacterSelect;

			OutputDebugStringA("オンライン削除\n");
		}
	}

	void Online::GameStart()
	{
		// ロードが終わってゲーム開始可能になったことを通知する。
		m_onlineTwoPlayerMatchEngine->NotifyPossibleStartPlayGame();
		// ほかのプレイヤーがゲーム開始可能になるまで待つ。
		m_step = enStep_WaitAllPlayerStartGame;

		OutputDebugStringA("ゲーム開始可能\n");
	}

	GamePad& Online::GetPlayerGamePad(int no)
	{
		return m_onlineTwoPlayerMatchEngine->GetGamePad(no);
	};
}