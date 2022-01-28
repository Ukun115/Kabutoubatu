 /// <summary>
 /// プレイヤーカメラの移動方向変更クラス
 /// </summary>
#include "stdafx.h"
#include "CameraMoveDirectionChange.h"
#include "../Player/Player.h"
#include "PlayerCamera.h"

namespace nsKabutoubatu
{
	bool CameraMoveDirectionChange::Start()
	{
		//プレイヤーのインスタンスを検索
		m_player = FindGO<Player>(nsStdafx::PLAYER_NAME[m_targetPlayer]);
		//プレイヤーカメラのインスタンスを検索
		m_playerCamera = FindGO<PlayerCamera>(nsStdafx::PLAYER_CAMERA_NAME);

		return true;
	}

	CameraMoveDirectionChange::~CameraMoveDirectionChange()
	{

	}

	void CameraMoveDirectionChange::Update()
	{
		//ステージ１の時のカメラ切り替えポイント

		//プレイヤーの位置を取得
		m_playerPosition = m_player->GetPosition();
		//右方向と奥方向がマイナスだったのでマイナス１をかけて正の数を扱えるようにする
		m_playerPosition.x *= -1;
		m_playerPosition.z *= -1;

		//ショップにいるとき、(この高さにショップがある)
		if (m_playerPosition.y >= 900.0f)
		{
			//ショップの固定カメラにする
			m_playerCamera->SetCameraState(enShop);
		}
		//ボスステージにいるとき、
		else if (m_playerPosition.y <= -900.0f)
		{
			//ボスステージの固定カメラにする
			m_playerCamera->SetCameraState(enBossStage);
		}
		//ステージにいるとき、
		else
		{
			switch (m_nowWayPoint)
			{
				//ウェイ１
			case enWay1:
				if (m_playerPosition.x >= 1400.0f)
				{
					//ウェイ2カメラにする
					Way2Camera();
				}

				break;

				//ウェイ2
			case enWay2:
				if (m_playerPosition.x < 1400.0f)
				{
					//ウェイ１カメラにする
					Way1Camera();
				}
				if (m_playerPosition.z >= 6046.0f)
				{
					//ウェイ3カメラにする
					Way3Camera();
				}

				break;

				//ウェイ3
			case enWay3:
				if (m_playerPosition.z < 6046.0f)
				{
					//ウェイ2カメラにする
					Way2Camera();
				}

				break;
			}
		}
	}

	//ウェイ１のカメラ設定
	void CameraMoveDirectionChange::Way1Camera()
	{
		//X方向に切り替える
		m_playerCamera->SetCameraState(enX);
		//YとZを決める
		m_playerCamera->SetXMoveCameraPos(0.0f, 0.0f);

		//現在どこにいるのかを変更する
		m_nowWayPoint = enWay1;
	}

	//ウェイ2のカメラ設定
	void CameraMoveDirectionChange::Way2Camera()
	{
		//Z方向に切り替える
		m_playerCamera->SetCameraState(enZ);
		//XとYを決める
		m_playerCamera->SetZMoveCameraPos(-1750.0f, 0.0f);

		//現在どこにいるのかを変更する
		m_nowWayPoint = enWay2;
	}

	//ウェイ3のカメラ設定
	void CameraMoveDirectionChange::Way3Camera()
	{
		//X方向に切り替える
		m_playerCamera->SetCameraState(enX);
		//YとZを決める
		m_playerCamera->SetXMoveCameraPos(0.0f, -6400.0f);

		//現在どこにいるのかを変更する
		m_nowWayPoint = enWay3;
	}
}