#pragma once

namespace apn::visual_font::hook
{
	//
	// このクラスはGDI関連APIのフックを担当します。
	//
	inline struct gdi_api_t : entry_t
	{
		//
		// 初期化処理を実行します。
		//
		virtual BOOL on_init() override
		{
			MY_TRACE_FUNC("");

			DetourTransactionBegin();
			DetourUpdateThread(::GetCurrentThread());

			my::hook::attach(ExtTextOutW);

			// フックをコミットします。
			auto result = (DetourTransactionCommit() == NO_ERROR);

			if (!result)
			{
				MY_TRACE("APIフックに失敗しました\n");

				return FALSE;
			}

			return TRUE;
		}

		//
		// 後始末処理を実行します。
		//
		virtual BOOL on_exit() override
		{
			MY_TRACE_FUNC("");

			return TRUE;
		}

		//
		// このクラスは::ExtTextOutW()をフックします。
		//
		struct {
			inline static BOOL hook_proc(HDC dc, int x, int y, UINT options, LPCRECT rc, LPCWSTR text, UINT c, CONST INT* dx)
			{
				// 処理中の場合はTRUEになります。
				static auto flag_processing = FALSE;

				// 処理中の場合はデフォルト処理だけを実行します。
				if (flag_processing)
					return orig_proc(dc, x, y, options, rc, text, c, dx);

				// 処理中フラグをセットします。
				flag_processing = TRUE;

				// スコープ終了時に処理中フラグをリセットします。
				my::scope_exit scope_exit([&]() { flag_processing = FALSE; });

				// フォントリストボックスのフックが有効の場合は
				if (instant_window::font_listbox_t::flag_hook && text)
				{
					// 引数から項目名を取得します。
					auto font_name = std::wstring(text, c);

					// 項目名がフォント名の場合は
					if (available_fonts.contains(font_name))
					{
						// フォントを使用して項目名を描画します。

						my::gdi::unique_ptr<HFONT> font(::CreateFontW(
							hive.font_listbox.font_height, 0, 0, 0, 0, FALSE, FALSE, FALSE,
							DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
							DEFAULT_QUALITY, DEFAULT_PITCH, font_name.c_str()));
						my::gdi::selector font_selector(dc, font.get());

						auto preview_text = hive.font_listbox.sample_text_format.length() ?
							my::replace(hive.font_listbox.sample_text_format, L"%font%", font_name) : font_name;

						return orig_proc(dc, x, y, options, rc,
							preview_text.c_str(), (UINT)preview_text.length(), dx);
					}
				}

				return orig_proc(dc, x, y, options, rc, text, c, dx);
			}
			inline static decltype(&hook_proc) orig_proc = ::ExtTextOutW;
		} ExtTextOutW;
	} gdi_api;
}
