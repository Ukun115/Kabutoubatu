/// <summary>
/// ��N���X
/// </summary>
#include "stdafx.h"
#include "Arrow.h"
#include "../../Online/OnlineUpdateSpeed.h"

namespace nsKabutoubatu
{
	namespace nsArrow
	{
		const float ARROW_SPEED_POWER = 15.0f;
		const char* ARROW_MODEL_FILE_NAME = "Arrow";
	}

	bool Arrow::Start()
	{
		//�I�����C�����̍X�V���x�Ǘ��N���X�̃C���X�^���X������
		m_onlineUpdateSpeed = FindGO<OnlineUpdateSpeed>(nsStdafx::ONLINEUPDATESPEED_NAME);

		//�ړ����x�𐳋K��
		m_moveSpeed.Normalize();
		//�n�ʂƕ��s�ɂ����ړ����Ȃ��悤�ɂ��邽��y������0�ɂ���B
		m_moveSpeed.y = 0.0f;

		//���f����������
		m_arrowModel = NewGO<SkinModelRender>();
		m_arrowModel->SetShadowCaster(true);	//�e�𗎂Ƃ�
		m_arrowModel->SetOutline(false);		//�֊s�������Ȃ�
		m_arrowModel->SetSilhouette(true);		//�V���G�b�g������
		m_arrowModel->SetPlayerMode(m_playerNum);	//�v���C���[�P���Q����n��
		m_arrowModel->Init(nsArrow::ARROW_MODEL_FILE_NAME);
		//���f���̈ʒu���X�V
		m_arrowModel->SetPosition(m_pos);
		m_moveSpeed *= nsArrow::ARROW_SPEED_POWER * m_onlineUpdateSpeed->GetSpeed();

		//��̌������Z�b�g
		//��]�p�x�����߂�
		float m_rotAngle = atan2(m_moveSpeed.x, m_moveSpeed.z);
		//��]���X�V
		Quaternion m_rot;
		m_rot.SetRotation(Vector3::AxisY, m_rotAngle);
		m_arrowModel->SetRotation(m_rot);

		return true;
	}

	Arrow::~Arrow()
	{
		//���f���폜
		DeleteGO(m_arrowModel);
	}

	void Arrow::Update()
	{
		m_deleteTimer++;
		if (m_deleteTimer > 120 / m_onlineUpdateSpeed->GetSpeed())
		{
			DeleteGO(this);
		}

		//���f���̈ʒu���X�V
		m_pos += m_moveSpeed;
		m_arrowModel->SetPosition(m_pos);
	}
}