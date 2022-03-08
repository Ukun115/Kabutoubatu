/// <summary>
/// �G�̊��N���X
/// </summary>
#include "stdafx.h"
#include "EnemyBase.h"
#include "../Player/Player.h"
#include "../Player/PlayerStatus.h"
#include "../AccompanyAI/AccompanyAI.h"
#include "../Camera/PlayerCamera.h"
#include "../Item/Weapon/Arrow.h"
#include "../Item/Weapon/Weapon.h"
#include "../Online/OnlineUpdateSpeed.h"

namespace nsKabutoubatu
{
	bool EnemyBase::Start()
	{
		m_onlineUpdateSpeed = FindGO< OnlineUpdateSpeed>(nsStdafx::ONLINEUPDATESPEED_NAME);

		//�C���X�^���X������
		for (int playerNum = enPlayer1; playerNum < m_playerNum; playerNum++)
		{
			//�v���C���[
			m_player[playerNum] = FindGO<Player>(nsStdafx::PLAYER_NAME[playerNum]);
			//�v���C���[�X�e�[�^�X
			m_playerStatus[playerNum] = FindGO<PlayerStatus>(nsStdafx::PLAYER_STATUS_NAME[playerNum]);
			//����
			m_weapon[playerNum] = FindGO<Weapon>(nsStdafx::WEAPON_NAME[playerNum]);
		}
		//�\���v���C�̎��A
		if (m_playerNum == 1)
		{
			//���sAI(���Ƃ�)
			m_accompanyAI = FindGO<AccompanyAI>(nsStdafx::ACCOMPANYAI_NAME);
		}
		//�v���C���[�J����
		m_playerCamera = FindGO<PlayerCamera>(nsStdafx::PLAYER_CAMERA_NAME);

		//�G�ʂ̃X�^�[�g���\�b�h����
		SubStart();

		return true;
	}

	EnemyBase::~EnemyBase()
	{

	}

	void EnemyBase::Update()
	{
		//�U�����󂯂鏈��
		AttackReceive();

		//�G�ꂽ�v���C���[�Ƀ_���[�W��^���鏈��
		HitBody();

		//�G�ʂ̃A�b�v�f�[�g
		SubUpdate();

		for (int playerNum = enPlayer1; playerNum < m_playerNum; playerNum++)
		{
			if (m_accompanyAI != nullptr)
			{
				if (m_accompanyAI->GetNowState() != 2)
				{
					//�v���C���[�Ƃ̋����̒������擾
					m_playerToEnemyVector = m_player[playerNum]->GetPosition() - m_pos;
					m_DistanceLength = m_playerToEnemyVector.Length();
					//�v���C���[�̋߂��ɓG�����āA��������[�h�̎��A�v���C���[�����ɂ����B
					if (m_DistanceLength < 200 && m_accompanyAI->GetMode() == 1)
					{
						////���sAI�����v�b�V���A�^�b�N�U�������Ă���
						//Vector3 m_dir;
						//m_dir = m_pos - m_accompanyAI->GetPosition();
						//m_accompanyAI->SetShieldPushDashDirection(m_dir);
					}
				}
			}
		}

		m_cameraTargetPos = m_playerCamera->GetCameraTarget();

		if (m_receiveDamageFlg)
		{
			//�_���[�W���󂯂����̐ԐF
			m_model->SetDameageRed(true);
			m_damageTimer++;
			m_receiveDamageFlg = false;
		}
		//�_���[�W�^�C�}�[�N��
		if (m_damageTimer > 0)
		{
			m_damageTimer++;
		}
		//�^�C�}�[��10�ɂȂ�����A
		if (m_damageTimer > 10/ m_onlineUpdateSpeed->GetSpeed())
		{
			//�_���[�W���󂯂����̐ԐF���Ȃ���
			m_model->SetDameageRed(false);
			m_damageTimer = 0;
		}
	}

	//�v���C���[�̍U�����󂯂鏈��
	void EnemyBase::AttackReceive()
	{
		//���S������U�����󂯂Ȃ�
		if (m_hitPoint == 0)
		{
			m_receiveDamageFlg = false;

			return;
		}

		//�v���C���[�̐l�������s
		for (int playerNum = enPlayer1; playerNum < m_playerNum; playerNum++)
		{
			//�U��������Ȃ��Ƃ��͎��s���Ȃ��B
			if (!m_player[playerNum]->IsAttack())
			{
				//�U������^�C�}�[��������
				m_attackJudgeTimer[playerNum] = 0;

				continue;
			}

			//�|�̎��͍U���͔��˂�����ɂ���̂Ŏ��s���Ȃ�
			if (m_playerStatus[playerNum]->GetHaveWeapon() == 3)
			{
				//��Ƃ̓����蔻��
				QueryGOs<Arrow>(nsStdafx::ARROW_NAME, [&](Arrow* m_arrow)->bool
				{
					if ((m_pos - m_arrow->GetPosition()).Length() < m_attackReceiveLength)
					{
						//�U�����󂯂�
						m_receiveDamageFlg = true;

						m_lastAttackPlayer = playerNum;
						//�������
						DeleteGO(m_arrow);
						//�k���|�C���^�����Ă����B
						m_arrow = nullptr;

						return true;
					}
					else
					{
						m_receiveDamageFlg = false;
						return false;
					}
				});

				continue;
			}

			//����U�����u�ԂɍU����������Ȃ��悤�Ƀf�B���C��������
			//�U������^�C�}�[��i�߂�
			m_attackJudgeTimer[playerNum]++;
			if (m_attackJudgeTimer[playerNum] == m_weapon[playerNum]->GetAttackJudgeTimer()/ m_onlineUpdateSpeed->GetSpeed())
			{
				//�v���C���[�Ƃ̋����̒������擾
				m_playerToEnemyVector = m_pos - m_player[playerNum]->GetPosition();
				m_DistanceLength = m_playerToEnemyVector.Length();
				//�������߂�������
				if (m_DistanceLength < m_attackReceiveLength)
				{
					//���K��
					m_playerToEnemyVector.Normalize();

					//�v���C���[�̐��ʂ̃x�N�g���ƁA�G�l�~�[����v���C���[�Ɍ������x�N�g���́B
					//����(cos��)�����߂�B
					float cos = m_player[playerNum]->GetForwardVector().Dot(m_playerToEnemyVector);
					//����(cos��)����p�x(��)�����߂�B
					float angle = acosf(cos);
					//�p�x(��)��80����菬������΁B
					if (angle <= (Math::PI / 180.0f) * 80.0f)
					{
						//�U�����󂯂�
						m_receiveDamageFlg = true;
						m_lastAttackPlayer = playerNum;
					}
				}
				else
				{
					//�U�����󂯂Ȃ�
					m_receiveDamageFlg = false;
				}
			}
			else
			{
				//�U�����󂯂Ȃ�
				m_receiveDamageFlg = false;
			}
		}
	}

	//�v���C���[�ƐڐG����ƃ_���[�W��^���鏈��
	void EnemyBase::HitBody()
	{
		if (!m_canHitBody)
		{
			return;
		}
		//�v���C���[���G�ꂽ��v���C���[�Ƀ_���[�W��^����I
		for (int playerNum = 0; playerNum < m_playerNum; playerNum++)
		{
			//�S�[�X�g��ԈȊO�̃v���C���[�ɑ΂��čs��
			if (m_player[playerNum]->GetNowState() == 2)
			{
				continue;
			}
			//�������擾
			m_dir[playerNum] = m_player[playerNum]->GetPosition() - m_pos;
			//�v���C���[�Ƃ̋������߂�������
			if (m_dir[playerNum].Length() < m_hitBodyLength)
			{
				//���K��
				m_dir[playerNum].Normalize();
				//������΂��p���[�ɂ���
				m_dir[playerNum] *= 10.0f;

				//�v���C���[�Ɉړ����x(&�ړ�����)��n��
				m_player[playerNum]->SetMoveSpeed(m_dir[playerNum]);
				//�v���C���[�̏�Ԃ��U�����󂯂Ă����Ԃɂ���
				m_player[playerNum]->SetNowState(1);
				//�v���C���[�̃q�b�g�|�C���g���P���炷
				m_playerStatus[playerNum]->ReduceHitPoint(1);
				//
				m_player[playerNum]->InitDamageTimer();
			}
		}
	}

	void EnemyBase::Gravity()
	{
		m_moveSpeed.y -= nsStdafx::GRAVITY * m_onlineUpdateSpeed->GetSpeed();
	}
}