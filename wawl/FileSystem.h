#pragma once

//wawl Header
#include "BaseType.h"
//C++ STL
#include <memory>

namespace wawl {
	namespace fs {

		//�A�N�Z�X�w��q
		enum class AccessDesc : Dword {
			None = NULL,
			All = GENERIC_ALL,
			Execute = GENERIC_EXECUTE,
			Read = GENERIC_READ,
			Write = GENERIC_WRITE,
			Delete = DELETE,
			ReadSecurityCtrl = READ_CONTROL,
			EnableSync = SYNCHRONIZE,
			WriteDac = WRITE_DAC,
			ChangeOwner = WRITE_OWNER,
			RightAll = STANDARD_RIGHTS_ALL,
			RightNormal = STANDARD_RIGHTS_REQUIRED,
			AccessSysSecurity = ACCESS_SYSTEM_SECURITY,
			AllowMaximum = MAXIMUM_ALLOWED,
			RightAllSpecificDesc = SPECIFIC_RIGHTS_ALL
		};
		using UnifyAccessDesc = _impl_UnifyEnum < AccessDesc >;

		//�t�@�C���V�F�A�`��
		enum class FileSharePermit : Dword {
			None = NULL,
			Delete = FILE_SHARE_DELETE,
			Read = FILE_SHARE_READ,
			Write = FILE_SHARE_WRITE
		};
		using UnifyFileSharePermit = _impl_UnifyEnum < FileSharePermit >;

		//�t�@�C�������K��
		enum class FileCreateProv : Dword {
			New = CREATE_NEW,
			AlwaysNew = CREATE_ALWAYS,
			OpenExisting = OPEN_EXISTING,
			AlwaysOpen = OPEN_ALWAYS,
			ClearExisting = TRUNCATE_EXISTING
		};
		using UnifyFileCreateProv = _impl_UnifyEnum < FileCreateProv >;

		//�t�@�C���̑����L�q�q
		enum class FileAttrib : Dword {
			Archive = FILE_ATTRIBUTE_ARCHIVE,
			Encrypt = FILE_ATTRIBUTE_ENCRYPTED,
			Hide = FILE_ATTRIBUTE_HIDDEN,
			Normal = FILE_ATTRIBUTE_NORMAL,
			NotIndexed = FILE_ATTRIBUTE_NOT_CONTENT_INDEXED,
			Offline = FILE_ATTRIBUTE_OFFLINE,
			ReadOnly = FILE_ATTRIBUTE_READONLY,
			System = FILE_ATTRIBUTE_SYSTEM,
			Tmporary = FILE_ATTRIBUTE_TEMPORARY,

			NoCaching = FILE_FLAG_WRITE_THROUGH,
			Overlapp = FILE_FLAG_OVERLAPPED,
			NoBuffering = FILE_FLAG_NO_BUFFERING,
			RandomAccess = FILE_FLAG_RANDOM_ACCESS,
			SequentialAccess = FILE_FLAG_SEQUENTIAL_SCAN,
			CloseToDelete = FILE_FLAG_DELETE_ON_CLOSE,
			Buckup = FILE_FLAG_BACKUP_SEMANTICS,
			UsePosixSemantics = FILE_FLAG_POSIX_SEMANTICS,
			NoReparsing = FILE_FLAG_OPEN_REPARSE_POINT,
			NoRecall = FILE_FLAG_OPEN_NO_RECALL,

			Anonymous = SECURITY_ANONYMOUS,
			Identification = SECURITY_IDENTIFICATION,
			Impersonation = SECURITY_IMPERSONATION,
			Delegation = SECURITY_IMPERSONATION,
			DynamicTracking = SECURITY_CONTEXT_TRACKING,
			EffectiveOnly = SECURITY_EFFECTIVE_ONLY,
			EnableSecurityCamouflage = SECURITY_SQOS_PRESENT
		};
		using UnifyFileAttrib = _impl_UnifyEnum < FileAttrib >;

		//�t�@�C��
		class File {
		public:
			File() = default;
			File(const File&) = default;
			File& operator = (const File&) = default;

			File(
				const TString* fileName,
				const UnifyAccessDesc* accessDesc,
				const UnifyFileSharePermit* shareMode,
				const SecurityAttrib* secAttr,
				const UnifyFileCreateProv* createProv,
				const UnifyFileAttrib* fileAttr,
				const File* baseFile
				) :
				secAttr_(std::make_shared<SecurityAttrib>(*secAttr)) {
				file_ = ::CreateFile(
					(fileName == nullptr ? nullptr : fileName->c_str()),
					(accessDesc == nullptr ? GENERIC_ALL : accessDesc->get()),
					(shareMode == nullptr ? NULL : shareMode->get()),
					(secAttr == nullptr ? nullptr : &(secAttr_.get()->get())),
					(createProv == nullptr ? CREATE_ALWAYS : createProv->get()),
					(fileAttr == nullptr ? FILE_ATTRIBUTE_NORMAL : fileAttr->get()),
					(baseFile == nullptr ? nullptr : baseFile->get())
					);

				if (file_ == INVALID_HANDLE_VALUE)
					throw std::runtime_error{ "Faild to CreateFile." };
			}
			File(
				const TString& fileName,
				const UnifyFileCreateProv& createProv
				) :
				File(
				&fileName,
				nullptr,
				nullptr,
				nullptr,
				&createProv,
				nullptr,
				nullptr
				) {}
			File(
				const TString& fileName,
				const UnifyFileCreateProv& createProv,
				const File& baseFile
				) :
				File(
				&fileName,
				nullptr,
				nullptr,
				nullptr,
				&createProv,
				nullptr,
				&baseFile
				) {}
			File(
				const TString& fileName,
				const UnifyAccessDesc& accessDesc,
				const UnifyFileSharePermit& shareMode,
				const UnifyFileCreateProv& createProv
				) :
				File(
				&fileName,
				&accessDesc,
				&shareMode,
				nullptr,
				&createProv,
				nullptr,
				nullptr
				) {}
			File(
				const TString& fileName,
				const UnifyAccessDesc& accessDesc,
				const UnifyFileSharePermit& shareMode,
				const SecurityAttrib& secAttr,
				const UnifyFileCreateProv& createProv,
				const UnifyFileAttrib& fileAttr
				) :
				File(
				&fileName,
				&accessDesc,
				&shareMode,
				&secAttr,
				&createProv,
				&fileAttr,
				nullptr
				) {}
			File(
				const TString& fileName,
				const UnifyAccessDesc& accessDesc,
				const UnifyFileSharePermit& shareMode,
				const SecurityAttrib& secAttr,
				const UnifyFileCreateProv& createProv,
				const UnifyFileAttrib& fileAttr,
				const File& baseFile
				) :
				File(
				&fileName,
				&accessDesc,
				&shareMode,
				&secAttr,
				&createProv,
				&fileAttr,
				&baseFile
				) {}
			
			//�����̒l���擾
			GeneralHandle& get() {
				return file_;
			}
			const GeneralHandle& get() const {
				return file_;
			}
			GeneralHandle& operator () () {
				return file_;
			}
			const GeneralHandle& operator () () const{
				return file_;
			}

		private:
			//�{��
			GeneralHandle file_ = nullptr;
			//SecurityAttrib�̕ۑ��p
			std::shared_ptr<SecurityAttrib> secAttr_ = nullptr;

		};

		//�A�v���P�[�V�����N���̂��߂̏��
		class StartupInfo {
		public:
			StartupInfo(
				const TString* desktopName = nullptr,
				const TString* wndTitle = nullptr,
				const Position* wndPos = nullptr,
				const Rectangle* wndSize = nullptr,
				const Rectangle* consoleBuf = nullptr,
				const UnifyConsoleStrColor* consoleStrColors = nullptr,
				const UnifyConsoleBgColor* consoleBgColors = nullptr,
				const UnifyStartupOption* startupOptions = nullptr,
				const UnifyWndShowMode* wndShowModes = nullptr,
				const File* stdInput = nullptr,
				const File* stdOutput = nullptr,
				const File* stdError = nullptr
				) {
				::ZeroMemory(&suInfo_, sizeof(suInfo_));

				//�T�C�Y�ݒ�
				suInfo_.cb = sizeof(::STARTUPINFO);
				//�f�X�N�g�b�v���w��
				if (desktopName != nullptr)
					myDesktopName_ = std::make_shared<TString>(*desktopName),
					suInfo_.lpDesktop = const_cast<TChar*>(myDesktopName_->c_str());
				//Window�̃^�C�g���w��
				if (wndTitle != nullptr)
					myTitle_ = std::make_shared<TString>(*wndTitle),
					suInfo_.lpTitle = const_cast<TChar*>(myTitle_->c_str());
				//Window�̍��W�ݒ�
				if (wndPos != nullptr)
					suInfo_.dwFlags |= STARTF_USEPOSITION,
					suInfo_.dwX = wndPos->x,
					suInfo_.dwY = wndPos->y;
				//Window�̑傫���ݒ�
				if (wndSize != nullptr)
					suInfo_.dwFlags |= STARTF_USESIZE,
					suInfo_.dwXSize = wndSize->x,
					suInfo_.dwYSize = wndSize->y;
				//Console�̃o�b�t�@�[�̑傫���ݒ�
				if (consoleBuf != nullptr)
					suInfo_.dwFlags |= STARTF_USECOUNTCHARS,
					suInfo_.dwXCountChars = consoleBuf->x,
					suInfo_.dwYCountChars = consoleBuf->y;
				//Console�̕����F�A�w�i�F�ݒ�
				if (consoleStrColors != nullptr)
					suInfo_.dwFlags |= STARTF_USEFILLATTRIBUTE,
					suInfo_.dwFillAttribute |= consoleStrColors->get();
				if (consoleBgColors != nullptr)
					suInfo_.dwFlags |= STARTF_USEFILLATTRIBUTE,
					suInfo_.dwFillAttribute |= consoleBgColors->get();
				if (startupOptions != nullptr)
					suInfo_.dwFlags |= startupOptions->get();
				//Window�\���`���̐ݒ�
				if (wndShowModes != nullptr)
					suInfo_.dwFlags |= STARTF_USESHOWWINDOW,
					suInfo_.wShowWindow = wndShowModes->get();
				//�W�����́A�o�́A�G���[�o�̓t�@�C���̐ݒ�
				if (stdInput != nullptr)
					suInfo_.dwFlags |= STARTF_USESTDHANDLES,
					suInfo_.hStdInput = stdInput->get();
				if (stdOutput != nullptr)
					suInfo_.dwFlags |= STARTF_USESTDHANDLES,
					suInfo_.hStdOutput = stdOutput->get();
				if (stdError != nullptr)
					suInfo_.dwFlags |= STARTF_USESTDHANDLES,
					suInfo_.hStdError = stdError->get();
			}
			StartupInfo(
				const Position& wndPos
				) :
				StartupInfo(
				nullptr,
				nullptr,
				&wndPos,
				nullptr,
				nullptr,
				nullptr,
				nullptr,
				nullptr,
				nullptr,
				nullptr,
				nullptr,
				nullptr
				) {}
			StartupInfo(
				const Position& wndPos,
				const Rectangle& wndSize
				) :
				StartupInfo(
				nullptr,
				nullptr,
				&wndPos,
				&wndSize,
				nullptr,
				nullptr,
				nullptr,
				nullptr,
				nullptr,
				nullptr,
				nullptr,
				nullptr
				) {}
			StartupInfo(
				const UnifyStartupOption& startupOptions,
				const UnifyWndShowMode& wndShowModes
				) :
				StartupInfo(
				nullptr,
				nullptr,
				nullptr,
				nullptr,
				nullptr,
				nullptr,
				nullptr,
				&startupOptions,
				&wndShowModes,
				nullptr,
				nullptr,
				nullptr
				) {}
			StartupInfo(
				const Rectangle& consoleBuf,
				const UnifyConsoleStrColor& consoleStrColors,
				const UnifyConsoleBgColor& consoleBgColors
				) :
				StartupInfo(
				nullptr,
				nullptr,
				nullptr,
				nullptr,
				&consoleBuf,
				&consoleStrColors,
				&consoleBgColors,
				nullptr,
				nullptr,
				nullptr,
				nullptr,
				nullptr
				) {}
			StartupInfo(
				const File& stdInput,
				const File& stdOutput,
				const File& stdError
				) :
				StartupInfo(
				nullptr,
				nullptr,
				nullptr,
				nullptr,
				nullptr,
				nullptr,
				nullptr,
				nullptr,
				nullptr,
				&stdInput,
				&stdOutput,
				&stdError
				) {}
			StartupInfo(
				const Rectangle& consoleBuf,
				const UnifyConsoleStrColor& consoleStrColors,
				const UnifyConsoleBgColor& consoleBgColors,
				const File& stdInput,
				const File& stdOutput,
				const File& stdError
				) :
				StartupInfo(
				nullptr,
				nullptr,
				nullptr,
				nullptr,
				&consoleBuf,
				&consoleStrColors,
				&consoleBgColors,
				nullptr,
				nullptr,
				&stdInput,
				&stdOutput,
				&stdError
				) {}
			StartupInfo(
				const TString& desktopName,
				const TString& wndTitle,
				const Position& wndPos,
				const Rectangle& wndSize,
				const Rectangle& consoleBuf,
				const UnifyConsoleStrColor& consoleStrColors,
				const UnifyConsoleBgColor& consoleBgColors,
				const UnifyStartupOption& startupOptions,
				const UnifyWndShowMode& wndShowModes,
				const File& stdInput,
				const File& stdOutput,
				const File& stdError
				) :
				StartupInfo(
				&desktopName,
				&wndTitle,
				&wndPos,
				&wndSize,
				&consoleBuf,
				&consoleStrColors,
				&consoleBgColors,
				&startupOptions,
				&wndShowModes,
				&stdInput,
				&stdOutput,
				&stdError
				) {}

			//����
			StartupInfo& operator |= (const StartupInfo& si) {
				//�f�X�N�g�b�v���w��
				if (suInfo_.lpDesktop == nullptr)
					//ToDo : lpDesktop��shared_ptr�Ŏ����ŕێ�
					myDesktopName_ = std::make_shared<TString>(si.get().lpDesktop),
					suInfo_.lpDesktop = const_cast<TChar*>(myDesktopName_->c_str());
				//Window�̃^�C�g���w��
				if (suInfo_.lpTitle == nullptr)
					myTitle_ = std::make_shared<TString>(si.get().lpTitle),
					suInfo_.lpTitle = const_cast<TChar*>(myTitle_->c_str());
				//Window�̍��W�ݒ�
				if ((suInfo_.dwFlags & STARTF_USEPOSITION) == 0)
					suInfo_.dwFlags |= STARTF_USEPOSITION,
					suInfo_.dwX = si.get().dwX,
					suInfo_.dwY = si.get().dwY;
				//Window�̑傫���ݒ�
				if ((suInfo_.dwFlags & STARTF_USESIZE) == 0)
					suInfo_.dwFlags |= STARTF_USESIZE,
					suInfo_.dwXSize = si.get().dwXSize,
					suInfo_.dwYSize = si.get().dwYSize;
				//Console�̃o�b�t�@�[�̑傫���ݒ�
				if ((suInfo_.dwFlags & STARTF_USECOUNTCHARS) == 0)
					suInfo_.dwFlags |= STARTF_USECOUNTCHARS,
					suInfo_.dwXCountChars = si.get().dwXCountChars,
					suInfo_.dwYCountChars = si.get().dwYCountChars;
				//Console�̕����F�A�w�i�F�ݒ�
				if ((suInfo_.dwFlags & STARTF_USEFILLATTRIBUTE) == 0)
					suInfo_.dwFlags |= STARTF_USEFILLATTRIBUTE,
					suInfo_.dwFillAttribute |= si.get().dwFillAttribute;
				//Window�\���`���̐ݒ�
				if ((suInfo_.dwFlags & STARTF_USESHOWWINDOW) == 0)
					suInfo_.dwFlags |= STARTF_USESHOWWINDOW,
					suInfo_.wShowWindow = si.get().wShowWindow;
				//�W�����́A�o�́A�G���[�o�̓t�@�C���̐ݒ�
				if ((suInfo_.dwFlags & STARTF_USESTDHANDLES) == 0)
					suInfo_.dwFlags |= STARTF_USESTDHANDLES,
					suInfo_.hStdInput = si.get().hStdInput,
					suInfo_.hStdOutput = si.get().hStdOutput,
					suInfo_.hStdError = si.get().hStdError;

				suInfo_.dwFlags |= si.get().dwFlags;
			}
			
			//�����̒l���擾
			::STARTUPINFO& get() {
				return suInfo_;
			}
			const ::STARTUPINFO& get() const {
				return suInfo_;
			}
			::STARTUPINFO& operator () () {
				return suInfo_;
			}
			const ::STARTUPINFO& operator () () const {
				return suInfo_;
			}

		private:
			//�{��
			::STARTUPINFO suInfo_;
			//������desktopName��title��ێ�
			std::shared_ptr<TString> myDesktopName_ = nullptr, myTitle_ = nullptr;
		};

		//ToDO : ProcessInfomation���b�p�[

	} //::wawl::fs
} //::wawl