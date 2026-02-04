#pragma once

namespace apn::visual_font
{
	//
	// このクラスはコンフィグダイアログです。
	//
	inline struct config_dialog_t : config_dialog_base_t
	{
		//
		// コンフィグでコントロールを更新します。
		//
		virtual BOOL on_to_ui() override
		{
			MY_TRACE_FUNC("");

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
		virtual BOOL on_from_ui() override
		{
			MY_TRACE_FUNC("");

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
		// WM_COMMANDを処理します。
		//
		virtual void on_command(UINT code, UINT control_id, HWND control) override
		{
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
		}
	} config_dialog;
}
