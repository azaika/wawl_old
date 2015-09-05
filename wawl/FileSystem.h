#pragma once

//wawl Header
#include "BaseType.h"
//C++ STL
#include <memory>

namespace wawl {
	namespace fs {

		//ファイルハンドル
		using FileHandle = ::HANDLE;
		//HANDLE内部型
		using _impl_UnderHandle = std::remove_pointer<::HANDLE>::type;

		//セキュリティ指定子
		class SecurityDesc {
		public:
			SecurityDesc() {
				::InitializeSecurityDescriptor(&secDesc_, SECURITY_DESCRIPTOR_REVISION);
			}
			SecurityDesc(const SecurityDesc&) = default;
			SecurityDesc(SecurityDesc&&) = default;
			SecurityDesc& operator = (const SecurityDesc&) = default;
			SecurityDesc& operator = (SecurityDesc&&) = default;

			//内部の値を取得
			::SECURITY_DESCRIPTOR& get() {
				return secDesc_;
			}
			const ::SECURITY_DESCRIPTOR& get() const {
				return secDesc_;
			}
			::SECURITY_DESCRIPTOR& operator () () {
				return secDesc_;
			}
			const ::SECURITY_DESCRIPTOR& operator () () const {
				return secDesc_;
			}

		private:
			::SECURITY_DESCRIPTOR secDesc_;

		};
		//セキュリティ記述子
		class SecurityAttrib {
			friend SecurityAttrib;

		public:
			SecurityAttrib(SecurityAttrib&&) = default;
			SecurityAttrib& operator = (SecurityAttrib&&) = default;

			SecurityAttrib(bool doInheritHandle, const SecurityDesc& secDesc) :
				secDesc_(std::make_unique<SecurityDesc>(secDesc)) {
				secAttr_.nLength = sizeof(::SECURITY_ATTRIBUTES);
				secAttr_.bInheritHandle = doInheritHandle;
				secAttr_.lpSecurityDescriptor = &secDesc_->get();
			}
			explicit SecurityAttrib(bool doInheritHandle = false) {
				secAttr_.nLength = sizeof(::SECURITY_ATTRIBUTES);
				secAttr_.bInheritHandle = doInheritHandle;
				secAttr_.lpSecurityDescriptor = nullptr;
			}
			SecurityAttrib(const SecurityAttrib& val) {
				secAttr_.nLength = sizeof(::SECURITY_ATTRIBUTES);
				secAttr_.bInheritHandle = val.get().bInheritHandle;

				if (val.secDesc_ != nullptr) {
					secDesc_ = std::make_unique<SecurityDesc>(*val.secDesc_);
					secAttr_.lpSecurityDescriptor = secDesc_.get();
				}
				else
					secAttr_.lpSecurityDescriptor = nullptr;
			}
			
			//内部の値を取得
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
			//本体
			::SECURITY_ATTRIBUTES secAttr_;
			//自分のSecurityDesc
			std::unique_ptr<SecurityDesc> secDesc_ = nullptr;

		};

		//コンソールの色
		enum class ConsoleColor : Dword {
			Blue = BACKGROUND_BLUE,
			Green = BACKGROUND_GREEN,
			Red = BACKGROUND_RED,
			Intensity = BACKGROUND_INTENSITY
		};
		using UnifyConsoleColor = _impl_UnifyEnum < ConsoleColor >;

		//アプリケーション起動時のオプション
		enum class StartupOption : Dword {
			Fullscreen = STARTF_RUNFULLSCREEN,
			ForceChangeCursor = STARTF_FORCEONFEEDBACK,
			ForceImmutCursor = STARTF_FORCEOFFFEEDBACK,
			NoPinningTaskbar = STARTF_PREVENTPINNING || STARTF_TITLEISAPPID,
			RelateTitleWithAppID = STARTF_TITLEISAPPID,
			RelateTitleWithLnk = STARTF_TITLEISLINKNAME
		};
		using UnifyStartupOption = _impl_UnifyEnum < StartupOption >;

		//アクセス指定子
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

		//ファイルシェア形式
		enum class FileSharePermit : Dword {
			None = NULL,
			Delete = FILE_SHARE_DELETE,
			Read = FILE_SHARE_READ,
			Write = FILE_SHARE_WRITE
		};
		using UnifyFileSharePermit = _impl_UnifyEnum < FileSharePermit >;

		//ファイル生成規定
		enum class FileCreateProv : Dword {
			New = CREATE_NEW,
			AlwaysNew = CREATE_ALWAYS,
			OpenExisting = OPEN_EXISTING,
			AlwaysOpen = OPEN_ALWAYS,
			ClearExisting = TRUNCATE_EXISTING
		};
		using UnifyFileCreateProv = _impl_UnifyEnum < FileCreateProv >;

		//ファイルの属性記述子
		enum class FileAttrib : Dword {
			Archive = FILE_ATTRIBUTE_ARCHIVE,
			Encrypt = FILE_ATTRIBUTE_ENCRYPTED,
			Hide = FILE_ATTRIBUTE_HIDDEN,
			Normal = FILE_ATTRIBUTE_NORMAL,
			NotIndexed = FILE_ATTRIBUTE_NOT_CONTENT_INDEXED,
			Offline = FILE_ATTRIBUTE_OFFLINE,
			ReadOnly = FILE_ATTRIBUTE_READONLY,
			System = FILE_ATTRIBUTE_SYSTEM,
			Temporary = FILE_ATTRIBUTE_TEMPORARY,

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

		//ファイル
		class File {
		public:
			File() = default;
			File(const File&) = default;
			File(File&&) = default;
			File& operator = (const File&) = default;
			File& operator = (File&&) = default;

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
					[](::HANDLE h) { ::CloseHandle(h), delete h; }
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
			
			//内部の値を取得
			FileHandle get() const {
				return file_.get();
			}
			FileHandle operator () () const {
				return file_.get();
			}
			
		private:
			//本体
			std::shared_ptr<_impl_UnderHandle> file_ = nullptr;
			//一部引数の保存
			std::shared_ptr<SecurityAttrib> mySecAttr_ = nullptr;

		};

		//アプリケーション起動のための情報
		class StartupInfo {
			friend StartupInfo;
		public:
			StartupInfo(const StartupInfo&) = default;
			StartupInfo(StartupInfo&&) = default;
			StartupInfo& operator = (const StartupInfo&) = default;
			StartupInfo& operator = (StartupInfo&&) = default;

			StartupInfo(
				const TString* desktopName = nullptr,
				const TString* wndTitle = nullptr,
				const Position* wndPos = nullptr,
				const Rectangle* wndSize = nullptr,
				const Rectangle* consoleBuf = nullptr,
				const UnifyConsoleColor* consoleStrColors = nullptr,
				const UnifyConsoleColor* consoleBgColors = nullptr,
				const UnifyStartupOption* startupOptions = nullptr,
				const UnifyWndShowMode* wndShowModes = nullptr,
				const File* stdInput = nullptr,
				const File* stdOutput = nullptr,
				const File* stdError = nullptr
				) {
				::ZeroMemory(&suInfo_, sizeof(suInfo_));

				//サイズ設定
				suInfo_.cb = sizeof(::STARTUPINFO);
				//デスクトップ名指定
				if (desktopName != nullptr)
					myDesktopName_ = *desktopName,
					suInfo_.lpDesktop = const_cast<TChar*>(myDesktopName_.c_str());
				//Windowのタイトル指定
				if (wndTitle != nullptr)
					myTitle_ = *wndTitle,
					suInfo_.lpTitle = const_cast<TChar*>(myTitle_.c_str());
				//Windowの座標設定
				if (wndPos != nullptr)
					suInfo_.dwFlags |= STARTF_USEPOSITION,
					suInfo_.dwX = wndPos->x,
					suInfo_.dwY = wndPos->y;
				//Windowの大きさ設定
				if (wndSize != nullptr)
					suInfo_.dwFlags |= STARTF_USESIZE,
					suInfo_.dwXSize = wndSize->x,
					suInfo_.dwYSize = wndSize->y;
				//Consoleのバッファーの大きさ設定
				if (consoleBuf != nullptr)
					suInfo_.dwFlags |= STARTF_USECOUNTCHARS,
					suInfo_.dwXCountChars = consoleBuf->x,
					suInfo_.dwYCountChars = consoleBuf->y;
				//Consoleの文字色、背景色設定
				if (consoleStrColors != nullptr)
					suInfo_.dwFlags |= STARTF_USEFILLATTRIBUTE,
					suInfo_.dwFillAttribute |= consoleStrColors->get();
				if (consoleBgColors != nullptr)
					suInfo_.dwFlags |= STARTF_USEFILLATTRIBUTE,
					suInfo_.dwFillAttribute |= consoleBgColors->get();
				if (startupOptions != nullptr)
					suInfo_.dwFlags |= startupOptions->get();
				//Window表示形式の設定
				if (wndShowModes != nullptr)
					suInfo_.dwFlags |= STARTF_USESHOWWINDOW,
					suInfo_.wShowWindow = wndShowModes->get();
				//標準入力、出力、エラー出力ファイルの設定
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
				const UnifyConsoleColor& consoleStrColors,
				const UnifyConsoleColor& consoleBgColors
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
				const UnifyConsoleColor& consoleStrColors,
				const UnifyConsoleColor& consoleBgColors,
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
				const UnifyConsoleColor& consoleStrColors,
				const UnifyConsoleColor& consoleBgColors,
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

			//合成
			StartupInfo& operator |= (const StartupInfo& si) {
				//デスクトップ名指定
				if (suInfo_.lpDesktop == nullptr &&
					si.get().lpDesktop != nullptr)
					myDesktopName_ = si.get().lpDesktop,
					suInfo_.lpDesktop = const_cast<TChar*>(myDesktopName_.c_str());
				//Windowのタイトル指定
				if (suInfo_.lpTitle == nullptr &&
					si.get().lpTitle != nullptr)
					myTitle_ = si.get().lpTitle,
					suInfo_.lpTitle = const_cast<TChar*>(myTitle_.c_str());
				//Windowの座標設定
				if ((suInfo_.dwFlags & STARTF_USEPOSITION) == 0 &&
					(si.get().dwFlags & STARTF_USEPOSITION) != 0)
					suInfo_.dwFlags |= STARTF_USEPOSITION,
					suInfo_.dwX = si.get().dwX,
					suInfo_.dwY = si.get().dwY;
				//Windowの大きさ設定
				if ((suInfo_.dwFlags & STARTF_USESIZE) == 0 &&
					(si.get().dwFlags & STARTF_USESIZE) != 0)
					suInfo_.dwFlags |= STARTF_USESIZE,
					suInfo_.dwXSize = si.get().dwXSize,
					suInfo_.dwYSize = si.get().dwYSize;
				//Consoleのバッファーの大きさ設定
				if ((suInfo_.dwFlags & STARTF_USECOUNTCHARS) == 0 &&
					(si.get().dwFlags & STARTF_USECOUNTCHARS) != 0)
					suInfo_.dwFlags |= STARTF_USECOUNTCHARS,
					suInfo_.dwXCountChars = si.get().dwXCountChars,
					suInfo_.dwYCountChars = si.get().dwYCountChars;
				//Consoleの文字色、背景色設定
				if ((suInfo_.dwFlags & STARTF_USEFILLATTRIBUTE) == 0 &&
					(si.get().dwFlags & STARTF_USEFILLATTRIBUTE) != 0)
					suInfo_.dwFlags |= STARTF_USEFILLATTRIBUTE,
					suInfo_.dwFillAttribute |= si.get().dwFillAttribute;
				//Window表示形式の設定
				if ((suInfo_.dwFlags & STARTF_USESHOWWINDOW) == 0 &&
					(si.get().dwFlags & STARTF_USESHOWWINDOW) != 0)
					suInfo_.dwFlags |= STARTF_USESHOWWINDOW,
					suInfo_.wShowWindow = si.get().wShowWindow;
				//標準入力、出力、エラー出力ファイルの設定
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
			
			//内部の値を取得
			auto& get() {
				return suInfo_;
			}
			const ::STARTUPINFO& get() const {
				return suInfo_;
			}
			auto& operator () () {
				return suInfo_;
			}
			const auto& operator () () const {
				return suInfo_;
			}

		private:
			//本体
			::STARTUPINFO suInfo_;
			//自分の一部のメンバを保持
			TString myDesktopName_, myTitle_;
			File myInput_{}, myOutput_{}, myError_{};
		};

		//プロセス情報
		class ProcInfo {
		public:
			ProcInfo() {
				procInfo_->dwProcessId = 0;
				procInfo_->dwThreadId = 0;
				procInfo_->hProcess = nullptr;
				procInfo_->hThread = nullptr;
			}
			ProcInfo(const ProcInfo&) = default;
			ProcInfo(ProcInfo&&) = default;
			ProcInfo& operator = (const ProcInfo&) = default;
			ProcInfo& operator = (ProcInfo&&) = default;

			auto& get() {
				return *procInfo_;
			}
			const auto& get() const {
				return *procInfo_;
			}
			auto& operator () () {
				return *procInfo_;
			}
			const auto& operator () () const {
				return *procInfo_;
			}
			//shared_ptrを返す
			auto& getPtr() {
				return procInfo_;
			}
			const auto& getPtr() const{
				return procInfo_;
			}
			
		private:
			std::shared_ptr<::PROCESS_INFORMATION> procInfo_ = std::make_shared<::PROCESS_INFORMATION>();
			
		};

		//プロセス生成規定
		enum class ProcCreateProv : Dword {
			NotInheritJob = CREATE_BREAKAWAY_FROM_JOB,
			NotInheritErrorMode = CREATE_DEFAULT_ERROR_MODE,
			DosMode = CREATE_FORCEDOS,
			CreateNewConsole = CREATE_NEW_CONSOLE,
			CreateNewProcGroup = CREATE_NEW_PROCESS_GROUP,
			NotCreateWindow = CREATE_NO_WINDOW,
			VdmMode = CREATE_SEPARATE_WOW_VDM,
			SharedVdmMode = CREATE_SHARED_WOW_VDM,
			SuspendPrimary = CREATE_SUSPENDED,
			DebugMode = DEBUG_PROCESS,
			NotInheritDebugMode = DEBUG_ONLY_THIS_PROCESS,
			DisallowAccessConsole = DETACHED_PROCESS,

			PriorityLv1 = IDLE_PRIORITY_CLASS,
			PriorityLv2 = BELOW_NORMAL_PRIORITY_CLASS,
			PriorityLv3 = NORMAL_PRIORITY_CLASS,
			PriorityLv4 = ABOVE_NORMAL_PRIORITY_CLASS,
			PriorityLv5 = HIGH_PRIORITY_CLASS,
			MaxPriority = REALTIME_PRIORITY_CLASS
		};
		using UnifyProcCreateProv = _impl_UnifyEnum<ProcCreateProv>;

		class Process {
		public:
			Process() = default;
			Process(const Process&) = default;
			Process& operator = (const Process&) = default;
			Process(Process&&) = default;
			Process& operator = (Process&&) = default;

			Process(
				const TString* appName,
				const TString* cmdLineArgs,
				const SecurityAttrib* procAttrib,
				const SecurityAttrib* threadAttrib,
				bool doInheritHandle,
				const UnifyProcCreateProv* createProv,
				const TString* envVars,
				const TString* currentDir,
				const StartupInfo* startupInfo
				) {
				if (procAttrib != nullptr)
					myProcAttrib_ = std::make_shared<SecurityAttrib>(*procAttrib);
				if (threadAttrib != nullptr)
					myThreadAttrib_ = std::make_shared<SecurityAttrib>(*threadAttrib);
				if (startupInfo != nullptr)
					myStartupInfo_ = std::make_shared<StartupInfo>(*startupInfo);

				::PROCESS_INFORMATION* tmpProcInfo = new ::PROCESS_INFORMATION();

				if (
					::CreateProcess(
						(appName == nullptr ? nullptr : appName->c_str()),
						(cmdLineArgs == nullptr ? nullptr : const_cast<TChar*>(cmdLineArgs->c_str())),
						(procAttrib == nullptr ? nullptr : &myProcAttrib_->get()),
						(threadAttrib == nullptr ? nullptr : &myThreadAttrib_->get()),
						doInheritHandle,
						(createProv == nullptr ? NORMAL_PRIORITY_CLASS : createProv->get()),
						(envVars == nullptr ? nullptr : reinterpret_cast<void*>(const_cast<TChar*>(envVars->c_str()))),
						(currentDir == nullptr ? nullptr : currentDir->c_str()),
						(startupInfo == nullptr ? nullptr : &myStartupInfo_->get()),
						tmpProcInfo
						) == 0
					)
					delete tmpProcInfo,
					throw std::runtime_error("Failed to CreateProcess.");

				procInfo_.getPtr() = std::shared_ptr<::PROCESS_INFORMATION>{
					tmpProcInfo,
					[](::PROCESS_INFORMATION* pi) {
					if (pi != nullptr)
						::CloseHandle(pi->hProcess),
						::CloseHandle(pi->hThread),
						delete pi;
				} };
			}
			Process(
				const TString& cmdLine,
				const StartupInfo& startupInfo
				) :
				Process(
					nullptr,
					&cmdLine,
					nullptr,
					nullptr,
					false,
					nullptr,
					nullptr,
					nullptr,
					&startupInfo
					) {}
			Process(
				const TString& cmdLine,
				const UnifyProcCreateProv& createProv,
				const StartupInfo& startupInfo
				) :
				Process(
					nullptr,
					&cmdLine,
					nullptr,
					nullptr,
					false,
					&createProv,
					nullptr,
					nullptr,
					&startupInfo
					) {}
			Process(
				const TString& appName,
				const TString& cmdLineArgs,
				const UnifyProcCreateProv& createProv,
				const TString& currentDir,
				const StartupInfo& startupInfo
				) :
				Process(
					&appName,
					&cmdLineArgs,
					nullptr,
					nullptr,
					false,
					&createProv,
					nullptr,
					&currentDir,
					&startupInfo
					) {}
			Process(
				const TString& appName,
				const TString& cmdLineArgs,
				const SecurityAttrib& procAttrib,
				const SecurityAttrib& threadAttrib,
				bool doInheritHandle,
				const UnifyProcCreateProv& createProv,
				const TString& envVars,
				const TString& currentDir,
				const StartupInfo& startupInfo
				) :
				Process(
					&appName,
					&cmdLineArgs,
					&procAttrib,
					&threadAttrib,
					doInheritHandle,
					&createProv,
					&envVars,
					&currentDir,
					&startupInfo
					) {}

		private:
			//プロセス状況
			ProcInfo procInfo_;
			//一部引数の保存
			std::shared_ptr<SecurityAttrib> myProcAttrib_ = nullptr, myThreadAttrib_ = nullptr;
			std::shared_ptr<StartupInfo> myStartupInfo_ = nullptr;
			
		};

	} //::wawl::fs
} //::wawl