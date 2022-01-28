/// <summary>
/// 宿屋シーンの選択文字クラス
/// </summary>
#include "stdafx.h"
#include "HotelSelectFont.h"

namespace nsKabutoubatu
{
	namespace nsHotelSelectFont
	{
		const Vector4   FONT_COLOR = { 1.0f,1.0f,1.0f,1.0f };	//制限時間フォントの色
		const float     FONT_ROT = 0.0f;			//フォントの傾き
		const Vector2   FONT_PIV = { 1.0f,1.0f };	//フォントの基点
		const float     FONT_SCA = 1.0f;			//フォントの拡大率

		const wchar_t* CURSOR_NAME = L"o";
		const wchar_t* KUD_NAME[2] = { L"泊まる :  1000",L"出る" };
		const wchar_t* WAIT_NAME = L"待機中...";
	}

	bool HotelSelectFont::Start()
	{
		switch (m_playerNum)
		{
			//1Pの場合
		case 0:
			for (int fontNum = 0; fontNum < 2; fontNum++)
			{
				FONT_POS[fontNum].x = -550.0f;
				FONT_POS[fontNum].y -= 50.0f;
				CURSOR_POS[fontNum].x = -600.0f;
				CURSOR_POS[fontNum].y -= 50.0f;
			}

			break;

			//2Pの場合
		case 1:
			for (int fontNum = 0; fontNum < 2; fontNum++)
			{
				FONT_POS[fontNum].x = 280.0f;
				FONT_POS[fontNum].y -= 50.0f;
				CURSOR_POS[fontNum].x = 230.0f;
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
			nsHotelSelectFont::FONT_COLOR,	//色
			nsHotelSelectFont::FONT_ROT,		//傾き
			nsHotelSelectFont::FONT_SCA,		//拡大率
			nsHotelSelectFont::FONT_PIV		//基点
		);
		//文字の境界線表示
		m_cursorFont->SetShadowParam(true, 2.0f, Vector4::Black);
		swprintf_s(m_text, nsHotelSelectFont::CURSOR_NAME);
		m_cursorFont->SetText(m_text);

		//泊まる泊まらない文字を初期化
		for (int fontNum = enStay; fontNum < enHotelModeNum; fontNum++)
		{
			m_stayOrEnter[fontNum] = NewGO<FontRender>();
			m_stayOrEnter[fontNum]->Init
			(
				L"",			//テキスト
				FONT_POS[fontNum],	//位置
				nsHotelSelectFont::FONT_COLOR,	//色
				nsHotelSelectFont::FONT_ROT,		//傾き
				nsHotelSelectFont::FONT_SCA,		//拡大率
				nsHotelSelectFont::FONT_PIV		//基点
			);
			//文字の境界線表示
			m_stayOrEnter[fontNum]->SetShadowParam(true, 2.0f, Vector4::Black);
			swprintf_s(m_text, nsHotelSelectFont::KUD_NAME[fontNum]);
			m_stayOrEnter[fontNum]->SetText(m_text);
		}

		//待機中文字を初期化
		m_waitFont = NewGO<FontRender>();
		m_waitFont->Init
		(
			L"",			//テキスト
			FONT_POS[0],	//位置
			nsHotelSelectFont::FONT_COLOR,	//色
			nsHotelSelectFont::FONT_ROT,		//傾き
			nsHotelSelectFont::FONT_SCA,		//拡大率
			nsHotelSelectFont::FONT_PIV		//基点
		);
		//文字の境界線表示
		m_waitFont->SetShadowParam(true, 2.0f, Vector4::Black);
		swprintf_s(m_text, nsHotelSelectFont::WAIT_NAME);
		m_waitFont->SetText(m_text);
		//初めは非表示
		m_waitFont->Deactivate();

		return true;
	}

	HotelSelectFont::~HotelSelectFont()
	{
		//カーソル文字削除
		DeleteGO(m_cursorFont);
		//泊まる泊まらない文字削除
		for (int fontNum = enStay; fontNum < enHotelModeNum; fontNum++)
		{
			DeleteGO(m_stayOrEnter[fontNum]);
		}
		//待機中文字削除
		DeleteGO(m_waitFont);
	}

	void HotelSelectFont::Update()
	{
		switch (m_nowHotelState)
		{
			//泊まる泊まらないの選択状態
		case enSelectState:
			//選択カーソル文字の位置を更新
			m_cursorFont->SetPosition(CURSOR_POS[m_nowHotelMode]);
			//泊まる泊まらない文字表示
			for (int fontNum = enStay; fontNum < enHotelModeNum; fontNum++)
			{
				m_stayOrEnter[fontNum]->Activate();
			}

			break;

			//泊まる状態
		case enStayState:
			//泊まる泊まらない文字非表示
			for (int fontNum = enStay; fontNum < enHotelModeNum; fontNum++)
			{
				m_stayOrEnter[fontNum]->Deactivate();
			}
			//選択カーソルを非表示
			m_cursorFont->Deactivate();
			//待機中文字を表示
			m_waitFont->Activate();

			break;

			//店から出るスタンバイ状態
		case enEnterState:
			//泊まる泊まらない文字非表示
			for (int fontNum = enStay; fontNum < enHotelModeNum; fontNum++)
			{
				m_stayOrEnter[fontNum]->Deactivate();
			}
			//選択カーソルを非表示
			m_cursorFont->Deactivate();
			//待機中文字を表示
			m_waitFont->Activate();

			break;
		}
	}
};