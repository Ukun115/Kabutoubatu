 /// <summary>
 /// �Ƃ��X���C��(�U�R�G)�̉������U���̂Ƃ��N���X
 /// </summary>
#include "stdafx.h"
#include "Needle.h"
#include "../../Player/Player.h"
#include "../../Player/PlayerStatus.h"
#include "../../Online/OnlineUpdateSpeed.h"

namespace nsKabutoubatu
{
	namespace nsNeedle
	{
		//�Ƃ��̈ړ���
		const float NEEDLE_SPEED = 8.0f;
		//�Ƃ��̐ڐG�͈�
		const float NEEDLE_HIT_RANGE = 80.0f;
	}

	void Needle::SubStart()
	{
		m_onlineUpdateSpeed = FindGO<OnlineUpdateSpeed>(nsStdafx::ONLINEUPDATESPEED_NAME);

		//�ڐG������Ȃ���
		SetCanHitBody(false);

		//�v���C���[�̃C���X�^���X������
		for (int playerNum = enPlayer1; playerNum < GetPlayerNum(); playerNum++)
		{
			m_player[playerNum] = FindGO<Player>(nsStdafx::PLAYER_NAME[playerNum]);
			m_playerStatus[playerNum] = FindGO<PlayerStatus>(nsStdafx::PLAYER_STATUS_NAME[playerNum]);
		}

		//�U�����󂯂�͈�
		SetAttackReceiveLength(1);
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

		m_model->Init("enemy_needleSlime_needle");
		//���f���̈ʒu���X�V
		m_model->SetPosition(m_pos);
		m_moveSpeed *= nsNeedle::NEEDLE_SPEED;
		m_moveSpeed *= m_onlineUpdateSpeed->GetSpeed();
		m_moveSpeed.y = 0.0f;

		//�Ƃ��̌������Z�b�g
		//��]�p�x�����߂�
		float m_rotAngle = atan2(m_moveSpeed.x, m_moveSpeed.z);
		//��]���X�V
		Quaternion m_rot;
		m_rot.SetRotation(Vector3::AxisY, m_rotAngle);
		m_model->SetRotation(m_rot);
	}

	Needle::~Needle()
	{
		//���f�����폜
		DeleteGO(m_model);
	}

	void Needle::SubUpdate()
	{
		m_timer++;

		if (m_timer < 20/ m_onlineUpdateSpeed->GetSpeed())
		{
			return;
		}
		if (m_timer == 20/m_onlineUpdateSpeed->GetSpeed())
		{
			m_needleSlimeSound[enAttackSound] = NewGO<SoundSource>();
			m_needleSlimeSound[enAttackSound]->Init(L"Assets/sound/Enemy_NeedleSlime_Attack.wav");
			m_needleSlimeSound[enAttackSound]->SetVolume(0.3f);
			m_needleSlimeSound[enAttackSound]->Play(false);
		}

		if (m_timer > 60/ m_onlineUpdateSpeed->GetSpeed())
		{
			//�Ƃ��폜
			DeleteGO(this);
		}

		for (int playerNum = enPlayer1; playerNum < GetPlayerNum(); playerNum++)
		{
			//�S�[�X�g��Ԃ̎��͓�����Ȃ�
			if (m_player[playerNum]->GetNowState() == 2)
			{
				continue;
			}

			//�v���C���[�ƐڐG������A
			if ((m_player[playerNum]->GetPosition()-m_pos).Length() < nsNeedle::NEEDLE_HIT_RANGE)
			{
				//�v���C���[�̃q�b�g�|�C���g���P���炷
				m_playerStatus[playerNum]->ReduceHitPoint(1);
				//�v���C���[�̈ړ����~�߂�
				Vector3 m_newPlayerMoveSpeed = m_player[playerNum]->GetMoveSpeed();
				m_newPlayerMoveSpeed /= 10000.0f;
				m_player[playerNum]->SetMoveSpeed(m_newPlayerMoveSpeed);
				//
				m_player[playerNum]->InitDamageTimer();

				//TODO:�ڐG���ʉ����Đ�
				if ((m_cameraTargetPos - m_pos).Length() < nsStdafx::SOUND_CAN_HEARD_RANGE)
				{

				}

				//
				m_player[playerNum]->InitDamageTimer();

				//�Ƃ��폜
				DeleteGO(this);
			}
		}

		//���f���̈ʒu���X�V
		m_pos += m_moveSpeed;
		m_model->SetPosition(m_pos);
		//�ʒu���Z�b�g
		SetPosition(m_pos);
	}
}