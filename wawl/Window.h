#pragma once

#include "System.h"

namespace wawl {
	namespace wnd {

		class Prop {
		public:
			Prop() {
				ZeroMemory(&wndClass_, sizeof(wndClass_));
				wndClass_.cbSize = sizeof(::WNDCLASSEX);

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
			::WNDCLASSEX wndClass_;

		};

		class Window {
		public:

		private:
			::HWND hwnd_;

		};

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