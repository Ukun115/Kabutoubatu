 /// <summary>
 /// とげスライム(ザコ敵)の遠距離攻撃のとげクラス
 /// </summary>
#include "stdafx.h"
#include "Needle.h"
#include "../../Player/Player.h"
#include "../../Player/PlayerStatus.h"
#include "../../Online/OnlineUpdateSpeed.h"

namespace nsKabutoubatu
{
	namespace nsNeedle
	{
		//とげの移動力
		const float NEEDLE_SPEED = 8.0f;
		//とげの接触範囲
		const float NEEDLE_HIT_RANGE = 80.0f;
	}

	void Needle::SubStart()
	{
		m_onlineUpdateSpeed = FindGO<OnlineUpdateSpeed>(nsStdafx::ONLINEUPDATESPEED_NAME);

		//接触判定をなくす
		SetCanHitBody(false);

		//プレイヤーのインスタンスを検索
		for (int playerNum = enPlayer1; playerNum < GetPlayerNum(); playerNum++)
		{
			m_player[playerNum] = FindGO<Player>(nsStdafx::PLAYER_NAME[playerNum]);
			m_playerStatus[playerNum] = FindGO<PlayerStatus>(nsStdafx::PLAYER_STATUS_NAME[playerNum]);
		}

		//攻撃を受ける範囲
		SetAttackReceiveLength(1);
		//位置をセット
		SetPosition(m_pos);
		//接触範囲をセット
		SetHitBodyLength(80.0f);

		//モデルを初期化
		m_model = NewGO<SkinModelRender>();
		//モデルの上方向をZアップに指定
		m_model->SetModelUpAxis(enModelUpAxisZ);
		m_model->SetShadowCaster(true);	//影を落とす
		m_model->SetSilhouette(true);	//シルエットを落とす
		m_model->SetPlayerMode(2);	//敵用のシルエット
		m_model->SetOutline(true);//輪郭線をつける

		m_model->Init("enemy_needleSlime_needle");
		//モデルの位置を更新
		m_model->SetPosition(m_pos);
		m_moveSpeed *= nsNeedle::NEEDLE_SPEED;
		m_moveSpeed *= m_onlineUpdateSpeed->GetSpeed();
		m_moveSpeed.y = 0.0f;

		//とげの向きをセット
		//回転角度を求める
		float m_rotAngle = atan2(m_moveSpeed.x, m_moveSpeed.z);
		//回転を更新
		Quaternion m_rot;
		m_rot.SetRotation(Vector3::AxisY, m_rotAngle);
		m_model->SetRotation(m_rot);
	}

	Needle::~Needle()
	{
		//モデルを削除
		DeleteGO(m_model);
	}

	void Needle::SubUpdate()
	{
		m_timer++;

		if (m_timer < 20/ m_onlineUpdateSpeed->GetSpeed())
		{
			return;
		}
		if (m_timer == 20/m_onlineUpdateSpeed->GetSpeed())
		{
			m_needleSlimeSound[enAttackSound] = NewGO<SoundSource>();
			m_needleSlimeSound[enAttackSound]->Init(L"Assets/sound/Enemy_NeedleSlime_Attack.wav");
			m_needleSlimeSound[enAttackSound]->SetVolume(0.3f);
			m_needleSlimeSound[enAttackSound]->Play(false);
		}

		if (m_timer > 60/ m_onlineUpdateSpeed->GetSpeed())
		{
			//とげ削除
			DeleteGO(this);
		}

		for (int playerNum = enPlayer1; playerNum < GetPlayerNum(); playerNum++)
		{
			//ゴースト状態の時は当たらない
			if (m_player[playerNum]->GetNowState() == 2)
			{
				continue;
			}

			//プレイヤーと接触したら、
			if ((m_player[playerNum]->GetPosition()-m_pos).Length() < nsNeedle::NEEDLE_HIT_RANGE)
			{
				//プレイヤーのヒットポイントを１減らす
				m_playerStatus[playerNum]->ReduceHitPoint(1);
				//プレイヤーの移動を止める
				Vector3 m_newPlayerMoveSpeed = m_player[playerNum]->GetMoveSpeed();
				m_newPlayerMoveSpeed /= 10000.0f;
				m_player[playerNum]->SetMoveSpeed(m_newPlayerMoveSpeed);
				//
				m_player[playerNum]->InitDamageTimer();

				//TODO:接触効果音を再生
				if ((m_cameraTargetPos - m_pos).Length() < nsStdafx::SOUND_CAN_HEARD_RANGE)
				{

				}

				//
				m_player[playerNum]->InitDamageTimer();

				//とげ削除
				DeleteGO(this);
			}
		}

		//モデルの位置を更新
		m_pos += m_moveSpeed;
		m_model->SetPosition(m_pos);
		//位置をセット
		SetPosition(m_pos);
	}
}