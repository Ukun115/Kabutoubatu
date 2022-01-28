/// <summary>
/// ゲームクリアシーンクラス
/// </summary>
#include "stdafx.h"
#include "GameClearScene.h"
#include "../Utilities/GameUpdateStop.h"
#include "../InGame/Player/PlayerStatus.h"
#include "../Title/TitleScene.h"
#include "../InGame/GameScene.h"

namespace nsKabutoubatu
{
	namespace nsGameClearScene
	{
		const Vector4   FONT_COLOR = { 1.0f,1.0f,1.0f,1.0f };
		const float     FONT_ROT = 0.0f;
		const Vector2   FONT_PIV = { 1.0f,1.0f };
		const float     FONT_SCA = 0.7f;
	}

	bool GameClearScene::Start()
	{
		//インスタンスを検索
		m_gameScene = FindGO<GameScene>(nsStdafx::GAMESCENE_NAME);
		m_gameUpdateStop = FindGO<GameUpdateStop>(nsStdafx::GAMEUPDATESTOP_NAME);

		//ゲームの更新を止める
		m_gameUpdateStop->SetIsStop(true);
		//UI移動に切り替える
		m_gameUpdateStop->SetMoveTarget(0);

		for (int playerNum = 0; playerNum < m_playerNum; playerNum++)
		{
			//インスタンスを検索
			m_playerStatus[playerNum] = FindGO<PlayerStatus>(nsStdafx::PLAYER_STATUS_NAME[playerNum]);

			//敵を倒した数/////////////////////////////////////////////////////////////////
			switch (playerNum)
			{
			case 0:
				m_fontPos.x = -15.0f;
				break;
			case 1:
				m_fontPos.x = 160.0f;
				break;
			}
			m_fontPos.y = 50.0f;
			m_enemyKillNumberFont[playerNum] = NewGO<FontRenderSub>();
			m_enemyKillNumberFont[playerNum]->Init
			(
				L"",		//テキスト
				m_fontPos,	//位置
				nsGameClearScene::FONT_COLOR,	//色
				nsGameClearScene::FONT_ROT,		//傾き
				nsGameClearScene::FONT_SCA,		//拡大率
				nsGameClearScene::FONT_PIV		//基点
			);

			swprintf_s(m_text, L"%d", m_playerStatus[playerNum]->GetEnemyKillNum());
			m_enemyKillNumberFont[playerNum]->SetText(m_text);
			//初めは非表示
			m_enemyKillNumberFont[playerNum]->Deactivate();
			m_enemyKillNumberFont[playerNum]->StartEnd();
			//位置ずらし
			for (int i = 1; i < 10; i++)
			{
				if (m_playerStatus[playerNum]->GetEnemyKillNum() >= pow(10, i))
				{
					m_fontPos.x -= 6.0f;
				}
				else
				{
					//位置セット
					m_enemyKillNumberFont[playerNum]->SetPosition(m_fontPos);
					//for文を終わらせる
					i = 10;
				}
			}
			///////////////////////////////////////////////////////////////////////////////////


			//コイン枚数///////////////////////////////////////////////////////////////////////
			switch (playerNum)
			{
			case 0:
				m_fontPos.x = -15.0f;
				break;
			case 1:
				m_fontPos.x = 160.0f;
				break;
			}
			m_fontPos.y = -2.0f;
			m_coinGetNumberFont[playerNum] = NewGO<FontRenderSub>();
			m_coinGetNumberFont[playerNum]->Init
			(
				L"",		//テキスト
				m_fontPos,	//位置
				nsGameClearScene::FONT_COLOR,	//色
				nsGameClearScene::FONT_ROT,		//傾き
				nsGameClearScene::FONT_SCA,		//拡大率
				nsGameClearScene::FONT_PIV		//基点
			);

			swprintf_s(m_text, L"%d", m_playerStatus[playerNum]->GetTotalCoinNum());
			m_coinGetNumberFont[playerNum]->SetText(m_text);
			//初めは非表示
			m_coinGetNumberFont[playerNum]->Deactivate();
			m_coinGetNumberFont[playerNum]->StartEnd();

			//位置ずらし
			for (int i = 1; i < 10; i++)
			{
				if (m_playerStatus[playerNum]->GetTotalCoinNum() >= pow(10, i))
				{
					m_fontPos.x -= 6.0f;
				}
				else
				{
					//位置セット
					m_coinGetNumberFont[playerNum]->SetPosition(m_fontPos);
					//for文を終わらせる
					i = 10;
				}
			}
			///////////////////////////////////////////////////////////////////////////////////


			//回復回数/////////////////////////////////////////////////////////////////////////
			switch (playerNum)
			{
			case 0:
				m_fontPos.x = -15.0f;
				break;
			case 1:
				m_fontPos.x = 160.0f;
				break;
			}
			m_fontPos.y = -54.0f;
			m_recoveryReceiveNumberFont[playerNum] = NewGO<FontRenderSub>();
			m_recoveryReceiveNumberFont[playerNum]->Init
			(
				L"",		//テキスト
				m_fontPos,	//位置
				nsGameClearScene::FONT_COLOR,	//色
				nsGameClearScene::FONT_ROT,		//傾き
				nsGameClearScene::FONT_SCA,		//拡大率
				nsGameClearScene::FONT_PIV		//基点
			);

			swprintf_s(m_text, L"%d", m_playerStatus[playerNum]->GetRecoveryReceiveNum());
			m_recoveryReceiveNumberFont[playerNum]->SetText(m_text);
			//初めは非表示
			m_recoveryReceiveNumberFont[playerNum]->Deactivate();
			m_recoveryReceiveNumberFont[playerNum]->StartEnd();
			//位置ずらし
			for (int i = 1; i < 10; i++)
			{
				if (m_playerStatus[playerNum]->GetRecoveryReceiveNum() >= pow(10, i))
				{
					m_fontPos.x -= 6.0f;
				}
				else
				{
					//位置セット
					m_recoveryReceiveNumberFont[playerNum]->SetPosition(m_fontPos);
					//for文を終わらせる
					i = 10;
				}
			}
			///////////////////////////////////////////////////////////////////////////////////


			//死亡回数/////////////////////////////////////////////////////////////////////////
			switch (playerNum)
			{
			case 0:
				m_fontPos.x = -15.0f;
				break;
			case 1:
				m_fontPos.x = 160.0f;
				break;
			}
			m_fontPos.y = -106.0f;
			m_deathNumberFont[playerNum] = NewGO<FontRenderSub>();
			m_deathNumberFont[playerNum]->Init
			(
				L"",		//テキスト
				m_fontPos,	//位置
				nsGameClearScene::FONT_COLOR,	//色
				nsGameClearScene::FONT_ROT,		//傾き
				nsGameClearScene::FONT_SCA,		//拡大率
				nsGameClearScene::FONT_PIV		//基点
			);

			swprintf_s(m_text, L"%d", m_playerStatus[playerNum]->GetDeathNum());
			m_deathNumberFont[playerNum]->SetText(m_text);
			//初めは非表示
			m_deathNumberFont[playerNum]->Deactivate();
			m_deathNumberFont[playerNum]->StartEnd();
			//位置ずらし
			for (int i = 1; i < 10; i++)
			{
				if (m_playerStatus[playerNum]->GetDeathNum() >= pow(10, i))
				{
					m_fontPos.x -= 6.0f;
				}
				else
				{
					//位置セット
					m_deathNumberFont[playerNum]->SetPosition(m_fontPos);
					//for文を終わらせる
					i = 10;
				}
			}
			///////////////////////////////////////////////////////////////////////////////////
		}

		//ゲームクリアサウンドを再生
		m_gameClearSound = NewGO<SoundSource>();
		m_gameClearSound->Init(L"Assets/sound/GameClear.wav");
		m_gameClearSound->SetVolume(0.7f);
		m_gameClearSound->Play(false);
		//「討伐完了！！」画像を作成
		m_toubatuKannryou = NewGO<SpriteRenderSub>(nsStdafx::PRIORITY_0);
		m_toubatuKannryou->Init("toubatuKannryou", 1280, 820);

		//ゲームクリア画像を作成
		m_gameClearSprite = NewGO<SpriteRenderSub>(nsStdafx::PRIORITY_1);
		m_gameClearSprite->Init("GameClear", 1400.0f, 800.0f);
		m_gameClearSprite->SetMulColor(1.0f,1.0f,1.0f,0.0f);

		return true;
	}

	GameClearScene::~GameClearScene()
	{
		//ゲームクリアサウンドを削除
		DeleteGO(m_gameClearSound);
		//「討伐完了！！」画像を削除
		DeleteGO(m_toubatuKannryou);
		//ゲームクリア画像を削除
		DeleteGO(m_gameClearSprite);
		//決定サウンドを削除
		DeleteGO(m_decideSound);
		//フォント
		for (int playerNum = 0; playerNum < m_playerNum; playerNum++)
		{
			DeleteGO(m_enemyKillNumberFont[playerNum]);
			DeleteGO(m_coinGetNumberFont[playerNum]);
			DeleteGO(m_recoveryReceiveNumberFont[playerNum]);
			DeleteGO(m_deathNumberFont[playerNum]);
		}
	}

	void GameClearScene::UiUpdate()
	{
		if (m_spriteChangeTimer < 300)
		{
			m_spriteChangeTimer++;

			//ゲームクリア音が終了したら、
			if (m_spriteChangeTimer == 250)
			{
				//「討伐完了！！」画像を隠す
				m_toubatuKannryou->Deactivate();
				//ゲームクリア画像を表示
				m_gameClearSprite->SetMulColor(1.0f, 1.0f, 1.0f, 1.0f);

				//スコアを表示
				for (int playerNum = 0; playerNum < m_playerNum; playerNum++)
				{
					m_enemyKillNumberFont[playerNum]->Activate();
					m_coinGetNumberFont[playerNum]->Activate();
					m_recoveryReceiveNumberFont[playerNum]->Activate();
					m_deathNumberFont[playerNum]->Activate();
				}
			}
		}
		else
		{
			//Aボタンが押されたらタイトル画面に戻る
			for (int playerNum = 0; playerNum < m_playerNum; playerNum++)
			{
				if (m_decideSound == nullptr)
				{
					if (g_pad[playerNum]->IsTrigger(enButtonA))
					{
						//決定サウンド
						m_decideSound = NewGO<SoundSource>();
						m_decideSound->Init(L"Assets/sound/Decide.wav");
						m_decideSound->SetVolume(0.5f);
						//決定サウンドを再生
						m_decideSound->Play(false);	//ワンショット再生

						m_gameUpdateStop->SetIsStop(false);
						//ゲームシーンを削除する
						m_gameScene->DeleteGameSceneClass();
						//タイトルシーンを生成
						NewGO<TitleScene>(nsStdafx::PRIORITY_0, nsStdafx::TITLESCENE_NAME);
						//このクラスを破棄する
						DeleteGO(this);
					}
				}
			}
		}
	}
}