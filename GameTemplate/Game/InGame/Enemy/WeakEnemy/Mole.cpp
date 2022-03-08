/// <summary>
/// ���O��(�U�R�G)�N���X
/// </summary>
#include "stdafx.h"
#include "Mole.h"
#include "../../Player/Player.h"
#include "../../Player/PlayerStatus.h"
#include "../DropCoin.h"
#include "../../Online/OnlineUpdateSpeed.h"

namespace nsKabutoubatu
{
	namespace nsMole
	{
		//�����ʒu
		const Vector3 WEAKENEMY_START_POS = { 0.0f,0.0f,-300.0f };
		//�ړ���
		const float MOVE_POWER = 15.0f;
		//�U���͈�
		const float ATTACK_RANGE = 400.0f;
		//�U������
		const float ATTACK_TIME = 50.0f;
		//�U����
		const int ATTACK_POWER = 1;
		//�U���x������
		const float ATTACK_DELAY_TIME = 50.0f;
		//�����_���ړ��̈ړ���
		const float RANDOM_MOVE_POWER = 2.0f;
		//������΂���
		const float BLOWAWAY_POWER = 20.0f;
	}

	void Mole::SubStart()
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

		//�G�̏���������
		EnemyInit();

		//�G�N�X�N�����[�V�����}�[�N�摜��������
		m_exclamationMark = NewGO<SkinModelRender>();
		m_exclamationMark->Init("ExclamationMark");
		//���߂͔�\��
		m_exclamationMark->Deactivate();

		//���ʃx�N�g�����v�Z
		m_forward = Vector3::AxisZ;
		m_rot.Apply(m_forward);

		//�ڐG������Ȃ���
		SetCanHitBody(false);

		m_randomDelayTimer = (rand() % 50);
		m_randomDelayTimer += 80;

		m_model->SetAnimationSpeed(m_onlineUpdateSpeed->GetSpeed());
	}

	Mole::~Mole()
	{
		//���f�����폜
		DeleteGO(m_model);
		//�G�N�X�N�����[�V�����}�[�N���폜
		DeleteGO(m_exclamationMark);
	}

	void Mole::SubUpdate()
	{
		switch (m_nowState)
		{
			//��������&�����_���ړ����
		case enPlayerSearchAndRandomMove:
			//��������
			DistanceSearch();
			//�����_���ړ�
			RandomMove();

			//�����A�j���[�V�����Đ�
			m_model->SetAnimationSpeed(1.0f * m_onlineUpdateSpeed->GetSpeed());
			m_model->PlayAnimation(enWalkAnimation);

			break;
			//�ːi�U�����
		case enAttack:
			//�U��(�ːi)����܂ł̒x��
			if (m_attackDelayTimer < nsMole::ATTACK_DELAY_TIME/ m_onlineUpdateSpeed->GetSpeed())
			{
				//�ːi�U����ԂɂȂ����Ƃ��A1�񂾂����s�����B
				if (m_attackDelayTimer == 0)
				{
					//TODO:�����T�E���h���Đ�
					if ((m_cameraTargetPos - m_pos).Length() < nsStdafx::SOUND_CAN_HEARD_RANGE)
					{

					}
				}

				m_attackDelayTimer++;

				//�G�N�X�N�����[�V�����}�[�N�̕\��
				ExclamationMarkActive();

				//�ːi�A�j���[�V�����Đ�
				m_model->SetAnimationSpeed(1.0f * m_onlineUpdateSpeed->GetSpeed());
				m_model->PlayAnimation(enTackleAnimation);
			}
			//�U���x���^�C�}�[���I��
			else
			{
				//�u�����I�v��UI���f�����\��
				m_exclamationMark->Deactivate();
				//�U��(�ːi)
				Attack();
			}

			break;
			//�v���C���[������΂����
		case enBlowAway:

			//���̃X�e�[�g�ɗ������ɂP�x�̂ݎ��s
			if (m_blowAwayAfterWaitTimer == 0)
			{
				//�v���C���[�ƂԂ��������ʉ����Đ�
				if ((m_cameraTargetPos - m_pos).Length() < nsStdafx::SOUND_CAN_HEARD_RANGE)
				{
					m_moleSound[enAttackSound] = NewGO<SoundSource>();
					m_moleSound[enAttackSound]->Init(L"Assets/sound/Enemy_Mole_Attack.wav");
					m_moleSound[enAttackSound]->SetVolume(0.3f);
					m_moleSound[enAttackSound]->Play(false);
				}

				//�v���C���[�ɐ�����΂��͂�n��
				PassBlowAwayDirection();
				//�ړ����~�߂�
				m_moveSpeed = Vector3::Zero;
			}
			m_blowAwayAfterWaitTimer++;
			//������΂���̒�~���Ԃ��I�[�o�[������
			if (m_blowAwayAfterWaitTimer > 70/ m_onlineUpdateSpeed->GetSpeed())
			{
				//������΂���̒�~���Ԃ�������
				m_blowAwayAfterWaitTimer = 0;
				//��������&�����_���ړ���Ԃɂ���
				m_nowState = enPlayerSearchAndRandomMove;
			}

			//Win�A�j���[�V�����Đ�
			m_model->SetAnimationSpeed(1.0f * m_onlineUpdateSpeed->GetSpeed());
			m_model->PlayAnimation(enWinAnimation);

			break;

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
		if(GetReceiveAttackFlg())
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
			m_hitPoint = max(m_hitPoint,0);
		}

		//HP���O�ɂȂ����玀��
		if (m_hitPoint == 0)
		{
			//���S�X�e�[�g�Ɉڍs
			m_nowState = enDeath;

			//�G��|��������+1����
			m_playerStatus[GetLastKillPlayer()]->AddEnemyKillNum();

			//���S�A�j���[�V�����Đ�
			m_model->SetAnimationSpeed(2.0f * m_onlineUpdateSpeed->GetSpeed());
			m_model->PlayAnimation(enDeathAnimation);

			//�u�����I�v��UI(�G�N�X�N�����[�V�����}�[�N)��\������Ă������\���ɂ���
			m_exclamationMark->Deactivate();

			m_moveSpeed /= 10000.0f;
		}

		//�d��
		Gravity();

		//�ʒu�E��]�E�L�����R�����X�V
		DataUpdate();

		//���ʃx�N�g�����v�Z
		m_forward = Vector3::AxisZ;
		m_rot.Apply(m_forward);
	}

	void Mole::UiUpdate()
	{
		//�����A�j���[�V����
		m_model->PlayAnimation(enWalkAnimation);
	}

	//�����_���ړ��������\�b�h
	void Mole::RandomMove()
	{
		//�U���˒��O���ƃ����_���ړ�
		if (m_plaToEneDistanceLength[m_nearPlayer] > nsMole::ATTACK_RANGE)
		{
			//�����_���Ɉړ����x������
			if (m_randomMoveTimer % m_randomDelayTimer == 0)
			{
				//120�t���[���ňړ�������ύX
				m_moveSpeed.x = (rand() % 100) / 100.0f;
				m_moveSpeed.x -= 0.5f;
				m_moveSpeed.z = (rand() % 100) / 100.0f;
				m_moveSpeed.z -= 0.5f;
				//���K��
				m_moveSpeed.Normalize();
				//�ړ����x��������
				m_moveSpeed *= nsMole::RANDOM_MOVE_POWER * m_onlineUpdateSpeed->GetSpeed();

				m_randomDelayTimer = (rand() % 50);
				m_randomDelayTimer += 80;
				m_randomDelayTimer /= m_onlineUpdateSpeed->GetSpeed();

				m_randomMoveTimer = 0;
			}
			//�ړ��^�C�}�[���C���N�������g
			m_randomMoveTimer++;
		}
	}

	//�����������\�b�h
	void Mole::DistanceSearch()
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
			m_plaToEneDistanceLength[playerNum] = m_playerToEnemyDistanceDirecion[playerNum].Length();
		}

		m_nearPlayer = enPlayer1;
		if (GetPlayerNum() == 2)
		{
			//�PP�Ƃ̋����A�QP�Ƃ̋������r���A�߂��ق��̃v���C���[�ԍ���ۑ�
			if (m_plaToEneDistanceLength[enPlayer1] < m_plaToEneDistanceLength[enPlayer2])
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
		if(IsAttackRangeIn())
		{
			//�U����Ԃɂ���
			m_nowState = enAttack;
			//�������v���C���[�̕����Ɍ�������
			m_moveSpeed = m_playerToEnemyDistanceDirecion[m_nearPlayer];
		}
	}

	//�U��(�ːi)�������\�b�h
	void Mole::Attack()
	{
		//������ɍs���Ȃ��悤�ɕ␳
		m_playerToEnemyDistanceDirecion[m_nearPlayer].y = min(0, m_playerToEnemyDistanceDirecion[m_nearPlayer].y);
		//�ړ����x�ɂ���
		m_moveSpeed = m_playerToEnemyDistanceDirecion[m_nearPlayer] * nsMole::MOVE_POWER * m_onlineUpdateSpeed->GetSpeed();

		//TODO:�ːi�U���T�E���h���Đ�
		if ((m_cameraTargetPos -m_pos).Length() < nsStdafx::SOUND_CAN_HEARD_RANGE)
		{

		}

		//�U�����Ԃ��J�E���g
		m_attackTimer++;

		for (int playerNum = enPlayer1; playerNum < GetPlayerNum(); playerNum++)
		{
			//�v���C���[�Ƃ̋��������߂�
			m_playerToEnemyDistanceDirecion2[playerNum] = m_player[playerNum]->GetPosition() - m_pos;
			m_plaToEneDistanceLength[playerNum] = m_playerToEnemyDistanceDirecion2[playerNum].Length();
			//�v���C���[�ɂԂ�������A
			if (m_plaToEneDistanceLength[playerNum] < 70.0f)
			{
				//�S�[�X�g��Ԃ̃v���C���[�ɑ΂��Ă͓ːi�U�����s��Ȃ�
				if (m_player[playerNum]->GetNowState() == 2)
				{
					continue;
				}

				//�ǂ���̃v���C���[�𐁂���΂�����ۑ�
				m_blowAwayPlayer = playerNum;
				//�v���C���[������΂���Ԃɂ���
				m_nowState = enBlowAway;
				//�U���x���^�C�}�[��������
				m_attackDelayTimer = 0;
				//
				m_player[playerNum]->InitDamageTimer();
			}
		}

		//�U�����Ԃ��I�������A
		if (m_attackTimer > nsMole::ATTACK_TIME/ m_onlineUpdateSpeed->GetSpeed())
		{
			//����������Ԃɂ���
			m_nowState = enPlayerSearchAndRandomMove;
			//�U�����Ԃ�������
			m_attackTimer = 0;
			//�U���x���^�C�}�[��������
			m_attackDelayTimer = 0;
			//�ړ����x��������
			m_moveSpeed.Normalize();
		}
	}

	//�X�V�������\�b�h
	void Mole::DataUpdate()
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

	//�G�N�X�N�����[�V�����}�[�N�̕\���������\�b�h
	void Mole::ExclamationMarkActive()
	{
		//�G�N�X�N�����[�V�����}�[�N����G�̓���ɒu��
		m_exclamationMarkPos = { m_pos.x-40.0f,m_pos.y+70.0f,m_pos.z };
		m_exclamationMark->SetPosition(m_exclamationMarkPos);
		//�u�����I�v��UI(�G�N�X�N�����[�V�����}�[�N)��\��
		m_exclamationMark->Activate();
	}

	//��G�̐�����΂�������n���������\�b�h
	void Mole::PassBlowAwayDirection()
	{
		//�i��ł������(������΂�����)�𐳋K��
		m_moveSpeed.Normalize();
		//������΂��͂ɕς���
		m_moveSpeed *= nsMole::BLOWAWAY_POWER;
		m_moveSpeed.y = 7.5f;
		//�v���C���[�Ɉړ����x(&�ړ�����)��n��
		m_player[m_blowAwayPlayer]->SetMoveSpeed(m_moveSpeed);
		//�v���C���[�̏�Ԃ��U�����󂯂Ă����Ԃɂ���
		m_player[m_blowAwayPlayer]->SetNowState(1);
		//�v���C���[�̃q�b�g�|�C���g���P���炷
		m_playerStatus[m_blowAwayPlayer]->ReduceHitPoint(nsMole::ATTACK_POWER);
	}

	//�G�̏������������郁�\�b�h
	void Mole::EnemyInit()
	{
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
		m_animationClips[enWalkAnimation].Load("enemy_mole_walk");		//�������[�V���������[�h
		m_animationClips[enWalkAnimation].SetLoopFlag(true);	//���[�v���[�V�����ɂ���B
		m_animationClips[enTackleAnimation].Load("enemy_mole_tackle");		//�ːi���[�V���������[�h
		m_animationClips[enTackleAnimation].SetLoopFlag(true);	//���[�v���[�V�����ɂ���B
		m_animationClips[enWinAnimation].Load("enemy_mole_win");		//�������[�V���������[�h
		m_animationClips[enWinAnimation].SetLoopFlag(false);	//�����V���b�g�Đ��ɂ���B
		m_animationClips[enDeathAnimation].Load("enemy_mole_death");		//���S���[�V���������[�h
		m_animationClips[enDeathAnimation].SetLoopFlag(false);	//�����V���b�g�Đ��ɂ���B
		m_model->Init("enemy_mole","enemy_mole", m_animationClips, enAnimationNum);
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
		//������Ԃ̓v���C���[��T���Ă�����
		m_nowState = enPlayerSearchAndRandomMove;
	}

	//�U���͈͓��ɓ�������t���O�I�����郁�\�b�h
	bool Mole::IsAttackRangeIn()
	{
		//�v���C���[�Ƃ̋������߂�������A
		if (m_plaToEneDistanceLength[m_nearPlayer] < nsMole::ATTACK_RANGE)
		{
			//�G�l�~�[�̐��ʂ̃x�N�g���ƁA�G�l�~�[����v���C���[�Ɍ������x�N�g���́B
			//����(cos��)�����߂�B
			float cos = m_forward.Dot(m_playerToEnemyDistanceDirecion[m_nearPlayer]);
			//����(cos��)����p�x(��)�����߂�B
			float angle = acosf(cos);
			//�p�x(��)��120����菬������΁B
			if (angle <= (Math::PI / 180.0f) * 120.0f)
			{
				//�v���C���[���U���͈͓��ɂ���
				return true;
			}
			else
			{
				//�v���C���[���U���͈͊O�ɂ���
				return false;
			}
		}
		else
		{
			return false;
		}
	}
};