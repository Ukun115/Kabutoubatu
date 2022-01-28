/// <summary>
/// ライトクラス
/// </summary>

#include "stdafx.h"

namespace nsKabutoubatu
{
	namespace nsLight
	{
		//ディレクションライト
		const Vector3 DIRLIG_DIR = { 0.0f, -1.0f ,-0.5f };	//方向
		const Vector3 DIRLIG_COLOR = { 0.4f, 0.4f ,0.4f };	//色
		//スポットライト
		const Vector3 SPOLIG_POS = { 0.0f, 50.0f ,0.0f };	//位置
		const Vector3 SPOLIG_DIR = { 1.0f, -1.0f ,-1.0f };	//方向
		const float	  SPOLIG_RAN = 100.0f;					//影響範囲
		const float	  SPOLIG_ANG = 75.0f;					//射出角度
		//半球ライト
		const Vector3 HEMISPHERELIG_GROUNDCOLOR = { 0.5f,0.3f,0.1f };	//地面色
		const Vector3 HEMISPHERELIG_SKYCOLOR = { 0.15f,0.3f,0.45f };	//天球色
		const Vector3 HEMISPHERELIG_GROUNDNORMAL = { 0.0f,1.0f,0.0f };	//地面の法線
	}

	bool Light::Start()
	{
		//視点の位置を設定する
		m_ligData.eyePos = g_camera3D->GetPosition();

		//ライトカメラの位置を設定。これはライトの位置
		m_ligCameraPos = g_camera3D->GetPosition();
		m_ligCamera.SetPosition(m_ligCameraPos);
		//ライトカメラの注視点を設定。これがライトが照らしている場所
		m_ligCameraTarget = g_camera3D->GetTarget();
		m_ligCamera.SetTarget(m_ligCameraTarget);
		//ライトカメラの上方向を計算して求める
		Vector3 Vec_x = Vector3::AxisX;
		Vector3 TarPos = m_ligCameraTarget - m_ligCameraPos;
		TarPos.Normalize();
		Vector3 CameraUp;
		CameraUp.Cross(TarPos, Vec_x);
		//ライトカメラの上方向のベクトルを正規化
		CameraUp.Normalize();
		//上方向を設定。
		m_ligCamera.SetUp(CameraUp);
		//ライトビュープロジェクション行列を計算している
		m_ligCamera.Update();

		return true;
	}

	void Light::Update()
	{
		//ライトカメラの位置を更新
		m_ligCameraPos = g_camera3D->GetPosition();
		//ライトカメラの位置をカメラの位置より遠くする
		m_ligCameraPos.x -= 100.0f;
		m_ligCameraPos.y += 1000.0f;
		m_ligCameraPos.z += 1000.0f;
		//ライトカメラの視点を更新
		m_ligCamera.SetPosition(m_ligCameraPos);
		//カメラの注視点を取得
		m_ligCameraTarget = g_camera3D->GetTarget();
		//ライトカメラの注視点を更新
		m_ligCamera.SetTarget(m_ligCameraTarget);
		//ライトビュープロジェクション行列を計算している
		m_ligCamera.Update();
		//ライトビュープロジェクション行列を拡張定数バッファに設定する。
		m_ligData.mLVP = m_ligCamera.GetViewProjectionMatrix();
	}

	//ディレクションライトの方向,色を設定する関数
	void Light::SetDirectionLightData()
	{
		//ディレクションライトの方向を設定
		m_ligData.dirLigData.dirDirection = nsLight::DIRLIG_DIR;
		//ディレクションライトを正規化して大きさ1のベクトルにする
		m_ligData.dirLigData.dirDirection.Normalize();
		//ディレクションライトの色を設定
		m_ligData.dirLigData.dirColor = nsLight::DIRLIG_COLOR;
	}

	//ポイントライトの座標,色,影響範囲を設定する関数
	void Light::SetPointLightData(const Vector3 position, const Vector3 color, const float range, const int lignum)
	{
		//ポイントライトの座標を設定
		m_ligData.poiLigData[lignum].ptPosition = position;
		//ポイントライトの色を設定
		m_ligData.poiLigData[lignum].ptColor = color;
		//ポイントライトの影響範囲を設定
		m_ligData.poiLigData[lignum].ptRange = range;
	}

	//スポットライトの座標,色,方向,射出範囲,射出角度を設定する関数
	void Light::SetSpotLightData()
	{
		//スポットライトの座標を設定
		m_ligData.spotLigData.spPosition = nsLight::SPOLIG_POS;
		//スポットライトの色を設定
		m_ligData.spotLigData.spColor = nsStdafx::WHITE;
		//スポットライトの方向を設定
		m_ligData.spotLigData.spDirection = nsLight::SPOLIG_DIR;
		//方向データだから大きさを1にするために正規化をする
		m_ligData.spotLigData.spDirection.Normalize();
		//スポットライトの射出範囲を設定
		m_ligData.spotLigData.spRange = nsLight::SPOLIG_RAN;
		//スポットライトの射出角度を設定
		//(デグリー単位(75度)からラジアン単位に変換した値を代入)
		m_ligData.spotLigData.spAngle = Math::DegToRad(nsLight::SPOLIG_ANG);
	}

	//半球ライトの地面色（照り返しのライト）,天球色,地面の法線を設定する関数
	void Light::SetHemiSphereLightData()
	{
		//地面色を設定
		m_ligData.hemiSphereLigData.hemiGroundColor = nsLight::HEMISPHERELIG_GROUNDCOLOR;
		//天球色を設定
		m_ligData.hemiSphereLigData.hemiSkyColor = nsLight::HEMISPHERELIG_SKYCOLOR;
		//地面の法線を設定
		m_ligData.hemiSphereLigData.hemiGroundNormal = nsLight::HEMISPHERELIG_GROUNDNORMAL;
	}
}