/// <summary>
/// 最初のボスステージクラス
/// </summary>
#pragma once

namespace nsKabutoubatu
{
	/// <summary>
	/// 前方宣言
	/// </summary>
	class FirstBoss;		//最初のボス
	class GameScene;		//ゲームシーン
	class GameClearScene;	//ゲームクリアシーン
	class Fade;				//フェード

	class FirstBossStage : public IGameObject
	{
	private:
		SkinModelRender* m_stageModel = nullptr;		//モデル
		Fade* m_fade = nullptr;
		GameScene* m_gameScene = nullptr;
		GameClearScene* m_gameClearScene = nullptr;
		FirstBoss* m_firstBoss = nullptr;	//ボス敵
		SoundSource* m_firstBossBGM = nullptr;

		PhysicsStaticObject m_physicsStaticObject;		//静的物理オブジェクトクラス

		Vector3 m_pos;
		Quaternion m_rot;
		int m_playerNum = 0;

		/// <summary>
		/// 呼ばれて最初に実行される関数
		/// </summary>
		bool Start()override final;

		/// <summary>
		/// デストラクタ
		/// </summary>
		~FirstBossStage();

		/// <summary>
		/// 毎フレーム実行される更新処理関数
		/// </summary>
		void Update()override final;

	public:
		void SetPlayerNumber(const int playerNum) { m_playerNum = playerNum; };
	};
}