/// <summary>
/// �ŏ��̃{�X�N���X
/// </summary>
#include "stdafx.h"
#include "FirstBoss.h"
#include "../Player/Player.h"
#include "../Player/PlayerStatus.h"
#include "ConfusionStar.h"
#include "../Online/OnlineUpdateSpeed.h"

namespace nsKabutoubatu
{
	namespace nsFirstBoss
	{
		//�X�|�[�����鏉�����W
		const Vector3 INIT_POSITION = { 0.0f,-9500.0f,-400.0f };
		//�q�b�g�|�C��0�g�̍ő�l
		const float MAX_HITPOINT = 40;
		//�����_���ړ��̈ړ���
		const float RANDOM_MOVE_POWER = 2.0f;

		const char* HP_UI_NAME[2] = { "HPBar","HPBar_RemainingHP" };

		const Vector4   FONT_COLOR = { 1.0f,1.0f,1.0f,1.0f };	//�������ԃt�H���g�̐F
		const float     FONT_ROT = 0.0f;			//�t�H���g�̌X��
		const Vector2   FONT_PIV = { 1.0f,1.0f };	//�t�H���g�̊�_
		const float     FONT_SCA = 0.5f;			//�t�H���g�̊g�嗦

		const float ATTACK_RANGE = 200.0f;

		//�{�X�̃��f���̃t�@�C���p�X
		const char* BOSS_MODEL_FILE_PATH = {"enemy_firstBoss"};
		//�A�j���[�V�����̃t�@�C���p�X
		const char* ANIMATION_FILE_PATH[7] =
		{
			"enemy_firstBoss_idle",					//�A�C�h��
			"enemy_firstBoss_walk",					//����
			"enemy_firstBoss_run",					//����
			"enemy_firstBoss_Attack_HornsUp",		//�p�˂��グ
			"enemy_firstBoss_Attack_HornsForward",	//�p�˂�
			"enemy_firstBoss_Damage",				//�_���[�W
			"enemy_firstBoss_death",				//���S
		};
		//�A�j���[�V�������ꂼ��̃��[�v�A�j���[�V�������ǂ����̃t���O
		const bool ANIMATION_LOOP_FLAG[7] =
		{
			true,	//�A�C�h��
			true,	//����
			true,	//����
			false,	//�p�˂��グ
			false,	//�p�˂�
			false,	//�_���[�W
			false,	//���S
		};
	}

	void FirstBoss::SubStart()
	{

		m_onlineUpdateSpeed = FindGO< OnlineUpdateSpeed>(nsStdafx::ONLINEUPDATESPEED_NAME);

		//HP��ݒ�
		m_hitPoint = 40;
		//�U�����󂯂�͈�
		SetAttackReceiveLength(300.0f);
		//�ʒu���Z�b�g
		SetPosition(nsFirstBoss::INIT_POSITION);
		//�ڐG�͈͂��Z�b�g
		SetHitBodyLength(160.0f);

		//�v���C���[�̃C���X�^���X������
		for (int playerNum = enPlayer1; playerNum < GetPlayerNum(); playerNum++)
		{
			m_player[playerNum] = FindGO<Player>(nsStdafx::PLAYER_NAME[playerNum]);
			m_playerStatus[playerNum] = FindGO<PlayerStatus>(nsStdafx::PLAYER_STATUS_NAME[playerNum]);
		}

		for (int hpBarUINum = enHPBar; hpBarUINum < enHPBarUINum; hpBarUINum++)
		{
			m_hpBarUI[hpBarUINum] = NewGO<SpriteRender>();
			m_hpBarUI[hpBarUINum]->Init(nsFirstBoss::HP_UI_NAME[hpBarUINum], 500, 20);
			m_hpBarUI[hpBarUINum]->SetPosition({ -240.0f,200.0f,0.0f });
			//��_���摜�̍���ɂ���
			m_hpBarUI[hpBarUINum]->SetPivot({ 0.0f, 0.0f });
		}
		//HP�o�[�̍����̓G��
		m_firstBossName = NewGO<FontRender>();
		//�t�H���g�̏�����
		m_firstBossName->Init
		(
			L"",			//�e�L�X�g
			{ -310.0f,225.0f },	//�ʒu
			nsFirstBoss::FONT_COLOR,	//�F
			nsFirstBoss::FONT_ROT,		//�X��
			nsFirstBoss::FONT_SCA,		//�g�嗦
			nsFirstBoss::FONT_PIV		//��_
		);
		//�����̋��E���\��
		m_firstBossName->SetShadowParam(true, 1.0f, Vector4::Black);
		swprintf_s(m_firstBossNameText, L"Boss");
		m_firstBossName->SetText(m_firstBossNameText);

		//���f����������
		m_model = NewGO<SkinModelRender>();
		//���f���̏������Z�A�b�v�Ɏw��
		m_model->SetModelUpAxis(enModelUpAxisZ);
		m_model->SetShadowCaster(true);	//�e�𗎂Ƃ�
		m_model->SetSilhouette(true);	//�V���G�b�g�𗎂Ƃ�
		m_model->SetPlayerMode(2);	//�G�p�̃V���G�b�g
		m_model->SetOutline(true);//�֊s��������
		//�A�j���[�V���������[�h
		for (int animationNum = enIdleAnimation; animationNum < enAnimationNum; animationNum++)
		{
			m_animationClips[animationNum].Load(nsFirstBoss::ANIMATION_FILE_PATH[animationNum]);
			m_animationClips[animationNum].SetLoopFlag(nsFirstBoss::ANIMATION_LOOP_FLAG[animationNum]);
		}
		//�G�̃��f������������
		m_model->Init(nsFirstBoss::BOSS_MODEL_FILE_PATH, nsFirstBoss::BOSS_MODEL_FILE_PATH, m_animationClips, enAnimationNum);
		//���f���̈ʒu���X�V
		m_pos = nsFirstBoss::INIT_POSITION;
		m_model->SetPosition(m_pos);
		//�L�����R����������
		m_charaCon.Init(100.0f, 100.0f, m_pos);
		//���ʃx�N�g�����v�Z
		m_forward = Vector3::AxisZ;
		m_rot.Apply(m_forward);

		m_model->SetAnimationSpeed(m_onlineUpdateSpeed->GetSpeed());
	}

	FirstBoss::~FirstBoss()
	{
		//���f���̍폜
		DeleteGO(m_model);
		//HP�o�[UI�̍폜
		for (int hpBarUINum = enHPBar; hpBarUINum < enHPBarUINum; hpBarUINum++)
		{
			DeleteGO(m_hpBarUI[hpBarUINum]);
		}

		DeleteGO(m_firstBossName);
	}

	void FirstBoss::SubUpdate()
	{
		//�n�ʂɂ��Ă��Ȃ��Ƃ��͈ړ����������Ȃ��B
		if (m_charaCon.IsOnGround())
		{
			switch (m_nowState)
			{
			//�A�C�h�����
			case enIdle:
				if (m_idleTimer == 0)
				{
					//���n���Đ�
					m_sound[enLandingSound] = NewGO<SoundSource>();
					m_sound[enLandingSound]->Init(L"Assets/sound/FirstBoss_Landing.wav");
					m_sound[enLandingSound]->SetVolume(0.5f);
					m_sound[enLandingSound]->Play(false);
				}
				m_idleTimer++;
				if (m_idleTimer > 60/ m_onlineUpdateSpeed->GetSpeed())
				{
					//�ʏ��Ԃɂ���
					m_nowState = enNormalState;
				}

				break;

				//�ʏ���
			case enNormalState:
				//�ړ����
				switch (m_moveState)
				{
					//�����_���ړ����
				case enRandomMoveState:
					//�����_���ړ�
					RandomMove();
					//�v���C���[�ʒu����
					DistanceSearch();

					break;

					//�_�b�V���U��
				case enDashAttackState:
					if (m_moveStopTimer == 0)
					{
						//�`���[�W���Đ�
						m_sound[enChargeSound] = NewGO<SoundSource>();
						m_sound[enChargeSound]->Init(L"Assets/sound/FirstBoss_Charge.wav");
						m_sound[enChargeSound]->SetVolume(0.5f);
						m_sound[enChargeSound]->Play(false);
					}
					m_moveStopTimer++;
					if (m_moveStopTimer >= 120/ m_onlineUpdateSpeed->GetSpeed())
					{
						if (m_moveStopTimer == 120/ m_onlineUpdateSpeed->GetSpeed())
						{
							m_moveSpeed *= 20.0f;

							//�ːi���Đ�
							m_sound[enDashAttackSound] = NewGO<SoundSource>();
							m_sound[enDashAttackSound]->Init(L"Assets/sound/FirstBoss_DashAttack.wav");
							m_sound[enDashAttackSound]->SetVolume(0.5f);
							m_sound[enDashAttackSound]->Play(false);
						}
						//�ڐG������Ȃ���
						SetCanHitBody(false);

						//�ǂɓ���������A���A�������Ă��Ȃ��Ƃ��A
						if (m_charaCon.IsHitWall() && m_confusionStar == nullptr)
						{
							if (m_animState != enIdleAnimation)
							{
								//�������

								//�G���~�߂�
								m_moveSpeed.Normalize();
								m_moveSpeed /= 10.0f;
								//�A�j���[�V�������A�C�h����Ԃɂ���
								m_animState = enIdleAnimation;
								//�������ł���Ƃ��A
								if (m_canConfusion)
								{
									//�������f����\��
									m_confusionStar = NewGO<ConfusionStar>();
									m_confusionStar->SetPosition(m_pos);

									//�����ł��Ȃ��悤�ɂ���
									m_canConfusion = false;
								}
							}
						}
						//�������Ă���Ƃ��A
						if (m_confusionStar != nullptr)
						{
							m_confusionTimer++;
							if (m_confusionTimer == 300/ m_onlineUpdateSpeed->GetSpeed())
							{
								//�����A�j���[�V�����ɂ���
								m_animState = enWalkAnimation;
							}
							if (m_confusionTimer > 500/ m_onlineUpdateSpeed->GetSpeed())
							{
								//�������f�����폜
								DeleteGO(m_confusionStar);
								m_confusionStar = nullptr;
								//������Ԃ�����(�����_���ړ��ɂȂ�)
								m_moveState = enRandomMoveState;
								//�^�C�}�[��������
								m_confusionTimer = 0;
								//�����ł���悤�ɂ���
								m_canConfusion = true;
								//�ڐG����𕜊�����r��
								SetCanHitBody(true);
								//�ړ��X�g�b�v�^�C�}�[��������
								m_moveStopTimer = 0;
							}
						}
					}

					break;

					//�p�グ�U�����
				case enAttackHornsUpState:
					if (m_moveStopTimer == 0)
					{
						//�p�グ�U�����Đ�
						m_sound[enAttackHornsUpSound] = NewGO<SoundSource>();
						m_sound[enAttackHornsUpSound]->Init(L"Assets/sound/FirstBoss_Attack.wav");
						m_sound[enAttackHornsUpSound]->SetVolume(0.5f);
						m_sound[enAttackHornsUpSound]->Play(false);
					}
					m_moveStopTimer++;
					if (m_moveStopTimer >= 60/ m_onlineUpdateSpeed->GetSpeed())
					{
						//�A�j���[�V�������p�グ�U���ɐ؂�ւ�
						m_animState = enAttackHornsUpAnimation;

						if (IsAttackRangeIn())
						{
							//1�{�X�̐��ʂɓ˂���΂�
							m_player[m_nearPlayer]->SetMoveSpeed(m_forward * 10.0f * m_onlineUpdateSpeed->GetSpeed());
						}

						//�A�j���[�V�������I��������A
						if (!m_model->IsPlaying())
						{
							m_moveStopTimer++;
							if (m_moveStopTimer > 120/ m_onlineUpdateSpeed->GetSpeed())
							{
								//�����_���ړ���Ԃɂ���
								m_moveState = enRandomMoveState;
								//�����_���ړ��^�C�}�[��������
								m_randomMoveTimer = 0;
								//�ړ��X�g�b�v�^�C�}�[��������
								m_moveStopTimer = 0;
							}
						}
					}

					break;

					//�p�˂��U�����
				case enAttackHornsForwardState:
					if (m_moveStopTimer == 0)
					{
						//�p�˂��U�����Đ�
						m_sound[enAttackHornsForwardSound] = NewGO<SoundSource>();
						m_sound[enAttackHornsForwardSound]->Init(L"Assets/sound/FirstBoss_Attack.wav");
						m_sound[enAttackHornsForwardSound]->SetVolume(0.5f);
						m_sound[enAttackHornsForwardSound]->Play(false);
					}
					m_moveStopTimer++;
					if (m_moveStopTimer >= 60/ m_onlineUpdateSpeed->GetSpeed())
					{
						//�A�j���[�V�������p�˂��U���ɐ؂�ւ�
						m_animState = enAttackHornsForwardAnimation;

						if (IsAttackRangeIn())
						{
							//1�{�X�̐��ʂɓ˂���΂�
							m_player[m_nearPlayer]->SetMoveSpeed(m_forward * 10.0f * m_onlineUpdateSpeed->GetSpeed());
						}

						//�A�j���[�V�������I��������A
						if (!m_model->IsPlaying())
						{
							m_moveStopTimer2++;
							if (m_moveStopTimer2 > 120/ m_onlineUpdateSpeed->GetSpeed())
							{
								//�����_���ړ���Ԃɂ���
								m_moveState = enRandomMoveState;
								//�����_���ړ��^�C�}�[��������
								m_randomMoveTimer = 0;
								//�ړ��X�g�b�v�^�C�}�[��������
								m_moveStopTimer = 0;
								m_moveStopTimer2 = 0;
							}
						}
					}

					break;
				}

				//�U�����󂯂����A
				if (GetReceiveAttackFlg())
				{
					//�q�b�g�|�C���g���P���炷
					m_hitPoint--;
				}
				//HP���O�ȉ��ɂȂ�Ȃ��悤�ɕ␳
				m_hitPoint = int(max(0.0f, m_hitPoint));
				//HP���O�ɂȂ����玀��
				if (m_hitPoint == 0)
				{
					//���S�A�j���[�V�������Đ�
					m_animState = enDeathAnimation;
					//���S���
					m_nowState = enDeathState;
					//�G��|��������+1����
					m_playerStatus[GetLastKillPlayer()]->AddEnemyKillNum();
					//�ړ����x���O�ɂ���
					m_moveSpeed = Vector3::Zero;
					//�ڐG�͈͂��Z�b�g
					SetHitBodyLength(1.0f);
				}

				break;

				//���S���
			case enDeathState:
				//���񂾂Ƃ��ɍ����������ĂȂ�������A
				if (m_confusionStar != nullptr)
				{
					//�������f�����폜
					DeleteGO(m_confusionStar);
				}
				//���S�A�j���[�V�������I��������A
				if (!m_model->IsPlaying())
				{
					m_gameClearFlg = true;
				}

				break;
			}
		}

		//HP�o�[�X�V����
		m_hpBarUI[enRemainingHP]->SetScale({ m_hitPoint / nsFirstBoss::MAX_HITPOINT ,1.0f,1.0f });

		//�d��
		Gravity();

		//�X�V����
		DataUpdate();
	}

	void FirstBoss::UiUpdate()
	{
		if (m_nowState == enDeathState)
		{
			return;
		}
		//�A�C�h���A�j���[�V�����ɂ���
		m_animState = enIdleAnimation;
		//�A�j���[�V�����Đ�
		m_model->PlayAnimation(m_animState);
	}

	//�X�V�������\�b�h
	void FirstBoss::DataUpdate()
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

		//���ʃx�N�g�����v�Z
		m_forward = Vector3::AxisZ;
		m_rot.Apply(m_forward);

		//�U�����󂯂�͈�
		SetAttackReceiveLength(270.0f);
		//�ʒu���Z�b�g
		SetPosition(m_pos);

		//�A�j���[�V�����Đ�
		m_model->PlayAnimation(m_animState);
	}

	//�����_���ړ��������\�b�h
	void FirstBoss::RandomMove()
	{
		//�����_���Ɉړ�����������
		if (m_randomMoveTimer % 120 == 0)
		{
			//120�t���[���ňړ�������ύX
			m_moveSpeed.x = (rand() % 100) / 100.0f;
			m_moveSpeed.x -= 0.5f;
			m_moveSpeed.z = (rand() % 100) / 100.0f;
			m_moveSpeed.z -= 0.5f;
			//���K��
			m_moveSpeed.Normalize();
			//�ړ����x��������
			m_moveSpeed *= nsFirstBoss::RANDOM_MOVE_POWER * m_onlineUpdateSpeed->GetSpeed();
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

		//�A�j���[�V����������ɐ؂�ւ�
		m_animState = enWalkAnimation;
	}

	//�v���C���[�Ƃ̋����������\�b�h
	void FirstBoss::DistanceSearch()
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
			switch (rand() % 10)
			{
			case 0:
			case 1:
			case 2:
				//�p�グ�A�j���[�V�����ɂ���
				m_animState = enAttackHornsUpAnimation;
				//�p�グ�U����Ԃɂ���
				m_moveState = enAttackHornsUpState;

				break;

			case 3:
			case 4:
			case 5:
				//�p�˂��A�j���[�V�����ɂ���
				m_animState = enAttackHornsForwardAnimation;
				//�p�˂��U����Ԃɂ���
				m_moveState = enAttackHornsForwardState;

				break;

			//�����_���ړ��𑱍s
			default:
				return;
			}
			//�������v���C���[�̕����Ɍ�������
			m_moveSpeed = m_playerToEnemyDistanceDirecion[m_nearPlayer];
			//�ړ����x�𗎂Ƃ�
			m_moveSpeed.Normalize();
			m_moveSpeed /= 100.0f;
		}
		else
		{
			switch (rand() % int(200/ m_onlineUpdateSpeed->GetSpeed()))
			{
			case 0:
				//�_�b�V���U����Ԃɂ���
				m_moveState = enDashAttackState;
				//�v���C���[�����Ɍ�������
				m_moveSpeed = m_playerToEnemyDistanceDirecion[m_nearPlayer];
				//�A�j���[�V�����𑖂�U���ɐ؂�ւ�
				m_animState = enRunAnimation;

				break;
			}
		}
	}

	//�U���͈͓��ɓ�������t���O�I�����郁�\�b�h
	bool FirstBoss::IsAttackRangeIn()
	{
		//�v���C���[�Ƃ̋������߂�������A
		if (m_plaToEneDistanceLength[m_nearPlayer] < nsFirstBoss::ATTACK_RANGE)
		{
			//�G�l�~�[�̐��ʂ̃x�N�g���ƁA�G�l�~�[����v���C���[�Ɍ������x�N�g���́B
			//����(cos��)�����߂�B
			float cos = m_forward.Dot(m_playerToEnemyDistanceDirecion[m_nearPlayer]);
			//����(cos��)����p�x(��)�����߂�B
			float angle = acosf(cos);
			//�p�x(��)��60����菬������΁B
			if (angle <= (Math::PI / 180.0f) * 60.0f)
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