/// <summary>
/// スカイキューブクラス
/// </summary>
#include "stdafx.h"
#include "SkyCube.h"
#include "Light.h"

namespace nsKabutoubatu
{
	bool SkyCube::Start()
	{
		m_light = FindGO<Light>(nsStdafx::LIGHT_NAME);

		//時間帯によってディレクションライトのカラーを変更
		switch (m_skyCubeType)
		{
		//朝・昼
		case enDayType:
			m_directionLightColor = { 0.4f,0.4f,0.4f };
			break;
		//朝焼け・夕暮れ
		case enSunlizeType:
			m_directionLightColor = { 0.7f,0.2f,0.2f };
			break;
		//夜
		case enNightType:
			//環境光のみ
			m_directionLightColor = { 0.0f,0.0f,0.0f };
			break;
		}
		//設定したカラーをセット
		m_light->ChangeDirectionLightDataColor(m_directionLightColor);

		//テクスチャのファイルパス
		m_mapTextureFilePaths[enDayType] = L"Assets/modelData/skyCubeMapDay.dds";
		m_mapTextureFilePaths[enSunlizeType] = L"Assets/modelData/skyCubeMapSunrise.dds";
		m_mapTextureFilePaths[enNightType] = L"Assets/modelData/skyCubeMapNight.dds";

		//スカイキューブの大きさを超大きくする
		m_sca *= 10000.0f;

		//tkmファイルのファイルパスを指定
		m_skyCubeInitData.m_tkmFilePath = "Assets/modelData/sky.tkm";
		//シェーダーファイルのファイルパスを指定
		m_skyCubeInitData.m_fxFilePath = "Assets/shader/SkyCubeMap.fx";

		m_skyCubeInitData.m_vsEntryPointFunc = "VSMain";

		//スカイキューブのテクスチャ
		for (int textureNum = 0; textureNum < enSkyCubeType_Num; textureNum++) {
			m_texture[textureNum].InitFromDDSFile(m_mapTextureFilePaths[textureNum]);
		}
		//テクスチャをfxに渡す
		m_skyCubeInitData.m_expandShaderResourceView[0] = &m_texture[m_skyCubeType];
		//輝度の数値をfxに渡す
		m_skyCubeInitData.m_expandConstantBuffer[0] = &m_luminance;
		m_skyCubeInitData.m_expandConstantBufferSize[0] = sizeof(m_luminance);

		//初期化情報を使ってスカイキューブ表示処理を初期化する
		m_skyCubeModel.Init(m_skyCubeInitData);

		return true;
	}

	void SkyCube::Update()
	{
		//回転角度を更新
		m_rotAngle += 0.01f;
		//回転角度をクォータニオンにセット
		m_rot.SetRotationDeg(Vector3::AxisY, m_rotAngle);
		//スカイキューブの位置、回転、拡大を更新
		m_skyCubeModel.UpdateWorldMatrix(m_pos, m_rot, m_sca);
	}

	//スカイキューブの描画メソッド
	void SkyCube::ModelRender(RenderContext& rc)
	{
		//スカイキューブのドローコールを実行し、表示する
		m_skyCubeModel.Draw(rc);
	}
}