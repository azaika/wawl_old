#pragma once

#define WAWL_ENABLE_SYSTEM

//WinAPI
#define NOMINMAX
#include <Windows.h>
#include <tchar.h>
#undef NOMINMAX

//wawl based Header
#include "BaseType.h"

//���[�U�[��`Main
void wawlMain();

namespace wawl {
	namespace sys {

		using AppHandle = HINSTANCE;

		//���̃A�v����AppHandle���擾
		inline AppHandle getAppHandle() {
			return static_cast<::HINSTANCE>(::GetModuleHandle(0));
		}

		//���S�ȃR�}���h���C���������擾
		inline TString getFullCmdArgs() {
			return ::GetCommandLine();
		}

		//�V�X�e���N���b�N���~���b�P�ʂŎ擾
		inline std::size_t getTimeMilisec() {
			return static_cast<std::size_t>(timeGetTime());
		}

		enum class ExitMode : unsigned int {
			Logoff = EWX_LOGOFF,
			Poweroff = EWX_POWEROFF,
			Reboot = EWX_REBOOT,
			Shutdown = EWX_SHUTDOWN
		};
		inline bool Exit(const ExitMode mode) {
			return ::ExitWindowsEx(static_cast<unsigned int>(mode), 0) != 0;
		}
		inline bool Exit(const ExitMode mode, bool doWaitHang) {
			return ::ExitWindowsEx(static_cast<unsigned int>(mode) | (doWaitHang ? EWX_FORCEIFHUNG : EWX_FORCE), 0) != 0;
		}

		inline bool StartShutdown(const TString& machineName, const TString& msg, Dword timeOut = 0, Bool isForce = false, Bool doRestart = false) {
			return ::InitiateSystemShutdown(
				const_cast<TChar*>(machineName.c_str()),
				const_cast<TChar*>(msg.c_str()),
				timeOut,
				isForce,
				doRestart
				) != 0;
		}
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

			//�R�}���h���C���������擾
			friend inline TString getCmdArgs() {
				return cmdArgStr_;
			}

			friend inline int getWndShowmode() {
				return windowShowmode_;
			}

			//�V�X�e���̃^�C�}�[����\��ύX�B�K���I���������ResetTimeResolution()���Ăяo��
			friend inline bool changeTimeRes(unsigned int res) {
				return (
					sysRes_ == 0 ?
					sysRes_ = res, ::timeBeginPeriod(res) == TIMERR_NOERROR :
					false
					);
			}
			//�V�X�e���̃^�C�}�[����\�����ɖ߂�
			friend inline bool resetTimeRes() {
				int tmpRes = sysRes_;

				return (
					sysRes_ != 0 ?
					sysRes_ = 0, ::timeEndPeriod(tmpRes) == TIMERR_NOERROR :
					false
					);
			}

			static void _impl_setWinMainArgs(const TString& cmdLine, int cmdShow) {
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

	} //::wawl::sys
} //::wawl

int WINAPI _tWinMain(
	HINSTANCE hInst,
	HINSTANCE hPrevInst,
	LPTSTR cmdLine,
	int cmdShow
	) {
	using namespace wawl;

	sys::_impl_System::_impl_setWinMainArgs(cmdLine, cmdShow);

	wawlMain();

	return 0;
}