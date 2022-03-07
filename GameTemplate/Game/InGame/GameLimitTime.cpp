/// <summary>
/// �Q�[���̐������ԃN���X
/// </summary>
#include "stdafx.h"
#include "GameLimitTime.h"
#include "../Graphics/Fade.h"

namespace nsKabutoubatu
{
	namespace nsGameLimitTime
	{
		const char* TIME_NEEDLE_NAME[2] = { "TimeNeedle_Long","TimeNeedle_Short" };

		//�Q�[���̐�������(��)
		const int GAMETIME = 4;

		//360�x���b���t���[�� = ���j��1�t���[���Ɉړ�����p�x
		//360 / 60 / 60 = 0.1�x
		const float LONG_NEEDLE_ANGLE = 0.1f;

		const Vector4   FONT_COLOR = { 1.0f,1.0f,1.0f,1.0f };	//�������ԃt�H���g�̐F
		const float     FONT_ROT = 0.0f;			//�t�H���g�̌X��
		const Vector2   FONT_PIV = { 1.0f,1.0f };	//�t�H���g�̊�_

		const int SECONDS = (60 - 1);	//�b
		const int GAME_TIME_LIMIT = (4 - 1);	//�Q�[���̃}�b�N�X�������ԁi���j

		//�������Ԃ̈ʒu
		const Vector2 LIMIT_TIME_POS[3] = { { -480.0f,210.0f }, {-560.0f,210.0f},{-510.0f,215.0f} };
		const float LIMIT_TIME_SCA = 1.0f;
	}

	bool GameLimitTime::Start()
	{
		//�������Ԃ̔w�i
		m_timeBackSprite = NewGO<SpriteRender>(nsStdafx::PRIORITY_0);
		m_timeBackSprite->Init("TimeBack", 250, 125);
		m_timeBackSprite->SetPosition({ -500.0f,180.0f,0.0f });
		//�������Ԃ̎��v�̐j(�Z�j�ƒ��j)
		for (int timeNeedleNum = enTimeNeedleLong; timeNeedleNum < enTimeNeedleNum; timeNeedleNum++)
		{
			m_timeNeedleSprite[timeNeedleNum] = NewGO<SpriteRender>(nsStdafx::PRIORITY_1);
			m_timeNeedleSprite[timeNeedleNum]->Init(nsGameLimitTime::TIME_NEEDLE_NAME[timeNeedleNum], 68, 34);
			m_timeNeedleSprite[timeNeedleNum]->SetPosition({ -583.5f,180.2f,0.0f });
			m_timeNeedleSprite[timeNeedleNum]->SetPivot({ 0.5f,0.3f });
		}

		//�Q�[���̐������Ԃ�ݒ�
		m_nowLimitTime[enMinutes] = nsGameLimitTime::GAME_TIME_LIMIT + 1;
		//�ŏ�0�b
		m_measureTime = pow(nsGameLimitTime::SECONDS, 2);

		//�������Ԃ̃R�����u�F�v
		m_timeColonFont = NewGO<FontRender>();
		m_timeColonFont->Init
		(
			L":",			//�e�L�X�g
			nsGameLimitTime::LIMIT_TIME_POS[2],	//�ʒu
			nsGameLimitTime::FONT_COLOR,	//�F
			nsGameLimitTime::FONT_ROT,		//�X��
			nsGameLimitTime::LIMIT_TIME_SCA,		//�g�嗦
			nsGameLimitTime::FONT_PIV		//��_
		);

		//�������ԃt�H���g(���ƕb)
		for (int limitTimeNum = enSeconds; limitTimeNum < enSecondsMinutesNum; limitTimeNum++)
		{
			m_limitTimeFont[limitTimeNum] = NewGO<FontRender>();
			m_limitTimeFont[limitTimeNum]->Init
			(
				L"",			//�e�L�X�g
				nsGameLimitTime::LIMIT_TIME_POS[limitTimeNum],	//�ʒu
				nsGameLimitTime::FONT_COLOR,	//�F
				nsGameLimitTime::FONT_ROT,		//�X��
				nsGameLimitTime::LIMIT_TIME_SCA,		//�g�嗦
				nsGameLimitTime::FONT_PIV		//��_
			);
			//�b�E���e�L�X�g�X�V
			swprintf_s(m_limitTimeText, L"%d", m_nowLimitTime[limitTimeNum]);
			m_limitTimeFont[limitTimeNum]->SetText(m_limitTimeText);
		}

		return true;
	}

	GameLimitTime::~GameLimitTime()
	{
		//�������Ԃ̃R�����u�F�v
		DeleteGO(m_timeColonFont);
		//�������Ԃ��폜
		for (int limitTimeNum = enSeconds; limitTimeNum < enSecondsMinutesNum; limitTimeNum++)
		{
			DeleteGO(m_limitTimeFont[limitTimeNum]);
		}

		//�������Ԃ̔w�i���폜
		DeleteGO(m_timeBackSprite);
		//�������Ԃ̎��v�̐j���폜
		for (int timeNeedleNum = enTimeNeedleLong; timeNeedleNum < enTimeNeedleNum; timeNeedleNum++)
		{
			DeleteGO(m_timeNeedleSprite[timeNeedleNum]);
		}
	}

	void GameLimitTime::Update()
	{
		if (m_fade != nullptr && m_fade->GetNowState() == 2)
		{
			DeleteGO(m_fade);
			m_fade = nullptr;
		}

		//��������(�b)���o�߂��Ă���
		if (m_canMeasure)
		{
			m_measureTime++;
		}
		m_nowLimitTime[enSeconds] = nsGameLimitTime::SECONDS - (m_measureTime / nsGameLimitTime::SECONDS);
		//�b���O�ɂȂ�����A
		if (m_nowLimitTime[enSeconds] == -1)
		{
			//�v�����Ă���^�C���̏�����
			m_measureTime = 0;
			//�c�莞�ԁi���j�����炷
			m_nowLimitTime[enMinutes]--;

			m_minuteChangeFlg = true;
		}
		//�b�E���e�L�X�g�X�V
		for (int limitTimeNum = enSeconds; limitTimeNum < enSecondsMinutesNum; limitTimeNum++)
		{
			//���̐��ɂȂ�Ȃ��悤�ɕ␳
			m_nowLimitTime[limitTimeNum]  = max(0,m_nowLimitTime[limitTimeNum]);

			if (limitTimeNum == enSeconds && m_nowLimitTime[enSeconds] < 10)
			{
			swprintf_s(m_limitTimeText, L"0%d", m_nowLimitTime[limitTimeNum]);
			}
			else
			{
				swprintf_s(m_limitTimeText, L"%d", m_nowLimitTime[limitTimeNum]);
			}
			m_limitTimeFont[limitTimeNum]->SetText(m_limitTimeText);
		}

		//�P�t���[���j����]�p�x����
		m_timeNeedleRotAngle[enTimeNeedleLong] -= nsGameLimitTime::LONG_NEEDLE_ANGLE;
		m_timeNeedleRotAngle[enTimeNeedleShort] -= double((nsGameLimitTime::LONG_NEEDLE_ANGLE) / (nsGameLimitTime::GAMETIME));

		//double�^�Ƃ��g���Ă��Ă��p�x�͐��m�Ȓl�͏o�Ȃ����߁A�������Ԃ̕����ω����邲�ƂɒZ�j���j������̈ʒu�Ɉړ����␳������悤�ɂ���
		if(m_minuteChangeFlg&& m_nowLimitTime[enMinutes] != nsGameLimitTime::GAME_TIME_LIMIT)
		{
			//���j��0�x�̈ʒu�Ɉړ�
			m_timeNeedleRotAngle[enTimeNeedleLong] = 0.0f;
			//�Z�j�̊p�x�w��
			m_timeNeedleRotAngle[enTimeNeedleShort] = (360/ nsGameLimitTime::GAMETIME)* (m_nowLimitTime[enMinutes]+1);

			m_minuteChangeFlg = false;
		}

		//���v�̐j�̉�]���X�V
		for (int timeNeedleNum = enTimeNeedleLong; timeNeedleNum < enTimeNeedleNum; timeNeedleNum++)
		{
			//�f�O���[�P�ʂł̉�]
			m_timeNeedleRot[timeNeedleNum].SetRotationDeg(Vector3::AxisZ, m_timeNeedleRotAngle[timeNeedleNum]);
			m_timeNeedleSprite[timeNeedleNum]->SetRotation(m_timeNeedleRot[timeNeedleNum]);
		}

	}

	//�������Ԃ������Ȃ������ǂ������擾���郁�\�b�h
	bool GameLimitTime::TimeOver()
	{
		if (m_nowLimitTime[enSeconds] == 0 && m_nowLimitTime[enMinutes] == 0)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	void GameLimitTime::SetCanMeasure(const bool canMeasure)
	{
		m_canMeasure = canMeasure;

		m_fade = NewGO<Fade>();
		m_fade->Init(1.0f,0,0);
	}
}