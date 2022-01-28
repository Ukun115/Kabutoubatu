/// <summary>
/// トゲスライム(ザコ敵)クラス
/// </summary>
#include "stdafx.h"
#include "NeedleSlime.h"
#include "../../Player/Player.h"
#include "../../Player/PlayerStatus.h"
#include "../DropCoin.h"
#include "Needle.h"


namespace nsKabutoubatu
{
	namespace nsNeedleSlime
	{
		//ランダム移動の移動力
		const float RANDOM_MOVE_POWER = 0.5f;
		//攻撃範囲
		const float ATTACK_RANGE = 500.0f;
	}

	void NeedleSlime::SubStart()
	{
		//HPを設定
		m_hitPoint = 4;

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
		m_animationClips[enIdle].Load("enemy_needleSlime_idle");		//アイドルモーションをロード
		m_animationClips[enIdle].SetLoopFlag(true);		//ループ再生
		m_animationClips[enWalk].Load("enemy_needleSlime_walk");		//歩きモーションをロード
		m_animationClips[enWalk].SetLoopFlag(true);		//ループ再生
		m_animationClips[enAttack].Load("enemy_needleSlime_attack");		//攻撃モーションをロード
		m_animationClips[enAttack].SetLoopFlag(false);	//ワンショット再生
		m_animationClips[enDeath].Load("enemy_needleSlime_death");		//死亡モーションをロード
		m_animationClips[enDeath].SetLoopFlag(false);	//ワンショット再生

		m_model->Init("enemy_needleSlime", "enemy_needleSlime", m_animationClips, enAnimationNum);
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
	}

	NeedleSlime::~NeedleSlime()
	{
		//モデルを削除
		DeleteGO(m_model);

		if (m_needle != nullptr)
		{
			DeleteGO(m_needle);
		}
	}

	void NeedleSlime::SubUpdate()
	{
		switch (m_nowState)
		{
		//アイドル中
		case enIdle:
			//地面についているときのみ実行
			if (m_charaCon.IsOnGround())
			{
				//向きをプレイヤーの方向に向かせる
				m_moveSpeed = m_playerToEnemyDistanceDirecion[m_nearPlayer];
				m_moveSpeed /= 10000.0f;
				m_idleTimer++;
				if (m_idleTimer > 160)
				{
					//歩き状態にする
					m_nowState = enWalk;
					//タイマーを初期化
					m_idleTimer = 0;
				}
			}

			break;

		//歩き中
		case enWalk:
			//距離検索
			DistanceSearch();
			//ランダム移動
			RandomMove();

			break;

		//攻撃中
		case enAttack:
			if (!m_model->IsPlaying())
			{
				m_nowState = enIdle;
			}
			break;

		//死亡中
		case enDeath:

			//接触範囲をセット
			SetHitBodyLength(1.0f);

			//死亡アニメーションが終わったら死ぬ
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

		//体力が0になったら、
		if (m_hitPoint == 0)
		{
			//敵を倒した数を+1する
			m_playerStatus[GetLastKillPlayer()]->AddEnemyKillNum();
			//死亡状態に遷移
			m_model->SetAnimationSpeed(4.0f);
			m_nowState = enDeath;
		}

		//重力
		Gravity();

		//位置・回転・キャラコンを更新
		DataUpdate();
	}

	void NeedleSlime::UiUpdate()
	{
		//歩きアニメーション
		m_model->PlayAnimation(enIdle);
	}

	//更新処理メソッド
	void NeedleSlime::DataUpdate()
	{
		//アニメーション再生
		m_model->PlayAnimation(m_nowState);

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

	//距離検索メソッド
	void NeedleSlime::DistanceSearch()
	{
		//プレイヤーとの距離を求める
		for (int playerNum = enPlayer1; playerNum < GetPlayerNum(); playerNum++)
		{
			//ゴースト状態のプレイヤーに対しては距離検索を行わない
			if (m_player[playerNum]->GetNowState() == 2)
			{
				//ゴースト状態のプレイヤーとの距離を果てしなく遠くする
				m_plaToEneDistanceLength[playerNum] = 9999999.9f;

				continue;
			}

			m_playerToEnemyDistanceDirecion[playerNum] = m_player[playerNum]->GetPosition() - m_pos;
			m_playerToEnemyDistanceDirecion[playerNum].y += 50.0f;
			m_plaToEneDistanceLength[playerNum] = m_playerToEnemyDistanceDirecion[playerNum].Length();
		}

		m_nearPlayer = enPlayer1;
		if (GetPlayerNum() == 2)
		{
			//１Pとの距離、２Pとの距離を比較し、近いほうのプレイヤー番号を保存
			if (m_plaToEneDistanceLength[enPlayer1] < m_plaToEneDistanceLength[enPlayerNum::enPlayer2])
			{
				m_nearPlayer = enPlayer1;
			}
			else
			{
				m_nearPlayer = enPlayer2;
			}
		}
		//一番近いプレイヤーから敵のベクトルを正規化して方向だけの情報にする
		m_playerToEnemyDistanceDirecion[m_nearPlayer].Normalize();

		//攻撃範囲に入ったら、
		if (IsAttackRangeIn())
		{
			//攻撃状態にする
			m_nowState = enAttack;
			//向きをプレイヤーの方向に向かせる
			m_moveSpeed = m_playerToEnemyDistanceDirecion[m_nearPlayer];
			m_moveSpeed /= 1000.0f;
			m_idleTimer = 0;

			//針を飛ばす
			m_needle = NewGO<Needle>();
			m_needle->SetPlayerNum(GetPlayerNum());
			m_needle->SetMoveDirection(m_moveSpeed*1000.0f);

			m_needleInitPos = Vector3::Zero;

			if(m_moveSpeed.x > 0)
			{
				m_needleInitPos.x = 70.0f;
			}
			else
			{
				m_needleInitPos.x = -70.0f;
			}

			//とげをとげスライムの前方から発射するようにする。一次方程式を利用。
			m_needleInitPos.z = ((m_moveSpeed.z*100)/(m_moveSpeed.x*100))*m_needleInitPos.x;
			m_needleInitPos += m_pos;
			m_needleInitPos.y += 15.0f;
			m_needle->SetInitPosition(m_needleInitPos);
		}
	}

	//ランダム移動処理メソッド
	void NeedleSlime::RandomMove()
	{
		//攻撃射程外だとランダム移動
		if (m_plaToEneDistanceLength[m_nearPlayer] > nsNeedleSlime::ATTACK_RANGE)
		{
			//ランダムに移動速度を決定
			if (m_randomMoveTimer % 240 == 0)
			{
				//120フレームで移動方向を変更
				m_moveSpeed.x = (rand() % 100) / 100.0f;
				m_moveSpeed.x -= 0.5f;
				m_moveSpeed.z = (rand() % 100) / 100.0f;
				m_moveSpeed.z -= 0.5f;
				//正規化
				m_moveSpeed.Normalize();
				//移動速度をかける
				m_moveSpeed *= nsNeedleSlime::RANDOM_MOVE_POWER;
			}
			//移動タイマーをインクリメント
			m_randomMoveTimer++;

			//壁に当たっていたら、
			if (m_charaCon.IsHitWall())
			{
				//ランダム移動タイマーを初期化
				m_randomMoveTimer = 0;
				//逆方向に移動するようにする
				m_moveSpeed *= -1.0f;
			}
		}
	}

	//攻撃範囲内に入ったらフラグオンするメソッド
	bool NeedleSlime::IsAttackRangeIn()
	{
		//空中にいるときは攻撃不可
		if (m_charaCon.IsJump())
		{
			return false;
		}

		//プレイヤーとの距離が近かったら、
		if (m_plaToEneDistanceLength[m_nearPlayer] < nsNeedleSlime::ATTACK_RANGE)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
}