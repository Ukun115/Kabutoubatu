/// <summary>
/// ステージ上の建物クラス
/// </summary>
#pragma once

namespace nsKabutoubatu
{
	/// <summary>
	/// 前方宣言
	/// </summary>
	class Player;			//プレイヤー
	class PlayerRotation;	//プレイヤーの回転
	class PlayerAnimation;	//プレイヤーのアニメーション
	class PlayerCamera;		//プレイヤーカメラ
	class AccompanyAI;		//同行AI
	class HotelScene;		//宿屋シーン
	class ItemShopScene;	//アイテムショップシーン
	class FirstBossStage;	//最初のボス
	class Fade;				//フェード
	class OnlineUpdateSpeed;

	class StageBuilding : public IGameObject
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

		//プレイヤーの状態
		enum enPlayerNowState
		{
			enNormal,			//通常
			enSufferAnAttack,	//攻撃を受けた状態
			enGhost,			//ゴースト状態
			enDontMove			//何もしない状態
		};

		//ドアの状態
		enum enDoorState
		{
			enClose,	//開ける
			enOpen		//閉じる
		};

		//ショップのタイプ
		enum enShopType
		{
			enHotel,			//ホテル
			enItemShop,			//アイテムショップ
			enFirstBossHouse	//最初のボスの建物
		};

		SkinModelRender* m_building = nullptr;		//建物モデル
		SkinModelRender* m_door = nullptr;	//ドアモデル
		SkinModelRender* m_darknessEntrance = nullptr;	//暗闇入口モデル
		Fade* m_fade = nullptr;
		SpriteRender* m_hukidasiSprite[enPlayerNum] = { nullptr };
		Player* m_player[enPlayerNum] = { nullptr };
		PlayerRotation* m_playerRotation[enPlayerNum] = { nullptr };
		PlayerAnimation* m_playerAnimation[enPlayerNum] = { nullptr };
		PlayerCamera* m_playerCamera = nullptr;
		AccompanyAI* m_accompaniAI = nullptr;
		SoundSource* m_buildingSound = nullptr;
		SoundSource* m_doorOpenSound = nullptr;
		SoundSource* m_doorCloseSound = nullptr;
		//宿屋シーン
		HotelScene* m_hotelScene = nullptr;
		//アイテムショップシーン
		ItemShopScene* m_itemShopScene = nullptr;
		//最初のボスステージ
		FirstBossStage* m_firstBossStage = nullptr;
		OnlineUpdateSpeed* m_onlineUpdateSpeed = nullptr;
		GamePad* m_gamePad[enPlayerNum] = { nullptr };

		PhysicsStaticObject m_physicsStaticObject[2];		//静的物理オブジェクトクラス

		int m_playerNum = enPlayerNum;
		Vector2 m_playerScreenPos;
		//プレイヤーから建物にのびるベクトル
		Vector3 m_newPlayerMoveDirection;
		Vector3 m_pos;		//位置
		Vector3 m_doorPos;		//ドアの位置
		Quaternion m_rot;	//回転
		//シーンに入るまでの遅延タイマー
		int m_sceneChangeDelayTimer = 0;
		int m_doorSlideTimer = 0;
		int m_gyaku = 1;
		int m_doorState = enOpen;
		float m_rotationAngle = 0.0f;
		float m_buildingEnterLength = 0.0f;
		float m_canDoorTatchLength = 0.0f;
		//ショップタイプ
		int m_shopType = enHotel;
		/// <summary>
		/// フラグ
		/// </summary>
		//ドアに触れているかどうか
		bool m_doorTatchFlg[enPlayerNum] = { false };

		/// <summary>
		/// 呼ばれて最初に実行される関数
		/// </summary>
		bool Start()override final;

		/// <summary>
		/// デストラクタ
		/// </summary>
		~StageBuilding();

		/// <summary>
		/// 毎フレーム実行される更新処理関数
		/// </summary>
		void Update()override final;

		//モデルの初期化メソッド
		void BuildingModelInit();

		//ドアモデルの初期化メソッド
		void DoorModelInit();

		//暗闇入口モデルの初期化メソッド
		void DarknessEntranceInit();

		//建物に入ったかどうかの判定メソッド
		void JudgmentEntered();

		//シーン遷移メソッド
		void SceneTransition();

		//吹き出しを表示させるメソッド
		void HukidasiActivate(const int playerNum);

	public:
		//位置セットメソッド
		void SetPosition(const Vector3& pos) { m_pos = pos; };

		//ショップのタイプをセットするメソッド
		void SetShopType(const int shopType) { m_shopType = shopType; };

		void SetPlayerNumber(const int playerNum) { m_playerNum = playerNum; };

		/// <summary>
		/// ドアに触れているかどうかを取得
		/// </summary>
		/// <returns>ドアに触れているかどうか</returns>
		bool GetDoorTatchFlg(const int playerNum){ return m_doorTatchFlg[playerNum]; };

		void SetPlayerGamePad(GamePad& gamePad,const int playerNo) { m_gamePad[playerNo] = &gamePad; };
	};
}