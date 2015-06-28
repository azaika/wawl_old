#pragma once

//WinAPI
#define NOMINMAX
#include <Windows.h>
#include <tchar.h>
#undef NOMINMAX

//wawl based Header
#include "BaseType.h"

//���[�U�[��`Main
void WawlMain();

namespace wawl {
	namespace sys {

		using AppHandle = HINSTANCE;

		//���̃A�v����AppHandle���擾
		inline AppHandle GetAppHandle() {
			return static_cast<HINSTANCE>(::GetModuleHandle(0));
		}

		//���S�ȃR�}���h���C���������擾
		inline TString GetFullCmdLine() {
			return ::GetCommandLine();
		}

		//�V�X�e���N���b�N���~���b�P�ʂŎ擾
		inline std::size_t GetTimeMilisec() {
			return static_cast<std::size_t>(timeGetTime());
		}

		class _impl_System final{
		public:
			_impl_System() = delete;
			_impl_System(const _impl_System&) = delete;
			_impl_System(_impl_System&&) = delete;
			void operator = (const _impl_System&) = delete;

			//�R�}���h���C���������擾
			friend inline TString GetCmdLine() {
				return cmdArgStr_;
			}
			
			friend inline int GetWindowShowmode() {
				return windowShowmode_;
			}

			//�V�X�e���̃^�C�}�[����\��ύX�B�K���I���������ResetTimeResolution()���Ăяo��
			friend inline bool ChangeTimeResolution(unsigned int res) {
				return (
					sysRes_ == 0 ?
					sysRes_ = res, ::timeBeginPeriod(res) == TIMERR_NOERROR :
					false
					);
			}
			//�V�X�e���̃^�C�}�[����\�����ɖ߂�
			friend inline bool ResetTimeResolution() {
				int tmpRes = sysRes_;

				return (
					sysRes_ != 0 ?
					sysRes_ = 0, ::timeEndPeriod(tmpRes) == TIMERR_NOERROR :
					false
					);
			}

			static void SetWinMainArgs(const TString& cmdLine, int cmdShow) {
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

		bool RunCmdLine(const TString& cmdLine, bool showWindow = false) {

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

	sys::_impl_System::SetWinMainArgs(cmdLine, cmdShow);

	WawlMain();

	return 0;
}