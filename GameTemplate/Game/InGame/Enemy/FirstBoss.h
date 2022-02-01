/// <summary>
/// �ŏ��̃{�X�N���X
/// </summary>
#pragma once
#include "EnemyBase.h"

namespace nsKabutoubatu
{
	/// <summary>
	/// �O���錾
	/// </summary>
	class ConfusionStar;	//�������̐�

	class FirstBoss : public EnemyBase	//�G�l�~�[�̃x�[�X�N���X���p��
	{
	private:
		/// <summary>
		/// �񋓌^
		/// </summary>


		//�A�j���[�V����
		enum enMoleAnimation
		{
			enIdleAnimation,				//�A�C�h���A�j���[�V����
			enWalkAnimation,				//�����A�j���[�V����
			enRunAnimation,					//�_�b�V���A�j���[�V����
			enAttackHornsUpAnimation,		//�p�グ�U���A�j���[�V����
			enAttackHornsForwardAnimation,	//�p�˂��U���A�j���[�V����
			enDamageAnimation,				//�_���[�W�A�j���[�V����
			enDeathAnimation,				//���S�A�j���[�V����
			enAnimationNum					//�A�j���[�V�����̑���
		};

		//HP�o�[UI
		enum enHPBarUI
		{
			enHPBar,		//HP�o�[
			enRemainingHP,	//�c��HP
			enHPBarUINum	//HP�o�[UI�̐�
		};

		//���
		enum enState
		{
			enIdle,			//�A�C�h��
			enNormalState,	//�ʏ�
			enDeathState	//���S
		};

		//�ړ����
		enum enMoveState
		{
			enRandomMoveState,			//�����_���ړ�
			enDashAttackState,			//�_�b�V���U��
			enAttackHornsUpState,		//�p�グ�U��
			enAttackHornsForwardState	//�p�˂��U��
		};

		//�T�E���h
		enum enFirstBossSound
		{
			enLandingSound,					//���n��
			enChargeSound,					//�`���[�W��
			enDashAttackSound,				//�ːi�U����
			enAttackHornsUpSound,			//�p�グ��
			enAttackHornsForwardSound,		//�p�˂���
			enSoundNum						//���ʉ��̑���
		};

		SpriteRender* m_hpBarUI[enHPBarUINum] = { nullptr };
		FontRender* m_firstBossName = nullptr;
		FontRender* m_salespersonTalk = nullptr;
		ConfusionStar* m_confusionStar = nullptr;
		SoundSource* m_sound[enSoundNum] = { nullptr };

		AnimationClip m_animationClips[enAnimationNum];

		int m_idleTimer = 0;
		int m_moveState = enRandomMoveState;
		wchar_t m_firstBossNameText[256];
		Vector3 m_playerToEnemyDistanceDirecion[enPlayerNum];	//�v���C���[�ɐL�тĂ���x�N�g��
		float m_plaToEneDistanceLength[enPlayerNum];			//�v���C���[�Ƃ̋����̒���
		int m_moveStopTimer = 0;
		int m_moveStopTimer2 = 0;
		int m_randomMoveTimer = 0;
		//���ʃx�N�g��
		Vector3 m_forward;
		int m_animState = enIdleAnimation;
		int m_confusionTimer = 0;
		/// <summary>
		/// �t���O
		/// </summary>
		bool m_canConfusion = true;
		bool m_gameClearFlg = false;

		/// <summary>
		/// �Ă΂�čŏ��Ɏ��s�����֐�
		/// </summary>
		void SubStart()override final;

		/// <summary>
		/// �f�X�g���N�^
		/// </summary>
		~FirstBoss();

		/// <summary>
		/// ���t���[�����s�����X�V�����֐�
		/// </summary>
		void SubUpdate()override final;

		/// <summary>
		/// ���t���[�����s�����X�V�����֐�
		/// (�ʏ�̍X�V���~�߂�UI�����X�V�����������Ɏg���X�V�����֐�)
		/// </summary>
		void UiUpdate()override final;

		//�X�V�������\�b�h
		void DataUpdate();

		//�����_���ړ��������\�b�h
		void RandomMove();

		//�v���C���[�Ƃ̋����������\�b�h
		void DistanceSearch();

		//�U���͈͓��ɓ�������t���O�I�����郁�\�b�h
		bool IsAttackRangeIn();

	public:
		/// <summary>
		/// �Q�[���N���A�������ǂ������擾
		/// </summary>
		/// <returns>�Q�[���N���A�������ǂ���</returns>
		bool GetGameClearFlg()const { return m_gameClearFlg; };
	};
}