/// <summary>
/// �������̐����f���N���X
/// </summary>
#pragma once
#include "stdafx.h"
#include "ConfusionStar.h"
#include "../Camera/PlayerCamera.h"

namespace nsKabutoubatu
{
	bool ConfusionStar::Start()
	{
		//�G�̓��̏�
		m_pos.y += 200.0f;

		m_model = NewGO<SkinModelRender>();
		m_model->SetShadowCaster(false);
		m_model->SetSilhouette(true);	//�V���G�b�g�𗎂Ƃ�
		m_model->SetOutline(false);//�֊s��������
		m_model->Init("ConfusionStar");
		m_model->SetPosition(m_pos);

		m_playerCamera = FindGO<PlayerCamera>(nsStdafx::PLAYER_CAMERA_NAME);

		//�s���s�������Đ�
		if ((m_playerCamera->GetCameraTarget() - m_pos).Length() < nsStdafx::SOUND_CAN_HEARD_RANGE)
		{
			//�������ʉ��Đ�
			m_sound = NewGO<SoundSource>();
			m_sound->Init(L"Assets/sound/FirstBoss_Confusion.wav");
			m_sound->SetVolume(0.5f);
			m_sound->Play(false);

		}

		return true;
	}

	ConfusionStar::~ConfusionStar()
	{
		//�������f���폜
		DeleteGO(m_model);
		//�������ʉ����폜
		DeleteGO(m_sound);
	}

	void ConfusionStar::Update()
	{
		//��]
		Rotation();
	}

	//��]���郁�\�b�h
	void ConfusionStar::Rotation()
	{
		m_rotationAngle -= 0.05f;
		//��]���X�V
		m_rot.SetRotation(Vector3::AxisY, m_rotationAngle);
		m_model->SetRotation(m_rot);
	}
}