/// <summary>
/// ����N���X
/// </summary>
#include "stdafx.h"
#include "Weapon.h"
#include "../../Player/Player.h"
#include "../../Player/PlayerStatus.h"

namespace nsKabutoubatu
{
	namespace nsWeapon
	{
		const char WEAPON_FILEPATH[4][256] = { "WoodStick","Sword","LargeSword","Bow" };
		const int WEAPON_ATTACK_POWER[4] = {1,2,4,2};
		const int ATTACK_JUDGE_TIME[4] = {30,30,70,99999};
	}

	bool Weapon:: Start()
	{
		//�v���C���[�̃C���X�^���X������
		m_player = FindGO<Player>(nsStdafx::PLAYER_NAME[m_playerNum]);
		m_playerStatus = FindGO<PlayerStatus>(nsStdafx::PLAYER_STATUS_NAME[m_playerNum]);

		//���탂�f���̏�����
		Init(m_weaponNum);

		//�E��{�[��������
		m_rightHandBoneId = m_player->GetSkeleton()->FindBoneID(L"Character1_RightHandMiddle1");
		// ���������{�[��ID���g���āA�{�[�����擾����B
		m_rightHandBone = m_player->GetSkeleton()->GetBone(m_rightHandBoneId);

		//����{�[��������
		m_leftHandBoneId = m_player->GetSkeleton()->FindBoneID(L"Character1_LeftHandMiddle1");
		// ���������{�[��ID���g���āA�{�[�����擾����B
		m_leftHandBone = m_player->GetSkeleton()->GetBone(m_leftHandBoneId);

		return true;
	}

	Weapon::~Weapon()
	{
		//���f�����폜
		DeleteGO(m_model);
	}

	void Weapon::Update()
	{
		//�{�[���̃��[���h���W�E��]�E�g�嗦���X�V
		for (int boneNum = 0; boneNum < m_player->GetSkeleton()->GetNumBones(); boneNum++)
		{
			m_player->GetSkeleton()->GetBone(boneNum)->CalcWorldTRS(m_pos, m_rot, m_sca);
		}

		if (m_weaponNum == enBow)
		{
			//�擾�����{�[���̃��[���h���W���擾
			m_pos = m_leftHandBone->GetPosition();
			//�擾�����{�[���̉�]���擾
			m_rot = m_leftHandBone->GetRotation();
		}
		else
		{
			//�擾�����{�[���̃��[���h���W���擾
			m_pos = m_rightHandBone->GetPosition();
			//�擾�����{�[���̉�]���擾
			m_rot = m_rightHandBone->GetRotation();
		}

		//�ʒu�X�V
		m_model->SetPosition(m_pos);
		//��]�X�V
		m_model->SetRotation(m_rot);

		m_playerStatus->ChangeHavingWeapon(m_weaponNum);

		//���킲�Ƃ̍U���q�b�g�^�C�~���O��ݒ�
		m_attackJudgeTimer = nsWeapon::ATTACK_JUDGE_TIME[m_weaponNum];

		//�΂̋ʒ��͕���̃��f�����B��
		if (m_player->GetFireBallActive())
		{
			//TODO:���f���������Ȃ��悤�ɂ���B
			m_model->SetFireBall(true);
		}
		else
		{
			//TODO:���f����������悤�ɂ���B
			m_model->SetFireBall(false);
		}
	}

	void Weapon::UiUpdate()
	{
		//�{�[���̃��[���h���W�E��]�E�g�嗦���X�V
		for (int boneNum = 0; boneNum < m_player->GetSkeleton()->GetNumBones(); boneNum++)
		{
			m_player->GetSkeleton()->GetBone(boneNum)->CalcWorldTRS(m_pos, m_rot, m_sca);
		}

		if (m_weaponNum == enBow)
		{
			//�擾�����{�[���̃��[���h���W���擾
			m_pos = m_leftHandBone->GetPosition();
			//�擾�����{�[���̉�]���擾
			m_rot = m_leftHandBone->GetRotation();
		}
		else
		{
			//�擾�����{�[���̃��[���h���W���擾
			m_pos = m_rightHandBone->GetPosition();
			//�擾�����{�[���̉�]���擾
			m_rot = m_rightHandBone->GetRotation();
		}

		//�ʒu�X�V
		m_model->SetPosition(m_pos);
		//��]�X�V
		m_model->SetRotation(m_rot);

		m_playerStatus->ChangeHavingWeapon(m_weaponNum);

		//���킲�Ƃ̍U���q�b�g�^�C�~���O��ݒ�
		m_attackJudgeTimer = nsWeapon::ATTACK_JUDGE_TIME[m_weaponNum];
	}

	void Weapon::Init(const int weaponNum)
	{
		//�O�̃��f�����폜
		if (m_model != nullptr)
		{
			DeleteGO(m_model);
		}

		m_weaponNum = weaponNum;

		//���f����������
		m_model = NewGO<SkinModelRender>();
		m_model->SetShadowCaster(true);	//�e�𗎂Ƃ�
		m_model->SetOutline(false);		//�֊s�������Ȃ�
		m_model->SetSilhouette(true);		//�V���G�b�g������
		m_model->SetPlayerMode(m_playerNum);	//�v���C���[�P���Q����n��

		//����t�@�C���p�X
		m_weaponModelName = nsWeapon::WEAPON_FILEPATH[m_weaponNum];
		//�U����
		m_attackDamage = nsWeapon::WEAPON_ATTACK_POWER[m_weaponNum];

		m_model->Init(m_weaponModelName);
	}
}