#pragma once

#define WAWL_ENABLE_WINDOW

#include "System.h"
#include "Utility.h"
#include <unordered_map>
#include <functional>

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
				bitmap_ = std::unique_ptr<::HBITMAP, decltype(&release)>(
					new ::HBITMAP(
						static_cast<::HBITMAP>(
							::CopyImage(hbm, IMAGE_BITMAP, 0, 0, LR_COPYRETURNORG)
							)
						),
					release
					);
			}

			Bitmap(const TString& path) {
				bitmap_ = std::unique_ptr<::HBITMAP, decltype(&release)>(
					new ::HBITMAP(
						static_cast<::HBITMAP>(
							::LoadImage(nullptr, path.c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE)
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

		enum class Msg : unsigned int {
			Create = WM_CREATE,
			Timer = WM_TIMER,
			Destroy = WM_DESTROY,
		};

		enum class DefaultIcon : UintPtr {
			App = reinterpret_cast<UintPtr>(IDI_APPLICATION),
			Info = reinterpret_cast<UintPtr>(IDI_INFORMATION),
			Question = reinterpret_cast<UintPtr>(IDI_QUESTION),
			Error = reinterpret_cast<UintPtr>(IDI_ERROR),
			Warning = reinterpret_cast<UintPtr>(IDI_WARNING),
			WinLogo = reinterpret_cast<UintPtr>(IDI_WINLOGO)
		};
		enum class DefaultCursor : UintPtr {
			Arrow = reinterpret_cast<UintPtr>(IDC_ARROW),
			Cross = reinterpret_cast<UintPtr>(IDC_CROSS),
			IBeam = reinterpret_cast<UintPtr>(IDC_IBEAM),
			Wait = reinterpret_cast<UintPtr>(IDC_WAIT)
		};

		enum class Color : int {
			White = WHITE_BRUSH,
			Black = BLACK_BRUSH,
			DarkGray = DKGRAY_BRUSH,
			LightGray = LTGRAY_BRUSH,
			Hollow = HOLLOW_BRUSH
		};

		//���R�[�h
		/*enum class MenuType : unsigned int {
			UseBitmap = MFT_BITMAP,
			LineBreak = MFT_MENUBARBREAK,
			Break = MFT_MENUBREAK,
			OwnerDraw = MFT_OWNERDRAW,
			RadioButton = MFT_RADIOCHECK,
			RightJustify = MFT_RIGHTJUSTIFY,
			RightOrder = MFT_RIGHTORDER,
			Separator = MFT_SEPARATOR
		};
		using UnifyMenuType = _impl_UnifyEnum<MenuType>;*/

		enum class MenuOption {
			LineBreak = MFT_MENUBARBREAK,
			Break = MFT_MENUBREAK,
			OwnerDraw = MFT_OWNERDRAW,
			RightJustify = MFT_RIGHTJUSTIFY,
			RightOrder = MFT_RIGHTORDER
		};
		using UnifyMenuOption = _impl_UnifyEnum<MenuOption>;

		enum class MenuState : unsigned int {
			Clear = NULL,
			Check = MFS_CHECKED,
			Default = MFS_DEFAULT,
			Disable = MFS_DISABLED,
			Hilite = MFS_HILITE
		};
		using UnifyMenuState = _impl_UnifyEnum<MenuState>;

		enum class MenuBarBitmap : UintPtr {
			Callback = reinterpret_cast<UintPtr>(HBMMENU_CALLBACK),
			Close = reinterpret_cast<UintPtr>(HBMMENU_MBAR_CLOSE),
			DisabledClose = reinterpret_cast<UintPtr>(HBMMENU_MBAR_CLOSE_D),
			Minimize = reinterpret_cast<UintPtr>(HBMMENU_MBAR_MINIMIZE),
			DisabledMinimize = reinterpret_cast<UintPtr>(HBMMENU_MBAR_MINIMIZE_D),
			Restore = reinterpret_cast<UintPtr>(HBMMENU_MBAR_RESTORE),
			System = reinterpret_cast<UintPtr>(HBMMENU_SYSTEM)
		};
		enum class PopupMenuBitmap : UintPtr {
			Callback = reinterpret_cast<UintPtr>(HBMMENU_CALLBACK),
			Close = reinterpret_cast<UintPtr>(HBMMENU_POPUP_CLOSE),
			Maximize = reinterpret_cast<UintPtr>(HBMMENU_POPUP_MAXIMIZE),
			Minimize = reinterpret_cast<UintPtr>(HBMMENU_POPUP_MINIMIZE),
			Restore = reinterpret_cast<UintPtr>(HBMMENU_POPUP_RESTORE),
			System = reinterpret_cast<UintPtr>(HBMMENU_SYSTEM)
		};

		//MenuBarBitmap, PopupMenuBitmap, Bitmap�̂����ꂩ��ێ�
		class MenuBitmap {
		private:
			//�ǂ̌^��
			enum class Tag {
				MenuBar,
				PopupMenu,
				Bitmap
			} tag_;
			
			//�ێ�����^�̖������X�g
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

			//HBITMAP�ɕϊ�
			explicit operator ::HBITMAP() const {
				if (tag_ == Tag::MenuBar)
					return reinterpret_cast<::HBITMAP>(mbb_);
				else if (tag_ == Tag::PopupMenu)
					return reinterpret_cast<::HBITMAP>(pmb_);
				else if (tag_ == Tag::Bitmap)
					return bmp_();
			}

		};

		//Menu�̃A�C�e���f�[�^�B���[�U�[�����̏ꏊ�œƎ���ID���w�肷��ꍇ�AID: 10000�ȏ�͎g�p�s�B
		template<typename T>
		class _impl_MenuItem {
		public:
			_impl_MenuItem() = default;
			_impl_MenuItem(const _impl_MenuItem&) = default;

			//���R�[�h
			/*_impl_MenuItem(
				const UnifyMenuType& option,
				const UnifyMenuState& states,
				const T& subMenu,
				const Bitmap& checkMark,
				const Bitmap& uncheckMark,
				const TString& str,
				const MenuBitmap& menuBmp
				) :
				_impl_MenuItem(
					&option,
					&states,
					&subMenu,
					&checkMark,
					&uncheckMark,
					&str,
					&menuBmp
					) {}*/

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

			//id�̌��݂̍ő�l
			static int _impl_idCap_;

		private:
			::MENUITEMINFO info_ = {};
			int id_ = _impl_idCap_;

			//���R�[�h
			/*_impl_MenuItem(
				const UnifyMenuType* option,
				const UnifyMenuState* states,
				const T* subMenu,
				const Bitmap* checkMark,
				const Bitmap* uncheckMark,
				const TString* str,
				const MenuBitmap* menuBmp
				) {
				::ZeroMemory(&info_, sizeof(info_));
				info_.cbSize = sizeof(::MENUITEMINFO);
				info_.fMask |= MIIM_ID;
				info_.wID = id_;

				if (option != nullptr)
					info_.fMask |= MIIM_FTYPE,
					info_.fType = option->get();
				if (states != nullptr)
					info_.fMask |= MIIM_STATE,
					info_.fState = states->get();
				if (subMenu != nullptr)
					info_.fMask |= MIIM_SUBMENU,
					info_.hSubMenu = subMenu->get();
				if (checkMark != nullptr)
					info_.fMask |= MIIM_CHECKMARKS,
					info_.hbmpChecked = checkMark->get();
				if (uncheckMark != nullptr)
					info_.fMask |= MIIM_CHECKMARKS,
					info_.hbmpUnchecked = uncheckMark->get();
				if (str != nullptr)
					info_.fMask |= MIIM_STRING,
					info_.dwTypeData = &str->at(0),
					info_.cch = static_cast<unsigned int>(str->size() + 1);
				if (menuBmp != nullptr)
					info_.fMask |= MIIM_BITMAP,
					info_.hbmpItem = static_cast<::HBITMAP>(*menuBmp);

				++_impl_idCap_;
			}*/

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

				//TODO: �R�s�[�n�����̋L�q

				if (menu_ == nullptr)
					throw Error(::GetLastError());

				id_ = idCap_;
				++idCap_;
			}

			//���j���[��ǉ�
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
			//id�̌��݂̍ő�l
			static int idCap_;
			int id_ = idCap_;
			::HMENU menu_ = nullptr;

		};
		int PopupMenu::idCap_ = 10000;

		//Menu�̋�؂��
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
			static ::MENUITEMINFO info_;

		};
		::MENUITEMINFO MenuSeparator::info_;

		//Menu�̕�����
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
				const UnifyMenuOption& options,
				const UnifyMenuState& states
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
				){}
			MenuString(
				const TString& str,
				const UnifyMenuOption& options,
				const UnifyMenuState& states,
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
				const UnifyMenuOption& options,
				const UnifyMenuState& states,
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

			MenuString(
				const TString* str,
				const UnifyMenuOption* options,
				const UnifyMenuState* states,
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

		//TODO : MENU�̃��b�p���
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

		private:
			::HMENU menu_ = nullptr;

		};

		enum class PropOption : unsigned int {
			AlignClientByByte = CS_BYTEALIGNCLIENT,
			AlignWndByByte = CS_BYTEALIGNWINDOW,
			RedrawWidth = CS_HREDRAW,
			RedrawHeight = CS_VREDRAW,
			CallOnDoubleClick = CS_DBLCLKS,
			NoClose = CS_NOCLOSE,
			ShareDC = CS_CLASSDC,
			OriginalDC = CS_OWNDC,
			ParentDC = CS_PARENTDC,
			Global = CS_GLOBALCLASS,
			SaveBitmap = CS_SAVEBITS,
			DropShadow = CS_DROPSHADOW
		};
		using UnifyPropOption = _impl_UnifyEnum<PropOption>;

		//Window�v���V�[�W���̐錾
		::LRESULT CALLBACK _impl_MsgProc(::HWND hwnd, unsigned int uMsg, ::WPARAM wParam, ::LPARAM lParam);

		class Prop {
		public:
			Prop() {
				::WNDCLASSEX wndClass;

				ZeroMemory(&wndClass, sizeof(wndClass));
				wndClass.cbSize = sizeof(::WNDCLASSEX);
				wndClass.style = CS_HREDRAW | CS_VREDRAW;
				wndClass.lpfnWndProc = _impl_MsgProc;
				wndClass.hInstance = sys::getAppHandle();
				wndClass.hIcon = ::LoadIcon(nullptr, IDI_APPLICATION);
				wndClass.hIconSm = ::LoadIcon(nullptr, IDI_APPLICATION);
				wndClass.hCursor = ::LoadCursor(nullptr, IDC_ARROW);
				wndClass.hbrBackground = reinterpret_cast<::HBRUSH>(::GetStockObject(NULL_BRUSH));
				wndClass.lpszMenuName = nullptr;
				wndClass.lpszClassName = util::valToTStr(idCap_).c_str();

				if (atom_ = ::RegisterClassEx(&wndClass), atom_ == false)
					throw Error(::GetLastError());

				id_ = idCap_;
				++idCap_;
			}
			//TODO : �R���X�g���N�^�̎�ޒǉ�

			TString getName() const {
				return util::valToTStr(id_);
			}
			Word getId() const {
				return atom_;
			}

		private:
			static int idCap_;
			int id_;
			Word atom_;

			//TODO: �����R���X�g���N�^�̒ǉ�

		};
		int Prop::idCap_ = 0;

		enum class ExtStyle : Dword {
			
		};

		class Window {
		public:
			// Window�v���V�[�W���̎���
			friend ::LRESULT CALLBACK _impl_MsgProc(::HWND hwnd, unsigned int msg, ::WPARAM wParam, ::LPARAM lParam) {
				if (Window::winRefs_.find(hwnd) == Window::winRefs_.end())
					return 1;
				else {
					auto& wnd = *Window::winRefs_[hwnd];

					if (wnd.msgProcs_.find(msg) == wnd.msgProcs_.end())
						return ::DefWindowProc(hwnd, msg, wParam, lParam);
					else
						return wnd.msgProcs_[msg](wParam, lParam);
				}

				return 0;
			}

			Window() {
				::HWND hwnd = nullptr;

				winRefs_.insert(std::make_pair(hwnd, this));
			}

			Window& addMsgProc(const Msg msg, const std::function<int(UintPtr, IntPtr)>& proc) {
				msgProcs_.insert(std::make_pair(util::unpackEnum(msg), proc));

				return *this;
			}
			Window& removeMsgProc(const Msg msg) {
				msgProcs_.erase(util::unpackEnum(msg));

				return *this;
			}

			Position toScreenPos(const Position& clientPos) {
				LPPOINT posBuf;
				posBuf->x = clientPos.x;
				posBuf->y = clientPos.y;

				if (!::ClientToScreen(hwnd_, posBuf))
					throw Error(::GetLastError());

				return Position(posBuf->x, posBuf->y);
			}
			Position toClientPos(const Position& screenPos) {
				LPPOINT posBuf;
				posBuf->x = screenPos.x;
				posBuf->y = screenPos.y;

				if (!::ClientToScreen(hwnd_, posBuf))
					throw Error(::GetLastError());

				return Position(posBuf->x, posBuf->y);
			}

		private:
			//���g�ւ̎Q�Ƃ�ێ�
			static std::unordered_map<::HWND, Window*> winRefs_;
			//���b�Z�[�W�v���V�[�W��
			std::unordered_map<unsigned int, std::function<int(UintPtr, IntPtr)>> msgProcs_;
			::HWND hwnd_;

		};
		std::unordered_map<::HWND, Window*> Window::winRefs_;

		namespace mb {
			//showMessage�ŕ\������{�^��
			enum class Button : unsigned int {
				Failed = MB_ABORTRETRYIGNORE,
				Help = MB_HELP,
				OK = MB_OK,
				IsOK = MB_OKCANCEL,
				DoRetry = MB_RETRYCANCEL,
				YesNo = MB_YESNO,
				YesNoCancel = MB_YESNOCANCEL
			};
			using UnifyButton = _impl_UnifyEnum<Button>;
			//showMessage�ŕ\������A�C�R��
			enum class Icon : unsigned int {
				Warning = MB_ICONWARNING,
				Info = MB_ICONINFORMATION,
				Question = MB_ICONQUESTION,
				Stop = MB_ICONSTOP
			};
			using UnifyIcon = _impl_UnifyEnum<Icon>;
			//showMessage�\�����̑���Window����K��
			enum class CtrlRegLevel : unsigned int {
				App = MB_APPLMODAL,
				System = MB_SYSTEMMODAL,
				Task = MB_TASKMODAL
			};
			using UnifyCtrlRegLevel = _impl_UnifyEnum<CtrlRegLevel>;
			//showMessage�̕\���`��
			enum class Style : unsigned int {
				OnlyDefaultDesktop = MB_DEFAULT_DESKTOP_ONLY,
				RightText = MB_RIGHT,
				RightToLeft = MB_RTLREADING,
				Foreground = MB_SETFOREGROUND,
				TopMost = MB_TOPMOST,
				Notification = MB_SERVICE_NOTIFICATION,
				NotificationOnNT351 = MB_SERVICE_NOTIFICATION_NT3X
			};
			using UnifyStyle = _impl_UnifyEnum<Style>;
			//showMessage�̕Ԃ�l
			enum class Result : int {
				Abort = IDABORT,
				Cancel = IDCANCEL,
				Continue = IDCONTINUE,
				Ignore = IDIGNORE,
				No = IDNO,
				OK = IDOK,
				Retry = IDRETRY,
				TryAgain = IDTRYAGAIN,
				Yes = IDYES
			};

			Result show(const TString& title, const TString& text, const UnifyButton& button) {
				return static_cast<Result>(
					::MessageBox(
						nullptr,
						text.c_str(),
						title.c_str(),
						button.get()
						)
					);
			}

			//TODO : showMessage�̃I�[�o�[���[�h�ǉ�
		} //::wawl::wnd::mb
	} //::wawl::wnd
} //::wawl