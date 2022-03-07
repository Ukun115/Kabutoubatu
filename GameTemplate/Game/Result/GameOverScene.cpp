/// <summary>
/// ゲームオーバーシーンクラス
/// </summary>
#include "stdafx.h"
#include "GameOverScene.h"
#include "../InGame/GameScene.h"
#include "../Title/TitleScene.h"
#include "../Utilities/GameUpdateStop.h"

namespace nsKabutoubatu
{
	namespace nsGameOverScene
	{

	}

	bool GameOverScene::Start()
	{
		m_gameUpdateStop = FindGO<GameUpdateStop>(nsStdafx::GAMEUPDATESTOP_NAME);
		m_gameUpdateStop->SetIsStop(true);
		m_gameUpdateStop->SetMoveTarget(0);

		m_gameScene = FindGO<GameScene>(nsStdafx::GAMESCENE_NAME);

		//ゲームオーバージングル
		m_gameOverGingle = NewGO<SoundSource>();
		m_gameOverGingle->Init(L"Assets/sound/GameOver.wav");
		m_gameOverGingle->SetVolume(0.5f);
		//ゲームオーバージングルを再生
		m_gameOverGingle->Play(false);	//ワンショット再生

		//背景画像を作成
		m_gameOverSprite = NewGO<SpriteRender>(nsStdafx::PRIORITY_0);
		m_gameOverSprite->Init("GameOver", 1400.0f, 800.0f);
		m_gameOverSprite->SetMulColor(1.0f, 1.0f, 1.0f, m_alphaValue);

		m_aPushSprite = NewGO<SpriteRender>(nsStdafx::PRIORITY_0);
		m_aPushSprite->Init("GameOverAPush", 1400.0f, 800.0f);
		m_aPushSprite->SetMulColor(1.0f, 1.0f, 1.0f, m_alphaValue);

		return true;
	}

	GameOverScene::~GameOverScene()
	{
		//背景画像を削除
		DeleteGO(m_gameOverSprite);
		//「Aボタンでタイトル画面に戻る」画像を削除
		DeleteGO(m_aPushSprite);
	}

	void GameOverScene::UiUpdate()
	{
		if (m_alphaValue <= 1.0f)
		{
			m_alphaValue += 0.008f;
			m_gameOverSprite->SetMulColor(1.0f, 1.0f, 1.0f, m_alphaValue);
			m_aPushSprite->SetMulColor(1.0f, 1.0f, 1.0f, m_alphaValue);
		}
		else
		{
			//Aボタンが押されたらタイトル画面に戻る
			for (int playerNum = enPlayer1; playerNum < m_playerNum; playerNum++)
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