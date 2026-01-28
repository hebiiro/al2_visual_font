#pragma once

namespace apn::visual_font::instant_window
{
	//
	// このクラスはダイアログのエンティティを管理します。
	//
	inline struct manager_t
	{
		//
		// このクラスはダイアログのエンティティのベースクラスです。
		//
		struct entity_base_t : my::Window, my::lockable_t, std::enable_shared_from_this<entity_base_t>
		{
		};

		//
		// ダイアログのエンティティを保持するためのプールです。
		//
		std::unordered_map<HWND, std::shared_ptr<entity_base_t>> entities;

		//
		// 初期化処理を実行します。
		//
		BOOL init()
		{
			MY_TRACE_FUNC("");

			return TRUE;
		}

		//
		// 後始末処理を実行します。
		//
		BOOL exit()
		{
			MY_TRACE_FUNC("");

			entities.clear();

			return TRUE;
		}

		//
		// ダイアログのエンティティをプールに追加します。
		//
		void add(HWND hwnd, const std::shared_ptr<entity_base_t>& entity)
		{
			entities[hwnd] = entity;
		}

		//
		// ダイアログのエンティティをプールから削除します。
		//
		void erase(HWND hwnd)
		{
			entities.erase(hwnd);
		}
	} manager;

	//
	// このクラスはダイアログのエンティティです。
	//
	struct entity_t : manager_t::entity_base_t
	{
		//
		// 初期化処理です。
		//
		virtual BOOL subclass(HWND hwnd) override
		{
			if (!__super::subclass(hwnd)) return FALSE;

			manager.add(hwnd, shared_from_this());

			return TRUE;
		}

		//
		// 後始末処理です。
		//
		virtual void post_nc_destroy() override
		{
			__super::post_nc_destroy();

			manager.erase(*this);
		}
	};
}
