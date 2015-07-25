#pragma once

//wawl Header
#include "BaseType.h"
//C++ STL
#include <memory>

namespace wawl {
	namespace fs {

		//�t�@�C���n���h��
		using FileHandle = ::HANDLE;
		//HANDLE�����^
		using _impl_UnderHandle = std::remove_pointer<::HANDLE>::type;

		//�Z�L�����e�B�w��q
		class SecurityDesc {
		public:
			SecurityDesc() {
				::InitializeSecurityDescriptor(&secDesc_, SECURITY_DESCRIPTOR_REVISION);
			}
			SecurityDesc(const SecurityDesc&) = default;

			//�����̒l���擾
			::SECURITY_DESCRIPTOR& get() {
				return secDesc_;
			}
			const ::SECURITY_DESCRIPTOR& get() const {
				return secDesc_;
			}
			::SECURITY_DESCRIPTOR& operator () () {
				return secDesc_;
			}
			const ::SECURITY_DESCRIPTOR& operator () () const{
				return secDesc_;
			}

		private:
			::SECURITY_DESCRIPTOR secDesc_;

		};
		//�Z�L�����e�B�L�q�q
		class SecurityAttrib{
		public:
			SecurityAttrib(const SecurityAttrib&) = default;

			SecurityAttrib(bool doInheritHandle, const SecurityDesc& secDesc) :
				mySecDesc_(std::make_shared<SecurityDesc>(secDesc)) {
				secAttr_.nLength = sizeof(::SECURITY_ATTRIBUTES);
				secAttr_.bInheritHandle = doInheritHandle;
				secAttr_.lpSecurityDescriptor = mySecDesc_.get();
			}
			explicit SecurityAttrib(bool doInheritHandle = false) {
				secAttr_.nLength = sizeof(::SECURITY_ATTRIBUTES);
				secAttr_.bInheritHandle = doInheritHandle;
				secAttr_.lpSecurityDescriptor = nullptr;
			}
			
			//�����̒l���擾
			::SECURITY_ATTRIBUTES& get() {
				return secAttr_;
			}
			const ::SECURITY_ATTRIBUTES& get() const{
				return secAttr_;
			}
			::SECURITY_ATTRIBUTES& operator () () {
				return secAttr_;
			}
			const ::SECURITY_ATTRIBUTES& operator () () const {
				return secAttr_;
			}

		private:
			//�{��
			::SECURITY_ATTRIBUTES secAttr_;

			std::shared_ptr<SecurityDesc> mySecDesc_;

		};

		//CUI�ł̕�����F
		enum class ConsoleStrColor : Dword {
			Blue = FOREGROUND_BLUE,
			Green = FOREGROUND_GREEN,
			Red = FOREGROUND_RED,
			Intensity = FOREGROUND_INTENSITY
		};
		using UnifyConsoleStrColor = _impl_UnifyEnum < ConsoleStrColor >;

		//CUI�ł̔w�i�F
		enum class ConsoleBgColor : Dword{
			Blue = BACKGROUND_BLUE,
			Green = BACKGROUND_GREEN,
			Red = BACKGROUND_RED,
			Intensity = BACKGROUND_INTENSITY
		};
		using UnifyConsoleBgColor = _impl_UnifyEnum < ConsoleBgColor >;

		//�A�v���P�[�V�����N�����̃I�v�V����
		enum class StartupOption : Dword {
			Fullscreen = STARTF_RUNFULLSCREEN,
			ForceChangeCursor = STARTF_FORCEONFEEDBACK,
			ForceImmutCursor = STARTF_FORCEOFFFEEDBACK,
			NoPinningTaskbar = STARTF_PREVENTPINNING || STARTF_TITLEISAPPID,
			RelateTitleWithAppID = STARTF_TITLEISAPPID,
			RelateTitleWithLnk = STARTF_TITLEISLINKNAME
		};
		using UnifyStartupOption = _impl_UnifyEnum < StartupOption >;

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
			File(File&&) = default;

			File(
				const TString* fileName,
				const UnifyAccessDesc* accessDesc,
				const UnifyFileSharePermit* shareMode,
				const SecurityAttrib* secAttr,
				const UnifyFileCreateProv* createProv,
				const UnifyFileAttrib* fileAttr,
				const File* baseFile
				) {
				if (secAttr != nullptr)
					mySecAttr_ = std::make_shared<SecurityAttrib>(*secAttr);
				file_ = std::shared_ptr<_impl_UnderHandle>(
					::CreateFile(
					(fileName == nullptr ? nullptr : fileName->c_str()),
					(accessDesc == nullptr ? GENERIC_ALL : accessDesc->get()),
					(shareMode == nullptr ? NULL : shareMode->get()),
					(secAttr == nullptr ? nullptr : &(mySecAttr_->get())),
					(createProv == nullptr ? CREATE_ALWAYS : createProv->get()),
					(fileAttr == nullptr ? FILE_ATTRIBUTE_NORMAL : fileAttr->get()),
					(baseFile == nullptr ? nullptr : baseFile->get())
					),
					::CloseHandle
					);

				if (file_.get() == INVALID_HANDLE_VALUE)
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
			FileHandle get() const {
				return file_.get();
			}
			FileHandle operator () () const {
				return file_.get();
			}
			
		private:
			//�{��
			std::shared_ptr<_impl_UnderHandle> file_ = nullptr;
			//SecurityAttrib�̕ۑ�
			std::shared_ptr<SecurityAttrib> mySecAttr_ = nullptr;

		};

		//�A�v���P�[�V�����N���̂��߂̏��
		class StartupInfo {
			friend StartupInfo;
		public:
			StartupInfo(const StartupInfo&) = default;
			StartupInfo(StartupInfo&&) = default;

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
					myDesktopName_ = *desktopName,
					suInfo_.lpDesktop = const_cast<TChar*>(myDesktopName_.c_str());
				//Window�̃^�C�g���w��
				if (wndTitle != nullptr)
					myTitle_ = *wndTitle,
					suInfo_.lpTitle = const_cast<TChar*>(myTitle_.c_str());
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
					myInput_ = *stdInput,
					suInfo_.dwFlags |= STARTF_USESTDHANDLES,
					suInfo_.hStdInput = myInput_.get();
				if (stdOutput != nullptr)
					myOutput_ = *stdOutput,
					suInfo_.dwFlags |= STARTF_USESTDHANDLES,
					suInfo_.hStdOutput = myOutput_.get();
				if (stdError != nullptr)
					myError_ = *stdError,
					suInfo_.dwFlags |= STARTF_USESTDHANDLES,
					suInfo_.hStdError = myError_.get();
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
				if (suInfo_.lpDesktop == nullptr &&
					si.get().lpDesktop != nullptr)
					myDesktopName_ = si.get().lpDesktop,
					suInfo_.lpDesktop = const_cast<TChar*>(myDesktopName_.c_str());
				//Window�̃^�C�g���w��
				if (suInfo_.lpTitle == nullptr &&
					si.get().lpTitle != nullptr)
					myTitle_ = si.get().lpTitle,
					suInfo_.lpTitle = const_cast<TChar*>(myTitle_.c_str());
				//Window�̍��W�ݒ�
				if ((suInfo_.dwFlags & STARTF_USEPOSITION) == 0 &&
					(si.get().dwFlags & STARTF_USEPOSITION) != 0)
					suInfo_.dwFlags |= STARTF_USEPOSITION,
					suInfo_.dwX = si.get().dwX,
					suInfo_.dwY = si.get().dwY;
				//Window�̑傫���ݒ�
				if ((suInfo_.dwFlags & STARTF_USESIZE) == 0 &&
					(si.get().dwFlags & STARTF_USESIZE) != 0)
					suInfo_.dwFlags |= STARTF_USESIZE,
					suInfo_.dwXSize = si.get().dwXSize,
					suInfo_.dwYSize = si.get().dwYSize;
				//Console�̃o�b�t�@�[�̑傫���ݒ�
				if ((suInfo_.dwFlags & STARTF_USECOUNTCHARS) == 0 &&
					(si.get().dwFlags & STARTF_USECOUNTCHARS) != 0)
					suInfo_.dwFlags |= STARTF_USECOUNTCHARS,
					suInfo_.dwXCountChars = si.get().dwXCountChars,
					suInfo_.dwYCountChars = si.get().dwYCountChars;
				//Console�̕����F�A�w�i�F�ݒ�
				if ((suInfo_.dwFlags & STARTF_USEFILLATTRIBUTE) == 0 &&
					(si.get().dwFlags & STARTF_USEFILLATTRIBUTE) != 0)
					suInfo_.dwFlags |= STARTF_USEFILLATTRIBUTE,
					suInfo_.dwFillAttribute |= si.get().dwFillAttribute;
				//Window�\���`���̐ݒ�
				if ((suInfo_.dwFlags & STARTF_USESHOWWINDOW) == 0 &&
					(si.get().dwFlags & STARTF_USESHOWWINDOW) != 0)
					suInfo_.dwFlags |= STARTF_USESHOWWINDOW,
					suInfo_.wShowWindow = si.get().wShowWindow;
				//�W�����́A�o�́A�G���[�o�̓t�@�C���̐ݒ�
				if ((suInfo_.dwFlags & STARTF_USESTDHANDLES) == 0 &&
					(si.get().dwFlags & STARTF_USESTDHANDLES) != 0)
					myInput_ = si.myInput_,
					myOutput_ = si.myOutput_,
					myError_ = si.myError_,
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
			//�����̈ꕔ�̃����o��ێ�
			TString myDesktopName_ = nullptr, myTitle_ = nullptr;
			File myInput_{}, myOutput_{}, myError_{};
		};

		//�v���Z�X���
		class ProcessInfo {
		public:
			ProcessInfo() {
				procInfo_.dwProcessId = 0;
				procInfo_.dwThreadId = 0;
				procInfo_.hProcess = nullptr;
				procInfo_.hThread = nullptr;
			}
			ProcessInfo(const ProcessInfo&) = default;
			ProcessInfo(ProcessInfo&&) = default;

			::PROCESS_INFORMATION& get() {
				return procInfo_;
			}
			const ::PROCESS_INFORMATION& get() const {
				return procInfo_;
			}
			::PROCESS_INFORMATION& operator () () {
				return procInfo_;
			}
			const ::PROCESS_INFORMATION& operator () () const {
				return procInfo_;
			}

		private:
			::PROCESS_INFORMATION procInfo_;
			
		};

		//ToDo : Process�N���X�ǉ�
		class Process {
		public:

		private:

		};

	} //::wawl::fs
} //::wawl