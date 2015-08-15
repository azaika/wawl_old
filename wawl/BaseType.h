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
			_impl_UnifyEnum(const _impl_UnifyEnum<EnumType>&) = default;
			_impl_UnifyEnum(_impl_UnifyEnum<EnumType>&&) = default;
			_impl_UnifyEnum<EnumType>& operator = (const _impl_UnifyEnum<EnumType>&) = default;
			_impl_UnifyEnum<EnumType>& operator = (_impl_UnifyEnum<EnumType>&&) = default;

			_impl_UnifyEnum(const EnumType& val) :
				vals_(static_cast<ValueType>(val)) {}
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
		
#endif

} //::wawl