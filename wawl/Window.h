#pragma once

#include "System.h"
#include "Utility.h"
#include <unordered_map>
#include <functional>

namespace wawl {
	namespace wnd {

		//test code
		void ReadImageByInstance(const TString& instName) {
			static_cast<::HBITMAP>(::LoadImage(sys::getAppHandle(), instName.c_str(), IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR | LR_DEFAULTSIZE));
		}
		//~test code

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

		enum class MenuOption : unsigned int {
			UseBitmap = MFT_BITMAP,
			LineBreak = MFT_MENUBARBREAK,
			Break = MFT_MENUBREAK,
			OwnerDraw = MFT_OWNERDRAW,
			RadioButton = MFT_RADIOCHECK,
			RightJustify = MFT_RIGHTJUSTIFY,
			RightOrder = MFT_RIGHTORDER,
			SectionLine = MFT_SEPARATOR
		};
		using UnifyMenuOption = _impl_UnifyEnum<MenuOption>;

		enum class MenuType : unsigned int {
			Check = MFS_CHECKED,
			Uncheck = MFS_UNCHECKED,
			Default = MFS_DEFAULT,
			Disable = MFS_DISABLED,
			Enable = MFS_ENABLED,
			Hilite = MFS_HILITE,
			Unhilite = MFS_UNHILITE
		};
		using UnifyMenuType = _impl_UnifyEnum<MenuType>;

		class MenuInfo {
		public:
			MenuInfo(
				const UnifyMenuOption* option,
				const UnifyMenuType* type
				) {
				::ZeroMemory(&info_, sizeof(info_));
				info_.cbSize = sizeof(::MENUITEMINFO);
				info_.fMask |= MIIM_ID;
				info_.wID = id_;

				if (option != nullptr)
					info_.fMask |= MIIM_FTYPE,
					info_.fType = option->get();
				if (type != nullptr)
					info_.fMask |= MIIM_STATE,
					info_.fState = type->get();
				//nullptrでなければmask追加して設定

				++idCap_;
			}

			::MENUITEMINFO& get() {
				return info_;
			}
			const ::MENUITEMINFO& get() const {
				return info_;
			}
			::MENUITEMINFO& operator() () {
				return info_;
			}
			const ::MENUITEMINFO& operator() () const {
				return info_;
			}

			int getId() const {
				return id_;
			}

		private:
			static int idCap_;
			::MENUITEMINFO info_;
			int id_ = idCap_;

		};
		int MenuInfo::idCap_ = 0;

		//ToDo : MENUのラッパ作る
		class Menu {
		public:
			Menu(const Menu&) = default;
			Menu(Menu&&) = default;
			Menu& operator = (const Menu&) = default;
			Menu& operator = (Menu&&) = default;

			Menu() :
				menu_(::CreateMenu()) {
				if (menu_ == nullptr)
					throw std::runtime_error("Failed to CreateMenu");
			}

			Menu& add(const Menu& child) {
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

		//Windowプロシージャの宣言
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
					throw std::runtime_error{"Failed to RegisterClassEx. Error Code: " + std::to_string(::GetLastError())};

				id_ = idCap_;
				++idCap_;
			}
			//ToDo : コンストラクタの種類追加

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

		};
		int Prop::idCap_ = 0;

		enum class ExtStyle : Dword {
			
		};

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

			Window() {
				::HWND hwnd = nullptr;

				

				winRefs_.insert(std::pair<::HWND, Window*>{hwnd, this});
			}

			Window& addMsgProc(const Msg msg, const std::function<int(UintPtr, IntPtr)>& proc) {
				msgProcs_.insert(std::pair<::UINT, std::function<int(UintPtr, IntPtr)>>{util::unpackEnum(msg), proc});

				return *this;
			}
			Window& removeMsgProc(const Msg msg) {
				msgProcs_.erase(util::unpackEnum(msg));

				return *this;
			}

		private:
			//自身への参照を保持
			static std::unordered_map<::HWND, Window*> winRefs_;
			//メッセージプロシージャ
			std::unordered_map<unsigned int, std::function<int(UintPtr, IntPtr)>> msgProcs_;
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

		//ToDo : showMessageのオーバーロード追加

	}
}