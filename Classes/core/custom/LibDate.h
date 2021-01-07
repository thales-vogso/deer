/// \file LibDate.h
/// DateTime��ͷ�ļ�
/// ����ʱ������

#ifndef _WEBDEVLIB_DATE_H_
#define _WEBDEVLIB_DATE_H_ 

#include <string>
#include <ctime>
#include "cocos2d.h"

using namespace std;

// WEB Application Develop Library namaspace
namespace custom
{
	/// \page vars ȫ�ֱ����б�
	/// \var <b>one_sec</b> ʱ������һ����
	const time_t one_sec = 1;

	/// \page vars ȫ�ֱ����б�
	/// \var <b>one_min</b> ʱ������һ����
	const time_t one_min = 60;

	/// \page vars ȫ�ֱ����б�
	/// \var <b>one_hour</b> ʱ������һСʱ
	const time_t one_hour = 60*60;

	/// \page vars ȫ�ֱ����б�
	/// \var <b>one_day</b> ʱ������һ��
	const time_t one_day = 60*60*24;

	/// \page vars ȫ�ֱ����б�
	/// \var <b>one_week</b> ʱ������һ��
	const time_t one_week = 60*60*24*7;


	/// DateTime��
	class LibDate
	{
		private:
		time_t _time;
		struct tm _tm;

		/**
		*	��ʼʱ��
		*/
		static clock_t _startTime;
		static cocos2d::ValueMapIntKey _startTimeMap;
		
		public:
		
		/// Ĭ�Ϲ��캯��,�Ե�ǰʱ�乹�����
		LibDate()
		{
			set();
		}
		
		/// ����Ϊ time_t �Ĺ��캯��
		LibDate(const time_t &tt)
		{
			set( tt );
		}
		
		/// ����Ϊ tm �ṹ�Ĺ��캯��
		LibDate(const tm &st)
		{
			set( st );
		}
		
		/// ����Ϊָ��ʱ��Ĺ��캯��
		LibDate(const int year, const int mon, const int mday,
			  const int hour=0, const int min=0, const int sec=0 )
		{
			set( year, mon, mday, hour, min, sec );
		}
		
		/// ����Ϊ"YYYY-MM-DD HH:MM:SS"��ʽ�ַ����Ĺ��캯��
		LibDate(const string &datetime,
			  const string &datemark = "-", 
			  const string &dtmark = " ",
			  const string &timemark = ":" )
		{
			set( datetime, datemark, dtmark, timemark );
		}

		/// �������캯��
		LibDate(const LibDate &date)
		{
			set( date );
		}
			  
		/// ��������
		~LibDate() {}
		
		/// ��ֵ����
		LibDate& operator=(const LibDate &date);
		/// ��ֵ����
		LibDate& operator=(const time_t &tt);

		/// ��������
		LibDate& operator+=(const LibDate &date);
		/// ��������
		LibDate& operator+=(const time_t &tt);
		
		/// �ݼ�����
		LibDate& operator-=(const LibDate &date);
		/// �ݼ�����
		LibDate& operator-=(const time_t &tt);

		/// ������λ�����
		inline int year() const {return _tm.tm_year+1900;}
		/// �����·�
		inline int month() const {return _tm.tm_mon+1;}
		/// ���ص�������
		inline int m_day() const {return _tm.tm_mday;}
		/// ���ص�������
		inline int w_day() const {return _tm.tm_wday;}
		/// ���ص�������
		inline int y_day() const {return _tm.tm_yday;}
		/// ����Сʱ
		inline int hour() const {return _tm.tm_hour;}
		/// ���ط���
		inline int min() const {return _tm.tm_min;}
		/// ��������
		inline int sec() const {return _tm.tm_sec;}
		
		/// ���� 1970-1-1 0:0:0 ����������
		inline long secs() const {return _time;}
		/// ���� 1970-1-1 0:0:0 �����ķ�����
		inline long mins() const {return ( _time/one_min );}
		/// ���� 1970-1-1 0:0:0 ������Сʱ��
		inline long hours() const {return ( _time/one_hour );}
		/// ���� 1970-1-1 0:0:0 ����������
		inline long days() const {return ( _time/one_day );}
		/// ���� 1970-1-1 0:0:0 ����������
		inline long weeks() const {return ( _time/one_week );}
		
		/// �Ե�ǰʱ�����ö���
		void set();
		/// �� time_t �������ö���
		void set( const time_t &tt );
		/// �� tm �ṹ�������ö���
		void set( const tm &st );
		/// ��ָ��ʱ�����ö���
		void set( const int year, const int mon, const int mday, 
				  const int hour=0, const int min=0, const int sec=0 );
		/// �� Date �������ö���
		void set(const LibDate &date);
		/// ��"YYYY-MM-DD HH:MM:SS"��ʽ�ַ������ö���
		void set( const string &datetime,
				  const string &datemark = "-", 
				  const string &dtmark = " ",
				  const string &timemark = ":" );
		
		/// ���� time_t ���͵Ķ���ֵ
		inline time_t value() const {return secs();}
		/// ���� struct tm ���͵Ķ���ֵ
		inline tm struct_tm() const {return _tm;}
		
		/// ��������ַ���
		string date( const string &datemark = "-", 
					 const bool leadingzero = true ) const;
					 
		/// ���ʱ���ַ���
		string time( const string &timemark = ":", 
					 const bool leadingzero = true ) const;
					 
		/// �������ʱ���ַ���
		string datetime( const string &datemark = "-", 
						 const string &dtmark = " ",
						 const string &timemark = ":",
						 const bool leadingzero = true ) const;
						 
		/// ��� GMT ��ʽ����ʱ���ַ���
		string gmt_datetime() const;

		//-----------------------------------------------------------
		/**
		*	��ʼ
		*/
		static void startRunTime(int key);
		static void startRunTime();
		/**
		*	��ȡִ��ʱ��
		*/
		static int getRunTime(int key);
		static int getRunTime();
	};
	
	// operators
	
	/*!
	\page fns ȫ�ֺ����б�
	<hr>Date ����<br><br>
	\fn <b>Date operator+( const Date &date1, const Date &date2 )</b><br>
	ʱ�����<br><br>
	\fn <b>Date operator+( const Date &date, const time_t &tt )</b><br>
	ʱ�����<br><br>
	\fn <b>Date operator-( const Date &date1, const Date &date2 )</b><br>
	ʱ�����<br><br>
	\fn <b>Date operator-( const Date &date, const time_t &tt )</b><br>
	ʱ�����<br><br>
	\fn <b>bool operator==( const Date &left, const Date &right )</b><br>
	ʱ����ȱȽ�<br><br>
	\fn <b>bool operator==( const Date &left, const time_t &right )</b><br>
	ʱ����ȱȽ�<br><br>
	\fn <b>bool operator!=( const Date &left, const Date &right )</b><br>
	ʱ�䲻��ȱȽ�<br><br>
	\fn <b>bool operator!=( const Date &left, const time_t &right )</b><br>
	ʱ�䲻��ȱȽ�<br><br>
	\fn <b>bool operator>( const Date &left, const Date &right )</b><br>
	ʱ����ڱȽ�<br><br>
	\fn <b>bool operator>( const Date &left, const time_t &right )</b><br>
	ʱ����ڱȽ�<br><br>
	\fn <b>bool operator<( const Date &left, const Date &right )</b><br>
	ʱ��С�ڱȽ�<br><br>
	\fn <b>bool operator<( const Date &left, const time_t &right )</b><br>
	ʱ��С�ڱȽ�<br><br>
	\fn <b>bool operator>=( const Date &left, const Date &right )</b><br>
	ʱ�䲻С�ڱȽ�<br><br>
	\fn <b>bool operator>=( const Date &left, const time_t &right )</b><br>
	ʱ�䲻С�ڱȽ�<br><br>
	\fn <b>bool operator<=( const Date &left, const Date &right )</b><br>
	ʱ�䲻���ڱȽ�<br><br>
	\fn <b>bool operator<=( const Date &left, const time_t &right )</b><br> 
	ʱ�䲻���ڱȽ�
	*/
	
	/// ��Ӳ���
	LibDate operator+(const LibDate &date1, const LibDate &date2);
	/// ��Ӳ���
	LibDate operator+(const LibDate &date, const time_t &tt);
	
	/// �������
	LibDate operator-(const LibDate &date1, const LibDate &date2);
	/// �������
	LibDate operator-(const LibDate &date, const time_t &tt);

	///	�ж���Ȳ���
	inline bool operator==(const LibDate &left, const LibDate &right)
	{
		return ( left.value() == right.value() );
	}
	/// �ж���Ȳ���
	inline bool operator==(const LibDate &left, const time_t &right)
	{
		return ( left.value() == right );
	}

	///	�жϲ���Ȳ���
	inline bool operator!=(const LibDate &left, const LibDate &right)
	{
		return ( left.value() != right.value() );
	}
	/// �жϲ���Ȳ���
	inline bool operator!=(const LibDate &left, const time_t &right)
	{
		return ( left.value() != right );
	}

	///	�жϴ��ڲ���
	inline bool operator>(const LibDate &left, const LibDate &right)
	{
		return ( left.value() > right.value() );
	}
	/// �жϴ��ڲ���
	inline bool operator>(const LibDate &left, const time_t &right)
	{
		return ( left.value() > right );
	}

	///	�ж�С�ڲ���
	inline bool operator<(const LibDate &left, const LibDate &right)
	{
		return ( left.value() < right.value() );
	}
	/// �ж�С�ڲ���
	inline bool operator<(const LibDate &left, const time_t &right)
	{
		return ( left.value() < right );
	}

	///	�жϲ�С�ڲ���
	inline bool operator>=(const LibDate &left, const LibDate &right)
	{
		return ( left.value() >= right.value() );
	}
	/// �жϲ�С�ڲ���
	inline bool operator>=(const LibDate &left, const time_t &right)
	{
		return ( left.value() >= right );
	}

	///	�жϲ����ڲ���
	inline bool operator<=(const LibDate &left, const LibDate &right)
	{
		return ( left.value() <= right.value() );
	}
	/// �жϲ����ڲ���
	inline bool operator<=(const LibDate &left, const time_t &right)
	{
		return ( left.value() <= right );
	}



}

#endif //_WEBDEVLIB_DATE_H_
