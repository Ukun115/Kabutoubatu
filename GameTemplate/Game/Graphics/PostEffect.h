/// <summary>
/// ポストエフェクトクラス
/// </summary>
#pragma once

namespace nsKabutoubatu
{
	/// <summary>
	/// 前方宣言
	/// </summary>
	class DepthShadow;	//デプスシャドウ
	class Bloom;		//ブルーム
	class DoF;			//被写界深度
	class Silhouette;	//シルエット描画
	class MiniMap;		//ミニマップ

	class PostEffect : public IGameObject
	{
	private:
		DepthShadow* m_depthShadow;		//デプスシャドウ
		Bloom* m_bloom;					//川瀬式ブルーム
		DoF* m_dof;						//被写界深度
		Silhouette* m_silhouette;		//シルエット
		MiniMap* m_miniMap;				//ミニマップ

	public:
		//ポストエフェクトの初期化メソッド
		void Init(RenderTarget& mainRenderTarget , RenderTarget& zprepassRenderTarget);

		//シャドウを描画
		void DrawShadow(RenderContext& rc);

		//シルエットを描画
		void DrawSilhouette(RenderContext& rc);

		//ミニマップを描画するメソッド
		void DrawMiniMap(RenderContext& rc);

		//レンダーターゲットにメインレンダーターゲットをセットするメソッド
		void SettingRenderTarget(RenderContext& rc,RenderTarget& mainRenderTarget);

		//ポストエフェクトを描画数するメソッド
		void DrawPostEffect(RenderContext& rc , RenderTarget& mainRenderTarget);
	};
}