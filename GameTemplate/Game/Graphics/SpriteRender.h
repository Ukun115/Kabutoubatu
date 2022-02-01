/// <summary>
/// 画像描画クラス
/// </summary>
#pragma once

namespace nsKabutoubatu
{
	class SpriteRender : public IGameObject
	{
	private:
		SpriteInitData m_spriteInitData;	//スプライトイニットデータ構造体のデータを作成
		Sprite m_sprite;					//スプライトクラスを作成

		Vector3    m_pos = Vector3::Zero;			//位置
		Quaternion m_rot = Quaternion::Identity;	//回転
		Vector3    m_sca = Vector3::One;			//拡大率
		Vector4    m_mulColor = Vector4::White;		//カラー
		Vector2    m_pivot;							//基点
		char m_filePath[256];						//ファイルパス
		/// <summary>
		/// フラグ
		/// </summary>
		bool m_pivotChangeFlg = false;

		/// <summary>
		/// 毎フレーム実行される更新処理関数
		/// </summary>
		void Update()override final;

		/// <summary>
		/// 毎フレーム実行される更新処理関数
		/// (通常の更新を止めてUIだけ更新させたい時に使う更新処理関数)
		/// </summary>
		void UiUpdate()override final;

		//画像描画メソッド
		void SpriteDataRender(RenderContext& renderContext)override final;

	public:
		//画像を初期化するメソッド
		void Init(const char* filepath, const float width, const float height, AlphaBlendMode mode = AlphaBlendMode::AlphaBlendMode_Trans);

		/// <summary>
		/// セッター
		/// </summary>
		//画像の位置を設定するメソッド
		void SetPosition(const Vector3& pos) { m_pos = pos; }

		//画像の回転を設定するメソッド
		void SetRotation(const Quaternion& rot) { m_rot = rot; }

		//画像の拡大率を設定するメソッド
		void SetScale(const Vector3& sca) { m_sca = sca; }

		//画像の色を設定するメソッド
		void SetMulColor(const float r, const float g, const float b, const float a) { m_sprite.SetMulColor(r, g, b, a); }

		//画像の基点を設定するメソッド
		void SetPivot(const Vector2& piv)
		{
			m_pivot = piv;
			m_pivotChangeFlg = true;
		}
	};
}