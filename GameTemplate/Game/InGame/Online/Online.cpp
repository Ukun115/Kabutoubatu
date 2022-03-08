/// <summary>
/// オンライン通信クラス
/// </summary>
#include "stdafx.h"
#include "Online.h"
#include "SyncOnlineTwoPlayerMatchEngine.h"
#include "OnlineErrorScene.h"
#include "OnlineUpdateSpeed.h"

namespace nsKabutoubatu
{
	namespace nsOnline
	{
		//プレイヤー番号の初期化値
		int PLAYER_NO_INIT_VALUE = -1;
	}

	void Online::Update()
	{
		//ロビーの更新処理
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
				&m_playerNo,								//プレイヤー番号
				sizeof(int),
				[&](void* pData, int size) { OnAllPlayerJoined(pData, size); },
				[&]() { OnAllPlayerStartGame(); },
				[&]() { OnError(); }
			);

			//デバックログ
			OutputDebugStringA("オンライン初期化\n");

			m_onlineUpdateSpeed = FindGO<OnlineUpdateSpeed>(nsStdafx::ONLINEUPDATESPEED_NAME);
		}
	}

	//すべてのプレイヤーがルームにジョインしたときに呼ばれる処理。
	void Online::OnAllPlayerJoined(void* pData, int size)
	{
		m_isJoined = true;

		//デバックログ
		OutputDebugStringA("全てのプレイヤーがルームに参加\n");
	}

	//すべてのプレイヤーがゲームスタートした時に呼ばれる処理。
	void Online::OnAllPlayerStartGame()
	{
		//ステップをインゲームにする
		m_step = enStep_InGame;

		//デバックログ
		OutputDebugStringA("全てのプレイヤーがゲームをスタート\n");

		m_onlineUpdateSpeed->SetIsOnline(true);
	}

	//通信エラーが起きた。
	void Online::OnError()
	{
		//デバックログ
		OutputDebugStringA("通信エラーが起きました。\n");

		//ステップをエラーにする
		m_step = enStep_Error;

		//通信エラーシーンに遷移
		m_onlineErrorScene = NewGO<OnlineErrorScene>();
		m_onlineErrorScene->SetPlayerGamePad(m_onlineTwoPlayerMatchEngine->GetGamePad(m_playerNo));

		m_onlineUpdateSpeed->SetIsOnline(false);
	}

	void Online::DeleteData()
	{
		//オンラインマッチングしてしまわないように削除しておく
		if (m_playerNo != nsOnline::PLAYER_NO_INIT_VALUE)
		{
			m_playerNo = nsOnline::PLAYER_NO_INIT_VALUE;
			//削除
			delete m_onlineTwoPlayerMatchEngine;
			//ヌルポインタを入れておく。
			m_onlineTwoPlayerMatchEngine = nullptr;
			//ステップをキャラクターセレクトに戻す
			m_step = enStep_CharacterSelect;

			//デバックログ
			OutputDebugStringA("オンライン削除\n");

			m_onlineUpdateSpeed->SetIsOnline(false);
		}
	}

	void Online::GameStart()
	{
		// ロードが終わってゲーム開始可能になったことを通知する。
		m_onlineTwoPlayerMatchEngine->NotifyPossibleStartPlayGame();
		// ほかのプレイヤーがゲーム開始可能になるまで待つ。
		m_step = enStep_WaitAllPlayerStartGame;

		//デバックログ
		OutputDebugStringA("ゲーム開始可能\n");

		m_onlineUpdateSpeed->SetIsOnline(true);
	}

	//プレイヤーのゲームパッドを取得する関数
	GamePad& Online::GetPlayerGamePad(const int no)
	{
		return m_onlineTwoPlayerMatchEngine->GetGamePad(no);
	};
}