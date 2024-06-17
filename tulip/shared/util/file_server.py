#!/usr/bin/env python

"""Extend Python's built in HTTP server to save files

curl or wget can be used to send files with options similar to the following

  curl -X PUT --upload-file somefile.txt http://localhost:8000
  wget -O- --method=PUT --body-file=somefile.txt http://localhost:8000/somefile.txt

__Note__: curl automatically appends the filename onto the end of the URL so
the path can be omitted.

"""
import os
try:
    import http.server as server
except ImportError:
    # Handle Python 2.x
    import SimpleHTTPServer as server

class HTTPRequestHandler(server.SimpleHTTPRequestHandler):
    def do_PUT(self):
        self.send_response(200)
        self.end_headers()
        print(str(self.headers))
        path = self.headers['Tulip-Filename']

        if "Content-Length" in self.headers:
            content_length = int(self.headers["Content-Length"])
            body = self.rfile.read(content_length)
            with open(path, "wb") as out_file:
                out_file.write(body)
        elif "chunked" in self.headers.get("Transfer-Encoding", ""):
            with open(path, "wb") as out_file:
                while True:
                    line = self.rfile.readline().strip()
                    chunk_length = int(line, 16)

                    if chunk_length != 0:
                        chunk = self.rfile.read(chunk_length)
                        out_file.write(chunk)

                    # Each chunk is followed by an additional empty newline
                    # that we have to consume.
                    self.rfile.readline()

                    # Finally, a chunk size of 0 is an end indication
                    if chunk_length == 0:
                        break


if __name__ == '__main__':
    server.test(HandlerClass=HTTPRequestHandler)
