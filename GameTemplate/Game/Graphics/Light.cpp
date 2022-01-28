/// <summary>
/// ���C�g�N���X
/// </summary>

#include "stdafx.h"

namespace nsKabutoubatu
{
	namespace nsLight
	{
		//�f�B���N�V�������C�g
		const Vector3 DIRLIG_DIR = { 0.0f, -1.0f ,-0.5f };	//����
		const Vector3 DIRLIG_COLOR = { 0.4f, 0.4f ,0.4f };	//�F
		//�X�|�b�g���C�g
		const Vector3 SPOLIG_POS = { 0.0f, 50.0f ,0.0f };	//�ʒu
		const Vector3 SPOLIG_DIR = { 1.0f, -1.0f ,-1.0f };	//����
		const float	  SPOLIG_RAN = 100.0f;					//�e���͈�
		const float	  SPOLIG_ANG = 75.0f;					//�ˏo�p�x
		//�������C�g
		const Vector3 HEMISPHERELIG_GROUNDCOLOR = { 0.5f,0.3f,0.1f };	//�n�ʐF
		const Vector3 HEMISPHERELIG_SKYCOLOR = { 0.15f,0.3f,0.45f };	//�V���F
		const Vector3 HEMISPHERELIG_GROUNDNORMAL = { 0.0f,1.0f,0.0f };	//�n�ʂ̖@��
	}

	bool Light::Start()
	{
		//���_�̈ʒu��ݒ肷��
		m_ligData.eyePos = g_camera3D->GetPosition();

		//���C�g�J�����̈ʒu��ݒ�B����̓��C�g�̈ʒu
		m_ligCameraPos = g_camera3D->GetPosition();
		m_ligCamera.SetPosition(m_ligCameraPos);
		//���C�g�J�����̒����_��ݒ�B���ꂪ���C�g���Ƃ炵�Ă���ꏊ
		m_ligCameraTarget = g_camera3D->GetTarget();
		m_ligCamera.SetTarget(m_ligCameraTarget);
		//���C�g�J�����̏�������v�Z���ċ��߂�
		Vector3 Vec_x = Vector3::AxisX;
		Vector3 TarPos = m_ligCameraTarget - m_ligCameraPos;
		TarPos.Normalize();
		Vector3 CameraUp;
		CameraUp.Cross(TarPos, Vec_x);
		//���C�g�J�����̏�����̃x�N�g���𐳋K��
		CameraUp.Normalize();
		//�������ݒ�B
		m_ligCamera.SetUp(CameraUp);
		//���C�g�r���[�v���W�F�N�V�����s����v�Z���Ă���
		m_ligCamera.Update();

		return true;
	}

	void Light::Update()
	{
		//���C�g�J�����̈ʒu���X�V
		m_ligCameraPos = g_camera3D->GetPosition();
		//���C�g�J�����̈ʒu���J�����̈ʒu��艓������
		m_ligCameraPos.x -= 100.0f;
		m_ligCameraPos.y += 1000.0f;
		m_ligCameraPos.z += 1000.0f;
		//���C�g�J�����̎��_���X�V
		m_ligCamera.SetPosition(m_ligCameraPos);
		//�J�����̒����_���擾
		m_ligCameraTarget = g_camera3D->GetTarget();
		//���C�g�J�����̒����_���X�V
		m_ligCamera.SetTarget(m_ligCameraTarget);
		//���C�g�r���[�v���W�F�N�V�����s����v�Z���Ă���
		m_ligCamera.Update();
		//���C�g�r���[�v���W�F�N�V�����s����g���萔�o�b�t�@�ɐݒ肷��B
		m_ligData.mLVP = m_ligCamera.GetViewProjectionMatrix();
	}

	//�f�B���N�V�������C�g�̕���,�F��ݒ肷��֐�
	void Light::SetDirectionLightData()
	{
		//�f�B���N�V�������C�g�̕�����ݒ�
		m_ligData.dirLigData.dirDirection = nsLight::DIRLIG_DIR;
		//�f�B���N�V�������C�g�𐳋K�����đ傫��1�̃x�N�g���ɂ���
		m_ligData.dirLigData.dirDirection.Normalize();
		//�f�B���N�V�������C�g�̐F��ݒ�
		m_ligData.dirLigData.dirColor = nsLight::DIRLIG_COLOR;
	}

	//�|�C���g���C�g�̍��W,�F,�e���͈͂�ݒ肷��֐�
	void Light::SetPointLightData(const Vector3 position, const Vector3 color, const float range, const int lignum)
	{
		//�|�C���g���C�g�̍��W��ݒ�
		m_ligData.poiLigData[lignum].ptPosition = position;
		//�|�C���g���C�g�̐F��ݒ�
		m_ligData.poiLigData[lignum].ptColor = color;
		//�|�C���g���C�g�̉e���͈͂�ݒ�
		m_ligData.poiLigData[lignum].ptRange = range;
	}

	//�X�|�b�g���C�g�̍��W,�F,����,�ˏo�͈�,�ˏo�p�x��ݒ肷��֐�
	void Light::SetSpotLightData()
	{
		//�X�|�b�g���C�g�̍��W��ݒ�
		m_ligData.spotLigData.spPosition = nsLight::SPOLIG_POS;
		//�X�|�b�g���C�g�̐F��ݒ�
		m_ligData.spotLigData.spColor = nsStdafx::WHITE;
		//�X�|�b�g���C�g�̕�����ݒ�
		m_ligData.spotLigData.spDirection = nsLight::SPOLIG_DIR;
		//�����f�[�^������傫����1�ɂ��邽�߂ɐ��K��������
		m_ligData.spotLigData.spDirection.Normalize();
		//�X�|�b�g���C�g�̎ˏo�͈͂�ݒ�
		m_ligData.spotLigData.spRange = nsLight::SPOLIG_RAN;
		//�X�|�b�g���C�g�̎ˏo�p�x��ݒ�
		//(�f�O���[�P��(75�x)���烉�W�A���P�ʂɕϊ������l����)
		m_ligData.spotLigData.spAngle = Math::DegToRad(nsLight::SPOLIG_ANG);
	}

	//�������C�g�̒n�ʐF�i�Ƃ�Ԃ��̃��C�g�j,�V���F,�n�ʂ̖@����ݒ肷��֐�
	void Light::SetHemiSphereLightData()
	{
		//�n�ʐF��ݒ�
		m_ligData.hemiSphereLigData.hemiGroundColor = nsLight::HEMISPHERELIG_GROUNDCOLOR;
		//�V���F��ݒ�
		m_ligData.hemiSphereLigData.hemiSkyColor = nsLight::HEMISPHERELIG_SKYCOLOR;
		//�n�ʂ̖@����ݒ�
		m_ligData.hemiSphereLigData.hemiGroundNormal = nsLight::HEMISPHERELIG_GROUNDNORMAL;
	}
}