#pragma once

#include "System.h"
#include "Utility.h"
#include <unordered_map>
#include <functional>

namespace wawl {
	namespace wnd {

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

		//Windowプロシージャの宣言
		::LRESULT CALLBACK _impl_MsgProc(::HWND hwnd, unsigned int uMsg, ::WPARAM wParam, ::LPARAM lParam);

		//ToDo : MENUのラッパ作る

		//ちょっと書いてみた by yuki74w
		//MENUはCreateMenuでHMENU型の変数を作り、
		//InsertMenuItemでMENUをどんどん挿入していく。

		enum class MenuSetting {
			Bitmap=MIIM_BITMAP,
			CheckMarks=MIIM_CHECKMARKS,
			Data=MIIM_DATA,
			FType=MIIM_FTYPE,
			ID=MIIM_ID,
			State=MIIM_STATE,
			String=MIIM_STRING,
			SubMenu=MIIM_SUBMENU,
			Type=MIIM_TYPE,
		};
		using UnifyMenuSetting = _impl_UnifyEnum<MenuSetting>;

		enum class MenuType {
			Bitmap=MFT_BITMAP,
			MenuBarBreak=MFT_MENUBARBREAK,
			MenuBreak=MFT_MENUBREAK,
			OwnerDraw=MFT_OWNERDRAW,
			RadioCheck=MFT_RADIOCHECK,
			RightJustify=MFT_RIGHTJUSTIFY,
			RightOrder=MFT_RIGHTORDER,
			Separator=MFT_SEPARATOR,
			String=MFT_STRING,
		};
		using UnifyMenuType = _impl_UnifyEnum<MenuType>;

		enum class MenuState {
			Checked = MFS_CHECKED,
			Default = MFS_DEFAULT,
			DisAbled = MFS_DISABLED,
			Enabled = MFS_ENABLED,
			Grayed = MFS_GRAYED,
			HighLight = MFS_HILITE,
			UnChecked=MFS_UNCHECKED,
			UnHighLight=MFS_UNHILITE,
		};
		using UnifyMenuState = _impl_UnifyEnum<MenuState>;

		class SubMenu {
		public:
			SubMenu() {
				Entity = CreatePopupMenu();
			}

			auto& get() {
				return Entity;
			}
			const auto& get() const {
				return Entity;
			}
			auto& operator()() {
				return Entity;
			}
			const auto& operator()() const {
				return Entity;
			}

		private:
			::HMENU Entity;
		};

		class MenuItemInformation {
		public:
			///とりあえずよく使いそうなものしか記述してない

			///<summary>文字が表示されるだけのメニューアイテム</summary>
			MenuItemInformation(std::string Text) {
				Entity.cbSize = sizeof(MENUITEMINFO);
				Entity.fMask = static_cast<UINT>(MenuSetting::Type);
				Entity.fType = static_cast<UINT>(MenuType::String);
			}

			///<summary>サブメニュー持ち</summary>
			MenuItemInformation(std::string Text,SubMenu ItemSubMenu) {
				Entity.cbSize = sizeof(MENUITEMINFO);
				Entity.fMask = UnifyMenuSetting({ static_cast<UINT>(MenuSetting::SubMenu),static_cast<UINT>(MenuSetting::Type) }).get();
				Entity.fType = static_cast<UINT>(MenuType::String);
				Entity.hSubMenu = ItemSubMenu.get();
			}

			///<summary>区切り線</summary>
			MenuItemInformation() {
				Entity.cbSize = sizeof(MENUITEMINFO);
				Entity.fMask = static_cast<UINT>(MenuSetting::Type);
				Entity.fType = static_cast<UINT>(MenuType::Separator);
			}




			auto& get() {
				return Entity;
			}
			const auto& get() const{
				return Entity;
			}
			auto& operator()() {
				return Entity;
			}
			const auto& operator()() const{
				return Entity;
			}

		private:
			MENUITEMINFO Entity;
		};

		class Menu {
		public:
			Menu() {
				hmenu_ = CreateMenu();
			}

			void Insert(MenuItemInformation ItemInfo,unsigned int position) {
				InsertMenuItem(hmenu_, position, TRUE, &ItemInfo());
			}


		private:
			//自身への参照を保持
			static std::unordered_map<::HMENU, Menu*> menuRefs_;
			::HMENU hmenu_;

		};


		//ここまで
		


		class Prop {
		public:
			Prop() {
				ZeroMemory(&wndClass_, sizeof(wndClass_));
				wndClass_.cbSize = sizeof(::WNDCLASSEX);
				wndClass_.style = CS_HREDRAW | CS_VREDRAW;
				wndClass_.lpfnWndProc = _impl_MsgProc;
				wndClass_.hInstance = sys::getAppHandle();
				wndClass_.hIcon = ::LoadIcon(nullptr, IDI_APPLICATION);
				wndClass_.hIconSm = ::LoadIcon(nullptr, IDI_APPLICATION);
				wndClass_.hCursor = ::LoadCursor(nullptr, IDC_ARROW);
				wndClass_.hbrBackground = reinterpret_cast<::HBRUSH>(::GetStockObject(NULL_BRUSH));
				wndClass_.lpszMenuName = nullptr;
				wndClass_.lpszClassName = util::valToTStr(classNum_).c_str();

				if (id_ = ::RegisterClassEx(&wndClass_), id_ == false)
					throw std::runtime_error{"Failed to RegisterClassEx"};
			}

			auto& get() {
				return wndClass_;
			}
			const auto& get() const {
				return wndClass_;
			}
			auto& operator () () {
				return wndClass_;
			}
			const auto& operator () () const {
				return wndClass_;
			}

		private:
			static int classNum_;
			Word id_;
			::WNDCLASSEX wndClass_;

		};
		int Prop::classNum_ = 0;

		class Window {
		public:
			// Windowプロシージャの実態
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

		private:
			//自身への参照を保持
			static std::unordered_map<::HWND, Window*> winRefs_;
			//メッセージプロシージャ
			std::unordered_map<::UINT, std::function<int(UintPtr, IntPtr)>> msgProcs_;
			::HWND hwnd_;

		};
		std::unordered_map<::HWND, Window*> Window::winRefs_;

		//showMessageで表示するボタン
		enum class ChoiceButton : unsigned int {
			Failed = MB_ABORTRETRYIGNORE,
			Help = MB_HELP,
			OK = MB_OK,
			IsOK = MB_OKCANCEL,
			DoRetry = MB_RETRYCANCEL,
			YesNo = MB_YESNO,
			YesNoCancel = MB_YESNOCANCEL
		};
		using UnifyChoiceButton = _impl_UnifyEnum<ChoiceButton>;
		//showMessageで表示するアイコン
		enum class ShowIcon : unsigned int {
			Warning = MB_ICONWARNING,
			Info = MB_ICONINFORMATION,
			Question = MB_ICONQUESTION,
			Stop = MB_ICONSTOP
		};
		using UnifyShowIcon = _impl_UnifyEnum<ShowIcon>;
		//showMessage表示中の他のWindow操作規制
		enum class CtrlRegLevel : unsigned int {
			App = MB_APPLMODAL,
			System = MB_SYSTEMMODAL,
			Task = MB_TASKMODAL
		};
		using UnifyCtrlRegLevel = _impl_UnifyEnum<CtrlRegLevel>;
		//showMessageの表示形式
		enum class MBShowStyle : unsigned int {
			OnlyDefaultDesktop = MB_DEFAULT_DESKTOP_ONLY,
			RightText = MB_RIGHT,
			RightToLeft = MB_RTLREADING,
			Foreground = MB_SETFOREGROUND,
			TopMost = MB_TOPMOST,
			Notification = MB_SERVICE_NOTIFICATION,
			NotificationOnNT351 = MB_SERVICE_NOTIFICATION_NT3X
		};
		using UnifyMBShowStyle = _impl_UnifyEnum<MBShowStyle>;
		//showMessageの返り値
		enum class MBReturn : int {
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

		MBReturn showMessage(const TString& title, const TString& text, const UnifyChoiceButton& button) {
			return static_cast<MBReturn>(
				::MessageBox(
					nullptr,
					text.c_str(),
					title.c_str(),
					button.get()
					)
				);
		}

	}
}