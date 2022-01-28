/// <summary>
/// �~�j�}�b�v�N���X
/// </summary>
#include "stdafx.h"
#include "MiniMap.h"

namespace nsKabutoubatu
{
	namespace nsMiniMap
	{
		//�~�j�}�b�v�̉𑜓x
		const int MINIMAP_WEIDTH = 512;
		const int MINIMAP_HEIGHT = 512;
		//�~�b�v���x��
		const int MIP_LEBEL = 1;
		//�e�N�X�`���z��̃T�C�Y
		const int ARRAY_SIZE = 1;
		//�z��̍ŏ��̔ԍ�
		const int FIRST_ARRAY_NUMBER = 0;

		const char* const SPRITE_FX= "Assets/shader/sprite.fx";
	}

	bool MiniMap::Start()
	{
		/////////////�~�j�}�b�v�J�����ݒ�//////////////
		{
			//�t�@�[��ݒ�
			m_miniMapCamera.SetFar(100000);
			//���_���X�e�[�W�̐^��ɂ���
			m_cameraPos = { -1700.0f,8000.0f,-3870.0f };
			m_miniMapCamera.SetPosition(m_cameraPos);
			m_cameraTargetPos = { -1700.0f,0.0f,-3870.0f };
			//�����_���X�e�[�W�̒����ɂ���
			m_miniMapCamera.SetTarget(m_cameraTargetPos);
			//�������ݒ�B
			m_miniMapCamera.SetUp({ 0.0f,0.0f,-1.0f });
			//�r���[�v���W�F�N�V�����s����v�Z���Ă���
			m_miniMapCamera.Update();
		}
		///////////�~�j�}�b�v�J�����ݒ�END///////////////

		///////////////////////////�~�j�}�b�v�摜�ݒ�///////////////////////////////
		{
			//�~�j�}�b�v��\�����邽�߂̃X�v���C�g������������
			m_miniMapSpriteInitData.m_textures[nsMiniMap::FIRST_ARRAY_NUMBER] = &m_miniMap.GetRenderTargetTexture();
			m_miniMapSpriteInitData.m_fxFilePath = nsMiniMap::SPRITE_FX;
			m_miniMapSpriteInitData.m_width = 460;
			m_miniMapSpriteInitData.m_height = 270;
			m_miniMapSprite.Init(m_miniMapSpriteInitData);
			m_miniMapSprite.SetRotation({ 0.0f,0.0f,0.0f,1.0f });
		}
		/////////////////////////�~�j�}�b�v�摜�ݒ�END//////////////////////////////

		//���߂̓}�b�v��\��
		//�\����\���ؑւ̓v���C���[�N���X�ŏ����Ă�
		Deactivate();

		return true;
	}

	//�~�j�}�b�v�̏��������\�b�h
	void MiniMap::Init()
	{
		//�~�j�}�b�v�`��p�̃����_�����O�^�[�Q�b�g���쐬����B
		m_miniMap.Create(
			nsMiniMap::MINIMAP_WEIDTH,
			nsMiniMap::MINIMAP_HEIGHT,
			nsMiniMap::MIP_LEBEL,
			nsMiniMap::ARRAY_SIZE,
			//�~�j�}�b�v�̃J���[�o�b�t�@�̃t�H�[�}�b�g��ύX���Ă���B
			DXGI_FORMAT_R32G32B32A32_FLOAT,
			DXGI_FORMAT_D32_FLOAT,
			clearColor
		);
	}

	//�~�j�}�b�v�`�惁�\�b�h
	void MiniMap::DrawMiniMap(RenderContext& rc)
	{
		// �����_�����O�^�[�Q�b�g���~�j�}�b�v�ɕύX����
		rc.WaitUntilToPossibleSetRenderTarget(m_miniMap);
		rc.SetRenderTargetAndViewport(m_miniMap);
		//�����_�����O�^�[�Q�b�g���N���A�ɂ���
		rc.ClearRenderTargetView(m_miniMap.GetRTVCpuDescriptorHandle(), m_miniMap.GetRTVClearColor());
		rc.ClearDepthStencilView(m_miniMap.GetDSVCpuDescriptorHandle(), m_miniMap.GetDSVClearValue());
		//�}�b�v����������
		GameObjectManager::GetInstance()->ExecuteMiniMapRender(rc);
		// �������݊����҂�
		rc.WaitUntilFinishDrawingToRenderTarget(m_miniMap);
	}

	//�~�j�}�b�v�̉摜�`��֐�
	void MiniMap::SpriteDataRender(RenderContext& renderContext)
	{
		//�h���[�֐�
		m_miniMapSprite.Draw(renderContext);
	}
}