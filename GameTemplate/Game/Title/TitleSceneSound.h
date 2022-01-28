/// <summary>
/// �^�C�g���V�[���̃T�E���h�N���X
/// </summary>
#pragma once

namespace nsKabutoubatu
{
	/// <summary>
	/// �O���錾
	/// </summary>
	class TitleScene;	//�^�C�g���V�[��

	class TitleSceneSound : public IGameObject
	{
	private:
		SoundSource* m_titleSceneSound = nullptr;
		SoundSource* m_selectSound = nullptr;
		SoundSource* m_decideSound = nullptr;

		/// <summary>
		/// �Ă΂�čŏ��Ɏ��s�����֐�
		/// </summary>
		bool Start()override final;

		/// <summary>
		/// �f�X�g���N�^
		/// </summary>
		~TitleSceneSound();

		/// <summary>
		/// ���t���[�����s�����X�V�����֐�
		/// </summary>
		void Update()override final;

	public:
		//�I�����Đ����\�b�h
		void PlaySelectSound();

		//���艹�Đ����\�b�h
		void PlayDecideSound();

		/// <summary>
		/// ���艹�����Ă��邩�ǂ������擾
		/// </summary>
		/// <returns>���艹�����Ă��邩�ǂ���</returns>
		bool GetIsPlayingDecideSound()const { return m_decideSound->IsPlaying(); }
	};
}