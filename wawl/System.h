#pragma once

//WinAPI
#define NOMINMAX
#include <Windows.h>
#include <tchar.h>
#undef NOMINMAX

//wawl based Header
#include "BaseType.h"

//ユーザー定義Main
void wawlMain(const wawl::TString&);

namespace wawl {
	namespace sys {

		using AppHandle = HINSTANCE;

		//このアプリのAppHandleを取得
		inline AppHandle getAppHandle() {
			return static_cast<::HINSTANCE>(::GetModuleHandle(0));
		}

		//完全なコマンドライン引数を取得
		inline TString getFullCmdLine() {
			return ::GetCommandLine();
		}

		//システムクロックをミリ秒単位で取得
		inline std::size_t getTimeMilisec() {
			return static_cast<std::size_t>(timeGetTime());
		}

		class _impl_System final{
		public:
			_impl_System() = delete;
			_impl_System(const _impl_System&) = delete;
			_impl_System(_impl_System&&) = delete;
			void operator = (const _impl_System&) = delete;

			//コマンドライン引数を取得
			friend inline TString getCmdLine() {
				return cmdArgStr_;
			}
			
			friend inline int getWndShowmode() {
				return windowShowmode_;
			}

			//システムのタイマー分解能を変更。必ず終わったあとResetTimeResolution()を呼び出す
			friend inline bool changeTimeResolution(unsigned int res) {
				return (
					sysRes_ == 0 ?
					sysRes_ = res, ::timeBeginPeriod(res) == TIMERR_NOERROR :
					false
					);
			}
			//システムのタイマー分解能を元に戻す
			friend inline bool resetTimeResolution() {
				int tmpRes = sysRes_;

				return (
					sysRes_ != 0 ?
					sysRes_ = 0, ::timeEndPeriod(tmpRes) == TIMERR_NOERROR :
					false
					);
			}

			static void setWinMainArgs(const TString& cmdLine, int cmdShow) {
				//初回呼び出しかどうか
				static bool isFirst = true;

				if (isFirst) { //初回呼び出しならば
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

		bool runCmdLine(const TString& cmdLine, bool showWindow = false) {

			STARTUPINFO si;
			PROCESS_INFORMATION pi = {};

			//STARTUPINFO 構造体の内容を取得
			GetStartupInfo(&si);

			return ::CreateProcess(
				nullptr,
				const_cast<TChar*>(cmdLine.c_str()),
				nullptr,
				nullptr,
				false,
				(showWindow ? NULL : CREATE_NO_WINDOW),
				nullptr,
				nullptr,
				&si,
				&pi
				) != 0;
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

	wawlMain(cmdLine);

	return 0;
}