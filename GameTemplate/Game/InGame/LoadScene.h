/// <summary>
/// ロード画面クラス
/// </summary>
#pragma once

namespace nsKabutoubatu
{
	class LoadScene : public IGameObject
	{
	private:
		/// <summary>
		/// 列挙型
		/// </summary>

		//点画像
		enum enPoint
		{
			enPointOne,		//点１つ目
			enPointTwo,		//点２つ目
			enPointThree,	//点３つ目
			enPointNum		//点の総数
		};

		SpriteRender* m_backScreen = nullptr;
		SpriteRender* m_point[enPointNum] = { nullptr };

		int m_pointActiveTimer = 0;

		/// <summary>
		/// 呼ばれて最初に実行される関数
		/// </summary>
		bool Start()override final;

		/// <summary>
		/// デストラクタ
		/// </summary>
		~LoadScene();

		/// <summary>
		/// 毎フレーム実行される更新処理関数
		/// </summary>
		void Update()override final;
	};
}