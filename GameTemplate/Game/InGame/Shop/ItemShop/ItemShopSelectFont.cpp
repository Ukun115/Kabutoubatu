/// <summary>
/// �A�C�e���V���b�v�V�[���̑I�𕶎��N���X
/// </summary>
#include "stdafx.h"
#include "ItemShopSelectFont.h"

namespace nsKabutoubatu
{
	namespace nsItemShopSceneFont
	{
		const Vector4   FONT_COLOR = { 1.0f,1.0f,1.0f,1.0f };	//�������ԃt�H���g�̐F
		const float     FONT_ROT = 0.0f;			//�t�H���g�̌X��
		const Vector2   FONT_PIV = { 1.0f,1.0f };	//�t�H���g�̊�_
		const float     FONT_SCA = 1.0f;			//�t�H���g�̊g�嗦

		const Vector2 ITEM_VALUE_POS[3] = { {-320.0f,-160.0f},{-50.0f,-160.0f},{220.0f,-160.0f} };

		const wchar_t* CURSOR_NAME = L"o";
		const wchar_t* KUD_NAME[3] = { L"����",L"����",L"�o��" };
		const wchar_t* ITEM1KARA4_NAME[4] = { L"�r���V���[�Y",L"�񕜔�",L"�|",L"�߂�" };
		const wchar_t* STATUS_UP_ITEM[6] = { L"�r���V���[�Y",L"���^����", L"�V�g�̗�", L"�񕜔�", L"�n�[�g�N�b�L�[", L"�߂�" };
		const wchar_t* ITEM_NEDAN_NAME[3] = { L"    100",L"    200",L"    500" };
		const wchar_t* WAIT_NAME = L"�ҋ@��...";
		const wchar_t* ITEM_SETUMEI_SENTENCE[3] = { L"�X�e�[�^�X�㏸\n�A�C�e���ł��B\n���g�̈ړ����x��\n�オ��܂��B\n( ��������� :10 )",
													L"�񕜃A�C�e���ł��B\n�������񕜂���\n���Ƃ��ł��܂��B\n�����ɂ͎g���Ȃ�\n�̂Œ���!\n( ��������� :50 )",
													L"����A�C�e���ł��B\n����������G���U��\n���邱�Ƃ��ł��܂��B\n( ��͖����ł� )" };
		const wchar_t* STATUS_UP_ITEM_SETUMEI_SENTENCE[5] = { L"�X�e�[�^�X�㏸\n�A�C�e���ł��B\n���g�̈ړ����x��\n�オ��܂��B\n( ��������� :10 )",
															  L"�擾�R�C���㏸\n�A�C�e���ł��B\n�������ƂɎ擾�{����\n+0.1�{���܂��B\n( ��������� :10 )",
															  L"�W�����v�͏㏸\n�A�C�e���ł��B\n�������ƂɃW�����v��\n�����������Ȃ���\n�����܂��B\n( ��������� :10 )",
															  L"�񕜃A�C�e���ł��B\n�������񕜂���\n���Ƃ��ł��܂��B\n�����ɂ͎g���Ȃ�\n�̂Œ���!\n( ��������� :50 )",
															  L"�񕜗͏㏸\n�A�C�e���ł��B\n�������Ƃɖ�����\n�񕜂���͂�+1\n�㏸���܂��B\n( ��������� :50 )" };
		const Vector2 ITEM_SETUMEI_SENTENCE_POS[2] = { {-550.0f,70.0f},{350.0f,70.0f} };
		const Vector2 SETUMEI_ITEM_NAME_POS[2] = { {-550.0f,120.0f},{350.0f,120.0f} };
	}
	//�r���V���[�Y
	//�����ɂ��
	//�V�g�̗�
	//�񕜔�
	//����

	bool ItemShopSelectFont::Start()
	{
		switch (m_playerNum)
		{
		//1P�̏ꍇ
		case 0:
			for (int fontNum = 0; fontNum < 6; fontNum++)
			{
				FONT_POS[fontNum].x = -550.0f;
				FONT_POS[fontNum].y -= 50.0f;
				CURSOR_POS[fontNum].x = -600.0f;
				CURSOR_POS[fontNum].y -= 50.0f;
			}

			break;

		//2P�̏ꍇ
		case 1:
			for (int fontNum = 0; fontNum < 6; fontNum++)
			{
				FONT_POS[fontNum].x = 320.0f;
				FONT_POS[fontNum].y -= 50.0f;
				CURSOR_POS[fontNum].x = 270.0f;
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
			nsItemShopSceneFont::FONT_COLOR,	//�F
			nsItemShopSceneFont::FONT_ROT,		//�X��
			nsItemShopSceneFont::FONT_SCA,		//�g�嗦
			nsItemShopSceneFont::FONT_PIV		//��_
		);
		//�����̋��E���\��
		m_cursorFont->SetShadowParam(true, 2.0f, Vector4::Black);
		swprintf_s(m_text, nsItemShopSceneFont::CURSOR_NAME);
		m_cursorFont->SetText(m_text);

		//�����A����A�o�镶����������
		for (int fontNum = enBuy; fontNum < enShoppingModeNum; fontNum++)
		{
			m_KUDFont[fontNum] = NewGO<FontRender>();
			m_KUDFont[fontNum]->Init
			(
				L"",			//�e�L�X�g
				FONT_POS[fontNum],	//�ʒu
				nsItemShopSceneFont::FONT_COLOR,	//�F
				nsItemShopSceneFont::FONT_ROT,		//�X��
				nsItemShopSceneFont::FONT_SCA,		//�g�嗦
				nsItemShopSceneFont::FONT_PIV		//��_
			);
			//�����̋��E���\��
			m_KUDFont[fontNum]->SetShadowParam(true, 2.0f, Vector4::Black);
			swprintf_s(m_text, nsItemShopSceneFont::KUD_NAME[fontNum]);
			m_KUDFont[fontNum]->SetText(m_text);
		}

		//�A�C�e���P�A�A�C�e���Q�A�A�C�e���R�A�߂镶����������
		for (int fontNum = enFirstItem; fontNum < enBuySelectStateNum; fontNum++)
		{
			m_Item1kara3BackFont[fontNum] = NewGO<FontRender>();
			m_Item1kara3BackFont[fontNum]->Init
			(
				L"",			//�e�L�X�g
				FONT_POS[fontNum],	//�ʒu
				nsItemShopSceneFont::FONT_COLOR,	//�F
				nsItemShopSceneFont::FONT_ROT,		//�X��
				nsItemShopSceneFont::FONT_SCA,		//�g�嗦
				nsItemShopSceneFont::FONT_PIV		//��_
			);
			//�����̋��E���\��
			m_Item1kara3BackFont[fontNum]->SetShadowParam(true, 2.0f, Vector4::Black);
			swprintf_s(m_text, nsItemShopSceneFont::ITEM1KARA4_NAME[fontNum]);
			m_Item1kara3BackFont[fontNum]->SetText(m_text);
		}
		//�A�C�e���P�A�A�C�e���Q�A�A�C�e���R�A�߂��\��
		for (int fontNum = enFirstItem; fontNum < enBuySelectStateNum; fontNum++)
		{
			m_Item1kara3BackFont[fontNum]->Deactivate();
		}

		//�X�e�[�^�X�A�b�v�A�C�e���P�`�T�Ɩ߂镶����������
		for (int fontNum = enStatusUpItem1; fontNum < enStatusUpItemNum; fontNum++)
		{
			m_statusUpItem1kara5BackFont[fontNum] = NewGO<FontRender>();
			m_statusUpItem1kara5BackFont[fontNum]->Init
			(
				L"",			//�e�L�X�g
				FONT_POS[fontNum],	//�ʒu2.
				nsItemShopSceneFont::FONT_COLOR,	//�F
				nsItemShopSceneFont::FONT_ROT,		//�X��
				nsItemShopSceneFont::FONT_SCA,		//�g�嗦
				nsItemShopSceneFont::FONT_PIV		//��_
			);
			//�����̋��E���\��
			m_statusUpItem1kara5BackFont[fontNum]->SetShadowParam(true, 2.0f, Vector4::Black);
			swprintf_s(m_text, nsItemShopSceneFont::STATUS_UP_ITEM[fontNum]);
			m_statusUpItem1kara5BackFont[fontNum]->SetText(m_text);
		}
		//�A�C�e���P�A�A�C�e���Q�A�A�C�e���R�A�߂��\��
		for (int fontNum = enFirstItem; fontNum < enBuySelectStateNum; fontNum++)
		{
			m_statusUpItem1kara5BackFont[fontNum]->Deactivate();
		}

		//�ҋ@��������������
		m_waitFont = NewGO<FontRender>();
		m_waitFont->Init
		(
			L"",			//�e�L�X�g
			FONT_POS[0],	//�ʒu
			nsItemShopSceneFont::FONT_COLOR,	//�F
			nsItemShopSceneFont::FONT_ROT,		//�X��
			nsItemShopSceneFont::FONT_SCA,		//�g�嗦
			nsItemShopSceneFont::FONT_PIV		//��_
		);
		//�����̋��E���\��
		m_waitFont->SetShadowParam(true, 2.0f, Vector4::Black);
		swprintf_s(m_text, nsItemShopSceneFont::WAIT_NAME);
		m_waitFont->SetText(m_text);
		//���߂͔�\��
		m_waitFont->Deactivate();

		for (int fontNum = 0; fontNum < 3; fontNum++)
		{
			//�A�C�e���̒l�i������������
			m_itemValueFont[fontNum] = NewGO<FontRender>();
			m_itemValueFont[fontNum]->Init
			(
				L"",			//�e�L�X�g
				nsItemShopSceneFont::ITEM_VALUE_POS[fontNum],	//�ʒu
				nsItemShopSceneFont::FONT_COLOR,	//�F
				nsItemShopSceneFont::FONT_ROT,		//�X��
				0.5f,		//�g�嗦
				nsItemShopSceneFont::FONT_PIV		//��_
			);
			//�����̋��E���\��
			m_itemValueFont[fontNum]->SetShadowParam(true, 2.0f, Vector4::Black);
			swprintf_s(m_text, nsItemShopSceneFont::ITEM_NEDAN_NAME[fontNum]);
			m_itemValueFont[fontNum]->SetText(m_text);
		}

		//�A�C�e���������t�H���g��������
		m_itemSetumeiSentence = NewGO<FontRender>();
		m_itemSetumeiSentence->Init
		(
			L"",			//�e�L�X�g
			nsItemShopSceneFont::ITEM_SETUMEI_SENTENCE_POS[m_playerNum],	//�ʒu
			nsItemShopSceneFont::FONT_COLOR,	//�F
			nsItemShopSceneFont::FONT_ROT,		//�X��
			0.5f,		//�g�嗦
			nsItemShopSceneFont::FONT_PIV		//��_
		);
		//�����̋��E���\��
		m_itemSetumeiSentence->SetShadowParam(true, 2.0f, Vector4::Black);
		m_itemSetumeiSentence->Deactivate();	//��\��
		//��������A�C�e�����t�H���g��������
		m_setumeiItemName = NewGO<FontRender>();
		m_setumeiItemName->Init
		(
			L"",			//�e�L�X�g
			nsItemShopSceneFont::SETUMEI_ITEM_NAME_POS[m_playerNum],	//�ʒu
			nsItemShopSceneFont::FONT_COLOR,	//�F
			nsItemShopSceneFont::FONT_ROT,		//�X��
			0.8f,		//�g�嗦
			nsItemShopSceneFont::FONT_PIV		//��_
		);
		//�����̋��E���\��
		m_setumeiItemName->SetShadowParam(true, 2.0f, Vector4::Black);
		m_setumeiItemName->Deactivate();	//��\��

		return true;
	}

	ItemShopSelectFont::~ItemShopSelectFont()
	{
		//�J�[�\�������폜
		DeleteGO(m_cursorFont);
		//�����A����A�o��A�����폜
		for (int fontNum = enBuy; fontNum < enShoppingModeNum; fontNum++)
		{
			DeleteGO(m_KUDFont[fontNum]);
		}
		//�A�C�e���P�A�A�C�e���Q�A�A�C�e���R�A�߂�
		for (int fontNum = enFirstItem; fontNum < enBuySelectStateNum; fontNum++)
		{
			DeleteGO(m_Item1kara3BackFont[fontNum]);
		}
		//�X�e�[�^�X�A�b�v�A�C�e��1�`5�Ɩ߂�\��
		for (int fontNum = enStatusUpItem1; fontNum < enStatusUpItemNum; fontNum++)
		{
			DeleteGO(m_statusUpItem1kara5BackFont[fontNum]);
		}
		//�ҋ@�������폜
		DeleteGO(m_waitFont);
		//�A�C�e���̒l�i�����̍폜
		for (int fontNum = 0; fontNum < 3; fontNum++)
		{
			DeleteGO(m_itemValueFont[fontNum]);
		}
		//�A�C�e���������t�H���g���폜
		DeleteGO(m_itemSetumeiSentence);
		//��������A�C�e�����t�H���g���폜
		DeleteGO(m_setumeiItemName);
	}

	void ItemShopSelectFont::Update()
	{
		switch (m_nowShoppingState)
		{
			//�����A����A�o��A�̑I�����
		case enShoppingModeSelectState:
			//�I���J�[�\�������̈ʒu���X�V
			m_cursorFont->SetPosition(CURSOR_POS[m_nowShoppingMode]);
			//�����A����A�o��A�����\��
			for (int fontNum = 0; fontNum < enShoppingModeNum; fontNum++)
			{
				m_KUDFont[fontNum]->Activate();
			}
			//�A�C�e���P�A�A�C�e���Q�A�A�C�e���R�A�߂��\��
			for (int fontNum = enFirstItem; fontNum < enBuySelectStateNum; fontNum++)
			{
				m_Item1kara3BackFont[fontNum]->Deactivate();
			}
			//�X�e�[�^�X�A�b�v�A�C�e��1�`5�Ɩ߂��\��
			for (int fontNum = enStatusUpItem1; fontNum < enStatusUpItemNum; fontNum++)
			{
				m_statusUpItem1kara5BackFont[fontNum]->Deactivate();
			}

			break;

			//�w�����
		case enBuyState:
			//�I���J�[�\�������̈ʒu���X�V
			m_cursorFont->SetPosition(CURSOR_POS[m_nowShoppingMode]);
			//�����A����A�o��A������\��
			for (int fontNum = enBuy; fontNum < enShoppingModeNum; fontNum++)
			{
				m_KUDFont[fontNum]->Deactivate();
			}
			//�A�C�e���P�A�A�C�e���Q�A�A�C�e���R�A�߂�\��
			for (int fontNum = enFirstItem; fontNum < enBuySelectStateNum; fontNum++)
			{
				if (m_isFontActive)
				{
					//�t�H���g��\��
					m_Item1kara3BackFont[fontNum]->Activate();
					m_cursorFont->Activate();
					//���������\��
					m_itemSetumeiSentence->Deactivate();
					//��������A�C�e�������\��
					m_setumeiItemName->Deactivate();
				}
				else
				{
					//�t�H���g���\��
					m_Item1kara3BackFont[fontNum]->Deactivate();
					m_cursorFont->Deactivate();
					//��������\��
					swprintf_s(m_text, nsItemShopSceneFont::ITEM_SETUMEI_SENTENCE[m_nowShoppingMode]);
					m_itemSetumeiSentence->SetText(m_text);
					m_itemSetumeiSentence->Activate();
					//��������A�C�e������\��
					swprintf_s(m_text, nsItemShopSceneFont::ITEM1KARA4_NAME[m_nowShoppingMode]);
					m_setumeiItemName->SetText(m_text);
					m_setumeiItemName->Activate();
				}
			}

			//����X�{�^���������ꂽ�Ȃ�A
			if (g_pad[m_playerNum]->IsTrigger(enButtonX) && m_nowShoppingMode != 3)
			{
				m_isFontActive = !m_isFontActive;
			}

			break;

			//���p���
		case enSoldState:
			//�I���J�[�\�������̈ʒu���X�V
			m_cursorFont->SetPosition(CURSOR_POS[m_nowShoppingMode]);
			//�����A����A�o��A������\��
			for (int fontNum = enBuy; fontNum < enShoppingModeNum; fontNum++)
			{
				m_KUDFont[fontNum]->Deactivate();
			}
			//�X�e�[�^�X�A�b�v�A�C�e��1�`5�Ɩ߂�\��
			for (int fontNum = enStatusUpItem1; fontNum < enStatusUpItemNum; fontNum++)
			{
				if (m_isFontActive)
				{
					//�t�H���g��\��
					m_statusUpItem1kara5BackFont[fontNum]->Activate();
					m_cursorFont->Activate();
					//���������\��
					m_itemSetumeiSentence->Deactivate();
					//��������A�C�e�������\��
					m_setumeiItemName->Deactivate();
				}
				else
				{
					//�t�H���g���\��
					m_statusUpItem1kara5BackFont[fontNum]->Deactivate();
					m_cursorFont->Deactivate();
					//��������\��
					swprintf_s(m_text, nsItemShopSceneFont::STATUS_UP_ITEM_SETUMEI_SENTENCE[m_nowShoppingMode]);
					m_itemSetumeiSentence->SetText(m_text);
					m_itemSetumeiSentence->Activate();
					//��������A�C�e������\��
					swprintf_s(m_text, nsItemShopSceneFont::STATUS_UP_ITEM[m_nowShoppingMode]);
					m_setumeiItemName->SetText(m_text);
					m_setumeiItemName->Activate();
				}
			}

			//����X�{�^���������ꂽ�Ȃ�A
			if (g_pad[m_playerNum]->IsTrigger(enButtonX) && m_nowShoppingMode != 5)
			{
				m_isFontActive = !m_isFontActive;
			}

			break;

			//�X����o��X�^���o�C���
		case enEnterState:
			//�����A����A�o��A������\��
			for (int fontNum = enBuy; fontNum < enShoppingModeNum; fontNum++)
			{
				m_KUDFont[fontNum]->Deactivate();
			}
			//�I���J�[�\�����\��
			m_cursorFont->Deactivate();
			//�ҋ@��������\��
			m_waitFont->Activate();

			break;
		}
	}
};