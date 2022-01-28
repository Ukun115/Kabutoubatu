/// <summary>
/// ステージクラス
/// </summary>
#pragma once

namespace nsKabutoubatu
{
	/// <summary>
	/// 前方宣言
	/// </summary>
	class StageBuilding;	//ステージ上の建物
	class Player;			//プレイヤー
	class Mole;				//モグラ(ザコ敵)
	class Slime;			//スライム(ザコ敵)
	class NeedleSlime;		//トゲスライム(ザコ敵)

	class Stage : public IGameObject
	{
	private:
		SkinModelRender* m_floorModel = nullptr;		//ステージの足場モデル
		SkinModelRender* m_hitStageAcceModel = nullptr;		//ステージ上の当たり判定のある装飾モデル
		SkinModelRender* m_dontHitStageAcceModel = nullptr;	//当たり判定のないステージの装飾モデル
		SkinModelRender* m_seaModel = nullptr;	//海モデル
		Player* m_player[2] = { nullptr };
		StageBuilding* m_shopBuiding[4] = { nullptr };
		Mole* m_mole[4] = { nullptr };
		Slime* m_slime[4] = { nullptr };
		NeedleSlime* m_needleSlime[4] = { nullptr };

		PhysicsStaticObject m_physicsStaticObject[2];		//静的物理オブジェクトクラス

		Vector3 m_weakEnemyRandomRespownPos;
		int m_playerNum = 0;

		/// <summary>
		/// 呼ばれて最初に実行される関数
		/// </summary>
		bool Start()override final;

		/// <summary>
		/// デストラクタ
		/// </summary>
		~Stage();

		/// <summary>
		/// 毎フレーム実行される更新処理関数
		/// </summary>
		void Update()override final;

		//ザコ敵のリスポーン処理メソッド
		void WeakEnemyResporn();

		void PlayerAttackOk();

		Vector3 WeakEnemyRandomRespownPos(const int x1, const int z1, const int x2, const int z2);

	public:
		void SetPlayerNumber(const int playerNum) { m_playerNum = playerNum; };
	};
}