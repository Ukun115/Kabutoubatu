/// <summary>
/// ミニマップクラス
/// </summary>
#include "stdafx.h"
#include "MiniMap.h"

namespace nsKabutoubatu
{
	namespace nsMiniMap
	{
		//ミニマップの解像度
		const int MINIMAP_WEIDTH = 512;
		const int MINIMAP_HEIGHT = 512;
		//ミップレベル
		const int MIP_LEBEL = 1;
		//テクスチャ配列のサイズ
		const int ARRAY_SIZE = 1;
		//配列の最初の番号
		const int FIRST_ARRAY_NUMBER = 0;

		const char* const SPRITE_FX= "Assets/shader/sprite.fx";
	}

	bool MiniMap::Start()
	{
		/////////////ミニマップカメラ設定//////////////
		{
			//ファーを設定
			m_miniMapCamera.SetFar(100000);
			//視点をステージの真上にする
			m_cameraPos = { -1700.0f,8000.0f,-3870.0f };
			m_miniMapCamera.SetPosition(m_cameraPos);
			m_cameraTargetPos = { -1700.0f,0.0f,-3870.0f };
			//注視点をステージの中央にする
			m_miniMapCamera.SetTarget(m_cameraTargetPos);
			//上方向を設定。
			m_miniMapCamera.SetUp({ 0.0f,0.0f,-1.0f });
			//ビュープロジェクション行列を計算している
			m_miniMapCamera.Update();
		}
		///////////ミニマップカメラ設定END///////////////

		///////////////////////////ミニマップ画像設定///////////////////////////////
		{
			//ミニマップを表示するためのスプライトを初期化する
			m_miniMapSpriteInitData.m_textures[nsMiniMap::FIRST_ARRAY_NUMBER] = &m_miniMap.GetRenderTargetTexture();
			m_miniMapSpriteInitData.m_fxFilePath = nsMiniMap::SPRITE_FX;
			m_miniMapSpriteInitData.m_width = 460;
			m_miniMapSpriteInitData.m_height = 270;
			m_miniMapSprite.Init(m_miniMapSpriteInitData);
			m_miniMapSprite.SetRotation({ 0.0f,0.0f,0.0f,1.0f });
		}
		/////////////////////////ミニマップ画像設定END//////////////////////////////

		//初めはマップ非表示
		//表示非表示切替はプレイヤークラスで書いてる
		Deactivate();

		return true;
	}

	//ミニマップの初期化メソッド
	void MiniMap::Init()
	{
		//ミニマップ描画用のレンダリングターゲットを作成する。
		m_miniMap.Create(
			nsMiniMap::MINIMAP_WEIDTH,
			nsMiniMap::MINIMAP_HEIGHT,
			nsMiniMap::MIP_LEBEL,
			nsMiniMap::ARRAY_SIZE,
			//ミニマップのカラーバッファのフォーマットを変更している。
			DXGI_FORMAT_R32G32B32A32_FLOAT,
			DXGI_FORMAT_D32_FLOAT,
			clearColor
		);
	}

	//ミニマップ描画メソッド
	void MiniMap::DrawMiniMap(RenderContext& rc)
	{
		// レンダリングターゲットをミニマップに変更する
		rc.WaitUntilToPossibleSetRenderTarget(m_miniMap);
		rc.SetRenderTargetAndViewport(m_miniMap);
		//レンダリングターゲットをクリアにする
		rc.ClearRenderTargetView(m_miniMap.GetRTVCpuDescriptorHandle(), m_miniMap.GetRTVClearColor());
		rc.ClearDepthStencilView(m_miniMap.GetDSVCpuDescriptorHandle(), m_miniMap.GetDSVClearValue());
		//マップを書き込む
		GameObjectManager::GetInstance()->ExecuteMiniMapRender(rc);
		// 書き込み完了待ち
		rc.WaitUntilFinishDrawingToRenderTarget(m_miniMap);
	}

	//ミニマップの画像描画関数
	void MiniMap::SpriteDataRender(RenderContext& renderContext)
	{
		//ドロー関数
		m_miniMapSprite.Draw(renderContext);
	}
}