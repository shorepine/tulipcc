// WinHTTP-backed HTTP(S) fetch for the Tulip Desktop Windows port.
//
// The MinGW MicroPython build has no socket/ssl modules, so tuliprequests
// (and with it Tulip World) can't use its normal socket path. Windows ships
// a full HTTP+TLS stack in winhttp.dll — expose one blocking call:
//
//   tulip.win_http_fetch(method, url, headers_or_None, body_or_None)
//       -> (status_code:int, content:bytes)
//
// headers is a single "K: V\r\nK2: V2" string. tuliprequests.py wraps this
// in a requests-compatible request()/Response when `import socket` fails.

#include <windows.h>
#include <winhttp.h>
#include <string.h>

#include "py/runtime.h"
#include "py/mphal.h"

static wchar_t *u8_to_wide(const char *s) {
    int n = MultiByteToWideChar(CP_UTF8, 0, s, -1, NULL, 0);
    wchar_t *w = m_new(wchar_t, n > 0 ? n : 1);
    if (n > 0) {
        MultiByteToWideChar(CP_UTF8, 0, s, -1, w, n);
    } else {
        w[0] = 0;
    }
    return w;
}

static void raise_http_os_error(const char *what) {
    mp_raise_msg_varg(&mp_type_OSError, MP_ERROR_TEXT("%s failed (winhttp err %u)"),
        what, (unsigned)GetLastError());
}

mp_obj_t tulip_win_http_fetch(size_t n_args, const mp_obj_t *args) {
    (void)n_args;
    const char *method = mp_obj_str_get_str(args[0]);
    const char *url = mp_obj_str_get_str(args[1]);
    const char *headers = (args[2] == mp_const_none) ? NULL : mp_obj_str_get_str(args[2]);
    mp_buffer_info_t body = {0};
    if (args[3] != mp_const_none) {
        mp_get_buffer_raise(args[3], &body, MP_BUFFER_READ);
    }

    wchar_t *wurl = u8_to_wide(url);
    wchar_t whost[256] = {0};
    wchar_t wpath[2048] = {0};
    URL_COMPONENTSW uc = {0};
    uc.dwStructSize = sizeof(uc);
    uc.lpszHostName = whost;
    uc.dwHostNameLength = sizeof(whost) / sizeof(whost[0]);
    uc.lpszUrlPath = wpath;
    uc.dwUrlPathLength = sizeof(wpath) / sizeof(wpath[0]);
    if (!WinHttpCrackUrl(wurl, 0, 0, &uc)) {
        raise_http_os_error("parse url");
    }
    bool secure = (uc.nScheme == INTERNET_SCHEME_HTTPS);

    HINTERNET ses = WinHttpOpen(L"TulipDesktop/1.0",
        WINHTTP_ACCESS_TYPE_DEFAULT_PROXY, WINHTTP_NO_PROXY_NAME, WINHTTP_NO_PROXY_BYPASS, 0);
    if (!ses) {
        raise_http_os_error("open session");
    }
    HINTERNET conn = WinHttpConnect(ses, whost, uc.nPort, 0);
    if (!conn) {
        WinHttpCloseHandle(ses);
        raise_http_os_error("connect");
    }
    wchar_t *wmethod = u8_to_wide(method);
    HINTERNET req = WinHttpOpenRequest(conn, wmethod, wpath, NULL,
        WINHTTP_NO_REFERER, WINHTTP_DEFAULT_ACCEPT_TYPES, secure ? WINHTTP_FLAG_SECURE : 0);
    if (!req) {
        WinHttpCloseHandle(conn);
        WinHttpCloseHandle(ses);
        raise_http_os_error("open request");
    }

    wchar_t *wheaders = headers ? u8_to_wide(headers) : NULL;
    BOOL ok = WinHttpSendRequest(req,
        wheaders ? wheaders : WINHTTP_NO_ADDITIONAL_HEADERS, wheaders ? (DWORD)-1 : 0,
        body.len ? (LPVOID)body.buf : WINHTTP_NO_REQUEST_DATA, (DWORD)body.len,
        (DWORD)body.len, 0);
    if (ok) {
        ok = WinHttpReceiveResponse(req, NULL);
    }
    DWORD status = 0;
    if (ok) {
        DWORD sz = sizeof(status);
        ok = WinHttpQueryHeaders(req, WINHTTP_QUERY_STATUS_CODE | WINHTTP_QUERY_FLAG_NUMBER,
            WINHTTP_HEADER_NAME_BY_INDEX, &status, &sz, WINHTTP_NO_HEADER_INDEX);
    }

    vstr_t vstr;
    vstr_init(&vstr, 4096);
    while (ok) {
        DWORD avail = 0;
        if (!WinHttpQueryDataAvailable(req, &avail)) {
            ok = FALSE;
            break;
        }
        if (avail == 0) {
            break; // done
        }
        char *dst = vstr_add_len(&vstr, avail);
        DWORD got = 0;
        if (!WinHttpReadData(req, dst, avail, &got)) {
            ok = FALSE;
            break;
        }
        vstr.len -= (avail - got); // trim short read
    }

    DWORD err = ok ? 0 : GetLastError();
    WinHttpCloseHandle(req);
    WinHttpCloseHandle(conn);
    WinHttpCloseHandle(ses);
    if (!ok) {
        vstr_clear(&vstr);
        mp_raise_msg_varg(&mp_type_OSError, MP_ERROR_TEXT("http request failed (winhttp err %u)"),
            (unsigned)err);
    }

    mp_obj_t items[2] = {
        MP_OBJ_NEW_SMALL_INT(status),
        mp_obj_new_bytes((const byte *)vstr.buf, vstr.len),
    };
    vstr_clear(&vstr);
    return mp_obj_new_tuple(2, items);
}
MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(tulip_win_http_fetch_obj, 4, 4, tulip_win_http_fetch);
