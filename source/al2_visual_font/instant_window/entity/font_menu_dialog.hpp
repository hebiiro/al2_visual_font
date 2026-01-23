#pragma once

namespace apn::visual_font::instant_window
{
	//
	// このクラスは『フォントメニューの設定』ダイアログをサブクラス化します。
	//
	struct font_menu_dialog_t : entity_t
	{
		//
		// ホットキーのIDです。
		//
		inline static constexpr struct c_hot_key_t {
			inline static constexpr uint32_t c_copy = 1;
		} c_hot_key;

		//
		// CDDS_ITEMPREPAINTで使用するフォントです。
		//
		my::gdi::unique_ptr<HFONT> font;

		//
		// 初期化処理です。
		//
		virtual BOOL subclass(HWND hwnd) override
		{
			// クラス名を取得します。
			auto class_name = my::get_class_name(hwnd);

			// ダイアログではない場合はサブクラス化しません。
			if (::lstrcmpiW(class_name.c_str(), L"#32770")) return FALSE;

			// ウィンドウ名を取得します。
			auto window_name = my::get_window_text(hwnd);

			// タイトル名を翻訳します。
			auto title = hive.aviutl2.config->get_language_text(
				hive.aviutl2.config, L"Dialog", hive.font_menu_dialog.title.c_str());

			// 『フォントメニューの設定』ダイアログではない場合はサブクラス化しません。
			if (window_name != title) return FALSE;

			// 『フォントメニューの設定』ダイアログをリサイズ可能にする場合は
			if (hive.font_menu_dialog.flag_resize)
			{
				// ウィンドウスタイルを変更します。
				my::modify_style(hwnd, 0, WS_THICKFRAME);
			}

			// ホットキーでフォント名をコピーする場合は
			if (hive.font_menu_dialog.flag_copy)
			{
				// ホットキーを登録します。
				::RegisterHotKey(hwnd, c_hot_key.c_copy, MOD_CONTROL | MOD_NOREPEAT, L'C');
			}

			// サブクラス化します。
			return __super::subclass(hwnd);
		}

		//
		// カスタムドローを処理します。
		//
		LRESULT on_custom_draw(HWND hwnd, UINT message, WPARAM w_param, LPARAM l_param)
		{
			auto nm = (NMLVCUSTOMDRAW*)l_param;

			MY_TRACE_FUNC("stage = {/hex}, text_color = {/hex}, text_bk_color = {/hex}, sub_item = {/}",
				nm->nmcd.dwDrawStage, nm->clrText, nm->clrTextBk, nm->iSubItem);

			switch (nm->nmcd.dwDrawStage)
			{
			case CDDS_PREPAINT:
				{
					return CDRF_NOTIFYITEMDRAW;
				}
			case CDDS_ITEMPREPAINT:
				{
					auto hwnd = nm->nmcd.hdr.hwndFrom;
					auto dc = nm->nmcd.hdc;
					auto item_index = nm->nmcd.dwItemSpec;
					auto sub_item_index = 0;

					// 項目名を取得します。
					WCHAR text[MAX_PATH] = {};
					ListView_GetItemText(hwnd, item_index, sub_item_index, text, std::size(text));

					// 項目名がフォント名の場合は
					if (available_fonts.contains(text))
					{
						// 項目のサイズを取得します。
						auto rc = RECT {};
						ListView_GetItemRect(hwnd, item_index, &rc, LVIR_LABEL);

						// フォントを作成します。
						font.reset(::CreateFontW(
							my::get_height(rc) - 2, 0, 0, 0, 0, FALSE, FALSE, FALSE,
							DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
							DEFAULT_QUALITY, DEFAULT_PITCH, text));

						// デバイスコンテキストにフォントをセットします。
						::SelectObject(dc, font.get());

						return CDRF_NEWFONT;
					}

					break;
				}
			}

			return __super::on_wnd_proc(hwnd, message, w_param, l_param);
		}

		//
		// ウィンドウプロシージャです。
		//
		virtual LRESULT on_wnd_proc(HWND hwnd, UINT message, WPARAM w_param, LPARAM l_param) override
		{
			MY_TRACE_FUNC("{/hex}, {/}, {/hex}, {/hex}", hwnd, my::message_to_string(message), w_param, l_param);

			switch (message)
			{
			case WM_NOTIFY:
				{
					// リストビューを視覚化しない場合は何もしません。
					if (!hive.font_menu_dialog.flag_use) break;

					auto nm = (NMHDR*)l_param;

					if (nm->code == NM_CUSTOMDRAW)
						return on_custom_draw(hwnd, message, w_param, l_param);

					break;
				}
			case WM_SIZE:
				{
					// リサイズ可能ではない場合は何もしません。
					if (!hive.font_menu_dialog.flag_resize) break;

					// ダイアログのサイズに合わせてコントロールを整列します。

					constexpr auto space = 12;

					auto client_rc = my::get_client_rect(hwnd);

					auto list = ::GetWindow(hwnd, GW_CHILD);
					auto list_rc = my::get_window_rect(list);
					my::map_window_points(nullptr, hwnd, &list_rc);

					auto ok = ::GetWindow(list, GW_HWNDNEXT);
					auto ok_rc = my::get_window_rect(ok);
					my::map_window_points(nullptr, hwnd, &ok_rc);

					auto base_w = my::get_width(ok_rc);
					auto base_h = my::get_height(ok_rc);

					list_rc.left = client_rc.left + space;
					list_rc.top = client_rc.top + space;
					list_rc.right = client_rc.right - space;
					list_rc.bottom = client_rc.bottom - (base_h + space * 2);
					my::set_window_rect(list, &list_rc);

					ok_rc.left = client_rc.right - (base_w + space);
					ok_rc.top = client_rc.bottom - (base_h + space);
					ok_rc.right = ok_rc.left + base_w;
					ok_rc.bottom = ok_rc.top + base_h;
					my::set_window_rect(ok, &ok_rc);

					break;
				}
			case WM_HOTKEY:
				{
					// Ctrl+Cが実行された場合は
					if (w_param == c_hot_key.c_copy)
					{
						// リストビューを取得します。
						auto listview = ::GetWindow(hwnd, GW_CHILD);

						// 選択項目のインデックスを取得します。
						auto selected_item_index = ListView_GetNextItem(listview, -1, LVNI_SELECTED);
						if (selected_item_index < 0) break;

						// 選択項目のテキストを取得します。
						WCHAR text[MAX_PATH] = {};
						ListView_GetItemText(listview, selected_item_index, 0, text, std::size(text));

						// 選択項目のテキストをクリップボードにコピーします。
						if (text[0]) set_clipboard_text(text, hwnd);

						return 0;
					}

					break;
				}
			}

			return __super::on_wnd_proc(hwnd, message, w_param, l_param);
		}
	};
}
