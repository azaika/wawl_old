#pragma once

#define WAWL_ENABLE_MENU

//wawl Header
#include "System.h"
#include "BaseType.h"
#include "Utility.h"
//C++ STL
#include <memory>

namespace wawl {
	namespace wnd {
		
		class Bitmap {
		public:
			Bitmap() = default;
			Bitmap(const Bitmap& bmp) {
				bitmap_ = ValType(
					new ::HBITMAP(
						static_cast<::HBITMAP>(
							::CopyImage(bmp(), IMAGE_BITMAP, 0, 0, LR_COPYRETURNORG)
							)
						),
					release
					);

				if (bitmap_ == nullptr)
					throw Error(::GetLastError());
			}
			Bitmap(Bitmap&& bmp) {
				bitmap_ = std::move(bmp.bitmap_);
			}
			Bitmap(const ::HBITMAP hbm) {
				bitmap_ = ValType(
					new ::HBITMAP(
						static_cast<::HBITMAP>(
							::CopyImage(hbm, IMAGE_BITMAP, 0, 0, LR_COPYRETURNORG)
							)
						),
					release
					);

				if (bitmap_ == nullptr)
					throw Error(::GetLastError());
			}

			Bitmap(const TString& path) {
				bitmap_ = std::unique_ptr<::HBITMAP, decltype(&release)>(
					new ::HBITMAP(
						static_cast<::HBITMAP>(
							::LoadImage(
								nullptr,
								path.c_str(),
								IMAGE_BITMAP,
								0,
								0,
								LR_DEFAULTSIZE | LR_LOADFROMFILE
								)
							)
						),
					release
					);

				if (bitmap_ == nullptr)
					throw Error(::GetLastError());
			}

			Bitmap& operator = (const Bitmap& bmp) {
				bitmap_ = std::unique_ptr<::HBITMAP, decltype(&release)>(
					new ::HBITMAP(
						static_cast<::HBITMAP>(
							::CopyImage(bmp(), IMAGE_BITMAP, 0, 0, LR_COPYRETURNORG)
							)
						),
					release
					);

				if (bitmap_ == nullptr)
					throw Error(::GetLastError());

				return *this;
			}
			Bitmap& operator = (Bitmap&& bmp) {
				bitmap_ = std::move(bmp.bitmap_);

				return *this;
			}

			::HBITMAP get() const {
				return *bitmap_;
			}
			::HBITMAP operator ()() const {
				return *bitmap_;
			}

		private:
			static void release(::HBITMAP* bmpPtr) {
				if (bmpPtr != nullptr)
					::DeleteObject(*bmpPtr);
			}

			using ValType = std::unique_ptr<::HBITMAP, decltype(&release)>;
			ValType bitmap_{ nullptr, release };

		};

		//リソースからBitmapを読み込む
		Bitmap makeBitmapBySrc(const TString& instName) {
			return Bitmap(
				static_cast<::HBITMAP>(
					::LoadImage(
						sys::getAppHandle(),
						instName.c_str(),
						IMAGE_BITMAP,
						0,
						0,
						LR_DEFAULTCOLOR | LR_DEFAULTSIZE
						)
					)
				);
		}
		
		enum class MenuOption {
			LineBreak = MFT_MENUBARBREAK,
			Break = MFT_MENUBREAK,
			OwnerDraw = MFT_OWNERDRAW,
			RightJustify = MFT_RIGHTJUSTIFY,
			RightOrder = MFT_RIGHTORDER
		};
		
		enum class MenuState : unsigned int {
			Clear = 0,
			Check = MFS_CHECKED,
			Default = MFS_DEFAULT,
			Disable = MFS_DISABLED,
			Hilite = MFS_HILITE
		};
		
		//デフォルトで用意されているMenu用Bitmap
		enum class MenuBarBitmap : UintPtr {
			Callback = reinterpret_cast<UintPtr>(HBMMENU_CALLBACK),
			Close = reinterpret_cast<UintPtr>(HBMMENU_MBAR_CLOSE),
			DisabledClose = reinterpret_cast<UintPtr>(HBMMENU_MBAR_CLOSE_D),
			Minimize = reinterpret_cast<UintPtr>(HBMMENU_MBAR_MINIMIZE),
			DisabledMinimize = reinterpret_cast<UintPtr>(HBMMENU_MBAR_MINIMIZE_D),
			Restore = reinterpret_cast<UintPtr>(HBMMENU_MBAR_RESTORE),
			System = reinterpret_cast<UintPtr>(HBMMENU_SYSTEM)
		};
		//デフォルトで用意されているPopupMenu用Bitmap
		enum class PopupMenuBitmap : UintPtr {
			Callback = reinterpret_cast<UintPtr>(HBMMENU_CALLBACK),
			Close = reinterpret_cast<UintPtr>(HBMMENU_POPUP_CLOSE),
			Maximize = reinterpret_cast<UintPtr>(HBMMENU_POPUP_MAXIMIZE),
			Minimize = reinterpret_cast<UintPtr>(HBMMENU_POPUP_MINIMIZE),
			Restore = reinterpret_cast<UintPtr>(HBMMENU_POPUP_RESTORE),
			System = reinterpret_cast<UintPtr>(HBMMENU_SYSTEM)
		};

		//MenuBarBitmap, PopupMenuBitmap, Bitmapのいずれかを保持
		class MenuBitmap {
		private:
			//どの型か
			enum class Tag {
				MenuBar,
				PopupMenu,
				Bitmap
			} tag_;

			//保持する型の無名リスト
			union {
				MenuBarBitmap mbb_;
				PopupMenuBitmap pmb_;
				Bitmap bmp_;
			};

			void clean() {
				if (tag_ == Tag::MenuBar)
					delete &mbb_;
				else if (tag_ == Tag::PopupMenu)
					delete &pmb_;
				else if (tag_ == Tag::Bitmap)
					delete &bmp_;
			}

		public:
			MenuBitmap(const MenuBarBitmap& mbb) {
				new (&mbb_) MenuBarBitmap{ mbb };
			}
			MenuBitmap(const PopupMenuBitmap& pmb) {
				new (&pmb_) PopupMenuBitmap{ pmb };
			}
			MenuBitmap(const Bitmap& bmp) {
				new (&bmp_) Bitmap{ bmp };
			}
			MenuBitmap(const MenuBitmap& mbmp) :
				tag_(mbmp.tag_) {
				if (tag_ == Tag::MenuBar)
					new (&mbb_) MenuBarBitmap{ mbmp.mbb_ };
				else if (tag_ == Tag::PopupMenu)
					new (&pmb_) PopupMenuBitmap{ mbmp.pmb_ };
				else if (tag_ == Tag::Bitmap)
					new (&bmp_) Bitmap{ mbmp.bmp_ };
			}

			MenuBitmap& operator = (const MenuBitmap& mbmp) {
				if (mbmp.tag_ == tag_) {
					if (tag_ == Tag::MenuBar)
						mbb_ = mbmp.mbb_;
					else if (tag_ == Tag::PopupMenu)
						pmb_ = mbmp.pmb_;
					else if (tag_ == Tag::Bitmap)
						bmp_ = mbmp.bmp_;
				}
				else {
					clean();

					if (tag_ == Tag::MenuBar)
						new (&mbb_) MenuBarBitmap{ mbmp.mbb_ };
					else if (tag_ == Tag::PopupMenu)
						new (&pmb_) PopupMenuBitmap{ mbmp.pmb_ };
					else if (tag_ == Tag::Bitmap)
						new (&bmp_) Bitmap{ mbmp.bmp_ };
				}
			}

			~MenuBitmap() {
				clean();
			}

			//HBITMAPに変換
			explicit operator ::HBITMAP() const {
				if (tag_ == Tag::MenuBar)
					return reinterpret_cast<::HBITMAP>(mbb_);
				else if (tag_ == Tag::PopupMenu)
					return reinterpret_cast<::HBITMAP>(pmb_);
				else if (tag_ == Tag::Bitmap)
					return bmp_();
			}

		};

		//Menuのアイテムデータ。
		//ユーザーが他の場所で独自にIDを指定する場合、ID: 10000以上は使用不可。
		template<typename T>
		class _impl_MenuItem {
		public:
			_impl_MenuItem() = default;
			_impl_MenuItem(const _impl_MenuItem&) = default;

			_impl_MenuItem(const ::MENUITEMINFO menuItem) {
				info_ = menuItem;
			}

			auto& get() {
				return info_;
			}
			const auto& get() const {
				return info_;
			}
			auto& operator() () {
				return info_;
			}
			const auto& operator() () const {
				return info_;
			}

			int getId() const {
				return id_;
			}

			//idの現在の最大値
			static int _impl_idCap_;

		private:
			::MENUITEMINFO info_ = {};
			int id_ = _impl_idCap_;

		};
		class PopupMenu;
		using MenuItem = _impl_MenuItem<PopupMenu>;
		int MenuItem::_impl_idCap_ = 100000;

		class PopupMenu {
		public:
			PopupMenu() :
				menu_(::CreatePopupMenu()) {
				if (menu_ == nullptr)
					throw Error(::GetLastError());

				id_ = idCap_;
				++idCap_;
			}
			PopupMenu(const PopupMenu& menu) :
				menu_(::CreatePopupMenu()) {

				//TODO: コピー系処理の記述

				if (menu_ == nullptr)
					throw Error(::GetLastError());

				id_ = idCap_;
				++idCap_;
			}

			//メニューを追加
			PopupMenu& add(const MenuItem& newItem) {
				if (!::InsertMenuItem(
					menu_,
					newItem.getId(),
					false,
					&newItem()
					))
					throw Error(::GetLastError());

				return *this;
			}

			auto get() const {
				return menu_;
			}
			auto operator () () const {
				return menu_;
			}

			int getId() const {
				return id_;
			}

		private:
			//idの現在の最大値
			static int idCap_;
			int id_ = idCap_;
			::HMENU menu_ = nullptr;

		};
		int PopupMenu::idCap_ = 10000;

		//Menuの区切り線
		class MenuSeparator {
		public:
			MenuSeparator(const MenuSeparator&) = default;
			MenuSeparator(MenuSeparator&&) = default;
			MenuSeparator& operator = (const MenuSeparator&) = default;
			MenuSeparator& operator = (MenuSeparator&&) = default;

			MenuSeparator(bool drawByOwner = false) {
				ZeroMemory(&info_, sizeof(info_));
				info_.cbSize = sizeof(::MENUITEMINFO);
				info_.fMask |= MIIM_FTYPE;
				info_.fType = MFT_SEPARATOR;
				info_.wID = MenuItem::_impl_idCap_;

				if (drawByOwner)
					info_.fType = MFT_OWNERDRAW;

				++MenuItem::_impl_idCap_;
			}

			operator MenuItem() const {
				return static_cast<MenuItem>(info_);
			}

		private:
			::MENUITEMINFO info_;

		};

		//Menuの文字列
		class MenuString {
		public:
			MenuString(const MenuString&) = default;
			MenuString(MenuString&&) = default;
			MenuString& operator = (const MenuString&) = default;
			MenuString& operator = (MenuString&&) = default;

			MenuString() {
				ZeroMemory(&info_, sizeof(info_));
				info_.cbSize = sizeof(::MENUITEMINFO);

				info_.fMask |= MIIM_STRING;
				info_.wID = MenuItem::_impl_idCap_;
				++MenuItem::_impl_idCap_;
			}
			MenuString(
				const TString& str
				) :
				MenuString(
					&str,
					nullptr,
					nullptr,
					nullptr,
					nullptr,
					nullptr,
					nullptr
					) {}
			MenuString(
				const TString& str,
				const UnifyEnum<MenuOption>& options,
				const UnifyEnum<MenuState>& states
				) :
				MenuString(
					&str,
					&options,
					&states,
					nullptr,
					nullptr,
					nullptr,
					nullptr
					) {}
			MenuString(
				const TString& str,
				const PopupMenu& subMenu
				) :
				MenuString(
					&str,
					nullptr,
					nullptr,
					nullptr,
					nullptr,
					nullptr,
					&subMenu
					) {}
			MenuString(
				const TString& str,
				const UnifyEnum<MenuOption>& options,
				const UnifyEnum<MenuState>& states,
				const PopupMenu& subMenu
				) :
				MenuString(
					&str,
					&options,
					&states,
					nullptr,
					nullptr,
					nullptr,
					&subMenu
					) {}
			MenuString(
				const TString& str,
				const UnifyEnum<MenuOption>& options,
				const UnifyEnum<MenuState>& states,
				const MenuBitmap& menuBmp
				) :
				MenuString(
					&str,
					&options,
					&states,
					&menuBmp,
					nullptr,
					nullptr,
					nullptr
					) {}
			MenuString(
				const TString& str,
				const MenuBitmap& menuBmp,
				const PopupMenu& subMenu
				) :
				MenuString(
					&str,
					nullptr,
					nullptr,
					&menuBmp,
					nullptr,
					nullptr,
					&subMenu
					) {}

			operator MenuItem() {
				return static_cast<MenuItem>(info_);
			}

		private:
			::MENUITEMINFO info_;
			
			//ルートコンストラクタ
			MenuString(
				const TString* str,
				const UnifyEnum<MenuOption>* options,
				const UnifyEnum<MenuState>* states,
				const MenuBitmap* menuBmp,
				const Bitmap* checkMark,
				const Bitmap* uncheckMark,
				const PopupMenu* subMenu
				) {
				info_.cbSize = sizeof(::MENUITEMINFO);

				info_.fMask |= MIIM_STRING;
				info_.wID = MenuItem::_impl_idCap_;

				if (str != nullptr)
					info_.dwTypeData = const_cast<TChar*>(str->c_str());
				if (options != nullptr)
					info_.fMask |= MIIM_FTYPE,
					info_.fType |= options->get();
				if (states != nullptr)
					info_.fMask |= MIIM_STATE,
					info_.fState |= states->get();
				if (menuBmp != nullptr)
					info_.fMask |= MIIM_BITMAP,
					info_.hbmpItem = static_cast<::HBITMAP>(*menuBmp);
				if (checkMark != nullptr)
					info_.fMask |= MIIM_CHECKMARKS,
					info_.hbmpChecked = checkMark->get();
				if (uncheckMark != nullptr)
					info_.fMask |= MIIM_CHECKMARKS,
					info_.hbmpUnchecked = uncheckMark->get();
				if (subMenu != nullptr)
					info_.fMask |= MIIM_SUBMENU,
					info_.hSubMenu = subMenu->get();

				++MenuItem::_impl_idCap_;
			}
		};

		class Menu {
		public:
			Menu(const Menu&) = default;
			Menu(Menu&&) = default;
			Menu& operator = (const Menu&) = default;
			Menu& operator = (Menu&&) = default;

			Menu() :
				menu_(::CreateMenu()) {
				if (menu_ == nullptr)
					throw Error(::GetLastError());
			}
			
			//メニューに要素を追加する
			Menu& add(const TString& str, const PopupMenu& newItem) {
				MenuString child(str, newItem);

				if (!::InsertMenuItem(
					menu_,
					newItem.getId(),
					false,
					&static_cast<MenuItem>(child)()
					))
					throw Error(::GetLastError());

				return *this;
			}

			::HMENU get() const {
				return menu_;
			}
			::HMENU operator () () const {
				return menu_;
			}

		private:
			::HMENU menu_ = nullptr;

		};

	} //::wawl::wnd
} //::wawl