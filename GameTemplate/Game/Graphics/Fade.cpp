/// <summary>
/// �t�F�[�h�N���X
/// </summary>
#include "stdafx.h"
#include "Fade.h"

namespace nsKabutoubatu
{
	namespace nsFade
	{
		//�t�F�[�h�摜�̏c���T�C�Y
		const float FADE_IMAGE_SIZE = 1500.0f;
		//�t�F�[�h����X�s�[�h
		const float FADE_SPEED = 0.03f;
		//�^����
		const float ALL_WHITE = 1.0f;
		//R
		const float INIT_R = 1.0f;
		//G
		const float INIT_G = 1.0f;
		//B
		const float INIT_B = 1.0f;

		const char* const FADE_COLOR[2] = {"Fade_Black", "Fade_White"};
	}

	void Fade::Init(const float alphaValue, const int nowState, const int fadeColorState)
	{
		//���l��ݒ�
		m_alphaValue = alphaValue;
		//�t�F�[�h�̏�Ԃ�ݒ�
		m_nowState = nowState;
		//�t�F�[�h�̐F�̃X�e�[�g��ݒ�
		m_fadeColorState = fadeColorState;

		//�t�F�[�h�摜��������
		m_fadeSprite = NewGO<SpriteRenderSub>(nsStdafx::PRIORITY_10, nullptr);
		m_fadeSprite->Init(nsFade::FADE_COLOR[m_fadeColorState], nsFade::FADE_IMAGE_SIZE, nsFade::FADE_IMAGE_SIZE);
		//�A���t�@�l��ݒ�
		m_fadeSprite->SetMulColor(0.0f, 0.0f, 0.0f, m_alphaValue);
	}

	Fade::~Fade()
	{
		//�t�F�[�h�摜���폜
		DeleteGO(m_fadeSprite);
	}

	void Fade::Update()
	{
		//��Ԃɉ����ĕω�
		switch (m_nowState)
		{
		//�t�F�[�h�C����
		case StateIn:
			m_alphaValue -= nsFade::FADE_SPEED;
			//���S�Ƀt�F�[�h�C��������A
			if (m_alphaValue <= 0.0f) {
				//�t�F�[�h���폜����
				DeleteGO(this);
			}
			break;
		//�t�F�[�h�A�E�g��
		case StateOut:
			m_alphaValue += nsFade::FADE_SPEED;
			//���S�ɐ^�����ɂȂ�����ҋ@������
			if (m_alphaValue >= nsFade::ALL_WHITE) {
				//�ҋ@���ɂ���
				m_nowState = StateWait;
			}
			break;
		}
		//�A���t�@�l���X�V
		m_fadeSprite->SetMulColor(nsFade::INIT_R, nsFade::INIT_G, nsFade::INIT_B, m_alphaValue);
	}

	void Fade::UiUpdate()
	{
		//��Ԃɉ����ĕω�
		switch (m_nowState)
		{
			//�t�F�[�h�C����
		case StateIn:
			m_alphaValue -= nsFade::FADE_SPEED;
			//���S�Ƀt�F�[�h�C��������A
			if (m_alphaValue <= 0.0f) {
				//�t�F�[�h���폜����
				DeleteGO(this);
			}
			break;
			//�t�F�[�h�A�E�g��
		case StateOut:
			m_alphaValue += nsFade::FADE_SPEED;
			//���S�ɐ^�����ɂȂ�����ҋ@������
			if (m_alphaValue >= nsFade::ALL_WHITE) {
				//�ҋ@���ɂ���
				m_nowState = StateWait;
			}
			break;
		}
		//�A���t�@�l���X�V
		m_fadeSprite->SetMulColor(nsFade::INIT_R, nsFade::INIT_G, nsFade::INIT_B, m_alphaValue);
	}
}