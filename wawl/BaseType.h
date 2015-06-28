#pragma once

//WinAPI
#define NOMINMAX
#include <Windows.h>
#include <tchar.h>
#undef NOMINMAX
//C++STL
#include <string>

namespace wawl {

	namespace {

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

		//��������16bit����
		using Word = ::WORD;
		//��������32bit����
		using Dword = ::DWORD;

		//�n���h��
		using GeneralHandle = ::HANDLE;

	}

	namespace {

		//��{�I�Ȕėp���W�^
		template <typename ValueType>
		struct Coordinates {

			ValueType x = 0, y = 0;

			//�f�t�H���g�R���X�g���N�^
			Coordinates() = default;

			//�R�s�[�R���X�g���N�^
			template <typename T>
			explicit Coordinates(const Coordinates<T>& p_) :
				x(static_cast<ValueType>(p_.x)),
				y(static_cast<ValueType>(p_.y)) {}

			template <typename T>
			Coordinates(const T& x_, const T& y_) :
				x(static_cast<ValueType>(x_)),
				y(static_cast<ValueType>(y_)) {}

			template <typename T>
			inline void operator = (const Coordinates<T>& p_) {
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

			//�����̒l���擾
			inline ValueType& Get() {
				return vals_;
			}
			inline const ValueType& Get() const {
				return vals_;
			}
			inline ValueType& operator () () {
				return vals_;
			}
			inline const ValueType& operator () () const{
				return vals_;
			}

			//����
			inline _impl_UnifyEnum<EnumType>& Compose(const EnumType& val) {
				vals_ |= static_cast<ValueType>(val);
				return *this;
			}
			inline _impl_UnifyEnum<EnumType>& operator | (const EnumType& val) {
				return Compose(val);
			}

		private:
			ValueType vals_ = ValueType{};

		};

	}

	namespace {

		//�Z�L�����e�B�w��q
		class SecurityDesc {
		public:
			SecurityDesc() {
				::InitializeSecurityDescriptor(&secDesc_, SECURITY_DESCRIPTOR_REVISION);
			}
			SecurityDesc(const SecurityDesc& secDesc) = default;
			SecurityDesc(SecurityDesc&& secDesc) :
				secDesc_(std::move(secDesc.Get())) {}

			//�����̒l���擾
			inline ::SECURITY_DESCRIPTOR& Get() {
				return secDesc_;
			}
			inline const ::SECURITY_DESCRIPTOR& Get() const {
				return secDesc_;
			}
			inline ::SECURITY_DESCRIPTOR& operator () () {
				return secDesc_;
			}
			inline const ::SECURITY_DESCRIPTOR& operator () () const{
				return secDesc_;
			}

		private:
			::SECURITY_DESCRIPTOR secDesc_;

		};
		//�Z�L�����e�B�L�q�q
		class SecurityAttrib{
		public:
			SecurityAttrib(bool doInheritHandle, SecurityDesc&& secDesc) {
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
				secAttr_(std::move(secAttr.Get())) {}

			//�����̒l���擾
			::SECURITY_ATTRIBUTES& Get() {
				return secAttr_;
			}
			const ::SECURITY_ATTRIBUTES& Get() const{
				return secAttr_;
			}
			inline ::SECURITY_ATTRIBUTES& operator () () {
				return secAttr_;
			}
			inline const ::SECURITY_ATTRIBUTES& operator () () const {
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
		using ConsoleStrColors = _impl_UnifyEnum < ConsoleStrColor > ;

		//CUI�ł̔w�i�F
		enum class ConsoleBackgroundColor : Dword{
			Blue = BACKGROUND_BLUE,
			Green = BACKGROUND_GREEN,
			Red = BACKGROUND_RED,
			Intensity = BACKGROUND_INTENSITY
		};
		using ConsoleBackgroundColors = _impl_UnifyEnum < ConsoleBackgroundColor > ;

		//�A�v���P�[�V�����N�����̃I�v�V����
		enum class StartupOption : Dword {
			RunFullscreen = STARTF_RUNFULLSCREEN,
			ForceChangeCursor = STARTF_FORCEONFEEDBACK,
			ForceImmutableCursor = STARTF_FORCEOFFFEEDBACK,
			NoPinningTaskbar = STARTF_PREVENTPINNING || STARTF_TITLEISAPPID,
			RelateTitleWithAppID = STARTF_TITLEISAPPID,
			RelateTitleWithLnk = STARTF_TITLEISLINKNAME
		};
		using StartupInfoes = _impl_UnifyEnum < StartupOption > ;

		//Window�\���`��
		enum class WndShowMode : Word {
			//ToDo : SW_Hoge�n�}�N���̃��l�[���ǉ�
		};
		using WndShowModes = _impl_UnifyEnum < WndShowMode > ;

		//�t�@�C���n���h��
		class FileHandle {
		public:
			//ToDo : �����֘A�ǉ�

			//�����̒l���擾
			inline GeneralHandle& Get() {
				return fileHandle_;
			}
			inline const GeneralHandle& Get() const {
				return fileHandle_;
			}
			inline GeneralHandle& operator () () {
				return fileHandle_;
			}
			inline const GeneralHandle& operator () () const {
				return fileHandle_;
			}

		private:
			GeneralHandle fileHandle_ = nullptr;

		};

		//�A�v���P�[�V�����N���̂��߂̏��
		class StartupInfo {
		public:
			StartupInfo(
				const TString* desktopName,
				const TString* wndTitle,
				const Position* wndPos,
				const Rectangle* wndRect,
				const Rectangle* consoleBuf,
				const ConsoleStrColors* consoleStrColors,
				const ConsoleBackgroundColors* consoleBackgroundColors,
				const WndShowModes* wndShowModes,
				const FileHandle* stdInput,
				const FileHandle* stdOutput,
				const FileHandle* stdError
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
					suInfo_.dwFillAttribute |= consoleStrColors->Get();
				if (consoleBackgroundColors != nullptr)
					suInfo_.dwFillAttribute |= consoleBackgroundColors->Get();
				//Window�\���`���̐ݒ�
				if (wndShowModes != nullptr)
					suInfo_.wShowWindow = wndShowModes->Get();
				//�W�����́A�o�́A�G���[�o�̓t�@�C���̐ݒ�
				if (stdInput != nullptr)
					suInfo_.hStdInput = stdInput->Get();
				if (stdOutput != nullptr)
					suInfo_.hStdOutput = stdOutput->Get();
				if (stdError != nullptr)
					suInfo_.hStdError = stdError->Get();
			}
			StartupInfo(
				const TString& desktopName,
				const TString& wndTitle,
				const Position& wndPos,
				const Rectangle& wndRect,
				const Rectangle& consoleBuf,
				const ConsoleStrColors& consoleStrColors,
				const ConsoleBackgroundColors& consoleBackgroundColors,
				const WndShowModes& wndShowModes,
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
				&consoleBackgroundColors,
				&wndShowModes,
				&stdInput,
				&stdOutput,
				&stdError
				) {}
			//ToDo : �R���X�g���N�^�̎�ޑ���
			StartupInfo() = default;

			//�����̒l���擾
			inline ::STARTUPINFO& Get() {
				return suInfo_;
			}
			inline const ::STARTUPINFO& Get() const {
				return suInfo_;
			}
			inline ::STARTUPINFO& operator () () {
				return suInfo_;
			}
			inline const ::STARTUPINFO& operator () () const {
				return suInfo_;
			}

		private:
			::STARTUPINFO suInfo_;

		};

	}

} //::wawl