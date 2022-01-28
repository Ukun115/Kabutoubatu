/// <summary>
/// �\�����[�h���̓��sAI(�ȍ~�u���Ƃ��v�ƌĂ�)�̌��N���X
/// </summary>
#include "stdafx.h"
#include "AccompanyAISword.h"
#include "AccompanyAI.h"

namespace nsKabutoubatu
{
	bool AccompanyAISword::Start()
	{
		//���sAI(���Ƃ�)�̃C���X�^���X������
		m_accompaanyAi = FindGO<AccompanyAI>(nsStdafx::ACCOMPANYAI_NAME);

		//���f����������
		m_model = NewGO<SkinModelRender>();
		m_model->SetShadowCaster(true);	//�e�𗎂Ƃ�
		m_model->SetOutline(false);		//�֊s�������Ȃ�
		m_model->SetSilhouette(true);		//�F�V���G�b�g������
		m_model->SetPlayerMode(1);
		//Z�A�b�v�ɕύX
		m_model->SetModelUpAxis(enModelUpAxisZ);
		m_model->Init("AccompanyAI_Sword");

		//�E��{�[��������
		m_rightHandBoneId = m_accompaanyAi->GetSkeleton()->FindBoneID(L"mixamorig:RightHand");
		// ���������{�[��ID���g���āA�{�[�����擾����B
		m_rightHandBone = m_accompaanyAi->GetSkeleton()->GetBone(m_rightHandBoneId);

		return true;
	}

	AccompanyAISword::~AccompanyAISword()
	{
		//���f�����폜
		DeleteGO(m_model);
	}

	void AccompanyAISword::Update()
	{
		if (m_accompaanyAi->GetMode() == enAttackMode)
		{
			m_model->Activate();
		}
		else
		{
			m_model->Deactivate();
		}

		//�{�[���̃��[���h���W�E��]�E�g�嗦���X�V
		for (int boneNum = 0; boneNum < m_accompaanyAi->GetSkeleton()->GetNumBones(); boneNum++)
		{
			m_accompaanyAi->GetSkeleton()->GetBone(boneNum)->CalcWorldTRS(m_pos, m_rot, m_sca);
		}
		//�擾�����{�[���̃��[���h���W���擾
		m_pos = m_rightHandBone->GetPosition();
		//�擾�����{�[���̉�]���擾
		m_rot = m_rightHandBone->GetRotation();

		//�ʒu�X�V
		m_model->SetPosition(m_pos);
		//��]�X�V
		m_model->SetRotation(m_rot);
	}
}