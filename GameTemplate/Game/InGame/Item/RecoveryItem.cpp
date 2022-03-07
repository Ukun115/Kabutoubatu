/// <summary>
/// �񕜃A�C�e���N���X
/// </summary>
#include "stdafx.h"
#include "RecoveryItem.h"
#include "../Player/Player.h"
#include "../Player/PlayerStatus.h"
#include "../Online/OnlineUpdateSpeed.h"

namespace nsKabutoubatu
{
	namespace nsRecoveryItem
	{
		//�ړ���
		float MOVE_POWER = 4.0f;
	}

	bool RecoveryItem::Start()
	{
		m_onlineUpdateSpeed = FindGO< OnlineUpdateSpeed>(nsStdafx::ONLINEUPDATESPEED_NAME);

		if (m_isSoloPlay)
		{
			m_totalPlayerNum = enPlayer2;
		}

		//�v���C���[�̃C���X�^���X������
		for (int playerNum = enPlayer1; playerNum < m_totalPlayerNum; playerNum++)
		{
			m_player[playerNum] = FindGO<Player>(nsStdafx::PLAYER_NAME[playerNum]);
			m_playerStatus[playerNum] = FindGO<PlayerStatus>(nsStdafx::PLAYER_STATUS_NAME[playerNum]);
		}

		//�񕜃A�C�e���̃��f����������
		RecoveryItemInit();

		return true;
	}

	RecoveryItem::~RecoveryItem()
	{
		//���f�����폜
		DeleteGO(m_model);
	}

	void RecoveryItem::Update()
	{
		//�񕜃A�C�e���̈ʒu���v���C���[�̓���ɒu�����߂̎擾
		if (m_canPlayerMoveSpeedGet)
		{
			//�ʒu���v���C���[�̓���Ɏ����Ă���
			m_pos = m_player[m_playerNum]->GetPosition();
			m_pos.y += 140.0f;
			//�v���C���[�̈ړ����x���擾
			m_moveSpeed = m_player[m_playerNum]->GetMoveSpeed();
		}

		//�v���C���[�̉񕜃A�C�e���̏�Ԃɂ���ĕω�
		switch (m_player[m_playerNum]->GetRecoveryItemState())
		{
		//�v���C���[�ɂ����������Ă�����
		case enRecoverySet:
			m_canPlayerMoveSpeedGet = true;

			break;

		//�v���C���[�ɓ�����ꂽ���
		case enRecoveryThrow:
			//�\���v���C�̎��͑����Ɏ��g���񕜂���
			if (m_isSoloPlay)
			{
				//�v���C���[��HP�����łɍő�̎��A
				if (m_playerStatus[m_playerNum]->GetHitPoint() == m_playerStatus[m_playerNum]->GetMaxHitPoint())
				{
					//�񕜂������ɉ񕜃A�C�e�������܂��B
					//�����ɓ�����Ȃ�������莝���ɖ߂�
					m_playerStatus[m_playerNum]->AddRecoveryItemNum();
				}
				//�v���C���[��HP���ő傶��Ȃ��Ƃ��A
				else
				{
					//�q�b�g�|�C���g���v���C���[�̉񕜗͕��񕜂�����
					m_playerStatus[m_playerNum]->AddHitPoint(m_recoveryPower);
					m_playerStatus[m_playerNum]->AddRecoveryReceiveNum(m_recoveryPower);
				}
				//�v���C���[�̏�Ԃ��񕜏�������Ԃɂ���
				//���̃��\�b�h�Ń��f�����폜����
				m_player[m_playerNum]->SetRecoveryState(enRecoveryDelete);
			}
			else
			{
				if (m_canPlayerMoveSpeedGet)
				{
					//�v���C���[�̈ړ����x���擾
					m_moveSpeed = m_player[m_playerNum]->GetMoveSpeed();
					//���K�����ăv���C���[�̈ړ������݂̂��擾
					m_moveSpeed.Normalize();
					//�L�����R���̈ʒu���X�V
					m_charaCon.SetPosition(m_pos);
					//�ړ������ɑ��x������B
					m_moveSpeed *= nsRecoveryItem::MOVE_POWER * m_onlineUpdateSpeed->GetSpeed();
					//�΂ߏ�ɓ�����悤��Y��ݒ�
					m_moveSpeed.y = 5.0f;

					//�L�����R����������
					m_charaCon.Init(10.0f, 10.0f, m_pos);

					m_canPlayerMoveSpeedGet = false;
				}

				//�ړ����x�����͂�ݒ肷��
				m_decreasePower = m_moveSpeed;
				m_decreasePower *= -1.0f;
				//���C�͂��ړ����x�ɉ��Z����
				m_moveSpeed.x += m_decreasePower.x * g_gameTime->GetFrameDeltaTime();
				m_moveSpeed.z += m_decreasePower.z * g_gameTime->GetFrameDeltaTime();

				//�d��
				Gravity();

				//�L�����N�^�[�R���g���[���[���g�����ړ������ɕύX�B
				m_pos = m_charaCon.Execute(
					m_moveSpeed,
					1.0f,
					m_isHitGround,
					m_hitGroundNormal
				);

				//�n�ʂɂ�����폜
				if (!m_canPlayerMoveSpeedGet && m_charaCon.IsOnGround())
				{
					//�v���C���[�̏�Ԃ��񕜏�������Ԃɂ���
					//���̃��\�b�h�Ń��f�����폜����
					m_player[m_playerNum]->SetRecoveryState(enRecoveryDelete);
					//�����ɓ�����Ȃ�������莝���ɖ߂�
					m_playerStatus[m_playerNum]->AddRecoveryItemNum();
				}
				//�����ĂȂ��v���C���[�ɉ񕜃A�C�e��������������A
				if (!m_isSoloPlay)
				{
					if ((m_player[(m_playerNum + 1) % 2]->GetPosition() - m_pos).Length() < 150.0f)
					{
						//�v���C���[��HP�����łɍő�̎��A
						if (m_playerStatus[(m_playerNum + 1) % 2]->GetHitPoint() == m_playerStatus[(m_playerNum + 1) % 2]->GetMaxHitPoint())
						{
							//�v���C���[�̏�Ԃ��񕜏�������Ԃɂ���
							//���̃��\�b�h�Ń��f�����폜����
							m_player[m_playerNum]->SetRecoveryState(enRecoveryDelete);
							//�����ɓ�����Ȃ�������莝���ɖ߂�
							m_playerStatus[m_playerNum]->AddRecoveryItemNum();

							return;
						}

						//�q�b�g�|�C���g���v���C���[�̉񕜗͕��񕜂�����
						m_playerStatus[(m_playerNum + 1) % 2]->AddHitPoint(m_recoveryPower);
						m_playerStatus[(m_playerNum + 1) % 2]->AddRecoveryReceiveNum(m_recoveryPower);

						//�v���C���[�Ƀq�b�g�����t���O�𗧂Ă�
						m_playerHit = true;

						//�v���C���[�̏�Ԃ��񕜏�������Ԃɂ���
						//���̃��\�b�h�Ń��f�����폜����
						m_player[m_playerNum]->SetRecoveryState(enRecoveryDelete);
					}
				}
				//�߂�����^���ɗ��������玩���폜����
				//��������Ȃ��ƃv���C���[�������Ȃ��Ȃ�A
				//�v���C���[�̃X�g���X�ɂȂ邽�߁B
				m_fallTimer++;
				if (m_fallTimer > 100/m_onlineUpdateSpeed->GetSpeed())
				{
					//�v���C���[�̏�Ԃ��񕜏�������Ԃɂ���
					//���̃��\�b�h�Ń��f�����폜����
					m_player[m_playerNum]->SetRecoveryState(enRecoveryDelete);
				}
			}

			break;
		}
		//�f�[�^���X�V
		DataUpdate();
	}

	//�񕜃A�C�e�����f���̏�����
	void RecoveryItem::RecoveryItemInit()
	{
		//�񕜃A�C�e�����f����������
		//���f����������
		m_model = NewGO<SkinModelRender>();
		m_model->SetShadowCaster(false);	//�e�𗎂Ƃ��Ȃ�
		m_model->Init("RecoveryItem");
		//�ʒu���v���C���[�̓���Ɏ����Ă���
		m_pos = m_player[m_playerNum]->GetPosition();
		m_pos.y += 140.0f;
		//���f���̈ʒu���X�V
		m_model->SetPosition(m_pos);
	}

	//�X�V�������\�b�h
	void RecoveryItem::DataUpdate()
	{
		//�ʒu
		m_model->SetPosition(m_pos);
		//��]�p�x
		m_rotAngle = atan2(m_moveSpeed.x, m_moveSpeed.z);
		//��]
		m_rot.SetRotation(Vector3::AxisY, m_rotAngle);
		m_model->SetRotation(m_rot);
	}
};