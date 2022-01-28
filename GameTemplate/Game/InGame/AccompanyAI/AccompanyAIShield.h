/// <summary>
/// ソロモード時の同行AI(以降「おとも」と呼ぶ)の盾クラス
/// </summary>
#pragma once

namespace nsKabutoubatu
{
	/// <summary>
	/// 前方宣言
	/// </summary>
	class AccompanyAI;	//同行AI

	class AccompanyAIShield	: public IGameObject
	{
	private:
		/// <summary>
		/// 列挙型
		/// </summary>

		//攻守の状態
		enum enAttackAndDefenseMode
		{
			enAttackMode,	//攻撃モード
			enDefenseMode,	//守備モード
		};

		AccompanyAI* m_accompaanyAi = nullptr;
		SkinModelRender* m_model = nullptr;		//モデル
		Bone* m_leftHandBone = nullptr;

		Vector3		 m_pos;		//位置
		Quaternion	 m_rot;		//回転
		Vector3		 m_sca;		//拡大率
		int m_leftHandBoneId = 0;

		/// <summary>
		/// 呼ばれて最初に実行される関数
		/// </summary>
		bool Start()override final;

		/// <summary>
		/// デストラクタ
		/// </summary>
		~AccompanyAIShield()override final;

		/// <summary>
		/// 毎フレーム実行される更新処理関数
		/// </summary>
		void Update()override final;
	};
}