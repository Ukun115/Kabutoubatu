/// <summary>
/// ライトクラス
/// </summary>
#pragma once

/// <summary>
/// 前方宣言
/// </summary>
class Camera;	//カメラ

namespace nsKabutoubatu
{
	/// <summary>
	/// 構造体
	/// </summary>

	//ディレクションライト
	struct DirLigData
	{
		Vector3 dirDirection;	//方向
		float pad1 = 0.0f;		//HLSL側の定数バッファのfloat3型(12個分)の変数は16の倍数のアドレスに配置されるため、C++側にはパディングを埋めておき、埋め合わせをしておく。
		Vector3 dirColor;		//色
		float pad2 = 0.0f;		//パディング
	};

	//ポイントライト
	struct PoiLigData
	{
		Vector3 ptPosition;		//位置
		float pad = 0.0f;		//パディング
		Vector3 ptColor;		//色
		float ptRange = 0.0f;	//影響範囲
	};

	//スポットライト
	struct SpotLigData
	{
		Vector3 spPosition;		//位置
		float pad = 0.0f;		//パディング
		Vector3 spColor;		//色
		float spRange = 0.0f;	//影響範囲
		Vector3 spDirection;	//射出方向
		float spAngle = 0.0f;	//射出角度
	};

	//半球ライト
	struct HemiSphereLight
	{
		Vector3 hemiGroundColor;	//地面色（照り返しのライト）
		float pad1 = 0.0f;			//パディング
		Vector3 hemiSkyColor;		//天球色
		float pad2 = 0.0f;			//パディング
		Vector3 hemiGroundNormal;	//地面の法線
		float pad3 = 0.0f;			//パディング
	};

	//ライト全体
	struct ligData
	{
		DirLigData  dirLigData;				//ディレクションライト構造体データ
		PoiLigData  poiLigData[12];			//ポイントライト構造体データ
		SpotLigData spotLigData;			//スポットライト構造体データ
		HemiSphereLight hemiSphereLigData;	//半球ライト構造体データ

		Vector3 eyePos;						//視点の位置

		float pad = 0.0f;					//パディング

		Matrix mLVP;						//ライトビュープロジェクション行列
	};

	class Light : public IGameObject
	{
	private:
		ligData m_ligData;		//ライト構造体のデータ
		Camera m_ligCamera;		//ライトカメラ

		Vector3 m_ligCameraPos;		//ライトカメラの視点
		Vector3 m_ligCameraTarget;	//ライトカメラの注視点

		/// <summary>
		/// 呼ばれて最初に実行される関数
		/// </summary>
		bool Start()override final;

		/// <summary>
		/// 毎フレーム実行される更新処理関数
		/// </summary>
		void Update()override final;

	public:
		/// <summary>
		/// セッター
		/// </summary>
		//ディレクションライトの方向,色を設定するメソッド
		void SetDirectionLightData();

		//ポイントライトの座標,色,影響範囲を設定するメソッド
		void SetPointLightData(const Vector3 position, const Vector3 color, const float range, const int lignum);

		//スポットライトの座標,色,方向,射出範囲,射出角度を設定するメソッド
		void SetSpotLightData();

		//半球ライトの地面色（照り返しのライト）,天球色,地面の法線を設定するメソッド
		void SetHemiSphereLightData();

		void ChangeDirectionLightDataColor(Vector3& color) { m_ligData.dirLigData.dirColor = color; };

		/// <summary>
		/// ライトのデータを取得
		/// </summary>
		/// <returns>ライトのデータ</returns>
		ligData GetLight()const { return m_ligData; }

		/// <summary>
		/// ライトのデータのアドレスを取得
		/// </summary>
		/// <returns>ライトのデータのアドレス</returns>
		ligData* GetLightAdoress() { return &m_ligData; }

		/// <summary>
		/// ライトカメラのアドレスを取得
		/// </summary>
		/// <returns>ライトカメラのアドレス</returns>
		Camera* GetLightCamera() { return &m_ligCamera; }
	};
}