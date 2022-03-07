/// <summary>
/// コインクラス
/// </summary>
#include "stdafx.h"
#include "DropCoin.h"
#include "../Player/Player.h"
#include "../Player/PlayerStatus.h"
#include "../Camera/PlayerCamera.h"
#include "../Online/OnlineUpdateSpeed.h"

namespace nsKabutoubatu
{
	bool DropCoin::Start()
	{
		m_onlineUpdateSpeed = FindGO<OnlineUpdateSpeed> (nsStdafx::ONLINEUPDATESPEED_NAME);

		//プレイヤーのインスタンスを検索
		for (int playerNum = enPlayer1; playerNum < m_playerNum; playerNum++)
		{
			m_player[playerNum] = FindGO<Player>(nsStdafx::PLAYER_NAME[playerNum]);
			m_playerStatus[playerNum] = FindGO<PlayerStatus>(nsStdafx::PLAYER_STATUS_NAME[playerNum]);
		}

		m_playerCamera = FindGO<PlayerCamera>(nsStdafx::PLAYER_CAMERA_NAME);

		//コインモデルを初期化
		CoinModelInit();

		return true;
	}

	DropCoin::~DropCoin()
	{
		//モデルを破棄
		DeleteGO(m_coin);
	}

	void DropCoin::Update()
	{
		//コインのバウンド処理
		Bound();
		//コインの回転処理
		Turn();
		//コイン取得処理
		CoinGet();
	}

	//コインモデルの初期化メソッド
	void DropCoin::CoinModelInit()
	{
		//モデルを初期化
		m_coin = NewGO<SkinModelRender>();
		m_coin->Init("Coin");
		//初めのコインのバウンドパワー
		m_boundPower = m_boundPowerDown * 100.0f;
		//調整しない状態だとモデルが地面に埋もれたので、Y座標を調整
		m_pos.y += 50.0f;
		//スタート位置のY座標を保存
		m_boundStartPosY = m_pos.y;
		//初期位置を更新
		m_coin->SetPosition(m_pos);
	}

	//コインがバウンドする処理メソッド
	void DropCoin::Bound()
	{
		m_pos.y += m_boundPower * g_gameTime->GetFrameDeltaTime() * m_onlineUpdateSpeed->GetSpeed();
		m_boundPower -= 980.0f * g_gameTime->GetFrameDeltaTime() * m_onlineUpdateSpeed->GetSpeed();

		//地面に落ちた時、
		if (m_boundStartPosY > m_pos.y)
		{
			m_getOkFlg = true;

			//バウンドパワーが0になったら、
			if (m_boundPowerDown == 0)
			{
				//クラスを破棄
				DeleteGO(this);
			}

			//バウンド音の初期化
			if ((m_playerCamera->GetCameraTarget() - m_pos).Length() < nsStdafx::SOUND_CAN_HEARD_RANGE)
			{
				m_coinSound = NewGO<SoundSource>();
				m_coinSound->Init(L"Assets/sound/Coin_Bound.wav");
				m_coinSound->SetVolume(0.2f);
				//バウンド音を再生
				m_coinSound->Play(false);
			}

			//バウンドパワーをダウンさせる
			m_boundPowerDown--;
			//次の高さのバウンドパワーにする
			m_boundPower = m_boundPowerDown * 100.0f;
		}
		//位置を更新
		m_coin->SetPosition(m_pos);
	}

	//コインの回転処理メソッド
	void DropCoin::Turn()
	{
		//回転速度を遅くしていく
		m_firstRotSpeed -= 0.2f;
		//m_firstRotSpeedが負の数になってコインが逆回転しないように回転速度の補正
		m_firstRotSpeed = max(0.0f, m_firstRotSpeed);
		//回転角度を更新
		m_rotAngle += m_firstRotSpeed;
		//回転角度をクォータニオンにセット
		m_rot.SetRotationDeg(Vector3::AxisY, m_rotAngle);
		//モデルを回転させる
		m_coin->SetRotation(m_rot);
	}

	//コイン取得処理メソッド
	void DropCoin::CoinGet()
	{
		if (m_getOkFlg)
		{

			for (int playerNum = enPlayer1; playerNum < m_playerNum; playerNum++)
			{
				//プレイヤーとコインとの距離を計算
				m_playerToCoinDistancelength = (m_player[playerNum]->GetPosition() - m_pos).Length();
				//プレイヤーとコインの距離を測り、コインにプレイヤーが触れていたら、
				if (m_playerToCoinDistancelength < 250.0f)
				{
					//プレイヤーの所持金を増やす
					m_playerStatus[playerNum]->AddCoin(m_coinValue);
					//クラスを破棄
					DeleteGO(this);
				}
			}
		}
	}
};