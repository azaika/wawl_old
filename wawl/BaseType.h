#pragma once

//WinAPI
#define NOMINMAX
#include <Windows.h>
#include <tchar.h>
#undef NOMINMAX
//C++STL
#include <string>

namespace wawl {

	//組み込み型using
#ifdef TRUE

		//Ascii文字型
		using AChar = ::CHAR;
		//ワイド文字型
		using WChar = ::WCHAR;
		//汎用文字型
		using TChar = ::TCHAR;

		//Ascii文字列型
		using AString = std::basic_string < AChar > ;
		//ワイド文字列型
		using WString = std::basic_string < WChar > ;
		//汎用文字列型
		using TString = std::basic_string < TChar > ;

		//符号無し16bit整数
		using Word = ::WORD;
		//符号無し32bit整数
		using Dword = ::DWORD;

		//ハンドル
		using GeneralHandle = ::HANDLE;

#endif

	//ライブラリの都合的な型
#ifdef TRUE

		//基本的な汎用座標型
		template <typename ValueType>
		struct Coordinates {

			ValueType x = 0, y = 0;

			//デフォルトコンストラクタ
			Coordinates() = default;

			//コピーコンストラクタ
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

		//汎用enum組み合わせ型
		template <typename EnumType>
		class _impl_UnifyEnum {
		public:
			//enumの内部型
			using ValueType = typename std::underlying_type<EnumType>::type;

			//内部の値を取得
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

			//合成
			_impl_UnifyEnum<EnumType>& compose(const EnumType& val) {
				vals_ |= static_cast<ValueType>(val);
				return *this;
			}
			_impl_UnifyEnum<EnumType>& operator | (const EnumType& val) {
				return compose(val);
			}

		private:
			ValueType vals_ = ValueType{};

		};

#endif

	//WinAPI構造体のラップ
#ifdef TRUE

		//セキュリティ指定子
		class SecurityDesc {
		public:
			SecurityDesc() {
				::InitializeSecurityDescriptor(&secDesc_, SECURITY_DESCRIPTOR_REVISION);
			}
			SecurityDesc(const SecurityDesc& secDesc) = default;
			SecurityDesc(SecurityDesc&& secDesc) :
				secDesc_(std::move(secDesc.get())) {}

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
			const ::SECURITY_DESCRIPTOR& operator () () const{
				return secDesc_;
			}

		private:
			::SECURITY_DESCRIPTOR secDesc_;

		};
		//セキュリティ記述子
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
			::SECURITY_ATTRIBUTES secAttr_;

		};

		//CUIでの文字列色
		enum class ConsoleStrColor : Dword {
			Blue = FOREGROUND_BLUE,
			Green = FOREGROUND_GREEN,
			Red = FOREGROUND_RED,
			Intensity = FOREGROUND_INTENSITY
		};
		using ConsoleStrColors = _impl_UnifyEnum < ConsoleStrColor > ;

		//CUIでの背景色
		enum class ConsoleBackgroundColor : Dword{
			Blue = BACKGROUND_BLUE,
			Green = BACKGROUND_GREEN,
			Red = BACKGROUND_RED,
			Intensity = BACKGROUND_INTENSITY
		};
		using ConsoleBackgroundColors = _impl_UnifyEnum < ConsoleBackgroundColor > ;

		//アプリケーション起動時のオプション
		enum class StartupOption : Dword {
			RunFullscreen = STARTF_RUNFULLSCREEN,
			ForceChangeCursor = STARTF_FORCEONFEEDBACK,
			ForceImmutableCursor = STARTF_FORCEOFFFEEDBACK,
			NoPinningTaskbar = STARTF_PREVENTPINNING || STARTF_TITLEISAPPID,
			RelateTitleWithAppID = STARTF_TITLEISAPPID,
			RelateTitleWithLnk = STARTF_TITLEISLINKNAME
		};
		using StartupInfoes = _impl_UnifyEnum < StartupOption > ;

		//Window表示形式
		enum class WndShowMode : Word {
			//ToDo : SW_Hoge系マクロのリネーム追加
		};
		using WndShowModes = _impl_UnifyEnum < WndShowMode > ;

		//ファイルハンドル
		class FileHandle {
		public:
			//ToDo : 生成関連追加

			//内部の値を取得
			GeneralHandle& get() {
				return fileHandle_;
			}
			const GeneralHandle& get() const {
				return fileHandle_;
			}
			GeneralHandle& operator () () {
				return fileHandle_;
			}
			const GeneralHandle& operator () () const {
				return fileHandle_;
			}

		private:
			GeneralHandle fileHandle_ = nullptr;

		};

		//アプリケーション起動のための情報
		class StartupInfo {
		public:
			StartupInfo(
				const TString* desktopName = nullptr,
				const TString* wndTitle = nullptr,
				const Position* wndPos = nullptr,
				const Rectangle* wndRect = nullptr,
				const Rectangle* consoleBuf = nullptr,
				const ConsoleStrColors* consoleStrColors = nullptr,
				const ConsoleBackgroundColors* consoleBackgroundColors = nullptr,
				const WndShowModes* wndShowModes = nullptr,
				const FileHandle* stdInput = nullptr,
				const FileHandle* stdOutput = nullptr,
				const FileHandle* stdError = nullptr
				) {
				::ZeroMemory(&suInfo_, sizeof(suInfo_));

				//サイズ設定
				suInfo_.cb = sizeof(::STARTUPINFO);
				//デスクトップ名指定
				if (desktopName != nullptr)
					suInfo_.lpDesktop = const_cast<TChar*>(desktopName->c_str());
				//Windowのタイトル指定
				if (wndTitle != nullptr)
					suInfo_.lpTitle = const_cast<TChar*>(wndTitle->c_str());
				//Windowの座標設定
				if (wndPos != nullptr)
					suInfo_.dwX = wndPos->x,
					suInfo_.dwY = wndPos->y;
				//Windowの大きさ設定
				if (wndRect != nullptr)
					suInfo_.dwXSize = wndRect->x,
					suInfo_.dwYSize = wndRect->y;
				//Consoleのバッファーの大きさ設定
				if (consoleBuf != nullptr)
					suInfo_.dwXCountChars = consoleBuf->x,
					suInfo_.dwYCountChars = consoleBuf->y;
				//Consoleの文字色、背景色設定
				if (consoleStrColors != nullptr)
					suInfo_.dwFillAttribute |= consoleStrColors->get();
				if (consoleBackgroundColors != nullptr)
					suInfo_.dwFillAttribute |= consoleBackgroundColors->get();
				//Window表示形式の設定
				if (wndShowModes != nullptr)
					suInfo_.wShowWindow = wndShowModes->get();
				//標準入力、出力、エラー出力ファイルの設定
				if (stdInput != nullptr)
					suInfo_.hStdInput = stdInput->get();
				if (stdOutput != nullptr)
					suInfo_.hStdOutput = stdOutput->get();
				if (stdError != nullptr)
					suInfo_.hStdError = stdError->get();
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
			//ToDo : コンストラクタの種類増加

			//内部の値を取得
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