/// <summary>
/// ゲームオーバーシーンクラス
/// </summary>
#pragma once

namespace nsKabutoubatu
{
	/// <summary>
	/// 前方宣言
	/// </summary>
	class GameUpdateStop;	//ゲーム更新ストップ
	class GameScene;		//ゲームシーン

	class GameOverScene : public IGameObject
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

		GameUpdateStop* m_gameUpdateStop = nullptr;
		GameScene* m_gameScene = nullptr;
		SoundSource* m_gameOverGingle = nullptr;
		SoundSource* m_decideSound = nullptr;
		SpriteRender* m_gameOverSprite = nullptr;
		SpriteRender* m_aPushSprite = nullptr;

		int m_playerNum = enPlayerNum;
		float m_alphaValue = 0.0f;

		/// <summary>
		/// 呼ばれて最初に実行される関数
		/// </summary>
		bool Start()override final;

		/// <summary>
		/// デストラクタ
		/// </summary>
		~GameOverScene();

		/// <summary>
		/// 毎フレーム実行される更新処理関数
		/// (通常の更新を止めてUIだけ更新させたい時に使う更新処理関数)
		/// </summary>
		void UiUpdate()override final;

	public:
		void SetPlayerNumber(const int playerNum) { m_playerNum = playerNum; };
	};
}