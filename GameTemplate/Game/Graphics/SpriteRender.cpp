/// <summary>
/// 画像描画クラス
/// </summary>
#include "stdafx.h"

namespace nsKabutoubatu
{
	void SpriteRender::Update()
	{
		//位置、回転、拡大率を更新
		if (m_pivotChangeFlg)
		{
			m_sprite.Update(m_pos, m_rot, m_sca, m_pivot);
		}
		else
		{
			m_sprite.Update(m_pos, m_rot, m_sca);
		}
	}

	void SpriteRender::UiUpdate()
	{
		//位置、回転、拡大率を更新
		if (m_pivotChangeFlg)
		{
			m_sprite.Update(m_pos, m_rot, m_sca, m_pivot);
		}
		else
		{
			m_sprite.Update(m_pos, m_rot, m_sca);
		}
	}

	//画像を初期化する関数
	void SpriteRender::Init(const char* filePath, const float width, const float height, AlphaBlendMode mode)
	{
		//ファイル名をファイルパスにする
		sprintf(m_filePath, "Assets/image/dds/%s.dds", filePath);
		m_spriteInitData.m_ddsFilePath[0] = m_filePath;					//ファイルパス
		m_spriteInitData.m_fxFilePath = "Assets/shader/sprite.fx";		//シェーダーファイルのパスを指定
		m_spriteInitData.m_width = width;								//横幅
		m_spriteInitData.m_height = height;								//縦幅
		m_spriteInitData.m_alphaBlendMode = mode;						//透過
		//画像を初期化
		m_sprite.Init(m_spriteInitData);
	}

	//描画関数
	void SpriteRender::SpriteDataRender(RenderContext& renderContext)
	{
		//ドロー関数
		m_sprite.Draw(renderContext);
	}
}