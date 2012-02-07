// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__45D84E80_0D80_41DF_90A1_2FAE8918088B__INCLUDED_)
#define AFX_STDAFX_H__45D84E80_0D80_41DF_90A1_2FAE8918088B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers

#include <stdio.h>

// TODO: reference additional headers your program requires here
#ifndef WIN32
 #include <sys/time.h>
 #include <sys/types.h>
 #include <sys/socket.h>
 #include <netinet/in.h>
 #include <netdb.h>
 #include <netinet/tcp.h>
 #include <arpa/inet.h>
 #include <sys/utsname.h>
 #include <unistd.h>
 #include <fcntl.h>
 #include<ctype.h>
 typedef int SOCKET;
 #define SOCKET_ERROR -1
 typedef void* LPVOID;
#else
 #include <winsock2.h>
 #include <ws2tcpip.h>
 
 typedef int socklen_t;
 #pragma comment(lib,"ws2_32.lib")
#endif
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__45D84E80_0D80_41DF_90A1_2FAE8918088B__INCLUDED_)
