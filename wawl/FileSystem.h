#pragma once

#define WAWL_ENABLE_FILESYSTEM

//wawl Header
#include "BaseType.h"
//C++ STL
#include <fstream>
#include <algorithm>
#include <cctype>
#include <cstdio>
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
		public:
			SecurityAttrib(SecurityAttrib&&) = default;
			SecurityAttrib& operator = (SecurityAttrib&&) = default;

			SecurityAttrib() :
				SecurityAttrib(false) {}

			SecurityAttrib(bool doInheritHandle, const SecurityDesc& secDesc) :
				secDesc_(std::make_unique<SecurityDesc>(secDesc)) {
				secAttr_.nLength = sizeof(::SECURITY_ATTRIBUTES);
				secAttr_.bInheritHandle = doInheritHandle;
				secAttr_.lpSecurityDescriptor = &secDesc_->get();
			}
			explicit SecurityAttrib(bool doInheritHandle) {
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

			SecurityAttrib& operator = (const SecurityAttrib& val) {
				secAttr_.nLength = sizeof(::SECURITY_ATTRIBUTES);
				secAttr_.bInheritHandle = val.get().bInheritHandle;

				if (val.secDesc_ != nullptr) {
					secDesc_ = std::make_unique<SecurityDesc>(*val.secDesc_);
					secAttr_.lpSecurityDescriptor = secDesc_.get();
				}
				else
					secAttr_.lpSecurityDescriptor = nullptr;

				return *this;
			}

			//内部の値を取得
			::SECURITY_ATTRIBUTES& get() {
				return secAttr_;
			}
			const ::SECURITY_ATTRIBUTES& get() const {
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

		//アプリケーション起動時のオプション
		enum class StartupOption : Dword {
			Fullscreen = STARTF_RUNFULLSCREEN,
			ForceChangeCursor = STARTF_FORCEONFEEDBACK,
			ForceImmutCursor = STARTF_FORCEOFFFEEDBACK,
			NoPinningTaskbar = STARTF_PREVENTPINNING || STARTF_TITLEISAPPID,
			RelateTitleWithAppID = STARTF_TITLEISAPPID,
			RelateTitleWithLnk = STARTF_TITLEISLINKNAME
		};

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

		//ファイルシェア形式
		enum class FileSharePermit : Dword {
			None = NULL,
			Delete = FILE_SHARE_DELETE,
			Read = FILE_SHARE_READ,
			Write = FILE_SHARE_WRITE
		};

		//ファイル生成規定
		enum class FileCreateProv : Dword {
			New = CREATE_NEW,
			AlwaysNew = CREATE_ALWAYS,
			OpenExisting = OPEN_EXISTING,
			AlwaysOpen = OPEN_ALWAYS,
			ClearExisting = TRUNCATE_EXISTING
		};

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

		//ファイル
		class File {
		public:
			File() = default;
			File(const File&) = default;
			File(File&&) = default;
			File& operator = (const File&) = default;
			File& operator = (File&&) = default;

			File(
				const TString& fileName,
				const UnifyEnum<FileCreateProv>& createProv
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
				const UnifyEnum<FileCreateProv>& createProv,
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
				const UnifyEnum<AccessDesc>& accessDesc,
				const UnifyEnum<FileSharePermit>& shareMode,
				const UnifyEnum<FileCreateProv>& createProv
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
				const UnifyEnum<AccessDesc>& accessDesc,
				const UnifyEnum<FileSharePermit>& shareMode,
				const SecurityAttrib& secAttr,
				const UnifyEnum<FileCreateProv>& createProv,
				const UnifyEnum<FileAttrib>& fileAttr
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
				const UnifyEnum<AccessDesc>& accessDesc,
				const UnifyEnum<FileSharePermit>& shareMode,
				const SecurityAttrib& secAttr,
				const UnifyEnum<FileCreateProv>& createProv,
				const UnifyEnum<FileAttrib>& fileAttr,
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

			bool open(
				const TString& fileName,
				const UnifyEnum<FileCreateProv>& createProv
				) {
				*this = File(
					&fileName,
					nullptr,
					nullptr,
					nullptr,
					&createProv,
					nullptr,
					nullptr
					);

				return file_ != nullptr;
			}

			//TODO: openのオーバーロード追加

			void close() {
				file_ = nullptr;
			}

			//内部の値を取得
			FileHandle get() const {
				return *file_;
			}
			FileHandle operator () () const {
				return *file_;
			}

		private:
			//本体
			std::shared_ptr<FileHandle> file_ = nullptr;
			//一部引数の保存
			std::shared_ptr<SecurityAttrib> mySecAttr_ = nullptr;

			//ルートコンストラクタ
			File(
				const TString* fileName,
				const UnifyEnum<AccessDesc>* accessDesc,
				const UnifyEnum<FileSharePermit>* shareMode,
				const SecurityAttrib* secAttr,
				const UnifyEnum<FileCreateProv>* createProv,
				const UnifyEnum<FileAttrib>* fileAttr,
				const File* baseFile
				) {
				if (secAttr != nullptr)
					mySecAttr_ = std::make_shared<SecurityAttrib>(*secAttr);
				file_ = std::shared_ptr<FileHandle>(
					new FileHandle(
					::CreateFile(
					(fileName == nullptr ? nullptr : fileName->c_str()),
					(accessDesc == nullptr ? GENERIC_ALL : accessDesc->get()),
					(shareMode == nullptr ? NULL : shareMode->get()),
					(secAttr == nullptr ? nullptr : &(mySecAttr_->get())),
					(createProv == nullptr ? CREATE_ALWAYS : createProv->get()),
					(fileAttr == nullptr ? FILE_ATTRIBUTE_NORMAL : fileAttr->get()),
					(baseFile == nullptr ? nullptr : baseFile->get())
					)),
					[](::HANDLE* h) {
						if (h != nullptr)
							::CloseHandle(*h), delete h;
					}
				);

				if (*file_ == INVALID_HANDLE_VALUE)
					throw Error(::GetLastError());
			}

		};

		//アプリケーション起動のための情報
		class StartupInfo {
		public:
			StartupInfo(const StartupInfo&) = default;
			StartupInfo(StartupInfo&&) = default;
			StartupInfo& operator = (const StartupInfo&) = default;
			StartupInfo& operator = (StartupInfo&&) = default;

			StartupInfo() :
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
					nullptr,
					nullptr,
					nullptr
				) {}
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
				const UnifyEnum<StartupOption>& startupOptions,
				const UnifyEnum<WndShowMode>& wndShowModes
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
				const UnifyEnum<ConsoleColor>& consoleStrColors,
				const UnifyEnum<ConsoleColor>& consoleBgColors
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
				const UnifyEnum<ConsoleColor>& consoleStrColors,
				const UnifyEnum<ConsoleColor>& consoleBgColors,
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
				const UnifyEnum<ConsoleColor>& consoleStrColors,
				const UnifyEnum<ConsoleColor>& consoleBgColors,
				const UnifyEnum<StartupOption>& startupOptions,
				const UnifyEnum<WndShowMode>& wndShowModes,
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

			//ルートコンストラクタ
			StartupInfo(
				const TString* desktopName,
				const TString* wndTitle,
				const Position* wndPos,
				const Rectangle* wndSize,
				const Rectangle* consoleBuf,
				const UnifyEnum<ConsoleColor>* consoleStrColors,
				const UnifyEnum<ConsoleColor>* consoleBgColors,
				const UnifyEnum<StartupOption>* startupOptions,
				const UnifyEnum<WndShowMode>* wndShowModes,
				const File* stdInput,
				const File* stdOutput,
				const File* stdError
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
		};

		//プロセス情報
		struct ProcInfo {
			Handle procHandle = 0, threadHandle = 0;
			Dword procId = 0, threadId = 0;

			ProcInfo() = default;
			ProcInfo(const ProcInfo&) = default;
			ProcInfo(ProcInfo&&) = default;
			ProcInfo& operator = (const ProcInfo&) = default;
			ProcInfo& operator = (ProcInfo&&) = default;

			ProcInfo(const ::PROCESS_INFORMATION& pi) {
				procId = pi.dwProcessId;
				threadId = pi.dwThreadId;
				procHandle = pi.hProcess;
				threadHandle = pi.hThread;
			}

			operator ::PROCESS_INFORMATION() {
				return *reinterpret_cast<PROCESS_INFORMATION*>(this);
			}

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

		class Process {
		public:
			static constexpr Dword Running = STILL_ACTIVE;

			Process() = default;
			Process(Process&&) = default;
			Process& operator = (Process&&) = default;

			Process(
				const TString& cmdLine
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
				nullptr
				) {}
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
				const UnifyEnum<ProcCreateProv>& createProv,
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
				const UnifyEnum<ProcCreateProv>& createProv,
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
				const UnifyEnum<ProcCreateProv>& createProv,
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

			void open(
				const TString& cmdLine
				) {
				Process proc(
					cmdLine
					);
				procInfo_ = ValType(proc.procInfo_.release(), releaseInfo);
				procAttrib_ = proc.procAttrib_;
				threadAttrib_ = proc.threadAttrib_;
				startupInfo_ = proc.startupInfo_;
				error_ = proc.error_;
			}
			void open(
				const TString& cmdLine,
				const StartupInfo& startupInfo
				) {
				Process proc(
					cmdLine,
					startupInfo
					);
				procInfo_ = ValType(proc.procInfo_.release(), releaseInfo);
				procAttrib_ = proc.procAttrib_;
				threadAttrib_ = proc.threadAttrib_;
				startupInfo_ = proc.startupInfo_;
				error_ = proc.error_;
			}

			//TODO: openのオーバーロード追加

			void release(unsigned int exitCode = 0) {
				procInfo_.reset();
			}

			bool terminate(unsigned int exitCode = 0) {
				if (procInfo_) {
					bool isSuccess = ::TerminateProcess(procInfo_->procHandle, exitCode) != 0;

					procInfo_.reset();

					return isSuccess;
				}

				return false;
			}

			Dword getExitCode() const {
				Dword ret;
				::GetExitCodeProcess(procInfo_->procHandle, &ret);
				return ret;
			}

			bool isAlive() {
				return procInfo_ != nullptr;
			}
			
			explicit operator bool() {
				return procInfo_ != nullptr;
			}

			const Error& getError() const {
				return error_;
			}

			ProcInfo& get() {
				return *procInfo_;
			}
			const ProcInfo& get() const {
				return *procInfo_;
			}
			ProcInfo& operator() () {
				return *procInfo_;
			}
			const ProcInfo& operator() () const {
				return *procInfo_;
			}

		private:
			static void releaseInfo(ProcInfo* pi) {
				::CloseHandle(pi->procHandle);
				::CloseHandle(pi->threadHandle);
			}
			using ValType = std::unique_ptr<ProcInfo, decltype(&releaseInfo)>;

			//プロセス情報
			ValType procInfo_ = ValType(nullptr, releaseInfo);
			//一部引数の保存
			SecurityAttrib procAttrib_, threadAttrib_;
			StartupInfo startupInfo_;
			//エラーコード
			Error error_ = Error(0);

			//ルートコンストラクタ
			Process(
				const TString* appName,
				const TString* cmdLineArgs,
				const SecurityAttrib* procAttrib,
				const SecurityAttrib* threadAttrib,
				bool doInheritHandle,
				const UnifyEnum<ProcCreateProv>* createProv,
				const TString* envVars,
				const TString* currentDir,
				const StartupInfo* startupInfo
				) {
				if (procAttrib)
					procAttrib_ = *procAttrib;
				if (threadAttrib)
					threadAttrib_ = *threadAttrib;

				if (startupInfo)
					startupInfo_ = *startupInfo;
				else
					startupInfo_ = StartupInfo();

				ProcInfo* tmpProcInfo = new ProcInfo();

				if (
					::CreateProcess(
						(appName == nullptr ? nullptr : appName->c_str()),
						(cmdLineArgs == nullptr ? nullptr : const_cast<TChar*>(cmdLineArgs->c_str())),
						(procAttrib == nullptr ? nullptr : &procAttrib_.get()),
						(threadAttrib == nullptr ? nullptr : &threadAttrib_.get()),
						doInheritHandle,
						(createProv == nullptr ? NORMAL_PRIORITY_CLASS : createProv->get()),
						(envVars == nullptr ? nullptr : reinterpret_cast<void*>(const_cast<TChar*>(envVars->c_str()))),
						(currentDir == nullptr ? nullptr : currentDir->c_str()),
						&startupInfo_.get(),
						reinterpret_cast<::PROCESS_INFORMATION*>(tmpProcInfo)
						) == 0
					)
					delete tmpProcInfo, error_ = Error(::GetLastError());
				else {
					procInfo_ = ValType(
						tmpProcInfo,
						releaseInfo
					);
				}
			}

		};

		//test code

		//ファイルタイム
		class FileTime {
		public:
			FileTime() = default;
			FileTime(const FileTime&) = default;

			void set(const FILETIME& fileTime) {
				SYSTEMTIME systemTime;
				FileTimeToSystemTime(&fileTime, &systemTime);
				year_ = systemTime.wYear;
				month_ = systemTime.wMonth;
				day_ = systemTime.wDay;
				dayOfWeek_ = systemTime.wDayOfWeek;
				hour_ = systemTime.wHour;
				minute_ = systemTime.wMinute;
				second_ = systemTime.wSecond;
				milliSecond_ = systemTime.wMilliseconds;
				week_ = static_cast<Week>(systemTime.wDayOfWeek);
			}

			::FILETIME operator()() {
				return Entity;
			}

			Word get(const TString& value) {

				if (value == _T("year"))
					return year_;
				if (value == _T("month"))
					return month_;
				if (value == _T("day"))
					return day_;
				if (value == _T("dayofweek"))
					return dayOfWeek_;
				if (value == _T("hour"))
					return hour_;
				if (value == _T("minute"))
					return minute_;
				if (value == _T("second"))
					return second_;
				if (value == _T("millisecond"))
					return milliSecond_;
				if (value == _T("week"))
					return week_;
			}

		private:
			::FILETIME Entity;
			Word year_, month_, day_, dayOfWeek_, hour_, minute_, second_, milliSecond_;
			enum Week : Word {
				Sunday,
				Monday,
				TuesDay,
				WednesDay,
				ThursDay,
				FriDay,
				SaturDay
			} week_;
		};

		//INIファイル
		class IniFile {
		public:
			IniFile() = default;
			IniFile(const IniFile&) = default;

			IniFile(const TString& fileName) {
				fileName_ = fileName;

				std::ifstream ifs(fileName);
				if (ifs.fail()) {
					std::ofstream ofs(fileName);
					ofs.close();
				}
			}

			void open(const TString& fileName) {
				fileName_ = fileName;

				std::ifstream ifs(fileName);
				if (ifs.fail()) {
					std::ofstream ofs(fileName);
					ofs.close();
				}
			}

			TString read(const TString& sectionName, const TString& keyName) {
				TCHAR *ret;
				::GetPrivateProfileString(
					sectionName.c_str(),
					keyName.c_str(),
					L"",
					ret,
					sizeof(ret) / sizeof(TChar),
					fileName_.c_str()
					);
				return ret;
			}

			void write(const TString& sectionName, const TString& data) {
				::WritePrivateProfileSection(sectionName.c_str(), data.c_str(), fileName_.c_str());
			}
			void write(const TString& sectionName, const TString& keyName, const TString& data) {
				::WritePrivateProfileString(sectionName.c_str(), keyName.c_str(), data.c_str(), fileName_.c_str());
			}

		private:
			TString fileName_;

		};

		class Dll {
		public:
			Dll(const TString& fileName) {
				dll_ = ::LoadLibrary(fileName.c_str());
			}
			~Dll() {
				::FreeLibrary(dll_);
			}

			//getter
			auto& get() {
				return dll_;
			}
			const auto& get() const {
				return dll_;
			}
			auto& operator()() {
				return dll_;
			}
			const auto& operator()() const {
				return dll_;
			}

			FARPROC getFunc(const AString& name) {
				return ::GetProcAddress(dll_, name.c_str());
			}

		private:
			::HMODULE dll_;
		};

	} //::wawl::fs
} //::wawl