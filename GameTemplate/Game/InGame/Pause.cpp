/// <summary>
/// ポーズ機能クラス
/// </summary>
#include "stdafx.h"
#include "Pause.h"

namespace nsKabutoubatu
{
	namespace nsPause
	{
		const float PAUSE_SPRITE_TATE = 600.0f;
		const float PAUSE_SPRITE_YOKO = 300.0f;
		const float GRAYBACK_SCA = 1500.0f;
		const float HALF_VOLUME = 0.5f;
	}

	bool Pause::Start()
	{
		m_pauseSprite = NewGO<SpriteRenderSub>(nsStdafx::PRIORITY_6);
		//ポーズ画像を初期化
		m_pauseSprite->Init("Pause", nsPause::PAUSE_SPRITE_TATE, nsPause::PAUSE_SPRITE_YOKO);
		m_pauseSprite->Deactivate();	//初めは非表示

		m_grayBack = NewGO<SpriteRenderSub>(nsStdafx::PRIORITY_1);
		//ポーズ中灰色にする画像を初期化
		m_grayBack->Init("GrayBack", nsPause::GRAYBACK_SCA, nsPause::GRAYBACK_SCA);
		m_grayBack->Deactivate();	//初めは非表示

		return true;
	}

	Pause::~Pause()
	{
		//ポーズ画像を削除
		DeleteGO(m_pauseSprite);
		//ポーズ画面の背景画像を削除
		DeleteGO(m_grayBack);
	}

	void Pause::Update()
	{
		//ポーズ機能を使えないときはreturn以降は実行しない
		if (!m_canPause)
		{
			return;
		}

		//ポーズ機能
		for (int plaNum = enPlayer1; plaNum < enTotalPlayerNum; plaNum++)
		{
			//セレクトボタンが押されたときのみreturn以降を処理する
			if (!g_pad[plaNum]->IsTrigger(enButtonSelect))
			{
				return;
			}
			//TODO:ポーズ画面を開く音再生

			//ポーズにする
			m_pauseSprite->Activate();
			m_grayBack->Activate();

			//ポーズ中にする
			m_isPauseFlg = true;
		}
	}

	//ポーズ中このアップデートしか呼ばれなくなる
	void Pause::PauseUpdate()
	{
		//ポーズ停止機能
		for (int plaNum = enPlayer1; plaNum < enTotalPlayerNum; plaNum++)
		{
			//セレクトボタンが押されたときのみreturn以降を処理する
			if (!g_pad[plaNum]->IsTrigger(enButtonSelect))
			{
				return;
			}

			//TODO:ポーズ閉じる音再生

			m_pauseSprite->Deactivate();
			m_grayBack->Deactivate();

			//ポーズ中を外す
			m_isPauseFlg = false;
		}
	}
}