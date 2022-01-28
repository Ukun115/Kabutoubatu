/// <summary>
/// �v���C���[�J�����N���X
/// </summary>
#include "stdafx.h"
#include "PlayerCamera.h"
#include "../Player/Player.h"
#include "CameraMoveDirectionChange.h"

namespace nsKabutoubatu
{
	bool PlayerCamera::Start()
	{
		//�v���C���[�̃C���X�^���X������
		m_player = FindGO<Player>(nsStdafx::PLAYER_NAME[m_targetPlayer]);
		//�J�����̈ړ�������ς���N���X�𐶐�
		m_cameraMoveDirectionChange = NewGO<CameraMoveDirectionChange>();
		m_cameraMoveDirectionChange->SetTargetPlayer(m_targetPlayer);
		//�����ʂ܂ł̋�����ݒ�
		g_camera3D->SetFar(1000000);
		g_camera3D->SetViewAngle(Math::DegToRad(38.0f));
		return true;
	}

	PlayerCamera::~PlayerCamera()
	{
		//�J�����̈ړ�������ς���N���X
		DeleteGO(m_cameraMoveDirectionChange);
	}

	void PlayerCamera::Update()
	{
		//�J�����̎��_�������_���v���C���[�̈ʒu�Ɉړ�
		m_cameraPos = m_player->GetPosition();
		m_cameraTarget = m_player->GetPosition();

		//�J�����̈ړ������̏�Ԃɂ���ČŒ肷��
		switch (m_cameraState)
		{
		//���X�N���[��
		case enX:
			//���_
			m_cameraPos.y = m_cameraTargetYPosition + 250.0f;
			m_cameraPos.z = m_cameraTargetZPosition + 1000.0f;
			//�����_
			m_cameraTarget.y = m_cameraTargetYPosition + 100.0f;
			m_cameraTarget.z = m_cameraTargetZPosition;
			break;
		//�c�X�N���[��
		case enY:
			//���_
			m_cameraPos.x = m_cameraTargetXPosition;
			m_cameraPos.y += 200.0f;
			m_cameraPos.z = m_cameraTargetZPosition + 1000.0f;
			//�����_
			m_cameraTarget.x = m_cameraTargetXPosition;
			m_cameraTarget.z = m_cameraTargetZPosition;
			break;
		//���X�N���[��
		case enZ:
			//���_
			m_cameraPos.x = m_cameraTargetXPosition;
			m_cameraPos.y = m_cameraTargetYPosition + 200.0f;
			m_cameraPos.z += 1000.0f;
			//�����_
			m_cameraTarget.x = m_cameraTargetXPosition;
			m_cameraTarget.y = m_cameraTargetYPosition;
			m_cameraTarget.z -= 500.0f;
			break;
		//�V���b�v�J����
		case enShop:
			//���_
			m_cameraPos = { 0.0f,10150.0f,600.0f };
			//�����_
			m_cameraTarget = { 0.0f, 10100.0f, 0.0f };
			break;
		//�{�X�X�e�[�W�J����
		case enBossStage:
			//���_
			m_cameraPos = { 0.0f,-9500.0f,1200.0f };
			//�����_
			m_cameraTarget = { 0.0f, -10000.0f, 0.0f };
			break;
		}

		//�J�����̈ʒu���X�V
		g_camera3D->SetPosition(m_cameraPos);
		//�J�����̒����_���v���C���[�̈ʒu�ɍX�V
		g_camera3D->SetTarget(m_cameraTarget);
		//�J�����r���[�v���W�F�N�V�����s����g���萔�o�b�t�@�ɐݒ肷��B
		m_cameraData.mCVP = g_camera3D->GetViewProjectionMatrix();
	}
}