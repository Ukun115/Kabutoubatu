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
		//カーソルの初期位置
		const int CURSOR_INIT_POS = 0;
		//シーン遷移できるフレーム間隔
		const int CAN_SCENECHANGE_FRAME_INTERVAL = 5;
		//オンライン時じゃないときの通常fps値
		const int NORMAL_FPS_VALUE = 60;
	}

	bool TitleScene::Start()
	{
		//オンライン通信エンジンを生成
		m_online = NewGO<nsKabutoubatu::Online>(nsStdafx::PRIORITY_0, nsStdafx::ONLINE_NAME);

		//タイトルシーンの画像クラスを生成
		m_titleSceneSprite = NewGO<TitleSceneSprite>();

		//タイトルシーンのサウンドクラスを生成
		m_titleSceneSound = NewGO<TitleSceneSound>();

		return true;
	}

	TitleScene::~TitleScene()
	{
		//タイトルシーンの画像クラスを削除
		DeleteGO(m_titleSceneSprite);
	}

	void TitleScene::Update()
	{
		switch (m_selectState)
		{
			//開始or終了選択
		case enStartOrEnd:
			//Aボタンが押されたとき、
			if (g_pad[enPlayer1]->IsTrigger(enButtonA))
			{
				//決定音再生
				m_titleSceneSound->PlayDecideSound();

				//カーソルが指しているものの処理を行う
				switch (m_cursorSelect)
				{
					//はじめる
				case enSelectStart:
					//モード選択に移行
					m_selectState = enGameModeSelect;
					//カーソルを初期位置に戻す
					m_cursorSelect = nsTitleScene::CURSOR_INIT_POS;
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
		case enGameModeSelect:
			//Aボタンが押されたとき、
			if (g_pad[enPlayer1]->IsTrigger(enButtonA))
			{
				//決定音再生
				m_titleSceneSound->PlayDecideSound();

				//カーソルが指しているものの処理を行う
				switch (m_cursorSelect)
				{
					//オンラインモード
				case enSelectOnline:
					//オンラインのルーム作成orルーム参加へ移行
					m_selectState = enOnlineRoomCreateOrJoin;
					//カーソルを初期位置に戻す
					m_cursorSelect = nsTitleScene::CURSOR_INIT_POS;

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
					m_selectState = enStartOrEnd;
					//カーソルを初期位置に戻す
					m_cursorSelect = nsTitleScene::CURSOR_INIT_POS;

					break;
				}
			}

			//カーソルの移動処理
			CursorMove(enSelectBack);

			break;

			//オンラインのルーム作成orルーム参加
		case enOnlineRoomCreateOrJoin:
			//オンラインのデータを削除
			m_online->DeleteData();

			//Aボタンが押されたとき、
			if (g_pad[enPlayer1]->IsTrigger(enButtonA))
			{
				//決定音再生
				m_titleSceneSound->PlayDecideSound();

				//カーソルが指しているものの処理を行う
				switch (m_cursorSelect)
				{
					//ルーム作成
				case enRoomCreate:
					m_online->SetPlayerNo(enPlayer1);	//1Pとして登録
					//オンラインマッチ待機中に移行
					m_selectState = enOnlineMatchWaiting;
					//カーソルを初期位置に戻す
					m_cursorSelect = nsTitleScene::CURSOR_INIT_POS;
					m_isRoomCreate = true;
					break;

					//ルーム参加
				case enRoomJoin:
					m_online->SetPlayerNo(enPlayer2);	//2Pとして登録
					//オンラインマッチ待機中に移行
					m_selectState = enOnlineMatchWaiting;
					//カーソルを初期位置に戻す
					m_cursorSelect = nsTitleScene::CURSOR_INIT_POS;
					m_isRoomCreate = false;
					break;

					//戻る
				case enSelectBackGameMode:
					//ゲームモード選択にもどる
					m_selectState = enGameModeSelect;
					//カーソルを初期位置に戻す
					m_cursorSelect = nsTitleScene::CURSOR_INIT_POS;
					break;
				}
			}

			//カーソルの移動処理
			CursorMove(enSelectBackGameMode);

			break;

			//オンラインマッチ待機中
		case enOnlineMatchWaiting:
			//オンライン処理
			m_online->OnlineInit();

			//Aボタンが押されたとき、
			if (g_pad[enPlayer1]->IsTrigger(enButtonA))
			{
				//決定音再生
				m_titleSceneSound->PlayDecideSound();
				//オンラインのルーム作成orルーム参加に戻る
				m_selectState = enOnlineRoomCreateOrJoin;
				//カーソルを初期位置に戻す
				m_cursorSelect = nsTitleScene::CURSOR_INIT_POS;
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
			// 自分が操作するゲームパッドナンバーを設定する
			m_playerNo = m_online->GetPlayerNo();
			//通信相手のゲームパッドナンバーを設定する
			m_otherPlayerNo = m_online->GetOtherPlayerNo();

			//ゲームパッドの番号を設定
			m_online->GetPlayerGamePad(m_playerNo).Init(m_playerNo);
			m_online->GetPlayerGamePad(m_otherPlayerNo).Init(m_otherPlayerNo);

			//自身のゲームパッドナンバーをゲームシーンクラスに渡す
			m_gameScene->SetPlayerNo(m_playerNo);
			//自身のゲームパッドをゲームシーンクラスに渡す
			m_gameScene->SetPlayerGamePad(m_online->GetPlayerGamePad(m_playerNo));
			//通信相手のゲームパッドナンバーをゲームシーンクラスに渡す
			m_gameScene->SetOtherPlayerNo(m_otherPlayerNo);
			//通信相手のゲームパッドをゲームシーンクラスに渡す
			m_gameScene->SetOtherPlayerGamePad(m_online->GetPlayerGamePad(m_otherPlayerNo));

			//自身のゲームパッド番号を出力にデバック表示
			sprintf(m_gamePadNo, "自分のゲームパッド番号は%dです。\n", m_playerNo);
			OutputDebugStringA(m_gamePadNo);
			//通信相手のゲームパッド番号を出力にデバック表示
			sprintf(m_gamePadNo, "相手のゲームパッド番号は%dです。\n", m_otherPlayerNo);
			OutputDebugStringA(m_gamePadNo);

			m_gameScene->SetOnlinePlay(true);

			m_online->GameStart();

			break;

		//ローカルマルチプレイモード
		case enSelectLocalMult:
			//ポーズ機能を使えるようにする
			m_pause = FindGO<Pause>(nsStdafx::PAUSE_NAME);
			m_pause->SetCanPause(true);
			break;

			//ソロモード
		case enSelectSolo:
			//プレイヤーの人数を一人に変更する。
			m_gameScene->SetPlayerNumber(enPlayer2);

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
		//選択できるのが１つしかない場合はカーソル移動処理を行わない。
		if (lastSelect == nsTitleScene::CURSOR_INIT_POS)
		{
			return;
		}

		//上ボタンが押されたら、
		if (g_pad[enPlayer1]->IsTrigger(enButtonUp))
		{
			//前の選択にカーソル移動
			m_cursorSelect--;
			//もし選択場所が一番上の前だったとき、
			if (m_cursorSelect < nsTitleScene::CURSOR_INIT_POS)
			{
				//選択場所を一番下にする
				m_cursorSelect = lastSelect;
			}

			//選択音再生
			m_titleSceneSound->PlaySelectSound();
		}
		//下ボタンが押されたら、
		if (g_pad[enPlayer1]->IsTrigger(enButtonDown))
		{
			//次の選択にカーソル移動
			m_cursorSelect++;
			//もし選択場所が一番下の次だったとき、
			if (m_cursorSelect > lastSelect)
			{
				//選択場所を一番上にする
				m_cursorSelect = nsTitleScene::CURSOR_INIT_POS;
			}

			//選択音再生
			m_titleSceneSound->PlaySelectSound();
		}
	}
};