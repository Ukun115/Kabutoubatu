/// <summary>
/// アイテムショップシーンの選択文字クラス
/// </summary>
#include "stdafx.h"
#include "ItemShopSelectFont.h"

namespace nsKabutoubatu
{
	namespace nsItemShopSceneFont
	{
		const Vector4   FONT_COLOR = { 1.0f,1.0f,1.0f,1.0f };	//制限時間フォントの色
		const float     FONT_ROT = 0.0f;			//フォントの傾き
		const Vector2   FONT_PIV = { 1.0f,1.0f };	//フォントの基点
		const float     FONT_SCA = 1.0f;			//フォントの拡大率

		const Vector2 ITEM_VALUE_POS[3] = { {-320.0f,-160.0f},{-50.0f,-160.0f},{220.0f,-160.0f} };

		const wchar_t* CURSOR_NAME = L"o";
		const wchar_t* KUD_NAME[3] = { L"買う",L"売る",L"出る" };
		const wchar_t* ITEM1KARA4_NAME[4] = { L"俊足シューズ",L"回復箱",L"弓",L"戻る" };
		const wchar_t* STATUS_UP_ITEM[6] = { L"俊足シューズ",L"金運小判", L"天使の翼", L"回復箱", L"ハートクッキー", L"戻る" };
		const wchar_t* ITEM_NEDAN_NAME[3] = { L"    100",L"    200",L"    500" };
		const wchar_t* WAIT_NAME = L"待機中...";
		const wchar_t* ITEM_SETUMEI_SENTENCE[3] = { L"ステータス上昇\nアイテムです。\n自身の移動速度が\n上がります。\n( 所持上限数 :10 )",
													L"回復アイテムです。\n味方を回復する\nことができます。\n自分には使えない\nので注意!\n( 所持上限数 :50 )",
													L"武器アイテムです。\n遠距離から敵を攻撃\nすることができます。\n( 矢は無限です )" };
		const wchar_t* STATUS_UP_ITEM_SETUMEI_SENTENCE[5] = { L"ステータス上昇\nアイテムです。\n自身の移動速度が\n上がります。\n( 所持上限数 :10 )",
															  L"取得コイン上昇\nアイテムです。\n買うごとに取得倍率が\n+0.1倍します。\n( 所持上限数 :10 )",
															  L"ジャンプ力上昇\nアイテムです。\n買うごとにジャンプの\n高さが高くなって\nいきます。\n( 所持上限数 :10 )",
															  L"回復アイテムです。\n味方を回復する\nことができます。\n自分には使えない\nので注意!\n( 所持上限数 :50 )",
															  L"回復力上昇\nアイテムです。\n買うごとに味方を\n回復する力が+1\n上昇します。\n( 所持上限数 :50 )" };
		const Vector2 ITEM_SETUMEI_SENTENCE_POS[2] = { {-550.0f,70.0f},{350.0f,70.0f} };
		const Vector2 SETUMEI_ITEM_NAME_POS[2] = { {-550.0f,120.0f},{350.0f,120.0f} };
	}
	//俊足シューズ
	//招きにゃんこ
	//天使の翼
	//回復箱
	//聖水

	bool ItemShopSelectFont::Start()
	{
		switch (m_playerNum)
		{
		//1Pの場合
		case 0:
			for (int fontNum = 0; fontNum < 6; fontNum++)
			{
				FONT_POS[fontNum].x = -550.0f;
				FONT_POS[fontNum].y -= 50.0f;
				CURSOR_POS[fontNum].x = -600.0f;
				CURSOR_POS[fontNum].y -= 50.0f;
			}

			break;

		//2Pの場合
		case 1:
			for (int fontNum = 0; fontNum < 6; fontNum++)
			{
				FONT_POS[fontNum].x = 320.0f;
				FONT_POS[fontNum].y -= 50.0f;
				CURSOR_POS[fontNum].x = 270.0f;
				CURSOR_POS[fontNum].y -= 50.0f;
			}

			break;
		}

		//カーソル文字を初期化
		m_cursorFont = NewGO<FontRender>();
		m_cursorFont->Init
		(
			L"",			//テキスト
			CURSOR_POS[0],	//位置
			nsItemShopSceneFont::FONT_COLOR,	//色
			nsItemShopSceneFont::FONT_ROT,		//傾き
			nsItemShopSceneFont::FONT_SCA,		//拡大率
			nsItemShopSceneFont::FONT_PIV		//基点
		);
		//文字の境界線表示
		m_cursorFont->SetShadowParam(true, 2.0f, Vector4::Black);
		swprintf_s(m_text, nsItemShopSceneFont::CURSOR_NAME);
		m_cursorFont->SetText(m_text);

		//買う、売る、出る文字を初期化
		for (int fontNum = enBuy; fontNum < enShoppingModeNum; fontNum++)
		{
			m_KUDFont[fontNum] = NewGO<FontRender>();
			m_KUDFont[fontNum]->Init
			(
				L"",			//テキスト
				FONT_POS[fontNum],	//位置
				nsItemShopSceneFont::FONT_COLOR,	//色
				nsItemShopSceneFont::FONT_ROT,		//傾き
				nsItemShopSceneFont::FONT_SCA,		//拡大率
				nsItemShopSceneFont::FONT_PIV		//基点
			);
			//文字の境界線表示
			m_KUDFont[fontNum]->SetShadowParam(true, 2.0f, Vector4::Black);
			swprintf_s(m_text, nsItemShopSceneFont::KUD_NAME[fontNum]);
			m_KUDFont[fontNum]->SetText(m_text);
		}

		//アイテム１、アイテム２、アイテム３、戻る文字を初期化
		for (int fontNum = enFirstItem; fontNum < enBuySelectStateNum; fontNum++)
		{
			m_Item1kara3BackFont[fontNum] = NewGO<FontRender>();
			m_Item1kara3BackFont[fontNum]->Init
			(
				L"",			//テキスト
				FONT_POS[fontNum],	//位置
				nsItemShopSceneFont::FONT_COLOR,	//色
				nsItemShopSceneFont::FONT_ROT,		//傾き
				nsItemShopSceneFont::FONT_SCA,		//拡大率
				nsItemShopSceneFont::FONT_PIV		//基点
			);
			//文字の境界線表示
			m_Item1kara3BackFont[fontNum]->SetShadowParam(true, 2.0f, Vector4::Black);
			swprintf_s(m_text, nsItemShopSceneFont::ITEM1KARA4_NAME[fontNum]);
			m_Item1kara3BackFont[fontNum]->SetText(m_text);
		}
		//アイテム１、アイテム２、アイテム３、戻る非表示
		for (int fontNum = enFirstItem; fontNum < enBuySelectStateNum; fontNum++)
		{
			m_Item1kara3BackFont[fontNum]->Deactivate();
		}

		//ステータスアップアイテム１～５と戻る文字を初期化
		for (int fontNum = enStatusUpItem1; fontNum < enStatusUpItemNum; fontNum++)
		{
			m_statusUpItem1kara5BackFont[fontNum] = NewGO<FontRender>();
			m_statusUpItem1kara5BackFont[fontNum]->Init
			(
				L"",			//テキスト
				FONT_POS[fontNum],	//位置2.
				nsItemShopSceneFont::FONT_COLOR,	//色
				nsItemShopSceneFont::FONT_ROT,		//傾き
				nsItemShopSceneFont::FONT_SCA,		//拡大率
				nsItemShopSceneFont::FONT_PIV		//基点
			);
			//文字の境界線表示
			m_statusUpItem1kara5BackFont[fontNum]->SetShadowParam(true, 2.0f, Vector4::Black);
			swprintf_s(m_text, nsItemShopSceneFont::STATUS_UP_ITEM[fontNum]);
			m_statusUpItem1kara5BackFont[fontNum]->SetText(m_text);
		}
		//アイテム１、アイテム２、アイテム３、戻る非表示
		for (int fontNum = enFirstItem; fontNum < enBuySelectStateNum; fontNum++)
		{
			m_statusUpItem1kara5BackFont[fontNum]->Deactivate();
		}

		//待機中文字を初期化
		m_waitFont = NewGO<FontRender>();
		m_waitFont->Init
		(
			L"",			//テキスト
			FONT_POS[0],	//位置
			nsItemShopSceneFont::FONT_COLOR,	//色
			nsItemShopSceneFont::FONT_ROT,		//傾き
			nsItemShopSceneFont::FONT_SCA,		//拡大率
			nsItemShopSceneFont::FONT_PIV		//基点
		);
		//文字の境界線表示
		m_waitFont->SetShadowParam(true, 2.0f, Vector4::Black);
		swprintf_s(m_text, nsItemShopSceneFont::WAIT_NAME);
		m_waitFont->SetText(m_text);
		//初めは非表示
		m_waitFont->Deactivate();

		for (int fontNum = 0; fontNum < 3; fontNum++)
		{
			//アイテムの値段文字を初期化
			m_itemValueFont[fontNum] = NewGO<FontRender>();
			m_itemValueFont[fontNum]->Init
			(
				L"",			//テキスト
				nsItemShopSceneFont::ITEM_VALUE_POS[fontNum],	//位置
				nsItemShopSceneFont::FONT_COLOR,	//色
				nsItemShopSceneFont::FONT_ROT,		//傾き
				0.5f,		//拡大率
				nsItemShopSceneFont::FONT_PIV		//基点
			);
			//文字の境界線表示
			m_itemValueFont[fontNum]->SetShadowParam(true, 2.0f, Vector4::Black);
			swprintf_s(m_text, nsItemShopSceneFont::ITEM_NEDAN_NAME[fontNum]);
			m_itemValueFont[fontNum]->SetText(m_text);
		}

		//アイテム説明文フォントを初期化
		m_itemSetumeiSentence = NewGO<FontRender>();
		m_itemSetumeiSentence->Init
		(
			L"",			//テキスト
			nsItemShopSceneFont::ITEM_SETUMEI_SENTENCE_POS[m_playerNum],	//位置
			nsItemShopSceneFont::FONT_COLOR,	//色
			nsItemShopSceneFont::FONT_ROT,		//傾き
			0.5f,		//拡大率
			nsItemShopSceneFont::FONT_PIV		//基点
		);
		//文字の境界線表示
		m_itemSetumeiSentence->SetShadowParam(true, 2.0f, Vector4::Black);
		m_itemSetumeiSentence->Deactivate();	//非表示
		//説明するアイテム名フォントを初期化
		m_setumeiItemName = NewGO<FontRender>();
		m_setumeiItemName->Init
		(
			L"",			//テキスト
			nsItemShopSceneFont::SETUMEI_ITEM_NAME_POS[m_playerNum],	//位置
			nsItemShopSceneFont::FONT_COLOR,	//色
			nsItemShopSceneFont::FONT_ROT,		//傾き
			0.8f,		//拡大率
			nsItemShopSceneFont::FONT_PIV		//基点
		);
		//文字の境界線表示
		m_setumeiItemName->SetShadowParam(true, 2.0f, Vector4::Black);
		m_setumeiItemName->Deactivate();	//非表示

		return true;
	}

	ItemShopSelectFont::~ItemShopSelectFont()
	{
		//カーソル文字削除
		DeleteGO(m_cursorFont);
		//買う、売る、出る、文字削除
		for (int fontNum = enBuy; fontNum < enShoppingModeNum; fontNum++)
		{
			DeleteGO(m_KUDFont[fontNum]);
		}
		//アイテム１、アイテム２、アイテム３、戻る
		for (int fontNum = enFirstItem; fontNum < enBuySelectStateNum; fontNum++)
		{
			DeleteGO(m_Item1kara3BackFont[fontNum]);
		}
		//ステータスアップアイテム1～5と戻る表示
		for (int fontNum = enStatusUpItem1; fontNum < enStatusUpItemNum; fontNum++)
		{
			DeleteGO(m_statusUpItem1kara5BackFont[fontNum]);
		}
		//待機中文字削除
		DeleteGO(m_waitFont);
		//アイテムの値段文字の削除
		for (int fontNum = 0; fontNum < 3; fontNum++)
		{
			DeleteGO(m_itemValueFont[fontNum]);
		}
		//アイテム説明文フォントを削除
		DeleteGO(m_itemSetumeiSentence);
		//説明するアイテム名フォントを削除
		DeleteGO(m_setumeiItemName);
	}

	void ItemShopSelectFont::Update()
	{
		switch (m_nowShoppingState)
		{
			//買う、売る、出る、の選択状態
		case enShoppingModeSelectState:
			//選択カーソル文字の位置を更新
			m_cursorFont->SetPosition(CURSOR_POS[m_nowShoppingMode]);
			//買う、売る、出る、文字表示
			for (int fontNum = 0; fontNum < enShoppingModeNum; fontNum++)
			{
				m_KUDFont[fontNum]->Activate();
			}
			//アイテム１、アイテム２、アイテム３、戻る非表示
			for (int fontNum = enFirstItem; fontNum < enBuySelectStateNum; fontNum++)
			{
				m_Item1kara3BackFont[fontNum]->Deactivate();
			}
			//ステータスアップアイテム1～5と戻る非表示
			for (int fontNum = enStatusUpItem1; fontNum < enStatusUpItemNum; fontNum++)
			{
				m_statusUpItem1kara5BackFont[fontNum]->Deactivate();
			}

			break;

			//購入状態
		case enBuyState:
			//選択カーソル文字の位置を更新
			m_cursorFont->SetPosition(CURSOR_POS[m_nowShoppingMode]);
			//買う、売る、出る、文字非表示
			for (int fontNum = enBuy; fontNum < enShoppingModeNum; fontNum++)
			{
				m_KUDFont[fontNum]->Deactivate();
			}
			//アイテム１、アイテム２、アイテム３、戻る表示
			for (int fontNum = enFirstItem; fontNum < enBuySelectStateNum; fontNum++)
			{
				if (m_isFontActive)
				{
					//フォントを表示
					m_Item1kara3BackFont[fontNum]->Activate();
					m_cursorFont->Activate();
					//説明文を非表示
					m_itemSetumeiSentence->Deactivate();
					//説明するアイテム名を非表示
					m_setumeiItemName->Deactivate();
				}
				else
				{
					//フォントを非表示
					m_Item1kara3BackFont[fontNum]->Deactivate();
					m_cursorFont->Deactivate();
					//説明文を表示
					swprintf_s(m_text, nsItemShopSceneFont::ITEM_SETUMEI_SENTENCE[m_nowShoppingMode]);
					m_itemSetumeiSentence->SetText(m_text);
					m_itemSetumeiSentence->Activate();
					//説明するアイテム名を表示
					swprintf_s(m_text, nsItemShopSceneFont::ITEM1KARA4_NAME[m_nowShoppingMode]);
					m_setumeiItemName->SetText(m_text);
					m_setumeiItemName->Activate();
				}
			}

			//もしXボタンが押されたなら、
			if (g_pad[m_playerNum]->IsTrigger(enButtonX) && m_nowShoppingMode != 3)
			{
				m_isFontActive = !m_isFontActive;
			}

			break;

			//売却状態
		case enSoldState:
			//選択カーソル文字の位置を更新
			m_cursorFont->SetPosition(CURSOR_POS[m_nowShoppingMode]);
			//買う、売る、出る、文字非表示
			for (int fontNum = enBuy; fontNum < enShoppingModeNum; fontNum++)
			{
				m_KUDFont[fontNum]->Deactivate();
			}
			//ステータスアップアイテム1～5と戻る表示
			for (int fontNum = enStatusUpItem1; fontNum < enStatusUpItemNum; fontNum++)
			{
				if (m_isFontActive)
				{
					//フォントを表示
					m_statusUpItem1kara5BackFont[fontNum]->Activate();
					m_cursorFont->Activate();
					//説明文を非表示
					m_itemSetumeiSentence->Deactivate();
					//説明するアイテム名を非表示
					m_setumeiItemName->Deactivate();
				}
				else
				{
					//フォントを非表示
					m_statusUpItem1kara5BackFont[fontNum]->Deactivate();
					m_cursorFont->Deactivate();
					//説明文を表示
					swprintf_s(m_text, nsItemShopSceneFont::STATUS_UP_ITEM_SETUMEI_SENTENCE[m_nowShoppingMode]);
					m_itemSetumeiSentence->SetText(m_text);
					m_itemSetumeiSentence->Activate();
					//説明するアイテム名を表示
					swprintf_s(m_text, nsItemShopSceneFont::STATUS_UP_ITEM[m_nowShoppingMode]);
					m_setumeiItemName->SetText(m_text);
					m_setumeiItemName->Activate();
				}
			}

			//もしXボタンが押されたなら、
			if (g_pad[m_playerNum]->IsTrigger(enButtonX) && m_nowShoppingMode != 5)
			{
				m_isFontActive = !m_isFontActive;
			}

			break;

			//店から出るスタンバイ状態
		case enEnterState:
			//買う、売る、出る、文字非表示
			for (int fontNum = enBuy; fontNum < enShoppingModeNum; fontNum++)
			{
				m_KUDFont[fontNum]->Deactivate();
			}
			//選択カーソルを非表示
			m_cursorFont->Deactivate();
			//待機中文字を表示
			m_waitFont->Activate();

			break;
		}
	}
};