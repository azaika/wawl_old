#pragma once

#include <cstdint>

//WinAPI
#define NOMINMAX
#include <Windows.h>
#include <tchar.h>
#undef NOMINMAX

//wawl based Header
#include "String.h"

//���[�U�[��`Main
void WawlMain();

namespace wawl {
	namespace sys {

		using AppHandle = HINSTANCE;

		class System final{
		public:
			//���̃A�v����AppHandle���擾
			inline static AppHandle GetAppHandle() {
				return static_cast<HINSTANCE>(::GetModuleHandle(0));
			}

			//�R�}���h���C���������擾
			inline static str::TString GetCmdLine() {
				return cmdArgStr;
			}
			//���S�ȃR�}���h���C���������擾
			inline static str::TString GetFullCmdLine() {
				return ::GetCommandLine();
			}

			inline static int GetWindowShowmode() {
				return windowShowmode;
			}

			//�V�X�e���N���b�N���~���b�P�ʂŎ擾
			inline static std::size_t GetTimeMilisec() {
				return timeGetTime();
			}

			//�V�X�e���̃^�C�}�[����\��ύX�B�K���I���������ResetTimeResolution()���Ăяo��
			inline static bool ChangeTimeResolution(unsigned int res) {
				return (
					sysClock == 0 ?
					sysClock = res, ::timeBeginPeriod(res) == TIMERR_NOERROR :
					false
					);
			}
			//�V�X�e���̃^�C�}�[����\�����ɖ߂�
			inline static bool ResetTimeResolution() {
				int tmpRes = sysClock;

				return (
					sysClock != 0 ?
					sysClock = 0, ::timeEndPeriod(tmpRes) == TIMERR_NOERROR :
					false
					);
			}

			static void _impl_SetWinMainArgs(const str::TString& cmdLine, int cmdShow) {
				//����Ăяo�����ǂ���
				static bool isFirst = true;

				if (isFirst) { //����Ăяo���Ȃ��
					isFirst = false;
					cmdArgStr = cmdLine;
					windowShowmode = cmdShow;
				}
			}

		private:
			//�R�}���h���C������
			static str::TString cmdArgStr;
			//OS����n���ꂽWindow������
			static int windowShowmode;

			//�V�X�e���N���b�N�ύX�l�̕ۑ�
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