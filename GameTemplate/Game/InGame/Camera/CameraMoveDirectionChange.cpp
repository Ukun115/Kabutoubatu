 /// <summary>
 /// �v���C���[�J�����̈ړ������ύX�N���X
 /// </summary>
#include "stdafx.h"
#include "CameraMoveDirectionChange.h"
#include "../Player/Player.h"
#include "PlayerCamera.h"

namespace nsKabutoubatu
{
	bool CameraMoveDirectionChange::Start()
	{
		//�v���C���[�̃C���X�^���X������
		m_player = FindGO<Player>(nsStdafx::PLAYER_NAME[m_targetPlayer]);
		//�v���C���[�J�����̃C���X�^���X������
		m_playerCamera = FindGO<PlayerCamera>(nsStdafx::PLAYER_CAMERA_NAME);

		return true;
	}

	CameraMoveDirectionChange::~CameraMoveDirectionChange()
	{

	}

	void CameraMoveDirectionChange::Update()
	{
		//�X�e�[�W�P�̎��̃J�����؂�ւ��|�C���g

		//�v���C���[�̈ʒu���擾
		m_playerPosition = m_player->GetPosition();
		//�E�����Ɖ��������}�C�i�X�������̂Ń}�C�i�X�P�������Đ��̐���������悤�ɂ���
		m_playerPosition.x *= -1;
		m_playerPosition.z *= -1;

		//�V���b�v�ɂ���Ƃ��A(���̍����ɃV���b�v������)
		if (m_playerPosition.y >= 900.0f)
		{
			//�V���b�v�̌Œ�J�����ɂ���
			m_playerCamera->SetCameraState(enShop);
		}
		//�{�X�X�e�[�W�ɂ���Ƃ��A
		else if (m_playerPosition.y <= -900.0f)
		{
			//�{�X�X�e�[�W�̌Œ�J�����ɂ���
			m_playerCamera->SetCameraState(enBossStage);
		}
		//�X�e�[�W�ɂ���Ƃ��A
		else
		{
			switch (m_nowWayPoint)
			{
				//�E�F�C�P
			case enWay1:
				if (m_playerPosition.x >= 1400.0f)
				{
					//�E�F�C2�J�����ɂ���
					Way2Camera();
				}

				break;

				//�E�F�C2
			case enWay2:
				if (m_playerPosition.x < 1400.0f)
				{
					//�E�F�C�P�J�����ɂ���
					Way1Camera();
				}
				if (m_playerPosition.z >= 6046.0f)
				{
					//�E�F�C3�J�����ɂ���
					Way3Camera();
				}

				break;

				//�E�F�C3
			case enWay3:
				if (m_playerPosition.z < 6046.0f)
				{
					//�E�F�C2�J�����ɂ���
					Way2Camera();
				}

				break;
			}
		}
	}

	//�E�F�C�P�̃J�����ݒ�
	void CameraMoveDirectionChange::Way1Camera()
	{
		//X�����ɐ؂�ւ���
		m_playerCamera->SetCameraState(enX);
		//Y��Z�����߂�
		m_playerCamera->SetXMoveCameraPos(0.0f, 0.0f);

		//���݂ǂ��ɂ���̂���ύX����
		m_nowWayPoint = enWay1;
	}

	//�E�F�C2�̃J�����ݒ�
	void CameraMoveDirectionChange::Way2Camera()
	{
		//Z�����ɐ؂�ւ���
		m_playerCamera->SetCameraState(enZ);
		//X��Y�����߂�
		m_playerCamera->SetZMoveCameraPos(-1750.0f, 0.0f);

		//���݂ǂ��ɂ���̂���ύX����
		m_nowWayPoint = enWay2;
	}

	//�E�F�C3�̃J�����ݒ�
	void CameraMoveDirectionChange::Way3Camera()
	{
		//X�����ɐ؂�ւ���
		m_playerCamera->SetCameraState(enX);
		//Y��Z�����߂�
		m_playerCamera->SetXMoveCameraPos(0.0f, -6400.0f);

		//���݂ǂ��ɂ���̂���ύX����
		m_nowWayPoint = enWay3;
	}
}