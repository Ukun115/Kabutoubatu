/// <summary>
/// 最初のボスクラス
/// </summary>
#include "stdafx.h"
#include "FirstBoss.h"
#include "../Player/Player.h"
#include "../Player/PlayerStatus.h"
#include "ConfusionStar.h"
#include "../Online/OnlineUpdateSpeed.h"

namespace nsKabutoubatu
{
	namespace nsFirstBoss
	{
		//スポーンする初期座標
		const Vector3 INIT_POSITION = { 0.0f,-9500.0f,-400.0f };
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

		//ボスのモデルのファイルパス
		const char* BOSS_MODEL_FILE_PATH = {"enemy_firstBoss"};
		//アニメーションのファイルパス
		const char* ANIMATION_FILE_PATH[7] =
		{
			"enemy_firstBoss_idle",					//アイドル
			"enemy_firstBoss_walk",					//歩き
			"enemy_firstBoss_run",					//走り
			"enemy_firstBoss_Attack_HornsUp",		//角突き上げ
			"enemy_firstBoss_Attack_HornsForward",	//角突き
			"enemy_firstBoss_Damage",				//ダメージ
			"enemy_firstBoss_death",				//死亡
		};
		//アニメーションそれぞれのループアニメーションかどうかのフラグ
		const bool ANIMATION_LOOP_FLAG[7] =
		{
			true,	//アイドル
			true,	//歩き
			true,	//走り
			false,	//角突き上げ
			false,	//角突き
			false,	//ダメージ
			false,	//死亡
		};
	}

	void FirstBoss::SubStart()
	{

		m_onlineUpdateSpeed = FindGO< OnlineUpdateSpeed>(nsStdafx::ONLINEUPDATESPEED_NAME);

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
		for (int animationNum = enIdleAnimation; animationNum < enAnimationNum; animationNum++)
		{
			m_animationClips[animationNum].Load(nsFirstBoss::ANIMATION_FILE_PATH[animationNum]);
			m_animationClips[animationNum].SetLoopFlag(nsFirstBoss::ANIMATION_LOOP_FLAG[animationNum]);
		}
		//敵のモデル情報を初期化
		m_model->Init(nsFirstBoss::BOSS_MODEL_FILE_PATH, nsFirstBoss::BOSS_MODEL_FILE_PATH, m_animationClips, enAnimationNum);
		//モデルの位置を更新
		m_pos = nsFirstBoss::INIT_POSITION;
		m_model->SetPosition(m_pos);
		//キャラコンを初期化
		m_charaCon.Init(100.0f, 100.0f, m_pos);
		//正面ベクトルを計算
		m_forward = Vector3::AxisZ;
		m_rot.Apply(m_forward);

		m_model->SetAnimationSpeed(m_onlineUpdateSpeed->GetSpeed());
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
		//地面についていないときは移動処理をしない。
		if (m_charaCon.IsOnGround())
		{
			switch (m_nowState)
			{
			//アイドル状態
			case enIdle:
				if (m_idleTimer == 0)
				{
					//着地音再生
					m_sound[enLandingSound] = NewGO<SoundSource>();
					m_sound[enLandingSound]->Init(L"Assets/sound/FirstBoss_Landing.wav");
					m_sound[enLandingSound]->SetVolume(0.5f);
					m_sound[enLandingSound]->Play(false);
				}
				m_idleTimer++;
				if (m_idleTimer > 60/ m_onlineUpdateSpeed->GetSpeed())
				{
					//通常状態にする
					m_nowState = enNormalState;
				}

				break;

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

					//ダッシュ攻撃
				case enDashAttackState:
					if (m_moveStopTimer == 0)
					{
						//チャージ音再生
						m_sound[enChargeSound] = NewGO<SoundSource>();
						m_sound[enChargeSound]->Init(L"Assets/sound/FirstBoss_Charge.wav");
						m_sound[enChargeSound]->SetVolume(0.5f);
						m_sound[enChargeSound]->Play(false);
					}
					m_moveStopTimer++;
					if (m_moveStopTimer >= 120/ m_onlineUpdateSpeed->GetSpeed())
					{
						if (m_moveStopTimer == 120/ m_onlineUpdateSpeed->GetSpeed())
						{
							m_moveSpeed *= 20.0f;

							//突進音再生
							m_sound[enDashAttackSound] = NewGO<SoundSource>();
							m_sound[enDashAttackSound]->Init(L"Assets/sound/FirstBoss_DashAttack.wav");
							m_sound[enDashAttackSound]->SetVolume(0.5f);
							m_sound[enDashAttackSound]->Play(false);
						}
						//接触判定をなくす
						SetCanHitBody(false);

						//壁に当たったら、かつ、混乱していないとき、
						if (m_charaCon.IsHitWall() && m_confusionStar == nullptr)
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
							if (m_confusionTimer == 300/ m_onlineUpdateSpeed->GetSpeed())
							{
								//歩きアニメーションにする
								m_animState = enWalkAnimation;
							}
							if (m_confusionTimer > 500/ m_onlineUpdateSpeed->GetSpeed())
							{
								//混乱モデルを削除
								DeleteGO(m_confusionStar);
								m_confusionStar = nullptr;
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
					if (m_moveStopTimer == 0)
					{
						//角上げ攻撃音再生
						m_sound[enAttackHornsUpSound] = NewGO<SoundSource>();
						m_sound[enAttackHornsUpSound]->Init(L"Assets/sound/FirstBoss_Attack.wav");
						m_sound[enAttackHornsUpSound]->SetVolume(0.5f);
						m_sound[enAttackHornsUpSound]->Play(false);
					}
					m_moveStopTimer++;
					if (m_moveStopTimer >= 60/ m_onlineUpdateSpeed->GetSpeed())
					{
						//アニメーションを角上げ攻撃に切り替え
						m_animState = enAttackHornsUpAnimation;

						if (IsAttackRangeIn())
						{
							//1ボスの正面に突き飛ばす
							m_player[m_nearPlayer]->SetMoveSpeed(m_forward * 10.0f * m_onlineUpdateSpeed->GetSpeed());
						}

						//アニメーションが終了したら、
						if (!m_model->IsPlaying())
						{
							m_moveStopTimer++;
							if (m_moveStopTimer > 120/ m_onlineUpdateSpeed->GetSpeed())
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
					if (m_moveStopTimer == 0)
					{
						//角突き攻撃音再生
						m_sound[enAttackHornsForwardSound] = NewGO<SoundSource>();
						m_sound[enAttackHornsForwardSound]->Init(L"Assets/sound/FirstBoss_Attack.wav");
						m_sound[enAttackHornsForwardSound]->SetVolume(0.5f);
						m_sound[enAttackHornsForwardSound]->Play(false);
					}
					m_moveStopTimer++;
					if (m_moveStopTimer >= 60/ m_onlineUpdateSpeed->GetSpeed())
					{
						//アニメーションを角突き攻撃に切り替え
						m_animState = enAttackHornsForwardAnimation;

						if (IsAttackRangeIn())
						{
							//1ボスの正面に突き飛ばす
							m_player[m_nearPlayer]->SetMoveSpeed(m_forward * 10.0f * m_onlineUpdateSpeed->GetSpeed());
						}

						//アニメーションが終了したら、
						if (!m_model->IsPlaying())
						{
							m_moveStopTimer2++;
							if (m_moveStopTimer2 > 120/ m_onlineUpdateSpeed->GetSpeed())
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
			m_moveSpeed *= nsFirstBoss::RANDOM_MOVE_POWER * m_onlineUpdateSpeed->GetSpeed();
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
			switch (rand() % int(200/ m_onlineUpdateSpeed->GetSpeed()))
			{
			case 0:
				//ダッシュ攻撃状態にする
				m_moveState = enDashAttackState;
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
};