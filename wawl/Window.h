#pragma once

#define WAWL_ENABLE_WINDOW

//wawl Header
#include "System.h"
#include "Menu.h"
#include "Utility.h"
//C++ STL
#include <unordered_map>
#include <functional>

namespace wawl {
	namespace wnd {

		enum class PropOption : unsigned int {
			AlignClientByByte = CS_BYTEALIGNCLIENT,
			AlignWndByByte = CS_BYTEALIGNWINDOW,
			VRedraw = CS_HREDRAW,
			HRedraw = CS_VREDRAW,
			CallOnDoubleClick = CS_DBLCLKS,
			NoClose = CS_NOCLOSE,
			ShareDC = CS_CLASSDC,
			OriginalDC = CS_OWNDC,
			ParentDC = CS_PARENTDC,
			Global = CS_GLOBALCLASS,
			SaveBitmap = CS_SAVEBITS,
			DropShadow = CS_DROPSHADOW
		};

		enum class DefaultIcon : UintPtr {
			App = reinterpret_cast<UintPtr>(IDI_APPLICATION),
			Info = reinterpret_cast<UintPtr>(IDI_INFORMATION),
			Question = reinterpret_cast<UintPtr>(IDI_QUESTION),
			Error = reinterpret_cast<UintPtr>(IDI_ERROR),
			Warning = reinterpret_cast<UintPtr>(IDI_WARNING),
			WinLogo = reinterpret_cast<UintPtr>(IDI_WINLOGO)
		};

		class Icon {
		public:
			Icon() = default;
			Icon(const Icon& icon) {
				icon_ = ValType(
					new ::HICON(
						::CopyIcon(icon())
						),
					release
					);

				if (icon_ == nullptr)
					throw Error(::GetLastError());
			}
			Icon(Icon&& icon) {
				icon_ = std::move(icon.icon_);
			}
			Icon(::HICON hIcon) {
				icon_ = ValType(
					new ::HICON(
						::CopyIcon(hIcon)
						),
					release
					);

				if (icon_ == nullptr)
					throw Error(::GetLastError());
			}

			Icon(DefaultIcon various) {
				icon_ = ValType(
					new ::HICON(
						::LoadIcon(nullptr, reinterpret_cast<TChar*>(various))
						),
						release
					);

				if (icon_ == nullptr)
					throw Error(::GetLastError());
			}
			Icon(const TString& path) {
				icon_ = ValType(
					new ::HICON(
						static_cast<::HICON>(
							::LoadImage(
								nullptr,
								path.c_str(),
								IMAGE_ICON,
								0,
								0,
								LR_DEFAULTSIZE | LR_LOADFROMFILE
								)
							)
						),
					release
					);

				if (icon_ == nullptr)
					throw Error(::GetLastError());
			}

			Icon& operator = (const Icon& icon) {
				icon_ = ValType(
					new ::HICON(
						::CopyIcon(icon())
						),
					release
					);

				if (icon_ == nullptr)
					throw Error(::GetLastError());

				return *this;
			}
			Icon& operator = (Icon&& icon) {
				icon_ = std::move(icon.icon_);

				return *this;
			}

			::HICON get() const {
				return *icon_;
			}
			::HICON operator ()() const {
				return *icon_;
			}

		private:
			static void release(::HICON* iconPtr) {
				if (iconPtr != nullptr)
					::DestroyIcon(*iconPtr);
			}

			using ValType = std::unique_ptr<::HICON, decltype(&release)>;
			ValType icon_{ nullptr, release };

		};

		Icon makeIconBySrc(const TString& instName) {
			return Icon(
				static_cast<::HICON>(
					::LoadImage(
						sys::getAppHandle(),
						instName.c_str(),
						IMAGE_ICON,
						0,
						0,
						LR_SHARED
						)
					)
				);
		}

		enum class DefaultCursor : UintPtr {
			Arrow = reinterpret_cast<UintPtr>(IDC_ARROW),
			Cross = reinterpret_cast<UintPtr>(IDC_CROSS),
			IBeam = reinterpret_cast<UintPtr>(IDC_IBEAM),
			Wait = reinterpret_cast<UintPtr>(IDC_WAIT)
		};

		class Cursor {
		public:
			Cursor() = default;
			Cursor(const Cursor& cursor) {
				cursor_ = ValType(
					new ::HCURSOR(
						CopyCursor(cursor())
						),
					release
					);

				if (cursor_ == nullptr)
					throw Error(::GetLastError());
			}
			Cursor(Cursor&& cursor) {
				cursor_ = std::move(cursor.cursor_);
			}
			Cursor(::HCURSOR hCursor) {
				cursor_ = ValType(
					new ::HCURSOR(
						CopyCursor(hCursor)
						),
					release
					);

				if (cursor_ == nullptr)
					throw Error(::GetLastError());
			}

			Cursor(DefaultCursor various) {
				cursor_ = ValType(
					new ::HCURSOR(
						::LoadCursor(nullptr, reinterpret_cast<TChar*>(various))
						),
					release
					);

				if (cursor_ == nullptr)
					throw Error(::GetLastError());
			}
			Cursor(const TString& path) {
				cursor_ = ValType(
					new ::HCURSOR(
						static_cast<::HCURSOR>(
							::LoadImage(
								nullptr,
								path.c_str(),
								IMAGE_CURSOR,
								0,
								0,
								LR_DEFAULTSIZE | LR_LOADFROMFILE
								)
							)
						),
					release
					);

				if (cursor_ == nullptr)
					throw Error(::GetLastError());
			}

			Cursor& operator = (const Cursor& cursor) {
				cursor_ = ValType(
					new ::HCURSOR(
						CopyCursor(cursor())
						),
					release
					);

				if (cursor_ == nullptr)
					throw Error(::GetLastError());

				return *this;
			}
			Cursor& operator = (Cursor&& cursor) {
				cursor_ = std::move(cursor.cursor_);

				return *this;
			}

			::HCURSOR get() const {
				return *cursor_;
			}
			::HCURSOR operator ()() const {
				return *cursor_;
			}

		private:
			static void release(::HCURSOR* cursorPtr) {
				if (cursorPtr != nullptr)
					::DestroyIcon(*cursorPtr);
			}

			using ValType = std::unique_ptr<::HCURSOR, decltype(&release)>;
			ValType cursor_{ nullptr, release };

		};

		Cursor makeCursorBySrc(const TString& instName) {
			return Cursor(
				::LoadCursor(
					sys::getAppHandle(),
					instName.c_str()
					)
				);
		}

		enum class Color : int {
			White = WHITE_BRUSH,
			Black = BLACK_BRUSH,
			DarkGray = DKGRAY_BRUSH,
			LightGray = LTGRAY_BRUSH,
			Hollow = HOLLOW_BRUSH
		};

		enum class Msg : unsigned int {
			Create = WM_CREATE,
			Timer = WM_TIMER,
			Destroy = WM_DESTROY,
			LClick = WM_LBUTTONDOWN,
			RClick = WM_RBUTTONDOWN
		};

		/*enum class DefaultProp : UintPtr {
			Button = reinterpret_cast<UintPtr>(TEXT("BUTTON")),
			ComboBox = reinterpret_cast<UintPtr>(TEXT("COMBOBOX")),
			Edit = reinterpret_cast<UintPtr>(TEXT("EDIT")),
			ListBox = reinterpret_cast<UintPtr>(TEXT("LISTBOX")),
			MdiClient = reinterpret_cast<UintPtr>(TEXT("MDICLIENT")),
			RichEditV1 = reinterpret_cast<UintPtr>(TEXT("RichEdit")),
			RichEditV2 = reinterpret_cast<UintPtr>(TEXT("RICHEDIT_CLASS")),
			Rect = reinterpret_cast<UintPtr>(TEXT("RECT")),
		};*/

		//Windowプロシージャの宣言
		::LRESULT CALLBACK _impl_MsgProc(::HWND hwnd, unsigned int uMsg, ::WPARAM wParam, ::LPARAM lParam);

		class Prop {
		public:
			Prop(const Prop&) = default;
			Prop& operator = (const Prop&) = default;
			Prop(Prop&&) = default;
			Prop& operator = (Prop&&) = default;

			Prop() :
				Prop(
					{PropOption::HRedraw, PropOption::VRedraw},
					DefaultIcon::App,
					DefaultIcon::App,
					DefaultCursor::Arrow,
					Color::Hollow
					) {}
			Prop(
				const TString& menuName
				) :
				Prop(
					{ PropOption::HRedraw, PropOption::VRedraw },
					DefaultIcon::App,
					DefaultIcon::App,
					DefaultCursor::Arrow,
					Color::Hollow,
					menuName
					) {}
			Prop(
				const UnifyEnum<PropOption>& options
				) :
				Prop(
					options,
					DefaultIcon::App,
					DefaultIcon::App,
					DefaultCursor::Arrow,
					Color::Hollow
					) {}
			Prop(
				const UnifyEnum<PropOption>& options,
				const Icon& icon,
				const Icon& smallIcon,
				const Cursor& cursor,
				const Color& backColor
				) :
				Prop(
					&options,
					&icon,
					&smallIcon,
					&cursor,
					&backColor,
					nullptr
					) {}
			Prop(
				const UnifyEnum<PropOption>& options,
				const Icon& icon,
				const Icon& smallIcon,
				const Cursor& cursor,
				const Color& backColor,
				const TString& menuName
				) :
				Prop(
					&options,
					&icon,
					&smallIcon,
					&cursor,
					&backColor,
					&menuName
					) {}

			//TODO: コンストラクタ追加

			TString getName() const {
				return util::valToTStr(id_);
			}
			Word getId() const {
				return atom_;
			}

		private:
			static int idCap_;
			int id_ = 0;
			Word atom_ = 0;

			//ルートコンストラクタ
			Prop(
				const UnifyEnum<PropOption>* options,
				const Icon* icon,
				const Icon* smallIcon,
				const Cursor* cursor,
				const Color* backColor,
				const TString* menuName
				) {
				::WNDCLASSEX wndClass;
				auto className = util::valToTStr(idCap_);

				ZeroMemory(&wndClass, sizeof(wndClass));
				wndClass.cbSize = sizeof(::WNDCLASSEX);
				wndClass.lpfnWndProc = _impl_MsgProc;
				wndClass.hInstance = sys::getAppHandle();
				wndClass.lpszClassName = className.c_str();

				if (options != nullptr)
					wndClass.style = options->get();
				if (icon != nullptr)
					wndClass.hIcon = icon->get();
				if (smallIcon != nullptr)
					wndClass.hIconSm = smallIcon->get();
				if (backColor != nullptr)
					wndClass.hbrBackground = reinterpret_cast<::HBRUSH>(::GetStockObject(util::unpackEnum(*backColor)));
				if (menuName != nullptr)
					wndClass.lpszMenuName = menuName->c_str();

				atom_ = ::RegisterClassEx(&wndClass);
				if (atom_ == false)
					throw Error(::GetLastError());

				id_ = idCap_;
				++idCap_;
			}

		};
		int Prop::idCap_ = 1;

		//Windowスタイル
		enum class Style : Dword {
			Bordered = WS_BORDER,
			Caption = WS_CAPTION,
			Child = WS_CHILD,
			NotDrawChildrenRegion = WS_CLIPCHILDREN,
			NotDrawBroserRegion = WS_CLIPSIBLINGS,
			Disabled = WS_DISABLED,
			ControlGroup = WS_GROUP,
			UseHorizontalScroll = WS_HSCROLL,
			UseVerticalScroll = WS_VSCROLL,
			Maxmize = WS_MAXIMIZE,
			EnableMaximize = WS_MAXIMIZEBOX,
			Minimize = WS_MINIMIZE,
			EnableMinimize = WS_MINIMIZEBOX,
			Tiled = WS_TILED,
			Overlapped = WS_OVERLAPPEDWINDOW,
			Popup = WS_POPUP,
			PopupSet = WS_POPUPWINDOW,
			EnableSizeChange = WS_SIZEBOX,
			SysMenu = WS_SYSMENU,
			AcceptTab = WS_TABSTOP,
			Visible = WS_VISIBLE
		};

		//拡張Windowスタイル
		enum class ExtStyle : Dword {
			EnableFileDD = WS_EX_ACCEPTFILES,
			ShowToTaskbar = WS_EX_APPWINDOW,
			UseEdgeFrame = WS_EX_CLIENTEDGE,
			EnableHelp = WS_EX_CONTEXTHELP,
			DistinguishChild = WS_EX_CONTROLPARENT,
			EmphasisDialog = WS_EX_DLGMODALFRAME,
			Layered = WS_EX_LAYERED,
			LeftOrder = WS_EX_LEFT,
			LeftScrollBar = WS_EX_LEFTSCROLLBAR,
			ReadFromLeft = WS_EX_LTRREADING,
			MdiChiled = WS_EX_MDICHILD,
			NoParentNotify = WS_EX_NOPARENTNOTIFY,
			Overlapped = WS_EX_OVERLAPPEDWINDOW,
			Palette = WS_EX_PALETTEWINDOW,
			RightOrder = WS_EX_RIGHT,
			RightScrollBar = WS_EX_RIGHTSCROLLBAR,
			ReadFromRight = WS_EX_RTLREADING,
			UseFloatFrame = WS_EX_STATICEDGE,
			ToolWindow = WS_EX_TOOLWINDOW,
			TopMost = WS_EX_TOPMOST,
			Transparent = WS_EX_TRANSPARENT
		};

		using ShowMode = WndShowMode;

		class Window {
		public:
			using ProcType = std::function<LongPtr(UintPtr, IntPtr)>;

			Window& operator = (const Window&) = delete;

			// Windowプロシージャの実態
			friend ::LRESULT CALLBACK _impl_MsgProc(::HWND hwnd, unsigned int msg, ::WPARAM wParam, ::LPARAM lParam);

			bool setTitle(const TString& title) {
				return ::SetWindowText(hwnd_, title.c_str()) != 0;
			}

			bool isAlive() {
				return hwnd_ != nullptr;
			}

			void update() {
				MSG msg = {};
				if (::PeekMessage(&msg, hwnd_, 0, 0, PM_REMOVE) != 0) {
					::TranslateMessage(&msg);
					::DispatchMessage(&msg);
				}

				if (isAlive() && ::UpdateWindow(hwnd_) == 0)
					throw Error(::GetLastError());
			}

			bool setShowMode() {
				if (::ShowWindow(hwnd_, sys::getWndShowmode()) == 0)
					return false;

				::UpdateWindow(hwnd_);
				return true;
			}
			bool setShowMode(ShowMode showMode) {
				if (::ShowWindow(hwnd_, util::unpackEnum(showMode)) == 0)
					return false;

				::UpdateWindow(hwnd_);
				return true;
			}
			
			bool resize(
				const Rectangle& nowSize,
				bool clientBase = true
				) {
				RECT nowWnd, nowClient;
				Rectangle newRect;
				
				::GetWindowRect(hwnd_, &nowWnd);
				::GetClientRect(hwnd_, &nowClient);

				if (clientBase) {
					newRect.x = (nowWnd.right - nowWnd.left) - (nowClient.right - nowClient.left) + nowSize.x;
					newRect.y = (nowWnd.bottom - nowWnd.top) - (nowClient.bottom - nowClient.top) + nowSize.y;
				}
				else {
					newRect.x = nowSize.x;
					newRect.y = nowSize.y;
				}

				return ::SetWindowPos(
					hwnd_,
					0, 0, 0,
					newRect.x,
					newRect.y,
					SWP_NOMOVE | SWP_NOZORDER
					) != 0;
			}

			bool move(
				const Position& newPos,
				bool doRedraw = true
				) {
				RECT rc;
				GetWindowRect(hwnd_, &rc);
				return MoveWindow(
					hwnd_,
					newPos.x, newPos.y,
					rc.right - rc.left,
					rc.bottom - rc.top,
					doRedraw
					) != 0;
			}

			//WindowStyleを取得
			UnifyEnum<Style> getStyle() const {
				return UnifyEnum<Style>(::GetWindowLong(hwnd_, GWL_STYLE));
			}
			//WindowStyleを設定
			auto setStyle(const UnifyEnum<Style>& newStyle) {
				::SetWindowLong(hwnd_, GWL_STYLE, newStyle());
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

				if (!::ScreenToClient(hwnd_, posBuf))
					throw Error(::GetLastError());

				return Position(posBuf->x, posBuf->y);
			}

			LongPtr defaultProc(Msg msg, UintPtr lp, IntPtr rp) {
				return ::DefWindowProc(hwnd_, util::unpackEnum(msg), lp, rp);
			}

			::HWND get() const {
				return hwnd_;
			}

			void clear() {
				hwnd_ = 0;
				msgProcs_.clear();
			}

		protected:
			::HWND hwnd_ = nullptr;
			//自身への参照を保持
			static std::unordered_map<::HWND, Window*> winRefs_;
			//CreateWindowExを呼んだクラス
			static Window* creater_;
			//メッセージプロシージャ
			std::unordered_map<unsigned int, ProcType> msgProcs_;

		};
		std::unordered_map<::HWND, Window*> Window::winRefs_;
		Window* Window::creater_ = nullptr;
		::LRESULT CALLBACK _impl_MsgProc(::HWND hwnd, unsigned int msg, ::WPARAM wParam, ::LPARAM lParam) {
			//存在しない場合リストに登録
			if (Window::winRefs_.find(hwnd) == Window::winRefs_.end()) {
				
				Window::winRefs_.insert(std::make_pair(hwnd, Window::creater_));

				return ::DefWindowProc(hwnd, msg, wParam, lParam);
			}

			//破棄されたら登録解除
			if (msg == WM_DESTROY) {
				//ユーザー定義のDestroyを呼ぶ
				const auto& procs = Window::winRefs_[hwnd]->msgProcs_;
				if (procs.find(WM_DESTROY) != procs.end())
					procs.at(WM_DESTROY)(wParam, lParam);

				Window::winRefs_[hwnd]->hwnd_ = nullptr;
				Window::winRefs_.erase(hwnd);

				return ::DefWindowProc(hwnd, msg, wParam, lParam);
			}

			auto& wnd = Window::winRefs_[hwnd];
			if (wnd->msgProcs_.find(msg) == wnd->msgProcs_.end())
				return ::DefWindowProc(hwnd, msg, wParam, lParam);
			else
				return wnd->msgProcs_[msg](wParam, lParam);

			return 0;
		}

		class RootWindow : public Window {
		public:
			RootWindow() = default;
			RootWindow(RootWindow&& rw) {
				hwnd_ = std::move(rw.hwnd_);
				msgProcs_ = std::move(rw.msgProcs_);

				winRefs_[hwnd_] = static_cast<Window*>(this);
			}
			RootWindow& operator = (RootWindow&& rw) {
				hwnd_ = std::move(rw.hwnd_);
				msgProcs_ = std::move(rw.msgProcs_);

				winRefs_[rw.hwnd_] = static_cast<Window*>(this);

				return *this;
			}

			RootWindow(const RootWindow&) = delete;
			RootWindow& operator = (const RootWindow&) = delete;

			RootWindow(
				const TString& titleName,
				const Prop& prop,
				const Rectangle& wndSize
				) :
				RootWindow(
					titleName,
					prop,
					wndSize,
					nullptr,
					nullptr,
					nullptr,
					nullptr
					) {}
			RootWindow(
				const TString& titleName,
				const Prop& prop,
				const Rectangle& wndSize,
				const UnifyEnum<Style>& styles
				) :
				RootWindow(
					titleName,
					prop,
					wndSize,
					nullptr,
					&styles,
					nullptr,
					nullptr
					) {}

			//TODO: コンストラクタの種類増加

			RootWindow(
				const TString& titleName,
				const Prop& prop,
				const Rectangle& wndSize,
				const Position& pos,
				const UnifyEnum<Style>& styles,
				const UnifyEnum<ExtStyle>& extStyles,
				const Menu& menu
				) : 
				RootWindow (
					titleName,
					prop,
					wndSize,
					&pos,
					&styles,
					&extStyles,
					&menu
					) {}

			auto& on(const Msg msg, const ProcType& proc) {
				msgProcs_.insert(std::make_pair(util::unpackEnum(msg), proc));

				return *this;
			}
			auto& off(const Msg msg) {
				msgProcs_.erase(util::unpackEnum(msg));

				return *this;
			}

		private:
			//ルートコンストラクタ
			RootWindow(
				const TString& titleName,
				const Prop& prop,
				const Rectangle& wndSize,
				const Position* pos,
				const UnifyEnum<Style>* styles,
				const UnifyEnum<ExtStyle>* extStyles,
				const Menu* menu
				) {
				creater_ = static_cast<Window*>(this);
				hwnd_ = CreateWindowEx(
					(extStyles != nullptr ? extStyles->get() : 0),
					prop.getName().c_str(),
					titleName.c_str(),
					(styles != nullptr ? styles->get() : 0),
					(pos != nullptr ? pos->x : CW_USEDEFAULT),
					(pos != nullptr ? pos->y : CW_USEDEFAULT),
					wndSize.x,
					wndSize.y,
					nullptr,
					(menu != nullptr ? menu->get() : nullptr),
					nullptr,
					nullptr
					);

				creater_ = nullptr;

				if (hwnd_ == nullptr || ::UpdateWindow(hwnd_) == 0)
					throw Error(::GetLastError());
			}

		};

		//MessageBox関連
		namespace mb {

			//showMessageで表示するボタン
			enum class Button : unsigned int {
				Failed = MB_ABORTRETRYIGNORE,
				Help = MB_HELP,
				OK = MB_OK,
				IsOK = MB_OKCANCEL,
				DoRetry = MB_RETRYCANCEL,
				YesNo = MB_YESNO,
				YesNoCancel = MB_YESNOCANCEL
			};
			//showMessageで表示するアイコン
			enum class Icon : unsigned int {
				Warning = MB_ICONWARNING,
				Info = MB_ICONINFORMATION,
				Question = MB_ICONQUESTION,
				Stop = MB_ICONSTOP
			};
			//showMessage表示中の他のWindow操作規制
			enum class CtrlRegLevel : unsigned int {
				App = MB_APPLMODAL,
				System = MB_SYSTEMMODAL,
				Task = MB_TASKMODAL
			};
			//showMessageの表示形式
			enum class Style : unsigned int {
				OnlyDefaultDesktop = MB_DEFAULT_DESKTOP_ONLY,
				RightText = MB_RIGHT,
				RightToLeft = MB_RTLREADING,
				Foreground = MB_SETFOREGROUND,
				TopMost = MB_TOPMOST,
				Notification = MB_SERVICE_NOTIFICATION,
				NotificationOnNT351 = MB_SERVICE_NOTIFICATION_NT3X
			};
			//showMessageの返り値
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
			
			//メッセージボックスを表示
			Result show(const TString& title, const TString& text, Button button) {
				return static_cast<Result>(
					::MessageBox(
						nullptr,
						text.c_str(),
						title.c_str(),
						util::unpackEnum(button)
						)
					);
			}

			//TODO : showMessageのオーバーロード追加

		} //::wawl::wnd::mb
	} //::wawl::wnd
} //::wawl