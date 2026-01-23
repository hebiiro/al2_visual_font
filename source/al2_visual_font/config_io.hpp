#pragma once

namespace apn::visual_font
{
	//
	// このクラスはコンフィグの入出力を担当します。
	//
	inline struct config_io_t : io_base_t
	{
		//
		// 初期化処理を実行します。
		//
		BOOL init()
		{
			MY_TRACE_FUNC("");

			auto module_file_path = my::get_module_file_name(hive.instance);
			auto config_folder_path = module_file_path.parent_path() / L"config";

			path = hive.config_file_name = config_folder_path /
				module_file_path.filename().replace_extension(L".json");
			MY_TRACE_STR(hive.config_file_name);

			// コンフィグフォルダを作成しておきます。
			try { std::filesystem::create_directories(config_folder_path); } catch (...) {}

			return TRUE;
		}

		//
		// 後始末処理を実行します。
		//
		BOOL exit()
		{
			MY_TRACE_FUNC("");

			return TRUE;
		}

		//
		// 更新処理を実行します。
		//
		BOOL update()
		{
			MY_TRACE_FUNC("");

			return config_dialog.to_ui();
		}

		//
		// ノードからフォントメニューの設定を読み込みます。
		//
		BOOL read_font_menu(n_json& root)
		{
			MY_TRACE_FUNC("");

			read_bool(root, "font_menu.flag_use", hive.font_menu.flag_use);
			read_size(root, "font_menu.item_size", hive.font_menu.item_size);
			read_size(root, "font_menu.space_size", hive.font_menu.space_size);
			read_int(root, "font_menu.font_height", hive.font_menu.font_height);
			read_int(root, "font_menu.nb_rows", hive.font_menu.nb_rows);
			read_string(root, "font_menu.sample_text_format", hive.font_menu.sample_text_format);

			return TRUE;
		}

		//
		// ノードにフォントメニューの設定を書き込みます。
		//
		BOOL write_font_menu(n_json& root)
		{
			MY_TRACE_FUNC("");

			write_bool(root, "font_menu.flag_use", hive.font_menu.flag_use);
			write_size(root, "font_menu.item_size", hive.font_menu.item_size);
			write_size(root, "font_menu.space_size", hive.font_menu.space_size);
			write_int(root, "font_menu.font_height", hive.font_menu.font_height);
			write_int(root, "font_menu.nb_rows", hive.font_menu.nb_rows);
			write_string(root, "font_menu.sample_text_format", hive.font_menu.sample_text_format);

			return TRUE;
		}

		//
		// ノードからフォントリストボックスの設定を読み込みます。
		//
		BOOL read_font_listbox(n_json& root)
		{
			MY_TRACE_FUNC("");

			read_bool(root, "font_listbox.flag_use", hive.font_listbox.flag_use);
			read_size(root, "font_listbox.window_size", hive.font_listbox.window_size);
			read_int(root, "font_listbox.item_height", hive.font_listbox.item_height);
			read_int(root, "font_listbox.font_height", hive.font_listbox.font_height);
			read_string(root, "font_listbox.sample_text_format", hive.font_listbox.sample_text_format);

			return TRUE;
		}

		//
		// ノードにフォントリストボックスの設定を書き込みます。
		//
		BOOL write_font_listbox(n_json& root)
		{
			MY_TRACE_FUNC("");

			write_bool(root, "font_listbox.flag_use", hive.font_listbox.flag_use);
			write_size(root, "font_listbox.window_size", hive.font_listbox.window_size);
			write_int(root, "font_listbox.item_height", hive.font_listbox.item_height);
			write_int(root, "font_listbox.font_height", hive.font_listbox.font_height);
			write_string(root, "font_listbox.sample_text_format", hive.font_listbox.sample_text_format);

			return TRUE;
		}

		//
		// ノードから「フォントメニューの設定」ダイアログの設定を読み込みます。
		//
		BOOL read_font_menu_dialog(n_json& root)
		{
			MY_TRACE_FUNC("");

			read_bool(root, "font_menu_dialog.flag_use", hive.font_menu_dialog.flag_use);
			read_bool(root, "font_menu_dialog.flag_resize", hive.font_menu_dialog.flag_resize);
			read_bool(root, "font_menu_dialog.flag_copy", hive.font_menu_dialog.flag_copy);
			read_string(root, "font_menu_dialog.title", hive.font_menu_dialog.title);

			return TRUE;
		}

		//
		// ノードに「フォントメニューの設定」ダイアログの設定を書き込みます。
		//
		BOOL write_font_menu_dialog(n_json& root)
		{
			MY_TRACE_FUNC("");

			write_bool(root, "font_menu_dialog.flag_use", hive.font_menu_dialog.flag_use);
			write_bool(root, "font_menu_dialog.flag_resize", hive.font_menu_dialog.flag_resize);
			write_bool(root, "font_menu_dialog.flag_copy", hive.font_menu_dialog.flag_copy);
			write_string(root, "font_menu_dialog.title", hive.font_menu_dialog.title);

			return TRUE;
		}

		//
		// ノードからコンフィグを読み込みます。
		//
		virtual BOOL read_node(n_json& root)
		{
			MY_TRACE_FUNC("");

			read_font_menu(root);
			read_font_listbox(root);
			read_font_menu_dialog(root);

			return TRUE;
		}

		//
		// ノードにコンフィグを書き込みます。
		//
		virtual BOOL write_node(n_json& root)
		{
			MY_TRACE_FUNC("");

			write_font_menu(root);
			write_font_listbox(root);
			write_font_menu_dialog(root);

			return TRUE;
		}
	} config_io;
}
