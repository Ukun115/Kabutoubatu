/// <summary>
/// 敵の基底クラス
/// </summary>
#include "stdafx.h"
#include "EnemyBase.h"
#include "../Player/Player.h"
#include "../Player/PlayerStatus.h"
#include "../AccompanyAI/AccompanyAI.h"
#include "../Camera/PlayerCamera.h"
#include "../Item/Weapon/Arrow.h"
#include "../Item/Weapon/Weapon.h"
#include "../Online/OnlineUpdateSpeed.h"

namespace nsKabutoubatu
{
	bool EnemyBase::Start()
	{
		m_onlineUpdateSpeed = FindGO< OnlineUpdateSpeed>(nsStdafx::ONLINEUPDATESPEED_NAME);

		//インスタンスを検索
		for (int playerNum = enPlayer1; playerNum < m_playerNum; playerNum++)
		{
			//プレイヤー
			m_player[playerNum] = FindGO<Player>(nsStdafx::PLAYER_NAME[playerNum]);
			//プレイヤーステータス
			m_playerStatus[playerNum] = FindGO<PlayerStatus>(nsStdafx::PLAYER_STATUS_NAME[playerNum]);
			//武器
			m_weapon[playerNum] = FindGO<Weapon>(nsStdafx::WEAPON_NAME[playerNum]);
		}
		//ソロプレイの時、
		if (m_playerNum == 1)
		{
			//同行AI(おとも)
			m_accompanyAI = FindGO<AccompanyAI>(nsStdafx::ACCOMPANYAI_NAME);
		}
		//プレイヤーカメラ
		m_playerCamera = FindGO<PlayerCamera>(nsStdafx::PLAYER_CAMERA_NAME);

		//敵個別のスタートメソッド処理
		SubStart();

		return true;
	}

	EnemyBase::~EnemyBase()
	{

	}

	void EnemyBase::Update()
	{
		//攻撃を受ける処理
		AttackReceive();

		//触れたプレイヤーにダメージを与える処理
		HitBody();

		//敵個別のアップデート
		SubUpdate();

		for (int playerNum = enPlayer1; playerNum < m_playerNum; playerNum++)
		{
			if (m_accompanyAI != nullptr)
			{
				if (m_accompanyAI->GetNowState() != 2)
				{
					//プレイヤーとの距離の長さを取得
					m_playerToEnemyVector = m_player[playerNum]->GetPosition() - m_pos;
					m_DistanceLength = m_playerToEnemyVector.Length();
					//プレイヤーの近くに敵がいて、かつ守備モードの時、プレイヤーを守りにいく。
					if (m_DistanceLength < 200 && m_accompanyAI->GetMode() == 1)
					{
						////同行AIが盾プッシュアタック攻撃をしてくる
						//Vector3 m_dir;
						//m_dir = m_pos - m_accompanyAI->GetPosition();
						//m_accompanyAI->SetShieldPushDashDirection(m_dir);
					}
				}
			}
		}

		m_cameraTargetPos = m_playerCamera->GetCameraTarget();

		if (m_receiveDamageFlg)
		{
			//ダメージを受けた時の赤色
			m_model->SetDameageRed(true);
			m_damageTimer++;
			m_receiveDamageFlg = false;
		}
		//ダメージタイマー起動
		if (m_damageTimer > 0)
		{
			m_damageTimer++;
		}
		//タイマーが10になったら、
		if (m_damageTimer > 10/ m_onlineUpdateSpeed->GetSpeed())
		{
			//ダメージを受けた時の赤色をなくす
			m_model->SetDameageRed(false);
			m_damageTimer = 0;
		}
	}

	//プレイヤーの攻撃を受ける処理
	void EnemyBase::AttackReceive()
	{
		//死亡したら攻撃を受けない
		if (m_hitPoint == 0)
		{
			m_receiveDamageFlg = false;

			return;
		}

		//プレイヤーの人数分実行
		for (int playerNum = enPlayer1; playerNum < m_playerNum; playerNum++)
		{
			//攻撃中じゃないときは実行しない。
			if (!m_player[playerNum]->IsAttack())
			{
				//攻撃判定タイマーを初期化
				m_attackJudgeTimer[playerNum] = 0;

				continue;
			}

			//弓の時は攻撃は発射した矢にあるので実行しない
			if (m_playerStatus[playerNum]->GetHaveWeapon() == 3)
			{
				//矢との当たり判定
				QueryGOs<Arrow>(nsStdafx::ARROW_NAME, [&](Arrow* m_arrow)->bool
				{
					if ((m_pos - m_arrow->GetPosition()).Length() < m_attackReceiveLength)
					{
						//攻撃を受ける
						m_receiveDamageFlg = true;

						m_lastAttackPlayer = playerNum;
						//矢を消す
						DeleteGO(m_arrow);
						//ヌルポインタを入れておく。
						m_arrow = nullptr;

						return true;
					}
					else
					{
						m_receiveDamageFlg = false;
						return false;
					}
				});

				continue;
			}

			//剣を振った瞬間に攻撃が当たらないようにディレイをかける
			//攻撃判定タイマーを進める
			m_attackJudgeTimer[playerNum]++;
			if (m_attackJudgeTimer[playerNum] == m_weapon[playerNum]->GetAttackJudgeTimer()/ m_onlineUpdateSpeed->GetSpeed())
			{
				//プレイヤーとの距離の長さを取得
				m_playerToEnemyVector = m_pos - m_player[playerNum]->GetPosition();
				m_DistanceLength = m_playerToEnemyVector.Length();
				//距離が近かったら
				if (m_DistanceLength < m_attackReceiveLength)
				{
					//正規化
					m_playerToEnemyVector.Normalize();

					//プレイヤーの正面のベクトルと、エネミーからプレイヤーに向かうベクトルの。
					//内積(cosθ)を求める。
					float cos = m_player[playerNum]->GetForwardVector().Dot(m_playerToEnemyVector);
					//内積(cosθ)から角度(θ)を求める。
					float angle = acosf(cos);
					//角度(θ)が80°より小さければ。
					if (angle <= (Math::PI / 180.0f) * 80.0f)
					{
						//攻撃を受ける
						m_receiveDamageFlg = true;
						m_lastAttackPlayer = playerNum;
					}
				}
				else
				{
					//攻撃を受けない
					m_receiveDamageFlg = false;
				}
			}
			else
			{
				//攻撃を受けない
				m_receiveDamageFlg = false;
			}
		}
	}

	//プレイヤーと接触するとダメージを与える処理
	void EnemyBase::HitBody()
	{
		if (!m_canHitBody)
		{
			return;
		}
		//プレイヤーが触れたらプレイヤーにダメージを与える！
		for (int playerNum = 0; playerNum < m_playerNum; playerNum++)
		{
			//ゴースト状態以外のプレイヤーに対して行う
			if (m_player[playerNum]->GetNowState() == 2)
			{
				continue;
			}
			//距離を取得
			m_dir[playerNum] = m_player[playerNum]->GetPosition() - m_pos;
			//プレイヤーとの距離が近かったら
			if (m_dir[playerNum].Length() < m_hitBodyLength)
			{
				//正規化
				m_dir[playerNum].Normalize();
				//吹っ飛ばしパワーにする
				m_dir[playerNum] *= 10.0f;

				//プレイヤーに移動速度(&移動方向)を渡す
				m_player[playerNum]->SetMoveSpeed(m_dir[playerNum]);
				//プレイヤーの状態を攻撃を受けている状態にする
				m_player[playerNum]->SetNowState(1);
				//プレイヤーのヒットポイントを１減らす
				m_playerStatus[playerNum]->ReduceHitPoint(1);
				//
				m_player[playerNum]->InitDamageTimer();
			}
		}
	}

	void EnemyBase::Gravity()
	{
		m_moveSpeed.y -= nsStdafx::GRAVITY * m_onlineUpdateSpeed->GetSpeed();
	}
}