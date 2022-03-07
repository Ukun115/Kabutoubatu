/// <summary>
/// スライム(ザコ敵)クラス
/// </summary>
#include "stdafx.h"
#include "Slime.h"
#include "../../Player/Player.h"
#include "../../Player/PlayerStatus.h"
#include "../DropCoin.h"
#include "../../Online/OnlineUpdateSpeed.h"

namespace nsKabutoubatu
{
	void Slime::SubStart()
	{
		//HPを設定
		m_hitPoint = 4;

		m_onlineUpdateSpeed = FindGO<OnlineUpdateSpeed>(nsStdafx::ONLINEUPDATESPEED_NAME);

		//プレイヤーのインスタンスを検索
		for (int playerNum = enPlayer1; playerNum < GetPlayerNum(); playerNum++)
		{
			m_player[playerNum] = FindGO<Player>(nsStdafx::PLAYER_NAME[playerNum]);
			m_playerStatus[playerNum] = FindGO<PlayerStatus>(nsStdafx::PLAYER_STATUS_NAME[playerNum]);
		}

		//攻撃を受ける範囲
		SetAttackReceiveLength(220);
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
		//アニメーションをロード
		m_animationClips[enKyoroKyoro].Load("enemy_slime_kyorokyoro");		//キョロキョロモーションをロード
		m_animationClips[enKyoroKyoro].SetLoopFlag(false);	//ワンショット再生
		m_animationClips[enJump].Load("enemy_slime_jump");		//ジャンプモーションをロード
		m_animationClips[enJump].SetLoopFlag(true);	//ループ再生
		m_animationClips[enDeath].Load("enemy_slime_death");		//死亡モーションをロード
		m_animationClips[enDeath].SetLoopFlag(false);	//ワンショット再生
		m_model->Init("enemy_slime", "enemy_slime", m_animationClips, enAnimationNum);
		//モデルの位置を更新
		m_model->SetPosition(m_pos);
		//キャラコンを初期化
		m_charaCon.Init(30.0f, 85.0f, m_pos);
		//キャラクターコントローラーを使った移動処理に変更。
		m_pos = m_charaCon.Execute(
			m_moveSpeed,
			1.0f,
			m_isHitGround,
			m_hitGroundNormal
		);

		m_model->SetAnimationSpeed(m_onlineUpdateSpeed->GetSpeed());
		//キョロキョロアニメーション再生
		m_model->PlayAnimation(enKyoroKyoro);
	}

	Slime::~Slime()
	{
		//モデルを削除
		DeleteGO(m_model);
	}

	void Slime::SubUpdate()
	{
		switch (m_nowState)
		{
		case enKyoroKyoro:
			//キョロキョロアニメーション再生
			m_model->PlayAnimation(enKyoroKyoro);
			//キョロキョロアニメーションが終わったら、
			if (!m_model->IsPlaying())
			{
				//ジャンプステートに移行
				m_nowState = enJump;
			}

			break;

		case enJump:
			//ジャンプアニメーション再生
			m_model->PlayAnimation(enJump);
			//地面についていたらジャンプ
			if (m_charaCon.IsOnGround())
			{
				//ジャンプ開始の時、
				if (m_jumpStartFlg)
				{
					//ジャンプ
					Jump();
					//ジャンプスタートかどうかフラグを折る
					m_jumpStartFlg = false;
				}
				//着地の時、
				else
				{
					//TODO:着地効果音を再生
					if ((m_cameraTargetPos - m_pos).Length() < nsStdafx::SOUND_CAN_HEARD_RANGE)
					{
						m_slimeSound[enAttackSound] = NewGO<SoundSource>();
						m_slimeSound[enAttackSound]->Init(L"Assets/sound/Enemy_Slime_Jump.wav");
						m_slimeSound[enAttackSound]->SetVolume(0.2f);
						m_slimeSound[enAttackSound]->Play(false);
					}

					//キョロキョロステートに移行
					m_nowState = enKyoroKyoro;
					//ジャンプスタートかどうかフラグを戻す
					m_jumpStartFlg = true;
					//正規化
					m_moveSpeed.Normalize();
					m_moveSpeed /= 100.0f;
				}
			}

			break;

		case enDeath:
			if (!m_model->IsPlaying())
			{
				//クラスを破棄
				DeleteGO(this);
				//コインを落とすようにする
				m_dropCoin = NewGO<DropCoin>();
				m_dropCoin->SetPlayerNumber(GetPlayerNum());
				//コインを落とす場所をモデルの位置に設定する
				m_dropCoin->SetPosition(m_pos);
			}

			break;
		}

		//ステージから落下したとき、
		if (-600.0f < m_pos.y && m_pos.y < -500.0f)
		{
			m_isFall = true;
		}

		//攻撃を受けた時、
		if (GetReceiveAttackFlg())
		{
			//ヒットポイントを減らす
			switch (m_playerStatus[m_lastAttackPlayer]->GetHaveWeapon())
			{
			case enWoodStick:
				m_hitPoint -= 1;
				break;
			case enSword:
				m_hitPoint -= 2;
				break;
			case enLargeSword:
				m_hitPoint -= 4;
				break;
			case enArow:
				m_hitPoint -= 1;
				break;
			}
			//マイナスにならないようにする
			m_hitPoint = max(m_hitPoint, 0);
		}

		//HPが０になったら死ぬ
		if (m_hitPoint == 0)
		{
			//死亡ステートに移行
			m_nowState = enDeath;
			//敵を倒した数を+1する
			m_playerStatus[GetLastKillPlayer()]->AddEnemyKillNum();
			//死亡アニメーション再生
			m_model->SetAnimationSpeed(1.5f * m_onlineUpdateSpeed->GetSpeed());
			m_model->PlayAnimation(enDeath);

			m_moveSpeed /= 10000.0f;

			//接触範囲をセット
			SetHitBodyLength(1.0f);
		}

		//重力
		Gravity();

		//位置・回転・キャラコンを更新
		DataUpdate();
	}

	void Slime::UiUpdate()
	{
		m_model->PlayAnimation(enKyoroKyoro);
	}

	//更新処理メソッド
	void Slime::DataUpdate()
	{
		//モデルの位置を更新
		m_model->SetPosition(m_pos);
		//回転角度を求める
		m_rotAngle = atan2(m_moveSpeed.x, m_moveSpeed.z);
		//回転を更新
		m_rot.SetRotation(Vector3::AxisY, m_rotAngle);
		m_model->SetRotation(m_rot);
		//キャラクターコントローラーを使った移動処理に変更。
		m_pos = m_charaCon.Execute(
			m_moveSpeed,
			1.0f,
			m_isHitGround,
			m_hitGroundNormal
		);

		//攻撃を受ける範囲
		SetAttackReceiveLength(220);
		//位置をセット
		SetPosition(m_pos);
	}

	//ジャンプメソッド
	void Slime::Jump()
	{
		//移動方向をランダム
		m_moveSpeed.x = (rand() % 100) / 100.0f;
		m_moveSpeed.x -= 0.5f;
		m_moveSpeed.z = (rand() % 100) / 100.0f;
		m_moveSpeed.z -= 0.5f;
		//正規化
		m_moveSpeed.Normalize();
		//移動速度をかける
		m_moveSpeed *= 5.0f * m_onlineUpdateSpeed->GetSpeed();

		m_moveSpeed.y = m_jumpPower * m_onlineUpdateSpeed->GetSpeed();
	}
}