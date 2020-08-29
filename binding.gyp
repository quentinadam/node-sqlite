{
    "targets": [{
        "target_name": "sqlite",
        "sources": [
          "./src/Database.cc",
          "./src/Statement.cc",
          "./src/main.cc",
          "./src/sqlite3.c",
        ],
        'cflags!': [
          '-fno-exceptions'
        ],
        'cflags_cc!': [ '-fno-exceptions',   '-std=c++11',
          '-stdlib=libc++'
         ],
        'xcode_settings': {
          'GCC_ENABLE_CPP_EXCEPTIONS': 'YES',
          'OTHER_CFLAGS_CC': [
            "-std=c++11",
            "-stdlib=libc++"
          ],
        }
     }]
}