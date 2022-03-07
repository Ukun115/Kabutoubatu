/// <summary>
/// �X���C��(�U�R�G)�N���X
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
		//HP��ݒ�
		m_hitPoint = 4;

		m_onlineUpdateSpeed = FindGO<OnlineUpdateSpeed>(nsStdafx::ONLINEUPDATESPEED_NAME);

		//�v���C���[�̃C���X�^���X������
		for (int playerNum = enPlayer1; playerNum < GetPlayerNum(); playerNum++)
		{
			m_player[playerNum] = FindGO<Player>(nsStdafx::PLAYER_NAME[playerNum]);
			m_playerStatus[playerNum] = FindGO<PlayerStatus>(nsStdafx::PLAYER_STATUS_NAME[playerNum]);
		}

		//�U�����󂯂�͈�
		SetAttackReceiveLength(220);
		//�ʒu���Z�b�g
		SetPosition(m_pos);
		//�ڐG�͈͂��Z�b�g
		SetHitBodyLength(80.0f);

		//���f����������
		m_model = NewGO<SkinModelRender>();
		//���f���̏������Z�A�b�v�Ɏw��
		m_model->SetModelUpAxis(enModelUpAxisZ);
		m_model->SetShadowCaster(true);	//�e�𗎂Ƃ�
		m_model->SetSilhouette(true);	//�V���G�b�g�𗎂Ƃ�
		m_model->SetPlayerMode(2);	//�G�p�̃V���G�b�g
		m_model->SetOutline(true);//�֊s��������
		//�A�j���[�V���������[�h
		m_animationClips[enKyoroKyoro].Load("enemy_slime_kyorokyoro");		//�L�����L�������[�V���������[�h
		m_animationClips[enKyoroKyoro].SetLoopFlag(false);	//�����V���b�g�Đ�
		m_animationClips[enJump].Load("enemy_slime_jump");		//�W�����v���[�V���������[�h
		m_animationClips[enJump].SetLoopFlag(true);	//���[�v�Đ�
		m_animationClips[enDeath].Load("enemy_slime_death");		//���S���[�V���������[�h
		m_animationClips[enDeath].SetLoopFlag(false);	//�����V���b�g�Đ�
		m_model->Init("enemy_slime", "enemy_slime", m_animationClips, enAnimationNum);
		//���f���̈ʒu���X�V
		m_model->SetPosition(m_pos);
		//�L�����R����������
		m_charaCon.Init(30.0f, 85.0f, m_pos);
		//�L�����N�^�[�R���g���[���[���g�����ړ������ɕύX�B
		m_pos = m_charaCon.Execute(
			m_moveSpeed,
			1.0f,
			m_isHitGround,
			m_hitGroundNormal
		);

		m_model->SetAnimationSpeed(m_onlineUpdateSpeed->GetSpeed());
		//�L�����L�����A�j���[�V�����Đ�
		m_model->PlayAnimation(enKyoroKyoro);
	}

	Slime::~Slime()
	{
		//���f�����폜
		DeleteGO(m_model);
	}

	void Slime::SubUpdate()
	{
		switch (m_nowState)
		{
		case enKyoroKyoro:
			//�L�����L�����A�j���[�V�����Đ�
			m_model->PlayAnimation(enKyoroKyoro);
			//�L�����L�����A�j���[�V�������I�������A
			if (!m_model->IsPlaying())
			{
				//�W�����v�X�e�[�g�Ɉڍs
				m_nowState = enJump;
			}

			break;

		case enJump:
			//�W�����v�A�j���[�V�����Đ�
			m_model->PlayAnimation(enJump);
			//�n�ʂɂ��Ă�����W�����v
			if (m_charaCon.IsOnGround())
			{
				//�W�����v�J�n�̎��A
				if (m_jumpStartFlg)
				{
					//�W�����v
					Jump();
					//�W�����v�X�^�[�g���ǂ����t���O��܂�
					m_jumpStartFlg = false;
				}
				//���n�̎��A
				else
				{
					//TODO:���n���ʉ����Đ�
					if ((m_cameraTargetPos - m_pos).Length() < nsStdafx::SOUND_CAN_HEARD_RANGE)
					{
						m_slimeSound[enAttackSound] = NewGO<SoundSource>();
						m_slimeSound[enAttackSound]->Init(L"Assets/sound/Enemy_Slime_Jump.wav");
						m_slimeSound[enAttackSound]->SetVolume(0.2f);
						m_slimeSound[enAttackSound]->Play(false);
					}

					//�L�����L�����X�e�[�g�Ɉڍs
					m_nowState = enKyoroKyoro;
					//�W�����v�X�^�[�g���ǂ����t���O��߂�
					m_jumpStartFlg = true;
					//���K��
					m_moveSpeed.Normalize();
					m_moveSpeed /= 100.0f;
				}
			}

			break;

		case enDeath:
			if (!m_model->IsPlaying())
			{
				//�N���X��j��
				DeleteGO(this);
				//�R�C���𗎂Ƃ��悤�ɂ���
				m_dropCoin = NewGO<DropCoin>();
				m_dropCoin->SetPlayerNumber(GetPlayerNum());
				//�R�C���𗎂Ƃ��ꏊ�����f���̈ʒu�ɐݒ肷��
				m_dropCoin->SetPosition(m_pos);
			}

			break;
		}

		//�X�e�[�W���痎�������Ƃ��A
		if (-600.0f < m_pos.y && m_pos.y < -500.0f)
		{
			m_isFall = true;
		}

		//�U�����󂯂����A
		if (GetReceiveAttackFlg())
		{
			//�q�b�g�|�C���g�����炷
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
			//�}�C�i�X�ɂȂ�Ȃ��悤�ɂ���
			m_hitPoint = max(m_hitPoint, 0);
		}

		//HP���O�ɂȂ����玀��
		if (m_hitPoint == 0)
		{
			//���S�X�e�[�g�Ɉڍs
			m_nowState = enDeath;
			//�G��|��������+1����
			m_playerStatus[GetLastKillPlayer()]->AddEnemyKillNum();
			//���S�A�j���[�V�����Đ�
			m_model->SetAnimationSpeed(1.5f * m_onlineUpdateSpeed->GetSpeed());
			m_model->PlayAnimation(enDeath);

			m_moveSpeed /= 10000.0f;

			//�ڐG�͈͂��Z�b�g
			SetHitBodyLength(1.0f);
		}

		//�d��
		Gravity();

		//�ʒu�E��]�E�L�����R�����X�V
		DataUpdate();
	}

	void Slime::UiUpdate()
	{
		m_model->PlayAnimation(enKyoroKyoro);
	}

	//�X�V�������\�b�h
	void Slime::DataUpdate()
	{
		//���f���̈ʒu���X�V
		m_model->SetPosition(m_pos);
		//��]�p�x�����߂�
		m_rotAngle = atan2(m_moveSpeed.x, m_moveSpeed.z);
		//��]���X�V
		m_rot.SetRotation(Vector3::AxisY, m_rotAngle);
		m_model->SetRotation(m_rot);
		//�L�����N�^�[�R���g���[���[���g�����ړ������ɕύX�B
		m_pos = m_charaCon.Execute(
			m_moveSpeed,
			1.0f,
			m_isHitGround,
			m_hitGroundNormal
		);

		//�U�����󂯂�͈�
		SetAttackReceiveLength(220);
		//�ʒu���Z�b�g
		SetPosition(m_pos);
	}

	//�W�����v���\�b�h
	void Slime::Jump()
	{
		//�ړ������������_��
		m_moveSpeed.x = (rand() % 100) / 100.0f;
		m_moveSpeed.x -= 0.5f;
		m_moveSpeed.z = (rand() % 100) / 100.0f;
		m_moveSpeed.z -= 0.5f;
		//���K��
		m_moveSpeed.Normalize();
		//�ړ����x��������
		m_moveSpeed *= 5.0f * m_onlineUpdateSpeed->GetSpeed();

		m_moveSpeed.y = m_jumpPower * m_onlineUpdateSpeed->GetSpeed();
	}
}