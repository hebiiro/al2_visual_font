#pragma once

namespace apn::visual_font::hook
{
	//
	// このクラスはWH_CALLWNDPROCRETフックを担当します。
	//
	inline struct cwpr_t : entry_t
	{
		my::win_hook::unique_ptr<> win_hook;

		//
		// 初期化処理を実行します。
		//
		virtual BOOL on_init() override
		{
			MY_TRACE_FUNC("");

			win_hook.reset(::SetWindowsHookEx(WH_CALLWNDPROCRET, win_hook_proc, nullptr, ::GetCurrentThreadId()));

			return TRUE;
		}

		//
		// 後始末処理を実行します。
		//
		virtual BOOL on_exit() override
		{
			MY_TRACE_FUNC("");

			win_hook.reset();

			return TRUE;
		}

		//
		// WH_CALLWNDPROCRETのフックプロシージャです。
		//
		inline static LRESULT CALLBACK win_hook_proc(int code, WPARAM w_param, LPARAM l_param)
		{
			auto cs = (CWPRETSTRUCT*)l_param;
#ifdef _DEBUG
			if (0)
			{
				// デバッグ用のコードです。

				auto hwnd = cs->hwnd;
				auto message = cs->message;
				auto w_param = cs->wParam;
				auto l_param = cs->lParam;
				auto lResult = cs->lResult;
				auto class_name = my::get_class_name(hwnd);

				MY_TRACE_FUNC("{/hex}, {/hex}, {/hex}, {/hex}, {/} => {/hex}",
					hwnd, message, w_param, l_param, class_name, lResult);
			}
#endif
			switch (cs->message)
			{
			case WM_CREATE:
				{
					if (cs->lResult < 0) break;

					//
					// この関数は指定された文字列が等しい場合はTRUEを返します。
					// ただし、大文字小文字は区別しません。
					//
					constexpr auto eq = [](const std::wstring& lhs, LPCWSTR rhs)
					{
						return ::lstrcmpiW(lhs.c_str(), rhs) == 0;
					};

					// 作成されたウィンドウのハンドルを取得します。
					auto hwnd = cs->hwnd;
#ifdef _DEBUG
					// 作成されたウィンドウのクラス名を取得します。
					auto class_name = my::get_class_name(hwnd);

					// 作成されたウィンドウのウィンドウ名を取得します。
					auto window_name = my::get_window_text(hwnd);
#endif
					// リストボックスをサブクラス化します。
					if (std::make_shared<instant_window::font_listbox_t>()->subclass(hwnd)) break;

					// リストビューをサブクラス化します。
					if (std::make_shared<instant_window::font_menu_dialog_t>()->subclass(hwnd)) break;

					break;
				}
			}

			return ::CallNextHookEx(nullptr, code, w_param, l_param);
		}
	} cwpr;
}
