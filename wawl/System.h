#pragma once

//WinAPI
#define NOMINMAX
#include <Windows.h>
#include <tchar.h>
#undef NOMINMAX

//wawl based Header
#include "BaseType.h"
#include"FileSystem.h"

//���[�U�[��`Main
void wawlMain(const wawl::TString&);

namespace wawl {
	namespace sys {

		using AppHandle = HINSTANCE;

		//���̃A�v����AppHandle���擾
		inline AppHandle getAppHandle() {
			return static_cast<::HINSTANCE>(::GetModuleHandle(0));
		}

		//���S�ȃR�}���h���C���������擾
		inline TString getFullCmdLine() {
			return ::GetCommandLine();
		}

		//�V�X�e���N���b�N���~���b�P�ʂŎ擾
		inline std::size_t getTimeMilisec() {
			return static_cast<std::size_t>(timeGetTime());
		}

		class _impl_System final{
		public:
			_impl_System() = delete;
			_impl_System(const _impl_System&) = delete;
			_impl_System(_impl_System&&) = delete;
			void operator = (const _impl_System&) = delete;

			//�R�}���h���C���������擾
			friend inline TString getCmdLine() {
				return cmdArgStr_;
			}
			
			friend inline int getWndShowmode() {
				return windowShowmode_;
			}

			//�V�X�e���̃^�C�}�[����\��ύX�B�K���I���������ResetTimeResolution()���Ăяo��
			friend inline bool changeTimeResolution(unsigned int res) {
				return (
					sysRes_ == 0 ?
					sysRes_ = res, ::timeBeginPeriod(res) == TIMERR_NOERROR :
					false
					);
			}
			//�V�X�e���̃^�C�}�[����\�����ɖ߂�
			friend inline bool resetTimeResolution() {
				int tmpRes = sysRes_;

				return (
					sysRes_ != 0 ?
					sysRes_ = 0, ::timeEndPeriod(tmpRes) == TIMERR_NOERROR :
					false
					);
			}

			static void setWinMainArgs(const TString& cmdLine, int cmdShow) {
				//����Ăяo�����ǂ���
				static bool isFirst = true;

				if (isFirst) { //����Ăяo���Ȃ��
					isFirst = false;
					cmdArgStr_ = cmdLine;
					windowShowmode_ = cmdShow;
				}
			}

		private:
			//�R�}���h���C������
			static TString cmdArgStr_;
			//OS����n���ꂽWindow������
			static int windowShowmode_;

			//�V�X�e���N���b�N�ύX�l�̕ۑ�
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

			//STARTUPINFO �\���̂̓��e���擾
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