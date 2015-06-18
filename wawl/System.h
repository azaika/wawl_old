#pragma once

#include <cstdint>

//WinAPI
#define NOMINMAX
#include <Windows.h>
#include <tchar.h>
#undef NOMINMAX

//wawl based Header
#include "String.h"

//ユーザー定義Main
void WawlMain();

namespace wawl {
	namespace sys {

		using AppHandle = HINSTANCE;

		class System final{
		public:
			//このアプリのAppHandleを取得
			inline static AppHandle GetAppHandle() {
				return static_cast<HINSTANCE>(::GetModuleHandle(0));
			}

			//コマンドライン引数を取得
			inline static str::TString GetCmdLine() {
				return cmdArgStr;
			}
			//完全なコマンドライン引数を取得
			inline static str::TString GetFullCmdLine() {
				return ::GetCommandLine();
			}

			inline static int GetWindowShowmode() {
				return windowShowmode;
			}

			//システムクロックをミリ秒単位で取得
			inline static std::size_t GetTimeMilisec() {
				return timeGetTime();
			}

			//システムのタイマー分解能を変更。必ず終わったあとResetTimeResolution()を呼び出す
			inline static bool ChangeTimeResolution(unsigned int res) {
				return (
					sysClock == 0 ?
					sysClock = res, ::timeBeginPeriod(res) == TIMERR_NOERROR :
					false
					);
			}
			//システムのタイマー分解能を元に戻す
			inline static bool ResetTimeResolution() {
				int tmpRes = sysClock;

				return (
					sysClock != 0 ?
					sysClock = 0, ::timeEndPeriod(tmpRes) == TIMERR_NOERROR :
					false
					);
			}

			static void _impl_SetWinMainArgs(const str::TString& cmdLine, int cmdShow) {
				//初回呼び出しかどうか
				static bool isFirst = true;

				if (isFirst) { //初回呼び出しならば
					isFirst = false;
					cmdArgStr = cmdLine;
					windowShowmode = cmdShow;
				}
			}

		private:
			//コマンドライン引数
			static str::TString cmdArgStr;
			//OSから渡されたWindow初期化
			static int windowShowmode;

			//システムクロック変更値の保存
			static int sysClock;

		};
		str::TString System::cmdArgStr;
		int System::windowShowmode = 0;
		int System::sysClock = 0;

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

	return 0;
}