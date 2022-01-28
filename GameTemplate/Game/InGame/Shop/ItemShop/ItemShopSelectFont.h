/// <summary>
/// アイテムショップシーンの選択文字クラス
/// </summary>
#pragma once

namespace nsKabutoubatu
{
	class ItemShopSelectFont : public IGameObject
	{
	private:
		/// <summary>
		/// 列挙型
		/// </summary>

		//ショップのモード
		enum enShoppingMode
		{
			enBuy,				//買う
			enSold,				//売る
			enEnter,			//出る
			enShoppingModeNum	//ショップのモード総数
		};

		//ショップの状態
		enum enShoppingState
		{
			enShoppingModeSelectState,	//買う売る出るを選んでいる状態
			enBuyState,					//買う状態
			enSoldState,				//売る状態
			enEnterState				//出る状態
		};

		//購入選択状態
		enum enBuySelectState
		{
			enFirstItem,			//1つ目のアイテム
			enSecondItem,			//２つ目のアイテム
			enThirdItem,			//３つ目のアイテム
			enBackState,			//戻る
			enBuySelectStateNum		//購入選択状態の総数
		};

		//ステータス上昇アイテム
		enum enStatusUpItemSelect
		{
			enStatusUpItem1,	//ステータス上昇アイテム１つ目
			enStatusUpItem2,	//ステータス上昇アイテム２つ目
			enStatusUpItem3,	//ステータス上昇アイテム３つ目
			enStatusUpItem4,	//ステータス上昇アイテム４つ目
			enStatusUpItem5,	//ステータス上昇アイテム５つ目
			enBack,				//戻る
			enStatusUpItemNum	//ステータス上昇アイテム選択総数
		};

		//買う売る出る文字
		FontRender* m_KUDFont[enShoppingModeNum] = { nullptr };
		//アイテム１～３と戻る文字
		FontRender* m_Item1kara3BackFont[enBuySelectStateNum] = { nullptr };
		FontRender* m_statusUpItem1kara5BackFont[enStatusUpItemNum] = { nullptr };
		//アイテムの値段
		FontRender* m_itemValueFont[3] = { nullptr };
		//選択カーソル文字
		FontRender* m_cursorFont = nullptr;
		//待機中文字
		FontRender* m_waitFont = nullptr;
		//アイテム説明文
		FontRender* m_itemSetumeiSentence = nullptr;
		//説明するアイテム名
		FontRender* m_setumeiItemName= nullptr;

		//フォントの文字
		wchar_t m_text[256];
		//プレイヤー番号
		int m_playerNum = 0;
		//現在のショッピング状態
		int m_nowShoppingState = enShoppingModeSelectState;
		//現在のショッピングモード
		int m_nowShoppingMode = 0;
		bool m_isFontActive = true;
		Vector2 FONT_POS[6] = { {-550.0f,200.0f},{-550.0f,150.0f},{-550.0f,100.0f},{-550.0f,50.0f},{-500.0f,0.0f},{-500.0f,-50.0f} };
		Vector2 CURSOR_POS[6] = { {-600.0f,200.0f},{-600.0f,150.0f},{-600.0f,100.0f},{-600.0f,50.0f},{-600.0f,0.0f},{-600.0f,-50.0f} };

		/// <summary>
		/// 呼ばれて最初に実行される関数
		/// </summary>
		bool Start()override final;

		/// <summary>
		/// デストラクタ
		/// </summary>
		~ItemShopSelectFont();

		/// <summary>
		/// 毎フレーム実行される更新処理関数
		/// </summary>
		void Update()override final;

	public:
		//現在のショッピング状態セットメソッド
		void SetNowShoppingState(const int nowShoppingState) { m_nowShoppingState = nowShoppingState; };

		//現在のショッピングモードセットメソッド
		void SetNowShoppingMode(const int nowShoppingMode) { m_nowShoppingMode = nowShoppingMode; };

		//プレイヤー番号セットメソッド
		void SetPlayerNum(const int playerNum) { m_playerNum = playerNum; };
	};
}