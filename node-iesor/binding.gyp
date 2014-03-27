{
  "targets": [
    {
      "target_name": "nodeiesor",
      "sources": [ 
	      "iesor-world.cpp",
	      "src/worldWrapper.h", 
	      "src/worldWrapper.cpp" 
      ],
      "dependencies" : [
      	"../IESoR/iesor.gyp:iesor"
      ],
     'cflags!': [ '-fno-exceptions' ],
	 'cflags_cc!': [ '-fno-exceptions' ],
      
      'conditions': [
        ['OS=="mac"', {
          'xcode_settings': {
            'GCC_ENABLE_CPP_EXCEPTIONS': 'YES'
          }
        }]
      ]
    }
  ]
}
