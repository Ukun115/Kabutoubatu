/// <summary>
/// フォント表示処理クラス
/// </summary>
#pragma once

namespace nsKabutoubatu
{
	class FontRender : public IGameObject
	{
	private:
		/// <summary>
		/// 構造体
		/// </summary>

		//フォントの設定する値
		struct FontValue {
			wchar_t text[256];			//テキスト
			Vector2 position;			//位置
			Vector4 color;				//色
			float rotation = 0.0f;		//回転
			float scale = 0.0f;			//拡大率
			Vector2 pivot;				//基点
		};

		Font m_font;				//フォントクラスを作成
		FontValue m_fontValue;		//フォント構造体のデータを作成

		/// <summary>
		/// フラグ
		/// </summary>
		bool m_pivotChangeFlg = false;

	public:

		/// <summary>
		/// 呼ばれて最初に実行される関数
		/// </summary>
		bool Start()override final { return true; }

		//フォントの初期化メソッド
		void Init(
			const wchar_t* text,						//テキスト
			const Vector2 position,						//位置
			Vector4 color,								//色
			float rotation = 0.0f,						//回転
			float scale = 1.0f,							//拡大率
			Vector2 pivot = { 0.5f,0.5f }				//基点
		);

		//描画メソッド
		void FontDataRender(RenderContext& rc)override final;

		/// <summary>
		/// テキストを設定
		/// </summary>
		/// <param name="text">テキスト</param>
		void SetText(const wchar_t* text) { swprintf_s(m_fontValue.text, text); }

		/// <summary>
		/// テキストの位置を設定
		/// </summary>
		/// <param name="pos">テキストの位置</param>
		void SetPosition(const Vector2& pos) { m_fontValue.position = pos; }

		/// <summary>
		/// テキストの拡大率を設定
		/// </summary>
		/// <param name="sca">拡大率</param>
		//テキストの拡大率を設定するメソッド
		void SetScale(const float sca) { m_fontValue.scale = sca; }

		/// <summary>
		/// テキストの色を設定
		/// </summary>
		/// <param name="col">テキストの色</param>
		void SetColor(const Vector4& col) { m_fontValue.color = col; }

		/// <summary>
		/// テキストの基点を設定
		/// </summary>
		/// <param name="pivot">テキストの基点</param>
		//テキストの基点を設定するメソッド
		void SetPivot(const Vector2& pivot) { m_fontValue.pivot = pivot; }

		/// <summary>
		/// テキストの枠の影を設定
		/// </summary>
		/// <param name="isDrawShadow">影を書くかどうか</param>
		/// <param name="shadowOffset">影を描くときのピクセルのオフセット量</param>
		/// <param name="shadowColor">影の色</param>
		void SetShadowParam(const bool isDrawShadow, const float shadowOffset, const Vector4& shadowColor)
		{
			m_font.SetShadowParam(isDrawShadow, shadowOffset, shadowColor);
		}
	};
}