/// <summary>
/// ポストエフェクトクラス
/// </summary>
#include "stdafx.h"
#include "PostEffect.h"
#include "DepthShadow.h"	//デプスシャドウ
#include "Bloom.h"			//ブルーム
#include "DoF.h"			//被写界深度
#include "Silhouette.h"		//シルエット
#include "MiniMap.h"		//ミニマップ

namespace nsKabutoubatu
{
	//ポストエフェクトの初期化メソッド
	void PostEffect::Init(RenderTarget& mainRenderTarget, RenderTarget& zprepassRenderTarget)
	{
		//ポストエフェクトのそれぞれのインスタンスを生成
		m_depthShadow = NewGO<DepthShadow>(nsStdafx::PRIORITY_0,nsStdafx::DEPTHSHADOW_NAME);
		m_bloom = NewGO<Bloom>();
		m_dof = NewGO<DoF>(nsStdafx::PRIORITY_0, nsStdafx::DOF_NAME);
		m_silhouette = NewGO<Silhouette>(nsStdafx::PRIORITY_0, nsStdafx::SILHOUETTE_NAME);
		m_miniMap = NewGO<MiniMap>(nsStdafx::PRIORITY_9, nsStdafx::MINIMAP_NAME);

		//デプスシャドウを初期化
		m_depthShadow->Init();
		//ブルームを初期化
		m_bloom->Init(mainRenderTarget);
		//被写界深度を初期化
		m_dof->Init(mainRenderTarget, zprepassRenderTarget);
		//シルエットを初期化
		m_silhouette->Init();
		//ミニマップを初期化
		m_miniMap->Init();
	}

	//シャドウを描画するメソッド
	void PostEffect::DrawShadow(RenderContext& rc)
	{
		m_depthShadow->DrawShadow(rc);
	}

	//シルエットを描画するメソッド
	void PostEffect::DrawSilhouette(RenderContext& rc)
	{
		m_silhouette->DrawSilhouetto(rc);
	}

	//ミニマップを描画するメソッド
	void PostEffect::DrawMiniMap(RenderContext& rc)
	{
		m_miniMap->DrawMiniMap(rc);
	}

	//レンダーターゲットにメインレンダーターゲットをセットするメソッド
	void PostEffect::SettingRenderTarget(RenderContext& rc,RenderTarget& mainRenderTarget)
	{
		// レンダリングターゲットをm_mainRenderTargetに変更する
		// レンダリングターゲットとして利用できるまで待つ
		rc.WaitUntilToPossibleSetRenderTarget(mainRenderTarget);
		// レンダリングターゲットを設定
		rc.SetRenderTargetAndViewport(mainRenderTarget);

		// レンダリングターゲットをクリア
		rc.ClearRenderTargetView(mainRenderTarget);

		//被写界深度のレンダーターゲットをセット
		//被写界深度を移さないようにするため、下記をコメントアウトしています。
		//m_dof->SettingDoFRenderTarget(rc, mainRenderTarget);
	}


	//ポストエフェクトを描画数するメソッド
	void PostEffect::DrawPostEffect(RenderContext& rc,RenderTarget& mainRenderTarget)
	{
		//ブルームを描画
		m_bloom->DrawBloom(rc, mainRenderTarget);
		//被写界深度を描画
		m_dof->DrawDoF(rc, mainRenderTarget);
	}
}