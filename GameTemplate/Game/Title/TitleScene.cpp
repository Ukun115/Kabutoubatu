/// <summary>
/// タイトルシーンクラス
/// </summary>
#include "stdafx.h"
#include "TitleScene.h"
#include "../InGame/GameScene.h"
#include "../InGame/Online/Online.h"
#include "../InGame/Pause.h"
#include "TitleSceneSprite.h"
#include "TitleSceneSound.h"

namespace nsKabutoubatu
{
	namespace nsTitleScene
	{
		//タイマーを0に初期化する
		const int TIMER_COUNT_INIT = 0;
		//シーン遷移できるフレーム間隔
		const int CAN_SCENECHANGE_FRAME_INTERVAL = 5;
	}

	bool TitleScene::Start()
	{
		//インスタンスを検索
		m_online = FindGO<Online>(nsStdafx::ONLINE_NAME);

		//タイトルシーンの画像クラス
		m_titleSceneSprite = NewGO<TitleSceneSprite>();

		//タイトルシーンのサウンドクラス
		m_titleSceneSound = NewGO<TitleSceneSound>();

		return true;
	}

	TitleScene::~TitleScene()
	{
		//タイトルシーンの画像クラスを削除
		DeleteGO(m_titleSceneSprite);
		//タイトルシーンのサウンドクラスを削除
		DeleteGO(m_titleSceneSound);
	}

	void TitleScene::Update()
	{
		switch (m_selectState)
		{
		//開始or終了選択
		case 0:
			//Aボタンが押されたとき、
			if (g_pad[0]->IsTrigger(enButtonA))
			{
				//決定音再生
				m_titleSceneSound->PlayDecideSound();

				//カーソルが指しているものの処理を行う
				switch (m_cursorSelect)
				{
				//はじめる
				case enSelectStart:
					//モード選択に移行
					m_selectState = 1;
					//カーソルを初期位置に戻す
					m_cursorSelect = 0;
					break;
				//おわる
				case enSelectEnd:
					//exeを閉じてゲーム終了
					exit(EXIT_SUCCESS);
					//メモ//
					//exit(EXIT_FAILURE);は異常終了		EXIT_FAILURE = 1
					//exit(EXIT_SUCCESS);は正常終了		EXIT_SUCCESS = 0
					break;
				}
			}

			//カーソルの移動処理
			CursorMove(enSelectEnd);

			break;

		//ゲームモード選択
		case 1:
			//Aボタンが押されたとき、
			if (g_pad[0]->IsTrigger(enButtonA))
			{
				//決定音再生
				m_titleSceneSound->PlayDecideSound();

				//カーソルが指しているものの処理を行う
				switch (m_cursorSelect)
				{
				//オンラインモード
				case enSelectOnline:
					//オンラインのルーム作成orルーム参加へ移行
					m_selectState = 2;
					//カーソルを初期位置に戻す
					m_cursorSelect = 0;

					break;

				//ローカルマルチモード
				case enSelectLocalMult:
					//ゲームシーンに遷移する処理
					ChangeGameScene(enSelectLocalMult);

					break;

				//ソロモード
				case enSelectSolo:
					//ゲームシーンに遷移する処理
					ChangeGameScene(enSelectSolo);

					break;

					//もどる
				case enSelectBack:
					//はじめるおわる選択にもどる
					m_selectState = 0;
					//カーソルを初期位置に戻す
					m_cursorSelect = 0;

					break;
				}
			}

			//カーソルの移動処理
			CursorMove(enSelectBack);

			break;

			//オンラインのルーム作成orルーム参加
		case 2:
			//オンラインのデータを削除
			m_online->DeleteData();

			//Aボタンが押されたとき、
			if (g_pad[0]->IsTrigger(enButtonA))
			{
				//決定音再生
				m_titleSceneSound->PlayDecideSound();

				//カーソルが指しているものの処理を行う
				switch (m_cursorSelect)
				{
					//ルーム作成
				case enRoomCreate:
					m_online->SetPlayerNo(0);	//1Pとして登録
					//オンラインマッチ待機中に移行
					m_selectState = 3;
					//カーソルを初期位置に戻す
					m_cursorSelect = 0;
					m_isRoomCreate = true;
					break;

					//ルーム参加
				case enRoomJoin:
					m_online->SetPlayerNo(1);	//2Pとして登録
					//オンラインマッチ待機中に移行
					m_selectState = 3;
					//カーソルを初期位置に戻す
					m_cursorSelect = 0;
					m_isRoomCreate = false;
					break;

					//戻る
				case enSelectBackGameMode:
					//ゲームモード選択にもどる
					m_selectState = 1;
					//カーソルを初期位置に戻す
					m_cursorSelect = 0;
					break;
				}
			}

			//カーソルの移動処理
			CursorMove(enSelectBackGameMode);

			break;

		//オンラインマッチ待機中
		case 3:
			//オンライン処理
			m_online->OnlineInit();

			//Aボタンが押されたとき、
			if (g_pad[0]->IsTrigger(enButtonA))
			{
				//決定音再生
				m_titleSceneSound->PlayDecideSound();
				//オンラインのルーム作成orルーム参加に戻る
				m_selectState = 2;
				//カーソルを初期位置に戻す
				m_cursorSelect = 0;
			}

			//カーソルの移動処理
			CursorMove(0);


			//オンラインマッチング完了したら、
			if (m_online->GetIsJoined())
			{
				//遷移
				ChangeGameScene(enSelectOnline);
			}
			break;
		}

	}

	//ゲームシーンに遷移するメソッド
	void TitleScene::ChangeGameScene(const int mode)
	{
		if (m_gameScene != nullptr)
		{
			return;
		}
		//ゲームシーンを生成
		m_gameScene = NewGO<GameScene>(nsStdafx::PRIORITY_0,nsStdafx::GAMESCENE_NAME);
		switch (mode)
		{
		//オンラインモード
		case enSelectOnline:
			// 自分が操作するゲームパッドナンバーを渡す
			m_playerNo = m_online->GetPlayerNo();
			m_otherPlayerNo = m_online->GetOtherPlayerNo();
			m_gameScene->SetPlayerNo(m_playerNo);
			m_gameScene->SetPlayerGamePad(m_online->GetPlayerGamePad(m_playerNo));
			m_gameScene->SetOtherPlayerNo(m_otherPlayerNo);

			//自身のゲームパッド番号を出力にデバック表示
			sprintf(m_gamePadNo, "自分のゲームパッド番号は%dです。\n", m_playerNo);
			OutputDebugStringA(m_gamePadNo);
			//自身のゲームパッド番号を出力にデバック表示
			sprintf(m_gamePadNo, "相手のゲームパッド番号は%dです。\n", m_otherPlayerNo);
			OutputDebugStringA(m_gamePadNo);

			m_gameScene->SetOtherPlayerGamePad(m_online->GetPlayerGamePad(m_otherPlayerNo));
			m_gameScene->SetOnlinePlay(true);

			m_online->GameStart();

			break;

		//ローカルマルチプレイモード
		case enSelectLocalMult:
			//何もなし。

			break;

			//ソロモード
		case enSelectSolo:
			//プレイヤーの人数を一人に変更する。
			m_gameScene->SetPlayerNumber(1);

			//ポーズ機能を使えるようにする
			m_pause = FindGO<Pause>(nsStdafx::PAUSE_NAME);
			m_pause->SetCanPause(true);

			break;
		}

		//タイトルシーンを破棄
		DeleteGO(this);
	}

	//カーソルの移動処理
	void TitleScene::CursorMove(const int lastSelect)
	{
		if (lastSelect == 0)
		{
			return;
		}

		//上ボタンが押されたら、
		if (g_pad[0]->IsTrigger(enButtonUp))
		{
			//前の選択にカーソル移動
			m_cursorSelect--;
			//もし選択場所が一番上の前だったとき、
			if (m_cursorSelect < 0)
			{
				//選択場所を一番下にする
				m_cursorSelect = lastSelect;
			}

			//選択音再生
			m_titleSceneSound->PlaySelectSound();
		}
		//下ボタンが押されたら、
		if (g_pad[0]->IsTrigger(enButtonDown))
		{
			//次の選択にカーソル移動
			m_cursorSelect++;
			//もし選択場所が一番下の次だったとき、
			if (m_cursorSelect > lastSelect)
			{
				//選択場所を一番上にする
				m_cursorSelect = 0;
			}

			//選択音再生
			m_titleSceneSound->PlaySelectSound();
		}
	}
}