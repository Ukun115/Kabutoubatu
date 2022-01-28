/// <summary>
/// スカイキューブクラス
/// </summary>
#pragma once

namespace nsKabutoubatu
{
	/// <summary>
	/// 前方宣言
	/// </summary>
	class Light;	//ライト

	class SkyCube : public IGameObject
	{
	private:
		/// <summary>
		/// 列挙型
		/// </summary>

		//スカイキューブのテクスチャータイプ
		enum enSkyCubeType
		{
			enDayType,
			enSunlizeType,
			enNightType,
			enSkyCubeType_Num
		};

		Light* m_light = nullptr;

		ModelInitData m_skyCubeInitData;
		Model m_skyCubeModel;
		Texture m_texture[enSkyCubeType_Num];

		int	m_skyCubeType = enDayType;
		const wchar_t* m_mapTextureFilePaths[enSkyCubeType_Num];
		Vector3 m_directionLightColor;
		float m_rotAngle = 0.0f;
		float m_luminance = 1.0f;
		Vector3    m_pos = Vector3::Zero;			//位置
		Quaternion m_rot = Quaternion::Identity;	//回転
		Vector3    m_sca = Vector3::One;			//拡大率

		/// <summary>
		/// 呼ばれて最初に実行される関数
		/// </summary>
		bool Start()override final;

		/// <summary>
		/// 毎フレーム実行される更新処理関数
		/// </summary>
		void Update()override final;

		//モデルの描画メソッド
		void ModelRender(RenderContext& rc)override final;

	public:
		void SetChangeMap(const int mapNum)
		{
			m_skyCubeType = mapNum;
			if (m_skyCubeType == enSkyCubeType_Num)
			{
				m_skyCubeType = enDayType;
			}
		};

		/// <summary>
		/// スカイキューブマップテクスチャを取得
		/// </summary>
		/// <returns>スカイキューブマップテクスチャ</returns>
		Texture& GetSkyCubeMapTexture()
		{
			return m_texture[enSkyCubeType_Num];
		}
	};
}