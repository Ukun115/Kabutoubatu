/// <summary>
/// レンダリングエンジンクラス
/// </summary>
#include "stdafx.h"
#include "RenderingEngine.h"
#include "PostEffect.h"

namespace nsKabutoubatu
{
	void RenderingEngine::Init()
	{
		//メインレンダリングターゲットを初期化
		InitMainRenderTarget();
		//ZPrepass用のレンダリングターゲットを初期化
		InitZPrepassRenderTarget();
		//ポストエフェクトを初期化
		m_postEffect = NewGO<nsKabutoubatu::PostEffect>();
		m_postEffect->Init(m_mainRenderTarget,m_zprepassRenderTarget);
	}

	//ZPrepass用のレンダリングターゲットを初期化
	void RenderingEngine::InitZPrepassRenderTarget()
	{
		m_zprepassRenderTarget.Create(
			g_graphicsEngine->GetFrameBufferWidth(),
			g_graphicsEngine->GetFrameBufferHeight(),
			1,
			1,
			DXGI_FORMAT_R32G32B32A32_FLOAT,
			DXGI_FORMAT_D32_FLOAT
		);

	}

	//メインレンダリングターゲットを初期化
	void RenderingEngine::InitMainRenderTarget()
	{
		m_mainRenderTarget.Create(
			g_graphicsEngine->GetFrameBufferWidth(),
			g_graphicsEngine->GetFrameBufferHeight(),
			1,
			1,
			DXGI_FORMAT_R32G32B32A32_FLOAT,
			DXGI_FORMAT_D32_FLOAT
		);
	}

	void RenderingEngine::SettingZPrepass(RenderContext& rc)
	{
		// まず、レンダリングターゲットとして設定できるようになるまで待つ
		rc.WaitUntilToPossibleSetRenderTarget(m_zprepassRenderTarget);

		// レンダリングターゲットを設定
		rc.SetRenderTargetAndViewport(m_zprepassRenderTarget);

		// レンダリングターゲットをクリア
		rc.ClearRenderTargetView(m_zprepassRenderTarget);

		//ZPrepassを書き込む
		GameObjectManager::GetInstance()->ExecuteZPrepassRender(rc);

		rc.WaitUntilFinishDrawingToRenderTarget(m_zprepassRenderTarget);
	}

	//描画順
	void RenderingEngine::Execute(RenderContext& rc)
	{
		// シャドウマップへの描画
		//シャドウを描画
		m_postEffect->DrawShadow(rc);

		//シルエット描画
		m_postEffect->DrawSilhouette(rc);

		//ミニマップ描画
		m_postEffect->DrawMiniMap(rc);

		//ZPrepassを描画
		SettingZPrepass(rc);

		//レンダーターゲットにメインレンダーターゲットをセット
		m_postEffect->SettingRenderTarget(rc,m_mainRenderTarget);

		//モデルを描画
		GameObjectManager::GetInstance()->ExecuteModelRender(rc);

		// ポストエフェクトを描画
		m_postEffect->DrawPostEffect(rc, m_mainRenderTarget);

		//画像を描画
		GameObjectManager::GetInstance()->ExecuteSpriteRender(rc);

		//フォントを描画
		GameObjectManager::GetInstance()->ExecuteFontDataRender(rc);

		//画像を描画
		GameObjectManager::GetInstance()->ExecuteSpriteRenderSub(rc);

		//フォントを描画
		GameObjectManager::GetInstance()->ExecuteFontDataRenderSub(rc);
	}
}