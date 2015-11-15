#pragma once

#define WAWL_ENABLE_WINDOW

#include "System.h"
#include "Utility.h"
#include "Menu.h"
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
		using UnifyPropOption = _impl_UnifyEnum<PropOption>;

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

		//Window�v���V�[�W���̐錾
		::LRESULT CALLBACK _impl_MsgProc(::HWND hwnd, unsigned int uMsg, ::WPARAM wParam, ::LPARAM lParam);

		class Prop {
		public:
			Prop(const Prop&) = default;
			Prop& operator = (const Prop&) = default;
			Prop(Prop&&) = default;
			Prop& operator = (Prop&&) = default;

			Prop() :
				Prop(
					UnifyPropOption({PropOption::HRedraw, PropOption::VRedraw}),
					DefaultIcon::App,
					DefaultIcon::App,
					DefaultCursor::Arrow,
					Color::Hollow
					) {}
			Prop(
				const TString& menuName
				) :
				Prop(
					UnifyPropOption({ PropOption::HRedraw, PropOption::VRedraw }),
					DefaultIcon::App,
					DefaultIcon::App,
					DefaultCursor::Arrow,
					Color::Hollow,
					menuName
					) {}
			Prop(
				const UnifyPropOption& options
				) :
				Prop(
					options,
					DefaultIcon::App,
					DefaultIcon::App,
					DefaultCursor::Arrow,
					Color::Hollow
					) {}
			Prop(
				const UnifyPropOption& options,
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
				const UnifyPropOption& options,
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

			//TODO: �R���X�g���N�^�ǉ�

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

			//���[�g�R���X�g���N�^
			Prop(
				const UnifyPropOption* options,
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

		//Window�X�^�C��
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
		using UnifyStyle = _impl_UnifyEnum<Style>;

		//�g��Window�X�^�C��
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
		using UnifyExtStyle = _impl_UnifyEnum<ExtStyle>;

		using ShowMode = WndShowMode;

		class Window {
		public:
			using ProcType = std::function<int(UintPtr, IntPtr)>;

			// Window�v���V�[�W���̎���
			friend ::LRESULT CALLBACK _impl_MsgProc(::HWND hwnd, unsigned int msg, ::WPARAM wParam, ::LPARAM lParam);

			bool show() {
				return ::ShowWindow(hwnd_, sys::getWndShowmode()) == 0;
			}
			bool show(ShowMode showMode) {
				return ::ShowWindow(hwnd_, util::unpackEnum(showMode)) == 0;
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

			::HWND get() const {
				return hwnd_;
			}

			void clear() {
				hwnd_ = 0;
				msgProcs_.clear();
			}

		protected:
			::HWND hwnd_ = nullptr;
			//���g�ւ̎Q�Ƃ�ێ�
			static std::unordered_map<::HWND, Window*> winRefs_;
			//CreateWindowEx���Ă񂾃N���X
			static Window* creater_;
			//���b�Z�[�W�v���V�[�W��
			std::unordered_map<unsigned int, ProcType> msgProcs_;

		};
		std::unordered_map<::HWND, Window*> Window::winRefs_;
		Window* Window::creater_ = nullptr;
		::LRESULT CALLBACK _impl_MsgProc(::HWND hwnd, unsigned int msg, ::WPARAM wParam, ::LPARAM lParam) {
			//�������ꂽ���Ƀ��X�g�ɓo�^
			if (msg == WM_CREATE &&
				Window::winRefs_.find(hwnd) == Window::winRefs_.end()) {
				
				Window::winRefs_.insert(std::make_pair(hwnd, Window::creater_));

				return ::DefWindowProc(hwnd, msg, wParam, lParam);
			}

			//�j�����ꂽ��o�^����
			if (msg == WM_DESTROY &&
				Window::winRefs_.find(hwnd) != Window::winRefs_.end()) {
				//���[�U�[��`��Destroy���Ă�
				auto& procs = Window::winRefs_[hwnd]->msgProcs_;
				if (procs.find(WM_DESTROY) != procs.end())
					procs[WM_DESTROY](wParam, lParam);

				Window::winRefs_.erase(hwnd);

				return ::DefWindowProc(hwnd, msg, wParam, lParam);
			}

			//���݂��Ȃ��ꍇ�G���[
			if (Window::winRefs_.find(hwnd) == Window::winRefs_.end())
				return 1;

			auto& wnd = Window::winRefs_[hwnd];

			auto it = wnd->msgProcs_.find(1);
			auto ite = wnd->msgProcs_.end();

			if (wnd->msgProcs_.find(msg) == wnd->msgProcs_.end())
				return ::DefWindowProc(hwnd, msg, wParam, lParam);
			else
				return wnd->msgProcs_[msg](wParam, lParam);

			return 0;
		}

		class RootWindow : public Window {
		public:
			RootWindow() = default;
			RootWindow(RootWindow&&) = default;
			RootWindow& operator = (RootWindow&&) = default;

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
				const UnifyStyle& styles
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

			//TODO: �R���X�g���N�^�̎�ޑ���

			RootWindow(
				const TString& titleName,
				const Prop& prop,
				const Rectangle& wndSize,
				const Position& pos,
				const UnifyStyle& styles,
				const UnifyExtStyle& extStyles,
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

			RootWindow& addMsgProc(const Msg msg, const ProcType& proc) {
				msgProcs_.insert(std::make_pair(util::unpackEnum(msg), proc));

				return *this;
			}
			RootWindow& removeMsgProc(const Msg msg) {
				msgProcs_.erase(util::unpackEnum(msg));

				return *this;
			}

		private:
			RootWindow(
				const TString& titleName,
				const Prop& prop,
				const Rectangle& wndSize,
				const Position* pos,
				const UnifyStyle* styles,
				const UnifyExtStyle* extStyles,
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

		//MessageBox�֘A
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