/// <summary>
/// �t�H���g�\�������N���X
/// </summary>
#pragma once

namespace nsKabutoubatu
{
	class FontRender : public IGameObject
	{
	private:
		/// <summary>
		/// �\����
		/// </summary>

		//�t�H���g�̐ݒ肷��l
		struct FontValue {
			wchar_t text[256];			//�e�L�X�g
			Vector2 position;			//�ʒu
			Vector4 color;				//�F
			float rotation = 0.0f;		//��]
			float scale = 0.0f;			//�g�嗦
			Vector2 pivot;				//��_
		};

		Font m_font;				//�t�H���g�N���X���쐬
		FontValue m_fontValue;		//�t�H���g�\���̂̃f�[�^���쐬

		/// <summary>
		/// �t���O
		/// </summary>
		bool m_pivotChangeFlg = false;

	public:

		/// <summary>
		/// �Ă΂�čŏ��Ɏ��s�����֐�
		/// </summary>
		bool Start()override final { return true; }

		//�t�H���g�̏��������\�b�h
		void Init(
			const wchar_t* text,						//�e�L�X�g
			const Vector2 position,						//�ʒu
			Vector4 color,								//�F
			float rotation = 0.0f,						//��]
			float scale = 1.0f,							//�g�嗦
			Vector2 pivot = { 0.5f,0.5f }				//��_
		);

		//�`�惁�\�b�h
		void FontDataRender(RenderContext& rc)override final;

		/// <summary>
		/// �e�L�X�g��ݒ�
		/// </summary>
		/// <param name="text">�e�L�X�g</param>
		void SetText(const wchar_t* text) { swprintf_s(m_fontValue.text, text); }

		/// <summary>
		/// �e�L�X�g�̈ʒu��ݒ�
		/// </summary>
		/// <param name="pos">�e�L�X�g�̈ʒu</param>
		void SetPosition(const Vector2& pos) { m_fontValue.position = pos; }

		/// <summary>
		/// �e�L�X�g�̊g�嗦��ݒ�
		/// </summary>
		/// <param name="sca">�g�嗦</param>
		//�e�L�X�g�̊g�嗦��ݒ肷�郁�\�b�h
		void SetScale(const float sca) { m_fontValue.scale = sca; }

		/// <summary>
		/// �e�L�X�g�̐F��ݒ�
		/// </summary>
		/// <param name="col">�e�L�X�g�̐F</param>
		void SetColor(const Vector4& col) { m_fontValue.color = col; }

		/// <summary>
		/// �e�L�X�g�̊�_��ݒ�
		/// </summary>
		/// <param name="pivot">�e�L�X�g�̊�_</param>
		//�e�L�X�g�̊�_��ݒ肷�郁�\�b�h
		void SetPivot(const Vector2& pivot) { m_fontValue.pivot = pivot; }

		/// <summary>
		/// �e�L�X�g�̘g�̉e��ݒ�
		/// </summary>
		/// <param name="isDrawShadow">�e���������ǂ���</param>
		/// <param name="shadowOffset">�e��`���Ƃ��̃s�N�Z���̃I�t�Z�b�g��</param>
		/// <param name="shadowColor">�e�̐F</param>
		void SetShadowParam(const bool isDrawShadow, const float shadowOffset, const Vector4& shadowColor)
		{
			m_font.SetShadowParam(isDrawShadow, shadowOffset, shadowColor);
		}
	};
}