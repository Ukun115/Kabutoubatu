/// <summary>
/// ゲーム画面UIクラス
/// </summary>
#pragma once

namespace nsKabutoubatu
{
	/// <summary>
	/// 前方宣言
	/// </summary>
	class PlayerStatus;	//プレイヤーのステータス

	class GameScreenUI : public IGameObject
	{
	private:
		/// <summary>
		/// 列挙型
		/// </summary>

		//UIの名前。下に行くほど表示レイヤーの優先度が高くなる
		enum enUiName
		{
			enFlame,	//フレーム
			enBack,		//フレーム下の背景
			enIcon,		//アイコン
			enUiNum		//固定UIの総数
		};

		//プレイヤー番号
		enum enPlayerNum
		{
			enPlayer1,		//1P
			enPlayer2,		//2P
			enPlayerNum		//プレイヤーの総数
		};

		//武器
		enum enWeapon
		{
			enWoodStick,	//木の棒
			enSword,		//剣
			enLargeSword,	//大剣
			enBow,			//弓
			enWeaponNum		//武器の総数
		};

		//アイテム
		enum enItem
		{
			enItem1,	//アイテム１
			enItem2,	//アイテム２
			enItem3,	//アイテム３
			enItem4,	//アイテム４
			enItem5,	//アイテム５
			enItemNum	//アイテムの総数
		};

		//アイテムUI
		SpriteRender* m_itemUI[enPlayerNum][enItemNum] = { nullptr };
		//固定UI
		SpriteRender* m_fixedUI[enUiNum] = { nullptr };
		//ヒットポイントUI
		SpriteRender* m_HitPointUI[enPlayerNum][10] = { nullptr };
		PlayerStatus* m_playerStatus[enPlayerNum];
		SpriteRender* m_haveWeaponUI[enPlayerNum][enWeaponNum] = { nullptr };
		SpriteRender* m_playerIconUI[enPlayerNum] = { nullptr };
		FontRender* m_haveItemNum[enPlayerNum][enItemNum] = { nullptr };

		int m_itemNum[enPlayerNum][enItemNum] = {0};
		int m_playerNum = enPlayerNum;
		wchar_t m_itemText[256];
		int m_blinkingTimer[2] = { 0 };
		Vector2 m_itemFontPos;
		//ヒットポイント画像のファイル名
		char m_hitPointFileName[256];
		char m_weaponFileName[256];
		char m_itemFileName[256];

		/// <summary>
		/// 呼ばれて最初に実行される関数
		/// </summary>
		bool Start() override final;

		/// <summary>
		/// デストラクタ
		/// </summary>
		~GameScreenUI();

		/// <summary>
		/// 毎フレーム実行される更新処理関数
		/// </summary>
		void Update() override final;

		//ヒットポイントUIの初期化メソッド
		void HitPointUiInit(const int playerNum);

		void HaveWeaponUiInit(const int playerNum);

		void ItemUiInit(const int playerNum);

		//プレイヤーのヒットポイントを表示状態にするか非表示状態にするかを更新する処理メソッド
		void HitPointUiUpdate();

		//ヒットポイントが少ないときハートが点滅する処理
		void BlinkingHeart();

		//所持武器の画像表示更新処理
		void HaveWeapon();

		//アイテム所持数文字の初期化メソッド
		void ItemFontInit(const int playerNum);

		//能力アップアイテムの所持数文字の数更新
		void ItemNumUpdate();

	public:
		void SetPlayerNumber(const int playerNum) { m_playerNum = playerNum; };

		void AddItemNum(const int playerNum, const int itemNum) { m_itemNum[playerNum][itemNum]++; };

		void ReduceItemNum(const int playerNum, const int itemNum) { m_itemNum[playerNum][itemNum]--; };

		/// <summary>
		/// ステータス上昇アイテムを取得
		/// </summary>
		/// <returns>ステータス上昇アイテム</returns>
		int GetStatusUpItemNum(const int playerNum,const int statusUpItemNum)const { return m_itemNum[playerNum][statusUpItemNum]; };

		void ReduceStatusUpItemNum(const int playerNum, const int statusUpItemNum) { m_itemNum[playerNum][statusUpItemNum]--; };
	};
}