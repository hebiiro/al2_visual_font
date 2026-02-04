#pragma once

namespace apn::visual_font::hook
{
	//
	// このクラスはメニュー関連APIのフックを担当します。
	//
	inline struct menu_api_t : entry_t
	{
		//
		// このクラスはオーナードローを処理するためのダミーウィンドウです。
		//
		struct dummy_window_t : my::Window
		{
			//
			// メニュー描画用のテーマハンドルです。
			//
			my::theme::unique_ptr<> theme, immersive_start_theme;

			//
			// メニューの背景を塗りつぶすためのブラシです。
			//
			my::gdi::unique_ptr<HBRUSH> background_brush;

			//
			// 最後に処理したメニューです。
			//
			HMENU last_menu = {};

			//
			// 初期化処理を実行します。
			//
			BOOL init()
			{
				MY_TRACE_FUNC("");

				// ウィンドウクラスを登録します。
				WNDCLASSEXW wc = { sizeof(wc) };
				wc.style = CS_HREDRAW | CS_VREDRAW;
				wc.lpfnWndProc = ::DefWindowProc;
				wc.hInstance = hive.instance;
				wc.hCursor = ::LoadCursor(nullptr, IDC_ARROW);
				wc.lpszClassName = _T("visual_font.hook.menu_api.dummy_window");
				::RegisterClassExW(&wc);

				// 非表示のオーバーラップウィンドウを作成します。
				return __super::create(
					WS_EX_NOPARENTNOTIFY,
					wc.lpszClassName,
					wc.lpszClassName,
					WS_CAPTION | WS_SYSMENU | WS_THICKFRAME |
					WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
					0, 0, 100, 100,
					nullptr, nullptr, hive.instance, nullptr);
			}

			//
			// 後始末処理を実行します。
			//
			BOOL exit()
			{
				MY_TRACE_FUNC("");

				return destroy();
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
						MY_TRACE_FUNC("{/}, {/hex}, {/hex}", my::message_to_string(message), w_param, l_param);

						theme.reset(::OpenThemeData(hwnd, VSCLASS_MENU));
						immersive_start_theme.reset(::OpenThemeData(hwnd, L"ImmersiveStart::" VSCLASS_MENU));

						break;
					}
				case WM_DESTROY:
					{
						MY_TRACE_FUNC("{/}, {/hex}, {/hex}", my::message_to_string(message), w_param, l_param);

						immersive_start_theme.reset();
						theme.reset();

						break;
					}
				case WM_MEASUREITEM:
					{
						MY_TRACE_FUNC("{/}, {/hex}, {/hex}", my::message_to_string(message), w_param, l_param);

						auto mis = (MEASUREITEMSTRUCT*)l_param;

						// メニュー項目のサイズをセットします。
						mis->itemWidth = hive.font_menu.item_size.cx;
						mis->itemHeight = hive.font_menu.item_size.cy;

						return TRUE;
					}
				case WM_DRAWITEM:
					{
//						MY_TRACE_FUNC("{/}, {/hex}, {/hex}", my::message_to_string(message), w_param, l_param);

						//
						// この関数はメニューウィンドウを返します。
						//
						const auto get_menu_window = [](HDC dc)
						{
							auto current = HWND {};
							while (auto hwnd = ::FindWindowEx(nullptr, current, L"#32768", nullptr))
								current = hwnd;
							return current;
						};

						auto dis = (DRAWITEMSTRUCT*)l_param;
						auto dc = dis->hDC;
						auto menu = (HMENU)dis->hwndItem;
						auto rc = dis->rcItem;
						auto text = my::get_menu_item_text(menu, dis->itemID, MF_BYCOMMAND);
						auto text_rc = rc; ::InflateRect(&text_rc,
							-hive.font_menu.space_size.cx, -hive.font_menu.space_size.cy);

						// 現在処理しているメニューと最後に処理したメニューが異なる場合は
						if (last_menu != menu)
						{
							// 最後に処理したメニューをセットします。
							last_menu = menu;

							// ウィンドウハンドルを取得できた場合は
							if (auto hwnd = get_menu_window(dc))
							{
								MY_TRACE_HWND(hwnd);

								// この処理でポップアップメニューのボーダーの形状を変えられます。
								auto popup_menu_corner_mode = DWMWCP_ROUNDSMALL;
								::DwmSetWindowAttribute(hwnd, DWMWA_WINDOW_CORNER_PREFERENCE,
									&popup_menu_corner_mode, sizeof(popup_menu_corner_mode));

								// この処理でポップアップメニューのボーダーの色を変えられます。
								auto border_color = RGB(0, 0, 0);
								::GetThemeColor(theme.get(), MENU_POPUPBORDERS, 0, TMT_FILLCOLORHINT, &border_color);
								if (border_color == CLR_NONE) border_color = DWMWA_COLOR_NONE;
								::DwmSetWindowAttribute(hwnd, DWMWA_BORDER_COLOR, &border_color, sizeof(border_color));
							}
						}

						auto part_id = MENU_POPUPITEM;
						auto state_id = MPI_NORMAL;
						if (dis->itemState & ODS_DISABLED) state_id = MPI_DISABLED;
						else if (dis->itemState & ODS_SELECTED) state_id = MPI_HOT;

						MY_TRACE("text={/}, itemID={/hex}, itemData={/hex}, itemAction={/hex}, itemState={/hex}, state_id={/}\n",
							text, dis->itemID, dis->itemData, dis->itemAction, dis->itemState, state_id);

						// ガターなしの背景を描画します。
						::DrawThemeBackground(immersive_start_theme.get(), dc, part_id, state_id, &rc, nullptr);

						// テキストがフォント名の場合は
						if (available_fonts.contains(text))
						{
							// テキストの色を取得します。
							auto text_color = RGB(0, 0, 0);
							::GetThemeColor(theme.get(), part_id, state_id, TMT_TEXTCOLOR, &text_color);

							// デバイスコンテキストの状態をセットします。
							auto old_text_color = ::SetTextColor(dc, text_color);
							auto old_bk_mode = ::SetBkMode(dc, TRANSPARENT);

							// デバイスコンテキストにフォントをセットします。
							my::gdi::unique_ptr<HFONT> font(::CreateFontW(
								hive.font_menu.font_height, 0, 0, 0, 0, FALSE, FALSE, FALSE,
								DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
								DEFAULT_QUALITY, DEFAULT_PITCH, text.c_str()));
							my::gdi::selector font_selector(dc, font.get());

							// プレビュー用テキストを作成します。
							auto preview_text = hive.font_menu.sample_text_format.empty() ?
								text : my::replace(hive.font_menu.sample_text_format, L"%font%", text);

							// テキストを描画します。
							::DrawTextW(dc, preview_text.c_str(), (int)preview_text.length(), &text_rc, DT_LEFT | DT_TOP);
//							::DrawTextW(dc, text.c_str(), (int)text.length(), &text_rc, DT_LEFT | DT_VCENTER | DT_SINGLELINE);

							// デバイスコンテキストの状態をリセットします。
							::SetBkMode(dc, old_bk_mode);
							::SetTextColor(dc, old_text_color);
						}
						else
						{
							// テキストを描画します。
							::DrawThemeText(theme.get(), dc, part_id, state_id,
								text.c_str(), (int)text.length(), DT_LEFT | DT_VCENTER | DT_SINGLELINE, 0, &text_rc);
						}

						// サブメニュー項目の場合は
						if (::IsMenu((HMENU)(UINT_PTR)dis->itemID))
						{
							// アイコンのサイズです。
							constexpr auto c_icon_size = 14;

							// アイコンの位置を算出します。
							auto icon_rc = rc;
							icon_rc.left = rc.right - 26;
							icon_rc.top = (rc.top + rc.bottom) / 2 - c_icon_size / 2;
							icon_rc.right = icon_rc.left + c_icon_size;
							icon_rc.bottom = icon_rc.top + c_icon_size;

							// アイコンを描画します。
							::DrawThemeBackground(theme.get(), dc, MENU_POPUPSUBMENU, MPI_NORMAL, &icon_rc, nullptr);

							// システムに上書きされないようにクリップします。
							{
								auto rgn_rc = rc;
								rgn_rc.left = rgn_rc.right - my::get_height(rc);

								my::gdi::unique_ptr<HRGN> rgn(::CreateRectRgnIndirect(&rgn_rc));

								::ExtSelectClipRgn(dc, rgn.get(), RGN_DIFF);
							}
						}

						return TRUE;
					}
				}

				return __super::on_wnd_proc(hwnd, message, w_param, l_param);
			}
		} dummy_window;

		//
		// 初期化処理を実行します。
		//
		virtual BOOL on_init() override
		{
			MY_TRACE_FUNC("");

			DetourTransactionBegin();
			DetourUpdateThread(::GetCurrentThread());

			my::hook::attach(TrackPopupMenu);

			// フックをコミットします。
			auto result = (DetourTransactionCommit() == NO_ERROR);

			if (!result)
			{
				MY_TRACE("APIフックに失敗しました\n");

				return FALSE;
			}

			return dummy_window.init();
		}

		//
		// 後始末処理を実行します。
		//
		virtual BOOL on_exit() override
		{
			MY_TRACE_FUNC("");

			return dummy_window.exit();
		}

		//
		// 指定されたメニューがフォントメニューの場合はTRUEを返します。
		//
		inline static BOOL is_font_menu(HMENU menu)
		{
			// メニュー項目の数を取得します。
			auto c = ::GetMenuItemCount(menu);

			// メニュー項目を走査します。
			for (decltype(c) i = 0; i < c; i++)
			{
				// メニュー項目のテキストを取得します。
				auto text = my::get_menu_item_text(menu, i, MF_BYPOSITION);

				// テキストがフォント名の場合はTRUEを返します。
				if (available_fonts.contains(text)) return TRUE;
			}

			// フォント名が見つからなかった場合はFALSEを返します。
			return FALSE;
		}

		//
		// 指定されたメニューをビジュアライズします。
		//
		BOOL visualize(HMENU menu)
		{
			// 最後に処理したメニューをリセットします。
			dummy_window.last_menu = {};

			// メニューの背景色を変更します。
			{
				// 背景色のブラシを作成します。
				auto background_color = RGB(0, 0, 0);
				::GetThemeColor(dummy_window.immersive_start_theme.get(),
					MENU_POPUPBACKGROUND, 0, TMT_FILLCOLOR, &background_color);
				dummy_window.background_brush.reset(::CreateSolidBrush(background_color));

				// メニュー情報を更新します。
				MENUINFO mi = { sizeof(mi) };
				mi.fMask = MIM_BACKGROUND | MIM_APPLYTOSUBMENUS;
				mi.hbrBack = dummy_window.background_brush.get();
				::SetMenuInfo(menu, &mi);
			}

			//
			// この関数は指定されたメニューを再帰的にビジュアライズします。
			//
			const auto visualize = [](const auto& self, HMENU menu) -> BOOL
			{
				// メニュー項目の数を取得します。
				auto c = ::GetMenuItemCount(menu);

				// メニュー項目を走査します。
				for (decltype(c) i = 0; i < c; i++)
				{
					// メニュー項目情報を取得します。
					MENUITEMINFOW mii = { sizeof(mii) };
					mii.fMask = MIIM_FTYPE | MIIM_SUBMENU;
					::GetMenuItemInfoW(menu, i, TRUE, &mii);

					// メニュー項目のテキストを取得します。
					auto text = my::get_menu_item_text(menu, i, MF_BYPOSITION);

					// テキストがフォント名の場合は
//					if (available_fonts.contains(text))
					{
						// オーナードローで描画するようにします。
						mii.fType |= MFT_OWNERDRAW;
					}

					// 改行が指定されている場合は
					if (i && hive.font_menu.nb_rows > 0 && !(i % hive.font_menu.nb_rows))
					{
						// ここで改行するようにします。
						mii.fType |= MFT_MENUBREAK;
					}

					// メニュー情報を更新します。
					::SetMenuItemInfoW(menu, i, TRUE, &mii);

					// サブメニューが存在する場合は再帰的に処理します。
					if (mii.hSubMenu) self(self, mii.hSubMenu);
				}

				return TRUE;
			};

			// メニューを再帰的にビジュアライズします。
			return visualize(visualize, menu);
		}

		//
		// このクラスは::TrackPopupMenu()をフックします。
		//
		struct {
			inline static BOOL WINAPI hook_proc(HMENU menu, UINT flags, int x, int y, int reserved, HWND hwnd, LPCRECT rc)
			{
				MY_TRACE_FUNC("{/hex}, {/hex}, {/}, {/}, {/hex}", menu, flags, x, y, hwnd);

				// フォントメニューをビジュアライズする場合は
				if (hive.font_menu.flag_use && flags == (TPM_RIGHTBUTTON | TPM_NONOTIFY | TPM_RETURNCMD))
				{
					// ポップアップメニューがフォントメニューの場合は
					if (is_font_menu(menu))
					{
						// フォントメニューをビジュアライズします。
						menu_api.visualize(menu);
/*
						// WH_CALLWNDPROCRETフックをセットします。
						my::win_hook::unique_ptr<> win_hook(
							::SetWindowsHookEx(WH_CALLWNDPROCRET,
								[](int code, WPARAM w_param, LPARAM l_param) -> LRESULT
						{
							auto cs = (CWPRETSTRUCT*)l_param;
#ifdef _DEBUG
//							if (0)
							{
								// デバッグ用のコードです。

								auto hwnd = cs->hwnd;
								auto message = cs->message;
								auto w_param = cs->wParam;
								auto l_param = cs->lParam;
								auto lResult = cs->lResult;
								auto class_name = my::get_class_name(hwnd);

								MY_TRACE_FUNC("{/hex}, {/}, {/hex}, {/hex}, {/} => {/hex}",
									hwnd, my::message_to_string(message), w_param, l_param, class_name, lResult);
							}
#endif
							return ::CallNextHookEx(nullptr, code, w_param, l_param);
						},
						nullptr, ::GetCurrentThreadId()));
*/
						// ビジュアライズされたフォントメニューを表示します。
						return orig_proc(menu, flags, x, y, reserved, menu_api.dummy_window, rc);
					}
				}

				return orig_proc(menu, flags, x, y, reserved, hwnd, rc);
			}
			inline static decltype(&hook_proc) orig_proc = ::TrackPopupMenu;
		} TrackPopupMenu;
	} menu_api;
}
