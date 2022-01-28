/// <summary>
/// �v���C���[�N���X
/// </summary>
#include "stdafx.h"
#include "Player.h"
#include "../Item/RecoveryItem.h"
#include "../../Graphics/MiniMap.h"
#include "PlayerRotation.h"
#include "PlayerAnimation.h"
#include "PlayerSound.h"
#include "PlayerCharaCon.h"
#include "PlayerStatus.h"
#include "../Item/Weapon/Arrow.h"

namespace nsKabutoubatu
{
	namespace nsPlayer
	{
		//�v���C���[�P�̏����ʒu
		const Vector3 PLAYER1_START_POS = { 70.0f,0.0f,0.0f };
		//�v���C���[�Q�̏����ʒu
		const Vector3 PLAYER2_START_POS = { -70.0f,0.0f,0.0f };
		//�S�[�X�g��Ԃ̈ړ���
		const float GHOST_MOVE_POWER = 400.0f;
		//�ʏ�̖��C��
		const float NORMAL_FRICTION_POWER = 5.0f;
		//�����C��
		const float SUPER_FRICTION_POWER = 30.0f;
	}

	bool Player::Start()
	{
		m_miniMap = FindGO<MiniMap>(nsStdafx::MINIMAP_NAME);

		//�v���C���[�̉�]�N���X
		m_playerRotation = NewGO< PlayerRotation >(nsStdafx::PRIORITY_0,nsStdafx::PLAYER_ROTATION_NAME[m_playerNum]);
		m_playerRotation->SetPlayerNum(m_playerNum);
		//�v���C���[�̃A�j���[�V�����N���X
		m_playerAnimation = NewGO< PlayerAnimation > (nsStdafx::PRIORITY_0, nsStdafx::PLAYER_ANIMATION_NAME[m_playerNum]);
		m_playerAnimation->SetPlayerNum(m_playerNum);
		//�v���C���[�̃T�E���h�N���X
		m_playerSound = NewGO< PlayerSound >(nsStdafx::PRIORITY_0,nsStdafx::PLAYER_SOUND_NAME[m_playerNum]);
		//�v���C���[�̃L�����R���N���X
		m_playerCharaCon = NewGO<PlayerCharaCon >(nsStdafx::PRIORITY_0, nsStdafx::PLAYER_CHARACON_NAME[m_playerNum]);
		m_playerCharaCon->InitRigidBody(m_pos);
		//�v���C���[�̃X�e�[�^�X�N���X
		m_playerStatus = NewGO< PlayerStatus >(nsStdafx::PRIORITY_0,nsStdafx::PLAYER_STATUS_NAME[m_playerNum]);
		m_playerStatus->SetPlayerNum(m_playerNum);

		//���f����������
		m_model = NewGO<SkinModelRender>();
		m_model->SetShadowCaster(true);	//�e�𗎂Ƃ�
		m_model->SetSilhouette(true);	//�V���G�b�g�𗎂Ƃ�
		m_model->SetOutline(true);//�֊s��������
		//�����̃v���C���[�̏�Ԃ͒ʏ�
		m_nowState = enNormal;

		switch (m_playerNum)
		{
		//�PP
		case enPlayer1:

			//�����̏����擾�ł���悤�ɂ���
			if (!m_isSoloPlay)
			{
				m_otherPlayer = FindGO<Player>(nsStdafx::PLAYER_NAME[enPlayer2]);
			}

			m_model->SetPlayerMode(enPlayer1);	//�PP���Ƃ������Ƃ�fx�ɓn�����߂̃Z�b�g���\�b�h
			//Z�A�b�v�ɕύX
			m_model->SetModelUpAxis(enModelUpAxisZ);
			m_model->Init("Player1", "Player1", m_playerAnimation->GetAnimationClips(), m_playerAnimation->GetAnimationNum());
			m_pos = nsPlayer::PLAYER1_START_POS;
			m_playerCharaCon->InitRigidBody(m_pos);
			DataUpdate();

			//�΂̋ʃG�t�F�N�g�̏�����
			m_fireBallEffect.Init(u"Assets/effect/efk/FireBall_Red.efk");

			break;

		//�QP
		case enPlayer2:

			//�PP�̏����擾�ł���悤�ɂ���
			if (!m_isSoloPlay)
			{
				m_otherPlayer = FindGO<Player>(nsStdafx::PLAYER_NAME[enPlayer1]);
			}

			m_model->SetPlayerMode(enPlayer2);	//2P���Ƃ������Ƃ�fx�ɓn�����߂̃Z�b�g���\�b�h
			//Z�A�b�v�ɕύX
			m_model->SetModelUpAxis(enModelUpAxisZ);
			m_model->Init("Player2", "Player2", m_playerAnimation->GetAnimationClips(), m_playerAnimation->GetAnimationNum());
			m_pos = nsPlayer::PLAYER2_START_POS;
			m_playerCharaCon->InitRigidBody(m_pos);
			DataUpdate();

			//�΂̋ʃG�t�F�N�g�̏�����
			m_fireBallEffect.Init(u"Assets/effect/efk/FireBall_Blue.efk");

			break;
		}

		return true;
	}

	Player::~Player()
	{
		//���f�����폜
		DeleteGO(m_model);
		//�񕜃A�C�e�����폜
		if (m_recoveryItem != nullptr)
		{
			DeleteGO(m_recoveryItem);
		}
		if (!m_isSoloPlay)
		{
			DeleteGO(m_otherPlayer);
		}

		//�v���C���[�̉�]�N���X���폜
		DeleteGO(m_playerRotation);
		//�v���C���[�̃A�j���[�V�����N���X���폜
		DeleteGO(m_playerAnimation);
		//�v���C���[�̃T�E���h�N���X���폜
		DeleteGO(m_playerSound);
		//�v���C���[�̃L�����R���N���X���폜
		DeleteGO(m_playerCharaCon);
		//�v���C���[�̃X�e�[�^�X�N���X���폜
		DeleteGO(m_playerStatus);

		//�}�b�v��\��
		m_miniMap->Deactivate();
	}

	void Player::Update()
	{
		switch (m_nowState)
		{
		//�ʏ���
		case enNormal:
			//�U��������Ȃ��Ƃ��ʏ�ړ��ł���B
			if (!m_isAttackStateFlg)
			{
				//�ʏ�ړ���Ԏ��̃A�j���[�V��������
				m_playerAnimation->NormalStateAnimation();
				//�ʏ�ړ�
				NormalMove();
				//�W�����v
				Jump();
				//���
				Avoidance();
				//�񕜂̈�A�̏���
				Recovery();
			}
			//�U�����̎��A
			else
			{
				//�U���A�j���[�V�������؂ꂽ��U�����t���O��܂�
				if (!m_model->IsPlaying())
				{
					m_isAttackStateFlg = false;
				}
			}
			//�ʏ�̖��C�͂�������
			Friction(nsPlayer::NORMAL_FRICTION_POWER);

			//�c��̗͂𒲂ׂāA�c��̗͂��O�ɂȂ�����A
			if (m_playerStatus->GetHitPoint() == 0)
			{
				//�S�[�X�g��ԂɂȂ�B
				m_nowState = enGhost;
				//�S�[�X�g�Ȃ̂ŉe�𗎂Ƃ��Ȃ��悤�ɂ���
				m_model->SetShadowCaster(false);
				//�S�[�X�g��ԂȂ̂ŃL�����R�����폜
				m_playerCharaCon->RemoveRigidBoby();
			}

			//�����������\�b�h
			Fall();

			break;

		//�U�����󂯂����
		case enSufferAnAttack:
			//�A�j���[�V�������W�����v�ɐ؂�ւ�
			m_playerAnimation->ChangeAnimation(enPlayerAnimation::enAnimationClip_jump);
			//�ʏ�̖��C�͂�������
			Friction(nsPlayer::NORMAL_FRICTION_POWER);
			//�ړ����x�𒲂ׁA������x�܂ňړ����x����������A
			if (m_moveSpeed.Length() < 1.0f)
			{
				//�c��̗͂𒲂ׂāA�c��̗͂��O�ɂȂ�����A
				if (m_playerStatus->GetHitPoint() == 0)
				{
					//�S�[�X�g��ԂɂȂ�B
					m_nowState = enGhost;
					//�S�[�X�g�Ȃ̂ŉe�𗎂Ƃ��Ȃ��悤�ɂ���
					m_model->SetShadowCaster(false);
					//�S�[�X�g��ԂȂ̂ŃL�����R�����폜
					m_playerCharaCon->RemoveRigidBoby();
					//�f�X���𑝂₷
					m_playerStatus->AddDeathNum();

					m_damageTimer = 0;
					m_damageFlg = false;
				}
				//�c��̗͂��O����Ȃ��Ƃ��́A
				else
				{
					//�ʏ��Ԃɖ߂�
					m_nowState = enNormal;

					m_damageTimer = 0;
					m_damageFlg = false;
				}
			}

			//�񕜃A�C�e�������L�����Z������鏈��
			//�񕜃A�C�e�����폜
			if (m_recoveryItem != nullptr)
			{
				RecoveryItemDelete();
			}

			//��������
			Fall();

			break;

			//�S�[�X�g���
		case enGhost:
			if (m_model->GetFireBall())
			{
				if (m_playerStatus->GetHitPoint() > 0)
				{
					//���f����������悤�ɂ���B
					m_model->SetFireBall(false);
					//�ʏ��Ԃɖ߂�
					m_nowState = enNormal;
					//�e�𗎂Ƃ��悤�ɂ���
					m_model->SetShadowCaster(true);
					//�L�����R�����ď�����
					m_playerCharaCon->InitRigidBody(m_pos);
					//�ړ����x��������
					m_moveSpeed = Vector3::Zero;
					//�΂̋ʂ�����
					m_fireBallEffect.Stop();
					m_fireBallEffectTimer = 0;
					m_fireBallEffectActive = false;
				}
				//A�{�^���������ꂽ��A
				if (m_gamePad->IsTrigger(enButtonA))
				{
					if (!m_isSoloPlay)
					{
						//�����v���C���[��HP���R�ȏ�Ȃ�HP���n�\
						if (m_otherPlayer->m_playerStatus->GetHitPoint() >= 3)
						{
							//�����̗̑͂��Q���炷
							m_otherPlayer->m_playerStatus->ReduceHitPoint(2);
							//���񕜂��Ă�������ʂ��Z�b�g����
							m_playerStatus->AddRecoveryReceiveNum(2);
							//�����̗̑͂��P�ɂ���
							m_playerStatus->SetHitPoint(1);
							//���f����������悤�ɂ���B
							m_model->SetFireBall(false);
							//�ʏ��Ԃɖ߂�
							m_nowState = enNormal;
							//�e�𗎂Ƃ��悤�ɂ���
							m_model->SetShadowCaster(true);
							//�L�����R�����ď�����
							m_playerCharaCon->InitRigidBody(m_pos);
							//�ړ����x��������
							m_moveSpeed = Vector3::Zero;
							//�΂̋ʂ�����
							m_fireBallEffect.Stop();
							m_fireBallEffectTimer = 0;
							m_fireBallEffectActive = false;
						}
					}
				}
			}

			if (m_fireBallEffectActive == false)
			{
				//�_�E���A�j���[�V�����ɂ���
				m_playerAnimation->ChangeAnimation(enPlayerAnimation::enAnimationClip_down);
				//�A�j���[�V�������x���������߂R�{����
				m_model->SetAnimationSpeed(3.0f);
			}
			else
			{
				//�S�[�X�g��Ԃł̈ړ�
				GhostMove();
				m_model->SetAnimationSpeed(1.0f);

				//�����A�j���[�V�����ɂ���
				m_playerAnimation->ChangeAnimation(enPlayerAnimation::enAnimationClip_walk);

				//�G�t�F�N�g���Đ�
				if (m_fireBallEffectTimer == 0)
				{
					m_fireBallEffect.Play();
				}
				m_fireBallEffectTimer++;
				if (m_fireBallEffectTimer > 200)
				{
					m_fireBallEffectTimer = 0;
				}
			}
			//�_�E���A�j���[�V�������I��������A
			if (!m_model->IsPlaying()&& m_nowState != enNormal)
			{
				//���f���������Ȃ��悤�ɂ���B
				m_model->SetFireBall(true);

				m_fireBallEffectActive = true;
			}

			break;

		//�������Ȃ����
		case enDontMove:
			break;
		}

		//�_���[�W���󂯂���G�t�F�N�g���o���B
		DamageEffect();

		//�S�[�X�g��Ԃ���Ȃ��Ƃ��͏d�͂��󂯁A�L�����R���œ���
		if (m_nowState != enGhost)
		{
			//�d��
			Gravity(m_gravityState);
		}
		//�f�[�^���X�V
		DataUpdate();

		//�~�j�}�b�v���J������
		MiniMapOpen();
	}

	void Player::UiUpdate()
	{
		//�A�C�h���A�j���[�V�����ɂ���
		m_playerAnimation->ChangeAnimation(enPlayerAnimation::enAnimationClip_idle);
		//�A�j���[�V�����̏�ԍX�V
		m_model->PlayAnimation(m_playerAnimation->GetAnimationState());

	}

	//���C���\�b�h
	void Player::Friction(const float frictionPower)
	{
		//���C�͂�ݒ肷��
		m_friction = m_moveSpeed;
		m_friction *= -frictionPower;

		//���C�͂����Z����
		m_moveSpeed.x += m_friction.x * g_gameTime->GetFrameDeltaTime();
		m_moveSpeed.z += m_friction.z * g_gameTime->GetFrameDeltaTime();
	}

	//�ړ����\�b�h
	void Player::NormalMove()
	{
		//�匕�������Ă���Ƃ��͈ړ����x�ቺ
		if (m_playerStatus->GetHaveWeapon() == enLargeSword)
		{
			//���X�e�B�b�N�̓��͗ʂ����Z����
			m_moveSpeed.x -= m_gamePad->GetLStickXF() * m_playerStatus->GetMovePower()/2 * g_gameTime->GetFrameDeltaTime();
			m_moveSpeed.z -= m_gamePad->GetLStickYF() * m_playerStatus->GetMovePower() /2 * g_gameTime->GetFrameDeltaTime();
		}
		else
		{
			//���X�e�B�b�N�̓��͗ʂ����Z����
			m_moveSpeed.x -= m_gamePad->GetLStickXF() * m_playerStatus->GetMovePower() * g_gameTime->GetFrameDeltaTime();
			m_moveSpeed.z -= m_gamePad->GetLStickYF() * m_playerStatus->GetMovePower() * g_gameTime->GetFrameDeltaTime();
		}
	}

	//�S�[�X�g��Ԃ̎��̉E�X�e�B�b�N�ŏ㉺�ړ����郁�\�b�h
	void Player::GhostMove()
	{
		//���X�e�B�b�N�̓��͗ʂ����ړ��ɉ��Z����
		m_pos.x -= m_gamePad->GetLStickXF() * nsPlayer::GHOST_MOVE_POWER * g_gameTime->GetFrameDeltaTime();
		m_pos.z -= m_gamePad->GetLStickYF() * nsPlayer::GHOST_MOVE_POWER * g_gameTime->GetFrameDeltaTime();
		//�E�X�e�B�b�N�̓��͗ʂ��㉺�ړ�
		m_pos.y += m_gamePad->GetRStickYF() * nsPlayer::GHOST_MOVE_POWER * g_gameTime->GetFrameDeltaTime();

		//�v���C���[����]���������̂Œʏ�ړ��̏������u���Ă����B
		//(����͈ړ��͂�����邱�Ƃ͂Ȃ�)
		NormalMove();
	}

	//�W�����v���\�b�h
	void Player::Jump()
	{
		//�󒆂ɂ���Ƃ��̓W�����v�ł��Ȃ�
		if (m_playerCharaCon->IsJump())
		{
			return;
		}

		//B�{�^���ŃW�����v
		if (m_gamePad->IsTrigger(enButtonB))
		{
			//�W�����v��
			m_playerSound->PlayJumpSound();

			//�A�j���[�V�������W�����v�ɐ؂�ւ�
			m_playerAnimation->ChangeAnimation(enPlayerAnimation::enAnimationClip_jump);

			m_moveSpeed.y = m_playerStatus->GetJumpPower();

			//�匕�������Ă���Ƃ��̓W�����v�͒ቺ
			if (m_playerStatus->GetHaveWeapon() == enLargeSword)
			{
				m_moveSpeed.y /= 2.0f;
			}
		}
	}

	//������\�b�h
	void Player::Avoidance()
	{
		if (m_gamePad->IsTrigger(enButtonLB1)&& !m_isAvoidance && m_playerCharaCon->GetCharaCon()->IsOnGround())
		{
			m_playerAnimation->ChangeAnimation(enPlayerAnimation::enAnimationClip_rollingAvoidance);
			//����̈ړ����x����Z
			m_moveSpeed.x *= 3.0f;
			m_moveSpeed.z *= 3.0f;

			//������ʉ���炷
			m_playerSound->PlayAvoidanceSound();

			//��𒆂ɂ���
			m_isAvoidance = true;
		}
		//����A�j���[�V�������I�������A
		if (!m_model->IsPlaying())
		{
			//����A�j���[�V�������甲����
			m_playerAnimation->ChangeAnimation(enPlayerAnimation::enAnimationClip_idle);

			//������Ă��Ȃ��悤�ɂ���
			m_isAvoidance = false;
		}
		if (m_isAvoidance && m_playerAnimation->GetAnimationState() != enPlayerAnimation::enAnimationClip_rollingAvoidance)
		{
			//������Ă��Ȃ��悤�ɂ���
			m_isAvoidance = false;
		}

		//����A�j���[�V������
		if (m_playerAnimation->GetAnimationState() == enPlayerAnimation::enAnimationClip_rollingAvoidance)
		{
			m_model->SetAnimationSpeed(1.5f);
		}
		//����A�j���[�V�����ȊO
		else
		{
			m_model->SetAnimationSpeed(1.0f);
		}

	}

	//�v���C���[�̃f�[�^���X�V���郁�\�b�h
	//�ʒu�E��]�E�L�����R���E�A�j���[�V�������
	void Player::DataUpdate()
	{
		//�A�j���[�V�����̏�ԍX�V
		m_model->PlayAnimation(m_playerAnimation->GetAnimationState());

		//�΂̋ʍX�V
		m_fireBallEffect.SetPosition({ m_pos.x,m_pos.y+40.0f,m_pos.z });
		m_fireBallEffect.Update();

		//�S�[�X�g��Ԃ���Ȃ��Ƃ�����
		//�ړ����J�n���Ă���L�����R���ړ�
		if (m_nowState != enGhost)
		{
			//�L�����N�^�[�R���g���[���[���g�����ړ������ɕύX�B
			m_pos = m_playerCharaCon->RigidBodyExecute(m_moveSpeed,m_isHitGround,m_hitGroundNormal);
		}
		//�ʒu���X�V
		m_model->SetPosition(m_pos);
		//��]���X�V
		m_model->SetRotation(m_playerRotation->GetRotation());

		//���ʃx�N�g�����v�Z
		m_forward = Vector3::AxisZ;
		m_playerRotation->GetRotation().Apply(m_forward);
	}

	//�񕜏������\�b�h
	void Player::Recovery()
	{
		//�P�ȏ㎝���Ă���Ƃ��̂݉񕜃A�C�e�����g����A
		if (m_playerStatus->GetRecoveryItemNum() != 0)
		{
			//�񕜃A�C�e�����������܂�����
			// �����鏈���͉񕜃A�C�e���N���X�ɏ����B
			//X�{�^���������ꂽ��A
			if (m_gamePad->IsTrigger(enButtonX))
			{
				//�񕜃A�C�e���������Ă��Ȃ��Ƃ��A
				if (!m_haveRecoveryItem)
				{
					//�񕜃A�C�e��������
					m_recoveryState = enRecoverySet;
					m_haveRecoveryItem = true;
				}
				//�񕜃A�C�e���������Ă���Ƃ��A
				else
				{
					//�������Ă��Ȃ��Ƃ��A
					if (m_recoveryState != enRecoveryThrow)
					{
						//�񕜃A�C�e�������܂�
						RecoveryItemDelete();

					}
				}
			}
			//�h�A���J���Ă��Ȃ��Ƃ��̂ݎ��s�ł���
			if (m_nowDoorTatch)
			{
				return;
			}
			//�U���{�^��(A�{�^��)����������A
			if (m_gamePad->IsTrigger(enButtonA))
			{
				//�񕜃A�C�e���������Ă���Ƃ�
				if (m_haveRecoveryItem)
				{
					//�񕜃A�C�e���𓊂��鉹
					m_playerSound->PlayRecoveryItemThrowSound();

					//��������Ԃɂ���
					m_recoveryState = enRecoveryThrow;

					//���������P���炷
					m_playerStatus->ReduceRecoveryItemNum();
				}
				//�����Ă��Ȃ��Ƃ��͍U�����ɂ���
				else
				{
					//�U����
					m_playerSound->SetAttackSoundFlg(m_playerStatus->GetHaveWeapon());

					//�U���A�j���[�V����
					switch (m_playerStatus->GetHaveWeapon())
					{
					//�؂̖_
					case enWoodStick:
						m_playerAnimation->ChangeAnimation(enPlayerAnimation::enAnimationClip_attack);

						break;

					//���ʂ̌�
					case enSword:
						m_playerAnimation->ChangeAnimation(enPlayerAnimation::enAnimationClips_swordAttack);

						break;

					//�匕
					case enLargeSword:
						m_playerAnimation->ChangeAnimation(enPlayerAnimation::enAnimationClips_largeSwordAttack);

						break;

					//�|
					case enBow:
						m_playerAnimation->ChangeAnimation(enPlayerAnimation::enAnimationClips_bowAttack);

						//����΂�
						ShotArrow();

						break;
					}

					//�U�����t���O���I��
					m_isAttackStateFlg = true;
				}
			}
		}

		switch (m_recoveryState)
		{
		case enRecoverySet:
			//�񕜃A�C�e���𐶐�
			m_recoveryItem = NewGO<RecoveryItem>();
			m_recoveryItem->SetIsSoloPlay(m_isSoloPlay);
			//�v���C���[�ԍ����Z�b�g
			m_recoveryItem->SetPlayerNum(m_playerNum);
			//�v���C���[�̉񕜗͂��Z�b�g
			m_recoveryItem->SetRecoveryPower(m_playerStatus->GetRecoveryPower());

			m_recoveryState = enRecoveryStay;
			break;

		case enRecoveryDelete:

			//�����ɓ������Ă����Ƃ��A
			if (m_recoveryItem->GetIsPlayerHit())
			{
				//�񕜉�
				m_playerSound->PlayRecoverySound();
			}

			//�񕜃A�C�e�����폜
			RecoveryItemDelete();

			break;

		case enRecoveryThrow:
			//�����C��������
			Friction(nsPlayer::SUPER_FRICTION_POWER);
			//��������̏����͉񕜃A�C�e���N���X�ɔC����B
			//�񕜃A�C�e�����폜�����^�C�~���O���C����B
			break;

		case enRecoveryStay:
			//�������Ȃ�
			break;
		}
	}

	//�񕜃A�C�e�������܂����\�b�h
	void Player::RecoveryItemDelete()
	{
		if (m_recoveryItem)
		{
			DeleteGO(m_recoveryItem);
		}
		m_haveRecoveryItem = false;
		m_recoveryState = enRecoveryStay;
	}

	//�����������\�b�h
	void Player::Fall()
	{
		//�n�ʂɂ��Ă���Ƃ��A
		if (m_playerCharaCon->IsOnGround())
		{
			//�ʒu��ۑ�
			m_finalOnGroundPos = m_pos;
			m_finalOnGroundPos.y += 200.0f;

			//�ʏ�͒ʏ�d��
			m_gravityState = enNormalGravity;
		}
		//�v���C���[���X�e�[�W���痎�������Ƃ��A
		if (-600.0f < m_pos.y && m_pos.y < -500.0f)
		{
			//�L�����R�����폜
			m_playerCharaCon->RemoveRigidBoby();
			m_pos = m_finalOnGroundPos;
			//�L�����R�����ď�����
			m_playerCharaCon->InitRigidBody(m_pos);
			//�̗͂��P���炷
			m_playerStatus->ReduceHitPoint(1);

			//���d�͂Ń��X�|�[��
			m_gravityState = enHalfGravity;
		}
	}

	//�v���C���[�̈ʒu���ړ�������
	void Player::SetPositionA(const Vector3& position)
	{
		m_pos = position;
		//�L�����R������U�j��
		m_playerCharaCon->RemoveRigidBoby();
		//�L�����R�����ď�����
		m_playerCharaCon->InitRigidBody(m_pos);
	};

	//�~�j�}�b�v���J�����\�b�h
	void Player::MiniMapOpen()
	{
		//�V���b�v���ɂ���Ƃ��̓}�b�v�͊J���Ȃ��B
		if (m_nowState == enDontMove)
		{
			//�}�b�v��\��
			m_miniMap->Deactivate();

			return;
		}
		//�X�^�[�g�{�^���Ń~�j�}�b�v���J��
		if (m_gamePad->IsTrigger(enButtonStart))
		{
			//�\������Ă���Ƃ��A
			if (m_miniMap->IsActive())
			{
				//�}�b�v��\��
				m_miniMap->Deactivate();
			}
			//�}�b�v����\���̂Ƃ��A
			else
			{
				//�}�b�v�\��
				m_miniMap->Activate();
			}
		}
	}

	//����΂��֐�
	void Player::ShotArrow()
	{
			//��𐶐�
			m_arrow = NewGO<Arrow>(nsStdafx::PRIORITY_0,nsStdafx::ARROW_NAME);
			m_arrow->SetPlayerNum(m_playerNum);		//�v���C���[�ԍ���n��
			m_arrow->SetShotPosition({ m_pos.x,m_pos.y+50.0f,m_pos.z });			//�ˏo�ʒu��n��
			m_arrow->SetMoveDirection(m_moveSpeed);	///�ړ�������n��
	}

	void Player::Gravity(const int gravityPower)
	{
		switch (gravityPower)
		{
		case enNormalGravity:
			m_moveSpeed.y -= nsStdafx::GRAVITY;
			break;
		case enHalfGravity:
			m_moveSpeed.y -= nsStdafx::GRAVITY / 5;
			break;
		}
	}

	void Player::DamageEffect()
	{
		//�_���[�W���󂯂����̐ԐF
		if (m_damageFlg)
		{
			m_damageTimer++;
		}
		if (m_damageTimer == 1)
		{
			m_model->SetDameageRed(true);
		}
		//�^�C�}�[��10�ɂȂ�����A
		if (m_damageTimer > 10)
		{
			//�_���[�W���󂯂����̐ԐF���Ȃ���
			m_model->SetDameageRed(false);
			m_damageFlg = false;
		}
	}
};