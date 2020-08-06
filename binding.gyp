{
  "targets": [
    {
      "target_name": "node-printer",
      'cflags!': [ '-fno-exceptions' ],
      'cflags_cc!': [ '-fno-exceptions' ],
      'msvs_settings': {
        'VCCLCompilerTool': { 'ExceptionHandling': 1 },
      },
      "sources": [
        'src/main.cpp',
        'src/printer.h',
      ],
      'include_dirs' : [ "<!@(node -p \"require('node-addon-api').include\")" ],
      'conditions': [
        ['OS=="win"', {
          'sources': [
            'src/printer_win.cpp',
          ],
          'msvs_disabled_warnings': [
            4267,  # conversion from 'size_t' to 'int', possible loss of data
            4530,  # C++ exception handler used, but unwind semantics are not enabled
            4506,  # no definition for inline function
          ],
        }],
        ['OS not in ["mac", "win"]', {
          'sources': [
            'src/printer_posix.cc',
          ],
          'cflags': [
            '<!(pkg-config --cflags libsecret-1)',
            '-Wno-missing-field-initializers',
            '-Wno-deprecated-declarations',
          ],
          'link_settings': {
            'ldflags': [
              '<!(pkg-config --libs-only-L --libs-only-other libsecret-1)',
            ],
            'libraries': [
              '<!(pkg-config --libs-only-l libsecret-1)',
            ],
          },
        }]
      ],
    }
  ]
}