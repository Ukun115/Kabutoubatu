/// <summary>
/// �g�Q�X���C��(�U�R�G)�N���X
/// </summary>
#include "stdafx.h"
#include "NeedleSlime.h"
#include "../../Player/Player.h"
#include "../../Player/PlayerStatus.h"
#include "../DropCoin.h"
#include "Needle.h"


namespace nsKabutoubatu
{
	namespace nsNeedleSlime
	{
		//�����_���ړ��̈ړ���
		const float RANDOM_MOVE_POWER = 0.5f;
		//�U���͈�
		const float ATTACK_RANGE = 500.0f;
	}

	void NeedleSlime::SubStart()
	{
		//HP��ݒ�
		m_hitPoint = 4;

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
		m_animationClips[enIdle].Load("enemy_needleSlime_idle");		//�A�C�h�����[�V���������[�h
		m_animationClips[enIdle].SetLoopFlag(true);		//���[�v�Đ�
		m_animationClips[enWalk].Load("enemy_needleSlime_walk");		//�������[�V���������[�h
		m_animationClips[enWalk].SetLoopFlag(true);		//���[�v�Đ�
		m_animationClips[enAttack].Load("enemy_needleSlime_attack");		//�U�����[�V���������[�h
		m_animationClips[enAttack].SetLoopFlag(false);	//�����V���b�g�Đ�
		m_animationClips[enDeath].Load("enemy_needleSlime_death");		//���S���[�V���������[�h
		m_animationClips[enDeath].SetLoopFlag(false);	//�����V���b�g�Đ�

		m_model->Init("enemy_needleSlime", "enemy_needleSlime", m_animationClips, enAnimationNum);
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
	}

	NeedleSlime::~NeedleSlime()
	{
		//���f�����폜
		DeleteGO(m_model);

		if (m_needle != nullptr)
		{
			DeleteGO(m_needle);
		}
	}

	void NeedleSlime::SubUpdate()
	{
		switch (m_nowState)
		{
		//�A�C�h����
		case enIdle:
			//�n�ʂɂ��Ă���Ƃ��̂ݎ��s
			if (m_charaCon.IsOnGround())
			{
				//�������v���C���[�̕����Ɍ�������
				m_moveSpeed = m_playerToEnemyDistanceDirecion[m_nearPlayer];
				m_moveSpeed /= 10000.0f;
				m_idleTimer++;
				if (m_idleTimer > 160)
				{
					//������Ԃɂ���
					m_nowState = enWalk;
					//�^�C�}�[��������
					m_idleTimer = 0;
				}
			}

			break;

		//������
		case enWalk:
			//��������
			DistanceSearch();
			//�����_���ړ�
			RandomMove();

			break;

		//�U����
		case enAttack:
			if (!m_model->IsPlaying())
			{
				m_nowState = enIdle;
			}
			break;

		//���S��
		case enDeath:

			//�ڐG�͈͂��Z�b�g
			SetHitBodyLength(1.0f);

			//���S�A�j���[�V�������I������玀��
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

		//�̗͂�0�ɂȂ�����A
		if (m_hitPoint == 0)
		{
			//�G��|��������+1����
			m_playerStatus[GetLastKillPlayer()]->AddEnemyKillNum();
			//���S��ԂɑJ��
			m_model->SetAnimationSpeed(4.0f);
			m_nowState = enDeath;
		}

		//�d��
		Gravity();

		//�ʒu�E��]�E�L�����R�����X�V
		DataUpdate();
	}

	void NeedleSlime::UiUpdate()
	{
		//�����A�j���[�V����
		m_model->PlayAnimation(enIdle);
	}

	//�X�V�������\�b�h
	void NeedleSlime::DataUpdate()
	{
		//�A�j���[�V�����Đ�
		m_model->PlayAnimation(m_nowState);

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

	//�����������\�b�h
	void NeedleSlime::DistanceSearch()
	{
		//�v���C���[�Ƃ̋��������߂�
		for (int playerNum = enPlayer1; playerNum < GetPlayerNum(); playerNum++)
		{
			//�S�[�X�g��Ԃ̃v���C���[�ɑ΂��Ă͋����������s��Ȃ�
			if (m_player[playerNum]->GetNowState() == 2)
			{
				//�S�[�X�g��Ԃ̃v���C���[�Ƃ̋������ʂĂ��Ȃ���������
				m_plaToEneDistanceLength[playerNum] = 9999999.9f;

				continue;
			}

			m_playerToEnemyDistanceDirecion[playerNum] = m_player[playerNum]->GetPosition() - m_pos;
			m_playerToEnemyDistanceDirecion[playerNum].y += 50.0f;
			m_plaToEneDistanceLength[playerNum] = m_playerToEnemyDistanceDirecion[playerNum].Length();
		}

		m_nearPlayer = enPlayer1;
		if (GetPlayerNum() == 2)
		{
			//�PP�Ƃ̋����A�QP�Ƃ̋������r���A�߂��ق��̃v���C���[�ԍ���ۑ�
			if (m_plaToEneDistanceLength[enPlayer1] < m_plaToEneDistanceLength[enPlayerNum::enPlayer2])
			{
				m_nearPlayer = enPlayer1;
			}
			else
			{
				m_nearPlayer = enPlayer2;
			}
		}
		//��ԋ߂��v���C���[����G�̃x�N�g���𐳋K�����ĕ��������̏��ɂ���
		m_playerToEnemyDistanceDirecion[m_nearPlayer].Normalize();

		//�U���͈͂ɓ�������A
		if (IsAttackRangeIn())
		{
			//�U����Ԃɂ���
			m_nowState = enAttack;
			//�������v���C���[�̕����Ɍ�������
			m_moveSpeed = m_playerToEnemyDistanceDirecion[m_nearPlayer];
			m_moveSpeed /= 1000.0f;
			m_idleTimer = 0;

			//�j���΂�
			m_needle = NewGO<Needle>();
			m_needle->SetPlayerNum(GetPlayerNum());
			m_needle->SetMoveDirection(m_moveSpeed*1000.0f);

			m_needleInitPos = Vector3::Zero;

			if(m_moveSpeed.x > 0)
			{
				m_needleInitPos.x = 70.0f;
			}
			else
			{
				m_needleInitPos.x = -70.0f;
			}

			//�Ƃ����Ƃ��X���C���̑O�����甭�˂���悤�ɂ���B�ꎟ�������𗘗p�B
			m_needleInitPos.z = ((m_moveSpeed.z*100)/(m_moveSpeed.x*100))*m_needleInitPos.x;
			m_needleInitPos += m_pos;
			m_needleInitPos.y += 15.0f;
			m_needle->SetInitPosition(m_needleInitPos);
		}
	}

	//�����_���ړ��������\�b�h
	void NeedleSlime::RandomMove()
	{
		//�U���˒��O���ƃ����_���ړ�
		if (m_plaToEneDistanceLength[m_nearPlayer] > nsNeedleSlime::ATTACK_RANGE)
		{
			//�����_���Ɉړ����x������
			if (m_randomMoveTimer % 240 == 0)
			{
				//120�t���[���ňړ�������ύX
				m_moveSpeed.x = (rand() % 100) / 100.0f;
				m_moveSpeed.x -= 0.5f;
				m_moveSpeed.z = (rand() % 100) / 100.0f;
				m_moveSpeed.z -= 0.5f;
				//���K��
				m_moveSpeed.Normalize();
				//�ړ����x��������
				m_moveSpeed *= nsNeedleSlime::RANDOM_MOVE_POWER;
			}
			//�ړ��^�C�}�[���C���N�������g
			m_randomMoveTimer++;

			//�ǂɓ������Ă�����A
			if (m_charaCon.IsHitWall())
			{
				//�����_���ړ��^�C�}�[��������
				m_randomMoveTimer = 0;
				//�t�����Ɉړ�����悤�ɂ���
				m_moveSpeed *= -1.0f;
			}
		}
	}

	//�U���͈͓��ɓ�������t���O�I�����郁�\�b�h
	bool NeedleSlime::IsAttackRangeIn()
	{
		//�󒆂ɂ���Ƃ��͍U���s��
		if (m_charaCon.IsJump())
		{
			return false;
		}

		//�v���C���[�Ƃ̋������߂�������A
		if (m_plaToEneDistanceLength[m_nearPlayer] < nsNeedleSlime::ATTACK_RANGE)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
}