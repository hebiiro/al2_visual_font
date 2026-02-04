#pragma once

namespace apn::visual_font
{
	//
	// このクラスはデバッグ処理を担当します。
	//
	inline struct debug_t
	{
		//
		// 初期化処理を実行します。
		//
		BOOL init()
		{
#ifdef _DEBUG
//			if (0)
			{
				// 指定されたキーが押されているときだけログを出力するように、
				// カスタムロガーをセットします。
				static struct logger_t : my::Tracer::Logger {
					virtual void output(LPCTSTR raw, LPCTSTR text) override {
						if (::GetKeyState(VK_SHIFT) < 0) ::OutputDebugString(text);
					}
				} logger;
				my::Tracer::logger = &logger;
			}
#endif
			return TRUE;
		}

		//
		// 後始末処理を実行します。
		//
		BOOL exit()
		{
			return TRUE;
		}
	} debug;
}
