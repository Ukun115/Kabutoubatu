/// <summary>
/// プレイヤークラス
/// </summary>
#include "stdafx.h"
#include "Player.h"
#include "../Item/RecoveryItem.h"
#include "../../Graphics/MiniMap.h"
#include "PlayerRotation.h"
#include "PlayerAnimation.h"
#include "PlayerSound.h"
#include "PlayerCharaCon.h"
#include "PlayerStatus.h"
#include "../Item/Weapon/Arrow.h"

namespace nsKabutoubatu
{
	namespace nsPlayer
	{
		//プレイヤー１の初期位置
		const Vector3 PLAYER1_START_POS = { 70.0f,0.0f,0.0f };
		//プレイヤー２の初期位置
		const Vector3 PLAYER2_START_POS = { -70.0f,0.0f,0.0f };
		//ゴースト状態の移動力
		const float GHOST_MOVE_POWER = 400.0f;
		//通常の摩擦力
		const float NORMAL_FRICTION_POWER = 5.0f;
		//超摩擦力
		const float SUPER_FRICTION_POWER = 30.0f;
	}

	bool Player::Start()
	{
		m_miniMap = FindGO<MiniMap>(nsStdafx::MINIMAP_NAME);

		//プレイヤーの回転クラス
		m_playerRotation = NewGO< PlayerRotation >(nsStdafx::PRIORITY_0,nsStdafx::PLAYER_ROTATION_NAME[m_playerNum]);
		m_playerRotation->SetPlayerNum(m_playerNum);
		//プレイヤーのアニメーションクラス
		m_playerAnimation = NewGO< PlayerAnimation > (nsStdafx::PRIORITY_0, nsStdafx::PLAYER_ANIMATION_NAME[m_playerNum]);
		m_playerAnimation->SetPlayerNum(m_playerNum);
		//プレイヤーのサウンドクラス
		m_playerSound = NewGO< PlayerSound >(nsStdafx::PRIORITY_0,nsStdafx::PLAYER_SOUND_NAME[m_playerNum]);
		//プレイヤーのキャラコンクラス
		m_playerCharaCon = NewGO<PlayerCharaCon >(nsStdafx::PRIORITY_0, nsStdafx::PLAYER_CHARACON_NAME[m_playerNum]);
		m_playerCharaCon->InitRigidBody(m_pos);
		//プレイヤーのステータスクラス
		m_playerStatus = NewGO< PlayerStatus >(nsStdafx::PRIORITY_0,nsStdafx::PLAYER_STATUS_NAME[m_playerNum]);
		m_playerStatus->SetPlayerNum(m_playerNum);

		//モデルを初期化
		m_model = NewGO<SkinModelRender>();
		m_model->SetShadowCaster(true);	//影を落とす
		m_model->SetSilhouette(true);	//シルエットを落とす
		m_model->SetOutline(true);//輪郭線をつける
		//初期のプレイヤーの状態は通常
		m_nowState = enNormal;

		switch (m_playerNum)
		{
		//１P
		case enPlayer1:

			//相方の情報を取得できるようにする
			if (!m_isSoloPlay)
			{
				m_otherPlayer = FindGO<Player>(nsStdafx::PLAYER_NAME[enPlayer2]);
			}

			m_model->SetPlayerMode(enPlayer1);	//１Pだということをfxに渡すためのセットメソッド
			//Zアップに変更
			m_model->SetModelUpAxis(enModelUpAxisZ);
			m_model->Init("Player1", "Player1", m_playerAnimation->GetAnimationClips(), m_playerAnimation->GetAnimationNum());
			m_pos = nsPlayer::PLAYER1_START_POS;
			m_playerCharaCon->InitRigidBody(m_pos);
			DataUpdate();

			//火の玉エフェクトの初期化
			m_fireBallEffect.Init(u"Assets/effect/efk/FireBall_Red.efk");

			break;

		//２P
		case enPlayer2:

			//１Pの情報を取得できるようにする
			if (!m_isSoloPlay)
			{
				m_otherPlayer = FindGO<Player>(nsStdafx::PLAYER_NAME[enPlayer1]);
			}

			m_model->SetPlayerMode(enPlayer2);	//2Pだということをfxに渡すためのセットメソッド
			//Zアップに変更
			m_model->SetModelUpAxis(enModelUpAxisZ);
			m_model->Init("Player2", "Player2", m_playerAnimation->GetAnimationClips(), m_playerAnimation->GetAnimationNum());
			m_pos = nsPlayer::PLAYER2_START_POS;
			m_playerCharaCon->InitRigidBody(m_pos);
			DataUpdate();

			//火の玉エフェクトの初期化
			m_fireBallEffect.Init(u"Assets/effect/efk/FireBall_Blue.efk");

			break;
		}

		return true;
	}

	Player::~Player()
	{
		//モデルを削除
		DeleteGO(m_model);
		//回復アイテムを削除
		if (m_recoveryItem != nullptr)
		{
			DeleteGO(m_recoveryItem);
		}
		if (!m_isSoloPlay)
		{
			DeleteGO(m_otherPlayer);
		}

		//プレイヤーの回転クラスを削除
		DeleteGO(m_playerRotation);
		//プレイヤーのアニメーションクラスを削除
		DeleteGO(m_playerAnimation);
		//プレイヤーのサウンドクラスを削除
		DeleteGO(m_playerSound);
		//プレイヤーのキャラコンクラスを削除
		DeleteGO(m_playerCharaCon);
		//プレイヤーのステータスクラスを削除
		DeleteGO(m_playerStatus);

		//マップ非表示
		m_miniMap->Deactivate();
	}

	void Player::Update()
	{
		switch (m_nowState)
		{
		//通常状態
		case enNormal:
			//攻撃中じゃないとき通常移動できる。
			if (!m_isAttackStateFlg)
			{
				//通常移動状態時のアニメーション処理
				m_playerAnimation->NormalStateAnimation();
				//通常移動
				NormalMove();
				//ジャンプ
				Jump();
				//回避
				Avoidance();
				//回復の一連の処理
				Recovery();
			}
			//攻撃中の時、
			else
			{
				//攻撃アニメーションが切れたら攻撃中フラグを折る
				if (!m_model->IsPlaying())
				{
					m_isAttackStateFlg = false;
				}
			}
			//通常の摩擦力をかける
			Friction(nsPlayer::NORMAL_FRICTION_POWER);

			//残り体力を調べて、残り体力が０になったら、
			if (m_playerStatus->GetHitPoint() == 0)
			{
				//ゴースト状態になる。
				m_nowState = enGhost;
				//ゴーストなので影を落とさないようにする
				m_model->SetShadowCaster(false);
				//ゴースト状態なのでキャラコンを削除
				m_playerCharaCon->RemoveRigidBoby();
			}

			//落下処理メソッド
			Fall();

			break;

		//攻撃を受けた状態
		case enSufferAnAttack:
			//アニメーションをジャンプに切り替え
			m_playerAnimation->ChangeAnimation(enPlayerAnimation::enAnimationClip_jump);
			//通常の摩擦力をかける
			Friction(nsPlayer::NORMAL_FRICTION_POWER);
			//移動速度を調べ、ある程度まで移動速度が落ちたら、
			if (m_moveSpeed.Length() < 1.0f)
			{
				//残り体力を調べて、残り体力が０になったら、
				if (m_playerStatus->GetHitPoint() == 0)
				{
					//ゴースト状態になる。
					m_nowState = enGhost;
					//ゴーストなので影を落とさないようにする
					m_model->SetShadowCaster(false);
					//ゴースト状態なのでキャラコンを削除
					m_playerCharaCon->RemoveRigidBoby();
					//デス数を増やす
					m_playerStatus->AddDeathNum();

					m_damageTimer = 0;
					m_damageFlg = false;
				}
				//残り体力が０じゃないときは、
				else
				{
					//通常状態に戻す
					m_nowState = enNormal;

					m_damageTimer = 0;
					m_damageFlg = false;
				}
			}

			//回復アイテム所持キャンセルされる処理
			//回復アイテムを削除
			if (m_recoveryItem != nullptr)
			{
				RecoveryItemDelete();
			}

			//落下処理
			Fall();

			break;

			//ゴースト状態
		case enGhost:
			if (m_model->GetFireBall())
			{
				if (m_playerStatus->GetHitPoint() > 0)
				{
					//モデルを見えるようにする。
					m_model->SetFireBall(false);
					//通常状態に戻る
					m_nowState = enNormal;
					//影を落とすようにする
					m_model->SetShadowCaster(true);
					//キャラコンを再初期化
					m_playerCharaCon->InitRigidBody(m_pos);
					//移動速度を初期化
					m_moveSpeed = Vector3::Zero;
					//火の玉を消す
					m_fireBallEffect.Stop();
					m_fireBallEffectTimer = 0;
					m_fireBallEffectActive = false;
				}
				//Aボタンが押されたら、
				if (m_gamePad->IsTrigger(enButtonA))
				{
					if (!m_isSoloPlay)
					{
						//相方プレイヤーのHPが３以上ならHP譲渡可能
						if (m_otherPlayer->m_playerStatus->GetHitPoint() >= 3)
						{
							//相方の体力を２減らす
							m_otherPlayer->m_playerStatus->ReduceHitPoint(2);
							//総回復してもらった量をセットする
							m_playerStatus->AddRecoveryReceiveNum(2);
							//自分の体力を１にする
							m_playerStatus->SetHitPoint(1);
							//モデルを見えるようにする。
							m_model->SetFireBall(false);
							//通常状態に戻る
							m_nowState = enNormal;
							//影を落とすようにする
							m_model->SetShadowCaster(true);
							//キャラコンを再初期化
							m_playerCharaCon->InitRigidBody(m_pos);
							//移動速度を初期化
							m_moveSpeed = Vector3::Zero;
							//火の玉を消す
							m_fireBallEffect.Stop();
							m_fireBallEffectTimer = 0;
							m_fireBallEffectActive = false;
						}
					}
				}
			}

			if (m_fireBallEffectActive == false)
			{
				//ダウンアニメーションにする
				m_playerAnimation->ChangeAnimation(enPlayerAnimation::enAnimationClip_down);
				//アニメーションが遅かったため３倍する
				m_model->SetAnimationSpeed(3.0f);
			}
			else
			{
				//ゴースト状態での移動
				GhostMove();
				m_model->SetAnimationSpeed(1.0f);

				//歩きアニメーションにする
				m_playerAnimation->ChangeAnimation(enPlayerAnimation::enAnimationClip_walk);

				//エフェクトを再生
				if (m_fireBallEffectTimer == 0)
				{
					m_fireBallEffect.Play();
				}
				m_fireBallEffectTimer++;
				if (m_fireBallEffectTimer > 200)
				{
					m_fireBallEffectTimer = 0;
				}
			}
			//ダウンアニメーションが終了したら、
			if (!m_model->IsPlaying()&& m_nowState != enNormal)
			{
				//モデルを見えないようにする。
				m_model->SetFireBall(true);

				m_fireBallEffectActive = true;
			}

			break;

		//何もしない状態
		case enDontMove:
			break;
		}

		//ダメージを受けたらエフェクトを出す。
		DamageEffect();

		//ゴースト状態じゃないときは重力を受け、キャラコンで動く
		if (m_nowState != enGhost)
		{
			//重力
			Gravity(m_gravityState);
		}
		//データを更新
		DataUpdate();

		//ミニマップを開く処理
		MiniMapOpen();
	}

	void Player::UiUpdate()
	{
		//アイドルアニメーションにする
		m_playerAnimation->ChangeAnimation(enPlayerAnimation::enAnimationClip_idle);
		//アニメーションの状態更新
		m_model->PlayAnimation(m_playerAnimation->GetAnimationState());

	}

	//摩擦メソッド
	void Player::Friction(const float frictionPower)
	{
		//摩擦力を設定する
		m_friction = m_moveSpeed;
		m_friction *= -frictionPower;

		//摩擦力を加算する
		m_moveSpeed.x += m_friction.x * g_gameTime->GetFrameDeltaTime();
		m_moveSpeed.z += m_friction.z * g_gameTime->GetFrameDeltaTime();
	}

	//移動メソッド
	void Player::NormalMove()
	{
		//大剣をもっているときは移動速度低下
		if (m_playerStatus->GetHaveWeapon() == enLargeSword)
		{
			//左スティックの入力量を加算する
			m_moveSpeed.x -= m_gamePad->GetLStickXF() * m_playerStatus->GetMovePower()/2 * g_gameTime->GetFrameDeltaTime();
			m_moveSpeed.z -= m_gamePad->GetLStickYF() * m_playerStatus->GetMovePower() /2 * g_gameTime->GetFrameDeltaTime();
		}
		else
		{
			//左スティックの入力量を加算する
			m_moveSpeed.x -= m_gamePad->GetLStickXF() * m_playerStatus->GetMovePower() * g_gameTime->GetFrameDeltaTime();
			m_moveSpeed.z -= m_gamePad->GetLStickYF() * m_playerStatus->GetMovePower() * g_gameTime->GetFrameDeltaTime();
		}
	}

	//ゴースト状態の時の右スティックで上下移動するメソッド
	void Player::GhostMove()
	{
		//左スティックの入力量を横移動に加算する
		m_pos.x -= m_gamePad->GetLStickXF() * nsPlayer::GHOST_MOVE_POWER * g_gameTime->GetFrameDeltaTime();
		m_pos.z -= m_gamePad->GetLStickYF() * nsPlayer::GHOST_MOVE_POWER * g_gameTime->GetFrameDeltaTime();
		//右スティックの入力量を上下移動
		m_pos.y += m_gamePad->GetRStickYF() * nsPlayer::GHOST_MOVE_POWER * g_gameTime->GetFrameDeltaTime();

		//プレイヤーを回転させたいので通常移動の処理も置いておく。
		//(これは移動力が加わることはない)
		NormalMove();
	}

	//ジャンプメソッド
	void Player::Jump()
	{
		//空中にいるときはジャンプできない
		if (m_playerCharaCon->IsJump())
		{
			return;
		}

		//Bボタンでジャンプ
		if (m_gamePad->IsTrigger(enButtonB))
		{
			//ジャンプ音
			m_playerSound->PlayJumpSound();

			//アニメーションをジャンプに切り替え
			m_playerAnimation->ChangeAnimation(enPlayerAnimation::enAnimationClip_jump);

			m_moveSpeed.y = m_playerStatus->GetJumpPower();

			//大剣をもっているときはジャンプ力低下
			if (m_playerStatus->GetHaveWeapon() == enLargeSword)
			{
				m_moveSpeed.y /= 2.0f;
			}
		}
	}

	//回避メソッド
	void Player::Avoidance()
	{
		if (m_gamePad->IsTrigger(enButtonLB1)&& !m_isAvoidance && m_playerCharaCon->GetCharaCon()->IsOnGround())
		{
			m_playerAnimation->ChangeAnimation(enPlayerAnimation::enAnimationClip_rollingAvoidance);
			//回避の移動速度を乗算
			m_moveSpeed.x *= 3.0f;
			m_moveSpeed.z *= 3.0f;

			//回避効果音を鳴らす
			m_playerSound->PlayAvoidanceSound();

			//回避中にする
			m_isAvoidance = true;
		}
		//回避アニメーションが終わったら、
		if (!m_model->IsPlaying())
		{
			//回避アニメーションから抜ける
			m_playerAnimation->ChangeAnimation(enPlayerAnimation::enAnimationClip_idle);

			//回避していないようにする
			m_isAvoidance = false;
		}
		if (m_isAvoidance && m_playerAnimation->GetAnimationState() != enPlayerAnimation::enAnimationClip_rollingAvoidance)
		{
			//回避していないようにする
			m_isAvoidance = false;
		}

		//回避アニメーション中
		if (m_playerAnimation->GetAnimationState() == enPlayerAnimation::enAnimationClip_rollingAvoidance)
		{
			m_model->SetAnimationSpeed(1.5f);
		}
		//回避アニメーション以外
		else
		{
			m_model->SetAnimationSpeed(1.0f);
		}

	}

	//プレイヤーのデータを更新するメソッド
	//位置・回転・キャラコン・アニメーション状態
	void Player::DataUpdate()
	{
		//アニメーションの状態更新
		m_model->PlayAnimation(m_playerAnimation->GetAnimationState());

		//火の玉更新
		m_fireBallEffect.SetPosition({ m_pos.x,m_pos.y+40.0f,m_pos.z });
		m_fireBallEffect.Update();

		//ゴースト状態じゃないときかつ
		//移動を開始してからキャラコン移動
		if (m_nowState != enGhost)
		{
			//キャラクターコントローラーを使った移動処理に変更。
			m_pos = m_playerCharaCon->RigidBodyExecute(m_moveSpeed,m_isHitGround,m_hitGroundNormal);
		}
		//位置を更新
		m_model->SetPosition(m_pos);
		//回転を更新
		m_model->SetRotation(m_playerRotation->GetRotation());

		//正面ベクトルを計算
		m_forward = Vector3::AxisZ;
		m_playerRotation->GetRotation().Apply(m_forward);
	}

	//回復処理メソッド
	void Player::Recovery()
	{
		//１以上持っているときのみ回復アイテムを使える、
		if (m_playerStatus->GetRecoveryItemNum() != 0)
		{
			//回復アイテムを持つ＆しまう処理
			// 投げる処理は回復アイテムクラスに書く。
			//Xボタンが押されたら、
			if (m_gamePad->IsTrigger(enButtonX))
			{
				//回復アイテムを持っていないとき、
				if (!m_haveRecoveryItem)
				{
					//回復アイテムを持つ
					m_recoveryState = enRecoverySet;
					m_haveRecoveryItem = true;
				}
				//回復アイテムを持っているとき、
				else
				{
					//かつ投げていないとき、
					if (m_recoveryState != enRecoveryThrow)
					{
						//回復アイテムをしまう
						RecoveryItemDelete();

					}
				}
			}
			//ドアを開閉していないときのみ実行できる
			if (m_nowDoorTatch)
			{
				return;
			}
			//攻撃ボタン(Aボタン)を押したら、
			if (m_gamePad->IsTrigger(enButtonA))
			{
				//回復アイテムを持っているとき
				if (m_haveRecoveryItem)
				{
					//回復アイテムを投げる音
					m_playerSound->PlayRecoveryItemThrowSound();

					//投げた状態にする
					m_recoveryState = enRecoveryThrow;

					//所持数を１減らす
					m_playerStatus->ReduceRecoveryItemNum();
				}
				//持っていないときは攻撃中にする
				else
				{
					//攻撃音
					m_playerSound->SetAttackSoundFlg(m_playerStatus->GetHaveWeapon());

					//攻撃アニメーション
					switch (m_playerStatus->GetHaveWeapon())
					{
					//木の棒
					case enWoodStick:
						m_playerAnimation->ChangeAnimation(enPlayerAnimation::enAnimationClip_attack);

						break;

					//普通の剣
					case enSword:
						m_playerAnimation->ChangeAnimation(enPlayerAnimation::enAnimationClips_swordAttack);

						break;

					//大剣
					case enLargeSword:
						m_playerAnimation->ChangeAnimation(enPlayerAnimation::enAnimationClips_largeSwordAttack);

						break;

					//弓
					case enBow:
						m_playerAnimation->ChangeAnimation(enPlayerAnimation::enAnimationClips_bowAttack);

						//矢を飛ばす
						ShotArrow();

						break;
					}

					//攻撃中フラグをオン
					m_isAttackStateFlg = true;
				}
			}
		}

		switch (m_recoveryState)
		{
		case enRecoverySet:
			//回復アイテムを生成
			m_recoveryItem = NewGO<RecoveryItem>();
			m_recoveryItem->SetIsSoloPlay(m_isSoloPlay);
			//プレイヤー番号をセット
			m_recoveryItem->SetPlayerNum(m_playerNum);
			//プレイヤーの回復力をセット
			m_recoveryItem->SetRecoveryPower(m_playerStatus->GetRecoveryPower());

			m_recoveryState = enRecoveryStay;
			break;

		case enRecoveryDelete:

			//相方に当たっていたとき、
			if (m_recoveryItem->GetIsPlayerHit())
			{
				//回復音
				m_playerSound->PlayRecoverySound();
			}

			//回復アイテムを削除
			RecoveryItemDelete();

			break;

		case enRecoveryThrow:
			//超摩擦をかける
			Friction(nsPlayer::SUPER_FRICTION_POWER);
			//投げた後の処理は回復アイテムクラスに任せる。
			//回復アイテムが削除されるタイミングも任せる。
			break;

		case enRecoveryStay:
			//何もしない
			break;
		}
	}

	//回復アイテムをしまうメソッド
	void Player::RecoveryItemDelete()
	{
		if (m_recoveryItem)
		{
			DeleteGO(m_recoveryItem);
		}
		m_haveRecoveryItem = false;
		m_recoveryState = enRecoveryStay;
	}

	//落下処理メソッド
	void Player::Fall()
	{
		//地面についているとき、
		if (m_playerCharaCon->IsOnGround())
		{
			//位置を保存
			m_finalOnGroundPos = m_pos;
			m_finalOnGroundPos.y += 200.0f;

			//通常は通常重力
			m_gravityState = enNormalGravity;
		}
		//プレイヤーがステージから落下したとき、
		if (-600.0f < m_pos.y && m_pos.y < -500.0f)
		{
			//キャラコンを削除
			m_playerCharaCon->RemoveRigidBoby();
			m_pos = m_finalOnGroundPos;
			//キャラコンを再初期化
			m_playerCharaCon->InitRigidBody(m_pos);
			//体力を１減らす
			m_playerStatus->ReduceHitPoint(1);

			//半重力でリスポーン
			m_gravityState = enHalfGravity;
		}
	}

	//プレイヤーの位置を移動させる
	void Player::SetPositionA(const Vector3& position)
	{
		m_pos = position;
		//キャラコンを一旦破棄
		m_playerCharaCon->RemoveRigidBoby();
		//キャラコンを再初期化
		m_playerCharaCon->InitRigidBody(m_pos);
	};

	//ミニマップを開くメソッド
	void Player::MiniMapOpen()
	{
		//ショップ内にいるときはマップは開けない。
		if (m_nowState == enDontMove)
		{
			//マップ非表示
			m_miniMap->Deactivate();

			return;
		}
		//スタートボタンでミニマップを開く
		if (m_gamePad->IsTrigger(enButtonStart))
		{
			//表示されているとき、
			if (m_miniMap->IsActive())
			{
				//マップ非表示
				m_miniMap->Deactivate();
			}
			//マップが非表示のとき、
			else
			{
				//マップ表示
				m_miniMap->Activate();
			}
		}
	}

	//矢を飛ばす関数
	void Player::ShotArrow()
	{
			//矢を生成
			m_arrow = NewGO<Arrow>(nsStdafx::PRIORITY_0,nsStdafx::ARROW_NAME);
			m_arrow->SetPlayerNum(m_playerNum);		//プレイヤー番号を渡す
			m_arrow->SetShotPosition({ m_pos.x,m_pos.y+50.0f,m_pos.z });			//射出位置を渡す
			m_arrow->SetMoveDirection(m_moveSpeed);	///移動方向を渡す
	}

	void Player::Gravity(const int gravityPower)
	{
		switch (gravityPower)
		{
		case enNormalGravity:
			m_moveSpeed.y -= nsStdafx::GRAVITY;
			break;
		case enHalfGravity:
			m_moveSpeed.y -= nsStdafx::GRAVITY / 5;
			break;
		}
	}

	void Player::DamageEffect()
	{
		//ダメージを受けた時の赤色
		if (m_damageFlg)
		{
			m_damageTimer++;
		}
		if (m_damageTimer == 1)
		{
			m_model->SetDameageRed(true);
		}
		//タイマーが10になったら、
		if (m_damageTimer > 10)
		{
			//ダメージを受けた時の赤色をなくす
			m_model->SetDameageRed(false);
			m_damageFlg = false;
		}
	}
};