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
#include <thread>
#include <mutex>

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

		enum class PipeAccess : Dword {
			//クライアントが書き込み、サーバーは読み取り
			Inbound = 1,
			//サーバーが読み込み、クライアントが書き込み
			Outbound = 2,
			//自由
			Free = 3
		};

		enum class ConnectMode : Dword {
			//読み取り専用
			Read = 1ul << 31,
			//書き込み専用
			Write = 1ul << 30,
			//自由
			Free = Read | Write
		};

		/// <summary>PipeAccessをConnectModeに変換します。</summary>
		/// <param name="pa">変換元のPipeAccess</param>
		inline ConnectMode toConnectMode(PipeAccess pa) {
			return (
				pa == PipeAccess::Free ? ConnectMode::Free :
				pa == PipeAccess::Inbound ? ConnectMode::Write :
				ConnectMode::Read
				);
		}

		/// <summary>ConnectModeをPipeAccessに変換します。</summary>
		/// <param name="pa">変換元のConnectMode</param>
		inline PipeAccess toPipeAccess(ConnectMode pa) {
			return (
				pa == ConnectMode::Free ? PipeAccess::Free :
				pa == ConnectMode::Write ? PipeAccess::Inbound :
				PipeAccess::Outbound
				);
		}

		//ファイル
		class File {
		public:
			File() = default;
			File(const File&) = default;
			File(File&&) = default;
			File& operator = (const File&) = default;
			File& operator = (File&&) = default;

			File(FileHandle handle) {
				file_ = std::make_shared<FileHandle>(handle);
			}
			File(const std::shared_ptr<FileHandle>& handle) {
				file_ = handle;
			}

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
				file_.reset();
			}

			std::size_t getSize() const {
				LARGE_INTEGER li;
				if (::GetFileSizeEx(file_.get(), &li))
					throw Error(::GetLastError());

				return static_cast<std::size_t>(li.QuadPart);
			}

			bool read(TString& buffer) const {
				buffer.resize(getSize() + 1);
				Dword writtenSize;

				if (
					::ReadFile(
						file_.get(),
						const_cast<TChar*>(buffer.c_str()),
						buffer.size(),
						&writtenSize,
						nullptr
						)
					)
					throw Error(::GetLastError());

				return true;
			}
			bool write(const TString& str) {
				Dword writtenSize;

				if (::WriteFile(file_.get(), str.c_str(), str.size() + 1, &writtenSize, nullptr))
					throw Error(::GetLastError());

				return true;
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
				::SECURITY_ATTRIBUTES sa = (secAttr ? *secAttr : SecurityAttrib{}).get();

				file_ = std::shared_ptr<FileHandle>(
					new FileHandle(
						::CreateFile(
							(fileName == nullptr ? nullptr : fileName->c_str()),
							(accessDesc == nullptr ? GENERIC_ALL : accessDesc->get()),
							(shareMode == nullptr ? NULL : shareMode->get()),
							&sa,
							(createProv == nullptr ? CREATE_ALWAYS : createProv->get()),
							(fileAttr == nullptr ? FILE_ATTRIBUTE_NORMAL : fileAttr->get()),
							(baseFile == nullptr ? nullptr : baseFile->get())
							)),
					[](::HANDLE* h) {
					::CloseHandle(*h), delete h;
				}
				);

				if (*file_ == INVALID_HANDLE_VALUE)
					throw Error(::GetLastError());
			}

		};

		class PipeServer {
		public:
			PipeServer() = default;
			PipeServer(PipeServer&&) = default;
			PipeServer& operator = (PipeServer&&) = default;

			void start() {
				::OVERLAPPED ol = {};
				ol.hEvent = *events_[0];

				::ConnectNamedPipe(*pipe_, &ol);

				waitConnect_ = std::thread([this](Handle ev1, Handle ev2) {
					const Handle evs[2] = { ev1, ev2 };

					::WaitForMultipleObjects(
						2,
						evs,
						false,
						INFINITE
						);

					std::lock_guard<std::mutex> lock(mtx_);
					hasConnected_ = true;
				}, *events_[0], *events_[1]);
			}

			bool update() {
				if (!reaction_)
					return true;

				bool complete = false;
				{
					std::lock_guard<std::mutex> lock(mtx_);
					complete = hasConnected_;
					hasConnected_ = false;
				}

				if (complete) {
					auto pipe = File(pipe_);
					reaction_(pipe);
				}

				return true;
			}

			bool disconnect() {
				if (!pipe_)
					return false;

				return
					::FlushFileBuffers(*pipe_) != 0 &&
					::DisconnectNamedPipe(*pipe_) != 0;
			}

			/// <summary>クライアントへの応答時に呼び出される関数を設定します。</summary>
			/// <param name="f">サーバーの応答時に呼び出される関数</param>
			void reaction(const std::function<void(File&)>& f) {
				reaction_ = f;
			}

			PipeAccess getAccessMode() const {
				::DWORD ret, tmp;

				::GetNamedPipeInfo(*pipe_, &ret, &tmp, &tmp, &tmp);

				return static_cast<PipeAccess>(ret);
			}

		private:
			static void releaseHandle(Handle* h) {
				::CloseHandle(*h);
			}
			using HandlePtr = std::unique_ptr<Handle, decltype(&releaseHandle)>;

			TString name_;
			std::shared_ptr<Handle> pipe_ = nullptr;

			std::function<void(File&)> reaction_;

			mutable std::mutex mtx_;
			//接続待ちスレッド
			std::thread waitConnect_;
			//接続待ちEvent用Handle
			//0: 待機用, 1:強制終了用
			HandlePtr events_[2] = { { nullptr, releaseHandle },{ nullptr, releaseHandle } };
			bool hasConnected_ = false;

		};

		/// <summary>サーバー(パイプ)に接続します。</summary>
		/// <param name="serverName">サーバー(パイプ)の名前</param>
		/// <param name="mode">読み書きアクセス範囲</param>
		/// <param name="isRawName"><paramref name="serverName"/>がWinAPI準拠の名前になっているかどうか</param>
		/// <remarks><c>mode</c>がアクセスするサーバーの設定と一致している必要があります。</remarks>
		File connectServer(
			const TString& serverName,
			ConnectMode mode,
			bool isRawName
			) {
			Handle pipe;

			pipe = ::CreateFile(
				(isRawName ? serverName : TEXT("\\\\.\\pipe\\") + serverName).c_str(),
				static_cast<::DWORD>(mode),
				0,
				nullptr,
				OPEN_EXISTING,
				0,
				0
				);
			if (pipe == INVALID_HANDLE_VALUE)
				return File();

			return File(pipe);
		}
		/// <summary>サーバー(パイプ)に接続します。</summary>
		/// <param name="serverName">サーバー(パイプ)の名前</param>
		/// <param name="isRawName"><paramref name="serverName"/>がWinAPI準拠の名前になっているかどうか</param>
		/// <remarks>アクセスするサーバーのアクセス設定がFreeである必要があります。</remarks>
		inline File connectServer(
			const TString& serverName,
			bool isRawName = false
			) {
			return connectServer(serverName, ConnectMode::Free, isRawName);
		}

		std::pair<File, File> createPipe() {
			::HANDLE h1, h2;
			::CreatePipe(&h1, &h2, nullptr, 0);

			return std::pair<File, File>({ h1, h2 });
		}
		std::pair<File, File> createPipe(const SecurityAttrib& secAttrib) {
			::HANDLE h1, h2;
			::CreatePipe(
				&h1,
				&h2,
				const_cast<SECURITY_ATTRIBUTES*>(&secAttrib.get()),
				0
				);

			return std::pair<File, File>({ h1, h2 });
		}

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

			File getStdInput() const {
				if (!::AttachConsole(procInfo_->procId))
					throw Error(::GetLastError());

				::HANDLE h = ::GetStdHandle(STD_INPUT_HANDLE);

				if (!::AttachConsole(::GetCurrentProcessId()))
					throw Error(::GetLastError());

				return File(h);
			}
			File getStdOutput() const {
				if (!::AttachConsole(procInfo_->procId))
					throw Error(::GetLastError());

				::HANDLE h = ::GetStdHandle(STD_OUTPUT_HANDLE);

				if (!::AttachConsole(::GetCurrentProcessId()))
					throw Error(::GetLastError());

				return File(h);
			}
			File getStdError() const {
				if (!::AttachConsole(procInfo_->procId))
					throw Error(::GetLastError());

				::HANDLE h = ::GetStdHandle(STD_ERROR_HANDLE);

				if (!::AttachConsole(::GetCurrentProcessId()))
					throw Error(::GetLastError());

				return File(h);
			}

			bool isAlive() const {
				return procInfo_ != nullptr;
			}

			explicit operator bool() const {
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
				::STARTUPINFO si = (startupInfo ? *startupInfo : StartupInfo{}).get();
				::SECURITY_ATTRIBUTES procSa = (procAttrib ? *procAttrib : SecurityAttrib{}).get();
				::SECURITY_ATTRIBUTES threadSa = (threadAttrib ? *threadAttrib : SecurityAttrib{}).get();

				ProcInfo* tmpProcInfo = new ProcInfo();

				if (
					::CreateProcess(
						(appName == nullptr ? nullptr : appName->c_str()),
						(cmdLineArgs == nullptr ? nullptr : const_cast<TChar*>(cmdLineArgs->c_str())),
						(procAttrib == nullptr ? nullptr : &procSa),
						(threadAttrib == nullptr ? nullptr : &threadSa),
						doInheritHandle,
						(createProv == nullptr ? NORMAL_PRIORITY_CLASS : createProv->get()),
						(envVars == nullptr ? nullptr : reinterpret_cast<void*>(const_cast<TChar*>(envVars->c_str()))),
						(currentDir == nullptr ? nullptr : currentDir->c_str()),
						&si,
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
					TEXT(""),
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