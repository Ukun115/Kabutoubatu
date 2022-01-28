/// <summary>
/// 最初のボスクラス
/// </summary>
#include "stdafx.h"
#include "FirstBoss.h"
#include "../Player/Player.h"
#include "../Player/PlayerStatus.h"
#include "ConfusionStar.h"

namespace nsKabutoubatu
{
	namespace nsFirstBoss
	{
		//スポーンする初期座標
		const Vector3 INIT_POSITION = { 0.0f,-10000.0f,-400.0f };
		//ヒットポイン0トの最大値
		const float MAX_HITPOINT = 40;
		//ランダム移動の移動力
		const float RANDOM_MOVE_POWER = 2.0f;

		const char* HP_UI_NAME[2] = { "HPBar","HPBar_RemainingHP" };

		const Vector4   FONT_COLOR = { 1.0f,1.0f,1.0f,1.0f };	//制限時間フォントの色
		const float     FONT_ROT = 0.0f;			//フォントの傾き
		const Vector2   FONT_PIV = { 1.0f,1.0f };	//フォントの基点
		const float     FONT_SCA = 0.5f;			//フォントの拡大率

		const float ATTACK_RANGE = 200.0f;
	}

	void FirstBoss::SubStart()
	{
		//HPを設定
		m_hitPoint = 40;
		//攻撃を受ける範囲
		SetAttackReceiveLength(300.0f);
		//位置をセット
		SetPosition(nsFirstBoss::INIT_POSITION);
		//接触範囲をセット
		SetHitBodyLength(160.0f);

		//プレイヤーのインスタンスを検索
		for (int playerNum = enPlayer1; playerNum < GetPlayerNum(); playerNum++)
		{
			m_player[playerNum] = FindGO<Player>(nsStdafx::PLAYER_NAME[playerNum]);
			m_playerStatus[playerNum] = FindGO<PlayerStatus>(nsStdafx::PLAYER_STATUS_NAME[playerNum]);
		}

		for (int hpBarUINum = enHPBar; hpBarUINum < enHPBarUINum; hpBarUINum++)
		{
			m_hpBarUI[hpBarUINum] = NewGO<SpriteRender>();
			m_hpBarUI[hpBarUINum]->Init(nsFirstBoss::HP_UI_NAME[hpBarUINum], 500, 20);
			m_hpBarUI[hpBarUINum]->SetPosition({ -240.0f,200.0f,0.0f });
			//基点を画像の左上にする
			m_hpBarUI[hpBarUINum]->SetPivot({ 0.0f, 0.0f });
		}
		//HPバーの左側の敵名
		m_firstBossName = NewGO<FontRender>();
		//フォントの初期化
		m_firstBossName->Init
		(
			L"",			//テキスト
			{ -310.0f,225.0f },	//位置
			nsFirstBoss::FONT_COLOR,	//色
			nsFirstBoss::FONT_ROT,		//傾き
			nsFirstBoss::FONT_SCA,		//拡大率
			nsFirstBoss::FONT_PIV		//基点
		);
		//文字の境界線表示
		m_firstBossName->SetShadowParam(true, 1.0f, Vector4::Black);
		swprintf_s(m_firstBossNameText, L"Boss");
		m_firstBossName->SetText(m_firstBossNameText);



		//モデルを初期化
		m_model = NewGO<SkinModelRender>();
		//モデルの上方向をZアップに指定
		m_model->SetModelUpAxis(enModelUpAxisZ);
		m_model->SetShadowCaster(true);	//影を落とす
		m_model->SetSilhouette(true);	//シルエットを落とす
		m_model->SetPlayerMode(2);	//敵用のシルエット
		m_model->SetOutline(true);//輪郭線をつける
		//アニメーションをロード
		m_animationClips[enIdleAnimation].Load("enemy_firstBoss_idle");									//アニメーションをロード
		m_animationClips[enIdleAnimation].SetLoopFlag(true);											//ループ再生にする。
		m_animationClips[enWalkAnimation].Load("enemy_firstBoss_walk");									//歩きアニメーションをロード
		m_animationClips[enWalkAnimation].SetLoopFlag(true);											//ループ再生にする。
		m_animationClips[enRunAnimation].Load("enemy_firstBoss_run");									//走りアニメーションをロード
		m_animationClips[enRunAnimation].SetLoopFlag(true);												//ループ再生にする。
		m_animationClips[enAttackHornsUpAnimation].Load("enemy_firstBoss_Attack_HornsUp");				//角突き上げアニメーションをロード
		m_animationClips[enAttackHornsUpAnimation].SetLoopFlag(false);									//ワンショット再生にする。
		m_animationClips[enAttackHornsForwardAnimation].Load("enemy_firstBoss_Attack_HornsForward");	//角突きアニメーションをロード
		m_animationClips[enAttackHornsForwardAnimation].SetLoopFlag(false);								//ワンショット再生にする。
		m_animationClips[enDamageAnimation].Load("enemy_firstBoss_Damage");								//ダメージ受けたアニメーションをロード
		m_animationClips[enDamageAnimation].SetLoopFlag(false);											//ワンショット再生にする。
		m_animationClips[enDeathAnimation].Load("enemy_firstBoss_death");								//死亡アニメーションをロード
		m_animationClips[enDeathAnimation].SetLoopFlag(false);											//ワンショット再生にする。
		//敵のモデル情報を初期化
		m_model->Init("enemy_firstBoss", "enemy_firstBoss", m_animationClips, enAnimationNum);
		//モデルの位置を更新
		m_pos = nsFirstBoss::INIT_POSITION;
		m_model->SetPosition(m_pos);
		//キャラコンを初期化
		m_charaCon.Init(100.0f, 100.0f, m_pos);
		//キャラクターコントローラーを使った移動処理に変更。
		m_pos = m_charaCon.Execute(
			m_moveSpeed,
			1.0f,
			m_isHitGround,
			m_hitGroundNormal
		);
		//正面ベクトルを計算
		m_forward = Vector3::AxisZ;
		m_rot.Apply(m_forward);
	}

	FirstBoss::~FirstBoss()
	{
		//モデルの削除
		DeleteGO(m_model);
		//HPバーUIの削除
		for (int hpBarUINum = enHPBar; hpBarUINum < enHPBarUINum; hpBarUINum++)
		{
			DeleteGO(m_hpBarUI[hpBarUINum]);
		}

		DeleteGO(m_firstBossName);
	}

	void FirstBoss::SubUpdate()
	{
		switch(m_nowState)
		{
		//通常状態
		case enNormalState:
			//移動状態
			switch (m_moveState)
			{
				//ランダム移動状態
			case enRandomMoveState:
				//ランダム移動
				RandomMove();
				//プレイヤー位置検索
				DistanceSearch();

				break;

				//走り攻撃
			case enRunAttackState:
				m_moveStopTimer++;
				if (m_moveStopTimer >= 120)
				{
					if (m_moveStopTimer == 120)
					{
						m_moveSpeed *= 20.0f;
					}
					//接触判定をなくす
					SetCanHitBody(false);

					//壁に当たったら、
					if (m_charaCon.IsHitWall())
					{
						if (m_animState != enIdleAnimation)
						{
							//混乱状態

							//敵を止める
							m_moveSpeed.Normalize();
							m_moveSpeed /= 10.0f;
							//アニメーションをアイドル状態にする
							m_animState = enIdleAnimation;
							//混乱ができるとき、
							if (m_canConfusion)
							{
								//混乱モデルを表示
								m_confusionStar = NewGO<ConfusionStar>();
								m_confusionStar->SetPosition(m_pos);

								//混乱できないようにする
								m_canConfusion = false;
							}
						}
					}
					//混乱しているとき、
					if (m_confusionStar != nullptr)
					{
						m_confusionTimer++;
						if (m_confusionTimer == 300)
						{
							//歩きアニメーションにする
							m_animState = enWalkAnimation;
						}
						if (m_confusionTimer > 500)
						{
							//混乱モデルを削除
							DeleteGO(m_confusionStar);
							//混乱状態を解除(ランダム移動になる)
							m_moveState = enRandomMoveState;
							//タイマーを初期化
							m_confusionTimer = 0;
							//混乱できるようにする
							m_canConfusion = true;
							//接触判定を復活させrう
							SetCanHitBody(true);
							//移動ストップタイマーを初期化
							m_moveStopTimer = 0;
						}
					}
				}

				break;

				//角上げ攻撃状態
			case enAttackHornsUpState:
				m_moveStopTimer++;
				if (m_moveStopTimer >= 60)
				{
					//アニメーションを角上げ攻撃に切り替え
					m_animState = enAttackHornsUpAnimation;

					if (IsAttackRangeIn())
					{
						//1ボスの正面に突き飛ばす
						m_player[m_nearPlayer]->SetMoveSpeed(m_forward * 10.0f);
					}

					//アニメーションが終了したら、
					if (!m_model->IsPlaying())
					{
						m_moveStopTimer++;
						if (m_moveStopTimer > 120)
						{
							//ランダム移動状態にする
							m_moveState = enRandomMoveState;
							//ランダム移動タイマーを初期化
							m_randomMoveTimer = 0;
							//移動ストップタイマーを初期化
							m_moveStopTimer = 0;
						}
					}
				}

				break;

				//角突き攻撃状態
			case enAttackHornsForwardState:
				m_moveStopTimer++;
				if (m_moveStopTimer >= 60)
				{
					//アニメーションを角突き攻撃に切り替え
					m_animState = enAttackHornsForwardAnimation;

					if (IsAttackRangeIn())
					{
						//1ボスの正面に突き飛ばす
						m_player[m_nearPlayer]->SetMoveSpeed(m_forward * 10.0f);
					}

					//アニメーションが終了したら、
					if (!m_model->IsPlaying())
					{
						m_moveStopTimer2++;
						if (m_moveStopTimer2 > 120)
						{
							//ランダム移動状態にする
							m_moveState = enRandomMoveState;
							//ランダム移動タイマーを初期化
							m_randomMoveTimer = 0;
							//移動ストップタイマーを初期化
							m_moveStopTimer = 0;
							m_moveStopTimer2 = 0;
						}
					}
				}

				break;
			}

			//攻撃を受けた時、
			if (GetReceiveAttackFlg())
			{
				//ヒットポイントを１減らす
				m_hitPoint--;

				//ヒットポイントが半分になったら、
				if (m_hitPoint == nsFirstBoss::MAX_HITPOINT / 2)
				{
					//攻撃受けたアニメーションを再生
					m_animState = enDamageAnimation;

					//モードチェンジ
				}
			}
			//HPが０以下にならないように補正
			m_hitPoint = int(max(0.0f, m_hitPoint));
			//HPが０になったら死ぬ
			if (m_hitPoint == 0)
			{
				//死亡アニメーションを再生
				m_animState = enDeathAnimation;
				//死亡状態
				m_nowState = enDeathState;
				//敵を倒した数を+1する
				m_playerStatus[GetLastKillPlayer()]->AddEnemyKillNum();
				//移動速度を０にする
				m_moveSpeed = Vector3::Zero;
				//接触範囲をセット
				SetHitBodyLength(1.0f);
			}

			break;

		//死亡状態
		case enDeathState:
			//死んだときに混乱が消えてなかったら、
			if (m_confusionStar != nullptr)
			{
				//混乱モデルを削除
				DeleteGO(m_confusionStar);
			}
			//死亡アニメーションが終了したら、
			if (!m_model->IsPlaying())
			{
				m_gameClearFlg = true;
			}

			break;
		}

		//HPバー更新処理
		m_hpBarUI[enRemainingHP]->SetScale({ m_hitPoint / nsFirstBoss::MAX_HITPOINT ,1.0f,1.0f });

		//重力
		Gravity();

		//更新処理
		DataUpdate();
	}

	void FirstBoss::UiUpdate()
	{
		if (m_nowState == enDeathState)
		{
			return;
		}
		//アイドルアニメーションにする
		m_animState = enIdleAnimation;
		//アニメーション再生
		m_model->PlayAnimation(m_animState);
	}

	//更新処理メソッド
	void FirstBoss::DataUpdate()
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

		//正面ベクトルを計算
		m_forward = Vector3::AxisZ;
		m_rot.Apply(m_forward);

		//攻撃を受ける範囲
		SetAttackReceiveLength(270.0f);
		//位置をセット
		SetPosition(m_pos);

		//アニメーション再生
		m_model->PlayAnimation(m_animState);
	}

	//ランダム移動処理メソッド
	void FirstBoss::RandomMove()
	{
		//ランダムに移動方向を決定
		if (m_randomMoveTimer % 120 == 0)
		{
			//120フレームで移動方向を変更
			m_moveSpeed.x = (rand() % 100) / 100.0f;
			m_moveSpeed.x -= 0.5f;
			m_moveSpeed.z = (rand() % 100) / 100.0f;
			m_moveSpeed.z -= 0.5f;
			//正規化
			m_moveSpeed.Normalize();
			//移動速度をかける
			m_moveSpeed *= nsFirstBoss::RANDOM_MOVE_POWER;
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

		//アニメーションを歩きに切り替え
		m_animState = enWalkAnimation;
	}

	//プレイヤーとの距離検索メソッド
	void FirstBoss::DistanceSearch()
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
			switch (rand() % 10)
			{
			case 0:
			case 1:
			case 2:
				//角上げアニメーションにする
				m_animState = enAttackHornsUpAnimation;
				//角上げ攻撃状態にする
				m_moveState = enAttackHornsUpState;

				break;

			case 3:
			case 4:
			case 5:
				//角突きアニメーションにする
				m_animState = enAttackHornsForwardAnimation;
				//角突き攻撃状態にする
				m_moveState = enAttackHornsForwardState;

				break;

			//ランダム移動を続行
			default:
				return;
			}
			//向きをプレイヤーの方向に向かせる
			m_moveSpeed = m_playerToEnemyDistanceDirecion[m_nearPlayer];
			//移動速度を落とす
			m_moveSpeed.Normalize();
			m_moveSpeed /= 100.0f;
		}
		else
		{
			switch (rand() % 200)
			{
			case 0:
				//ダッシュ攻撃状態にする
				m_moveState = enRunAttackState;
				//プレイヤー方向に向かせる
				m_moveSpeed = m_playerToEnemyDistanceDirecion[m_nearPlayer];
				//アニメーションを走り攻撃に切り替え
				m_animState = enRunAnimation;

				break;
			}
		}
	}

	//攻撃範囲内に入ったらフラグオンするメソッド
	bool FirstBoss::IsAttackRangeIn()
	{
		//プレイヤーとの距離が近かったら、
		if (m_plaToEneDistanceLength[m_nearPlayer] < nsFirstBoss::ATTACK_RANGE)
		{
			//エネミーの正面のベクトルと、エネミーからプレイヤーに向かうベクトルの。
			//内積(cosθ)を求める。
			float cos = m_forward.Dot(m_playerToEnemyDistanceDirecion[m_nearPlayer]);
			//内積(cosθ)から角度(θ)を求める。
			float angle = acosf(cos);
			//角度(θ)が60°より小さければ。
			if (angle <= (Math::PI / 180.0f) * 60.0f)
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
}