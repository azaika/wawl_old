#pragma once

//WinAPI
#define NOMINMAX
#include <Windows.h>
#include <tchar.h>
#undef NOMINMAX

//wawl based Header
#include "BaseType.h"
#include"FileSystem.h"

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

		//Registry testcode
		namespace reg {
			enum class RegistryOption {
				BackupRestore = REG_OPTION_BACKUP_RESTORE,
				CreateLink = REG_OPTION_CREATE_LINK,
				NonVolatile = REG_OPTION_NON_VOLATILE,
				OpenLink = REG_OPTION_OPEN_LINK,
				Reserved = REG_OPTION_RESERVED,
				Volatile = REG_OPTION_VOLATILE,
			};
			using UnifyRegistryOption = _impl_UnifyEnum<RegistryOption>;

			enum class KeyOption {
				AllAccess = KEY_ALL_ACCESS,
				CreateLink = KEY_CREATE_LINK,
				CreateSubKey = KEY_CREATE_SUB_KEY,
				EnamerateSubKey = KEY_ENUMERATE_SUB_KEYS,
				Event = KEY_EVENT,
				Execute = KEY_EXECUTE,
				LengthMask = KEY_LENGTH_MASK,
				Notify = KEY_NOTIFY,
				QueryValue = KEY_QUERY_VALUE,
				Read = KEY_READ,
				SetValue = KEY_SET_VALUE,
				Wow64_32 = KEY_WOW64_32KEY,
				Wow64_64 = KEY_WOW64_64KEY,
				Wow64Res = KEY_WOW64_RES,
				Write = KEY_WRITE,
			};
			using UnifyKeyOption = _impl_UnifyEnum<KeyOption>;

			enum class RegistryType {
				Binary = REG_BINARY,
				DWord = REG_DWORD,
				ExpandString = REG_EXPAND_SZ,
				MultiString = REG_MULTI_SZ,
				QWord = REG_QWORD,
				String = REG_SZ,
			};
			using UnifyRegistryType = _impl_UnifyEnum<RegistryType>;


			class Value {
			public:
				Value(void *value) {
					this->value = value;
				}

				auto& get() {
					return value;
				}
				const auto& get() const {
					return value;
				}
				auto& operator () () {
					return value;
				}
				const auto& operator () () const {
					return value;
				}

				unsigned int size() {
					return sizeof(value);
				}

			private:
				void *value;

			};

			class Key {
			public:
				Key(HKEY CurrentKey, WString Name, RegistryOption regop, KeyOption keyop, fs::SecurityAttrib secatt) {
					DWORD DisPosition;
					ZeroMemory(_hkey, sizeof(_hkey));
					RegCreateKeyEx(CurrentKey, Name.c_str(), NULL, L"", static_cast<UINT>(regop), static_cast<UINT>(keyop), &secatt.get(), &_hkey, &DisPosition);
				}
				~Key() {
					RegCloseKey(_hkey);
				}
				void SetValue(WString Name, RegistryType RegType, Value value) {
					RegSetValueEx(_hkey, Name.c_str(), NULL, static_cast<DWORD>(RegType), (const BYTE*)value(), value.size());
				}
				void DeleteValue(WString Name) {
					RegDeleteValue(_hkey, Name.c_str());
				}

			private:
				::HKEY _hkey;

			};

		}


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