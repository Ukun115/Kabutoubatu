/// <summary>
/// ソロモード時の同行AI(以降「おとも」と呼ぶ)クラス
/// </summary>
#include "stdafx.h"
#include "AccompanyAI.h"
#include "../Player/Player.h"
#include <random>
#include "AccompanyAISword.h"
#include "AccompanyAIShield.h"
#include "AccompanyAIAnimation.h"

namespace nsKabutoubatu
{
	namespace nsAccompanyAI
	{
		//おともの初期位置
		const Vector3 ACCOMPANYAI_START_POS = { -50.0f,150.0f,0.0f };
		//キャラコンの半径
		const float CHARA_CON_RADIUS = 25.0f;
		//キャラコンの高さ
		const float CHARA_CON_HEIGHT = 30.0f;
		//移動力
		const float MOVE_POWER = 7.0f;
	}

	bool AccompanyAI::Start()
	{
		//インスタンスを検索
		//プレイヤー
		m_player = FindGO<Player>(nsStdafx::PLAYER_NAME[enPlayer1]);

		//アニメーション
		m_accompanyAIAnimation = NewGO<AccompanyAIAnimation>();

		//モデルを初期化
		m_otomoModel = NewGO<SkinModelRender>();
		m_otomoModel->SetShadowCaster(true);	//影を落とす
		m_otomoModel->SetSilhouette(true);	//シルエットを落とす
		m_otomoModel->SetOutline(true);//輪郭線をつける
		m_otomoModel->SetPlayerMode(enPlayer2);
		//Zアップに変更
		m_otomoModel->SetModelUpAxis(enModelUpAxisZ);
		m_otomoModel->Init("AccompanyAI", "AccompanyAI", m_accompanyAIAnimation->GetAnimationClips(), m_accompanyAIAnimation->GetAnimationNum());
		//初期位置を設定
		m_pos = nsAccompanyAI::ACCOMPANYAI_START_POS;
		m_otomoModel->SetPosition(m_pos);
		//キャラコンを初期化
		m_charaCon.Init(nsAccompanyAI::CHARA_CON_RADIUS, nsAccompanyAI::CHARA_CON_HEIGHT, m_pos);

		//剣
		m_sword = NewGO<AccompanyAISword>();
		//盾
		m_shield = NewGO<AccompanyAIShield>();

		return true;
	}

	AccompanyAI::~AccompanyAI()
	{
		//モデルを削除
		DeleteGO(m_otomoModel);
		//剣削除
		DeleteGO(m_sword);
		//盾削除
		DeleteGO(m_shield);
		//アニメーション
		DeleteGO(m_accompanyAIAnimation);
	}

	void AccompanyAI::Update()
	{
		switch (m_state)
		{
		case enStopping:
			m_moveSpeed = Vector3::Zero;
			m_accompanyAIAnimation->ChangeAnimation(enAnimationClip_idle);

			break;

		//プレイヤーの近くに立っている状態
		case enIdle:

			switch (m_attackAndDefenseMode)
			{
			//攻撃モードの時、
			case enAttackMode:
				//アイドルアニメーションにする
				m_accompanyAIAnimation->ChangeAnimation(enAnimationClip_idle);
				break;
			//守備モードの時、
			case enDefenseMode:
				//守備アイドルアニメーションにする
				m_accompanyAIAnimation->ChangeAnimation(enAnimationClip_shieldIdle);
				break;
			}

			//プレイヤーとの距離を計算
			m_playerPos = m_player->GetPosition();
			m_playerToMeLength = (m_playerPos - m_pos).Length();
			//プレイヤーとの距離が遠くなったら、
			if (m_playerToMeLength > 300.0f)
			{
				//同行する状態に移行
				m_state = enAccompany;

				m_isPlayerGroundWalk = false;
			}

			if (!m_isPlayerGroundWalk)
			{
				m_randomTimer++;
				//60フレームごとに、実行
				if (m_randomTimer % 60 == 0)
				{
					//プレイヤーを中心とする半径150の円周上にある点をランダムで取得し、その点に向かうために移動方向にする
					m_moveSpeed = PlayerAroundPos(150) - m_pos;
					//とりあえず正規化
					m_moveSpeed.Normalize();
					//移動力をかける
					m_moveSpeed *= 3.0f;

					//タイマーを初期化
					m_randomTimer = 0;

					//プレイヤーの周りを歩いているフラグをオンにする
					m_isPlayerGroundWalk = true;
				}
			}
			else
			{
				switch (m_attackAndDefenseMode)
				{
				//攻撃モードの時、
				case enAttackMode:
					//歩きアニメーションにする
					m_accompanyAIAnimation->ChangeAnimation(enAnimationClip_walk);
					break;
				//守備モードの時、
				case enDefenseMode:
					//守備歩きアニメーションにする
					m_accompanyAIAnimation->ChangeAnimation(enAnimationClip_shieldWalk);
					break;
				}

				//指定された位置まで来たら止まる  or  動く範囲から外れると止まる
				if ((m_pos - m_circleSetPos).Length() < 20.0f || m_playerToMeLength > 300.0f)
				{
					//プレイヤーの周りを歩いているフラグをオフにする
					m_isPlayerGroundWalk = false;

					m_moveSpeed *= 0.00001f;
				}
			}

			break;

		//同行する状態
		case enAccompany:

			//走りアニメーションにする
			m_accompanyAIAnimation->ChangeAnimation(enAnimationClip_run);

			//プレイヤーに伸びるベクトル(正規化)を計算
			m_playerPos = m_player->GetPosition();
			m_playerToMeDirection = m_playerPos - m_pos;
			m_playerToMeLength = m_playerToMeDirection.Length();
			m_playerToMeDirection.Normalize();
			//おともの移動速度にする
			m_moveSpeed.x = m_playerToMeDirection.x * nsAccompanyAI::MOVE_POWER;
			m_moveSpeed.z = m_playerToMeDirection.z * nsAccompanyAI::MOVE_POWER;

			//プレイヤーとの距離が近くなったら、
			if (m_playerToMeLength < 200.0f)
			{
				//プレイヤーの近くに立っている状態に移行
				m_state = enIdle;
				m_moveSpeed *= 0.00001f;
			}

			//プレイヤーとの距離があきすぎたら、
			if (m_playerToMeLength > 1000.0f)
			{
				//プレイヤーの頭上にスポーン
				m_pos = m_player->GetPosition();
				m_pos.y += 200.0f;
				//キャラコンを削除
				m_charaCon.RemoveRigidBoby();
				//キャラコンを再初期化
				m_charaCon.Init(25.0f, 30.0f, m_pos);
				//体力を１減らす
			}

			break;

		case enShieldDashAttack:
			//守備モードじゃなくなったら、
			if (m_attackAndDefenseMode == enAttackMode)
			{
				//同行状態にする
				m_state = enAccompany;
			}

			break;

			//攻撃する状態
		case enAttack:
			if (g_pad[0]->IsTrigger(enButtonX))
			{

			}

			//攻撃③に移ったら、
			if (m_attackJumpTimer > 0)
			{
				m_attackJumpTimer++;
			}
			//30フレーム過ぎたら、
			if (m_attackJumpTimer == 30)
			{
				//ジャンプさせる
				m_moveSpeed.y += 7.0f;
				//ジャンプアタックタイマーを初期化する
				m_attackJumpTimer = 0;
			}

			break;

			//攻撃を受けた状態
		case enReceiveAttack:


			break;

			//死んでいる状態
		case enDeath:


			break;
		}

		//落下処理
		Fall();
		//重力
		Gravity();
		//データ更新処理
		DataUpdate();

		//おともの攻守モードを変更する処理
		if (g_pad[0]->IsTrigger(enButtonY))
		{
			switch (m_attackAndDefenseMode)
			{
			case enAttackMode:
				//守備モードに変更
				m_attackAndDefenseMode = enDefenseMode;
				break;
			case enDefenseMode:
				//攻撃モードに変更
				m_attackAndDefenseMode = enAttackMode;
				break;
			}
		}
	}

	void AccompanyAI::UiUpdate()
	{
		//アイドルアニメーションにする
		m_accompanyAIAnimation->ChangeAnimation(enAnimationClip_idle);
		m_otomoModel->PlayAnimation(m_accompanyAIAnimation->GetAnimationState());
	}

	//落下処理メソッド
	void AccompanyAI::Fall()
	{
		//おともがステージから落下したとき、
		if (-600.0f < m_pos.y && m_pos.y < -500.0f)
		{
			m_pos = m_player->GetPosition();
			//キャラコンを削除
			m_charaCon.RemoveRigidBoby();

			//キャラコンを再初期化
			m_charaCon.Init(25.0f, 30.0f, m_pos);
			//体力を１減らす
			m_hitPoint--;
		}
	}

	//おとものデータを更新するメソッド
	//位置・回転・キャラコン・アニメーション状態
	void AccompanyAI::DataUpdate()
	{
		//ヒットポイントの補正
		m_hitPoint = min(10, m_hitPoint);	//ヒットポイントが最大体力以上にならないようにする
		m_hitPoint = max(0, m_hitPoint);		//ヒットポイントが0以下にならないようにする

		//アニメーションの状態更新
		switch (m_accompanyAIAnimation->GetAnimationState())
		{
		//攻撃の時のみアニメーション補完をなくす。
		case enAnimationClip_attack1:
		case enAnimationClip_attack2:
		case enAnimationClip_attack3:
			m_otomoModel->PlayAnimation(m_accompanyAIAnimation->GetAnimationState(),0.0f);
			break;
			//通常はアニメーション補完あり。(0.1s補完)
		default:
			m_otomoModel->PlayAnimation(m_accompanyAIAnimation->GetAnimationState(),0.1f);
			break;
		}

		//キャラクターコントローラーを使った移動処理に変更。
		m_pos = m_charaCon.Execute(
			m_moveSpeed,
			1.0f,
			m_isHitGround,
			m_hitGroundNormal
		);
		//位置を更新
		m_otomoModel->SetPosition(m_pos);
		//回転角度を求める
		m_rotAngle = atan2(m_moveSpeed.x, m_moveSpeed.z);
		if (m_state == enStopping)
		{
			m_rotAngle = -3.0f;
		}
		//回転を更新
		m_rot.SetRotation(Vector3::AxisY, m_rotAngle);
		m_otomoModel->SetRotation(m_rot);
	}

	//プレイヤーを中心とする、半径(第一引数)の円周上の点をランダムに取得するメソッド
	Vector3 AccompanyAI::PlayerAroundPos(const float r)
	{
		//0～360度をランダムで取得
		std::random_device random;
		m_randomAngle = random() % 360;

		//X座標を求める
		m_circleSetPos.x = float(r*cos(m_randomAngle));
		m_circleSetPos.x += m_player->GetPosition().x;
		//Y座標はなし
		m_circleSetPos.y = 0;
		//Z座標を求める
		m_circleSetPos.z = float(r*sin(m_randomAngle));
		m_circleSetPos.z += m_player->GetPosition().z;

		return m_circleSetPos;
	}

	void AccompanyAI::SetShieldPushDashDirection(Vector3& moveSpeed)
	{
		m_state = enShieldDashAttack;
		m_moveSpeed = moveSpeed;
		m_moveSpeed *= 2.0f;
	};

	//おともの位置を移動させる
	void AccompanyAI::SetPosition(const Vector3& position)
	{
		m_pos = position;
		//キャラコンを一旦破棄
		m_charaCon.RemoveRigidBoby();
		//キャラコンを再初期化
		m_charaCon.Init(25.0f, 30.0f,m_pos);
	};
}