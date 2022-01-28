/// <summary>
/// �v���C���[�̃T�E���h�N���X
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
			//����ɂ���čU�����̔����^�C�~���O��ς���
			switch (m_haveWeaponNo)
			{
			//�؂̖_
			case enWoodStick:
				m_weaponSoundTiming = 30;

				break;

				//���ʂ̌�
			case enSword:
				m_weaponSoundTiming = 10;

				break;

				//�匕
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
		//�����폜
		for (int playerSoundNum = enJumpSound; playerSoundNum < enPlayerSoundNum; playerSoundNum++)
		{
			DeleteGO(m_playerSound[playerSoundNum]);
		}
	}

	//�W�����v���Đ����\�b�h
	void PlayerSound::PlayJumpSound()
	{
		//�W�����v���̏�����
		m_playerSound[enJumpSound] = NewGO<SoundSource>();
		m_playerSound[enJumpSound]->Init(L"Assets/sound/Player_Jump.wav");
		m_playerSound[enJumpSound]->SetVolume(0.2f);
		//�W�����v�����Đ�
		m_playerSound[enJumpSound]->Play(false);
	}

	//�U�����Đ����\�b�h
	void PlayerSound::PlayWeaponAttackSound(const int haveWeaponNo)
	{
		//�U�����̏�����
		m_playerSound[enSwordWingSound] = NewGO<SoundSource>();
		switch (haveWeaponNo)
		{
			//�؂̖_
		case enWoodStick:
			m_playerSound[enSwordWingSound]->Init(L"Assets/sound/Player_WoodStick.wav");
			break;
			//���ʂ̌�
		case enSword:
			m_playerSound[enSwordWingSound]->Init(L"Assets/sound/Player_Sword_Swing.wav");
			break;
			//�匕
		case enLargeSword:
			m_playerSound[enSwordWingSound]->Init(L"Assets/sound/Player_LargeSword.wav");
			break;
			//�|
		case enBow:
			m_playerSound[enSwordWingSound]->Init(L"Assets/sound/Player_Arrow.wav");
			break;
		}
		m_playerSound[enSwordWingSound]->SetVolume(0.5f);
		//�U�������Đ�
		m_playerSound[enSwordWingSound]->Play(false);
	}

	//�񕜉��Đ����\�b�h
	void PlayerSound::PlayRecoverySound()
	{
		//�񕜉��̏�����
		m_playerSound[enRecoverySound] = NewGO<SoundSource>();
		m_playerSound[enRecoverySound]->Init(L"Assets/sound/Player_Recovery.wav");
		m_playerSound[enRecoverySound]->SetVolume(0.5f);
		//�񕜉����Đ�
		m_playerSound[enRecoverySound]->Play(false);
	}

	//�񕜃A�C�e���𓊂��鉹�Đ����\�b�h
	void PlayerSound::PlayRecoveryItemThrowSound()
	{
		//�񕜃A�C�e���𓊂��鉹�̏�����
		m_playerSound[enRecoveryItemThrowSound] = NewGO<SoundSource>();
		m_playerSound[enRecoveryItemThrowSound]->Init(L"Assets/sound/Player_RecoveryItemThrow.wav");
		m_playerSound[enRecoveryItemThrowSound]->SetVolume(0.3f);
		//�񕜃A�C�e���𓊂��鉹���Đ�
		m_playerSound[enRecoveryItemThrowSound]->Play(false);
	}

	//�R�C���Q�b�g���Đ����\�b�h
	void PlayerSound::PlayAddCoinSound()
	{
		m_playerSound[enAddCoinSound] = NewGO<SoundSource>();
		m_playerSound[enAddCoinSound]->Init(L"Assets/sound/Coin_Get.wav");
		m_playerSound[enAddCoinSound]->SetVolume(0.3f);
		//�擾�����Đ�
		m_playerSound[enAddCoinSound]->Play(false);
	}

	//������Đ����\�b�h
	void PlayerSound::PlayAvoidanceSound()
	{
		m_playerSound[enAvoidanceSound] = NewGO<SoundSource>();
		m_playerSound[enAvoidanceSound]->Init(L"Assets/sound/Player_Avoidance.wav");
		m_playerSound[enAvoidanceSound]->SetVolume(0.5f);
		//�擾�����Đ�
		m_playerSound[enAvoidanceSound]->Play(false);
	}
}