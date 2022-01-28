/// <summary>
/// プレイヤーのサウンドクラス
/// </summary>
#include "stdafx.h"
#include "PlayerSound.h"
#include "Player.h"

namespace nsKabutoubatu
{
	void PlayerSound::Update()
	{

		if (m_attackSoundFlg)
		{
			//武器によって攻撃音の発生タイミングを変える
			switch (m_haveWeaponNo)
			{
			//木の棒
			case enWoodStick:
				m_weaponSoundTiming = 30;

				break;

				//普通の剣
			case enSword:
				m_weaponSoundTiming = 10;

				break;

				//大剣
			case enLargeSword:
				m_weaponSoundTiming = 70;

				break;

			case enBow:
				m_weaponSoundTiming = 0;

				break;
			}
			m_attackSoundTimer++;
			if(m_attackSoundTimer > m_weaponSoundTiming)
			{
				PlayWeaponAttackSound(m_haveWeaponNo);
				m_attackSoundFlg = false;
				m_attackSoundTimer = 0;
			}
		}
	}

	PlayerSound::~PlayerSound()
	{
		//音を削除
		for (int playerSoundNum = enJumpSound; playerSoundNum < enPlayerSoundNum; playerSoundNum++)
		{
			DeleteGO(m_playerSound[playerSoundNum]);
		}
	}

	//ジャンプ音再生メソッド
	void PlayerSound::PlayJumpSound()
	{
		//ジャンプ音の初期化
		m_playerSound[enJumpSound] = NewGO<SoundSource>();
		m_playerSound[enJumpSound]->Init(L"Assets/sound/Player_Jump.wav");
		m_playerSound[enJumpSound]->SetVolume(0.2f);
		//ジャンプ音を再生
		m_playerSound[enJumpSound]->Play(false);
	}

	//攻撃音再生メソッド
	void PlayerSound::PlayWeaponAttackSound(const int haveWeaponNo)
	{
		//攻撃音の初期化
		m_playerSound[enSwordWingSound] = NewGO<SoundSource>();
		switch (haveWeaponNo)
		{
			//木の棒
		case enWoodStick:
			m_playerSound[enSwordWingSound]->Init(L"Assets/sound/Player_WoodStick.wav");
			break;
			//普通の剣
		case enSword:
			m_playerSound[enSwordWingSound]->Init(L"Assets/sound/Player_Sword_Swing.wav");
			break;
			//大剣
		case enLargeSword:
			m_playerSound[enSwordWingSound]->Init(L"Assets/sound/Player_LargeSword.wav");
			break;
			//弓
		case enBow:
			m_playerSound[enSwordWingSound]->Init(L"Assets/sound/Player_Arrow.wav");
			break;
		}
		m_playerSound[enSwordWingSound]->SetVolume(0.5f);
		//攻撃音を再生
		m_playerSound[enSwordWingSound]->Play(false);
	}

	//回復音再生メソッド
	void PlayerSound::PlayRecoverySound()
	{
		//回復音の初期化
		m_playerSound[enRecoverySound] = NewGO<SoundSource>();
		m_playerSound[enRecoverySound]->Init(L"Assets/sound/Player_Recovery.wav");
		m_playerSound[enRecoverySound]->SetVolume(0.5f);
		//回復音を再生
		m_playerSound[enRecoverySound]->Play(false);
	}

	//回復アイテムを投げる音再生メソッド
	void PlayerSound::PlayRecoveryItemThrowSound()
	{
		//回復アイテムを投げる音の初期化
		m_playerSound[enRecoveryItemThrowSound] = NewGO<SoundSource>();
		m_playerSound[enRecoveryItemThrowSound]->Init(L"Assets/sound/Player_RecoveryItemThrow.wav");
		m_playerSound[enRecoveryItemThrowSound]->SetVolume(0.3f);
		//回復アイテムを投げる音を再生
		m_playerSound[enRecoveryItemThrowSound]->Play(false);
	}

	//コインゲット音再生メソッド
	void PlayerSound::PlayAddCoinSound()
	{
		m_playerSound[enAddCoinSound] = NewGO<SoundSource>();
		m_playerSound[enAddCoinSound]->Init(L"Assets/sound/Coin_Get.wav");
		m_playerSound[enAddCoinSound]->SetVolume(0.3f);
		//取得音を再生
		m_playerSound[enAddCoinSound]->Play(false);
	}

	//回避音再生メソッド
	void PlayerSound::PlayAvoidanceSound()
	{
		m_playerSound[enAvoidanceSound] = NewGO<SoundSource>();
		m_playerSound[enAvoidanceSound]->Init(L"Assets/sound/Player_Avoidance.wav");
		m_playerSound[enAvoidanceSound]->SetVolume(0.5f);
		//取得音を再生
		m_playerSound[enAvoidanceSound]->Play(false);
	}
}