/// \file LibCode.h
/// ����,�ӽ��ܺ���ͷ�ļ�
/// BASE64����,�ַ������ܽ���,URI���뺯��
   
#ifndef _WEBDEVLIB_CODE_H_
#define _WEBDEVLIB_CODE_H_ 

#include <string>


// WEB Application Develop Library namaspace
namespace custom
{
  /// Base64����
	std::string base64_encode(const std::string& src);
	std::string base64_encode(const char *src, int src_len);
  
  /// Base64����
	std::string base64_decode(const std::string &src);
	std::string base64_decode(const char *src, int src_len);
  
  /// ��˫�ַ�������
	std::string vogso_encrypt(const std::string &source);
  
  /// ��˫�ַ�������
	std::string vogso_decrypt(const std::string &source);

  /// URI����
	std::string uri_encode(const std::string &source);
  
  /// URI���� (��ȫ���ַ����б���)
	std::string uri_encode_new(const std::string &source);
  
  /// URI����
	std::string uri_decode(const std::string &source);
}

#endif //_WEBDEVLIB_CODE_H_
