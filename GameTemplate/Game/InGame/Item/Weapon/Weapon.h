/// <summary>
/// 武器クラス
/// </summary>
#pragma once

namespace nsKabutoubatu
{
	/// <summary>
	/// 前方宣言
	/// </summary>
	class Player;		//プレイヤー
	class PlayerStatus;	//プレイヤーのステータス

	class Weapon : public IGameObject
	{
	private:
		/// <summary>
		/// 列挙型
		/// </summary>

		//武器
		enum enWeapon
		{
			enWoodStick,	//木の棒
			enSword,		//普通の剣
			enLargeSword,	//大剣
			enBow,			//弓
			enWeaponNum		//武器の総数
		};

		Player* m_player = nullptr;		//プレイヤー検索するためのインスタンス
		PlayerStatus* m_playerStatus = nullptr;
		SkinModelRender* m_model = nullptr;		//モデル
		Bone* m_rightHandBone = nullptr;
		Bone* m_leftHandBone = nullptr;

		Vector3 m_pos;	//位置
		Quaternion m_rot;		//回転
		Vector3 m_sca;	//拡大率
		int m_playerNum = 0;	//プレイヤー番号
		const char* m_weaponModelName;
		int m_attackDamage = 0;
		int m_rightHandBoneId = 0;
		int m_leftHandBoneId = 0;
		int m_weaponNum = 0;
		int m_attackJudgeTimer = 0;

		/// <summary>
		/// 呼ばれて最初に実行される関数
		/// </summary>
		bool Start()override final;

		/// <summary>
		/// デストラクタ
		/// </summary>
		~Weapon();

		/// <summary>
		/// 毎フレーム実行される更新処理関数
		/// </summary>
		void Update()override final;

		/// <summary>
		/// 呼ばれて最初に実行される関数
		/// (通常の更新を止めてUIだけ更新させたい時に使う更新処理関数)
		/// </summary>
		void UiUpdate()override final;

	public:
		//プレイヤー番号をセットするメソッド
		void SetPlayerNum(const int plaNum) { m_playerNum = plaNum; };

		//武器をセレクト
		void SetWeaponNum(const int weaponNum) { m_weaponNum = weaponNum; };

		void Init(const int weaponNum);

		/// <summary>
		/// 武器ごとのヒットタイミングを取得
		/// </summary>
		/// <returns>武器ごとのヒットタイミング</returns>
		int GetAttackJudgeTimer()const { return m_attackJudgeTimer; };
	};
}