#pragma once

namespace apn::visual_font
{
	//
	// このクラスはコンフィグダイアログです。
	//
	inline struct config_dialog_t : my::Dialog, lockable_t
	{
		virtual void on_update_controls() {}
		virtual void on_update_config() {}
		virtual void on_init_dialog() {}
		virtual void on_command(UINT code, UINT id, HWND control) {}

		//
		// 初期化処理を実行します。
		//
		BOOL init()
		{
			MY_TRACE_FUNC("");

			{
				// 初期化中にエディットボックスがコマンドを発行してしまうので、
				// それを防ぐためにロックしておきます。
				locker_t locker(this);

				if (!__super::create(hive.instance, MAKEINTRESOURCE(idd_config), nullptr))
				{
					hive.message_box(L"コンフィグダイアログの作成に失敗しました");

					return FALSE;
				}
			}

			return TRUE;
		}

		//
		// 後始末処理を実行します。
		//
		BOOL exit()
		{
			MY_TRACE_FUNC("");

			return __super::destroy();
		}

		//
		// コンフィグでコントロールを更新します。
		//
		BOOL to_ui()
		{
			MY_TRACE_FUNC("");

			if (is_locked()) return FALSE;

			locker_t locker(this);

			set_check(idc_font_menu_flag_use, hive.font_menu.flag_use);
			set_int(idc_font_menu_item_size_cx, hive.font_menu.item_size.cx);
			set_int(idc_font_menu_item_size_cy, hive.font_menu.item_size.cy);
			set_int(idc_font_menu_space_size_cx, hive.font_menu.space_size.cx);
			set_int(idc_font_menu_space_size_cy, hive.font_menu.space_size.cy);
			set_int(idc_font_menu_font_height, hive.font_menu.font_height);
			set_int(idc_font_menu_nb_rows, hive.font_menu.nb_rows);
			set_text(idc_font_menu_sample_text_format, hive.font_menu.sample_text_format);

			set_check(idc_font_listbox_flag_use, hive.font_listbox.flag_use);
			set_int(idc_font_listbox_window_size_cx, hive.font_listbox.window_size.cx);
			set_int(idc_font_listbox_window_size_cy, hive.font_listbox.window_size.cy);
			set_int(idc_font_listbox_item_height, hive.font_listbox.item_height);
			set_int(idc_font_listbox_font_height, hive.font_listbox.font_height);
			set_text(idc_font_listbox_sample_text_format, hive.font_listbox.sample_text_format);

			set_check(idc_font_menu_dialog_flag_use, hive.font_menu_dialog.flag_use);
			set_check(idc_font_menu_dialog_flag_resize, hive.font_menu_dialog.flag_resize);
			set_check(idc_font_menu_dialog_flag_copy, hive.font_menu_dialog.flag_copy);
			set_text(idc_font_menu_dialog_title, hive.font_menu_dialog.title);

			return TRUE;
		}

		//
		// コントロールでコンフィグを更新します。
		//
		BOOL from_ui()
		{
			MY_TRACE_FUNC("");

			if (is_locked()) return FALSE;

			get_check(idc_font_menu_flag_use, hive.font_menu.flag_use);
			get_int(idc_font_menu_item_size_cx, hive.font_menu.item_size.cx);
			get_int(idc_font_menu_item_size_cy, hive.font_menu.item_size.cy);
			get_int(idc_font_menu_space_size_cx, hive.font_menu.space_size.cx);
			get_int(idc_font_menu_space_size_cy, hive.font_menu.space_size.cy);
			get_int(idc_font_menu_font_height, hive.font_menu.font_height);
			get_int(idc_font_menu_nb_rows, hive.font_menu.nb_rows);
			get_text(idc_font_menu_sample_text_format, hive.font_menu.sample_text_format);

			get_check(idc_font_listbox_flag_use, hive.font_listbox.flag_use);
			get_int(idc_font_listbox_window_size_cx, hive.font_listbox.window_size.cx);
			get_int(idc_font_listbox_window_size_cy, hive.font_listbox.window_size.cy);
			get_int(idc_font_listbox_item_height, hive.font_listbox.item_height);
			get_int(idc_font_listbox_font_height, hive.font_listbox.font_height);
			get_text(idc_font_listbox_sample_text_format, hive.font_listbox.sample_text_format);

			get_check(idc_font_menu_dialog_flag_use, hive.font_menu_dialog.flag_use);
			get_check(idc_font_menu_dialog_flag_resize, hive.font_menu_dialog.flag_resize);
			get_check(idc_font_menu_dialog_flag_copy, hive.font_menu_dialog.flag_copy);
			get_text(idc_font_menu_dialog_title, hive.font_menu_dialog.title);

			return TRUE;
		}

		//
		// ウィンドウプロシージャです。
		//
		virtual LRESULT on_wnd_proc(HWND hwnd, UINT message, WPARAM w_param, LPARAM l_param) override
		{
			switch (message)
			{
			case WM_CREATE:
				{
					// このウィンドウをプラグインウィンドウに設定します。
					hive.plugin_window = hwnd;

					break;
				}
			case WM_DESTROY:
				{
					break;
				}
			case WM_INITDIALOG:
				{
					// スコープ終了時(デフォルト処理の後)に実行します。
					my::scope_exit scope_exit([&]()
					{
						// 子ウィンドウを列挙します。
						::EnumChildWindows(hwnd, [](HWND child, LPARAM l_param) -> BOOL
						{
							// 子ウィンドウを翻訳します。
							::SetWindowTextW(child, tr(my::get_window_text(child)));

							return TRUE;
						},
						0);
					});

					return __super::on_wnd_proc(hwnd, message, w_param, l_param);
				}
			case WM_COMMAND:
				{
					// ロックされている場合はWM_COMMANDを処理しません。
					if (is_locked()) break;

					auto control_id = LOWORD(w_param);
					auto code = HIWORD(w_param);
					auto control = (HWND)l_param;

					switch (control_id)
					{
					// チェックボックス用の処理です。
					case idc_font_menu_flag_use:
					case idc_font_listbox_flag_use:
					case idc_font_menu_dialog_flag_use:
					case idc_font_menu_dialog_flag_resize:
					case idc_font_menu_dialog_flag_copy:
						from_ui(); break;

					// エディットボックス用の処理です。
					case idc_font_menu_item_size_cx:
					case idc_font_menu_item_size_cy:
					case idc_font_menu_space_size_cx:
					case idc_font_menu_space_size_cy:
					case idc_font_menu_font_height:
					case idc_font_menu_nb_rows:
					case idc_font_menu_sample_text_format:
					case idc_font_listbox_window_size_cx:
					case idc_font_listbox_window_size_cy:
					case idc_font_listbox_font_height:
					case idc_font_listbox_item_height:
					case idc_font_listbox_sample_text_format:
					case idc_font_menu_dialog_title:
						if (code == EN_UPDATE) from_ui(); break;
					}

					break;
				}
			}

			return __super::on_wnd_proc(hwnd, message, w_param, l_param);
		}
	} config_dialog;
}
