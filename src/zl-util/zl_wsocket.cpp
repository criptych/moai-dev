// LuaSocket 2.0.2 license
// Copyright © 2004-2007 Diego Nehab
//
// Permission is hereby granted, free of charge, to any person obtaining a
// copy of this software and associated documentation files (the "Software"),
// to deal in the Software without restriction, including without limitation
// the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.


// modifications Copyright (c) 2014 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

SUPPRESS_EMPTY_FILE_WARNING

#ifdef _WIN32

/*=========================================================================*\
* Socket compatibilization module for Win32
* LuaSocket toolkit
*
* The penalty of calling select to avoid busy-wait is only paid when
* the I/O call fail in the first place. 
*
* RCS ID: $Id: wsocket.c,v 1.36 2007/06/11 23:44:54 diego Exp $
\*=========================================================================*/

#include <string.h>

#include <zl-util/zl_socket.h>
#include <zl-util/ZLDeviceTime.h>

/* WinSock doesn't have a strerror... */
static const char *wstrerror(int err);

/*-------------------------------------------------------------------------*\
* Initializes module 
\*-------------------------------------------------------------------------*/
int zl_socket_open(void) {
    WSADATA wsaData;
    WORD wVersionRequested = MAKEWORD(2, 0); 
    int err = WSAStartup(wVersionRequested, &wsaData );
    if (err != 0) return 0;
    if ((LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 0) &&
        (LOBYTE(wsaData.wVersion) != 1 || HIBYTE(wsaData.wVersion) != 1)) {
        WSACleanup();
        return 0; 
    }
    return 1;
}

/*-------------------------------------------------------------------------*\
* Close module 
\*-------------------------------------------------------------------------*/
int zl_socket_close(void) {
    WSACleanup();
    return 1;
}

/*-------------------------------------------------------------------------*\
* Wait for readable/writable/connected socket with timeout
\*-------------------------------------------------------------------------*/
#define WAITFD_R        1
#define WAITFD_W        2
#define WAITFD_E        4
#define WAITFD_C        (WAITFD_E|WAITFD_W)

/* disable warning for "conditional expression is constant." win 8.1 winsock.h has a while(0) for FD_SET*/
#pragma warning( push )
#pragma warning( disable : 4127 ) 

int zl_socket_waitfd(zl_socket* ps, int sw, double tm) {
    int ret;
    fd_set rfds, wfds, efds, *rp = NULL, *wp = NULL, *ep = NULL;
    struct timeval tv, *tp = NULL;
    double t;
    if (tm == 0.0) return ZL_IO_TIMEOUT;  /* optimize timeout == 0 case */
    if (sw & WAITFD_R) { 
        FD_ZERO(&rfds); 
		FD_SET(*ps, &rfds);
        rp = &rfds; 
    }
    if (sw & WAITFD_W) { FD_ZERO(&wfds); FD_SET(*ps, &wfds); wp = &wfds; }
    if (sw & WAITFD_C) { FD_ZERO(&efds); FD_SET(*ps, &efds); ep = &efds; }
	t = tm - ZLDeviceTime::GetTimeInSeconds();
	tp = NULL;
    if (t >= 0.0) {
        tv.tv_sec = (int) t;
        tv.tv_usec = (int) ((t-tv.tv_sec)*1.0e6);
        tp = &tv;
    }
    ret = select(0, rp, wp, ep, tp);
    if (ret == -1) return WSAGetLastError();
    if (ret == 0) return ZL_IO_TIMEOUT;
    if (sw == WAITFD_C && FD_ISSET(*ps, &efds)) return ZL_IO_CLOSED;
    return ZL_IO_DONE;
}
#pragma warning( pop )

/*-------------------------------------------------------------------------*\
* Put socket into blocking mode
\*-------------------------------------------------------------------------*/
void zl_socket_setblocking(zl_socket* ps) {
	u_long argp = 0;
	ioctlsocket(*ps, FIONBIO, &argp);
}

/*-------------------------------------------------------------------------*\
* Put socket into non-blocking mode
\*-------------------------------------------------------------------------*/
void zl_socket_setnonblocking(zl_socket* ps) {
	u_long argp = 1;
	ioctlsocket(*ps, FIONBIO, &argp);
}


/*-------------------------------------------------------------------------*\
* Select with int timeout in ms
\*-------------------------------------------------------------------------*/
int zl_socket_select(zl_socket n, fd_set *rfds, fd_set *wfds, fd_set *efds, 
        double tm) {
    struct timeval tv; 
    double t = tm;
    tv.tv_sec = (int) t;
    tv.tv_usec = (int) ((t - tv.tv_sec) * 1.0e6);
    if (n <= 0) {
        Sleep((DWORD) (1000*t));
        return 0;
    } else return select(0, rfds, wfds, efds, t >= 0.0? &tv: NULL);
}

/*-------------------------------------------------------------------------*\
* Close and inutilize socket
\*-------------------------------------------------------------------------*/
void zl_socket_destroy(zl_socket* ps) {
    if (*ps != SOCKET_INVALID) {
        zl_socket_setblocking(ps); /* close can take a long time on WIN32 */
        closesocket(*ps);
        *ps = SOCKET_INVALID;
    }
}

/*-------------------------------------------------------------------------*\
* 
\*-------------------------------------------------------------------------*/
void zl_socket_shutdown(zl_socket* ps, int how) {
    zl_socket_setblocking(ps);
    shutdown(*ps, how);
    zl_socket_setnonblocking(ps);
}

/*-------------------------------------------------------------------------*\
* Creates and sets up a socket
\*-------------------------------------------------------------------------*/
int zl_socket_create(zl_socket* ps, int domain, int type, int protocol) {
    *ps = socket(domain, type, protocol);
    if (*ps != SOCKET_INVALID) return ZL_IO_DONE;
    else return WSAGetLastError();
}

/*-------------------------------------------------------------------------*\
* Connects or returns error message
\*-------------------------------------------------------------------------*/
int zl_socket_connect(zl_socket* ps, zl_sockaddr *addr, socklen_t len, double tm) {
    int err;
    /* don't call on closed socket */
    if (*ps == SOCKET_INVALID) return ZL_IO_CLOSED;
    /* ask system to connect */
    if (connect(*ps, addr, len) == 0) return ZL_IO_DONE;
    /* make sure the system is trying to connect */
    err = WSAGetLastError();
    if (err != WSAEWOULDBLOCK && err != WSAEINPROGRESS) return err;
    /* zero timeout case optimization */
    if (tm == 0.0) return ZL_IO_TIMEOUT;
    /* we wait until something happens */
    err = zl_socket_waitfd(ps, WAITFD_C, tm);
    if (err == ZL_IO_CLOSED) {
        int errlen = sizeof(err);
        /* give windows time to set the error (yes, disgusting) */
        Sleep(10);
        /* find out why we failed */
        getsockopt(*ps, SOL_SOCKET, SO_ERROR, (char *)&err, &errlen); 
        /* we KNOW there was an error. if 'why' is 0, we will return
        * "unknown error", but it's not really our fault */
        return err > 0? err: ZL_IO_UNKNOWN; 
    } else return err;

}

/*-------------------------------------------------------------------------*\
* Binds or returns error message
\*-------------------------------------------------------------------------*/
int zl_socket_bind(zl_socket* ps, zl_sockaddr *addr, socklen_t len) {
    int err = ZL_IO_DONE;
    zl_socket_setblocking(ps);
    if (bind(*ps, addr, len) < 0) err = WSAGetLastError();
    zl_socket_setnonblocking(ps);
    return err;
}

/*-------------------------------------------------------------------------*\
* 
\*-------------------------------------------------------------------------*/
int zl_socket_listen(zl_socket* ps, int backlog) {
    int err = ZL_IO_DONE;
    zl_socket_setblocking(ps);
    if (listen(*ps, backlog) < 0) err = WSAGetLastError();
    zl_socket_setnonblocking(ps);
    return err;
}

/*-------------------------------------------------------------------------*\
* Accept with timeout
\*-------------------------------------------------------------------------*/
int zl_socket_accept(zl_socket* ps, zl_socket* pa, zl_sockaddr *addr, socklen_t *len, 
        double tm) {
    zl_sockaddr daddr;
    socklen_t dlen = sizeof(daddr);
    if (*ps == SOCKET_INVALID) return ZL_IO_CLOSED;
    if (!addr) addr = &daddr;
    if (!len) len = &dlen;
    for ( ;; ) {
        int err;
        /* try to get client socket */
        if ((*pa = accept(*ps, addr, len)) != SOCKET_INVALID) return ZL_IO_DONE;
        /* find out why we failed */
        err = WSAGetLastError(); 
        /* if we failed because there was no connectoin, keep trying */
        if (err != WSAEWOULDBLOCK && err != WSAECONNABORTED) return err;
        /* call select to avoid busy wait */
        if ((err = zl_socket_waitfd(ps, WAITFD_R, tm)) != ZL_IO_DONE) return err;
    } 
    /* can't reach here */
    //return ZL_IO_UNKNOWN; 
}

/*-------------------------------------------------------------------------*\
* Send with timeout
* On windows, if you try to send 10MB, the OS will buffer EVERYTHING 
* this can take an awful lot of time and we will end up blocked. 
* Therefore, whoever calls this function should not pass a huge buffer.
\*-------------------------------------------------------------------------*/
int zl_socket_send(zl_socket* ps, const char *data, size_t count, 
        size_t *sent, double tm)
{
    int err;
    *sent = 0;
    /* avoid making system calls on closed sockets */
    if (*ps == SOCKET_INVALID) return ZL_IO_CLOSED;
    /* loop until we send something or we give up on error */
    for ( ;; ) {
        /* try to send something */
		int put = send(*ps, data, (int) count, 0);
        /* if we sent something, we are done */
        if (put > 0) {
            *sent = put;
            return ZL_IO_DONE;
        }
        /* deal with failure */
        err = WSAGetLastError(); 
        /* we can only proceed if there was no serious error */
        if (err != WSAEWOULDBLOCK) return err;
        /* avoid busy wait */
        if ((err = zl_socket_waitfd(ps, WAITFD_W, tm)) != ZL_IO_DONE) return err;
    } 
    /* can't reach here */
    //return ZL_IO_UNKNOWN;
}

/*-------------------------------------------------------------------------*\
* Sendto with timeout
\*-------------------------------------------------------------------------*/
int zl_socket_sendto(zl_socket* ps, const char *data, size_t count, size_t *sent, 
       zl_sockaddr *addr, socklen_t len, double tm)
{
    int err;
    *sent = 0;
    if (*ps == SOCKET_INVALID) return ZL_IO_CLOSED;
    for ( ;; ) {
        int put = sendto(*ps, data, (int) count, 0, addr, len);
        if (put > 0) {
            *sent = put;
            return ZL_IO_DONE;
        }
        err = WSAGetLastError(); 
        if (err != WSAEWOULDBLOCK) return err;
        if ((err = zl_socket_waitfd(ps, WAITFD_W, tm)) != ZL_IO_DONE) return err;
    } 
    //return ZL_IO_UNKNOWN;
}

/*-------------------------------------------------------------------------*\
* Receive with timeout
\*-------------------------------------------------------------------------*/
int zl_socket_recv(zl_socket* ps, char *data, size_t count, size_t *got, double tm) {
    int err;
    *got = 0;
    if (*ps == SOCKET_INVALID) return ZL_IO_CLOSED;
    for ( ;; ) {
        int taken = recv(*ps, data, (int) count, 0);
        if (taken > 0) {
            *got = taken;
            return ZL_IO_DONE;
        }
        if (taken == 0) return ZL_IO_CLOSED;
        err = WSAGetLastError();
        if (err != WSAEWOULDBLOCK) return err;
        if ((err = zl_socket_waitfd(ps, WAITFD_R, tm)) != ZL_IO_DONE) return err;
    }
    //return ZL_IO_UNKNOWN;
}

/*-------------------------------------------------------------------------*\
* Recvfrom with timeout
\*-------------------------------------------------------------------------*/
int zl_socket_recvfrom(zl_socket* ps, char *data, size_t count, size_t *got, 
        zl_sockaddr *addr, socklen_t *len, double tm) {
    int err;
    *got = 0;
    if (*ps == SOCKET_INVALID) return ZL_IO_CLOSED;
    for ( ;; ) {
        int taken = recvfrom(*ps, data, (int) count, 0, addr, len);
        if (taken > 0) {
            *got = taken;
            return ZL_IO_DONE;
        }
        if (taken == 0) return ZL_IO_CLOSED;
        err = WSAGetLastError();
        if (err != WSAEWOULDBLOCK) return err;
        if ((err = zl_socket_waitfd(ps, WAITFD_R, tm)) != ZL_IO_DONE) return err;
    }
    //return ZL_IO_UNKNOWN;
}


/*-------------------------------------------------------------------------*\
* DNS helpers 
\*-------------------------------------------------------------------------*/
int zl_socket_gethostbyaddr(const char *addr, socklen_t len, struct hostent **hp) {
    *hp = gethostbyaddr(addr, len, AF_INET);
    if (*hp) return ZL_IO_DONE;
    else return WSAGetLastError();
}

int zl_socket_gethostbyname(const char *addr, struct hostent **hp) {
    *hp = gethostbyname(addr);
    if (*hp) return ZL_IO_DONE;
    else return  WSAGetLastError();
}

/*-------------------------------------------------------------------------*\
* Error translation functions
\*-------------------------------------------------------------------------*/

//----------------------------------------------------------------//
cc8* zl_ZL_IO_strerror(int err) {
	switch (err) {
	case ZL_IO_DONE: return NULL;
	case ZL_IO_CLOSED: return "closed";
	case ZL_IO_TIMEOUT: return "timeout";
	default: return "unknown error";
	}
}

const char *socket_hoststrerror(int err) {
    if (err <= 0) return zl_ZL_IO_strerror(err);
    switch (err) {
        case WSAHOST_NOT_FOUND: return "host not found";
        default: return wstrerror(err); 
    }
}

const char *socket_strerror(int err) {
    if (err <= 0) return zl_ZL_IO_strerror(err);
    switch (err) {
        case WSAEADDRINUSE: return "address already in use";
        case WSAECONNREFUSED: return "connection refused";
        case WSAEISCONN: return "already connected";
        case WSAEACCES: return "permission denied";
        case WSAECONNABORTED: return "closed";
        case WSAECONNRESET: return "closed";
        case WSAETIMEDOUT: return "timeout";
        default: return wstrerror(err);
    }
}

const char *socket_ioerror(zl_socket* ps, int err) {
	(void) ps;
	return zl_socket_strerror(err);
}

static const char *wstrerror(int err) {
    switch (err) {
        case WSAEINTR: return "Interrupted function call";
        case WSAEACCES: return "Permission denied";
        case WSAEFAULT: return "Bad address";
        case WSAEINVAL: return "Invalid argument";
        case WSAEMFILE: return "Too many open files";
        case WSAEWOULDBLOCK: return "Resource temporarily unavailable";
        case WSAEINPROGRESS: return "Operation now in progress";
        case WSAEALREADY: return "Operation already in progress";
        case WSAENOTSOCK: return "Socket operation on nonsocket";
        case WSAEDESTADDRREQ: return "Destination address required";
        case WSAEMSGSIZE: return "Message too long";
        case WSAEPROTOTYPE: return "Protocol wrong type for socket";
        case WSAENOPROTOOPT: return "Bad protocol option";
        case WSAEPROTONOSUPPORT: return "Protocol not supported";
        case WSAESOCKTNOSUPPORT: return "Socket type not supported";
        case WSAEOPNOTSUPP: return "Operation not supported";
        case WSAEPFNOSUPPORT: return "Protocol family not supported";
        case WSAEAFNOSUPPORT: 
            return "Address family not supported by protocol family"; 
        case WSAEADDRINUSE: return "Address already in use";
        case WSAEADDRNOTAVAIL: return "Cannot assign requested address";
        case WSAENETDOWN: return "Network is down";
        case WSAENETUNREACH: return "Network is unreachable";
        case WSAENETRESET: return "Network dropped connection on reset";
        case WSAECONNABORTED: return "Software caused connection abort";
        case WSAECONNRESET: return "Connection reset by peer";
        case WSAENOBUFS: return "No buffer space available";
        case WSAEISCONN: return "Socket is already connected";
        case WSAENOTCONN: return "Socket is not connected";
        case WSAESHUTDOWN: return "Cannot send after socket shutdown";
        case WSAETIMEDOUT: return "Connection timed out";
        case WSAECONNREFUSED: return "Connection refused";
        case WSAEHOSTDOWN: return "Host is down";
        case WSAEHOSTUNREACH: return "No route to host";
        case WSAEPROCLIM: return "Too many processes";
        case WSASYSNOTREADY: return "Network subsystem is unavailable";
        case WSAVERNOTSUPPORTED: return "Winsock.dll version out of range";
        case WSANOTINITIALISED: 
            return "Successful WSAStartup not yet performed";
        case WSAEDISCON: return "Graceful shutdown in progress";
        case WSAHOST_NOT_FOUND: return "Host not found";
        case WSATRY_AGAIN: return "Nonauthoritative host not found";
        case WSANO_RECOVERY: return "Nonrecoverable name lookup error"; 
        case WSANO_DATA: return "Valid name, no data record of requested type";
        default: return "Unknown error";
    }
}

#endif
