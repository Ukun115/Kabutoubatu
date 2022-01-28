/// <summary>
/// �h���V�[���̑I�𕶎��N���X
/// </summary>
#include "stdafx.h"
#include "HotelSelectFont.h"

namespace nsKabutoubatu
{
	namespace nsHotelSelectFont
	{
		const Vector4   FONT_COLOR = { 1.0f,1.0f,1.0f,1.0f };	//�������ԃt�H���g�̐F
		const float     FONT_ROT = 0.0f;			//�t�H���g�̌X��
		const Vector2   FONT_PIV = { 1.0f,1.0f };	//�t�H���g�̊�_
		const float     FONT_SCA = 1.0f;			//�t�H���g�̊g�嗦

		const wchar_t* CURSOR_NAME = L"o";
		const wchar_t* KUD_NAME[2] = { L"���܂� :  1000",L"�o��" };
		const wchar_t* WAIT_NAME = L"�ҋ@��...";
	}

	bool HotelSelectFont::Start()
	{
		switch (m_playerNum)
		{
			//1P�̏ꍇ
		case 0:
			for (int fontNum = 0; fontNum < 2; fontNum++)
			{
				FONT_POS[fontNum].x = -550.0f;
				FONT_POS[fontNum].y -= 50.0f;
				CURSOR_POS[fontNum].x = -600.0f;
				CURSOR_POS[fontNum].y -= 50.0f;
			}

			break;

			//2P�̏ꍇ
		case 1:
			for (int fontNum = 0; fontNum < 2; fontNum++)
			{
				FONT_POS[fontNum].x = 280.0f;
				FONT_POS[fontNum].y -= 50.0f;
				CURSOR_POS[fontNum].x = 230.0f;
				CURSOR_POS[fontNum].y -= 50.0f;
			}

			break;
		}

		//�J�[�\��������������
		m_cursorFont = NewGO<FontRender>();
		m_cursorFont->Init
		(
			L"",			//�e�L�X�g
			CURSOR_POS[0],	//�ʒu
			nsHotelSelectFont::FONT_COLOR,	//�F
			nsHotelSelectFont::FONT_ROT,		//�X��
			nsHotelSelectFont::FONT_SCA,		//�g�嗦
			nsHotelSelectFont::FONT_PIV		//��_
		);
		//�����̋��E���\��
		m_cursorFont->SetShadowParam(true, 2.0f, Vector4::Black);
		swprintf_s(m_text, nsHotelSelectFont::CURSOR_NAME);
		m_cursorFont->SetText(m_text);

		//���܂锑�܂�Ȃ�������������
		for (int fontNum = enStay; fontNum < enHotelModeNum; fontNum++)
		{
			m_stayOrEnter[fontNum] = NewGO<FontRender>();
			m_stayOrEnter[fontNum]->Init
			(
				L"",			//�e�L�X�g
				FONT_POS[fontNum],	//�ʒu
				nsHotelSelectFont::FONT_COLOR,	//�F
				nsHotelSelectFont::FONT_ROT,		//�X��
				nsHotelSelectFont::FONT_SCA,		//�g�嗦
				nsHotelSelectFont::FONT_PIV		//��_
			);
			//�����̋��E���\��
			m_stayOrEnter[fontNum]->SetShadowParam(true, 2.0f, Vector4::Black);
			swprintf_s(m_text, nsHotelSelectFont::KUD_NAME[fontNum]);
			m_stayOrEnter[fontNum]->SetText(m_text);
		}

		//�ҋ@��������������
		m_waitFont = NewGO<FontRender>();
		m_waitFont->Init
		(
			L"",			//�e�L�X�g
			FONT_POS[0],	//�ʒu
			nsHotelSelectFont::FONT_COLOR,	//�F
			nsHotelSelectFont::FONT_ROT,		//�X��
			nsHotelSelectFont::FONT_SCA,		//�g�嗦
			nsHotelSelectFont::FONT_PIV		//��_
		);
		//�����̋��E���\��
		m_waitFont->SetShadowParam(true, 2.0f, Vector4::Black);
		swprintf_s(m_text, nsHotelSelectFont::WAIT_NAME);
		m_waitFont->SetText(m_text);
		//���߂͔�\��
		m_waitFont->Deactivate();

		return true;
	}

	HotelSelectFont::~HotelSelectFont()
	{
		//�J�[�\�������폜
		DeleteGO(m_cursorFont);
		//���܂锑�܂�Ȃ������폜
		for (int fontNum = enStay; fontNum < enHotelModeNum; fontNum++)
		{
			DeleteGO(m_stayOrEnter[fontNum]);
		}
		//�ҋ@�������폜
		DeleteGO(m_waitFont);
	}

	void HotelSelectFont::Update()
	{
		switch (m_nowHotelState)
		{
			//���܂锑�܂�Ȃ��̑I�����
		case enSelectState:
			//�I���J�[�\�������̈ʒu���X�V
			m_cursorFont->SetPosition(CURSOR_POS[m_nowHotelMode]);
			//���܂锑�܂�Ȃ������\��
			for (int fontNum = enStay; fontNum < enHotelModeNum; fontNum++)
			{
				m_stayOrEnter[fontNum]->Activate();
			}

			break;

			//���܂���
		case enStayState:
			//���܂锑�܂�Ȃ�������\��
			for (int fontNum = enStay; fontNum < enHotelModeNum; fontNum++)
			{
				m_stayOrEnter[fontNum]->Deactivate();
			}
			//�I���J�[�\�����\��
			m_cursorFont->Deactivate();
			//�ҋ@��������\��
			m_waitFont->Activate();

			break;

			//�X����o��X�^���o�C���
		case enEnterState:
			//���܂锑�܂�Ȃ�������\��
			for (int fontNum = enStay; fontNum < enHotelModeNum; fontNum++)
			{
				m_stayOrEnter[fontNum]->Deactivate();
			}
			//�I���J�[�\�����\��
			m_cursorFont->Deactivate();
			//�ҋ@��������\��
			m_waitFont->Activate();

			break;
		}
	}
};