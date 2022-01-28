/// <summary>
/// 宿屋シーンの選択文字クラス
/// </summary>
#pragma once

namespace nsKabutoubatu
{
	class HotelSelectFont : public IGameObject
	{
	private:
		/// <summary>
		/// 列挙型
		/// </summary>

		//ホテルのモード
		enum enHotelMode
		{
			enStay,				//宿泊
			enEnter,			//出る
			enHotelModeNum		//ホテルのモードの総数
		};

		//ホテルの状態
		enum enHotelState
		{
			enSelectState,	//選択状態
			enStayState,	//宿泊状態
			enEnterState	//出る状態
		};

		//泊まる泊まらない文字
		FontRender* m_stayOrEnter[enHotelModeNum] = { nullptr };
		//選択カーソル文字
		FontRender* m_cursorFont = nullptr;
		//待機中文字
		FontRender* m_waitFont = nullptr;

		//フォントの文字
		wchar_t m_text[256];
		//プレイヤー番号
		int m_playerNum = 0;
		//現在の状態
		int m_nowHotelState = enSelectState;
		//現在のモード
		int m_nowHotelMode = 0;
		Vector2 FONT_POS[2] = { {-550.0f,200.0f},{-550.0f,150.0f} };
		Vector2 CURSOR_POS[2] = { {-600.0f,200.0f},{-600.0f,150.0f} };

		/// <summary>
		/// 呼ばれて最初に実行される関数
		/// </summary>
		bool Start()override final;

		/// <summary>
		/// デストラクタ
		/// </summary>
		~HotelSelectFont();

		/// <summary>
		/// 毎フレーム実行される更新処理関数
		/// </summary>
		void Update()override final;

	public:
		//現在のショッピング状態セットメソッド
		void SetNowHotelState(const int nowHotelState) { m_nowHotelState = nowHotelState; };

		//現在のショッピングモードセットメソッド
		void SetNowHotelMode(const int nowHotelMode) { m_nowHotelMode = nowHotelMode; };

		//プレイヤー番号セットメソッド
		void SetPlayerNum(const int playerNum) { m_playerNum = playerNum; };
	};
}