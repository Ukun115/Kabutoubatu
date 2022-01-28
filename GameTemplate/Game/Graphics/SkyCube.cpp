/// <summary>
/// �X�J�C�L���[�u�N���X
/// </summary>
#include "stdafx.h"
#include "SkyCube.h"
#include "Light.h"

namespace nsKabutoubatu
{
	bool SkyCube::Start()
	{
		m_light = FindGO<Light>(nsStdafx::LIGHT_NAME);

		//���ԑтɂ���ăf�B���N�V�������C�g�̃J���[��ύX
		switch (m_skyCubeType)
		{
		//���E��
		case enDayType:
			m_directionLightColor = { 0.4f,0.4f,0.4f };
			break;
		//���Ă��E�[���
		case enSunlizeType:
			m_directionLightColor = { 0.7f,0.2f,0.2f };
			break;
		//��
		case enNightType:
			//�����̂�
			m_directionLightColor = { 0.0f,0.0f,0.0f };
			break;
		}
		//�ݒ肵���J���[���Z�b�g
		m_light->ChangeDirectionLightDataColor(m_directionLightColor);

		//�e�N�X�`���̃t�@�C���p�X
		m_mapTextureFilePaths[enDayType] = L"Assets/modelData/skyCubeMapDay.dds";
		m_mapTextureFilePaths[enSunlizeType] = L"Assets/modelData/skyCubeMapSunrise.dds";
		m_mapTextureFilePaths[enNightType] = L"Assets/modelData/skyCubeMapNight.dds";

		//�X�J�C�L���[�u�̑傫���𒴑傫������
		m_sca *= 10000.0f;

		//tkm�t�@�C���̃t�@�C���p�X���w��
		m_skyCubeInitData.m_tkmFilePath = "Assets/modelData/sky.tkm";
		//�V�F�[�_�[�t�@�C���̃t�@�C���p�X���w��
		m_skyCubeInitData.m_fxFilePath = "Assets/shader/SkyCubeMap.fx";

		m_skyCubeInitData.m_vsEntryPointFunc = "VSMain";

		//�X�J�C�L���[�u�̃e�N�X�`��
		for (int textureNum = 0; textureNum < enSkyCubeType_Num; textureNum++) {
			m_texture[textureNum].InitFromDDSFile(m_mapTextureFilePaths[textureNum]);
		}
		//�e�N�X�`����fx�ɓn��
		m_skyCubeInitData.m_expandShaderResourceView[0] = &m_texture[m_skyCubeType];
		//�P�x�̐��l��fx�ɓn��
		m_skyCubeInitData.m_expandConstantBuffer[0] = &m_luminance;
		m_skyCubeInitData.m_expandConstantBufferSize[0] = sizeof(m_luminance);

		//�����������g���ăX�J�C�L���[�u�\������������������
		m_skyCubeModel.Init(m_skyCubeInitData);

		return true;
	}

	void SkyCube::Update()
	{
		//��]�p�x���X�V
		m_rotAngle += 0.01f;
		//��]�p�x���N�H�[�^�j�I���ɃZ�b�g
		m_rot.SetRotationDeg(Vector3::AxisY, m_rotAngle);
		//�X�J�C�L���[�u�̈ʒu�A��]�A�g����X�V
		m_skyCubeModel.UpdateWorldMatrix(m_pos, m_rot, m_sca);
	}

	//�X�J�C�L���[�u�̕`�惁�\�b�h
	void SkyCube::ModelRender(RenderContext& rc)
	{
		//�X�J�C�L���[�u�̃h���[�R�[�������s���A�\������
		m_skyCubeModel.Draw(rc);
	}
}