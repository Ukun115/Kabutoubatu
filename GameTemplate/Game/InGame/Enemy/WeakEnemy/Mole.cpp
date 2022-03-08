/// <summary>
/// モグラ(ザコ敵)クラス
/// </summary>
#include "stdafx.h"
#include "Mole.h"
#include "../../Player/Player.h"
#include "../../Player/PlayerStatus.h"
#include "../DropCoin.h"
#include "../../Online/OnlineUpdateSpeed.h"

namespace nsKabutoubatu
{
	namespace nsMole
	{
		//初期位置
		const Vector3 WEAKENEMY_START_POS = { 0.0f,0.0f,-300.0f };
		//移動力
		const float MOVE_POWER = 15.0f;
		//攻撃範囲
		const float ATTACK_RANGE = 400.0f;
		//攻撃時間
		const float ATTACK_TIME = 50.0f;
		//攻撃力
		const int ATTACK_POWER = 1;
		//攻撃遅延時間
		const float ATTACK_DELAY_TIME = 50.0f;
		//ランダム移動の移動力
		const float RANDOM_MOVE_POWER = 2.0f;
		//吹っ飛ばし力
		const float BLOWAWAY_POWER = 20.0f;
	}

	void Mole::SubStart()
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

		//敵の情報を初期化
		EnemyInit();

		//エクスクラメーションマーク画像を初期化
		m_exclamationMark = NewGO<SkinModelRender>();
		m_exclamationMark->Init("ExclamationMark");
		//初めは非表示
		m_exclamationMark->Deactivate();

		//正面ベクトルを計算
		m_forward = Vector3::AxisZ;
		m_rot.Apply(m_forward);

		//接触判定をなくす
		SetCanHitBody(false);

		m_randomDelayTimer = (rand() % 50);
		m_randomDelayTimer += 80;

		m_model->SetAnimationSpeed(m_onlineUpdateSpeed->GetSpeed());
	}

	Mole::~Mole()
	{
		//モデルを削除
		DeleteGO(m_model);
		//エクスクラメーションマークを削除
		DeleteGO(m_exclamationMark);
	}

	void Mole::SubUpdate()
	{
		switch (m_nowState)
		{
			//距離検索&ランダム移動状態
		case enPlayerSearchAndRandomMove:
			//距離検索
			DistanceSearch();
			//ランダム移動
			RandomMove();

			//歩きアニメーション再生
			m_model->SetAnimationSpeed(1.0f * m_onlineUpdateSpeed->GetSpeed());
			m_model->PlayAnimation(enWalkAnimation);

			break;
			//突進攻撃状態
		case enAttack:
			//攻撃(突進)するまでの遅延
			if (m_attackDelayTimer < nsMole::ATTACK_DELAY_TIME/ m_onlineUpdateSpeed->GetSpeed())
			{
				//突進攻撃状態になったとき、1回だけ実行される。
				if (m_attackDelayTimer == 0)
				{
					//TODO:発見サウンドを再生
					if ((m_cameraTargetPos - m_pos).Length() < nsStdafx::SOUND_CAN_HEARD_RANGE)
					{

					}
				}

				m_attackDelayTimer++;

				//エクスクラメーションマークの表示
				ExclamationMarkActive();

				//突進アニメーション再生
				m_model->SetAnimationSpeed(1.0f * m_onlineUpdateSpeed->GetSpeed());
				m_model->PlayAnimation(enTackleAnimation);
			}
			//攻撃遅延タイマーが終了
			else
			{
				//「発見！」のUIモデルを非表示
				m_exclamationMark->Deactivate();
				//攻撃(突進)
				Attack();
			}

			break;
			//プレイヤー吹っ飛ばし状態
		case enBlowAway:

			//このステートに来た時に１度のみ実行
			if (m_blowAwayAfterWaitTimer == 0)
			{
				//プレイヤーとぶつかった効果音を再生
				if ((m_cameraTargetPos - m_pos).Length() < nsStdafx::SOUND_CAN_HEARD_RANGE)
				{
					m_moleSound[enAttackSound] = NewGO<SoundSource>();
					m_moleSound[enAttackSound]->Init(L"Assets/sound/Enemy_Mole_Attack.wav");
					m_moleSound[enAttackSound]->SetVolume(0.3f);
					m_moleSound[enAttackSound]->Play(false);
				}

				//プレイヤーに吹っ飛ばす力を渡す
				PassBlowAwayDirection();
				//移動を止める
				m_moveSpeed = Vector3::Zero;
			}
			m_blowAwayAfterWaitTimer++;
			//吹っ飛ばし後の停止時間がオーバーしたら
			if (m_blowAwayAfterWaitTimer > 70/ m_onlineUpdateSpeed->GetSpeed())
			{
				//吹っ飛ばし後の停止時間を初期化
				m_blowAwayAfterWaitTimer = 0;
				//距離検索&ランダム移動状態にする
				m_nowState = enPlayerSearchAndRandomMove;
			}

			//Winアニメーション再生
			m_model->SetAnimationSpeed(1.0f * m_onlineUpdateSpeed->GetSpeed());
			m_model->PlayAnimation(enWinAnimation);

			break;

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
		if(GetReceiveAttackFlg())
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
			m_hitPoint = max(m_hitPoint,0);
		}

		//HPが０になったら死ぬ
		if (m_hitPoint == 0)
		{
			//死亡ステートに移行
			m_nowState = enDeath;

			//敵を倒した数を+1する
			m_playerStatus[GetLastKillPlayer()]->AddEnemyKillNum();

			//死亡アニメーション再生
			m_model->SetAnimationSpeed(2.0f * m_onlineUpdateSpeed->GetSpeed());
			m_model->PlayAnimation(enDeathAnimation);

			//「発見！」のUI(エクスクラメーションマーク)を表示されていたら非表示にする
			m_exclamationMark->Deactivate();

			m_moveSpeed /= 10000.0f;
		}

		//重力
		Gravity();

		//位置・回転・キャラコンを更新
		DataUpdate();

		//正面ベクトルを計算
		m_forward = Vector3::AxisZ;
		m_rot.Apply(m_forward);
	}

	void Mole::UiUpdate()
	{
		//歩きアニメーション
		m_model->PlayAnimation(enWalkAnimation);
	}

	//ランダム移動処理メソッド
	void Mole::RandomMove()
	{
		//攻撃射程外だとランダム移動
		if (m_plaToEneDistanceLength[m_nearPlayer] > nsMole::ATTACK_RANGE)
		{
			//ランダムに移動速度を決定
			if (m_randomMoveTimer % m_randomDelayTimer == 0)
			{
				//120フレームで移動方向を変更
				m_moveSpeed.x = (rand() % 100) / 100.0f;
				m_moveSpeed.x -= 0.5f;
				m_moveSpeed.z = (rand() % 100) / 100.0f;
				m_moveSpeed.z -= 0.5f;
				//正規化
				m_moveSpeed.Normalize();
				//移動速度をかける
				m_moveSpeed *= nsMole::RANDOM_MOVE_POWER * m_onlineUpdateSpeed->GetSpeed();

				m_randomDelayTimer = (rand() % 50);
				m_randomDelayTimer += 80;
				m_randomDelayTimer /= m_onlineUpdateSpeed->GetSpeed();

				m_randomMoveTimer = 0;
			}
			//移動タイマーをインクリメント
			m_randomMoveTimer++;
		}
	}

	//距離検索メソッド
	void Mole::DistanceSearch()
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
			m_plaToEneDistanceLength[playerNum] = m_playerToEnemyDistanceDirecion[playerNum].Length();
		}

		m_nearPlayer = enPlayer1;
		if (GetPlayerNum() == 2)
		{
			//１Pとの距離、２Pとの距離を比較し、近いほうのプレイヤー番号を保存
			if (m_plaToEneDistanceLength[enPlayer1] < m_plaToEneDistanceLength[enPlayer2])
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
		if(IsAttackRangeIn())
		{
			//攻撃状態にする
			m_nowState = enAttack;
			//向きをプレイヤーの方向に向かせる
			m_moveSpeed = m_playerToEnemyDistanceDirecion[m_nearPlayer];
		}
	}

	//攻撃(突進)処理メソッド
	void Mole::Attack()
	{
		//上方向に行かないように補正
		m_playerToEnemyDistanceDirecion[m_nearPlayer].y = min(0, m_playerToEnemyDistanceDirecion[m_nearPlayer].y);
		//移動速度にする
		m_moveSpeed = m_playerToEnemyDistanceDirecion[m_nearPlayer] * nsMole::MOVE_POWER * m_onlineUpdateSpeed->GetSpeed();

		//TODO:突進攻撃サウンドを再生
		if ((m_cameraTargetPos -m_pos).Length() < nsStdafx::SOUND_CAN_HEARD_RANGE)
		{

		}

		//攻撃時間をカウント
		m_attackTimer++;

		for (int playerNum = enPlayer1; playerNum < GetPlayerNum(); playerNum++)
		{
			//プレイヤーとの距離を求める
			m_playerToEnemyDistanceDirecion2[playerNum] = m_player[playerNum]->GetPosition() - m_pos;
			m_plaToEneDistanceLength[playerNum] = m_playerToEnemyDistanceDirecion2[playerNum].Length();
			//プレイヤーにぶつかったら、
			if (m_plaToEneDistanceLength[playerNum] < 70.0f)
			{
				//ゴースト状態のプレイヤーに対しては突進攻撃を行わない
				if (m_player[playerNum]->GetNowState() == 2)
				{
					continue;
				}

				//どちらのプレイヤーを吹っ飛ばすかを保存
				m_blowAwayPlayer = playerNum;
				//プレイヤー吹っ飛ばし状態にする
				m_nowState = enBlowAway;
				//攻撃遅延タイマーを初期化
				m_attackDelayTimer = 0;
				//
				m_player[playerNum]->InitDamageTimer();
			}
		}

		//攻撃時間が終わったら、
		if (m_attackTimer > nsMole::ATTACK_TIME/ m_onlineUpdateSpeed->GetSpeed())
		{
			//距離検索状態にする
			m_nowState = enPlayerSearchAndRandomMove;
			//攻撃時間を初期化
			m_attackTimer = 0;
			//攻撃遅延タイマーを初期化
			m_attackDelayTimer = 0;
			//移動速度を下げる
			m_moveSpeed.Normalize();
		}
	}

	//更新処理メソッド
	void Mole::DataUpdate()
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

	//エクスクラメーションマークの表示処理メソッド
	void Mole::ExclamationMarkActive()
	{
		//エクスクラメーションマークを弱敵の頭上に置く
		m_exclamationMarkPos = { m_pos.x-40.0f,m_pos.y+70.0f,m_pos.z };
		m_exclamationMark->SetPosition(m_exclamationMarkPos);
		//「発見！」のUI(エクスクラメーションマーク)を表示
		m_exclamationMark->Activate();
	}

	//弱敵の吹っ飛ばす方向を渡す処理メソッド
	void Mole::PassBlowAwayDirection()
	{
		//進んでいる方向(吹っ飛ばす方向)を正規化
		m_moveSpeed.Normalize();
		//吹っ飛ばす力に変える
		m_moveSpeed *= nsMole::BLOWAWAY_POWER;
		m_moveSpeed.y = 7.5f;
		//プレイヤーに移動速度(&移動方向)を渡す
		m_player[m_blowAwayPlayer]->SetMoveSpeed(m_moveSpeed);
		//プレイヤーの状態を攻撃を受けている状態にする
		m_player[m_blowAwayPlayer]->SetNowState(1);
		//プレイヤーのヒットポイントを１減らす
		m_playerStatus[m_blowAwayPlayer]->ReduceHitPoint(nsMole::ATTACK_POWER);
	}

	//敵の情報を初期化するメソッド
	void Mole::EnemyInit()
	{
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
		m_animationClips[enWalkAnimation].Load("enemy_mole_walk");		//歩きモーションをロード
		m_animationClips[enWalkAnimation].SetLoopFlag(true);	//ループモーションにする。
		m_animationClips[enTackleAnimation].Load("enemy_mole_tackle");		//突進モーションをロード
		m_animationClips[enTackleAnimation].SetLoopFlag(true);	//ループモーションにする。
		m_animationClips[enWinAnimation].Load("enemy_mole_win");		//勝利モーションをロード
		m_animationClips[enWinAnimation].SetLoopFlag(false);	//ワンショット再生にする。
		m_animationClips[enDeathAnimation].Load("enemy_mole_death");		//死亡モーションをロード
		m_animationClips[enDeathAnimation].SetLoopFlag(false);	//ワンショット再生にする。
		m_model->Init("enemy_mole","enemy_mole", m_animationClips, enAnimationNum);
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
		//初期状態はプレイヤーを探している状態
		m_nowState = enPlayerSearchAndRandomMove;
	}

	//攻撃範囲内に入ったらフラグオンするメソッド
	bool Mole::IsAttackRangeIn()
	{
		//プレイヤーとの距離が近かったら、
		if (m_plaToEneDistanceLength[m_nearPlayer] < nsMole::ATTACK_RANGE)
		{
			//エネミーの正面のベクトルと、エネミーからプレイヤーに向かうベクトルの。
			//内積(cosθ)を求める。
			float cos = m_forward.Dot(m_playerToEnemyDistanceDirecion[m_nearPlayer]);
			//内積(cosθ)から角度(θ)を求める。
			float angle = acosf(cos);
			//角度(θ)が120°より小さければ。
			if (angle <= (Math::PI / 180.0f) * 120.0f)
			{
				//プレイヤーが攻撃範囲内にいる
				return true;
			}
			else
			{
				//プレイヤーが攻撃範囲外にいる
				return false;
			}
		}
		else
		{
			return false;
		}
	}
};