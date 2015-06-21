#pragma once

#include <cstdint>

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

		class System final{
		public:
			System() = delete;
			System(const System&) = delete;
			System(System&&) = delete;
			void operator = (const System&) = delete;

			//���̃A�v����AppHandle���擾
			inline static AppHandle GetAppHandle() {
				return static_cast<HINSTANCE>(::GetModuleHandle(0));
			}

			//�R�}���h���C���������擾
			inline static TString GetCmdLine() {
				return cmdArgStr_;
			}
			//���S�ȃR�}���h���C���������擾
			inline static TString GetFullCmdLine() {
				return ::GetCommandLine();
			}

			inline static int GetWindowShowmode() {
				return windowShowmode_;
			}

			//�V�X�e���N���b�N���~���b�P�ʂŎ擾
			inline static std::size_t GetTimeMilisec() {
				return timeGetTime();
			}

			//�V�X�e���̃^�C�}�[����\��ύX�B�K���I���������ResetTimeResolution()���Ăяo��
			inline static bool ChangeTimeResolution(unsigned int res) {
				return (
					sysRes_ == 0 ?
					sysRes_ = res, ::timeBeginPeriod(res) == TIMERR_NOERROR :
					false
					);
			}
			//�V�X�e���̃^�C�}�[����\�����ɖ߂�
			inline static bool ResetTimeResolution() {
				int tmpRes = sysRes_;

				return (
					sysRes_ != 0 ?
					sysRes_ = 0, ::timeEndPeriod(tmpRes) == TIMERR_NOERROR :
					false
					);
			}

			static void _impl_SetWinMainArgs(const TString& cmdLine, int cmdShow) {
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