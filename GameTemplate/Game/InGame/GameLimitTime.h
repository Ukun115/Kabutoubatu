/// <summary>
/// �Q�[���̐������ԃN���X
/// </summary>
#pragma once

namespace nsKabutoubatu
{

	/// <summary>
	/// �O���錾
	/// </summary>
	class Fade;	//�t�F�[�h

	class GameLimitTime : public IGameObject
	{
	private:
		/// <summary>
		/// �񋓌^
		/// </summary>

		//���v�̐j
		enum enTimeNeedle
		{
			enTimeNeedleLong,	//���j
			enTimeNeedleShort,	//�Z�j
			enTimeNeedleNum		//���v�̐j�̑���
		};

		//��������
		enum enLimitTime
		{
			enSeconds,				//�b
			enMinutes,				//��
			enSecondsMinutesNum		//�������Ԃ̑���
		};

		SpriteRender* m_timeBackSprite = nullptr;
		SpriteRender* m_timeNeedleSprite[enTimeNeedleNum] = { nullptr };
		Fade* m_fade = nullptr;
		//�������ԃt�H���g
		FontRender* m_limitTimeFont[enSecondsMinutesNum] = { nullptr };
		FontRender* m_timeColonFont = nullptr;

		Quaternion m_timeNeedleRot[enTimeNeedleNum];
		double m_timeNeedleRotAngle[enTimeNeedleNum] = { 0.0f };
		wchar_t m_limitTimeText[256];
		int m_measureTime = 0;
		int m_nowLimitTime[enSecondsMinutesNum] = { 0 };
		int m_seconds = 60;
		/// <summary>
		/// �t���O
		/// </summary>
		bool m_minuteChangeFlg = false;
		bool m_canMeasure = true;

		/// <summary>
		/// �Ă΂�čŏ��Ɏ��s�����֐�
		/// </summary>
		bool Start() override final;

		/// <summary>
		/// �f�X�g���N�^
		/// </summary>
		~GameLimitTime();

		/// <summary>
		/// ���t���[�����s�����X�V�����֐�
		/// </summary>
		void Update() override final;

	public:
		//�������Ԃ������Ȃ������ǂ������擾���郁�\�b�h
		bool TimeOver();

		void SetCanMeasure(const bool canMeasure);
	};
}