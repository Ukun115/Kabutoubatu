/*!
 *@brief	�Q�[���I�u�W�F�N�g�̊��N���X�B
 */

#pragma once

#include <list>
#include <string>
#include "../../GameTemplate/Game/Utilities/Noncopyable.h"
class RenderContext;

/*!
*@brief	�Q�[���I�u�W�F�N�g�B
*/
class IGameObject : private Noncopyable	//�R�s�[�֎~�̂��߂�Noncopyable�N���X���p��
{
public:
	/*!
		*@brief	�f�X�g���N�^
		*/
	virtual ~IGameObject()
	{
	}
public:


	/*!
	*@brief	Update�̒��O�ŌĂ΂��J�n�����B
	*@details
	*
	* �{�֐���true��Ԃ��ƃQ�[���I�u�W�F�N�g�̏��������������Ɣ��f�����</br>
	* Update�֐����Ă΂�o���܂��Btrue��Ԃ��Ĉȍ~��Start�֐��͌Ă΂�Ȃ��Ȃ�܂��B</br>
	* �Q�[���I�u�W�F�N�g�̏������ɕ����t���[��������ꍇ�Ȃǂ�false��Ԃ��āA�������X�e�b�v�Ȃǂ��g����</br>
	* �K�؂ɏ��������s���Ă��������B
	*/
	virtual bool Start() { return true; }
	/*!
		*@brief	�X�V
		*/
	virtual void Update() {}
	//�|�[�Y���Ă΂��̍X�V���\�b�h
	virtual void PauseUpdate() {}
	//UI���o���Ă΂��̍X�V���\�b�h
	virtual void UiUpdate() {}
	/*!
	 *@brief	�`��
	*/
	virtual void ModelRender(RenderContext& renderContext)
	{
		(void)renderContext;
	}
	virtual void FontDataRender(RenderContext& renderContext)
	{
		(void)renderContext;
	}
	virtual void FontDataRenderSub(RenderContext& renderContext)
	{
		(void)renderContext;
	}
	virtual void ShadowRender(RenderContext& renderContext)
	{
		(void)renderContext;
	}
	virtual void ZPrepassRender(RenderContext& renderContext)
	{
		(void)renderContext;
	}
	virtual void SilhouetteRender(RenderContext& renderContext)
	{
		(void)renderContext;
	}
	virtual void SpriteDataRender(RenderContext& renderContext)
	{
		(void)renderContext;
	}
	virtual void SpriteDataRenderSub(RenderContext& renderContext)
	{
		(void)renderContext;
	}
	virtual void MiniMapRender(RenderContext& renderContext)
	{
		(void)renderContext;
	}

public:
	/*!
	*@brief Start�֐������������H
	*/
	bool IsStart() const
	{
		return m_isStart;
	}
	//Start�֐��������I��������B
	void StartEnd()
	{
		m_isStart = true;
	}
	/*!
	*@brief �A�N�e�B�u���ǂ����𔻒�B
	*/
	bool IsActive() const
	{
		return m_isActive;
	}
	/// <summary>
	/// �Q�[���I�u�W�F�N�g���A�N�e�B�u�ɂ���B
	/// </summary>
	void Activate()
	{
		m_isActive = true;
	}
	/// <summary>
	/// �Q�[���I�u�W�F�N�g���A�N�e�B�u�ɂ���B
	/// </summary>
	void Deactivate()
	{
		m_isActive = false;
	}

	/// <summary>
	/// ���S������B
	/// </summary>
	void Dead()
	{
		m_isDead = true;
	}
	/// <summary>
	/// ���S���Ă���H
	/// </summary>
	/// <returns>true���Ԃ��Ă����玀�S���Ă���</returns>
	bool IsDead() const
	{
		return m_isDead;
	}
	/// <summary>
	/// �Q�[���I�u�W�F�N�g�̖��O��ݒ�B
	/// </summary>
	/// <param name="name">���O</param>
	void SetName(const char* name)
	{
		if (name != nullptr) {
			m_name = name;
		}
	}


	std::string GetName() { return m_name; };


	/// <summary>
	/// �����œn���ꂽ���O���A���̃Q�[���I�u�W�F�N�g�̖��O�ƃ}�b�`���邩����B
	/// </summary>
	/// <param name="name"></param>
	/// <returns></returns>
	bool IsMatchName(const char* name) const
	{
		if (strcmp(m_name.c_str(), name) == 0) {
			return true;
		}
		return false;
	}
public:

	void ModelRenderWrapper(RenderContext& renderContext)
	{
		if (m_isActive && m_isStart && !m_isDead ) {
			ModelRender(renderContext);
		}
	}

	void FontDataRenderWrapper(RenderContext& renderContext)
	{
		if (m_isActive && m_isStart && !m_isDead) {
			FontDataRender(renderContext);
		}
	}

	void FontDataRenderWrapperSub(RenderContext& renderContext)
	{
		if (m_isActive && m_isStart && !m_isDead) {
			FontDataRenderSub(renderContext);
		}
	}

	void ShadowRenderWrapper(RenderContext& renderContext)
	{
		if (m_isActive && m_isStart && !m_isDead) {
			ShadowRender(renderContext);
		}
	}

	void ZPrepassRenderWrapper(RenderContext& renderContext)
	{
		if (m_isActive && m_isStart && !m_isDead) {
			ZPrepassRender(renderContext);
		}
	}

	void SilhouetteRenderWrapper(RenderContext& renderContext)
	{
		if (m_isActive && m_isStart && !m_isDead) {
			SilhouetteRender(renderContext);
		}
	}

	void SpriteRenderWrapper(RenderContext& renderContext)
	{
		if (m_isActive && m_isStart && !m_isDead) {
			SpriteDataRender(renderContext);
		}
	}

	void SpriteRenderWrapperSub(RenderContext& renderContext)
	{
		if (m_isActive && m_isStart && !m_isDead) {
			SpriteDataRenderSub(renderContext);
		}
	}

	void MiniMapRenderWrapper(RenderContext& renderContext)
	{
		if (m_isActive && m_isStart && !m_isDead) {
			MiniMapRender(renderContext);
		}
	}

	//�|�[�Y���̂݌Ă΂��X�V���\�b�h
	void PauseUpdateWrapper()
	{
		if (m_isActive && m_isStart && !m_isDead) {
			PauseUpdate();
		}
	}
	//UI���o���̂݌Ă΂��X�V���\�b�h
	void UiUpdateWrapper()
	{
		if (m_isActive && m_isStart && !m_isDead) {
			UiUpdate();
		}
	}

	void UpdateWrapper()
	{
		if (m_isActive && m_isStart && !m_isDead ) {
			Update();
		}
	}

	void StartWrapper()
	{
		if (m_isActive && !m_isStart && !m_isDead ) {
			if (Start()) {
				//���������������B
				m_isStart = true;
			}
		}
	}


	friend class CGameObjectManager;
protected:
	std::string m_name;								//�Q�[���I�u�W�F�N�g�̖��O
	bool m_isStart = false;							//Start�̊J�n�t���O�B
	bool m_isDead = false;							//���S�t���O�B
	bool m_isRegistDeadList = false;				//���S���X�g�ɐς܂�Ă���B
	bool m_isNewFromGameObjectManager;				//GameObjectManager��new���ꂽ�B
	bool m_isRegist = false;						//GameObjectManager�ɓo�^����Ă���H
	bool m_isActive = true;							//Active�t���O�B
};
