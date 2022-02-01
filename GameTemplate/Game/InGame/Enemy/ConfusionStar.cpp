/// <summary>
/// 混乱時の星モデルクラス
/// </summary>
#pragma once
#include "stdafx.h"
#include "ConfusionStar.h"
#include "../Camera/PlayerCamera.h"

namespace nsKabutoubatu
{
	bool ConfusionStar::Start()
	{
		//敵の頭の上
		m_pos.y += 200.0f;

		m_model = NewGO<SkinModelRender>();
		m_model->SetShadowCaster(false);
		m_model->SetSilhouette(true);	//シルエットを落とす
		m_model->SetOutline(false);//輪郭線をつける
		m_model->Init("ConfusionStar");
		m_model->SetPosition(m_pos);

		m_playerCamera = FindGO<PlayerCamera>(nsStdafx::PLAYER_CAMERA_NAME);

		//ピヨピヨ音を再生
		if ((m_playerCamera->GetCameraTarget() - m_pos).Length() < nsStdafx::SOUND_CAN_HEARD_RANGE)
		{
			//混乱効果音再生
			m_sound = NewGO<SoundSource>();
			m_sound->Init(L"Assets/sound/FirstBoss_Confusion.wav");
			m_sound->SetVolume(0.5f);
			m_sound->Play(false);

		}

		return true;
	}

	ConfusionStar::~ConfusionStar()
	{
		//混乱モデル削除
		DeleteGO(m_model);
		//混乱効果音を削除
		DeleteGO(m_sound);
	}

	void ConfusionStar::Update()
	{
		//回転
		Rotation();
	}

	//回転するメソッド
	void ConfusionStar::Rotation()
	{
		m_rotationAngle -= 0.05f;
		//回転を更新
		m_rot.SetRotation(Vector3::AxisY, m_rotationAngle);
		m_model->SetRotation(m_rot);
	}
}