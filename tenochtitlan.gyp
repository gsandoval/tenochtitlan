{
  'targets': [
    {
      'target_name': 'tenochtitlan',
      'type': 'executable',
      'msvs_guid': '356EC6A7-5070-4446-AA95-4FDF5B2395A1',
      'dependencies': [
      ],
      'include_dirs': [
        'include',
        'lib/libuv/include'
      ],
      'sources': [
        'src/management/logger.cpp',
        'src/management/applicationlifecyclelistener.cpp',
        'src/socket/buffer.cpp',
        'src/socket/socketexception.cpp',
        'src/socket/tcpsocket.cpp',
        'src/socket/servertcpsocket.cpp',
        'src/socket/clienttcpsocket.cpp',
        'src/socket/tcpclientconnection.cpp',
        'src/server.cpp',
        'src/server/socketserver.cpp',
        'src/server/socketserverworker.cpp',
        'src/http/httpsocketserverworker.cpp',
        'src/server/rawsocketserverworker.cpp',
        'src/server/socketserverthread.cpp',
        'src/parser/jsonparser.cpp',
        'src/parser/httpparser.cpp',
        'src/http/httpserver.cpp',
        'src/http/httprequestprocessor.cpp',
        'src/http/httpentity.cpp',
        'src/http/httpsocketserverworkercreator.cpp',
        'src/http/component/httpcomponent.cpp',
        'src/http/component/httpcomponentfactory.cpp',
        'src/http/component/httpcontext.cpp',
        'src/http/component/restcomponent.cpp',
        'src/http/component/staticresourcecomponent.cpp',
        'src/util/properties.cpp',
        'src/http/component/inputvalidationcomponent.cpp',
        'src/http/content/httpcontent.cpp',
        'src/http/content/httpfilecontent.cpp',
        'src/http/content/httpstringcontent.cpp',
        'src/util/filereadercache.cpp',
        'src/http/component/rest/controller.cpp',
        'src/http/component/rest/contactcontroller.cpp',
        'src/http/component/rest/homecontroller.cpp'
      ],
      'conditions': [
        [
          'OS=="linux"', {
          }
        ],
        [
          'OS=="win"', {
            'libraries': [
              '-llib/libuv/Release/lib/libuv'
            ],
            "configurations": {
              "Debug": {},
              "Release": {}
            }
          }, {
          # OS != "win",
            'xcode_settings': {
              'OTHER_CPLUSPLUSFLAGS': ['-std=c++11','-stdlib=libc++'],
              'OTHER_LDFLAGS': ['-stdlib=libc++']
            },
            'libraries': [
              'lib/libuv/build/Debug/libuv.a'
            ],
            "configurations": {
              "Debug": {},
              "Release": {}
            }
          }
        ]
      ]
    }
  ]
}



