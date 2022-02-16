/// <summary>
/// ゲームシーンクラス
/// </summary>
#include "stdafx.h"
#include "GameScene.h"
#include "Stage/Stage.h"
#include "Player/Player.h"
#include "Player/PlayerStatus.h"
#include "../Result/GameOverScene.h"
#include "GameScreenUI.h"
#include "Camera/PlayerCamera.h"
#include "../Graphics/SkyCube.h"
#include "Item/Weapon/Weapon.h"
#include "GameLimitTime.h"
#include "AccompanyAI/AccompanyAI.h"
#include "../Graphics/Fade.h"

namespace nsKabutoubatu
{
	namespace nsGameScene
	{
		const Vector2   NOWMONEY_POS[2] = { { -400.0f,-238.0f }, { 240.0f,-238.0f } };			//制限時間フォントの位置
		const Vector2   RECOVERY_ITEM_NUM_POS[2] = { { -90.0f,-319.0f }, { 550.0f,-319.0f } };			//制限時間フォントの位置
		const Vector4   FONT_COLOR = { 1.0f,1.0f,1.0f,1.0f };	//制限時間フォントの色
		const float     FONT_ROT = 0.0f;			//フォントの傾き
		const Vector2   FONT_PIV = { 1.0f,1.0f };	//フォントの基点
		const float     FONT_SCA = 0.5f;			//フォントの拡大率
	}

	bool GameScene::Start()
	{

		//ランダム関数の種を決める(数値はなんでもいい)
		//オンラインの際はランダム数値を相手側と合わせないといけないので、
		//ランダムの種を合わせる
		srand(100);

		m_fade = NewGO<Fade>();
		m_fade->Init(1.0f,0,1);

		//スカイキューブを生成
		m_skyCube = NewGO<SkyCube>(nsStdafx::PRIORITY_0, nsStdafx::SKYCUBE_NAME);

		m_gameLimitTime = NewGO<GameLimitTime>(nsStdafx::PRIORITY_0,nsStdafx::GAMELIMITTIME);

		//ゲーム画面のUIを作成
		//他よりも上に表示するために優先度を+1させている。
		m_gameScreenUI = NewGO<GameScreenUI>(nsStdafx::PRIORITY_1,nsStdafx::GAMESCREENUI_NAME);
		m_gameScreenUI->SetPlayerNumber(m_playerNum);

		//プレイヤーカメラ
		m_playerCamera = NewGO <PlayerCamera>(nsStdafx::PRIORITY_0, nsStdafx::PLAYER_CAMERA_NAME);
		m_playerCamera->SetTargetPlayer(m_playerNo);

		//ステージを生成
		m_stage = NewGO<Stage>();
		m_stage->SetPlayerNumber(m_playerNum);

		//オンラインプレイの時、
		if (m_online)
		{
			//プレイヤーを生成
			m_player[m_playerNo] = NewGO<Player>(nsStdafx::PRIORITY_0, nsStdafx::PLAYER_NAME[m_playerNo]);
			//1Pか2Pかの番号をセット
			m_player[m_playerNo]->SetPlayerNum(m_playerNo);
			//操作するパッドの番号をセット
			m_player[m_playerNo]->SetGamePad(*m_playerGamePad);
			//武器をセット
			m_weapon[m_playerNo] = NewGO<Weapon>(nsStdafx::PRIORITY_0, nsStdafx::WEAPON_NAME[m_playerNo]);
			m_weapon[m_playerNo]->SetPlayerNum(m_playerNo);
			//初期装備武器を設定
			m_weapon[m_playerNo]->SetWeaponNum(enWoodStick);
			//現在のお金文字を初期化
			NowMoneyFontInit(m_playerNo);
			//回復アイテム文字を初期化
			RecoveryItemFontInit(m_playerNo);

			//プレイヤーを生成
			m_player[m_otherPlayerNo] = NewGO<Player>(nsStdafx::PRIORITY_0, nsStdafx::PLAYER_NAME[m_otherPlayerNo]);
			//1Pか2Pかの番号をセット
			m_player[m_otherPlayerNo]->SetPlayerNum(m_otherPlayerNo);
			//操作するパッドの番号をセット
			m_player[m_otherPlayerNo]->SetGamePad(*m_otherPlayerGamePad);
			//武器をセット
			m_weapon[m_otherPlayerNo] = NewGO<Weapon>(nsStdafx::PRIORITY_0, nsStdafx::WEAPON_NAME[m_otherPlayerNo]);
			m_weapon[m_otherPlayerNo]->SetPlayerNum(m_otherPlayerNo);
			//初期装備武器を設定
			m_weapon[m_otherPlayerNo]->SetWeaponNum(enWoodStick);
			//現在のお金文字を初期化
			NowMoneyFontInit(m_otherPlayerNo);
			//回復アイテム文字を初期化
			RecoveryItemFontInit(m_otherPlayerNo);

			//プレイヤーのパッド情報をステージに送る
			m_stage->SetPlayerGamePad(*m_playerGamePad, m_playerNo);
			m_stage->SetPlayerGamePad(*m_otherPlayerGamePad, m_otherPlayerNo);
		}
		//オンラインプレイじゃないとき、
		else
		{
			//プレイヤーの人数分生成
			for (int playerNum = enPlayer1; playerNum < m_playerNum; playerNum++)
			{
				//プレイヤーを生成
				m_player[playerNum] = NewGO<Player>(nsStdafx::PRIORITY_0, nsStdafx::PLAYER_NAME[playerNum]);
				//1Pか2Pかの番号をセット
				m_player[playerNum]->SetPlayerNum(playerNum);
				//操作するパッドの番号をセット
				m_player[playerNum]->SetGamePad(*g_pad[playerNum]);
				//武器をセット
				m_weapon[playerNum] = NewGO<Weapon>( nsStdafx::PRIORITY_0,nsStdafx::WEAPON_NAME[playerNum]);
				m_weapon[playerNum]->SetPlayerNum(playerNum);
				//初期装備武器を設定
				m_weapon[playerNum]->SetWeaponNum(enSword);
				//現在のお金文字を初期化
				NowMoneyFontInit(playerNum);
				//回復アイテム文字を初期化
				RecoveryItemFontInit(playerNum);
				if (m_playerNum == 1)
				{
					//ソロモードということをプレイヤークラスに渡す
					m_player[playerNum]->SetIsSoloPlay(true);
					//味方AIを生成
					m_accompanyAI = NewGO<AccompanyAI>(nsStdafx::PRIORITY_0,nsStdafx::ACCOMPANYAI_NAME);
				}
			}
		}

		return true;
	}

	GameScene::~GameScene()
	{
		//プレイヤーの人数分生成
		for (int playerNum = enPlayer1; playerNum < m_playerNum; playerNum++)
		{
			//プレイヤーを削除
			DeleteGO(m_player[playerNum]);
			//お金文字を削除
			DeleteGO(m_nowMoney[playerNum]);
			//
			DeleteGO(m_haveRecoveryItemNum[playerNum]);
			//剣を削除
			DeleteGO(m_weapon[playerNum]);

			//味方AIを削除
			if (m_accompanyAI != nullptr)
			{
				DeleteGO(m_accompanyAI);
			}
		}

		//ゲームの制限時間クラスを削除
		DeleteGO(m_gameLimitTime);

		//スカイキューブを削除
		DeleteGO(m_skyCube);

		//プレイヤーカメラを削除
		DeleteGO(m_playerCamera);

		//ゲーム画面のUIを削除
		DeleteGO(m_gameScreenUI);

		//ステージを削除
		DeleteGO(m_stage);

		//ゲームBGMを削除
		DeleteGO(m_gameSceneSound);
	}

	void GameScene::Update()
	{
		if (m_fade != nullptr && m_fade->GetNowState() == 2)
		{
			DeleteGO(m_fade);
		}

		if (m_gameSceneSound == nullptr)
		{
			//ゲームBGMの初期化
			m_gameSceneSound = NewGO<SoundSource>();
			m_gameSceneSound->Init(L"Assets/sound/DayBGM.wav");
			m_gameSceneSound->SetVolume(0.2f);
			//ゲームBGMを再生
			m_gameSceneSound->Play(true);	//ループ再生
		}

		////プレイヤー１のRBボタンが押されたら、
		//if (g_pad[0]->IsTrigger(enButtonRB1))
		//{
		//	m_skyCubeMapTextureNum++;
		//	InitSky();
		//}

		//通信エラーになったらゲームを破棄する
		if (m_onlineError)
		{
			DeleteGO(this);
		}

		//ゲームオーバー移行処理
		GameOver();

		//制限時間が無くなると、
		if (m_gameLimitTime->TimeOver())
		{
			if (!m_gameOverScene)
			{
				//ゲームオーバー画面に移動
				m_gameOverScene = NewGO<GameOverScene>();
				m_gameOverScene->SetPlayerNumber(m_playerNum);
				m_gameSceneSound->SetVolume(0.0f);
			}
		}

		//プレイヤーの人数分生成
		for (int playerNum = enPlayer1; playerNum < m_playerNum; playerNum++)
		{
			if (m_playerStatus[playerNum] == nullptr)
			{
				m_playerStatus[playerNum] = FindGO<PlayerStatus>(nsStdafx::PLAYER_STATUS_NAME[playerNum]);

				continue;
			}

			//プレイヤーの回復アイテムの所持数をセットし、更新。
			RecoveryItemNumUpdate(playerNum);
			//プレイヤーの現在のお金をセットし、更新。
			NowMoneyNumUpdate(playerNum);
		}
	}

	//ゲームオーバー移行処理メソッド
	void GameScene::GameOver()
	{
		//死んだとき、（ゴースト状態の時）
		if (m_player[enPlayer1]->GetNowState() == 2)
		{
			if (m_playerNum == 2)
			{
				if (m_player[enPlayer2]->GetNowState() != 2)
				{
					return;
				}
			}
			if (!m_gameOverScene)
			{
				//ゲームオーバー画面に移動
				m_gameOverScene = NewGO<GameOverScene>();
				m_gameOverScene->SetPlayerNumber(m_playerNum);
				m_gameSceneSound->SetVolume(0.0f);
			}
		}
	}

	//回復アイテム文字の初期化メソッド
	void GameScene::RecoveryItemFontInit(const int playerNum)
	{
		m_haveRecoveryItemNum[playerNum] = NewGO<FontRender>();
		//回復アイテム所持数フォントの初期化
		m_haveRecoveryItemNum[playerNum]->Init
		(
			L"",			//テキスト
			nsGameScene::RECOVERY_ITEM_NUM_POS[playerNum],	//位置
			nsGameScene::FONT_COLOR,	//色
			nsGameScene::FONT_ROT,		//傾き
			nsGameScene::FONT_SCA,		//拡大率
			nsGameScene::FONT_PIV		//基点
		);
		//文字の境界線表示
		m_haveRecoveryItemNum[playerNum]->SetShadowParam(true, 1.5f, Vector4::Black);
	}

	//お金文字の初期化メソッド
	void GameScene::NowMoneyFontInit(const int playerNum)
	{
		//現在のお金のUIを初期化
		m_nowMoney[playerNum] = NewGO<FontRender>();
		//お金フォントの初期化
		m_nowMoney[playerNum]->Init
		(
			L"",			//テキスト
			nsGameScene::NOWMONEY_POS[playerNum],	//位置
			nsGameScene::FONT_COLOR,	//色
			nsGameScene::FONT_ROT,		//傾き
			nsGameScene::FONT_SCA,		//拡大率
			nsGameScene::FONT_PIV		//基点
		);
		//文字の境界線表示
		m_nowMoney[playerNum]->SetShadowParam(true, 1.5f, Vector4::Black);
	}

	//回復アイテムの所持数文字の数更新
	void GameScene::RecoveryItemNumUpdate(const int playerNum)
	{
		swprintf_s(m_haveRecoveryItemNumText, L"%d", m_playerStatus[playerNum]->GetRecoveryItemNum());
		m_haveRecoveryItemNum[playerNum]->SetText(m_haveRecoveryItemNumText);
	}

	//お金の所持数文字の数更新
	void GameScene::NowMoneyNumUpdate(const int playerNum)
	{
		//プレイヤーの所持している回復アイテム数をセットし、更新。
		swprintf_s(m_nowMoneyText, L"%d", m_playerStatus[playerNum]->GetCoinNum());
		m_nowMoney[playerNum]->SetText(m_nowMoneyText);
	}
}