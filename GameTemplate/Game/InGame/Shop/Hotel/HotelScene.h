/// <summary>
/// 宿屋シーンクラス
/// </summary>
#pragma once

namespace nsKabutoubatu
{
	/// <summary>
	/// 前方宣言
	/// </summary>
	class HotelSelect;		//宿屋シーンの選択
	class Player;			//プレイヤー
	class PlayerCamera;		//プレイヤーのカメラ
	class PlayerStatus;		//プレイヤーのステータス
	class AccompanyAI;		//同行AI
	class GameScene;		//ゲームシーン
	class GameLimitTime;	//ゲームの制限時間
	class Fade;				//フェード

	class HotelScene : public IGameObject
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

		//アニメーション
		enum enAnimation
		{
			enIdle,			//アイドル
			enGreething,	//挨拶
			enAnimationNum	//アニメーションの総数
		};


		PlayerCamera* m_playerCamera = nullptr;
		Player* m_player[enPlayerNum] = { nullptr };
		PlayerStatus* m_playerStatus[enPlayerNum] = { nullptr };
		AccompanyAI* m_accompanyAI = nullptr;
		GameLimitTime* m_gameLimitTime = nullptr;
		Fade* m_fade[2] = { nullptr };
		GameScene* m_gameScene = nullptr;
		SoundSource* m_shopBGM = nullptr;
		SpriteRender* m_hukidasi = nullptr;
		SkinModelRender* m_hotelInterior = nullptr;	//宿屋の内装モデル
		FontRender* m_salespersonTalk = nullptr;	//店員の会話文
		SkinModelRender* m_salesperson = nullptr;	//店員モデル
		HotelSelect* m_hotelSelect[enPlayerNum] = { nullptr };
		SpriteRender* m_coinSprite[2] = { nullptr };
		GamePad* m_playerGamePad[enPlayerNum] = { nullptr };

		PhysicsStaticObject m_physicsStaticObject;		//静的物理オブジェクトクラス
		AnimationClip m_animationClips[enAnimationNum];

		int m_playerNum = enPlayerNum;
		wchar_t m_talkText[256];	//店員の会話文
		Vector3 m_hotelPos;	//ステージ上の宿屋の位置
		Vector3 m_pos;		//宿屋の位置
		Quaternion m_rot;	//回転
		float m_coinPos = -355.0f;

		/// <summary>
		/// 呼ばれて最初に実行される関数
		/// </summary>
		bool Start()override final;

		/// <summary>
		/// デストラクタ
		/// </summary>
		~HotelScene();

		/// <summary>
		/// 毎フレーム実行される更新処理関数
		/// </summary>
		void Update()override final;

		//宿屋の内装モデルの初期化メソッド
		void HotelInteriorModelInit();

		//店員モデルの初期化メソッド
		void SalespersonModelInit();

		//宿屋から出るメソッド
		void EnterFromHotel();

		void CoinDeactivate(const int playerNum);

	public:
		//ステージ上の宿屋のポジション
		void SetHotelPosition(Vector3& hotelPosition) { m_hotelPos = hotelPosition; };

		void SetPlayerNumber(const int playerNum) { m_playerNum = playerNum; };

		void SetPlayerGamePad(GamePad& gamePad, const int playerNo) { m_playerGamePad[playerNo] = &gamePad; };
	};
}