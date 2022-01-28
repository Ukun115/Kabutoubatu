/// <summary>
/// タイトルシーンの画像クラス
/// </summary>
#pragma once

namespace nsKabutoubatu
{
	/// <summary>
	/// 前方宣言
	/// </summary>
	class TitleScene;	//タイトルシーン

	class TitleSceneSprite : public IGameObject
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

		TitleScene* m_titleScene = nullptr;
		SpriteRender* m_backSprite = nullptr;
		//カーソル画像
		SpriteRender* m_cursorSprite = nullptr;
		SpriteRender* m_firstSelectSprite = nullptr;
		SpriteRender* m_SecondSelectSprite = nullptr;
		SpriteRender* m_thirdSelectSprite = nullptr;
		SpriteRender* m_fourthSelectSprite = nullptr;
		SpriteRender* m_waitJoinPlayerSprite = nullptr;
		SpriteRender* m_searchJoinRoomSprite = nullptr;
		SpriteRender* m_waitJoinPlayerPoint[enPointNum] = { nullptr };
		SpriteRender* m_searchJoinRoomPoint[enPointNum] = { nullptr };

		int m_pointActiveTimer = 0;

		/// <summary>
		/// 呼ばれて最初に実行される関数
		/// </summary>
		bool Start()override final;

		/// <summary>
		/// デストラクタ
		/// </summary>
		~TitleSceneSprite();

		/// <summary>
		/// 毎フレーム実行される更新処理関数
		/// </summary>
		void Update()override final;
	};
}