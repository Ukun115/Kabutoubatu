/// <summary>
/// �摜�`��T�u�N���X
/// </summary>
#include "stdafx.h"
#include "SpriteRenderSub.h"

namespace nsKabutoubatu
{
	void SpriteRenderSub::Update()
	{
		//�ʒu�A��]�A�g�嗦���X�V
		if (m_pivotChangeFlg)
		{
			m_sprite.Update(m_pos, m_rot, m_sca, m_pivot);
		}
		else
		{
			m_sprite.Update(m_pos, m_rot, m_sca);
		}
	}

	//�摜������������֐�
	void SpriteRenderSub::Init(const char* filePath, float width, float height, AlphaBlendMode mode)
	{
		//�t�@�C�������t�@�C���p�X�ɂ���
		sprintf(m_filePath, "Assets/image/dds/%s.dds", filePath);
		m_spriteInitData.m_ddsFilePath[0] = m_filePath;					//�t�@�C���p�X
		m_spriteInitData.m_fxFilePath = "Assets/shader/sprite.fx";		//�V�F�[�_�[�t�@�C���̃p�X���w��
		m_spriteInitData.m_width = width;								//����
		m_spriteInitData.m_height = height;								//�c��
		m_spriteInitData.m_alphaBlendMode = mode;						//����
		//�摜��������
		m_sprite.Init(m_spriteInitData);
	}

	//�`��֐�
	void SpriteRenderSub::SpriteDataRenderSub(RenderContext& renderContext)
	{
		//�h���[�֐�
		m_sprite.Draw(renderContext);
	}
}