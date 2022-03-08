/// <summary>
/// 矢クラス
/// </summary>
#include "stdafx.h"
#include "Arrow.h"
#include "../../Online/OnlineUpdateSpeed.h"

namespace nsKabutoubatu
{
	namespace nsArrow
	{
		const float ARROW_SPEED_POWER = 15.0f;
		const char* ARROW_MODEL_FILE_NAME = "Arrow";
	}

	bool Arrow::Start()
	{
		//オンライン時の更新速度管理クラスのインスタンスを検索
		m_onlineUpdateSpeed = FindGO<OnlineUpdateSpeed>(nsStdafx::ONLINEUPDATESPEED_NAME);

		//移動速度を正規化
		m_moveSpeed.Normalize();
		//地面と平行にしか移動しないようにするためy方向を0にする。
		m_moveSpeed.y = 0.0f;

		//モデルを初期化
		m_arrowModel = NewGO<SkinModelRender>();
		m_arrowModel->SetShadowCaster(true);	//影を落とす
		m_arrowModel->SetOutline(false);		//輪郭線をつけない
		m_arrowModel->SetSilhouette(true);		//シルエットをつける
		m_arrowModel->SetPlayerMode(m_playerNum);	//プレイヤー１か２かを渡す
		m_arrowModel->Init(nsArrow::ARROW_MODEL_FILE_NAME);
		//モデルの位置を更新
		m_arrowModel->SetPosition(m_pos);
		m_moveSpeed *= nsArrow::ARROW_SPEED_POWER * m_onlineUpdateSpeed->GetSpeed();

		//矢の向きをセット
		//回転角度を求める
		float m_rotAngle = atan2(m_moveSpeed.x, m_moveSpeed.z);
		//回転を更新
		Quaternion m_rot;
		m_rot.SetRotation(Vector3::AxisY, m_rotAngle);
		m_arrowModel->SetRotation(m_rot);

		return true;
	}

	Arrow::~Arrow()
	{
		//モデル削除
		DeleteGO(m_arrowModel);
	}

	void Arrow::Update()
	{
		m_deleteTimer++;
		if (m_deleteTimer > 120 / m_onlineUpdateSpeed->GetSpeed())
		{
			DeleteGO(this);
		}

		//モデルの位置を更新
		m_pos += m_moveSpeed;
		m_arrowModel->SetPosition(m_pos);
	}
}