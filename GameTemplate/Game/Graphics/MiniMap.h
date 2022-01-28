/// <summary>
/// ミニマップクラス
/// </summary>
#pragma once

namespace nsKabutoubatu
{
	class MiniMap : public IGameObject
	{
	private:
		RenderTarget m_miniMap;	//ミニマップのレンダリングターゲット
		Camera m_miniMapCamera;		//ミニマップカメラ
		SpriteInitData m_miniMapSpriteInitData;
		Sprite m_miniMapSprite;
		SpriteRender* m_miniMapBack = nullptr;

		Vector3 m_cameraPos;			//視点
		Vector3 m_cameraTargetPos;		//注視点

		float clearColor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };

		/// <summary>
		/// 呼ばれて最初に実行される関数
		/// </summary>
		bool Start()override final;

		//描画関数
		void SpriteDataRender(RenderContext& renderContext);

	public:
		//ミニマップの初期化メソッド
		void Init();

		//ミニマップ描画メソッド
		void DrawMiniMap(RenderContext& rc);

		/// <summary>
		/// ミニマップのテクスチャを取得
		/// </summary>
		/// <returns>ミニマップのテクスチャ</returns>
		Texture& GetMiniMapTexture()
		{
			return m_miniMap.GetRenderTargetTexture();
		}

		/// <summary>
		/// ミニマップカメラのアドレスを取得
		/// </summary>
		/// <returns>ミニマップカメラのアドレス</returns>
		Camera* GetMiniMapCamera() { return &m_miniMapCamera; }
	};
}