#pragma once

#define WAWL_ENABLE_SYSTEM

//WinAPI
#define NOMINMAX
#include <Windows.h>
#include <tchar.h>
#undef NOMINMAX

#pragma comment(lib, "winmm.lib")

//wawl based Header
#include "BaseType.h"

//ユーザー定義Main
void wawlMain();

namespace wawl {
	namespace sys {

		using AppHandle = HINSTANCE;

		//このアプリのAppHandleを取得
		inline AppHandle getAppHandle() {
			return static_cast<::HINSTANCE>(::GetModuleHandle(0));
		}

		//完全なコマンドライン引数を取得
		inline TString getFullCmdArgs() {
			return ::GetCommandLine();
		}

		//システムクロックをミリ秒単位で取得
		inline std::size_t getTimeMilisec() {
			return static_cast<std::size_t>(timeGetTime());
		}
		
		//Exitの種類
		enum class ExitMode : unsigned int {
			Logoff = EWX_LOGOFF,
			Poweroff = EWX_POWEROFF,
			Reboot = EWX_REBOOT,
			Shutdown = EWX_SHUTDOWN
		};
		
		inline bool Exit(ExitMode mode) {
			return ::ExitWindowsEx(
				static_cast<unsigned int>(mode),
				0
				) != 0;
		}
		inline bool Exit(ExitMode mode, bool doWaitHang) {
			return ::ExitWindowsEx(
				static_cast<unsigned int>(mode) |
				(doWaitHang ? EWX_FORCEIFHUNG : EWX_FORCE),
				0
				) != 0;
		}
		
		//コンピューターをシャットダウンします
		inline bool StartShutdown(const TString& machineName, const TString& msg, Dword timeOut = 0, Bool force = false, Bool doRestart = false) {
			return ::InitiateSystemShutdown(
				const_cast<TChar*>(machineName.c_str()),
				const_cast<TChar*>(msg.c_str()),
				timeOut,
				force,
				doRestart
				) != 0;
		}
		//StartShutDownでtimeOutが設定されていた場合、その時間以内ならシャットダウンを中止します。
		inline bool AbortShutdown(const TString& machineName) {
			return ::AbortSystemShutdown(const_cast<TChar*>(machineName.c_str())) != 0;
		}

		class _impl_System final {
		public:
			_impl_System() = delete;
			_impl_System(const _impl_System&) = delete;
			_impl_System(_impl_System&&) = delete;
			void operator = (const _impl_System&) = delete;
			void operator = (_impl_System&&) = delete;

			//コマンドライン引数を取得
			friend const TString& getCmdArgs();

			friend int getWndShowmode();

			//システムのタイマー分解能を変更。必ず終わったあとResetTimeResolution()を呼び出す
			friend bool changeTimeRes(unsigned int res);
			//システムのタイマー分解能を元に戻す
			friend bool resetTimeRes();

			static void setWinMainArgs(const TString& cmdLine, int cmdShow) {
				//初回呼び出しの時のみ登録
				static bool isFirst = true;
				if (isFirst) {
					isFirst = false;
					cmdArgStr_ = cmdLine;
					windowShowmode_ = cmdShow;
				}
			}

		private:
			//コマンドライン引数
			static TString cmdArgStr_;
			//OSから渡されたWindow初期化
			static int windowShowmode_;

			//システムクロック変更値の保存
			static int sysRes_;

		};
		TString _impl_System::cmdArgStr_;
		int _impl_System::windowShowmode_ = 0;
		int _impl_System::sysRes_ = 0;

		int getWndShowmode() {
			return _impl_System::windowShowmode_;
		}
		const TString& getCmdArgs() {
			return _impl_System::cmdArgStr_;
		}
		bool changeTimeRes(unsigned int res) {
			return (
				_impl_System::sysRes_ == 0
				? _impl_System::sysRes_ = res, ::timeBeginPeriod(res) == TIMERR_NOERROR
				: false
				);
		}
		bool resetTimeRes() {
			int tmpRes = _impl_System::sysRes_;

			return (
				_impl_System::sysRes_ != 0
				? _impl_System::sysRes_ = 0, ::timeEndPeriod(tmpRes) == TIMERR_NOERROR
				: false
				);
		}

	} //::wawl::sys
} //::wawl

int WINAPI _tWinMain(
	HINSTANCE hInst,
	HINSTANCE hPrevInst,
	LPTSTR cmdLine,
	int cmdShow
	) {
	using namespace wawl;

	sys::_impl_System::setWinMainArgs(cmdLine, cmdShow);

	wawlMain();

	return 0;
}