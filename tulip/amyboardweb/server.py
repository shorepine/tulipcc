#!/usr/bin/env python

# Attribution: https://stackoverflow.com/questions/21956683/enable-access-control-on-simple-http-server

try:
    # Python 3
    from http.server import HTTPServer, SimpleHTTPRequestHandler, test as test_orig
    from urllib.parse import parse_qs, urlparse
    from urllib.request import Request, urlopen
    import sys

    def test(*args):
        test_orig(*args, port=int(sys.argv[1]) if len(sys.argv) > 1 else 8000)
except ImportError:  # Python 2
    from BaseHTTPServer import HTTPServer, test
    from SimpleHTTPServer import SimpleHTTPRequestHandler

class CORSRequestHandler(SimpleHTTPRequestHandler):
    def end_headers(self):
        self.send_header('Access-Control-Allow-Origin', '*')
        self.send_header('Access-Control-Allow-Methods', 'GET, HEAD, OPTIONS')
        self.send_header('Cross-Origin-Embedder-Policy', 'require-corp')
        self.send_header('Cross-Origin-Opener-Policy', 'same-origin')
        # Development server: avoid stale cached JS/CSS after local rebuilds.
        self.send_header('Cache-Control', 'no-store, no-cache, must-revalidate, max-age=0')
        self.send_header('Pragma', 'no-cache')
        self.send_header('Expires', '0')
        SimpleHTTPRequestHandler.end_headers(self)

    def do_OPTIONS(self):
        self.send_response(204)
        self.end_headers()

    def do_GET(self):
        if self.path.startswith("/proxy") or self.path.startswith("/api/firmware"):
            self._handle_proxy()
            return
        return SimpleHTTPRequestHandler.do_GET(self)

    def _handle_proxy(self):
        query = parse_qs(urlparse(self.path).query)
        url = query.get("url", [None])[0]
        if not url:
            self.send_error(400, "Missing url parameter")
            return

        parsed = urlparse(url)
        allow_hosts = {
            "github.com",
            "objects.githubusercontent.com",
            "raw.githubusercontent.com",
        }
        if parsed.scheme not in ("http", "https") or parsed.hostname not in allow_hosts:
            self.send_error(403, "URL not allowed")
            return

        try:
            req = Request(url, headers={"User-Agent": "tulipcc-local-proxy"})
            with urlopen(req) as resp:
                data = resp.read()
                self.send_response(getattr(resp, "status", 200))
                content_type = resp.headers.get("Content-Type", "application/octet-stream")
                self.send_header("Content-Type", content_type)
                self.send_header("Content-Length", str(len(data)))
                self.end_headers()
                self.wfile.write(data)
        except Exception as exc:
            self.send_error(502, "Proxy fetch failed: %s" % exc)

if __name__ == '__main__':
    import os
    os.chdir("stage")
    test(CORSRequestHandler, HTTPServer)
