import os
import socket
import logging
import subprocess
import traceback

from cffi import FFI

ffi = FFI()

THIS_FILE_PATH = os.path.abspath(os.path.dirname(__file__))

header = os.path.join(THIS_FILE_PATH, "../DVB-S2/dvbs2_c.h")
types_header = os.path.join(THIS_FILE_PATH, "../DVB-S2/dvb_types.h")

preprocessed_headers = subprocess.Popen(['cc', '-E', '-DDVBS2_NOINCLUDE',
                                         '-DDVBS2_PY',
                                         header,
                                         types_header,
                                         ],
                                        stdout=subprocess.PIPE).communicate()[0]
preprocessed_headers.decode("utf-8")
ffi.cdef(preprocessed_headers.decode("utf-8"))

lib = ffi.dlopen(os.path.join(THIS_FILE_PATH, '../build/Debug/', 'libdvbs2.so'))
