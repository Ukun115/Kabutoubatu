/// <summary>
/// �t�F�[�h�N���X
/// </summary>
#pragma once

namespace nsKabutoubatu
{
	class OnlineUpdateSpeed;

	class Fade : public IGameObject
	{
	private:
		/// <summary>
		/// �񋓌^
		/// </summary>

		//�t�F�[�h�̏��
		enum enState {
			StateIn,	//�t�F�[�h�C��
			StateOut,	//�t�F�[�h�A�E�g
			StateWait,	//�ҋ@
		};

		//�t�F�[�h�̐F
		enum enFadeColor
		{
			enWhite,
			enBlack
		};

		//�t�F�[�h�摜
		SpriteRenderSub* m_fadeSprite = nullptr;

		OnlineUpdateSpeed* m_onlineUpdateSpeed = nullptr;

		int m_fadeColorState = enWhite;
		//�摜�̃��l
		float m_alphaValue = 0.0f;
		int m_nowState = StateIn;  //���݂̃X�e�[�g

		/// <summary>
		/// �f�X�g���N�^
		/// </summary>
		~Fade();

		/// <summary>
		/// ���t���[�����s�����X�V�����֐�
		/// </summary>
		void Update()override final;

		/// <summary>
		/// ���t���[�����s�����X�V�����֐�
		/// (�ʏ�̍X�V���~�߂�UI�����X�V�����������Ɏg���X�V�����֐�)
		/// </summary>
		void UiUpdate()override final;

	public:
		/// <summary>
		/// �t�F�[�h�摜��������
		/// </summary>
		/// <param name="alphaValue">���l</param>
		/// <param name="nowState">�t�F�[�h�̏��</param>
		/// <param name="fadeColorState">�t�F�[�h�̐F�̃X�e�[�g</param>
		void Init(const float alphaValue, const int nowState, const int fadeColorState);

		/// <summary>
		/// �t�F�[�h�摜�̃��l���擾
		/// </summary>
		/// <returns>�t�F�[�h�摜�̃��l</returns>
		float GetAlphaValue()const { return m_alphaValue; }

		/// <summary>
		/// �t�F�[�h�̏�Ԃ��擾
		/// </summary>
		/// <returns>�t�F�[�h�̏��</returns>
		int GetNowState()const { return m_nowState; }
	};
}