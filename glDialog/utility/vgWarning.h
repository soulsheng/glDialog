

#ifndef __VGWARNING_H__
#define __VGWARNING_H__

#include "vgPrerequisites.h"


namespace vgFoundationSystem {

	/// �о�д���Warning�໹�Ǻ��б�Ҫ��.ԭ��:
	///   1.�����Ϸֹ�����Ҫ.�Ժ�Ϊ�˽�ʡ�洢����Ҫд���ļ�,
	/// ���������Ӧ�����������ʵ��.���������Ĺ��������,���л�
	/// �ȵ�.
	///   2.��ҪʱҲ��Ҫ�׳�Warning�쳣,���������Exception����
	/// ����Ĵ���.
	///   3.ͨ����־�ļ������,Ҫ����һ��struct��������Warning��Ϣ,
	/// ������໹�Ǳ�Ҫ��.
	/// �μ�:Exception��.
	class Warning
	{
	public:

		enum WarningCodes 
		{
			/// д���ļ�ʧ��.
			/// �Ƽ���Ӧ��ϵ: retry , cancel
			WARN_CANNOT_WRITE_TO_FILE,
			/// ���Ƽ���״̬,��Ȼ����ǿ������.
			/// �Ƽ���Ӧ��ϵ: ok , cancel
			WARN_INVALID_STATE,
			/// ���Ƽ��Ĵ������,Ҳ���������.
			/// �Ƽ���Ӧ��ϵ: ok , cancel
			WARN_INVALIDPARAMS,
			/// û�в��ҵ������Դ,ѯ���Ƿ����Ĭ�Ϸ�ʽ������������.
			/// �Ƽ���Ӧ��ϵ: retry , cancel
			WARN_ITEM_NOT_FOUND,
			/// �ļ�δ������ȡ,ѯ��ȡ�����ǲ���Ĭ�Ͽհ��ļ�.
			/// �Ƽ���Ӧ��ϵ: retry , cancel
			WARN_FILE_NOT_FOUND,
			/// �ڲ�Warning,ͨ������debugģʽ.
			/// �Ƽ���Ӧ��ϵ: ok
			WARN_INTERNAL_WARNING,
			/// ʵ�ֲ����,������������,Warningһ��.
			/// �Ƽ���Ӧ��ϵ: ok
			WARN_IMPLEMENT_ILL,	
		};


/// vc 6.0 �᲻��ʶMB_CANCELTRYCONTINUE
#if _MSC_VER == 1200	 
#define MB_CANCELTRYCONTINUE  MB_RETRYCANCEL
#define IDTRYAGAIN			IDRETRY
#define IDCONTINUE			IDIGNORE
#endif

		/**

		BUTTON_ABORT_RETRY_IGNORE
		ǿ���˳�,����,����

		BUTTON_CANCEL_TRY_CONTINUE
		ȡ��,����,����

		BUTTON_HELP	
		����.(������������������)

		BUTTON_OK			
		ȷ��.

		BUTTON_CANCEL
		ȡ��

		BUTTON_RETRY_CANCEL
		����,ȡ��

		BUTTON_YES_NO
		��,��

		BUTTON_YES_NO_CANCEL
		��,��,ȡ��

		BUTTON_NONE
		�������κζԻ���,Ĭ��Ϊд����־.

		*/
		enum WarningButtonCodes
		{
			BUTTON_ABORT_RETRY_IGNORE	= MB_ABORTRETRYIGNORE,
			BUTTON_CANCEL_TRY_CONTINUE	= MB_CANCELTRYCONTINUE,
			BUTTON_HELP					= MB_HELP,
			BUTTON_OK					= MB_OK,
			BUTTON_CANCEL				= MB_OKCANCEL,
			BUTTON_RETRY_CANCEL			= MB_RETRYCANCEL,
			BUTTON_YES_NO				= MB_YESNO,
			BUTTON_YES_NO_CANCEL		= MB_YESNOCANCEL,
			BUTTON_NONE					= 0x00000040L
		};

		enum WarningReturnCode {
			/// �رվ������ִ��.
			RETURN_IGNORE,
			/// �׳��쳣,��ֹ����.
			RETURN_ABORT,
			/// ��������.
			RETURN_RETRY,
			RETURN_OK,
			/// �ж�����.�������ڴ���һ����ļ�ʱ,�����м��˳�.
			RETURN_CANCEL,
			RETURN_YES,
			RETURN_NO,
			RETURN_CONTINUE,

			RETURN_NONE
		};


		/** Default constructor.
		*/
		Warning( int number, const String& description, const String& source );

		/** Advanced constructor.
		*/
		Warning( int number, const String& description, const String& source, const char* type, const char* file, long line );

		/** Copy constructor.
		*/
		Warning(const Warning& rhs);


		void operator = (const Warning& rhs);

		virtual const String& getFullDescription(void) const;

		/** Gets the error code.
		*/
		virtual int getNumber(void) const throw();

		string getNumberAsString(void) const ;

		/** Gets the source function.
		*/
		virtual const String &getSource() const { return source; }

		/** Gets source file name.
		*/
		virtual const String &getFile() const { return file; }

		/** Gets line number.
		*/
		virtual long getLine() const { return line; }


		virtual const String &getDescription(void) const { return description; }

		const char* what() const throw() { return getFullDescription().c_str(); }

		/// ������Լ̳���д�˷���.���û����д,��ΪĬ�ϵ���Warning�ķ�ʽ.
		virtual int showWarning(const int& flag = BUTTON_OK ) const
		{
			if ( (flag & BUTTON_NONE) != 0 )
			{
				return RETURN_NONE;
			}
			int val = MessageBox( NULL , this->what() , typeName.c_str() , flag | MB_ICONWARNING );
			return getWarningReturnFromWin32Btn( val );
		}

	protected:
		long line;
		///����:WarningCodes
		int number;		
		String typeName;
		String description;
		String source;
		String file;
		mutable String fullDesc;

		inline int showWin32MessageBox(const int& btn_type) const
		{
			return MessageBox( NULL , this->what() , typeName.c_str() ,
				btn_type );
		}

		int getWarningReturnFromWin32Btn(const int& val) const;
	};



	/// �������غ��������ı�Ҫ����.
	template <int num>
	struct WarningCodeType
	{
		enum { number = num };
	};


	class  IOWarning : public Warning
	{
	public:
		IOWarning(int number, const String& description, const String& source, const char* file, long line)
			: Warning(number, description, source, "IOWarning", file, line) {}
	};
	class  InvalidStateWarning : public Warning
	{
	public:
		InvalidStateWarning(int number, const String& description, const String& source, const char* file, long line)
			: Warning(number, description, source, "InvalidStateWarning", file, line) {}
	};
	class  InvalidParametersWarning : public Warning
	{
	public:
		InvalidParametersWarning(int number, const String& description, const String& source, const char* file, long line)
			: Warning(number, description, source, "InvalidParametersWarning", file, line) {}
	};
	class  ItemNotFoundWarning : public Warning
	{
	public:
		ItemNotFoundWarning(int number, const String& description, const String& source, const char* file, long line)
			: Warning(number, description, source, "ItemNotFoundWarning", file, line) {}
	};
	class  FileNotFoundWarning : public Warning
	{
	public:
		FileNotFoundWarning(int number, const String& description, const String& source, const char* file, long line)
			: Warning(number, description, source, "FileNotFoundWarning", file, line) {}
	};
	class  InternalWarning : public Warning
	{
	public:
		InternalWarning(int number, const String& description, const String& source, const char* file, long line)
			: Warning(number, description, source, "InternalWarning", file, line) {}
	};
	class  ImplementIllWarning : public Warning 
	{
	public:
		ImplementIllWarning(int number, const String& description, const String& source, const char* file, long line)
			: Warning(number, description, source, "ImplementIllWarning", file, line) {}
	};



	/// �������Ȳ�����ô���ӵĴ����ܶ�Warning�������.
	/// ֻ�Ǽ򵥵�ʵ�ּ���.
	class WarningFactory
	{
	public:
		WarningFactory()
		{

		}
		~WarningFactory()
		{

		}

		static int  makeWarning(
			WarningCodeType<Warning::WARN_CANNOT_WRITE_TO_FILE> code ,
			const int& flag , const String& desc, 
			const String& src, const char* file, long line)
		{
			const Warning& warn = IOWarning(code.number, desc, src, file, line);
			return warn.showWarning( flag );
		}
		static int  makeWarning(
			WarningCodeType<Warning::WARN_INVALID_STATE> code ,
			const int& flag , const String& desc, 
			const String& src, const char* file, long line)
		{
			const Warning& warn = InvalidStateWarning(code.number, desc, src, file, line);
			return warn.showWarning( flag );
		}
		static int  makeWarning(
			WarningCodeType<Warning::WARN_INVALIDPARAMS> code ,
			const int& flag , const String& desc, 
			const String& src, const char* file, long line)
		{
			const Warning& warn = InvalidParametersWarning(code.number, desc, src, file, line);
			return warn.showWarning( flag );
		}
		static int  makeWarning(
			WarningCodeType<Warning::WARN_ITEM_NOT_FOUND> code ,
			const int& flag , const String& desc, 
			const String& src, const char* file, long line)
		{
			const Warning& warn = ItemNotFoundWarning(code.number, desc, src, file, line);
			return warn.showWarning( flag );
		}
		static int  makeWarning(
			WarningCodeType<Warning::WARN_FILE_NOT_FOUND> code ,
			const int& flag , const String& desc, 
			const String& src, const char* file, long line)
		{
			const Warning& warn = FileNotFoundWarning(code.number, desc, src, file, line);
			return warn.showWarning( flag );
		}
		static int  makeWarning(
			WarningCodeType<Warning::WARN_INTERNAL_WARNING> code ,
			const int& flag , const String& desc, 
			const String& src, const char* file, long line)
		{
			const Warning& warn = InternalWarning(code.number, desc, src, file, line);
			return warn.showWarning( flag );
		}
		static int  makeWarning(
			WarningCodeType<Warning::WARN_IMPLEMENT_ILL> code ,
			const int& flag , const String& desc, 
			const String& src, const char* file, long line)
		{
			const Warning& warn = ImplementIllWarning(code.number, desc, src, file, line);
			return warn.showWarning( flag );
		}

	};



}// end of namespace vgFoundationSystem

#define VG_WARN(num, flags, description, src) vgFoundationSystem::WarningFactory::makeWarning( \
	vgFoundationSystem::WarningCodeType<num>(), flags,description, src, __FILE__, __LINE__ );



#endif // end of __VGWARNING_H__