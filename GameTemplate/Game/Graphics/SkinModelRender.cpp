/// <summary>
/// ���f���`��N���X
/// </summary>

#include "stdafx.h"
#include "Light.h"
#include "DepthShadow.h"
#include "RenderingEngine.h"
#include "Silhouette.h"
#include "MiniMap.h"
#include "../InGame/Camera/PlayerCamera.h"
#include "SkyCube.h"
#include "../ResourceBankManager.h"

namespace nsKabutoubatu
{
	void SkinModelRender::Update()
	{
		//���f���̈ʒu�A��]�A�g����X�V
		m_model.UpdateWorldMatrix(m_pos, m_rot, m_sca);
		//�X�P���g�����X�V
		if (m_tksFilePath != nullptr)
		{
			m_skeleton.Update(m_model.GetWorldMatrix());
		}
		//�A�j���[�V�������X�V
		if (m_animationClips != nullptr)
		{
			m_animation.Progress(g_gameTime->GetFrameDeltaTime() * m_animationSpeed);
		}
		//�V���h�E�̈ʒu�A��]�A�g����X�V
		if (m_shadowCaster)
		{
			m_shadowModel.UpdateWorldMatrix(m_pos, m_rot, m_sca);
		}
		//�֊s���̈ʒu�A��]�A�g����X�V
		if (m_modelMode.outline)
		{
			m_zPrepassModel.UpdateWorldMatrix(m_pos, m_rot, m_sca);
		}
		//�V���G�b�g�̈ʒu�A��]�A�g����X�V
		if (m_modelMode.silhouetteFlg)
		{
			m_silhouetteModel.UpdateWorldMatrix(m_pos, m_rot, m_sca);
		}
		//�~�j�}�b�v�̈ʒu�A��]�A�g����X�V
		m_miniMapModel.UpdateWorldMatrix(m_pos, m_rot, m_sca);
	}

	void SkinModelRender::UiUpdate()
	{
		//���f���̈ʒu�A��]�A�g����X�V
		m_model.UpdateWorldMatrix(m_pos, m_rot, m_sca);
		//�X�P���g�����X�V
		if (m_tksFilePath != nullptr)
		{
			m_skeleton.Update(m_model.GetWorldMatrix());
		}
		//�A�j���[�V�������X�V
		if (m_animationClips != nullptr)
		{
			m_animation.Progress(g_gameTime->GetFrameDeltaTime() * m_animationSpeed);
		}
		//�V���h�E�̈ʒu�A��]�A�g����X�V
		if (m_shadowCaster)
		{
			m_shadowModel.UpdateWorldMatrix(m_pos, m_rot, m_sca);
		}
		//�֊s���̈ʒu�A��]�A�g����X�V
		if (m_modelMode.outline)
		{
			m_zPrepassModel.UpdateWorldMatrix(m_pos, m_rot, m_sca);
		}
		//�V���G�b�g�̈ʒu�A��]�A�g����X�V
		if (m_modelMode.silhouetteFlg)
		{
			m_silhouetteModel.UpdateWorldMatrix(m_pos, m_rot, m_sca);
		}
		//�~�j�}�b�v�̈ʒu�A��]�A�g����X�V
		m_miniMapModel.UpdateWorldMatrix(m_pos, m_rot, m_sca);
	}

	//���f�������������郁�\�b�h
	void SkinModelRender::Init(const char* tkmFilePath,const char* tksFilePath,AnimationClip* animationClips, int animationClipsNum)
	{
		//�C���X�^���X��T��
		m_light = FindGO<Light>(nsStdafx::LIGHT_NAME);
		m_depthShadow = FindGO<DepthShadow>(nsStdafx::DEPTHSHADOW_NAME);
		m_zPrepass = FindGO<RenderingEngine>(nsStdafx::RENDERINGENGINE_NAME);
		m_silhouette = FindGO<Silhouette>(nsStdafx::SILHOUETTE_NAME);
		m_miniMap = FindGO<MiniMap>(nsStdafx::MINIMAP_NAME);
		m_playerCamera = FindGO<PlayerCamera>(nsStdafx::PLAYER_CAMERA_NAME);
		m_skyCube = FindGO<SkyCube>(nsStdafx::SKYCUBE_NAME);

		//�t�@�C�������t�@�C���p�X�ɂ���
		sprintf(m_filePathM, "Assets/modelData/%s.tkm", tkmFilePath);
		//tkm�t�@�C���̃t�@�C���p�X��ݒ肷��
		m_modelInitData.m_tkmFilePath = m_filePathM;
		//�g�p����V�F�[�_�[�t�@�C���p�X��ݒ肷��
		m_modelInitData.m_fxFilePath = "Assets/shader/model.fx";
		//�V�F�[�_�[�̒��_�V�F�[�_�[�̃G���g���[�֐����̎w��
		m_modelInitData.m_vsEntryPointFunc = "VSMain";
		//�V�F�[�_�[�̃s�N�Z���V�F�[�_�[�̃G���g���[�֐����̎w��
		m_modelInitData.m_vsSkinEntryPointFunc = "VSSkinMain";
		/*���f���̏���������萔�o�b�t�@�Ƃ��ăf�B�X�N���v�^�q�[�v��
		  �o�^���邽�߂Ƀ��f���̏��������Ƃ��ēn��					*/
		m_modelInitData.m_expandConstantBuffer[0] = m_light->GetLightAdoress();
		m_modelInitData.m_expandConstantBufferSize[0] = sizeof(m_light->GetLight());
		//�V�F�[�_�[�Ƀ��f���̃��[�h��n��
		m_modelInitData.m_expandConstantBuffer[1] = &m_modelMode;
		m_modelInitData.m_expandConstantBufferSize[1] = sizeof(m_modelMode);
		/*���f���̏���������萔�o�b�t�@�Ƃ��ăf�B�X�N���v�^�q�[�v��
		  �o�^���邽�߂Ƀ��f���̏��������Ƃ��ēn��					*/
		m_modelInitData.m_expandConstantBuffer[2] = m_playerCamera->GetCameraAdoress();
		m_modelInitData.m_expandConstantBufferSize[2] = sizeof(m_playerCamera->GetCamera());
		//�V���h�E�}�b�v���g��SRV�ɐݒ肷��B
		m_modelInitData.m_expandShaderResourceView[0] = &m_depthShadow->GetShadowMapTexture();
		//���f���݂̂̔�ʊE�[�x�}�b�v���g��SRV�ɐݒ肷��B
		m_modelInitData.m_expandShaderResourceView[1] = &m_zPrepass->GetZPrepassDepthTexture();
		//�V���G�b�g�}�b�v���g��SRV�ɐݒ肷��B
		m_modelInitData.m_expandShaderResourceView[2] = &m_silhouette->GetSilhouetteMapTexture();
		////�X�J�C�L���[�u�}�b�v���g��SRV�ɐݒ肷��B
		//m_modelInitData.m_expandShaderResourceView[3] = &m_skyCube->GetSkyCubeMapTexture();



		///////////////////////////////�~�j�}�b�v////////////////////////////////////
		//tkm�t�@�C���̃t�@�C���p�X��ݒ肷��
		m_miniMapModelInitData.m_tkmFilePath = m_filePathM;
		//�g�p����V�F�[�_�[�t�@�C���p�X��ݒ肷��
		m_miniMapModelInitData.m_fxFilePath = "Assets/shader/miniMap.fx";
		//�V�F�[�_�[�̒��_�V�F�[�_�[�̃G���g���[�֐����̎w��
		m_miniMapModelInitData.m_vsEntryPointFunc = "VSMain";
		//�V�F�[�_�[�̃s�N�Z���V�F�[�_�[�̃G���g���[�֐����̎w��
		m_miniMapModelInitData.m_vsSkinEntryPointFunc = "VSSkinMain";
		/*���f���̏���������萔�o�b�t�@�Ƃ��ăf�B�X�N���v�^�q�[�v��
		  �o�^���邽�߂Ƀ��f���̏��������Ƃ��ēn��					*/
		m_miniMapModelInitData.m_expandConstantBuffer[0] = m_light->GetLightAdoress();
		m_miniMapModelInitData.m_expandConstantBufferSize[0] = sizeof(m_light->GetLight());
		//�V���h�E���f����������
		m_miniMapModel.Init(m_miniMapModelInitData);
		///////////////////////////////�~�j�}�b�vEND//////////////////////////////////



		/////////////////////////////////////�X�P���g��////////////////////////////////////////////
		if (tksFilePath != nullptr)
		{
			//�t�@�C������ۑ�
			m_tksFilePath = tksFilePath;
			//�t�@�C�������t�@�C���p�X�ɂ���
			sprintf(m_filePathS, "Assets/modelData/%s.tks", tksFilePath);
			m_skeleton.Init(m_filePathS);
			//���f���ɃX�P���g���̃f�[�^��n��
			m_modelInitData.m_skeleton = &m_skeleton;
			//�V���h�E���f���ɃX�P���g���̃f�[�^��n��
			if (m_shadowCaster)
			{
				m_shadowModelInitData.m_skeleton = &m_skeleton;
			}
			//�֊s�����f���ɃX�P���g���̃f�[�^��n��
			if (m_modelMode.outline)
			{
				m_zPrepassModelInitData.m_skeleton = &m_skeleton;
			}
			//�V���G�b�g���f���ɃX�P���g���̃f�[�^��n��
			if (m_modelMode.silhouetteFlg)
			{
				m_silhouetteModelInitData.m_skeleton = &m_skeleton;
			}
		}
		/////////////////////////////////////�X�P���g��END////////////////////////////////////////////



		///////////////////////////////�A�j���[�V����////////////////////////////////////
		//�A�j���[�V������������
		if (animationClips != nullptr)
		{
			//�A�j���[�V�����N���b�v��ۑ�
			m_animationClips = animationClips;
			//�A�j���[�V�����N���b�v����ۑ�
			m_animationClipsNum = animationClipsNum;
			//�A�j���[�V������������
			m_animation.Init(m_skeleton, m_animationClips, m_animationClipsNum);
			//���f���̏�������w�肷��B
			m_modelInitData.m_modelUpAxis = m_skinModelUpAxis;
		}
		///////////////////////////////�A�j���[�V����END/////////////////////////////////



		//////////////////////////////////�V���h�E/////////////////////////////////////
		if (m_shadowCaster)
		{
			m_shadowModelInitData.m_tkmFilePath = m_filePathM;
			//�t�@�C�������t�@�C���p�X�ɂ���
			m_shadowModelInitData.m_fxFilePath = "Assets/shader/drawShadowMap.fx";
			//�V�F�[�_�[�̒��_�V�F�[�_�[�̃G���g���[�֐����̎w��
			m_shadowModelInitData.m_vsEntryPointFunc = "VSMain";
			//�V�F�[�_�[�̃s�N�Z���V�F�[�_�[�̃G���g���[�֐����̎w��
			m_shadowModelInitData.m_vsSkinEntryPointFunc = "VSSkinMain";
			//�V�F�[�_�[�Ƀ��f���̃��[�h��n��
			m_shadowModelInitData.m_expandConstantBuffer[0] = &m_modelMode;
			m_shadowModelInitData.m_expandConstantBufferSize[0] = sizeof(m_modelMode);
			//�V���h�E���f����������
			m_shadowModel.Init(m_shadowModelInitData);
		}
		////////////////////////////////////�V���h�EEND//////////////////////////////////



		//////////////////////////ZPrepass///////////////////////////////////////
		if (m_modelMode.outline)
		{
			m_zPrepassModelInitData.m_tkmFilePath = m_filePathM;
			//�t�@�C�������t�@�C���p�X�ɂ���
			m_zPrepassModelInitData.m_fxFilePath = "Assets/shader/ZPrepass.fx";
			//�V�F�[�_�[�̒��_�V�F�[�_�[�̃G���g���[�֐����̎w��
			m_zPrepassModelInitData.m_vsEntryPointFunc = "VSMain";
			//�V�F�[�_�[�̃s�N�Z���V�F�[�_�[�̃G���g���[�֐����̎w��
			m_zPrepassModelInitData.m_vsSkinEntryPointFunc = "VSSkinMain";
			m_zPrepassModelInitData.m_colorBufferFormat = DXGI_FORMAT_R32G32_FLOAT;
			//ZPrepass���f����������
			m_zPrepassModel.Init(m_zPrepassModelInitData);
		}
		//////////////////////////ZPrepassEND///////////////////////////////////////



		////////////////////////////////�V���G�b�g////////////////////////////////
		if (m_modelMode.silhouetteFlg)
		{
			//�V���h�E�̋t������Ă��Ƃł���
			//�V���h�E�̓��C�g�J�������������A�V���G�b�g�̓J�������g��

			m_silhouetteModelInitData.m_tkmFilePath = m_filePathM;
			//�t�@�C�������t�@�C���p�X�ɂ���
			m_silhouetteModelInitData.m_fxFilePath = "Assets/shader/drawSilhouetteMap.fx";
			//�V�F�[�_�[�̒��_�V�F�[�_�[�̃G���g���[�֐����̎w��
			m_silhouetteModelInitData.m_vsEntryPointFunc = "VSMain";
			//�V�F�[�_�[�̃s�N�Z���V�F�[�_�[�̃G���g���[�֐����̎w��
			m_silhouetteModelInitData.m_vsSkinEntryPointFunc = "VSSkinMain";
			//�V�F�[�_�[�Ƀ��f���̃��[�h��n��
			m_silhouetteModelInitData.m_expandConstantBuffer[0] = &m_modelMode;
			m_silhouetteModelInitData.m_expandConstantBufferSize[0] = sizeof(m_modelMode);
			//�V���G�b�g���f����������
			m_silhouetteModel.Init(m_silhouetteModelInitData);
		}
		///////////////////////////////�V���G�b�gEND//////////////////////////////////



		//�����������g���ă��f���\������������������
		m_model.Init(m_modelInitData);
	}


	//�`��֐�

	//���f���̕`�惁�\�b�h
	void SkinModelRender::ModelRender(RenderContext& rc)
	{
		if (m_fireBallFlg)
		{
			return;
		}

		//���f���̃h���[�R�[�������s���A�\������
		m_model.Draw(rc);
	}
	//�V���h�E�̕`�惁�\�b�h
	void SkinModelRender::ShadowRender(RenderContext& rc)
	{
		if (m_fireBallFlg)
		{
			return;
		}

		if (!m_shadowCaster)
		{
			return;
		}

		//�V���h�E
		m_shadowModel.Draw(rc, *m_light->GetLightCamera());
	}
	//�֊s���̕`�惁�\�b�h
	void SkinModelRender::ZPrepassRender(RenderContext& rc)
	{
		if (m_fireBallFlg)
		{
			return;
		}

		if (!m_modelMode.outline)
		{
			return;
		}

		//�֊s�����f��
		m_zPrepassModel.Draw(rc);
	}
	//�V���G�b�g���f���̕`�惁�\�b�h
	void SkinModelRender::SilhouetteRender(RenderContext& rc)
	{
		if (m_fireBallFlg)
		{
			return;
		}

		if (!m_modelMode.silhouetteFlg)
		{
			return;
		}

		//�V���G�b�g���f��
		//�J�������_����̃V���G�b�g�}�b�v
		m_silhouetteModel.Draw(rc);
	}

	//�~�j�}�b�v�̕`�惁�\�b�h
	void SkinModelRender::MiniMapRender(RenderContext& rc)
	{
		//�^�ォ��̃~�j�}�b�v
		m_miniMapModel.Draw(rc ,*m_miniMap->GetMiniMapCamera());
	}

	void SkinModelRender::PreLoadModel(const char* tkmFilePath)
	{
		//�v�����[�h�͂܂��ł��Ȃ�
		TkmFile* tkmFile = ResourceBankManager::GetInstance()->GetTkmFileFromBank(tkmFilePath);

		//���o�^
		if (tkmFile == nullptr)
		{
			//�o�^
			tkmFile = new TkmFile;
			tkmFile->Load(tkmFilePath);
			ResourceBankManager::GetInstance()->RegistTkmFileToBank(tkmFilePath, tkmFile);
		}
	}
}