/// \file LibCode.cpp
/// ����,�ӽ��ܺ���ʵ���ļ�

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <cctype>
#include "LibCode.h"

using namespace std;

// WEB Application Develop Library namaspace
namespace custom
{
  ////////////////////////////////////////////////////////////////////////////
  // BASE64����
  
  // BASE���
  const char uu_base64[65] =
  {
    'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
    'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
    'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
    'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
    'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
    'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
    'w', 'x', 'y', 'z', '0', '1', '2', '3',
    '4', '5', '6', '7', '8', '9', '+', '/', '='
  };
  
  // BASE�����
  const char dec_base64[] =
  {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    62, // '+'
    0, 0, 0,
    63, // '/'
    52, 53, 54, 55, 56, 57, 58, 59, 60, 61, // '0'-'9'
    0, 0, 0, 0, 0, 0, 0,
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12,
    13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, // 'A'-'Z'
    0, 0, 0, 0, 0, 0,
    26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38,
    39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, // 'a'-'z'
  };
    
  // BASE���
  const char b64_tab[256] =
  {
    '\177', '\177', '\177', '\177', '\177', '\177', '\177', '\177', // 000-007
    '\177', '\177', '\177', '\177', '\177', '\177', '\177', '\177', // 010-017
    '\177', '\177', '\177', '\177', '\177', '\177', '\177', '\177', // 020-027
    '\177', '\177', '\177', '\177', '\177', '\177', '\177', '\177', // 030-037
    '\177', '\177', '\177', '\177', '\177', '\177', '\177', '\177', // 040-047
    '\177', '\177', '\100', '\177', '\177', '\177', '\177', '\177', // 050-057
    '\64',  '\65',  '\66',  '\67',  '\70',  '\71',  '\72',  '\73',  // 060-067
    '\74',  '\75',  '\177', '\177', '\177', '\177', '\177', '\177', // 070-077
    '\177', '\0',   '\1',   '\2',   '\3',   '\4',   '\5',   '\6',   // 100-107
    '\7',   '\10',  '\11',  '\12',  '\13',  '\14',  '\15',  '\16',  // 110-117
    '\17',  '\20',  '\21',  '\22',  '\23',  '\24',  '\25',  '\26',  // 120-127
    '\27',  '\30',  '\31',  '\177', '\177', '\177', '\177', '\177', // 130-137
    '\177', '\32',  '\33',  '\34',  '\35',  '\36',  '\37',  '\40',  // 140-147
    '\41',  '\42',  '\43',  '\44',  '\45',  '\46',  '\47',  '\50',  // 150-157
    '\51',  '\52',  '\53',  '\54',  '\55',  '\56',  '\57',  '\60',  // 160-167
    '\61',  '\62',  '\63',  '\76',  '\177', '\77',  '\177', '\177', // 170-177
    '\177', '\177', '\177', '\177', '\177', '\177', '\177', '\177', // 200-207
    '\177', '\177', '\177', '\177', '\177', '\177', '\177', '\177', // 210-217
    '\177', '\177', '\177', '\177', '\177', '\177', '\177', '\177', // 220-227
    '\177', '\177', '\177', '\177', '\177', '\177', '\177', '\177', // 230-237
    '\177', '\177', '\177', '\177', '\177', '\177', '\177', '\177', // 240-247
    '\177', '\177', '\177', '\177', '\177', '\177', '\177', '\177', // 250-257
    '\177', '\177', '\177', '\177', '\177', '\177', '\177', '\177', // 260-267
    '\177', '\177', '\177', '\177', '\177', '\177', '\177', '\177', // 270-277
    '\177', '\177', '\177', '\177', '\177', '\177', '\177', '\177', // 300-307
    '\177', '\177', '\177', '\177', '\177', '\177', '\177', '\177', // 310-317
    '\177', '\177', '\177', '\177', '\177', '\177', '\177', '\177', // 320-327
    '\177', '\177', '\177', '\177', '\177', '\177', '\177', '\177', // 330-337
    '\177', '\177', '\177', '\177', '\177', '\177', '\177', '\177', // 340-347
    '\177', '\177', '\177', '\177', '\177', '\177', '\177', '\177', // 350-357
    '\177', '\177', '\177', '\177', '\177', '\177', '\177', '\177', // 360-367
    '\177', '\177', '\177', '\177', '\177', '\177', '\177', '\177', // 370-377
  };
  
  /// \page fns ȫ�ֺ����б�
  /// \fn <hr><b>string base64_encode( const string &source )</b>
  /// 
  /// Base64����
  /// \param source ԭ�ַ���
  /// \return Sucess���ر�����,���򷵻ؿ��ַ���
  string base64_encode( const string &src ) {
    return base64_encode(src.c_str(), src.length());
  }
  
  string base64_encode( const char *src, int src_len ) {
    string enc;
    unsigned char tmp[4] = {0};
    
    for ( int i = 0; i < (int)(src_len / 3); ++i ) {
      tmp[1] = *src++;
      tmp[2] = *src++;
      tmp[3] = *src++;
      enc += uu_base64[tmp[1] >> 2];
      enc += uu_base64[((tmp[1] << 4) | (tmp[2] >> 4)) & 0x3F];
      enc += uu_base64[((tmp[2] << 2) | (tmp[3] >> 6)) & 0x3F];
      enc += uu_base64[tmp[3] & 0x3F];
      
      //if ( enc_len += 4, enc_len ==76 ) { enc += "\r\n"; enc_len = 0; }
    }
    
    //��ʣ�����ݽ��б���
    int mod = src_len % 3;
    if ( mod == 1 ) {
      tmp[1] = *src++;
      enc += uu_base64[(tmp[1] & 0xFC) >> 2];
      enc += uu_base64[((tmp[1] & 0x03) << 4)];
      enc += "==";
    } else if ( mod == 2 ) {
      tmp[1] = *src++;
      tmp[2] = *src++;
      enc += uu_base64[(tmp[1] & 0xFC) >> 2];
      enc += uu_base64[((tmp[1] & 0x03) << 4) | ((tmp[2] & 0xF0) >> 4)];
      enc += uu_base64[((tmp[2] & 0x0F) << 2)];
      enc += "=";
    }
    
    return enc;
  }
  
  /// \page fns ȫ�ֺ����б�
  /// \fn <hr><b>string base64_decode( const string &source )</b>
  /// 
  /// Base64����
  /// \param source BASE64�����ַ���
  /// \return Sucess���ؽ�����,���򷵻ؿ��ַ���
  string base64_decode( const string &src ) {
    return base64_decode(src.c_str(), src.length());
  }
  
  string base64_decode( const char *src, int src_len ) {
    string dec;
    int nvalue;
    int i = 0;
    
    while (i < src_len) {
      if ( *src != '\r' && *src !='\n' ) {
        nvalue = dec_base64[(int)*src++] << 18;
        nvalue += dec_base64[(int)*src++] << 12;
        dec += (nvalue & 0x00FF0000) >> 16;
        
        if (*src != '=') {
          nvalue += dec_base64[(int)*src++] << 6;
          dec += (nvalue & 0x0000FF00) >> 8;
          
          if (*src != '=') {
            nvalue += dec_base64[(int)*src++];
            dec += nvalue & 0x000000FF;
          }
        }
        
        i += 4;
      } else {
        src++;
        i++;
      }
    }
    
    return dec;
  }
  
  ////////////////////////////////////////////////////////////////////////////
  // Sina�ַ������ܽ���
  
  // ���ܳ���
  const int ENC_TIME_LEN = 20;
  // ���ܳ���
  const int ENC_STRING_LEN = 260;
  // ���ܳ���
  const int ENC_TOTAL_TABLE = 50;
  
  // ��Կ
  char LP_ENC_TABLE[]=
  {
    "-/0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ_!abcdefghijklmnopqrstuvwxyz\0"
    "tnmWkUJlYHfjdiyu_V58hDr!pcb6XAzSTFLRw73G9O2vMg0Q/EK4ZxeNC1BqsIPao-\0"
    "qxZLKTD4w8gEv/os3FlfQu1GN2OYRd-zSV!APHU5jbkIp0mXn_rJ7aMcyWiCBh69et\0"
    "Ca7!rSuJPf0_IneBUV4gXsW5ckbwAvt-zFGd68TlEOKhNmZRx31DH9p2oL/yiQYjMq\0"
    "y9HG1zsD6gm3hxMiTFJ0RBLl2KOPdIqt-V5vYfS4_brQcZ7AaUWu8jNke!no/XwEpC\0"
    "oj85W-BuY0ZUQap/SVDmAi!4krb6vhCqtFlIwgzJ3O1X27Hd9TLsfEcKMGxenRP_Ny\0"
    "eEflLtiswm7TX9NnzFuZd/GJK1OYIQyCqV4hP0-DUbWRkH8vjS!Bg_2rp5aMxA63co\0"
    "M_g4!q/BPZHSRjcx-Vs7vn5DrWbwhXoyCFJQ6mtuTOLAK8fIEzGi03k1Nl9padYU2e\0"
    "p30JGCni678zAE2atFBHIxlu1LOPQReoyVDXYZqsSb!drfgh_-5/mUKWc4jN9vwTkM\0"
    "NUmD5yx/YHf-d_k9qVi8ha4sW!b6XAMeoFuRw7CBzOGv1g0Q3tlnZTrL2JEcjIPSKp\0"
    "cTZuloanw8gtv3KjCF/fQ9JBLGOYRdpMeVsAPHyi-b5IW0mXUq4x7S1!kD_2Eh6zrN\0"
    "2S7s4e9xPf0qIUrEyVngXjDi!5bwAvNpMFBd68o/tOlhLmZRTCJaHzWGKu3k_QY-1c\0"
    "kzHBJMja6gmChT1_oFx0REu/GlOPdIcNpVivYfenqb4Q!Z7ASyD98-L5rsUK3XwtW2\0"
    "K-8iDpE9Y0ZyQSW3eVamA_sn54b6vh2cNF/IwgMxCOJXG7Hdzoujft!l1BTrURPqLk\0"
    "rtf/uN_jwm7oXzLUMF9Zd3BxlJOYIQk2cVnhP0paybDR5H8v-esEgqG4WiS1TA6C!K\0"
    "1qgnsc3EPZHeR-!TpVj7vUia4DbwhXKk2FxQ6mN9oOuAl8fItMB_0C5JL/zWSdYyGr\0"
    "WC0xB2U_678MAtGSNFEHIT/9JuOPQRrKkVaXYZcjebsd4fghqpi3mylD!n-Lzvwo51\0"
    "LymaikT3YHfpdq5zcV_8hSnjDsb6XA1rKF9Rw72EMOBvJg0QCN/UZo4uGxt!-IPelW\0"
    "!oZ9/KSUw8gNvCl-2F3fQzxEuBOYRdW1rVjAPHk_pbiID0mXycnT7eJs5aqGth6M4L\0"
    "Ge7jnrzTPf0cIy4tkVUgX-a_sibwAvLW1FEd68K3NO/humZRo2xSHMDBl9C5qQYpJ!\0"
    "5MHEx1-S6gm2hoJqKFT0Rt93B/OPdI!LWV_vYfrUcbnQsZ7Aekaz8pui4jylCXwNDG\0"
    "lp8_aWtzY0ZkQeDCrVSmAqjUinb6vhG!LF3Iwg1T2OxXB7HdMK9-fNs/JEo4yRPcu5\0"
    "4Nf39Lq-wm7KXMuy1FzZdCET/xOYIQ5G!VUhP0WSkbaRiH8vprjtgcBnD_eJoA62sl\0"
    "JcgUj!CtPZHrRpsoWV-7vy_SnabwhXl5GFTQ6mLzKO9A/8fIN1Eq02ixu3MDedYkB4\0"
    "D20TEGyq6781ANBeLFtHIo3zx9OPQR4l5VSXYZ!-rbjdnfghcW_Cmk/asUpuMvwKiJ\0"
    "ukmS_5oCYHfWdciM!Vq8heU-ajb6XAJ4lFzRw7Gt1OEvxg0Q2L3yZKn9BTNspIPr/D\0"
    "sKZz3leyw8gLv2/pGFCfQMTt9EOYRdDJ4V-APH5qWb_Ia0mXk!Uo7rxjiScBNh61nu\0"
    "Br7-U4MoPf0!IknN5VygXpSqj_bwAvuDJFtd68lCLO3h9mZRKGTeH1aE/z2icQYWxs\0"
    "i1HtTJpe6gmGhKxclFo0RNzCE3OPdIsuDVqvYf4y!bUQjZ7Ar5SM8W9_n-k/2XwLaB\0"
    "/W8qSDNMY0Z5Qra24VemAc-y_Ub6vhBsuFCIwgJoGOTXE7Hd1lzpfLj3xtKnkRP!9i\0"
    "nLfCzucpwm7lX19kJFMZd2to3TOYIQiBsVyhP0De5bSR_H8vW4-Ng!EUaqrxKA6Gj/\0"
    "x!gy-s2NPZH4RWjKDVp7vkqeUSbwhX/iBFoQ6muMlOzA38fILJtc0G_T9C1ardY5En\0"
    "aG0otBkc678JALEruFNHIKCMTzOPQRn/iVeXYZsp4b-dUfgh!Dq2m53SjyW91vwl_x\0"
    "95meqiK2YHfDd!_1sVc8hrypS-b6XAxn/FMRw7BNJOtvTg0QGuCkZlUzEoLjWIP43a\0"
    "jlZMC/rkw8guvG3WBF2fQ1oNztOYRdaxnVpAPHicDbqIS0mX5syK74T-_e!ELh6JU9\0"
    "E47pyn1KPf0sI5ULiVkgXWec-qbwAv9axFNd68/2uOChzmZRlBorHJSt3MG_!QYDTj\0"
    "_JHNoxWr6gmBhlT!/FK0RLM2tCOPdIj9aVcvYfnksbyQ-Z7A4ie18DzqUp53GXwuSE\0"
    "3D8ceaL1Y0ZiQ4SGnVrmA!pkqyb6vhEj9F2IwgxKBOoXt7HdJ/MWfu-CTNlU5RPsz_\0"
    "Uuf2M9!Wwm7/XJz5xF1ZdGNKCoOYIQ_EjVkhP0aribeRqH8vDnpLgstySc4TlA6B-3\0"
    "TsgkpjGLPZHnRD-laVW7v5cryebwhX3_EFKQ6m91/OMAC8fIuxN!0Bqoz2JS4dYitU\0"
    "SB0KNE5!678xAut49FLHIl21oMOPQRU3_VrXYZjWnbpdyfghsacGmiCe-kDzJvw/qT\0"
    "zimrc_lGYHfadsqJjV!8h4kWepb6XATU3F1Rw7ELxONvog0QB925Z/yMtKu-DIPnCS\0"
    "-/Z12345w8g9vBCDEFGfQJKLMNOYRdSTUVWAPH_!abcIe0mXijkl7nopqrstuh6xyz\0"
    "tn7WkUJlPf0jIiyu_V5gXDr!pcbwAvzSTFLd683G9O2hMmZR/EK4HxeNC1BqsQYao-\0"
    "qxHLKTD46gmEh/os3Fl0Ru1GN2OPdI-zSV!vYfU5jbkQpZ7An_rJ8aMcyWiCBXw9et\0"
    "Ca8!rSuJY0Z_QneBUV4mAsW5ckb6vht-zFGIwgTlEOKXN7Hdx31Df9p2oL/yiRPjMq\0"
    "y9fG1zsDwm73XxMiTFJZdBLl2KOYIQqt-V5hP0S4_brRcH8vaUWugjNke!no/A6EpC\0"
    "ojg5W-BuPZHURap/SVD7vi!4krbwhXCqtFlQ6mzJ3O1A28fI9TLs0EcKMGxendY_Ny\0"
    "eE0lLtis678TA9NnzFuHI/GJK1OPQRyCqV4XYZ-DUbWdkfghjS!Bm_2rp5aMxvw3co\0"
    "M_m4!q/BYHfSdjcx-Vs8hn5DrWb6XAoyCFJRw7tuTOLvKg0QEzGiZ3k1Nl9paIPU2e\0"
    "p3ZJGCniw8gzvE2atFBfQxlu1LOYRdeoyVDAPHqsSb!Ir0mX_-5/7UKWc4jN9h6TkM\0"
  };
  
  /// \page fns ȫ�ֺ����б�
  /// \fn <hr><b>string sina_encrypt( const string &source )</b>
  /// 
  /// Sina�ַ�������
  /// \param source ԭ�ַ���
  /// \return ����Sucess���ؼ��ܽ��,���򷵻ؿ��ַ���
  string vogso_encrypt( const string &source )
  {
    if ( source == "" ) return "";
    const char *lpSrcStr = source.c_str();
    string target = "";
    
    char psEncrypt[ENC_STRING_LEN];
    if ( !lpSrcStr || !*lpSrcStr ) return target;
    
    unsigned int nSrcLen = strlen( lpSrcStr );
    if( nSrcLen > ENC_STRING_LEN-60 ) return target;

    unsigned long nCheckSum = 0;
    unsigned int nDestSite = 0;
    unsigned int nDestLen = 0;
    char lpDestStr[ENC_STRING_LEN];
    time_t m_time = 0;
    char lpTimeStr[ENC_TIME_LEN];
    unsigned int nTableNum = 0;
    unsigned int nTableSite = 0;
    unsigned int nTableLen = strlen( LP_ENC_TABLE );
    unsigned int nMaskNum = 0;
    unsigned int nMaskSite = 0;
    char *lpTableStr = NULL;
    char *lpMaskStr = NULL;

    //-----------------------------------
    //Step 1
    //Get Local Time and Make Time String

    time( &m_time );
    sprintf( lpTimeStr, "%010ld", m_time );

    //-----------------------------------
    //Step 2
    //Create Table Number and Serial Number by Local Time

    srand( (unsigned)time(NULL) );
    nTableNum = rand() % ENC_TOTAL_TABLE + 1;
    nMaskNum = rand() % ENC_TOTAL_TABLE + 1;
    lpTableStr = LP_ENC_TABLE + nTableNum * (nTableLen+1);
    lpMaskStr = LP_ENC_TABLE + nMaskNum * (nTableLen+1);
    
    //-----------------------------------
    //Step 3
    //check sum

    nCheckSum = 0;
    int i = strlen( lpTimeStr );
    while( i-- ) nCheckSum += *(lpTimeStr+i);
    i = strlen( lpSrcStr );
    while( i-- ) nCheckSum += *(lpSrcStr+i);
    
    //-----------------------------------
    //Step 4
    //assumble 
    
    char cc = 0;
    char dd = 0;

    sprintf( lpDestStr, "%s%s%04ld", lpTimeStr, lpSrcStr, nCheckSum );
    nDestLen = strlen( lpDestStr );
    nMaskSite = 0;
    while( (cc=*(lpMaskStr + nMaskSite++)) ) 
    {
      nDestSite = cc % nDestLen;
      dd = *(lpDestStr + nDestSite);
      memcpy( lpDestStr+nDestSite, lpDestStr+nDestSite+1, nDestLen-nDestSite-1);
      *( lpDestStr+nDestLen-1) = dd;
    }     

    //-----------------------------------
    //Step 5
    //transfer by selected Table Number
    
    nDestSite = 0;
    while( (cc=*(lpDestStr + nDestSite)) ) 
    {
      nTableSite = 0;
      char dd = 0;
      while( (dd=*(LP_ENC_TABLE + nTableSite)) ) 
      {
        if ( cc == dd )
          *(lpDestStr + nDestSite) = *( lpTableStr + nTableSite );
        nTableSite++;
      }
      nDestSite++;
    }

    //-----------------------------------
    //Step 6
    //return
    
    strcpy( psEncrypt, lpDestStr );
    sprintf( lpDestStr, "%c%c%s", (char)nTableNum+64, nMaskNum+64, psEncrypt );
    strcpy( psEncrypt, lpDestStr );
    
    target = psEncrypt;
    return target;
  }
  
  /// \page fns ȫ�ֺ����б�
  /// \fn <hr><b>string sina_decrypt( const string &source )</b>
  /// 
  /// Sina�ַ�������
  /// \param source �����ַ���
  /// \return ����Sucess���ؽ��ܽ��,��ʽΪ <����ʱ��,��λ��><&><ԭ�ַ���> ,���򷵻ؿ��ַ���
  string vogso_decrypt( const string &source )
  {
    if ( source == "" ) return "";
    const char *lpSrcStr = source.c_str();
    string target = "";

    char psDecrypt[ENC_STRING_LEN];
    if ( !lpSrcStr || !*lpSrcStr ) return target;

    unsigned int nSrcLen = strlen( lpSrcStr );
    if ( nSrcLen > (unsigned int)ENC_STRING_LEN ) return target;

    unsigned int nTableNum = 0;
    unsigned int nTableSite = 0;
    unsigned int nTableLen = strlen( LP_ENC_TABLE );
    char *lpTable = NULL;
    unsigned int nMaskNum = 0;
    unsigned int nMaskSite = 0;
    char *lpMaskStr = NULL;
    unsigned long nCheckSum = 0;
    char *lpCheckStr = NULL;
    time_t m_time = 0;
    time_t enc_time = 0;
    unsigned int nDestLen = 0;
    unsigned int nDestSite = ENC_STRING_LEN;
    char lpDestStr[ENC_STRING_LEN];
    char cc = 0;
    char dd = 0;
    unsigned int i = 0;

    //-----------------------------------
    //Step 1
    //Get Table Number from Source String

    if ( *(lpSrcStr) < 65 ) return target;
    nTableNum = *(lpSrcStr)-64;
    if ( nTableNum > (unsigned int)ENC_TOTAL_TABLE ) return target;

    //-----------------------------------
    //Step 2
    //Read Table Content by Table Number

    lpTable = LP_ENC_TABLE + nTableNum * (nTableLen+1);

    //-----------------------------------
    //Step 3
    //Transfer from encrypt to Normal text

    sprintf( lpDestStr, "%s", lpSrcStr+2 );
    nDestLen = strlen( lpDestStr );
    nDestSite = 0;
    while( (cc=*(lpDestStr + nDestSite)) ) 
    {
      nTableSite = 0;
      while( (dd=*(lpTable + nTableSite)) ) 
      {
        if ( cc == dd ) 
          *(lpDestStr + nDestSite) = *(LP_ENC_TABLE + nTableSite );
        nTableSite++;
      }
      nDestSite++;
    }

    //-----------------------------------
    //Step 4
    //Get Serial Number from Source String

    if ( *(lpSrcStr+1) < 65 ) goto EXIT;
    nMaskNum = *(lpSrcStr+1)-64;
    if ( nMaskNum > (unsigned int)ENC_TOTAL_TABLE ) goto EXIT;

    //-----------------------------------
    //Step 5
    //Read Serial String by Serial Number
    
    lpMaskStr = LP_ENC_TABLE + nMaskNum * (nTableLen+1);

    //-----------------------------------
    //Step 6
    //Recombination Time , String & CheckSum

    nMaskSite = nTableLen;
    while( nMaskSite-- ) 
    {
      cc = *(lpMaskStr + nMaskSite);
      nDestSite = cc % nDestLen;
      dd = *(lpDestStr + nDestLen - 1);

      for( i = nDestLen-2 ; i >= nDestSite ; ) 
      {
        *(lpDestStr + i + 1) = *(lpDestStr + i);
        if ( i > 0 ) i--;
        else break;
      }

      *(lpDestStr + nDestSite) = dd;
    }

    //-----------------------------------
    //Step 7
    //Recalculate CheckSum , Check it
    
    nCheckSum = 0;
    i = strlen( lpDestStr ) - 4;
    while( i-- ) nCheckSum += *(lpDestStr+i);
    lpCheckStr = lpDestStr + nDestLen - 4;
    
    // comment for longer encrypted string error 2002/7/31
    //if ( nCheckSum != (unsigned int)atoi(lpCheckStr) ) goto EXIT;
    
    *lpCheckStr = 0;

    //-----------------------------------
    //Step 8
    //Get Local Time , Check Time Different
    
    time( &m_time );
    dd = *(lpDestStr + 10);
    *(lpDestStr + 10) = 0;
    enc_time = atol( lpDestStr );
    
    // comment for valid encrypted time 2002/7/31
    //if ( labs(m_time - enc_time) > 3600*ENC_VALID_HOUR ) goto EXIT;
    
    //-----------------------------------
    //Step 9
    //return
    
    strcpy( psDecrypt, lpDestStr+11 );
    sprintf( lpDestStr, "%ld&%c%s", (long)enc_time, dd, psDecrypt );
    strcpy( psDecrypt, lpDestStr );
    
    EXIT:
    target = psDecrypt;
    return target;
  }

  ////////////////////////////////////////////////////////////////////////////
  // URI�������

  // HEX�ַ�ת��ΪASC�ַ�
  // �ɺ���uri_decode()����
  char hex_to_asc( const string &src )
  {
    char digit;
    digit = ( src[0]>='A' ? ((src[0]&0xdf)-'A')+10 : (src[0]-'0') );
    digit *= 16;
    digit += ( src[1]>='A' ? ((src[1]&0xdf)-'A')+10 : (src[1]-'0') );
    
    return digit;
  }

  /// \page fns ȫ�ֺ����б�
  /// \fn <hr><b>string uri_encode( const string &source )</b>
  ///
  /// URI����
  /// \param source ԭ�ַ���
  /// \return �������ַ���
  string uri_encode( const string &source )
  {
    const char hextag = '%';
    const char reserved[] = ";/?:@&=+\0";
    const char unsafe[] = " \"#%<>\0";
    const char other[] = "'`[],~!$^(){}|\\";

    char element[3];
    char chr;
    string res;
    for ( unsigned int i=0; i<source.length(); i++ )
    {
      chr = source[i];
      //if ( iscntrl(chr) || strchr(reserved,chr) 
      //modify by minghui
      if ( strchr(reserved,chr) 
         || strchr(unsafe,chr) || strchr(other,chr) )
      {
        sprintf( element, "%c%02X", hextag, chr ); 
        res += element;
      }
      else
      {
        res += chr;
      }
    }
    
    return res;
  }
  
  /// \page fns ȫ�ֺ����б�
  /// \fn <hr><b>string uri_encode_new( const string &source )</b>
  ///
  /// URI����
  /// \param source ԭ�ַ���
  /// \return �������ַ���
  string uri_encode_new( const string &source ) {
    const char hextag = '%';
    
    char element[8];
    char chr;
    string res;
    for ( unsigned int i = 0; i < source.length(); i++ ) {
      chr = source[i];
      
      sprintf( element, "%c%02X\n", hextag, chr );
      res += element;
    }
    
    return res;
  }
  
  /// \page fns ȫ�ֺ����б�
  /// \fn <hr><b>string uri_decode( const string &source )</b>
  ///
  /// URI����
  /// \param source URI�����ַ���
  /// \return ������
  string uri_decode( const string &source )
  {
    const char hextag = '%';
    string::size_type pos = 0;
    string seq;
    string str = source;
    string rest = str;
    
    for ( str=""; ((pos=rest.find(hextag))!=rest.npos); )
    {
      if ( (pos+2)<rest.length() && isalnum(rest[pos+1]) && isalnum(rest[pos+2]) )
      {
        seq = rest.substr( pos+1, 2 );
        str = str + rest.substr(0,pos) + hex_to_asc(seq);
        rest = rest.erase( 0, pos+3 );
      }
      else
      {
        str = str + rest.substr(0, pos+1);
        rest = rest.erase(0, pos+1);
      }
    }

    str += rest;
    return str;
  }
}
