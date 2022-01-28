/// <summary>
/// プレイヤーカメラクラス
/// </summary>
#include "stdafx.h"
#include "PlayerCamera.h"
#include "../Player/Player.h"
#include "CameraMoveDirectionChange.h"

namespace nsKabutoubatu
{
	bool PlayerCamera::Start()
	{
		//プレイヤーのインスタンスを検索
		m_player = FindGO<Player>(nsStdafx::PLAYER_NAME[m_targetPlayer]);
		//カメラの移動方向を変えるクラスを生成
		m_cameraMoveDirectionChange = NewGO<CameraMoveDirectionChange>();
		m_cameraMoveDirectionChange->SetTargetPlayer(m_targetPlayer);
		//遠平面までの距離を設定
		g_camera3D->SetFar(1000000);
		g_camera3D->SetViewAngle(Math::DegToRad(38.0f));
		return true;
	}

	PlayerCamera::~PlayerCamera()
	{
		//カメラの移動方向を変えるクラス
		DeleteGO(m_cameraMoveDirectionChange);
	}

	void PlayerCamera::Update()
	{
		//カメラの視点＆注視点をプレイヤーの位置に移動
		m_cameraPos = m_player->GetPosition();
		m_cameraTarget = m_player->GetPosition();

		//カメラの移動方向の状態によって固定する
		switch (m_cameraState)
		{
		//横スクロール
		case enX:
			//視点
			m_cameraPos.y = m_cameraTargetYPosition + 250.0f;
			m_cameraPos.z = m_cameraTargetZPosition + 1000.0f;
			//注視点
			m_cameraTarget.y = m_cameraTargetYPosition + 100.0f;
			m_cameraTarget.z = m_cameraTargetZPosition;
			break;
		//縦スクロール
		case enY:
			//視点
			m_cameraPos.x = m_cameraTargetXPosition;
			m_cameraPos.y += 200.0f;
			m_cameraPos.z = m_cameraTargetZPosition + 1000.0f;
			//注視点
			m_cameraTarget.x = m_cameraTargetXPosition;
			m_cameraTarget.z = m_cameraTargetZPosition;
			break;
		//奥スクロール
		case enZ:
			//視点
			m_cameraPos.x = m_cameraTargetXPosition;
			m_cameraPos.y = m_cameraTargetYPosition + 200.0f;
			m_cameraPos.z += 1000.0f;
			//注視点
			m_cameraTarget.x = m_cameraTargetXPosition;
			m_cameraTarget.y = m_cameraTargetYPosition;
			m_cameraTarget.z -= 500.0f;
			break;
		//ショップカメラ
		case enShop:
			//視点
			m_cameraPos = { 0.0f,10150.0f,600.0f };
			//注視点
			m_cameraTarget = { 0.0f, 10100.0f, 0.0f };
			break;
		//ボスステージカメラ
		case enBossStage:
			//視点
			m_cameraPos = { 0.0f,-9500.0f,1200.0f };
			//注視点
			m_cameraTarget = { 0.0f, -10000.0f, 0.0f };
			break;
		}

		//カメラの位置を更新
		g_camera3D->SetPosition(m_cameraPos);
		//カメラの注視点をプレイヤーの位置に更新
		g_camera3D->SetTarget(m_cameraTarget);
		//カメラビュープロジェクション行列を拡張定数バッファに設定する。
		m_cameraData.mCVP = g_camera3D->GetViewProjectionMatrix();
	}
}