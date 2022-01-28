/// <summary>
/// アイテムショップシーンの選択クラス
/// </summary>
#pragma once

namespace nsKabutoubatu
{
	/// <summary>
	/// 前方宣言
	/// </summary>
	class ItemShopSelectFont;	//アイテムショップシーンの選択文字
	class Player;				//プレイヤー
	class PlayerStatus;			//プレイヤーのステータス
	class PlayerCamera;			//プレイヤーのカメラ
	class Weapon;				//武器
	class GameScreenUI;			//ゲーム画面UI

	class ItemShopSelect : public IGameObject
	{
	private:
		/// <summary>
		/// 列挙型
		/// </summary>

		//ショップのモード
		enum enShoppingMode
		{
			enBuy,		//購入
			enSold,		//売却
			enEnter		//出る
		};

		//ショップの状態
		enum enShoppingState
		{
			enShoppingModeSelectState,		//ショップモード選択状態
			enBuyState,						//購入状態
			enSoldState,					//売却状態
			enEnterState					//出る状態
		};

		//購入選択状態
		enum enBuySelectState
		{
			enFirstItem,		//商品１つ目
			enSecondItem,		//商品２つ目
			enThirdItem,		//商品３つ目
			enBackState			//戻る
		};

		//ステータス上昇アイテム
		enum enStatusUpItem
		{
			enStatusUpItem1,	//アイテム１
			enStatusUpItem2,	//アイテム２
			enStatusUpItem3,	//アイテム３
			enStatusUpItem4,	//アイテム４
			enStatusUpItem5,	//アイテム５
			enBack				//戻る
		};

		Player* m_player = nullptr;
		PlayerStatus* m_playerStatus = nullptr;
		SpriteRender* m_pushXSprite = nullptr;
		SpriteRender* m_ItemSetumeiSprite = nullptr;
		ItemShopSelectFont* m_itemShopSelectFont = nullptr;
		GameScreenUI* m_gameScreenUi = nullptr;
		SoundSource* m_selectSound = nullptr;
		SoundSource* m_decideSound = nullptr;
		SoundSource* m_cantDecideSound = nullptr;
		SoundSource* m_buySound = nullptr;
		Weapon* m_weapon = nullptr;

		int m_nowShoppingMode = enBuy;	//買う。売る。出る。の３つ
		int m_nowShoppingState = enShoppingModeSelectState;
		int m_playerNum = 0;

		/// <summary>
		/// 呼ばれて最初に実行される関数
		/// </summary>
		bool Start()override final;

		/// <summary>
		/// デストラクタ
		/// </summary>
		~ItemShopSelect();

		/// <summary>
		/// 毎フレーム実行される更新処理関数
		/// </summary>
		void Update()override final;

		//アイテムショップから出るメソッド
		void EnterFromShop();

		//選択メソッド
		void CursorMove(const int selectMin, const int selectMax);

		//次の状態に行くメソッド
		void NextState();

		//購入するときの選択
		void BuySelect();

		//売却するときの選択
		void SoldSelect();

	public:
		void SetPlayerNum(const int playerNum) { m_playerNum = playerNum; };

		/// <summary>
		/// 現在のショッピング状態を取得
		/// </summary>
		/// <returns>現在のショッピング状態</returns>
		int GetNowShoppingState()const { return m_nowShoppingState; };
	};
}