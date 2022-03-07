/// <summary>
/// 回復アイテムクラス
/// </summary>
#include "stdafx.h"
#include "RecoveryItem.h"
#include "../Player/Player.h"
#include "../Player/PlayerStatus.h"
#include "../Online/OnlineUpdateSpeed.h"

namespace nsKabutoubatu
{
	namespace nsRecoveryItem
	{
		//移動力
		float MOVE_POWER = 4.0f;
	}

	bool RecoveryItem::Start()
	{
		m_onlineUpdateSpeed = FindGO< OnlineUpdateSpeed>(nsStdafx::ONLINEUPDATESPEED_NAME);

		if (m_isSoloPlay)
		{
			m_totalPlayerNum = enPlayer2;
		}

		//プレイヤーのインスタンスを検索
		for (int playerNum = enPlayer1; playerNum < m_totalPlayerNum; playerNum++)
		{
			m_player[playerNum] = FindGO<Player>(nsStdafx::PLAYER_NAME[playerNum]);
			m_playerStatus[playerNum] = FindGO<PlayerStatus>(nsStdafx::PLAYER_STATUS_NAME[playerNum]);
		}

		//回復アイテムのモデルを初期化
		RecoveryItemInit();

		return true;
	}

	RecoveryItem::~RecoveryItem()
	{
		//モデルを削除
		DeleteGO(m_model);
	}

	void RecoveryItem::Update()
	{
		//回復アイテムの位置をプレイヤーの頭上に置くための取得
		if (m_canPlayerMoveSpeedGet)
		{
			//位置をプレイヤーの頭上に持っていく
			m_pos = m_player[m_playerNum]->GetPosition();
			m_pos.y += 140.0f;
			//プレイヤーの移動速度を取得
			m_moveSpeed = m_player[m_playerNum]->GetMoveSpeed();
		}

		//プレイヤーの回復アイテムの状態によって変化
		switch (m_player[m_playerNum]->GetRecoveryItemState())
		{
		//プレイヤーにもちあげられている状態
		case enRecoverySet:
			m_canPlayerMoveSpeedGet = true;

			break;

		//プレイヤーに投げられた状態
		case enRecoveryThrow:
			//ソロプレイの時は即座に自身を回復する
			if (m_isSoloPlay)
			{
				//プレイヤーのHPがすでに最大の時、
				if (m_playerStatus[m_playerNum]->GetHitPoint() == m_playerStatus[m_playerNum]->GetMaxHitPoint())
				{
					//回復させずに回復アイテムをしまう。
					//相方に当たらなかったら手持ちに戻す
					m_playerStatus[m_playerNum]->AddRecoveryItemNum();
				}
				//プレイヤーのHPが最大じゃないとき、
				else
				{
					//ヒットポイントをプレイヤーの回復力分回復させる
					m_playerStatus[m_playerNum]->AddHitPoint(m_recoveryPower);
					m_playerStatus[m_playerNum]->AddRecoveryReceiveNum(m_recoveryPower);
				}
				//プレイヤーの状態を回復初期化状態にする
				//このメソッドでモデルを削除する
				m_player[m_playerNum]->SetRecoveryState(enRecoveryDelete);
			}
			else
			{
				if (m_canPlayerMoveSpeedGet)
				{
					//プレイヤーの移動速度を取得
					m_moveSpeed = m_player[m_playerNum]->GetMoveSpeed();
					//正規化してプレイヤーの移動方向のみを取得
					m_moveSpeed.Normalize();
					//キャラコンの位置を更新
					m_charaCon.SetPosition(m_pos);
					//移動方向に速度をつける。
					m_moveSpeed *= nsRecoveryItem::MOVE_POWER * m_onlineUpdateSpeed->GetSpeed();
					//斜め上に投げるようにYを設定
					m_moveSpeed.y = 5.0f;

					//キャラコンを初期化
					m_charaCon.Init(10.0f, 10.0f, m_pos);

					m_canPlayerMoveSpeedGet = false;
				}

				//移動速度減衰力を設定する
				m_decreasePower = m_moveSpeed;
				m_decreasePower *= -1.0f;
				//摩擦力を移動速度に加算する
				m_moveSpeed.x += m_decreasePower.x * g_gameTime->GetFrameDeltaTime();
				m_moveSpeed.z += m_decreasePower.z * g_gameTime->GetFrameDeltaTime();

				//重力
				Gravity();

				//キャラクターコントローラーを使った移動処理に変更。
				m_pos = m_charaCon.Execute(
					m_moveSpeed,
					1.0f,
					m_isHitGround,
					m_hitGroundNormal
				);

				//地面についたら削除
				if (!m_canPlayerMoveSpeedGet && m_charaCon.IsOnGround())
				{
					//プレイヤーの状態を回復初期化状態にする
					//このメソッドでモデルを削除する
					m_player[m_playerNum]->SetRecoveryState(enRecoveryDelete);
					//相方に当たらなかったら手持ちに戻す
					m_playerStatus[m_playerNum]->AddRecoveryItemNum();
				}
				//投げてないプレイヤーに回復アイテムが当たったら、
				if (!m_isSoloPlay)
				{
					if ((m_player[(m_playerNum + 1) % 2]->GetPosition() - m_pos).Length() < 150.0f)
					{
						//プレイヤーのHPがすでに最大の時、
						if (m_playerStatus[(m_playerNum + 1) % 2]->GetHitPoint() == m_playerStatus[(m_playerNum + 1) % 2]->GetMaxHitPoint())
						{
							//プレイヤーの状態を回復初期化状態にする
							//このメソッドでモデルを削除する
							m_player[m_playerNum]->SetRecoveryState(enRecoveryDelete);
							//相方に当たらなかったら手持ちに戻す
							m_playerStatus[m_playerNum]->AddRecoveryItemNum();

							return;
						}

						//ヒットポイントをプレイヤーの回復力分回復させる
						m_playerStatus[(m_playerNum + 1) % 2]->AddHitPoint(m_recoveryPower);
						m_playerStatus[(m_playerNum + 1) % 2]->AddRecoveryReceiveNum(m_recoveryPower);

						//プレイヤーにヒットしたフラグを立てる
						m_playerHit = true;

						//プレイヤーの状態を回復初期化状態にする
						//このメソッドでモデルを削除する
						m_player[m_playerNum]->SetRecoveryState(enRecoveryDelete);
					}
				}
				//めっちゃ真下に落下したら自動削除する
				//これをしないとプレイヤーが動けなくなり、
				//プレイヤーのストレスになるため。
				m_fallTimer++;
				if (m_fallTimer > 100/m_onlineUpdateSpeed->GetSpeed())
				{
					//プレイヤーの状態を回復初期化状態にする
					//このメソッドでモデルを削除する
					m_player[m_playerNum]->SetRecoveryState(enRecoveryDelete);
				}
			}

			break;
		}
		//データを更新
		DataUpdate();
	}

	//回復アイテムモデルの初期化
	void RecoveryItem::RecoveryItemInit()
	{
		//回復アイテムモデルを初期化
		//モデルを初期化
		m_model = NewGO<SkinModelRender>();
		m_model->SetShadowCaster(false);	//影を落とさない
		m_model->Init("RecoveryItem");
		//位置をプレイヤーの頭上に持っていく
		m_pos = m_player[m_playerNum]->GetPosition();
		m_pos.y += 140.0f;
		//モデルの位置を更新
		m_model->SetPosition(m_pos);
	}

	//更新処理メソッド
	void RecoveryItem::DataUpdate()
	{
		//位置
		m_model->SetPosition(m_pos);
		//回転角度
		m_rotAngle = atan2(m_moveSpeed.x, m_moveSpeed.z);
		//回転
		m_rot.SetRotation(Vector3::AxisY, m_rotAngle);
		m_model->SetRotation(m_rot);
	}
};