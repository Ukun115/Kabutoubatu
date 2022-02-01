/// <summary>
/// �摜�`��N���X
/// </summary>
#pragma once

namespace nsKabutoubatu
{
	class SpriteRender : public IGameObject
	{
	private:
		SpriteInitData m_spriteInitData;	//�X�v���C�g�C�j�b�g�f�[�^�\���̂̃f�[�^���쐬
		Sprite m_sprite;					//�X�v���C�g�N���X���쐬

		Vector3    m_pos = Vector3::Zero;			//�ʒu
		Quaternion m_rot = Quaternion::Identity;	//��]
		Vector3    m_sca = Vector3::One;			//�g�嗦
		Vector4    m_mulColor = Vector4::White;		//�J���[
		Vector2    m_pivot;							//��_
		char m_filePath[256];						//�t�@�C���p�X
		/// <summary>
		/// �t���O
		/// </summary>
		bool m_pivotChangeFlg = false;

		/// <summary>
		/// ���t���[�����s�����X�V�����֐�
		/// </summary>
		void Update()override final;

		/// <summary>
		/// ���t���[�����s�����X�V�����֐�
		/// (�ʏ�̍X�V���~�߂�UI�����X�V�����������Ɏg���X�V�����֐�)
		/// </summary>
		void UiUpdate()override final;

		//�摜�`�惁�\�b�h
		void SpriteDataRender(RenderContext& renderContext)override final;

	public:
		//�摜�����������郁�\�b�h
		void Init(const char* filepath, const float width, const float height, AlphaBlendMode mode = AlphaBlendMode::AlphaBlendMode_Trans);

		/// <summary>
		/// �Z�b�^�[
		/// </summary>
		//�摜�̈ʒu��ݒ肷�郁�\�b�h
		void SetPosition(const Vector3& pos) { m_pos = pos; }

		//�摜�̉�]��ݒ肷�郁�\�b�h
		void SetRotation(const Quaternion& rot) { m_rot = rot; }

		//�摜�̊g�嗦��ݒ肷�郁�\�b�h
		void SetScale(const Vector3& sca) { m_sca = sca; }

		//�摜�̐F��ݒ肷�郁�\�b�h
		void SetMulColor(const float r, const float g, const float b, const float a) { m_sprite.SetMulColor(r, g, b, a); }

		//�摜�̊�_��ݒ肷�郁�\�b�h
		void SetPivot(const Vector2& piv)
		{
			m_pivot = piv;
			m_pivotChangeFlg = true;
		}
	};
}