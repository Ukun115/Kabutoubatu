/// <summary>
/// ゲーム画面UIクラス
/// </summary>
#include "stdafx.h"
#include "GameScreenUI.h"
#include "Player/PlayerStatus.h"

//UIのレイヤー
//０.フレームの背景　→　１.フレーム　→　２.アイコン　→　３.ヒットポイントUI

namespace nsKabutoubatu
{
	namespace nsGameScreenUI
	{
		const char* KOTEI_UI_NAME[3] = { "GameScreenUI_Back","GameScreenUI_flame","GameScreenUI_Icon_second" };
		const char* PLAYER_ICON_UI_NAME[2] = { "Player1_icon","Player2_icon" };

		const Vector2   ITEM_NUM_POS[2] = { { -650.0f,-310.0f }, { -15.0f,-310.0f } };			//制限時間フォントの位置
		const Vector4   FONT_COLOR = { 1.0f,1.0f,1.0f,1.0f };	//制限時間フォントの色
		const float     FONT_ROT = 0.0f;			//フォントの傾き
		const Vector2   FONT_PIV = { 1.0f,1.0f };	//フォントの基点
		const float     FONT_SCA = 0.5f;			//フォントの拡大率
	}

	bool GameScreenUI::Start()
	{
		//プレイヤー２人分
		for (int playerNum = enPlayer1; playerNum < m_playerNum; playerNum++)
		{
			//プレイヤーのインスタンスを検索
			m_playerStatus[playerNum] = FindGO<PlayerStatus>(nsStdafx::PLAYER_STATUS_NAME[playerNum]);

			//ヒットポイントUIを初期化
			HitPointUiInit(playerNum);

			//武器UIを初期化
			HaveWeaponUiInit(playerNum);

			//アイテムUIを初期化
			ItemUiInit(playerNum);
			//アイテム数フォントを初期化
			ItemFontInit(playerNum);

			//プレイヤーのアイコン画像
			m_playerIconUI[playerNum] = NewGO<SpriteRender>(nsStdafx::PRIORITY_3);
			m_playerIconUI[playerNum]->Init(nsGameScreenUI::PLAYER_ICON_UI_NAME[playerNum], 1280, 720);
		}
		//固定UI初期化
		for (int fixedUiNum = enFlame; fixedUiNum < enUiNum; fixedUiNum++)
		{
			m_fixedUI[fixedUiNum] = NewGO<SpriteRender>(fixedUiNum);
			m_fixedUI[fixedUiNum]->Init(nsGameScreenUI::KOTEI_UI_NAME[fixedUiNum], 1280, 720);
		}

		return true;
	}

	GameScreenUI::~GameScreenUI()
	{
		//固定UI削除
		for (int uiNum = enFlame; uiNum < enUiNum; uiNum++)
		{
			DeleteGO(m_fixedUI[uiNum]);
		}
		//ヒットポイントUIを削除
		for (int playerNum = enPlayer1; playerNum < m_playerNum; playerNum++)
		{
			for (int hitPointNum = 0; hitPointNum < 10; hitPointNum++)
			{
				DeleteGO(m_HitPointUI[playerNum][hitPointNum]);
			}
			//所持武器UIを削除
			for (int weaponNum = enWoodStick; weaponNum < enWeaponNum; weaponNum++)
			{
				DeleteGO(m_haveWeaponUI[playerNum][weaponNum]);
			}
		}

		//プレイヤーのアイコン画像を削除
		for (int playerNum = enPlayer1; playerNum < enPlayerNum; playerNum++)
		{
			DeleteGO(m_playerIconUI[playerNum]);
		}

		//ステータスアップアイテムの数フォントを削除
		for (int playerNum = enPlayer1; playerNum < m_playerNum; playerNum++)
		{
			for (int itemNum = enItem1; itemNum < enItemNum; itemNum++)
			{
				DeleteGO(m_itemUI[playerNum][itemNum]);
				DeleteGO(m_haveItemNum[playerNum][itemNum]);
			}
		}
	}

	void GameScreenUI::Update()
	{
		//プレイヤーのヒットポイントを表示状態にするか非表示状態にするかを更新する処理
		HitPointUiUpdate();

		//ヒットポイントが少ないときにハートを点滅させる処理
		BlinkingHeart();

		//所持武器の画像表示更新処理
		HaveWeapon();

		//所持アイテム数更新
		ItemNumUpdate();
	}

	//ヒットポイントUIの初期化メソッド
	void GameScreenUI::HitPointUiInit(const int playerNum)
	{
		//ヒットポイントUIを初期化
		for (int hitPointNum = 0; hitPointNum < 10; hitPointNum++)
		{
			m_HitPointUI[playerNum][hitPointNum] = NewGO<SpriteRender>(nsStdafx::PRIORITY_3);
			switch (playerNum)
			{
			case enPlayer1:
				sprintf(m_hitPointFileName, "Player1_HP_%d_second", (hitPointNum + 1));
				break;
			case enPlayer2:
				sprintf(m_hitPointFileName, "Player2_HP_%d_second", (hitPointNum + 1));
				break;
			}
			m_HitPointUI[playerNum][hitPointNum]->Init(m_hitPointFileName, 1280, 720);
		}
	}

	void GameScreenUI::HaveWeaponUiInit(const int playerNum)
	{
		for (int weaponNum = enWoodStick; weaponNum < enWeaponNum; weaponNum++)
		{
			m_haveWeaponUI[playerNum][weaponNum] = NewGO<SpriteRender>(nsStdafx::PRIORITY_3);
			switch (weaponNum)
			{
			//木の棒
			case enWoodStick:
				sprintf(m_weaponFileName, "Player%d_woodStick", playerNum + 1);
				break;
			//剣
			case enSword:
				sprintf(m_weaponFileName, "Player%d_sword", playerNum + 1);
				break;
			//大剣
			case enLargeSword:
				sprintf(m_weaponFileName, "Player%d_largeSword", playerNum + 1);
				break;
			case enBow:
				sprintf(m_weaponFileName, "Player%d_Bow", playerNum + 1);
				break;
			}
			m_haveWeaponUI[playerNum][weaponNum]->Init(m_weaponFileName, 1280, 720);
			m_haveWeaponUI[playerNum][weaponNum]->Deactivate();	//非表示
		}
	}

	void GameScreenUI::ItemUiInit(const int playerNum)
	{
		for (int itemNum = enItem1; itemNum < enItemNum; itemNum++)
		{
			m_itemUI[playerNum][itemNum] = NewGO<SpriteRender>(nsStdafx::PRIORITY_3);
			switch (itemNum)
			{
				//アイテム１
			case enItem1:
				sprintf(m_itemFileName, "Player%d_Item1_have", playerNum + 1);
				break;
				//アイテム２
			case enItem2:
				sprintf(m_itemFileName, "Player%d_Item2_have", playerNum + 1);
				break;
				//アイテム３
			case enItem3:
				sprintf(m_itemFileName, "Player%d_Item3_have", playerNum + 1);
				break;
				//アイテム４
			case enItem4:
				sprintf(m_itemFileName, "Player%d_Item4_have", playerNum + 1);
				break;
				//アイテム５
			case enItem5:
				sprintf(m_itemFileName, "Player%d_Item5_have", playerNum + 1);
				break;
			}
			m_itemUI[playerNum][itemNum]->Init(m_itemFileName, 1280, 720);
		}
	}

	//プレイヤーのヒットポイントを表示状態にするか非表示状態にするかを更新する処理メソッド
	void GameScreenUI::HitPointUiUpdate()
	{
		for (int playerNum = enPlayer1; playerNum < m_playerNum; playerNum++)
		{
			//表示
			for (int nowHitPoint = 0; nowHitPoint < m_playerStatus[playerNum]->GetHitPoint(); nowHitPoint++)
			{
				m_HitPointUI[playerNum][nowHitPoint]->Activate();
			}
			//非表示
			for (int nowHitPoint = m_playerStatus[playerNum]->GetHitPoint(); nowHitPoint < m_playerStatus[playerNum]->GetMaxHitPoint(); nowHitPoint++)
			{
				if (nowHitPoint < 0)
				{
					continue;
				}
				m_HitPointUI[playerNum][nowHitPoint]->Deactivate();
			}
		}
	}

	//ヒットポイントが少ないときハートが点滅する処理
	void GameScreenUI::BlinkingHeart()
	{
		for (int playerNum = enPlayer1; playerNum < m_playerNum; playerNum++)
		{
			//残りヒットポイントが１のとき、
			if (m_playerStatus[playerNum]->GetHitPoint() == 1)
			{
				//点滅
				if (m_blinkingTimer[playerNum] < 30)
				{
					//表示
					m_HitPointUI[playerNum][0]->Activate();
				}
				else if(m_blinkingTimer[playerNum] < 60)
				{
					//非表示
					m_HitPointUI[playerNum][0]->Deactivate();
				}
				else
				{
					//タイマーを初期化
					m_blinkingTimer[playerNum] = 0;
				}
					m_blinkingTimer[playerNum]++;
			}
			else if(m_playerStatus[playerNum]->GetHitPoint() != 0)
			{
				//タイマーを初期化
				m_blinkingTimer[playerNum] = 0;
				//表示
				m_HitPointUI[playerNum][0]->Activate();
			}
		}
	}

	//所持武器の画像表示更新処理
	void GameScreenUI::HaveWeapon()
	{
		for (int playerNum = enPlayer1; playerNum < m_playerNum; playerNum++)
		{
			//一旦全部非表示
			for (int weaponNum = enWoodStick; weaponNum < enWeaponNum; weaponNum++)
			{
				m_haveWeaponUI[playerNum][weaponNum]->Deactivate();
			}
			//持っている武器UIだけ表示
			m_haveWeaponUI[playerNum][m_playerStatus[playerNum]->GetHaveWeapon()]->Activate();
		}
	}

	//アイテム所持数文字の初期化メソッド
	void GameScreenUI::ItemFontInit(const int playerNum)
	{
		m_itemFontPos = nsGameScreenUI::ITEM_NUM_POS[playerNum];

		for (int itemNum = enItem1; itemNum < enItemNum; itemNum++)
		{
			m_itemFontPos.x += 91.5f;

			m_haveItemNum[playerNum][itemNum] = NewGO<FontRender>();
			m_haveItemNum[playerNum][itemNum]->Init
			(
				L"",			//テキスト
				m_itemFontPos,	//位置
				nsGameScreenUI::FONT_COLOR,	//色
				nsGameScreenUI::FONT_ROT,		//傾き
				nsGameScreenUI::FONT_SCA,		//拡大率
				nsGameScreenUI::FONT_PIV		//基点
			);
			//文字の境界線表示
			m_haveItemNum[playerNum][itemNum]->SetShadowParam(true, 1.5f, Vector4::Black);
		}
	}

	//能力アップアイテムの所持数文字の数更新
	void GameScreenUI::ItemNumUpdate()
	{
		for (int playerNum = enPlayer1; playerNum < m_playerNum; playerNum++)
		{
			for (int itemNum = enItem1; itemNum < enItemNum; itemNum++)
			{
				if (m_itemNum[playerNum][itemNum] > 0)
				{
					m_itemUI[playerNum][itemNum]->SetMulColor(1.0f, 1.0f, 1.0f, 1.0f);
				}
				else
				{
					m_itemUI[playerNum][itemNum]->SetMulColor(1.0f, 1.0f, 1.0f, 1.0f);
					//m_itemUI[playerNum][itemNum]->SetMulColor(0.15f, 0.15f, 0.15f, 1.0f);
				}
				//プレイヤーの所持している回復アイテム数をセットし、更新。
				swprintf_s(m_itemText, L"%d", m_itemNum[playerNum][itemNum]);
				m_haveItemNum[playerNum][itemNum]->SetText(m_itemText);
			}
		}
	}
}