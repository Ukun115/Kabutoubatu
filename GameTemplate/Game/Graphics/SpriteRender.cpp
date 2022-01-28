/// <summary>
/// �摜�`��N���X
/// </summary>
#include "stdafx.h"

namespace nsKabutoubatu
{
	void SpriteRender::Update()
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

	void SpriteRender::UiUpdate()
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
	void SpriteRender::Init(const char* filePath, const float width, const float height, AlphaBlendMode mode)
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
	void SpriteRender::SpriteDataRender(RenderContext& renderContext)
	{
		//�h���[�֐�
		m_sprite.Draw(renderContext);
	}
}