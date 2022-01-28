/// <summary>
/// プレイヤーのアニメーションクラス
/// </summary>
#include "stdafx.h"
#include "PlayerAnimation.h"
#include "Player.h"
#include "PlayerStatus.h"
#include "PlayerCharaCon.h"

namespace nsKabutoubatu
{
	namespace nsPlayerAnimation
	{
		//アニメーションのプレイヤー番号に続くファイルパス
		const char* ANIMATION_FILE_PATH[enAnimationClips_num] =
		{
			"_idle",				//アイドル
			"_run",					//走り(木の棒)
			"_walk",				//歩き
			"_jump",				//ジャンプ
			"_attack",				//攻撃(木の棒)
			"_down",				//ダウン
			"_rollingAvoidance",	//回転回避
			"_haveSwordRun",		//走り(剣)
			"_swordAttack",			//攻撃(剣)
			"_largeSwordRun",		//走り(大剣)
			"_largeSwordAttack",	//攻撃(大剣)
			"_bowRun",				//走り(弓)
			"_bowAttack"			//攻撃(弓)
		};
		//アニメーションそれぞれのループアニメーションかどうかのフラグ
		const bool ANIMATION_LOOP_FLAG[enAnimationClips_num] =
		{
			true,	//アイドル
			true,	//走り(木の棒)
			true,	//歩き
			false,	//ジャンプ
			false,	//攻撃(木の棒)
			false,	//ダウン
			false,	//回転回避
			true,	//走り(剣)
			false,	//攻撃(剣)
			true,	//走り(大剣)
			false,	//攻撃(大剣)
			true,	//走り(弓)
			false,	//攻撃(弓)
		};
	}

	bool PlayerAnimation::Start()
	{
		//インスタンスを検索
		m_player = FindGO<Player>(nsStdafx::PLAYER_NAME[m_playerNum]);
		m_playerStatus = FindGO<PlayerStatus>(nsStdafx::PLAYER_STATUS_NAME[m_playerNum]);
		m_playerCharaCon = FindGO<PlayerCharaCon>(nsStdafx::PLAYER_CHARACON_NAME[m_playerNum]);

		for (int animationNum = enAnimationClip_idle; animationNum < enAnimationClips_num; animationNum++)
		{
			switch (m_playerNum)
			{
				//１Pのとき
			case enPlayer1:
				//アニメーションのファイルパスを「Player1」から続くようにする。
				sprintf(m_animationFilePath, "Player1%s", nsPlayerAnimation::ANIMATION_FILE_PATH[animationNum]);
				break;
				//２Pのとき
			case enPlayer2:
				//アニメーションのファイルパスを「Player2」から続くようにする。
				sprintf(m_animationFilePath, "Player2%s", nsPlayerAnimation::ANIMATION_FILE_PATH[animationNum]);
				break;
			}

			//アニメーションをロード
			m_animationClips[animationNum].Load(m_animationFilePath);
			//ループアニメーションかどうかを設定
			m_animationClips[animationNum].SetLoopFlag(nsPlayerAnimation::ANIMATION_LOOP_FLAG[animationNum]);
		}

		return true;
	}
	//通常移動状態時のアニメーション処理メソッド
	void PlayerAnimation::NormalStateAnimation()
	{
		//回避アニメーションの時は通常移動アニメーションはしない。
		if (m_animState == enAnimationClip_rollingAvoidance)
		{
			return;
		}

		//プレイヤーが地面についているとき、
		if (m_playerCharaCon->IsOnGround())
		{
			//ほぼ止まっているとき、(絶対値が1.0fより小さい)
			if (fabsf(m_player->GetMoveSpeed().x) < 1.0f
				&& fabsf(m_player->GetMoveSpeed().z) < 1.0f)
			{
				//アニメーションをアイドルに切り替え
				m_animState = enAnimationClip_idle;
			}
			//少し動いているとき、
			else if (fabsf(m_player->GetMoveSpeed().x) < 5.0f
				&& fabsf(m_player->GetMoveSpeed().z) < 5.0f)
			{
				//アニメーションを歩きに切り替え
				m_animState = enAnimationClip_walk;
			}
			else
			{
				//アニメーションを走りに切り替え
				switch (m_playerStatus->GetHaveWeapon())
				{
				//木の棒
				case enWoodStick:
					m_animState = enAnimationClip_run;

					break;

				//普通の剣
				case enSword:
					m_animState = enAnimationClips_swordRun;

					break;
				//大剣
				case enLargeSword:
					m_animState = enAnimationClips_largeSwordRun;

					break;
				//弓
				case enBow:
					m_animState = enAnimationClips_bowRun;
				}
			}
		}
		else
		{
			//アニメーションをジャンプに切り替え
			m_animState = enAnimationClip_jump;
		}
	}
}