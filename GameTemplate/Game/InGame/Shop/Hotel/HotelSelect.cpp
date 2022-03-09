/// <summary>
/// �h���V�[���̑I���N���X
/// </summary>
#include "stdafx.h"
#include "HotelSelect.h"
#include "../../Player/PlayerStatus.h"
#include "HotelSelectFont.h"

namespace nsKabutoubatu
{
	namespace nsHotelSelect
	{
		//�h����
		const int STAY_MONEY = 1000;
	}

	bool HotelSelect::Start()
	{
		//�v���C���[�̃C���X�^���X������
		m_playerStatus = FindGO<PlayerStatus>(nsStdafx::PLAYER_STATUS_NAME[m_playerNum]);

		//�I���𕶎��\��������N���X�𐶐�
		m_hotelSelectFont = NewGO<HotelSelectFont>();
		//�v���C���[�ԍ����Z�b�g
		m_hotelSelectFont->SetPlayerNum(m_playerNum);

		return true;
	}

	HotelSelect::~HotelSelect()
	{
		//�z�e���I�𕶎��N���X���폜
		DeleteGO(m_hotelSelectFont);
	}

	void HotelSelect::Update()
	{
		switch (m_nowHotelState)
		{
			//���܂锑�܂�Ȃ��̑I�����
		case enSelectState:

			//�I���J�[�\���ړ�
			CursorMove(enStay, enEnter);

			//A�{�^���������ꂽ��A
			if (m_playerGamePad != nullptr)
			{
				if (m_playerGamePad->IsTrigger(enButtonA))
				{
					//���܂��ԁA�X����o��X�^���o�C��Ԃ̂����ꂩ�ɍs���B
					NextState();
					//�����l�ɖ߂��Ă���
					m_nowHotelMode = 0;
				}
			}
			else
			{
				if (g_pad[m_playerNum]->IsTrigger(enButtonA))
				{
					//���܂��ԁA�X����o��X�^���o�C��Ԃ̂����ꂩ�ɍs���B
					NextState();
					//�����l�ɖ߂��Ă���
					m_nowHotelMode = 0;
				}
			}

			break;

			//���܂���
		case enStayState:
			//��������ł��Ȃ�
			break;

			//�X����o��X�^���o�C���
		case enEnterState:
			//��������ł��Ȃ�
			break;
		}
		//�z�e���Z���N�g�����N���X�̏�Ԃ��X�V
		m_hotelSelectFont->SetNowHotelState(m_nowHotelState);
		m_hotelSelectFont->SetNowHotelMode(m_nowHotelMode);
	}

	//�I�����\�b�h
	void HotelSelect::CursorMove(const int selectMin, const int selectMax)
	{
		//������
		if (m_playerGamePad != nullptr)
		{
			if (m_playerGamePad->IsTrigger(enButtonDown)) {
				//���݃Z���N�g����Ă���̂��u�o��v��������A
				if (m_nowHotelMode == selectMax) {
					//�I������ԏ�ɖ߂�
					m_nowHotelMode = selectMin;
				}
				else
				{
					//����ɂ��炷
					m_nowHotelMode += 1;
				}

				//�I���T�E���h�̏�����
				m_selectSound = NewGO<SoundSource>();
				m_selectSound->Init(L"Assets/sound/Select.wav");
				m_selectSound->SetVolume(0.5f);
				m_selectSound->Play(false);	//�����V���b�g�Đ�
			}
			//�����
			if (m_playerGamePad->IsTrigger(enButtonUp)) {
				//���݃Z���N�g����Ă���̂��u�o��v��������A
				if (m_nowHotelMode == selectMin) {
					//�I������ԉ��ɖ߂�
					m_nowHotelMode = selectMax;
				}
				else
				{
					//���ɂ��炷
					m_nowHotelMode -= 1;
				}

				//�I���T�E���h�̏�����
				m_selectSound = NewGO<SoundSource>();
				m_selectSound->Init(L"Assets/sound/Select.wav");
				m_selectSound->SetVolume(0.5f);
				m_selectSound->Play(false);	//�����V���b�g�Đ�
			}
		}
		else
		{
			if (g_pad[m_playerNum]->IsTrigger(enButtonDown)) {
				//���݃Z���N�g����Ă���̂��u�o��v��������A
				if (m_nowHotelMode == selectMax) {
					//�I������ԏ�ɖ߂�
					m_nowHotelMode = selectMin;
				}
				else
				{
					//����ɂ��炷
					m_nowHotelMode += 1;
				}

				//�I���T�E���h�̏�����
				m_selectSound = NewGO<SoundSource>();
				m_selectSound->Init(L"Assets/sound/Select.wav");
				m_selectSound->SetVolume(0.5f);
				m_selectSound->Play(false);	//�����V���b�g�Đ�
			}
			//�����
			if (g_pad[m_playerNum]->IsTrigger(enButtonUp)) {
				//���݃Z���N�g����Ă���̂��u�o��v��������A
				if (m_nowHotelMode == selectMin) {
					//�I������ԉ��ɖ߂�
					m_nowHotelMode = selectMax;
				}
				else
				{
					//���ɂ��炷
					m_nowHotelMode -= 1;
				}

				//�I���T�E���h�̏�����
				m_selectSound = NewGO<SoundSource>();
				m_selectSound->Init(L"Assets/sound/Select.wav");
				m_selectSound->SetVolume(0.5f);
				m_selectSound->Play(false);	//�����V���b�g�Đ�
			}
		}
	}

	//���̏�Ԃɍs�����\�b�h
	void HotelSelect::NextState()
	{
		switch (m_nowHotelMode)
		{
		case enStay:
			//�������K�v���Ȃ��Ƃ��A���܂郂�[�h�ɂȂ�Ȃ��B
			if (m_playerStatus->GetCoinNum() < nsHotelSelect::STAY_MONEY)
			{
				//�Ԃԁ[�T�E���h�̏�����
				m_cantDecideSound = NewGO<SoundSource>();
				m_cantDecideSound->Init(L"Assets/sound/bubu.wav");
				m_cantDecideSound->SetVolume(0.5f);
				m_cantDecideSound->Play(false);	//�����V���b�g�Đ�

				return;
			}
			//�q�b�g�|�C���g�����^���̎��A���܂郂�[�h�ɂȂ�Ȃ��B
			if (m_playerStatus->GetHitPoint() == m_playerStatus->GetMaxHitPoint())
			{
				//�Ԃԁ[�T�E���h�̏�����
				m_cantDecideSound = NewGO<SoundSource>();
				m_cantDecideSound->Init(L"Assets/sound/bubu.wav");
				m_cantDecideSound->SetVolume(0.5f);
				m_cantDecideSound->Play(false);	//�����V���b�g�Đ�

				return;
			}
			//���܂郂�[�h�ɂ���
			m_nowHotelState = enStayState;
			//�h�����̂������g��
			m_playerStatus->ReduceMoney(nsHotelSelect::STAY_MONEY);

			//�w���T�E���h�̏�����
			m_buySound = NewGO<SoundSource>();
			m_buySound->Init(L"Assets/sound/buy.wav");
			m_buySound->SetVolume(0.5f);
			m_buySound->Play(false);	//�����V���b�g�Đ�

			break;

		case enEnter:
			//���X���o��X�^���o�C���[�h�ɂ���
			m_nowHotelState = enEnterState;

			//����T�E���h�̏�����
			m_decideSound = NewGO<SoundSource>();
			m_decideSound->Init(L"Assets/sound/Decide.wav");
			m_decideSound->SetVolume(0.5f);
			m_decideSound->Play(false);	//�����V���b�g�Đ�

			break;
		}
	}
}