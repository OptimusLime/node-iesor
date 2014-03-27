{
    "targets": [
        {
            "target_name": "json",
            "product_name": "json",
             "variables": {
            	"json_dir%": "../"
          	},
          	'cflags!': [ '-fno-exceptions' ],
            'cflags_cc!': [ '-fno-exceptions' ],
            'conditions': [
                ['OS=="mac"', {
                  'xcode_settings': {
                    'GCC_ENABLE_CPP_EXCEPTIONS': 'YES'
                  }
                }]
            ],
            "type": "static_library",
             "include_dirs": [
            	"<(json_dir)/",
            	"<(json_dir)/JSON",
                "<(json_dir)/JSON/src"
            ],
            "sources": [
                "<(json_dir)/JSON/json.h",
                "<(json_dir)/JSON/src/autolink.h",
                "<(json_dir)/JSON/src/config.h",
                "<(json_dir)/JSON/src/features.h",
                "<(json_dir)/JSON/src/forwards.h",
                "<(json_dir)/JSON/src/json_batchallocator.h",
                "<(json_dir)/JSON/src/json_tool.h",
                "<(json_dir)/JSON/src/reader.h",
                "<(json_dir)/JSON/src/value.h",
                "<(json_dir)/JSON/src/writer.h",

                "<(json_dir)/JSON/src/json_internalarray.inl",
                "<(json_dir)/JSON/src/json_internalmap.inl",
                "<(json_dir)/JSON/src/json_valueiterator.inl",
                "<(json_dir)/JSON/src/json_reader.cpp",
                "<(json_dir)/JSON/src/json_value.cpp",
                "<(json_dir)/JSON/src/json_writer.cpp"
            ],           
            "all_dependent_settings": {
            	"include_dirs": [ "<(json_dir)/JSON" ]
            }
        }
    ],
     "variables": {
        "conditions": [
            [
                "OS == 'mac'",
                {
                    "target_arch%": "x64"
                },
                {
                    "target_arch%": "ia32"
                }
            ]
        ]
    },
    "target_defaults": {
        "default_configuration": "Release",
        "defines": [

        ],
        "conditions": [
            [
                "OS == 'mac'",
                {
                    "defines": [
                        "DARWIN"
                    ]
                },
                {
                    "defines": [
                        "LINUX"
                    ]
                }
            ],
            [
                "OS == 'mac' and target_arch == 'x64'",
                {
                    "xcode_settings": {
                        "ARCHS": [
                            "x86_64"
                        ]
                    }
                }
            ]
        ],
        "configurations": {
            "Debug": {
                "cflags": [
                    "-g",
                    "-O0"
                ],
                "xcode_settings": {
                    "OTHER_CFLAGS": [
                        "-g",
                        "-O0"
                    ],
                    "OTHER_CPLUSPLUSFLAGS": [
                        "-g",
                        "-O0"
                    ]
                }
            },
            "Release": {
                "cflags": [
                    "-O3"
                ],
                "defines": [
                    "NDEBUG"
                ],
                "xcode_settings": {
                    "OTHER_CFLAGS": [
                        "-O3"
                    ],
                    "OTHER_CPLUSPLUSFLAGS": [
                        "-O3",
                        "-DNDEBUG"
                    ]
                }
            }
        }
    }
}