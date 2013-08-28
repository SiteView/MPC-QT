/*
	stdsoap2.c[pp] 2.8.14

	gSOAP runtime engine

gSOAP XML Web services tools
Copyright (C) 2000-2013, Robert van Engelen, Genivia Inc., All Rights Reserved.
This part of the software is released under ONE of the following licenses:
GPL, or the gSOAP public license, or Genivia's license for commercial use.
--------------------------------------------------------------------------------
Contributors:

Wind River Systems Inc., for the following additions under gSOAP public license:
  - vxWorks compatible options
--------------------------------------------------------------------------------
gSOAP public license.

The contents of this file are subject to the gSOAP Public License Version 1.3
(the "License"); you may not use this file except in compliance with the
License. You may obtain a copy of the License at
http://www.cs.fsu.edu/~engelen/soaplicense.html
Software distributed under the License is distributed on an "AS IS" basis,
WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License
for the specific language governing rights and limitations under the License.

The Initial Developer of the Original Code is Robert A. van Engelen.
Copyright (C) 2000-2013, Robert van Engelen, Genivia Inc., All Rights Reserved.
--------------------------------------------------------------------------------
GPL license.

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free Software
Foundation; either version 2 of the License, or (at your option) any later
version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with
this program; if not, write to the Free Software Foundation, Inc., 59 Temple
Place, Suite 330, Boston, MA 02111-1307 USA

Author contact information:
engelen@genivia.com / engelen@acm.org

This program is released under the GPL with the additional exemption that
compiling, linking, and/or using OpenSSL is allowed.
--------------------------------------------------------------------------------
A commercial use license is available from Genivia, Inc., contact@genivia.com
--------------------------------------------------------------------------------
*/

#define GSOAP_LIB_VERSION 20814

#ifdef AS400
# pragma convert(819)	/* EBCDIC to ASCII */
#endif

#include "stdsoap2.h"
#if defined(VXWORKS) && defined(WM_SECURE_KEY_STORAGE)
#include <ipcom_key_db.h>
#endif
#if GSOAP_VERSION != GSOAP_LIB_VERSION
# error "GSOAP VERSION MISMATCH IN LIBRARY: PLEASE REINSTALL PACKAGE"
#endif

#ifdef __BORLANDC__
# pragma warn -8060
#else
# ifdef WIN32
#  ifdef UNDER_CE
#   pragma comment(lib, "ws2.lib")	/* WinCE */
#  else
#   pragma comment(lib, "ws2_32.lib")
#  endif
#  pragma warning(disable : 4996) /* disable deprecation warnings */
# endif
#endif

#ifdef __cplusplus
SOAP_SOURCE_STAMP("@(#) stdsoap2.cpp ver 2.8.14 2013-02-04 00:00:00 GMT")
extern "C" {
#else
SOAP_SOURCE_STAMP("@(#) stdsoap2.c ver 2.8.14 2013-02-04 00:00:00 GMT")
#endif

/* 8bit character representing unknown/nonrepresentable character data (e.g. not supported by current locale with multibyte support enabled) */
#ifndef SOAP_UNKNOWN_CHAR
#define SOAP_UNKNOWN_CHAR (127)
#endif

/*      EOF=-1 */
#define SOAP_LT (soap_wchar)(-2) /* XML-specific '<' */
#define SOAP_TT (soap_wchar)(-3) /* XML-specific '</' */
#define SOAP_GT (soap_wchar)(-4) /* XML-specific '>' */
#define SOAP_QT (soap_wchar)(-5) /* XML-specific '"' */
#define SOAP_AP (soap_wchar)(-6) /* XML-specific ''' */

#define soap_blank(c)		((c)+1 > 0 && (c) <= 32)
#define soap_notblank(c)	((c) > 32)

#if defined(WIN32) && !defined(UNDER_CE)
#define soap_hash_ptr(p)	((PtrToUlong(p) >> 3) & (SOAP_PTRHASH - 1))
#else
#define soap_hash_ptr(p)	((size_t)(((unsigned long)(p) >> 3) & (SOAP_PTRHASH-1)))
#endif

#if !defined(WITH_LEAN) || defined(SOAP_DEBUG)
static void soap_init_logs(struct soap*);
#endif
#ifdef SOAP_DEBUG
static void soap_close_logfile(struct soap*, int);
static void soap_set_logfile(struct soap*, int, const char*);
#endif

#ifdef SOAP_MEM_DEBUG
static void soap_init_mht(struct soap*);
static void soap_free_mht(struct soap*);
static void soap_track_unlink(struct soap*, const void*);
#endif

#ifndef PALM_2
static int soap_set_error(struct soap*, const char*, const char*, const char*, const char*, int);
static int soap_copy_fault(struct soap*, const char*, const char*, const char*, const char*);
static int soap_getattrval(struct soap*, char*, size_t, soap_wchar);
#endif

#ifndef PALM_1
static void soap_free_ns(struct soap *soap);
static soap_wchar soap_char(struct soap*);
static soap_wchar soap_get_pi(struct soap*);
static int soap_isxdigit(int);
static void *fplugin(struct soap*, const char*);
static size_t soap_count_attachments(struct soap *soap);
static int soap_try_connect_command(struct soap*, int http_command, const char *endpoint, const char *action);
#ifdef WITH_NTLM
static int soap_ntlm_handshake(struct soap *soap, int command, const char *endpoint, const char *host, int port);
#endif
#ifndef WITH_NOIDREF
static int soap_has_copies(struct soap*, const char*, const char*);
static void soap_init_iht(struct soap*);
static void soap_free_iht(struct soap*);
static void soap_init_pht(struct soap*);
static void soap_free_pht(struct soap*);
#endif
#endif

#ifndef WITH_LEAN
static const char *soap_set_validation_fault(struct soap*, const char*, const char*);
static int soap_isnumeric(struct soap*, const char*);
static struct soap_nlist *soap_push_ns(struct soap *soap, const char *id, const char *ns, short utilized);
static void soap_utilize_ns(struct soap *soap, const char *tag);
#endif

#ifndef WITH_LEANER
#ifndef PALM_1
static struct soap_multipart *soap_new_multipart(struct soap*, struct soap_multipart**, struct soap_multipart**, char*, size_t);
static int soap_putdimefield(struct soap*, const char*, size_t);
static char *soap_getdimefield(struct soap*, size_t);
static void soap_select_mime_boundary(struct soap*);
static int soap_valid_mime_boundary(struct soap*);
static void soap_resolve_attachment(struct soap*, struct soap_multipart*);
#endif
#endif

#ifdef WITH_GZIP
static int soap_getgziphdr(struct soap*);
#endif

#ifdef WITH_OPENSSL
# ifndef SOAP_SSL_RSA_BITS
#  define SOAP_SSL_RSA_BITS 2048
# endif
static int soap_ssl_init_done = 0;
static int ssl_auth_init(struct soap*);
static int ssl_verify_callback(int, X509_STORE_CTX*);
static int ssl_verify_callback_allow_expired_certificate(int, X509_STORE_CTX*);
static int ssl_password(char*, int, int, void *);
#endif

#ifdef WITH_GNUTLS
# ifndef SOAP_SSL_RSA_BITS
#  define SOAP_SSL_RSA_BITS 2048
# endif
static int soap_ssl_init_done = 0;
static const char *ssl_verify(struct soap *soap, const char *host);
# if defined(HAVE_PTHREAD_H)
#  include <pthread.h>
   /* make GNUTLS thread safe with pthreads */
   GCRY_THREAD_OPTION_PTHREAD_IMPL;
# elif defined(HAVE_PTH_H)
   #include <pth.h>
   /* make GNUTLS thread safe with PTH */
   GCRY_THREAD_OPTION_PTH_IMPL;
# endif
#endif

#if !defined(WITH_NOHTTP) || !defined(WITH_LEANER)
#ifndef PALM_1
static const char *soap_decode(char*, size_t, const char*, const char*);
#endif
#endif

#ifndef WITH_NOHTTP
#ifndef PALM_1
static soap_wchar soap_getchunkchar(struct soap*);
static const char *http_error(struct soap*, int);
static int http_get(struct soap*);
static int http_405(struct soap*);
static int http_200(struct soap*);
static int http_post(struct soap*, const char*, const char*, int, const char*, const char*, size_t);
static int http_send_header(struct soap*, const char*);
static int http_post_header(struct soap*, const char*, const char*);
static int http_response(struct soap*, int, size_t);
static int http_parse(struct soap*);
static int http_parse_header(struct soap*, const char*, const char*);
#endif
#endif

#ifndef WITH_NOIO

#ifndef PALM_1
static int fsend(struct soap*, const char*, size_t);
static size_t frecv(struct soap*, char*, size_t);
static int tcp_init(struct soap*);
static const char *tcp_error(struct soap*);
#ifndef WITH_IPV6
static int tcp_gethost(struct soap*, const char *addr, struct in_addr *inaddr);
#endif
static SOAP_SOCKET tcp_connect(struct soap*, const char *endpoint, const char *host, int port);
static SOAP_SOCKET tcp_accept(struct soap*, SOAP_SOCKET, struct sockaddr*, int*);
static int tcp_select(struct soap*, SOAP_SOCKET, int, int);
static int tcp_disconnect(struct soap*);
static int tcp_closesocket(struct soap*, SOAP_SOCKET);
static int tcp_shutdownsocket(struct soap*, SOAP_SOCKET, int);
static const char *soap_strerror(struct soap*);
#endif

#define SOAP_TCP_SELECT_RCV 0x1
#define SOAP_TCP_SELECT_SND 0x2
#define SOAP_TCP_SELECT_ERR 0x4
#define SOAP_TCP_SELECT_ALL 0x7

#if defined(WIN32)
  #define SOAP_SOCKBLOCK(fd) \
  { u_long blocking = 0; \
    ioctlsocket(fd, FIONBIO, &blocking); \
  }
  #define SOAP_SOCKNONBLOCK(fd) \
  { u_long nonblocking = 1; \
    ioctlsocket(fd, FIONBIO, &nonblocking); \
  }
#elif defined(VXWORKS)
  #define SOAP_SOCKBLOCK(fd) \
  { u_long blocking = 0; \
    ioctl(fd, FIONBIO, (int)(&blocking)); \
  }
  #define SOAP_SOCKNONBLOCK(fd) \
  { u_long nonblocking = 1; \
    ioctl(fd, FIONBIO, (int)(&nonblocking)); \
  }
#elif defined(__VMS)
  #define SOAP_SOCKBLOCK(fd) \
  { int blocking = 0; \
    ioctl(fd, FIONBIO, &blocking); \
  }
  #define SOAP_SOCKNONBLOCK(fd) \
  { int nonblocking = 1; \
    ioctl(fd, FIONBIO, &nonblocking); \
  }
#elif defined(PALM)
  #define SOAP_SOCKBLOCK(fd) fcntl(fd, F_SETFL, fcntl(fd, F_GETFL, 0)&~O_NONBLOCK);
  #define SOAP_SOCKNONBLOCK(fd) fcntl(fd, F_SETFL, fcntl(fd, F_GETFL, 0)|O_NONBLOCK);
#elif defined(SYMBIAN)
  #define SOAP_SOCKBLOCK(fd) \
  { long blocking = 0; \
    ioctl(fd, 0/*FIONBIO*/, &blocking); \
  }
  #define SOAP_SOCKNONBLOCK(fd) \
  { long nonblocking = 1; \
    ioctl(fd, 0/*FIONBIO*/, &nonblocking); \
  }
#else
  #define SOAP_SOCKBLOCK(fd) fcntl(fd, F_SETFL, fcntl(fd, F_GETFL)&~O_NONBLOCK);
  #define SOAP_SOCKNONBLOCK(fd) fcntl(fd, F_SETFL, fcntl(fd, F_GETFL)|O_NONBLOCK);
#endif

#endif

#if defined(PALM) && !defined(PALM_2)
unsigned short errno;
#endif

#ifndef PALM_1
static const char soap_env1[42] = "http://schemas.xmlsoap.org/soap/envelope/";
static const char soap_enc1[42] = "http://schemas.xmlsoap.org/soap/encoding/";
static const char soap_env2[40] = "http://www.w3.org/2003/05/soap-envelope";
static const char soap_enc2[40] = "http://www.w3.org/2003/05/soap-encoding";
static const char soap_rpc[35] = "http://www.w3.org/2003/05/soap-rpc";
#endif

#ifndef PALM_1
const union soap_double_nan soap_double_nan = {{0xFFFFFFFF, 0xFFFFFFFF}};
const char soap_base64o[65] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
const char soap_base64i[81] = "\76XXX\77\64\65\66\67\70\71\72\73\74\75XXXXXXX\00\01\02\03\04\05\06\07\10\11\12\13\14\15\16\17\20\21\22\23\24\25\26\27\30\31XXXXXX\32\33\34\35\36\37\40\41\42\43\44\45\46\47\50\51\52\53\54\55\56\57\60\61\62\63";
#endif

#ifndef WITH_LEAN
static const char soap_indent[11] = "\n\t\t\t\t\t\t\t\t\t";
/* Alternative indentation form for SOAP_XML_INDENT:
static const char soap_indent[21] = "\n                   ";
*/
#endif

#ifndef SOAP_CANARY
# define SOAP_CANARY (0xC0DE)
#endif

static const char soap_padding[4] = "\0\0\0";
#define SOAP_STR_PADDING (soap_padding)
#define SOAP_STR_EOS (soap_padding)
#define SOAP_NON_NULL (soap_padding)

#ifndef WITH_LEAN
static const struct soap_code_map html_entity_codes[] = /* entities for XHTML parsing */
{ { 160, "nbsp" },
  { 161, "iexcl" },
  { 162, "cent" },
  { 163, "pound" },
  { 164, "curren" },
  { 165, "yen" },
  { 166, "brvbar" },
  { 167, "sect" },
  { 168, "uml" },
  { 169, "copy" },
  { 170, "ordf" },
  { 171, "laquo" },
  { 172, "not" },
  { 173, "shy" },
  { 174, "reg" },
  { 175, "macr" },
  { 176, "deg" },
  { 177, "plusmn" },
  { 178, "sup2" },
  { 179, "sup3" },
  { 180, "acute" },
  { 181, "micro" },
  { 182, "para" },
  { 183, "middot" },
  { 184, "cedil" },
  { 185, "sup1" },
  { 186, "ordm" },
  { 187, "raquo" },
  { 188, "frac14" },
  { 189, "frac12" },
  { 190, "frac34" },
  { 191, "iquest" },
  { 192, "Agrave" },
  { 193, "Aacute" },
  { 194, "Acirc" },
  { 195, "Atilde" },
  { 196, "Auml" },
  { 197, "Aring" },
  { 198, "AElig" },
  { 199, "Ccedil" },
  { 200, "Egrave" },
  { 201, "Eacute" },
  { 202, "Ecirc" },
  { 203, "Euml" },
  { 204, "Igrave" },
  { 205, "Iacute" },
  { 206, "Icirc" },
  { 207, "Iuml" },
  { 208, "ETH" },
  { 209, "Ntilde" },
  { 210, "Ograve" },
  { 211, "Oacute" },
  { 212, "Ocirc" },
  { 213, "Otilde" },
  { 214, "Ouml" },
  { 215, "times" },
  { 216, "Oslash" },
  { 217, "Ugrave" },
  { 218, "Uacute" },
  { 219, "Ucirc" },
  { 220, "Uuml" },
  { 221, "Yacute" },
  { 222, "THORN" },
  { 223, "szlig" },
  { 224, "agrave" },
  { 225, "aacute" },
  { 226, "acirc" },
  { 227, "atilde" },
  { 228, "auml" },
  { 229, "aring" },
  { 230, "aelig" },
  { 231, "ccedil" },
  { 232, "egrave" },
  { 233, "eacute" },
  { 234, "ecirc" },
  { 235, "euml" },
  { 236, "igrave" },
  { 237, "iacute" },
  { 238, "icirc" },
  { 239, "iuml" },
  { 240, "eth" },
  { 241, "ntilde" },
  { 242, "ograve" },
  { 243, "oacute" },
  { 244, "ocirc" },
  { 245, "otilde" },
  { 246, "ouml" },
  { 247, "divide" },
  { 248, "oslash" },
  { 249, "ugrave" },
  { 250, "uacute" },
  { 251, "ucirc" },
  { 252, "uuml" },
  { 253, "yacute" },
  { 254, "thorn" },
  { 255, "yuml" },
  {   0, NULL }
};
#endif

#ifndef WITH_NOIO
#ifndef WITH_LEAN
static const struct soap_code_map h_error_codes[] =
{
#ifdef HOST_NOT_FOUND
  { HOST_NOT_FOUND, "Host not found" },
#endif
#ifdef TRY_AGAIN
  { TRY_AGAIN, "Try Again" },
#endif
#ifdef NO_RECOVERY
  { NO_RECOVERY, "No Recovery" },
#endif
#ifdef NO_DATA
  { NO_DATA, "No Data" },
#endif
#ifdef NO_ADDRESS
  { NO_ADDRESS, "No Address" },
#endif
  { 0, NULL }
};
#endif
#endif

#ifndef WITH_NOHTTP
#ifndef WITH_LEAN
static const struct soap_code_map h_http_error_codes[] =
{ { 200, "OK" },
  { 201, "Created" },
  { 202, "Accepted" },
  { 203, "Non-Authoritative Information" },
  { 204, "No Content" },
  { 205, "Reset Content" },
  { 206, "Partial Content" },
  { 300, "Multiple Choices" },
  { 301, "Moved Permanently" },
  { 302, "Found" },
  { 303, "See Other" },
  { 304, "Not Modified" },
  { 305, "Use Proxy" },
  { 307, "Temporary Redirect" },
  { 400, "Bad Request" },
  { 401, "Unauthorized" },
  { 402, "Payment Required" },
  { 403, "Forbidden" },
  { 404, "Not Found" },
  { 405, "Method Not Allowed" },
  { 406, "Not Acceptable" },
  { 407, "Proxy Authentication Required" },
  { 408, "Request Time-out" },
  { 409, "Conflict" },
  { 410, "Gone" },
  { 411, "Length Required" },
  { 412, "Precondition Failed" },
  { 413, "Request Entity Too Large" },
  { 414, "Request-URI Too Large" },
  { 415, "Unsupported Media Type" },
  { 416, "Requested range not satisfiable" },
  { 417, "Expectation Failed" },
  { 500, "Internal Server Error" },
  { 501, "Not Implemented" },
  { 502, "Bad Gateway" },
  { 503, "Service Unavailable" },
  { 504, "Gateway Time-out" },
  { 505, "HTTP Version not supported" },
  {   0, NULL }
};
#endif
#endif

#ifdef WITH_OPENSSL
static const struct soap_code_map h_ssl_error_codes[] =
{
#define _SSL_ERROR(e) { e, #e }
  _SSL_ERROR(SSL_ERROR_SSL),
  _SSL_ERROR(SSL_ERROR_ZERO_RETURN),
  _SSL_ERROR(SSL_ERROR_WANT_READ),
  _SSL_ERROR(SSL_ERROR_WANT_WRITE),
  _SSL_ERROR(SSL_ERROR_WANT_CONNECT),
  _SSL_ERROR(SSL_ERROR_WANT_X509_LOOKUP),
  _SSL_ERROR(SSL_ERROR_SYSCALL),
  { 0, NULL }
};
#endif

#ifndef WITH_LEANER
static const struct soap_code_map mime_codes[] =
{ { SOAP_MIME_7BIT,		"7bit" },
  { SOAP_MIME_8BIT,		"8bit" },
  { SOAP_MIME_BINARY,		"binary" },
  { SOAP_MIME_QUOTED_PRINTABLE, "quoted-printable" },
  { SOAP_MIME_BASE64,		"base64" },
  { SOAP_MIME_IETF_TOKEN,	"ietf-token" },
  { SOAP_MIME_X_TOKEN,		"x-token" },
  { 0,				NULL }
};
#endif

#ifdef WIN32
static int tcp_done = 0;
#endif

#if defined(HP_UX) && defined(HAVE_GETHOSTBYNAME_R)
extern int h_errno;
#endif

/******************************************************************************/
#ifndef WITH_NOIO
#ifndef PALM_1
static int
fsend(struct soap *soap, const char *s, size_t n)
{ register int nwritten, err;
  SOAP_SOCKET sk;
#if defined(__cplusplus) && !defined(WITH_LEAN) && !defined(WITH_COMPAT)
  if (soap->os)
  { soap->os->write(s, (std::streamsize)n);
    if (soap->os->good())
      return SOAP_OK;
    soap->errnum = 0;
    return SOAP_EOF;
  }
#endif
  sk = soap->sendsk;
  if (!soap_valid_socket(sk))
    sk = soap->socket;
  while (n)
  { if (soap_valid_socket(sk))
    {
      if (soap->send_timeout)
      { for (;;)
        { register int r;
#ifdef WITH_OPENSSL
          if (soap->ssl)
            r = tcp_select(soap, sk, SOAP_TCP_SELECT_ALL, soap->send_timeout);
          else
#endif
#ifdef WITH_GNUTLS
          if (soap->session)
            r = tcp_select(soap, sk, SOAP_TCP_SELECT_ALL, soap->send_timeout);
          else
#endif
            r = tcp_select(soap, sk, SOAP_TCP_SELECT_SND | SOAP_TCP_SELECT_ERR, soap->send_timeout);
          if (r > 0)
            break;
          if (!r)
            return SOAP_EOF;
          err = soap->errnum;
          if (!err)
            return soap->error;
          if (err != SOAP_EINTR && err != SOAP_EAGAIN && err != SOAP_EWOULDBLOCK)
            return SOAP_EOF;
        }
      }
#ifdef WITH_OPENSSL
      if (soap->ssl)
        nwritten = SSL_write(soap->ssl, s, (int)n);
      else if (soap->bio)
        nwritten = BIO_write(soap->bio, s, (int)n);
      else
#endif
#ifdef WITH_GNUTLS
      if (soap->session)
        nwritten = gnutls_record_send(soap->session, s, n);
      else
#endif
#ifndef WITH_LEAN
      if ((soap->omode & SOAP_IO_UDP))
      { if (soap->peerlen)
          nwritten = sendto(sk, (char*)s, (SOAP_WINSOCKINT)n, soap->socket_flags, (struct sockaddr*)&soap->peer, (SOAP_WINSOCKINT)soap->peerlen);
        else
          nwritten = send(sk, s, (SOAP_WINSOCKINT)n, soap->socket_flags);
        /* retry and back-off algorithm */
        /* TODO: this is not very clear from specs so verify and limit conditions under which we should loop (e.g. ENOBUFS) */
        if (nwritten < 0)
        { int udp_repeat;
          int udp_delay;
          if ((soap->connect_flags & SO_BROADCAST))
            udp_repeat = 2; /* SOAP-over-UDP MULTICAST_UDP_REPEAT - 1 */
          else
            udp_repeat = 1; /* SOAP-over-UDP UNICAST_UDP_REPEAT - 1 */
          udp_delay = ((unsigned int)soap_random % 201) + 50; /* UDP_MIN_DELAY .. UDP_MAX_DELAY */
          do
          { tcp_select(soap, sk, SOAP_TCP_SELECT_ERR, -1000 * udp_delay);
            if (soap->peerlen)
              nwritten = sendto(sk, (char*)s, (SOAP_WINSOCKINT)n, soap->socket_flags, (struct sockaddr*)&soap->peer, (SOAP_WINSOCKINT)soap->peerlen);
            else
              nwritten = send(sk, s, (SOAP_WINSOCKINT)n, soap->socket_flags);
            udp_delay <<= 1;
            if (udp_delay > 500) /* UDP_UPPER_DELAY */
              udp_delay = 500;
          }
          while (nwritten < 0 && --udp_repeat > 0);
        }
	if (nwritten < 0)
        { err = soap_socket_errno(sk);
	  if (err && err != SOAP_EINTR)
	  { soap->errnum = err;
	    return SOAP_EOF;
	  }
          nwritten = 0; /* and call write() again */
	}
      }
      else
#endif
#if !defined(PALM) && !defined(AS400)
        nwritten = send(sk, s, (int)n, soap->socket_flags);
#else
        nwritten = send(sk, (void*)s, n, soap->socket_flags);
#endif
      if (nwritten <= 0)
      {
        register int r = 0;
        err = soap_socket_errno(sk);
#ifdef WITH_OPENSSL
        if (soap->ssl && (r = SSL_get_error(soap->ssl, nwritten)) != SSL_ERROR_NONE && r != SSL_ERROR_WANT_READ && r != SSL_ERROR_WANT_WRITE)
        { soap->errnum = err;
          return SOAP_EOF;
        }
#endif
#ifdef WITH_GNUTLS
        if (soap->session)
        { if (nwritten == GNUTLS_E_INTERRUPTED)
            err = SOAP_EINTR;
          else if (nwritten == GNUTLS_E_AGAIN)
            err = SOAP_EAGAIN;
        }
#endif
        if (err == SOAP_EWOULDBLOCK || err == SOAP_EAGAIN)
        {
#if defined(WITH_OPENSSL)
          if (soap->ssl && r == SSL_ERROR_WANT_READ)
            r = tcp_select(soap, sk, SOAP_TCP_SELECT_RCV | SOAP_TCP_SELECT_ERR, soap->send_timeout ? soap->send_timeout : -10000);
          else
#elif defined(WITH_GNUTLS)
          if (soap->session && !gnutls_record_get_direction(soap->session))
            r = tcp_select(soap, sk, SOAP_TCP_SELECT_RCV | SOAP_TCP_SELECT_ERR, soap->send_timeout ? soap->send_timeout : -10000);
          else
#endif
            r = tcp_select(soap, sk, SOAP_TCP_SELECT_SND | SOAP_TCP_SELECT_ERR, soap->send_timeout ? soap->send_timeout : -10000);
          if (!r && soap->send_timeout)
            return SOAP_EOF;
          if (r < 0 && soap->errnum != SOAP_EINTR)
            return SOAP_EOF;
        }
        else if (err && err != SOAP_EINTR)
        { soap->errnum = err;
          return SOAP_EOF;
        }
        nwritten = 0; /* and call write() again */
      }
    }
    else
    {
#ifdef WITH_FASTCGI
      nwritten = fwrite((void*)s, 1, n, stdout);
      fflush(stdout);
#else
#ifdef UNDER_CE
      nwritten = fwrite(s, 1, n, soap->sendfd);
#else
#ifdef VXWORKS
#ifdef WMW_RPM_IO
      if (soap->rpmreqid)
        nwritten = (httpBlockPut(soap->rpmreqid, (char*)s, n) == 0) ? n : -1;
      else
#endif
        nwritten = fwrite(s, sizeof(char), n, fdopen(soap->sendfd, "w"));
#else
#ifdef WIN32
      nwritten = _write(soap->sendfd, s, (unsigned int)n);
#else
      nwritten = write(soap->sendfd, s, (unsigned int)n);
#endif
#endif
#endif
#endif
      if (nwritten <= 0)
      {
#ifndef WITH_FASTCGI
        err = soap_errno;
#else
        err = EOF;
#endif
        if (err && err != SOAP_EINTR && err != SOAP_EWOULDBLOCK && err != SOAP_EAGAIN)
        { soap->errnum = err;
          return SOAP_EOF;
        }
        nwritten = 0; /* and call write() again */
      }
    }
    n -= nwritten;
    s += nwritten;
  }
  return SOAP_OK;
}
#endif
#endif

/******************************************************************************/
#ifndef PALM_1
SOAP_FMAC1
int
SOAP_FMAC2
soap_send_raw(struct soap *soap, const char *s, size_t n)
{ if (!n)
    return SOAP_OK;
#ifndef WITH_LEANER
  if (soap->fpreparesend && (soap->mode & SOAP_IO) != SOAP_IO_STORE && (soap->mode & SOAP_IO_LENGTH) && (soap->error = soap->fpreparesend(soap, s, n)))
    return soap->error;
  if (soap->ffiltersend && (soap->error = soap->ffiltersend(soap, &s, &n)))
    return soap->error;
#endif
  if (soap->mode & SOAP_IO_LENGTH)
    soap->count += n;
  else if (soap->mode & SOAP_IO)
  { register size_t i = SOAP_BUFLEN - soap->bufidx;
    while (n >= i)
    { memcpy(soap->buf + soap->bufidx, s, i);
      soap->bufidx = SOAP_BUFLEN;
      if (soap_flush(soap))
        return soap->error;
      s += i;
      n -= i;
      i = SOAP_BUFLEN;
    }
    memcpy(soap->buf + soap->bufidx, s, n);
    soap->bufidx += n;
  }
  else
    return soap_flush_raw(soap, s, n);
  return SOAP_OK;
}
#endif

/******************************************************************************/
#ifndef PALM_1
SOAP_FMAC1
int
SOAP_FMAC2
soap_flush(struct soap *soap)
{ register size_t n = soap->bufidx;
  if (n)
  {
#ifndef WITH_LEANER
    if ((soap->mode & SOAP_IO) == SOAP_IO_STORE)
    { register int r;
      if (soap->fpreparesend && (r = soap->fpreparesend(soap, soap->buf, n)))
        return soap->error = r;
    }
#endif
    soap->bufidx = 0;
#ifdef WITH_ZLIB
    if (soap->mode & SOAP_ENC_ZLIB)
    { soap->d_stream->next_in = (Byte*)soap->buf;
      soap->d_stream->avail_in = (unsigned int)n;
#ifdef WITH_GZIP
      soap->z_crc = crc32(soap->z_crc, (Byte*)soap->buf, (unsigned int)n);
#endif
      do
      { DBGLOG(TEST, SOAP_MESSAGE(fdebug, "Deflating %u bytes\n", soap->d_stream->avail_in));
        if (deflate(soap->d_stream, Z_NO_FLUSH) != Z_OK)
        { DBGLOG(TEST, SOAP_MESSAGE(fdebug, "Unable to deflate: %s\n", soap->d_stream->msg ? soap->d_stream->msg : SOAP_STR_EOS));
          return soap->error = SOAP_ZLIB_ERROR;
        }
        if (!soap->d_stream->avail_out)
        { if (soap_flush_raw(soap, soap->z_buf, SOAP_BUFLEN))
            return soap->error;
          soap->d_stream->next_out = (Byte*)soap->z_buf;
          soap->d_stream->avail_out = SOAP_BUFLEN;
        }
      } while (soap->d_stream->avail_in);
    }
    else
#endif
      return soap_flush_raw(soap, soap->buf, n);
  }
  return SOAP_OK;
}
#endif

/******************************************************************************/
#ifndef PALM_1
SOAP_FMAC1
int
SOAP_FMAC2
soap_flush_raw(struct soap *soap, const char *s, size_t n)
{ if ((soap->mode & SOAP_IO) == SOAP_IO_STORE)
  { register char *t;
    if (!(t = (char*)soap_push_block(soap, NULL, n)))
      return soap->error = SOAP_EOM;
    memcpy(t, s, n);
    return SOAP_OK;
  }
#ifndef WITH_LEANER
  if ((soap->mode & SOAP_IO) == SOAP_IO_CHUNK)
  { char t[16];
    sprintf(t, &"\r\n%lX\r\n"[soap->chunksize ? 0 : 2], (unsigned long)n);
    DBGMSG(SENT, t, strlen(t));
    if ((soap->error = soap->fsend(soap, t, strlen(t))))
      return soap->error;
    soap->chunksize += n;
  }
  DBGMSG(SENT, s, n);
#endif
  return soap->error = soap->fsend(soap, s, n);
}
#endif

/******************************************************************************/
#ifndef PALM_1
SOAP_FMAC1
int
SOAP_FMAC2
soap_send(struct soap *soap, const char *s)
{ if (s)
    return soap_send_raw(soap, s, strlen(s));
  return SOAP_OK;
}
#endif

/******************************************************************************/
#ifndef WITH_LEANER
#ifndef PALM_1
SOAP_FMAC1
int
SOAP_FMAC2
soap_send2(struct soap *soap, const char *s1, const char *s2)
{ if (soap_send(soap, s1))
    return soap->error;
  return soap_send(soap, s2);
}
#endif
#endif

/******************************************************************************/
#ifndef WITH_LEANER
#ifndef PALM_1
SOAP_FMAC1
int
SOAP_FMAC2
soap_send3(struct soap *soap, const char *s1, const char *s2, const char *s3)
{ if (soap_send(soap, s1)
   || soap_send(soap, s2))
    return soap->error;
  return soap_send(soap, s3);
}
#endif
#endif

/******************************************************************************/
#ifndef WITH_NOIO
#ifndef PALM_1
static size_t
frecv(struct soap *soap, char *s, size_t n)
{ register int r;
  register int retries = 100; /* max 100 retries with non-blocking sockets */
  SOAP_SOCKET sk;
  soap->errnum = 0;
#if defined(__cplusplus) && !defined(WITH_LEAN) && !defined(WITH_COMPAT)
  if (soap->is)
  { if (soap->is->good())
      return soap->is->read(s, (std::streamsize)n).gcount();
    return 0;
  }
#endif
  sk = soap->recvsk;
  if (!soap_valid_socket(sk))
    sk = soap->socket;
  if (soap_valid_socket(sk))
  { for (;;)
    {
#ifdef WITH_OPENSSL
      register int err = 0;
#endif
#ifdef WITH_OPENSSL
      if (soap->recv_timeout && !soap->ssl) /* SSL: sockets are nonblocking */
#else
      if (soap->recv_timeout)
#endif
      { for (;;)
        { r = tcp_select(soap, sk, SOAP_TCP_SELECT_RCV | SOAP_TCP_SELECT_ERR, soap->recv_timeout);
          if (r > 0)
            break;
          if (!r)
            return 0;
          r = soap->errnum;
          if (r != SOAP_EINTR && r != SOAP_EAGAIN && r != SOAP_EWOULDBLOCK)
            return 0;
        }
      }
#ifdef WITH_OPENSSL
      if (soap->ssl)
      { r = SSL_read(soap->ssl, s, (int)n);
        if (r > 0)
          return (size_t)r;
        err = SSL_get_error(soap->ssl, r);
        if (err != SSL_ERROR_NONE && err != SSL_ERROR_WANT_READ && err != SSL_ERROR_WANT_WRITE)
          return 0;
      }
      else if (soap->bio)
      { r = BIO_read(soap->bio, s, (int)n);
        if (r > 0)
          return (size_t)r;
        return 0;
      }
      else
#endif
#ifdef WITH_GNUTLS
      if (soap->session)
      { r = (int)gnutls_record_recv(soap->session, s, n);
        if (r >= 0)
          return (size_t)r;
      }
      else
#endif
      {
#ifndef WITH_LEAN
        if ((soap->omode & SOAP_IO_UDP))
        { SOAP_SOCKLEN_T k = (SOAP_SOCKLEN_T)sizeof(soap->peer);
          memset((void*)&soap->peer, 0, sizeof(soap->peer));
          r = recvfrom(sk, s, (SOAP_WINSOCKINT)n, soap->socket_flags, (struct sockaddr*)&soap->peer, &k);	/* portability note: see SOAP_SOCKLEN_T definition in stdsoap2.h */
          soap->peerlen = (size_t)k;
#ifndef WITH_IPV6
          soap->ip = ntohl(soap->peer.sin_addr.s_addr);
#endif
        }
        else
#endif
          r = recv(sk, s, (int)n, soap->socket_flags);
#ifdef PALM
        /* CycleSyncDisplay(curStatusMsg); */
#endif
        if (r >= 0)
          return (size_t)r;
        r = soap_socket_errno(sk);
        if (r != SOAP_EINTR && r != SOAP_EAGAIN && r != SOAP_EWOULDBLOCK)
        { soap->errnum = r;
          return 0;
        }
      }
#if defined(WITH_OPENSSL)
      if (soap->ssl && err == SSL_ERROR_WANT_WRITE)
         r = tcp_select(soap, sk, SOAP_TCP_SELECT_SND | SOAP_TCP_SELECT_ERR, soap->recv_timeout ? soap->recv_timeout : 5);
      else
#elif defined(WITH_GNUTLS)
      if (soap->session && gnutls_record_get_direction(soap->session))
         r = tcp_select(soap, sk, SOAP_TCP_SELECT_SND | SOAP_TCP_SELECT_ERR, soap->recv_timeout ? soap->recv_timeout : 5);
      else
#endif
        r = tcp_select(soap, sk, SOAP_TCP_SELECT_RCV | SOAP_TCP_SELECT_ERR, soap->recv_timeout ? soap->recv_timeout : 5);
      if (!r && soap->recv_timeout)
        return 0;
      if (r < 0)
      { r = soap->errnum;
        if (r != SOAP_EINTR && r != SOAP_EAGAIN && r != SOAP_EWOULDBLOCK)
          return 0;
      }
      if (retries-- <= 0)
        return 0;
#ifdef PALM
      r = soap_socket_errno(sk);
      if (r != SOAP_EINTR && retries-- <= 0)
      { soap->errnum = r;
        return 0;
      }
#endif
    }
  }
#ifdef WITH_FASTCGI
  return fread(s, 1, n, stdin);
#else
#ifdef UNDER_CE
  return fread(s, 1, n, soap->recvfd);
#else
#ifdef WMW_RPM_IO
  if (soap->rpmreqid)
    r = httpBlockRead(soap->rpmreqid, s, n);
  else
#endif
#ifdef WIN32
    r = _read(soap->recvfd, s, (unsigned int)n);
#else
    r = read(soap->recvfd, s, (unsigned int)n);
#endif
  if (r >= 0)
    return (size_t)r;
  soap->errnum = soap_errno;
  return 0;
#endif
#endif
}
#endif
#endif

/******************************************************************************/
#ifndef WITH_NOHTTP
#ifndef PALM_1
static soap_wchar
soap_getchunkchar(struct soap *soap)
{ if (soap->bufidx < soap->buflen)
    return soap->buf[soap->bufidx++];
  soap->bufidx = 0;
  soap->buflen = soap->chunkbuflen = soap->frecv(soap, soap->buf, SOAP_BUFLEN);
  DBGLOG(TEST, SOAP_MESSAGE(fdebug, "Read %u bytes from socket=%d/fd=%d\n", (unsigned int)soap->buflen, soap->socket, soap->recvfd));
  DBGMSG(RECV, soap->buf, soap->buflen);
  if (soap->buflen)
    return soap->buf[soap->bufidx++];
  return EOF;
}
#endif
#endif

/******************************************************************************/
#ifndef PALM_1
static int
soap_isxdigit(int c)
{ return (c >= '0' && c <= '9') || (c >= 'A' && c <= 'F') || (c >= 'a' && c <= 'f');
}
#endif

/******************************************************************************/
#ifndef PALM_1
SOAP_FMAC1
int
SOAP_FMAC2
soap_recv_raw(struct soap *soap)
{ register size_t ret;
#if !defined(WITH_LEANER) || defined(WITH_ZLIB)
  register int r;
#endif
#ifdef WITH_ZLIB
  if (soap->mode & SOAP_ENC_ZLIB)
  { if (soap->d_stream->next_out == Z_NULL)
    { soap->bufidx = soap->buflen = 0;
      return EOF;
    }
    if (soap->d_stream->avail_in || !soap->d_stream->avail_out)
    { DBGLOG(TEST, SOAP_MESSAGE(fdebug, "Inflating\n"));
      soap->d_stream->next_out = (Byte*)soap->buf;
      soap->d_stream->avail_out = SOAP_BUFLEN;
      r = inflate(soap->d_stream, Z_NO_FLUSH);
      if (r == Z_NEED_DICT && soap->z_dict)
        r = inflateSetDictionary(soap->d_stream, (const Bytef*)soap->z_dict, soap->z_dict_len);
      if (r == Z_OK || r == Z_STREAM_END)
      { soap->bufidx = 0;
        ret = soap->buflen = SOAP_BUFLEN - soap->d_stream->avail_out;
        if (soap->zlib_in == SOAP_ZLIB_GZIP)
          soap->z_crc = crc32(soap->z_crc, (Byte*)soap->buf, (unsigned int)ret);
        if (r == Z_STREAM_END)
        { DBGLOG(TEST, SOAP_MESSAGE(fdebug, "Inflated %lu->%lu bytes\n", soap->d_stream->total_in, soap->d_stream->total_out));
          soap->z_ratio_in = (float)soap->d_stream->total_in / (float)soap->d_stream->total_out;
          soap->d_stream->next_out = Z_NULL;
        }
        if (ret)
        { soap->count += ret;
          DBGLOG(RECV, SOAP_MESSAGE(fdebug, "\n---- decompressed ----\n"));
          DBGMSG(RECV, soap->buf, ret);
          DBGLOG(RECV, SOAP_MESSAGE(fdebug, "\n----\n"));
#ifndef WITH_LEANER
          if (soap->fpreparerecv && (r = soap->fpreparerecv(soap, soap->buf, ret)))
            return soap->error = r;
#endif
          return SOAP_OK;
        }
      }
      else if (r != Z_BUF_ERROR)
      { DBGLOG(TEST, SOAP_MESSAGE(fdebug, "Inflate error: %s\n", soap->d_stream->msg ? soap->d_stream->msg : SOAP_STR_EOS));
        soap->d_stream->next_out = Z_NULL;
        return soap->error = SOAP_ZLIB_ERROR;
      }
    }
zlib_again:
    if ((soap->mode & SOAP_IO) == SOAP_IO_CHUNK && !soap->chunksize)
    { memcpy(soap->buf, soap->z_buf, SOAP_BUFLEN);
      soap->buflen = soap->z_buflen;
    }
    DBGLOG(RECV, SOAP_MESSAGE(fdebug, "\n---- compressed ----\n"));
  }
#endif
#ifndef WITH_NOHTTP
  if ((soap->mode & SOAP_IO) == SOAP_IO_CHUNK) /* read HTTP chunked transfer */
  { for (;;)
    { register soap_wchar c;
      char *t, tmp[17];
      if (soap->chunksize)
      { soap->buflen = ret = soap->frecv(soap, soap->buf, soap->chunksize > SOAP_BUFLEN ? SOAP_BUFLEN : soap->chunksize);
        DBGLOG(TEST, SOAP_MESSAGE(fdebug, "Getting chunk: read %u bytes\n", (unsigned int)ret));
        DBGMSG(RECV, soap->buf, ret);
        soap->bufidx = 0;
        soap->chunksize -= ret;
        break;
      }
      t = tmp;
      if (!soap->chunkbuflen)
      { soap->chunkbuflen = ret = soap->frecv(soap, soap->buf, SOAP_BUFLEN);
        DBGLOG(TEST, SOAP_MESSAGE(fdebug, "Read %u bytes (chunked) from socket %d\n", (unsigned int)ret, soap->socket));
        DBGMSG(RECV, soap->buf, ret);
        soap->bufidx = 0;
        if (!ret)
        { soap->ahead = EOF;
	  return EOF;
        }
      }
      else
        soap->bufidx = soap->buflen;
      soap->buflen = soap->chunkbuflen;
      DBGLOG(TEST, SOAP_MESSAGE(fdebug, "Getting chunk size (idx=%u len=%u)\n", (unsigned int)soap->bufidx, (unsigned int)soap->buflen));
      while (!soap_isxdigit((int)(c = soap_getchunkchar(soap))))
      { if ((int)c == EOF)
        { soap->ahead = EOF;
	  return EOF;
        }
      }
      do
        *t++ = (char)c;
      while (soap_isxdigit((int)(c = soap_getchunkchar(soap))) && (size_t)(t - tmp) < sizeof(tmp)-1);
      while ((int)c != EOF && c != '\n')
        c = soap_getchunkchar(soap);
      if ((int)c == EOF)
      { soap->ahead = EOF;
        return EOF;
      }
      *t = '\0';
      DBGLOG(TEST, SOAP_MESSAGE(fdebug, "Chunk size = %s (hex)\n", tmp));
      soap->chunksize = (size_t)soap_strtoul(tmp, &t, 16);
      if (!soap->chunksize)
      { soap->bufidx = soap->buflen = soap->chunkbuflen = 0;
        DBGLOG(TEST, SOAP_MESSAGE(fdebug, "End of chunked message\n"));
        while ((int)c != EOF && c != '\n')
          c = soap_getchunkchar(soap);
        ret = 0;
        soap->ahead = EOF;
        break;
      }
      soap->buflen = soap->bufidx + soap->chunksize;
      DBGLOG(TEST, SOAP_MESSAGE(fdebug, "Moving buf len to idx=%u len=%u (%s)\n", (unsigned int)soap->bufidx, (unsigned int)soap->buflen, tmp));
      if (soap->buflen > soap->chunkbuflen)
      { soap->buflen = soap->chunkbuflen;
        soap->chunksize -= soap->buflen - soap->bufidx;
        soap->chunkbuflen = 0;
        DBGLOG(TEST, SOAP_MESSAGE(fdebug, "Passed end of buffer for chunked HTTP (%u bytes left)\n", (unsigned int)(soap->buflen - soap->bufidx)));
      }
      else if (soap->chunkbuflen)
        soap->chunksize = 0;
      ret = soap->buflen - soap->bufidx;
      if (ret)
        break;
    }
  }
  else
#endif
  { soap->bufidx = 0;
    soap->buflen = ret = soap->frecv(soap, soap->buf, SOAP_BUFLEN);
    DBGLOG(TEST, SOAP_MESSAGE(fdebug, "Read %u bytes from socket=%d/fd=%d\n", (unsigned int)ret, soap->socket, soap->recvfd));
    DBGMSG(RECV, soap->buf, ret);
  }
#ifdef WITH_ZLIB
  if (soap->mode & SOAP_ENC_ZLIB)
  { memcpy(soap->z_buf, soap->buf, SOAP_BUFLEN);
    soap->d_stream->next_in = (Byte*)(soap->z_buf + soap->bufidx);
    soap->d_stream->avail_in = (unsigned int)ret;
    soap->d_stream->next_out = (Byte*)soap->buf;
    soap->d_stream->avail_out = SOAP_BUFLEN;
    r = inflate(soap->d_stream, Z_NO_FLUSH);
    if (r == Z_NEED_DICT && soap->z_dict)
      r = inflateSetDictionary(soap->d_stream, (const Bytef*)soap->z_dict, soap->z_dict_len);
    if (r == Z_OK || r == Z_STREAM_END)
    { soap->bufidx = 0;
      soap->z_buflen = soap->buflen;
      soap->buflen = SOAP_BUFLEN - soap->d_stream->avail_out;
      if (soap->zlib_in == SOAP_ZLIB_GZIP)
        soap->z_crc = crc32(soap->z_crc, (Byte*)soap->buf, (unsigned int)soap->buflen);
      DBGLOG(TEST, SOAP_MESSAGE(fdebug, "Inflated %u bytes\n", (unsigned int)soap->buflen));
      if (ret && !soap->buflen && r != Z_STREAM_END)
        goto zlib_again;
      ret = soap->buflen;
      if (r == Z_STREAM_END)
      { DBGLOG(TEST, SOAP_MESSAGE(fdebug, "Inflated total %lu->%lu bytes\n", soap->d_stream->total_in, soap->d_stream->total_out));
        soap->z_ratio_in = (float)soap->d_stream->total_in / (float)soap->d_stream->total_out;
        soap->d_stream->next_out = Z_NULL;
      }
      DBGLOG(RECV, SOAP_MESSAGE(fdebug, "\n---- decompressed ----\n"));
      DBGMSG(RECV, soap->buf, ret);
#ifndef WITH_LEANER
      if (soap->fpreparerecv && (r = soap->fpreparerecv(soap, soap->buf, ret)))
        return soap->error = r;
#endif
    }
    else
    { DBGLOG(TEST, SOAP_MESSAGE(fdebug, "Unable to inflate: (%d) %s\n", r, soap->d_stream->msg ? soap->d_stream->msg : SOAP_STR_EOS));
      soap->d_stream->next_out = Z_NULL;
      return soap->error = SOAP_ZLIB_ERROR;
    }
  }
#endif
#ifndef WITH_LEANER
  if (soap->fpreparerecv
#ifdef WITH_ZLIB
   && soap->zlib_in == SOAP_ZLIB_NONE
#endif
   && (r = soap->fpreparerecv(soap, soap->buf + soap->bufidx, ret)))
    return soap->error = r;
#endif
  soap->count += ret;
  return !ret;
}
#endif

/******************************************************************************/
#ifndef PALM_1
SOAP_FMAC1
int
SOAP_FMAC2
soap_recv(struct soap *soap)
{
#ifndef WITH_LEANER
  if (soap->mode & SOAP_ENC_DIME)
  { if (soap->dime.buflen)
    { char *s;
      int i;
      unsigned char tmp[12];
      DBGLOG(TEST, SOAP_MESSAGE(fdebug, "DIME hdr for chunked DIME is in buffer\n"));
      soap->count += soap->dime.buflen - soap->buflen;
      soap->buflen = soap->dime.buflen;
      DBGLOG(TEST, SOAP_MESSAGE(fdebug, "Skip padding (%ld bytes)\n", -(long)soap->dime.size&3));
      for (i = -(long)soap->dime.size&3; i > 0; i--)
      { soap->bufidx++;
        if (soap->bufidx >= soap->buflen)
          if (soap_recv_raw(soap))
            return EOF;
      }
      DBGLOG(TEST, SOAP_MESSAGE(fdebug, "Get DIME hdr for next chunk\n"));
      s = (char*)tmp;
      for (i = 12; i > 0; i--)
      { *s++ = soap->buf[soap->bufidx++];
        if (soap->bufidx >= soap->buflen)
          if (soap_recv_raw(soap))
            return EOF;
      }
      soap->dime.flags = tmp[0] & 0x7;
      soap->dime.size = ((size_t)tmp[8] << 24) | ((size_t)tmp[9] << 16) | ((size_t)tmp[10] << 8) | ((size_t)tmp[11]);
      DBGLOG(TEST, SOAP_MESSAGE(fdebug, "Get DIME chunk (%u bytes)\n", (unsigned int)soap->dime.size));
      if (soap->dime.flags & SOAP_DIME_CF)
      { DBGLOG(TEST, SOAP_MESSAGE(fdebug, "More chunking\n"));
        soap->dime.chunksize = soap->dime.size;
        if (soap->buflen - soap->bufidx >= soap->dime.size)
        { soap->dime.buflen = soap->buflen;
          soap->buflen = soap->bufidx + soap->dime.chunksize;
        }
        else
          soap->dime.chunksize -= soap->buflen - soap->bufidx;
      }
      else
      { DBGLOG(TEST, SOAP_MESSAGE(fdebug, "Last chunk\n"));
        soap->dime.buflen = 0;
        soap->dime.chunksize = 0;
      }
      soap->count = soap->buflen - soap->bufidx;
      DBGLOG(TEST, SOAP_MESSAGE(fdebug, "%u bytes remaining\n", (unsigned int)soap->count));
      return SOAP_OK;
    }
    if (soap->dime.chunksize)
    { DBGLOG(TEST, SOAP_MESSAGE(fdebug, "Get next DIME hdr for chunked DIME (%u bytes chunk)\n", (unsigned int)soap->dime.chunksize));
      if (soap_recv_raw(soap))
        return EOF;
      if (soap->buflen - soap->bufidx >= soap->dime.chunksize)
      { soap->dime.buflen = soap->buflen;
        soap->count -= soap->buflen - soap->bufidx - soap->dime.chunksize;
        soap->buflen = soap->bufidx + soap->dime.chunksize;
      }
      else
        soap->dime.chunksize -= soap->buflen - soap->bufidx;
      DBGLOG(TEST, SOAP_MESSAGE(fdebug, "%lu bytes remaining, count=%lu\n", (unsigned long)(soap->buflen-soap->bufidx), (unsigned long)soap->count));
      return SOAP_OK;
    }
  }
  while (soap->ffilterrecv)
  { int err, last = soap->filterstop;
    if (last)
      soap->bufidx = soap->buflen = 0;
    if ((err = soap->ffilterrecv(soap, soap->buf, &soap->buflen, sizeof(soap->buf))))
      return soap->error = err;
    if (soap->buflen)
    { soap->bufidx = 0;
      soap->filterstop = last;
      return SOAP_OK;
    }
    if (last)
    { DBGLOG(TEST, SOAP_MESSAGE(fdebug, "Returning postponed error %d\n", last));
      soap->filterstop = SOAP_OK;
      return last;
    }
    soap->filterstop = soap_recv_raw(soap); /* do not call again after EOF */
  }
#endif
  return soap_recv_raw(soap);
}
#endif

/******************************************************************************/
#ifndef PALM_1
SOAP_FMAC1
soap_wchar
SOAP_FMAC2
soap_getchar(struct soap *soap)
{ register soap_wchar c;
  c = soap->ahead;
  if (c)
  { if (c != EOF)
      soap->ahead = 0;
    return c;
  }
  return soap_get1(soap);
}
#endif

/******************************************************************************/
#ifndef PALM_1
SOAP_FMAC1
const struct soap_code_map*
SOAP_FMAC2
soap_code(const struct soap_code_map *code_map, const char *str)
{ if (code_map && str)
  { while (code_map->string)
    { if (!strcmp(str, code_map->string)) /* case sensitive */
        return code_map;
      code_map++;
    }
  }
  return NULL;
}
#endif

/******************************************************************************/
#ifndef PALM_1
SOAP_FMAC1
long
SOAP_FMAC2
soap_code_int(const struct soap_code_map *code_map, const char *str, long other)
{ if (code_map)
  { while (code_map->string)
    { if (!soap_tag_cmp(str, code_map->string)) /* case insensitive */
        return code_map->code;
      code_map++;
    }
  }
  return other;
}
#endif

/******************************************************************************/
#ifndef PALM_1
SOAP_FMAC1
const char*
SOAP_FMAC2
soap_code_str(const struct soap_code_map *code_map, long code)
{ if (!code_map)
    return NULL;
  while (code_map->code != code && code_map->string)
    code_map++;
  return code_map->string;
}
#endif

/******************************************************************************/
#ifndef PALM_1
SOAP_FMAC1
long
SOAP_FMAC2
soap_code_bits(const struct soap_code_map *code_map, const char *str)
{ register long bits = 0;
  if (code_map)
  { while (str && *str)
    { const struct soap_code_map *p;
      for (p = code_map; p->string; p++)
      { register size_t n = strlen(p->string);
        if (!strncmp(p->string, str, n) && soap_blank((soap_wchar)str[n]))
        { bits |= p->code;
          str += n;
          while (*str > 0 && *str <= 32)
            str++;
          break;
        }
      }
      if (!p->string)
        return 0;
    }
  }
  return bits;
}
#endif

/******************************************************************************/
#ifndef PALM_1
SOAP_FMAC1
const char*
SOAP_FMAC2
soap_code_list(struct soap *soap, const struct soap_code_map *code_map, long code)
{ register char *t = soap->tmpbuf;
  if (code_map)
  { while (code_map->string)
    { if (code_map->code & code)
      { register const char *s = code_map->string;
        if (t != soap->tmpbuf)
          *t++ = ' ';
        while (*s && t < soap->tmpbuf + sizeof(soap->tmpbuf) - 1)
          *t++ = *s++;
        if (t == soap->tmpbuf + sizeof(soap->tmpbuf) - 1)
          break;
      }
      code_map++;
    }
  }
  *t = '\0';
  return soap->tmpbuf;
}
#endif

/******************************************************************************/
#ifndef PALM_1
static soap_wchar
soap_char(struct soap *soap)
{ char tmp[8];
  register int i;
  register soap_wchar c;
  register char *s = tmp;
  for (i = 0; i < 7; i++)
  { c = soap_get1(soap);
    if (c == ';' || (int)c == EOF)
      break;
    *s++ = (char)c;
  }
  *s = '\0';
  if (*tmp == '#')
  { if (tmp[1] == 'x' || tmp[1] == 'X')
      return (soap_wchar)soap_strtol(tmp + 2, NULL, 16);
    return (soap_wchar)soap_strtol(tmp + 1, NULL, 10);
  }
  if (!strcmp(tmp, "lt"))
    return '<';
  if (!strcmp(tmp, "gt"))
    return '>';
  if (!strcmp(tmp, "amp"))
    return '&';
  if (!strcmp(tmp, "quot"))
    return '"';
  if (!strcmp(tmp, "apos"))
    return '\'';
#ifndef WITH_LEAN
  return (soap_wchar)soap_code_int(html_entity_codes, tmp, SOAP_UNKNOWN_CHAR);
#else
  return SOAP_UNKNOWN_CHAR; /* use this to represent unknown code */
#endif
}
#endif

/******************************************************************************/
#ifdef WITH_LEAN
#ifndef PALM_1
soap_wchar
soap_get0(struct soap *soap)
{ if (soap->bufidx >= soap->buflen && soap_recv(soap))
    return EOF;
  return (unsigned char)soap->buf[soap->bufidx];
}
#endif
#endif

/******************************************************************************/
#ifdef WITH_LEAN
#ifndef PALM_1
soap_wchar
soap_get1(struct soap *soap)
{ if (soap->bufidx >= soap->buflen && soap_recv(soap))
    return EOF;
  return (unsigned char)soap->buf[soap->bufidx++];
}
#endif
#endif

/******************************************************************************/
#ifndef PALM_1
SOAP_FMAC1
soap_wchar
SOAP_FMAC2
soap_get(struct soap *soap)
{ register soap_wchar c;
  c = soap->ahead;
  if (c)
  { if ((int)c != EOF)
      soap->ahead = 0;
  }
  else
    c = soap_get1(soap);
  while ((int)c != EOF)
  { if (soap->cdata)
    { if (c == ']')
      { c = soap_get1(soap);
        if (c == ']')
        { c = soap_get0(soap);
          if (c == '>')
          { soap->cdata = 0;
            c = soap_get1(soap);
            c = soap_get1(soap);
          }
          else
          { soap_unget(soap, ']');
            return ']';
          }
        }
        else
        { soap_revget1(soap);
          return ']';
        }
      }
      else
        return c;
    }
    switch (c)
    { case '<':
        do c = soap_get1(soap);
        while (soap_blank(c));
        if (c == '!' || c == '?' || c == '%')
        { register int k = 1;
          if (c == '!')
          { c = soap_get1(soap);
            if (c == '[')
            { do c = soap_get1(soap);
              while ((int)c != EOF && c != '[');
              if ((int)c == EOF)
                break;
              soap->cdata = 1;
              c = soap_get1(soap);
              continue;
            }
            if (c == '-' && (c = soap_get1(soap)) == '-')
            { do
              { c = soap_get1(soap);
                if (c == '-' && (c = soap_get1(soap)) == '-')
                  break;
              } while ((int)c != EOF);
            }
          }
          else if (c == '?')
            c = soap_get_pi(soap);
          while ((int)c != EOF)
          { if (c == '<')
              k++;
            else if (c == '>')
            { if (--k <= 0)
                break;
            }
            c = soap_get1(soap);
          }
          if ((int)c == EOF)
            break;
          c = soap_get1(soap);
          continue;
        }
        if (c == '/')
          return SOAP_TT;
        soap_revget1(soap);
        return SOAP_LT;
      case '>':
        return SOAP_GT;
      case '"':
        return SOAP_QT;
      case '\'':
        return SOAP_AP;
      case '&':
        return soap_char(soap) | 0x80000000;
    }
    break;
  }
  return c;
}
#endif

/******************************************************************************/
#ifndef PALM_1
static soap_wchar
soap_get_pi(struct soap *soap)
{ char buf[64];
  register char *s = buf;
  register int i = sizeof(buf);
  register soap_wchar c = soap_getchar(soap);
  /* This is a quick way to parse XML PI and we could use a callback instead to
   * enable applications to intercept processing instructions */
  while ((int)c != EOF && c != '?')
  { if (--i > 0)
    { if (soap_blank(c))
        c = ' ';
      *s++ = (char)c;
    }
    c = soap_getchar(soap);
  }
  *s = '\0';
  DBGLOG(TEST, SOAP_MESSAGE(fdebug, "XML PI <?%s?>\n", buf));
  if (!strncmp(buf, "xml ", 4))
  { s = strstr(buf, " encoding=");
    if (s && s[10])
    { if (!soap_tag_cmp(s + 11, "iso-8859-1*")
       || !soap_tag_cmp(s + 11, "latin1*"))
      { DBGLOG(TEST, SOAP_MESSAGE(fdebug, "Switching to latin1 encoding\n"));
        soap->mode |= SOAP_ENC_LATIN;
      }
      else if (!soap_tag_cmp(s + 11, "utf-8*"))
      { DBGLOG(TEST, SOAP_MESSAGE(fdebug, "Switching to utf-8 encoding\n"));
        soap->mode &= ~SOAP_ENC_LATIN;
      }
    }
  }
  if ((int)c != EOF)
    c = soap_getchar(soap);
  return c;
}
#endif

/******************************************************************************/
#ifndef WITH_LEANER
#ifndef PALM_1
SOAP_FMAC1
int
SOAP_FMAC2
soap_move(struct soap *soap, long n)
{ DBGLOG(TEST, SOAP_MESSAGE(fdebug, "Moving %ld bytes forward\n", (long)n));
  for (; n > 0; n--)
    if ((int)soap_getchar(soap) == EOF)
      return SOAP_EOF;
  return SOAP_OK;
}
#endif
#endif

/******************************************************************************/
#ifndef WITH_LEANER
#ifndef PALM_1
SOAP_FMAC1
size_t
SOAP_FMAC2
soap_tell(struct soap *soap)
{ return soap->count - soap->buflen + soap->bufidx - (soap->ahead != 0);
}
#endif
#endif

/******************************************************************************/
#ifndef PALM_1
SOAP_FMAC1
int
SOAP_FMAC2
soap_pututf8(struct soap *soap, register unsigned long c)
{ char tmp[16];
  if (c < 0x80 && c > 0)
  { *tmp = (char)c;
    return soap_send_raw(soap, tmp, 1);
  }
#ifndef WITH_LEAN
  if (c >= 0x80)
  { register char *t = tmp;
    if (c < 0x0800)
      *t++ = (char)(0xC0 | ((c >> 6) & 0x1F));
    else
    { if (c < 0x010000)
        *t++ = (char)(0xE0 | ((c >> 12) & 0x0F));
      else
      { if (c < 0x200000)
          *t++ = (char)(0xF0 | ((c >> 18) & 0x07));
        else
        { if (c < 0x04000000)
            *t++ = (char)(0xF8 | ((c >> 24) & 0x03));
          else
          { *t++ = (char)(0xFC | ((c >> 30) & 0x01));
            *t++ = (char)(0x80 | ((c >> 24) & 0x3F));
          }
          *t++ = (char)(0x80 | ((c >> 18) & 0x3F));
        }
        *t++ = (char)(0x80 | ((c >> 12) & 0x3F));
      }
      *t++ = (char)(0x80 | ((c >> 6) & 0x3F));
    }
    *t++ = (char)(0x80 | (c & 0x3F));
    *t = '\0';
  }
  else
#endif
    sprintf(tmp, "&#%lu;", c);
  return soap_send(soap, tmp);
}
#endif

/******************************************************************************/
#ifndef PALM_1
SOAP_FMAC1
soap_wchar
SOAP_FMAC2
soap_getutf8(struct soap *soap)
{ register soap_wchar c, c1, c2, c3, c4;
  c = soap->ahead;
  if (c)
    soap->ahead = 0;
  else
    c = soap_get(soap);
  if (c < 0x80 || c > 0xFF || (soap->mode & SOAP_ENC_LATIN))
    return c;
  c1 = soap_get1(soap);
  if (c1 < 0x80)
  { soap_revget1(soap); /* doesn't look like this is UTF8 */
    return c;
  }
  c1 &= 0x3F;
  if (c < 0xE0)
    return ((soap_wchar)(c & 0x1F) << 6) | c1;
  c2 = (soap_wchar)soap_get1(soap) & 0x3F;
  if (c < 0xF0)
    return ((soap_wchar)(c & 0x0F) << 12) | (c1 << 6) | c2;
  c3 = (soap_wchar)soap_get1(soap) & 0x3F;
  if (c < 0xF8)
    return ((soap_wchar)(c & 0x07) << 18) | (c1 << 12) | (c2 << 6) | c3;
  c4 = (soap_wchar)soap_get1(soap) & 0x3F;
  if (c < 0xFC)
    return ((soap_wchar)(c & 0x03) << 24) | (c1 << 18) | (c2 << 12) | (c3 << 6) | c4;
  return ((soap_wchar)(c & 0x01) << 30) | (c1 << 24) | (c2 << 18) | (c3 << 12) | (c4 << 6) | (soap_wchar)(soap_get1(soap) & 0x3F);
}
#endif

/******************************************************************************/
#ifndef PALM_1
SOAP_FMAC1
int
SOAP_FMAC2
soap_puthex(struct soap *soap, const unsigned char *s, int n)
{ char d[2];
  register int i;
#ifdef WITH_DOM
  if ((soap->mode & SOAP_XML_DOM) && soap->dom)
  { if (!(soap->dom->data = soap_s2hex(soap, s, NULL, n)))
      return soap->error;
    return SOAP_OK;
  }
#endif
  for (i = 0; i < n; i++)
  { register int m = *s++;
    d[0] = (char)((m >> 4) + (m > 159 ? '7' : '0'));
    m &= 0x0F;
    d[1] = (char)(m + (m > 9 ? '7' : '0'));
    if (soap_send_raw(soap, d, 2))
      return soap->error;
  }
  return SOAP_OK;
}
#endif

/******************************************************************************/
#ifndef PALM_1
SOAP_FMAC1
unsigned char*
SOAP_FMAC2
soap_gethex(struct soap *soap, int *n)
{
#ifdef WITH_DOM
  if ((soap->mode & SOAP_XML_DOM) && soap->dom)
  { soap->dom->data = soap_string_in(soap, 0, -1, -1);
    return (unsigned char*)soap_hex2s(soap, soap->dom->data, NULL, 0, n);
  }
#endif
#ifdef WITH_FAST
  soap->labidx = 0;
  for (;;)
  { register char *s;
    register size_t i, k;
    if (soap_append_lab(soap, NULL, 0))
      return NULL;
    s = soap->labbuf + soap->labidx;
    k = soap->lablen - soap->labidx;
    soap->labidx = soap->lablen;
    for (i = 0; i < k; i++)
    { register char d1, d2;
      register soap_wchar c;
      c = soap_get(soap);
      if (soap_isxdigit(c))
      { d1 = (char)c;
        c = soap_get(soap);
        if (soap_isxdigit(c))
          d2 = (char)c;
        else
        { soap->error = SOAP_TYPE;
          return NULL;
        }
      }
      else
      { unsigned char *p;
        soap_unget(soap, c);
        if (n)
          *n = (int)(soap->lablen + i - k);
        p = (unsigned char*)soap_malloc(soap, soap->lablen + i - k);
        if (p)
          memcpy(p, soap->labbuf, soap->lablen + i - k);
        return p;
      }
      *s++ = (char)(((d1 >= 'A' ? (d1 & 0x7) + 9 : d1 - '0') << 4) + (d2 >= 'A' ? (d2 & 0x7) + 9 : d2 - '0'));
    }
  }
#else
  if (soap_new_block(soap) == NULL)
    return NULL;
  for (;;)
  { register int i;
    register char *s = (char*)soap_push_block(soap, NULL, SOAP_BLKLEN);
    if (!s)
    { soap_end_block(soap, NULL);
      return NULL;
    }
    for (i = 0; i < SOAP_BLKLEN; i++)
    { register char d1, d2;
      register soap_wchar c = soap_get(soap);
      if (soap_isxdigit(c))
      { d1 = (char)c;
        c = soap_get(soap);
        if (soap_isxdigit(c))
          d2 = (char)c;
        else
        { soap_end_block(soap, NULL);
          soap->error = SOAP_TYPE;
          return NULL;
        }
      }
      else
      { unsigned char *p;
        soap_unget(soap, c);
        if (n)
          *n = (int)soap_size_block(soap, NULL, i);
        p = (unsigned char*)soap_save_block(soap, NULL, 0);
        return p;
      }
      *s++ = ((d1 >= 'A' ? (d1 & 0x7) + 9 : d1 - '0') << 4) + (d2 >= 'A' ? (d2 & 0x7) + 9 : d2 - '0');
    }
  }
#endif
}
#endif

/******************************************************************************/
#ifndef PALM_1
SOAP_FMAC1
int
SOAP_FMAC2
soap_putbase64(struct soap *soap, const unsigned char *s, int n)
{ register int i;
  register unsigned long m;
  char d[4];
  if (!s)
    return SOAP_OK;
#ifdef WITH_DOM
  if ((soap->mode & SOAP_XML_DOM) && soap->dom)
  { if (!(soap->dom->data = soap_s2base64(soap, s, NULL, n)))
      return soap->error;
    return SOAP_OK;
  }
#endif
  for (; n > 2; n -= 3, s += 3)
  { m = s[0];
    m = (m << 8) | s[1];
    m = (m << 8) | s[2];
    for (i = 4; i > 0; m >>= 6)
      d[--i] = soap_base64o[m & 0x3F];
    if (soap_send_raw(soap, d, 4))
      return soap->error;
  }
  if (n > 0)
  { m = 0;
    for (i = 0; i < n; i++)
      m = (m << 8) | *s++;
    for (; i < 3; i++)
      m <<= 8;
    for (i++; i > 0; m >>= 6)
      d[--i] = soap_base64o[m & 0x3F];
    for (i = 3; i > n; i--)
      d[i] = '=';
    if (soap_send_raw(soap, d, 4))
      return soap->error;
  }
  return SOAP_OK;
}
#endif

/******************************************************************************/
#ifndef PALM_1
SOAP_FMAC1
unsigned char*
SOAP_FMAC2
soap_getbase64(struct soap *soap, int *n, int malloc_flag)
{ (void)malloc_flag;
#ifdef WITH_DOM
  if ((soap->mode & SOAP_XML_DOM) && soap->dom)
  { soap->dom->data = soap_string_in(soap, 0, -1, -1);
    return (unsigned char*)soap_base642s(soap, soap->dom->data, NULL, 0, n);
  }
#endif
#ifdef WITH_FAST
  soap->labidx = 0;
  for (;;)
  { register size_t i, k;
    register char *s;
    if (soap_append_lab(soap, NULL, 2))
      return NULL;
    s = soap->labbuf + soap->labidx;
    k = soap->lablen - soap->labidx;
    soap->labidx = 3 * (soap->lablen / 3);
    if (!s)
      return NULL;
    if (k > 2)
    { for (i = 0; i < k - 2; i += 3)
      { register unsigned long m = 0;
        register int j = 0;
        do
        { register soap_wchar c = soap_get(soap);
	  if (c < SOAP_AP)
	    c &= 0x7FFFFFFF;
          if (c == '=' || c < 0)
          { unsigned char *p;
            switch (j)
            { case 2:
                *s++ = (char)((m >> 4) & 0xFF);
                i++;
                break;
              case 3:
                *s++ = (char)((m >> 10) & 0xFF);
                *s++ = (char)((m >> 2) & 0xFF);
                i += 2;
            }
            if (n)
              *n = (int)(soap->lablen + i - k);
            p = (unsigned char*)soap_malloc(soap, soap->lablen + i - k);
            if (p)
              memcpy(p, soap->labbuf, soap->lablen + i - k);
            if (c >= 0)
            { while ((int)((c = soap_get(soap)) != EOF) && c != SOAP_LT && c != SOAP_TT)
                ;
            }
            soap_unget(soap, c);
            return p;
          }
          c -= '+';
          if (c >= 0 && c <= 79)
          { register int b = soap_base64i[c];
            if (b >= 64)
            { soap->error = SOAP_TYPE;
              return NULL;
            }
            m = (m << 6) + b;
            j++;
          }
          else if (!soap_blank(c + '+'))
          { soap->error = SOAP_TYPE;
            return NULL;
          }
        } while (j < 4);
        *s++ = (char)((m >> 16) & 0xFF);
        *s++ = (char)((m >> 8) & 0xFF);
        *s++ = (char)(m & 0xFF);
      }
    }
  }
#else
  if (soap_new_block(soap) == NULL)
    return NULL;
  for (;;)
  { register int i;
    register char *s = (char*)soap_push_block(soap, NULL, 3 * SOAP_BLKLEN); /* must be multiple of 3 */
    if (!s)
    { soap_end_block(soap, NULL);
      return NULL;
    }
    for (i = 0; i < SOAP_BLKLEN; i++)
    { register unsigned long m = 0;
      register int j = 0;
      do
      { register soap_wchar c = soap_get(soap);
        if (c == '=' || c < 0)
        { unsigned char *p;
          i *= 3;
          switch (j)
          { case 2:
              *s++ = (char)((m >> 4) & 0xFF);
              i++;
              break;
            case 3:
              *s++ = (char)((m >> 10) & 0xFF);
              *s++ = (char)((m >> 2) & 0xFF);
              i += 2;
          }
          if (n)
            *n = (int)soap_size_block(soap, NULL, i);
          p = (unsigned char*)soap_save_block(soap, NULL, 0);
          if (c >= 0)
          { while ((int)((c = soap_get(soap)) != EOF) && c != SOAP_LT && c != SOAP_TT)
              ;
          }
          soap_unget(soap, c);
          return p;
        }
        c -= '+';
        if (c >= 0 && c <= 79)
        { int b = soap_base64i[c];
          if (b >= 64)
          { soap->error = SOAP_TYPE;
            return NULL;
          }
          m = (m << 6) + b;
          j++;
        }
        else if (!soap_blank(c))
        { soap->error = SOAP_TYPE;
          return NULL;
        }
      } while (j < 4);
      *s++ = (char)((m >> 16) & 0xFF);
      *s++ = (char)((m >> 8) & 0xFF);
      *s++ = (char)(m & 0xFF);
    }
  }
#endif
}
#endif

/******************************************************************************/
#ifndef WITH_LEANER
#ifndef PALM_1
SOAP_FMAC1
int
SOAP_FMAC2
soap_xop_forward(struct soap *soap, unsigned char **ptr, int *size, char **id, char **type, char **options)
{ /* Check MTOM xop:Include element (within hex/base64Binary) */
  /* TODO: this code to be obsoleted with new import/xop.h conventions */
  short body = soap->body; /* should save type too? */
  if (!soap_peek_element(soap))
  { if (!soap_element_begin_in(soap, "xop:Include", 0, NULL))
    { if (soap_dime_forward(soap, ptr, size, id, type, options)
       || (soap->body && soap_element_end_in(soap, "xop:Include")))
        return soap->error;
    }
  }
  soap->body = body;
  return SOAP_OK;
}
#endif
#endif

/******************************************************************************/
#ifndef WITH_LEANER
#ifndef PALM_1
SOAP_FMAC1
int
SOAP_FMAC2
soap_dime_forward(struct soap *soap, unsigned char **ptr, int *size, char **id, char **type, char **options)
{ struct soap_xlist *xp;
  *ptr = NULL;
  *size = 0;
  *id = NULL;
  *type = NULL;
  *options = NULL;
  if (!*soap->href)
    return SOAP_OK;
  *id = soap_strdup(soap, soap->href);
  xp = (struct soap_xlist*)SOAP_MALLOC(soap, sizeof(struct soap_xlist));
  if (!xp)
    return soap->error = SOAP_EOM;
  xp->next = soap->xlist;
  xp->ptr = ptr;
  xp->size = size;
  xp->id = *id;
  xp->type = type;
  xp->options = options;
  soap->xlist = xp;
  return SOAP_OK;
}
#endif
#endif

/******************************************************************************/
#ifndef PALM_1
SOAP_FMAC1
char *
SOAP_FMAC2
soap_strdup(struct soap *soap, const char *s)
{ char *t = NULL;
  if (s && (t = (char*)soap_malloc(soap, strlen(s) + 1)))
    strcpy(t, s);
  return t;
}
#endif

/******************************************************************************/
#ifndef PALM_1
SOAP_FMAC1
wchar_t *
SOAP_FMAC2
soap_wstrdup(struct soap *soap, const wchar_t *s)
{ wchar_t *t = NULL;
  if (s)
  { size_t n = 0;
    while (s[n])
      n++;
    if ((t = (wchar_t*)soap_malloc(soap, sizeof(wchar_t)*(n+1))))
      memcpy(t, s, sizeof(wchar_t)*(n+1));
  }
  return t;
}
#endif

/******************************************************************************/
#ifndef PALM_1
SOAP_FMAC1
struct soap_blist*
SOAP_FMAC2
soap_new_block(struct soap *soap)
{ struct soap_blist *p;
  DBGLOG(TEST, SOAP_MESSAGE(fdebug, "New block sequence (prev=%p)\n", soap->blist));
  if (!(p = (struct soap_blist*)SOAP_MALLOC(soap, sizeof(struct soap_blist))))
  { soap->error = SOAP_EOM;
    return NULL;
  }
  p->next = soap->blist;
  p->ptr = NULL;
  p->size = 0;
  soap->blist = p;
  return p;
}
#endif

/******************************************************************************/
#ifndef PALM_1
SOAP_FMAC1
void*
SOAP_FMAC2
soap_push_block(struct soap *soap, struct soap_blist *b, size_t n)
{ char *p;
  if (!b)
    b = soap->blist;
  DBGLOG(TEST, SOAP_MESSAGE(fdebug, "Push block of %u bytes (%u bytes total)\n", (unsigned int)n, (unsigned int)b->size + (unsigned int)n));
  if (!(p = (char*)SOAP_MALLOC(soap, n + sizeof(char*) + sizeof(size_t))))
  { soap->error = SOAP_EOM;
    return NULL;
  }
  *(char**)p = b->ptr;
  *(size_t*)(p + sizeof(char*)) = n;
  b->ptr = p;
  b->size += n;
  return p + sizeof(char*) + sizeof(size_t);
}
#endif

/******************************************************************************/
#ifndef PALM_1
SOAP_FMAC1
void
SOAP_FMAC2
soap_pop_block(struct soap *soap, struct soap_blist *b)
{ char *p;
  if (!b)
    b = soap->blist;
  if (!b->ptr)
    return;
  DBGLOG(TEST, SOAP_MESSAGE(fdebug, "Pop block\n"));
  p = b->ptr;
  b->size -= *(size_t*)(p + sizeof(char*));
  b->ptr = *(char**)p;
  SOAP_FREE(soap, p);
}
#endif

/******************************************************************************/
#ifndef PALM_1
SOAP_FMAC1
void
SOAP_FMAC2
soap_update_pointers(struct soap *soap, char *start, char *end, char *p1, char *p2)
{
#ifndef WITH_NOIDREF
  int i;
  register struct soap_ilist *ip = NULL;
  register struct soap_flist *fp = NULL;
#ifndef WITH_LEANER
  register struct soap_xlist *xp = NULL;
#endif
  register void *p, **q;
  for (i = 0; i < SOAP_IDHASH; i++)
  { for (ip = soap->iht[i]; ip; ip = ip->next)
    { if (ip->ptr && (char*)ip->ptr >= start && (char*)ip->ptr < end)
      { DBGLOG(TEST, SOAP_MESSAGE(fdebug, "Update id='%s' %p -> %p\n", ip->id, ip->ptr, (char*)ip->ptr + (p1-p2)));
        ip->ptr = (char*)ip->ptr + (p1-p2);
      }
      for (q = &ip->link; q; q = (void**)p)
      { p = *q;
        if (p && (char*)p >= start && (char*)p < end)
        { DBGLOG(TEST, SOAP_MESSAGE(fdebug, "Link update id='%s' %p\n", ip->id, p));
          *q = (char*)p + (p1-p2);
        }
      }
      for (q = &ip->copy; q; q = (void**)p)
      { p = *q;
        if (p && (char*)p >= start && (char*)p < end)
        { DBGLOG(TEST, SOAP_MESSAGE(fdebug, "Copy chain update id='%s' %p\n", ip->id, p));
          *q = (char*)p + (p1-p2);
        }
      }
      for (fp = ip->flist; fp; fp = fp->next)
      { if ((char*)fp->ptr >= start && (char*)fp->ptr < end)
        { DBGLOG(TEST, SOAP_MESSAGE(fdebug, "Copy list update id='%s' %p\n", ip->id, fp));
          fp->ptr = (char*)fp->ptr + (p1-p2);
        }
      }
    }
  }
#ifndef WITH_LEANER
  for (xp = soap->xlist; xp; xp = xp->next)
  { if (xp->ptr && (char*)xp->ptr >= start && (char*)xp->ptr < end)
    { DBGLOG(TEST, SOAP_MESSAGE(fdebug, "Update id='%s' %p -> %p\n", xp->id ? xp->id : SOAP_STR_EOS, xp->ptr, (char*)xp->ptr + (p1-p2)));
      xp->ptr = (unsigned char**)((char*)xp->ptr + (p1-p2));
      xp->size = (int*)((char*)xp->size + (p1-p2));
      xp->type = (char**)((char*)xp->type + (p1-p2));
      xp->options = (char**)((char*)xp->options + (p1-p2));
    }
  }
#endif
#else
  (void)soap; (void)start; (void)end; (void)p1; (void)p2;
#endif
}
#endif

/******************************************************************************/
#ifndef WITH_NOIDREF
#ifndef PALM_1
static int
soap_has_copies(struct soap *soap, register const char *start, register const char *end)
{ register int i;
  register struct soap_ilist *ip = NULL;
  register struct soap_flist *fp = NULL;
  register const char *p;
  for (i = 0; i < SOAP_IDHASH; i++)
  { for (ip = soap->iht[i]; ip; ip = ip->next)
    { for (p = (const char*)ip->copy; p; p = *(const char**)p)
        if (p >= start && p < end)
          return SOAP_ERR;
      for (fp = ip->flist; fp; fp = fp->next)
        if ((const char*)fp->ptr >= start && (const char*)fp->ptr < end)
          return SOAP_ERR;
    }
  }
  return SOAP_OK;
}
#endif
#endif

/******************************************************************************/
#ifndef WITH_NOIDREF
#ifndef PALM_1
SOAP_FMAC1
int
SOAP_FMAC2
soap_resolve(struct soap *soap)
{ register int i;
  register struct soap_ilist *ip = NULL;
  register struct soap_flist *fp = NULL;
  short flag;
  DBGLOG(TEST, SOAP_MESSAGE(fdebug, "Resolving forwarded data\n"));
  for (i = 0; i < SOAP_IDHASH; i++)
  { for (ip = soap->iht[i]; ip; ip = ip->next)
    { if (ip->ptr)
      { register void *p, **q, *r;
        q = (void**)ip->link;
        ip->link = NULL;
        r = ip->ptr;
        DBGLOG(TEST, if (q) SOAP_MESSAGE(fdebug, "Traversing link chain to resolve id='%s'\n", ip->id));
        while (q)
        { p = *q;
          *q = r;
          DBGLOG(TEST,SOAP_MESSAGE(fdebug, "... link %p -> %p\n", q, r));
          q = (void**)p;
        }
      }
      else if (*ip->id == '#')
      { DBGLOG(TEST, SOAP_MESSAGE(fdebug, "Missing data for id='%s'\n", ip->id));
        strcpy(soap->id, ip->id + 1);
        return soap->error = SOAP_MISSING_ID;
      }
    }
  }
  do
  { flag = 0;
    DBGLOG(TEST, SOAP_MESSAGE(fdebug, "Resolution phase\n"));
    for (i = 0; i < SOAP_IDHASH; i++)
    { for (ip = soap->iht[i]; ip; ip = ip->next)
      { if (ip->ptr && !soap_has_copies(soap, (const char*)ip->ptr, (const char*)ip->ptr + ip->size))
        { if (ip->copy)
          { register void *p, **q = (void**)ip->copy;
            DBGLOG(TEST, if (q) SOAP_MESSAGE(fdebug, "Traversing copy chain to resolve id='%s'\n", ip->id));
            ip->copy = NULL;
            do
            { DBGLOG(TEST, SOAP_MESSAGE(fdebug, "... copy %p -> %p (%u bytes)\n", ip->ptr, q, (unsigned int)ip->size));
              p = *q;
              memcpy(q, ip->ptr, ip->size);
              q = (void**)p;
            } while (q);
            flag = 1;
          }
          for (fp = ip->flist; fp; fp = ip->flist)
          { register unsigned int k = fp->level;
            register void *p = ip->ptr;
            DBGLOG(TEST, SOAP_MESSAGE(fdebug, "Resolving forwarded data type=%d location=%p level=%u,%u id='%s'\n", ip->type, p, ip->level, fp->level, ip->id));
            while (ip->level < k)
            { register void **q = (void**)soap_malloc(soap, sizeof(void*));
              if (!q)
                return soap->error;
              *q = p;
              DBGLOG(TEST, SOAP_MESSAGE(fdebug, "Descending one level, new location=%p holds=%p...\n", q, *q));
              p = (void*)q;
              k--;
            }
            if (fp->fcopy)
              fp->fcopy(soap, ip->type, fp->type, fp->ptr, fp->len, p, ip->size);
            else
              soap_fcopy(soap, ip->type, fp->type, fp->ptr, fp->len, p, ip->size);
            ip->flist = fp->next;
            SOAP_FREE(soap, fp);
            flag = 1;
          }
        }
      }
    }
  } while (flag);
#ifdef SOAP_DEBUG
  for (i = 0; i < SOAP_IDHASH; i++)
  { for (ip = soap->iht[i]; ip; ip = ip->next)
    { if (ip->copy || ip->flist)
      { DBGLOG(TEST, SOAP_MESSAGE(fdebug, "Resolution error: forwarded data for id='%s' could not be propagated, please report this problem to the developers\n", ip->id));
      }
    }
  }
#endif
  DBGLOG(TEST, SOAP_MESSAGE(fdebug, "Resolution done\n"));
  return SOAP_OK;
}
#endif
#endif

/******************************************************************************/
#ifndef PALM_1
SOAP_FMAC1
size_t
SOAP_FMAC2
soap_size_block(struct soap *soap, struct soap_blist *b, size_t n)
{ if (!b)
    b = soap->blist;
  if (b->ptr)
  { b->size -= *(size_t*)(b->ptr + sizeof(char*)) - n;
    *(size_t*)(b->ptr + sizeof(char*)) = n;
  }
  return b->size;
}
#endif

/******************************************************************************/
#ifndef PALM_1
SOAP_FMAC1
char*
SOAP_FMAC2
soap_first_block(struct soap *soap, struct soap_blist *b)
{ char *p, *q, *r;
  if (!b)
    b = soap->blist;
  p = b->ptr;
  if (!p)
    return NULL;
  DBGLOG(TEST, SOAP_MESSAGE(fdebug, "First block\n"));
  r = NULL;
  do
  { q = *(char**)p;
    *(char**)p = r;
    r = p;
    p = q;
  } while (p);
  b->ptr = r;
  return r + sizeof(char*) + sizeof(size_t);
}
#endif

/******************************************************************************/
#ifndef PALM_1
SOAP_FMAC1
char*
SOAP_FMAC2
soap_next_block(struct soap *soap, struct soap_blist *b)
{ char *p;
  if (!b)
    b = soap->blist;
  p = b->ptr;
  if (p)
  { DBGLOG(TEST, SOAP_MESSAGE(fdebug, "Next block\n"));
    b->p