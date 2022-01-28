/// <summary>
/// アイテムショップシーンクラス
/// </summary>
#pragma once

namespace nsKabutoubatu
{
	/// <summary>
	/// 前方宣言
	/// </summary>
	class ItemShopSelect;	//アイテムショップシーンの選択
	class Player;			//プレイヤー
	class PlayerCamera;		//プレイヤーのカメラ
	class AccompanyAI;		//同行AI
	class GameScene;		//ゲームシーン
	class GameLimitTime;	//ゲームの制限時間
	class Fade;				//フェード

	class ItemShopScene : public IGameObject
	{
	private:
		/// <summary>
		/// 列挙型
		/// </summary>

		//プレイヤー番号
		enum enPlayerNum
		{
			enPlayer1,		//1P
			enPlayer2,		//2P
			enPlayerNum		//プレイヤーの総数
		};

		SpriteRender* m_coinSprite[3] = { nullptr };
		SkinModelRender* m_itemShopInterior = nullptr;	//アイテムショップの内装モデル
		SkinModelRender* m_shopItem[3] = { nullptr };	//アイテムショップの商品モデル
		SpriteRender* m_hukidasi = nullptr;
		AccompanyAI* m_accompanyAI = nullptr;
		PlayerCamera* m_playerCamera = nullptr;
		Player* m_player[enPlayerNum] = { nullptr };
		GameScene* m_gameScene = nullptr;
		GameLimitTime* m_gameLimitTime = nullptr;
		SoundSource* m_shopBGM = nullptr;
		Fade* m_fade[2] = { nullptr };
		SkinModelRender* m_salesperson = nullptr;	//店員モデル
		FontRender* m_salespersonTalk = nullptr;	//店員の会話文
		ItemShopSelect* m_ItemShopSelect[enPlayerNum] = { nullptr };

		PhysicsStaticObject m_physicsStaticObject;		//静的物理オブジェクトクラス
		AnimationClip m_animationClips[1];

		int m_playerNum = enPlayerNum;
		float m_coinSpritePos = 0.0f;
		Vector3 m_shopPos;	//ステージ上のショップの位置
		wchar_t m_talkText[256];	//店員の会話文
		Vector3 m_pos;		//ショップの位置
		Vector3 m_itemPos;		//アイテムの位置
		Quaternion m_rot;	//回転

		/// <summary>
		/// 呼ばれて最初に実行される関数
		/// </summary>
		bool Start()override final;

		/// <summary>
		/// デストラクタ
		/// </summary>
		~ItemShopScene();

		/// <summary>
		/// 毎フレーム実行される更新処理関数
		/// </summary>
		void Update()override final;

		//アイテムショップの内装モデルの初期化メソッド
		void ItemShopInteriorModelInit();

		//店員モデルの初期化メソッド
		void SalespersonModelInit();

		//アイテムショップから出るメソッド
		void EnterFromShop();

	public:
		//ステージ上のショップのポジション
		void SetItemShopPosition(Vector3& shopPosition) { m_shopPos = shopPosition; };

		void SetPlayerNumber(const int playerNum) { m_playerNum = playerNum; };
	};
}