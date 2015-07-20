#pragma once

//WinAPI
#define NOMINMAX
#include <Windows.h>
#include <tchar.h>
#undef NOMINMAX
//C++STL
#include <string>

namespace wawl {

	//�g�ݍ��݌^using
#ifdef TRUE

		//Ascii�����^
		using AChar = ::CHAR;
		//���C�h�����^
		using WChar = ::WCHAR;
		//�ėp�����^
		using TChar = ::TCHAR;

		//Ascii������^
		using AString = std::basic_string < AChar > ;
		//���C�h������^
		using WString = std::basic_string < WChar > ;
		//�ėp������^
		using TString = std::basic_string < TChar > ;

		//��������8bit����
		using Byte = ::BYTE;
		//��������16bit����
		using Word = ::WORD;
		//��������32bit����
		using Dword = ::DWORD;

		//�n���h��
		using GeneralHandle = ::HANDLE;

#endif

	//���C�u�����̓s���I�Ȍ^
#ifdef TRUE

		//��{�I�Ȕėp���W�^
		template <typename ValueType>
		struct Coordinates {

			ValueType x = 0, y = 0;

			//�f�t�H���g�R���X�g���N�^
			Coordinates() = default;

			//�R�s�[�R���X�g���N�^
			template <typename T>
			Coordinates(const Coordinates<T>& p_) :
				x(static_cast<ValueType>(p_.x)),
				y(static_cast<ValueType>(p_.y)) {}

			template <typename T>
			Coordinates(const T& x_, const T& y_) :
				x(static_cast<ValueType>(x_)),
				y(static_cast<ValueType>(y_)) {}

			template <typename T>
			void operator = (const Coordinates<T>& p_) {
				x = static_cast<ValueType>(p_.x);
				y = static_cast<ValueType>(p_.y);
			}

		};

		using Position = Coordinates < int > ;
		using Rectangle = Position;

		//�ėpenum�g�ݍ��킹�^
		template <typename EnumType>
		class _impl_UnifyEnum {
		public:
			//enum�̓����^
			using ValueType = typename std::underlying_type<EnumType>::type;

			_impl_UnifyEnum() = default;
			_impl_UnifyEnum(const _impl_UnifyEnum<ValueType>&) = default;
			_impl_UnifyEnum<ValueType>& operator = (const _impl_UnifyEnum<ValueType>&) = default;

			_impl_UnifyEnum(const std::initializer_list<ValueType>& valList) {
				for (auto&& val : valList)
					vals_ |= val;
			}

			//�����̒l���擾
			ValueType& get() {
				return vals_;
			}
			const ValueType& get() const {
				return vals_;
			}
			ValueType& operator () () {
				return vals_;
			}
			const ValueType& operator () () const{
				return vals_;
			}

			//����
			_impl_UnifyEnum<ValueType>& compose(const ValueType& val) {
				vals_ |= static_cast<ValueType>(val);
				return *this;
			}
			_impl_UnifyEnum<ValueType>& operator |= (const ValueType& val) {
				return compose(val);
			}
			_impl_UnifyEnum<ValueType>& operator | (const EnumType& val) const {
				return _impl_UnifyEnum < ValueType > {vals_ | val};
			}

			//�܂܂�Ă��邩�ǂ���
			bool isIncluded(const ValueType& val) {
				return (vals_ & val) == val;
			}

		private:
			ValueType vals_ = ValueType{};

		};

#endif

	//WinAPI�\���̂̃��b�v
#ifdef TRUE

		//�Z�L�����e�B�w��q
		class SecurityDesc {
		public:
			SecurityDesc() {
				::InitializeSecurityDescriptor(&secDesc_, SECURITY_DESCRIPTOR_REVISION);
			}
			SecurityDesc(const SecurityDesc& secDesc) = default;

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
			SecurityAttrib(bool doInheritHandle, SecurityDesc& secDesc) {
				secAttr_.nLength = sizeof(::SECURITY_ATTRIBUTES);
				secAttr_.bInheritHandle = doInheritHandle;
				secAttr_.lpSecurityDescriptor = &secDesc;
			}
			SecurityAttrib(bool doInheritHandle = false) {
				secAttr_.nLength = sizeof(::SECURITY_ATTRIBUTES);
				secAttr_.bInheritHandle = doInheritHandle;
				secAttr_.lpSecurityDescriptor = nullptr;
			}
			SecurityAttrib(const SecurityAttrib& secAttr) = default;
			SecurityAttrib(SecurityAttrib&& secAttr) :
				secAttr_(std::move(secAttr.get())) {}

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
			::SECURITY_ATTRIBUTES secAttr_;

		};

		//CUI�ł̕�����F
		enum class ConsoleStrColor : Dword {
			Blue = FOREGROUND_BLUE,
			Green = FOREGROUND_GREEN,
			Red = FOREGROUND_RED,
			Intensity = FOREGROUND_INTENSITY
		};
		using UnifyConsoleStrColor = _impl_UnifyEnum < ConsoleStrColor > ;

		//CUI�ł̔w�i�F
		enum class ConsoleBgColor : Dword{
			Blue = BACKGROUND_BLUE,
			Green = BACKGROUND_GREEN,
			Red = BACKGROUND_RED,
			Intensity = BACKGROUND_INTENSITY
		};
		using UnifyConsoleBgColor = _impl_UnifyEnum < ConsoleBgColor > ;

		//�A�v���P�[�V�����N�����̃I�v�V����
		enum class StartupOption : Dword {
			RunFullscreen = STARTF_RUNFULLSCREEN,
			ForceChangeCursor = STARTF_FORCEONFEEDBACK,
			ForceImmutableCursor = STARTF_FORCEOFFFEEDBACK,
			NoPinningTaskbar = STARTF_PREVENTPINNING || STARTF_TITLEISAPPID,
			RelateTitleWithAppID = STARTF_TITLEISAPPID,
			RelateTitleWithLnk = STARTF_TITLEISLINKNAME
		};
		using UnifyStartupOption = _impl_UnifyEnum < StartupOption > ;

		//Window�\���`��
		enum class WndShowMode : Word {
			ForceMinimize = SW_FORCEMINIMIZE,
			Hide = SW_HIDE,
			Maximize = SW_MAXIMIZE,
			Minimize = SW_MINIMIZE,
			Restore = SW_RESTORE,
			Show = SW_SHOW,
			DefaultShow = SW_SHOWDEFAULT,
			MaximizedShow = SW_SHOWMAXIMIZED,
			MinimizedShow = SW_SHOWMINIMIZED,
			InactivateMinimized = SW_SHOWMINNOACTIVE,
			NoactivateShow = SW_SHOWNA,
			NoactivateNormalShow = SW_SHOWNOACTIVATE,
			NormalShow = SW_SHOWNORMAL
		};
		using UnifyWndShowMode = _impl_UnifyEnum < WndShowMode > ;

		//�A�N�Z�X�w��q
		enum class AccessDesc : Dword {
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
		using UnifyAccessDesc = _impl_UnifyEnum < AccessDesc > ;

		//�t�@�C���V�F�A�`��
		enum class FileSharePermit : Dword {
			Delete = FILE_SHARE_DELETE,
			Read = FILE_SHARE_READ,
			Write = FILE_SHARE_WRITE
		};
		using UnifyFileSharePermit = _impl_UnifyEnum < FileSharePermit > ;

		//�t�@�C�������K��
		enum class FileCreateProv : Dword {
			New = CREATE_NEW,
			AlwaysNew = CREATE_ALWAYS,
			OpenExisting = OPEN_EXISTING,
			AlwaysOpen = OPEN_ALWAYS,
			ClearExisting = TRUNCATE_EXISTING
		};
		using UnifyFileCreateProv = _impl_UnifyEnum < FileCreateProv > ;

		//�t�@�C���̑����L�q�q
		enum class FileAttr : Dword {
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
		using UnifyFileAttr = _impl_UnifyEnum < FileAttr > ;

		//�t�@�C���n���h��
		using FileHandle = GeneralHandle;

		//�A�v���P�[�V�����N���̂��߂̏��
		class StartupInfo {
		public:
			StartupInfo(
				const TString* desktopName = nullptr,
				const TString* wndTitle = nullptr,
				const Position* wndPos = nullptr,
				const Rectangle* wndRect = nullptr,
				const Rectangle* consoleBuf = nullptr,
				const UnifyConsoleStrColor* consoleStrColors = nullptr,
				const UnifyConsoleBgColor* consoleBgColors = nullptr,
				const UnifyStartupOption* startupOptions = nullptr,
				const UnifyWndShowMode* wndShowModes = nullptr,
				const FileHandle stdInput = nullptr,
				const FileHandle stdOutput = nullptr,
				const FileHandle stdError = nullptr
				) {
				::ZeroMemory(&suInfo_, sizeof(suInfo_));

				//�T�C�Y�ݒ�
				suInfo_.cb = sizeof(::STARTUPINFO);
				//�f�X�N�g�b�v���w��
				if (desktopName != nullptr)
					suInfo_.lpDesktop = const_cast<TChar*>(desktopName->c_str());
				//Window�̃^�C�g���w��
				if (wndTitle != nullptr)
					suInfo_.lpTitle = const_cast<TChar*>(wndTitle->c_str());
				//Window�̍��W�ݒ�
				if (wndPos != nullptr)
					suInfo_.dwX = wndPos->x,
					suInfo_.dwY = wndPos->y;
				//Window�̑傫���ݒ�
				if (wndRect != nullptr)
					suInfo_.dwXSize = wndRect->x,
					suInfo_.dwYSize = wndRect->y;
				//Console�̃o�b�t�@�[�̑傫���ݒ�
				if (consoleBuf != nullptr)
					suInfo_.dwXCountChars = consoleBuf->x,
					suInfo_.dwYCountChars = consoleBuf->y;
				//Console�̕����F�A�w�i�F�ݒ�
				if (consoleStrColors != nullptr)
					suInfo_.dwFillAttribute |= consoleStrColors->get();
				if (consoleBgColors != nullptr)
					suInfo_.dwFillAttribute |= consoleBgColors->get();
				//Window�\���`���̐ݒ�
				if (wndShowModes != nullptr)
					suInfo_.wShowWindow = wndShowModes->get();
				//�W�����́A�o�́A�G���[�o�̓t�@�C���̐ݒ�
				if (stdInput != nullptr)
					suInfo_.hStdInput = stdInput;
				if (stdOutput != nullptr)
					suInfo_.hStdOutput = stdOutput;
				if (stdError != nullptr)
					suInfo_.hStdError = stdError;
			}
			StartupInfo(
				const TString& desktopName,
				const TString& wndTitle,
				const Position& wndPos,
				const Rectangle& wndRect,
				const Rectangle& consoleBuf,
				const UnifyConsoleStrColor& consoleStrColors,
				const UnifyConsoleBgColor& consoleBgColors,
				const UnifyStartupOption& startupOptions,
				const UnifyWndShowMode& wndShowModes,
				const FileHandle& stdInput,
				const FileHandle& stdOutput,
				const FileHandle& stdError
				) :
				StartupInfo(
				&desktopName,
				&wndTitle,
				&wndPos,
				&wndRect,
				&consoleBuf,
				&consoleStrColors,
				&consoleBgColors,
				&startupOptions,
				&wndShowModes,
				stdInput,
				stdOutput,
				stdError
				) {}
			//ToDo : �R���X�g���N�^�̎�ޑ���

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
			::STARTUPINFO suInfo_;

		};

#endif

} //::wawl