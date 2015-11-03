#pragma once

#define WAWL_ENABLE_BASETYPE
#define WAWL_ENABLE

//WinAPI
#define NOMINMAX
#include <Windows.h>
#include <tchar.h>
#undef NOMINMAX
//C++STL
#include <string>
#include <algorithm>

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
	using AString = std::basic_string < AChar >;
	//���C�h������^
	using WString = std::basic_string < WChar >;
	//�ėp������^
	using TString = std::basic_string < TChar >;

	//�����I�ɂ�int�����ǈꉞ�錾
	using Bool = ::BOOL;
	//��������8bit����
	using Byte = ::BYTE;
	//��������16bit����
	using Word = ::WORD;
	//��������32bit����
	using Dword = ::DWORD;
	//�����Ȃ�64bit����
	using Qword = unsigned long long;

	//�|�C���^���i�[���鐮���^
	using IntPtr = ::INT_PTR;
	using UintPtr = ::UINT_PTR;

	//�n���h��
	using Handle = ::HANDLE;

#endif

	//���C�u�����̓s���I�Ȍ^
#ifdef TRUE

	//��{�I�Ȕėp���W�^
	template <typename VTy>
	struct Coordinates {

		using ValueType = VTy;

		ValueType x = 0, y = 0;

		//�f�t�H���g�R���X�g���N�^
		Coordinates() = default;

		//�R�s�[�R���X�g���N�^
		template <typename T>
		Coordinates(const Coordinates<T>& p_) :
			x(static_cast<ValueType>(p_.x)),
			y(static_cast<ValueType>(p_.y)) {}

		Coordinates(const ValueType& x_, const ValueType& y_) :
			x(x_), y(y_) {}

		template <typename T>
		void operator = (const Coordinates<T>& p_) {
			x = static_cast<ValueType>(p_.x);
			y = static_cast<ValueType>(p_.y);
		}

	};

	using Position = Coordinates < int >;
	using Rectangle = Position;

	//����R�[�h
	//TODO : �ǉ�
	enum class Lang : Dword {
		UserDefault = LANG_USER_DEFAULT,
		SysDefault = LANG_SYSTEM_DEFAULT,
	};

	class Error {
	public:
		Error() = default;
		Error(const Error&) = default;
		Error& operator = (const Error&) = default;

		Error(Dword errorCode) :
			code_(errorCode) {}
		Error(Dword errorCode, const TString& funcName) :
			code_(errorCode),
			funcName_(funcName) {}

		TString msg(Lang lang = Lang::UserDefault) const {
			static TString buf;
			buf.resize(512);

			Dword bufSize = ::FormatMessage(
				FORMAT_MESSAGE_FROM_SYSTEM,
				0,
				code_,
				static_cast<Dword>(lang),
				&buf[0],
				static_cast<Dword>(buf.size() + 1),
				nullptr
				);

			if (bufSize == 0)
				::MessageBox(nullptr, L"Error", L"buf is 0", 0);

			return buf.substr(
				0,
				bufSize
				);
		}
		Dword getCode() const {
			return code_;
		}
		const TString& getFuncName() const {
			return funcName_;
		}

	private:
		const Dword code_ = 0;
		const TString funcName_;

	};

	//�ėpenum�g�ݍ��킹�^
	template <typename EnumType>
	class _impl_UnifyEnum {
	public:
		//enum�̓����^
		using ValueType = typename std::underlying_type<EnumType>::type;

		_impl_UnifyEnum() = default;
		_impl_UnifyEnum(const _impl_UnifyEnum<EnumType>&) = default;
		_impl_UnifyEnum(_impl_UnifyEnum<EnumType>&&) = default;
		_impl_UnifyEnum<EnumType>& operator = (const _impl_UnifyEnum<EnumType>&) = default;
		_impl_UnifyEnum<EnumType>& operator = (_impl_UnifyEnum<EnumType>&&) = default;

		constexpr _impl_UnifyEnum(const EnumType& val) :
			vals_(static_cast<ValueType>(val)) {}
		constexpr _impl_UnifyEnum(const std::initializer_list<ValueType>& valList) {
			for (auto&& val : valList)
				vals_ |= val;
		}
		constexpr _impl_UnifyEnum(const std::initializer_list<EnumType>& valList) {
			for (auto&& val : valList)
				vals_ |= static_cast<ValueType>(val);
		}

		//�����̒l���擾
		ValueType& get() {
			return vals_;
		}
		constexpr const ValueType& get() const {
			return vals_;
		}
		ValueType& operator () () {
			return vals_;
		}
		constexpr const ValueType& operator () () const {
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
		_impl_UnifyEnum<ValueType>&& operator | (const EnumType& val) const {
			return std::move(_impl_UnifyEnum<ValueType>(vals_ | val));
		}

		//�܂܂�Ă��邩�ǂ���
		constexpr bool has(const ValueType& val) const {
			return (vals_ & val) != 0;
		}
		constexpr bool has(const EnumType& val) const {
			return (vals_ & static_cast<ValueType>(val)) != 0;
		}
		bool has(const std::initializer_list<EnumType> vals) const {
			return std::find_if(
				vals.begin(),
				vals.end(),
				[&vals_](EnumType& val) { return (vals_ & val) != 0 }
			) == vals.end();
		}

	private:
		ValueType vals_ = ValueType{};

	};

#endif

	//WinAPI�\���̂̃��b�v
#ifdef TRUE

	//test code
	enum class WndShowmode_ : Dword {
		Hide = 1 << 0,
		Show = 1 << 1,
		Restore = 1 << 2,

		Active = 1 << 3,
		Inactive = 1 << 4,

		Max = 1 << 5,
		Min = 1 << 6,
		Normal = 1 << 7,

		Default = 1 << 8,
		ForceMin = 1 << 9
	};
	using UnifyWndShowmode_ = _impl_UnifyEnum < WndShowmode_ >;
	Word _impl_toSWConstant(const UnifyWndShowmode_& sw) {
		if (sw.has(WndShowmode_::Show)) {
			if (sw.has(WndShowmode_::Max))
				return SW_SHOWMAXIMIZED;
			else if (sw.has(WndShowmode_::Min)) {
				if (sw.has(WndShowmode_::Inactive))
					return SW_SHOWMINNOACTIVE;
				else
					return SW_SHOWMINIMIZED;
			}
			else if (sw.has(WndShowmode_::Normal)) {
				if (sw.has(WndShowmode_::Inactive))
					return SW_SHOWNOACTIVATE;
				else
					return SW_SHOWNORMAL;
			}
			else if (sw.has(WndShowmode_::Inactive))
				return SW_SHOWNA;
			else
				return SW_SHOW;
		}
		else if (sw.has(WndShowmode_::Hide))
			return SW_HIDE;
		else if (sw.has(WndShowmode_::Restore))
			return SW_RESTORE;
		else if (sw.has(WndShowmode_::Max))
			return SW_MAXIMIZE;
		else if (sw.has(WndShowmode_::Min))
			return SW_MINIMIZE;
		else if (sw.has(WndShowmode_::Default))
			return SW_SHOWDEFAULT;
		else if (sw.has(WndShowmode_::ForceMin))
			return SW_FORCEMINIMIZE;
	}
	//~test code

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
		NormalShow = SW_SHOWNORMAL,
		NoactivateNormalShow = SW_SHOWNOACTIVATE
	};
	using UnifyWndShowMode = _impl_UnifyEnum < WndShowMode >;

#endif

} //::wawl