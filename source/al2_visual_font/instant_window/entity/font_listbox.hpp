#pragma once

namespace apn::visual_font::instant_window
{
	//
	// このクラスはフォント一覧リストボックスをサブクラス化します。
	//
	struct font_listbox_t : entity_t
	{
		//
		// フックする必要がある場合はTRUEになります。
		//
		inline static BOOL flag_hook = FALSE;

		//
		// このリストボックスがフォント一覧の場合はTRUEになります。
		//
		BOOL flag_is_font_listbox = FALSE;

		//
		// 元のアイテムの高さです。
		//
		int orig_item_height = 0;

		//
		// チェック済みの項目数です。
		//
		int nb_checks = 0;

		//
		// チェックする最大項目数です。
		//
		int nb_max_checks = 4; // 全数検査は冗長なので、最初のn個だけチェックするようにします。

		//
		// 初期化処理です。
		//
		virtual BOOL subclass(HWND hwnd) override
		{
			{
				// 作成されたウィンドウのクラス名を取得します。
				auto class_name = my::get_class_name(hwnd);

				// リストボックスではない場合はサブクラス化しません。
				if (::lstrcmpi(class_name.c_str(), WC_LISTBOX)) return FALSE;
			}

			{
				// オーナーウィンドウを取得します。
				auto owner = ::GetWindow(hwnd, GW_OWNER);

				// オーナーウィンドウのクラス名を取得します。
				auto class_name = my::get_class_name(owner);

				// オーナーウィンドウがaviutl2ウィンドウではない場合はサブクラス化しません。
				if (class_name != L"aviutl2Manager") return FALSE;
			}

			return __super::subclass(hwnd);
		}

		//
		// フォント一覧リストボックスの表示予定位置を変更します。
		//
		BOOL set_fonts_pos(WINDOWPOS* wp)
		{
			MY_TRACE_FUNC("");
#if 0
			auto dpi = ::GetDpiForWindow(wp->hwnd);
			auto combobox_height = ::MulDiv(orig_item_height, USER_DEFAULT_SCREEN_DPI, dpi);
#else
			auto combobox_height = orig_item_height + 4;
#endif
			auto rc_combobox = RECT { wp->x, wp->y - combobox_height, wp->x + wp->cx, wp->y };
			auto rc_listbox = RECT { 0, 0, hive.font_listbox.window_size.cx, hive.font_listbox.window_size.cy };
			my::client_to_window(wp->hwnd, &rc_listbox);
			auto x = wp->x;
			auto y = wp->y;
			auto w = my::get_width(rc_listbox);
			auto h = my::get_height(rc_listbox);

			auto is_horz_available = FALSE;
			auto rc_monitor = my::get_monitor_rect(wp->hwnd);

			// リストボックスのボトムがモニタのボトムからはみ出す場合は
			if (rc_combobox.bottom + h > rc_monitor.bottom)
			{
				// リストボックスのトップがモニタのトップからはみ出す場合は
				if (rc_combobox.top - h < rc_monitor.top)
				{
					h = std::min<int>(h, my::get_height(rc_monitor));

					// 可能な限りコンボボックスの真横に配置します。
					y = my::get_center_y(rc_combobox) - h / 2;
					y = std::max<int>(y, rc_monitor.top);
					y = std::min<int>(y, rc_monitor.bottom - h);

					// リストボックスのレフトがモニタのレフトからはみ出す場合は
					if (rc_combobox.left - w < rc_monitor.left)
					{
						// リストボックスをコンボボックスの右側に配置します。
						x = rc_combobox.right;
						w = std::min<int>(w, rc_monitor.right - x);
					}
					else
					{
						// リストボックスをコンボボックスの左側に配置します。
						x = rc_combobox.left - w;
					}

					// xとwをセットしたのでフラグを立てます。
					is_horz_available = TRUE;
				}
				else
				{
					// リストボックスをコンボボックスの上側に配置します。
					y = rc_combobox.top - h;
				}
			}
			else
			{
				// リストボックスをコンボボックスの下側に配置します。
				y = rc_combobox.bottom;
			}

			// xとwがまだセットされていない場合は
			if (!is_horz_available)
			{
				// リストボックスの幅がモニタの幅を超えないように調整します。
				w = std::min<int>(w, my::get_width(rc_monitor));

				// リストボックスのレフトがモニタのレフトからはみ出す場合は
				if (rc_combobox.left < rc_monitor.left)
				{
					// リストボックスのレフトがモニタのレフトと一致するように配置します。
					x = rc_monitor.left;
				}
				// リストボックスのライトがモニタのライトからはみ出す場合は
				else if (rc_combobox.left + w > rc_monitor.right)
				{
					// リストボックスのライトがモニタのライトと一致するように配置します。
					x = rc_monitor.right - w;
				}
				else
				{
					// リストボックスのレフトがコンボボックスのレフトと一致するように配置します。
					x = rc_combobox.left;
				}
			}

			wp->x = x;
			wp->y = y;
			wp->cx = w;
			wp->cy = h;

			return TRUE;
		}

		//
		// ウィンドウプロシージャです。
		//
		virtual LRESULT on_wnd_proc(HWND hwnd, UINT message, WPARAM w_param, LPARAM l_param) override
		{
			MY_TRACE_FUNC("{/hex}, {/}, {/hex}, {/hex}", hwnd, my::message_to_string(message), w_param, l_param);

			switch (message)
			{
			case WM_SHOWWINDOW:
				{
					MY_TRACE("WM_SHOWWINDOW\n");

					if (w_param && flag_is_font_listbox)
					{
						MY_TRACE("フォント一覧リストボックスの表示サイズを変更します\n");

						::SendMessage(hwnd, LB_SETITEMHEIGHT, 0, hive.font_listbox.item_height);

						auto rc = my::get_window_rect(hwnd);
						auto wp = WINDOWPOS {
							.hwnd = hwnd,
							.x = rc.left,
							.y = rc.top,
							.cx = my::get_width(rc),
							.cy = my::get_height(rc),
						};
						set_fonts_pos(&wp);

						::SetWindowPos(hwnd, nullptr, wp.x, wp.y, wp.cx, wp.cy,
							SWP_NOZORDER | SWP_NOACTIVATE | SWP_FRAMECHANGED);
					}
					else
					{
						::SendMessage(hwnd, LB_SETITEMHEIGHT, 0, orig_item_height);
					}

					break;
				}
			case LB_RESETCONTENT:
				{
					MY_TRACE("LB_RESETCONTENT\n");

					if (!orig_item_height)
						orig_item_height = (LONG)::SendMessage(hwnd, LB_GETITEMHEIGHT, 0, 0);

					// フォントを使用して描画する場合は
					// とりあえずフォント一覧であるフラグをセットします。
					// このフラグは後続のLB_ADDSTRINGでリセットされる可能性があります。
					flag_is_font_listbox = hive.font_listbox.flag_use;
					nb_checks = 0;

					break;
				}
			case LB_ADDSTRING:
				{
					MY_TRACE("LB_ADDSTRING\n");

					// リストボックスがフォント一覧で
					// なおかつチェックがまだ終わっていない場合は
					if (flag_is_font_listbox && nb_checks < nb_max_checks)
					{
						// 項目名を取得します。
						auto text = (LPCWSTR)l_param;

						MY_TRACE("『{/}』がフォント名かチェックします\n", text);

						// 項目名がフォント名ではない場合は
						if (!available_fonts.contains(text))
						{
							MY_TRACE("このリストボックスはフォント一覧ではありませんでした\n");

							// フォント一覧であるフラグをリセットします。
							flag_is_font_listbox = FALSE;
						}

						// チェック済みの項目数を増やします。
						nb_checks++;
					}

					break;
				}
			}

			// フラグを保存しておきます。
			const auto old_flag_hook = flag_hook;

			// フラグを更新します。
			flag_hook = flag_is_font_listbox;

			// スコープ終了時にフラグを復元します。
			my::scope_exit scope_exit([&]() { flag_hook = old_flag_hook; });

			// デフォルト処理を実行します。
			return __super::on_wnd_proc(hwnd, message, w_param, l_param);
		}
	};
}
