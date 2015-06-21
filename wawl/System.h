#pragma once

#include <cstdint>

//WinAPI
#define NOMINMAX
#include <Windows.h>
#include <tchar.h>
#undef NOMINMAX

//wawl based Header
#include "BaseType.h"

//ユーザー定義Main
void WawlMain();

namespace wawl {
	namespace sys {

		using AppHandle = HINSTANCE;

		class System final{
		public:
			System() = delete;
			System(const System&) = delete;
			System(System&&) = delete;
			void operator = (const System&) = delete;

			//このアプリのAppHandleを取得
			inline static AppHandle GetAppHandle() {
				return static_cast<HINSTANCE>(::GetModuleHandle(0));
			}

			//コマンドライン引数を取得
			inline static TString GetCmdLine() {
				return cmdArgStr_;
			}
			//完全なコマンドライン引数を取得
			inline static TString GetFullCmdLine() {
				return ::GetCommandLine();
			}

			inline static int GetWindowShowmode() {
				return windowShowmode_;
			}

			//システムクロックをミリ秒単位で取得
			inline static std::size_t GetTimeMilisec() {
				return timeGetTime();
			}

			//システムのタイマー分解能を変更。必ず終わったあとResetTimeResolution()を呼び出す
			inline static bool ChangeTimeResolution(unsigned int res) {
				return (
					sysRes_ == 0 ?
					sysRes_ = res, ::timeBeginPeriod(res) == TIMERR_NOERROR :
					false
					);
			}
			//システムのタイマー分解能を元に戻す
			inline static bool ResetTimeResolution() {
				int tmpRes = sysRes_;

				return (
					sysRes_ != 0 ?
					sysRes_ = 0, ::timeEndPeriod(tmpRes) == TIMERR_NOERROR :
					false
					);
			}

			static void _impl_SetWinMainArgs(const TString& cmdLine, int cmdShow) {
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
		TString System::cmdArgStr_;
		int System::windowShowmode_ = 0;
		int System::sysRes_ = 0;

	}
}

int WINAPI _tWinMain(
	HINSTANCE hInst,
	HINSTANCE hPrevInst,
	LPTSTR cmdLine,
	int cmdShow
	) {
	using namespace wawl;

	sys::System::_impl_SetWinMainArgs(cmdLine, cmdShow);

	WawlMain();

	return 0;
}