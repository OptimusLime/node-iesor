{
    "targets": [
        {
            "target_name": "iesor",
            "product_name": "iesor",
             "variables": {
            	"iesor_dir%": "../"
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
            'dependencies': [
                "<(iesor_dir)/JSON/json.gyp:json",
                "<(iesor_dir)/Box2D/box2D.gyp:box2D"
            ],
             "include_dirs": [
                "<(iesor_dir)/",
            	"<(iesor_dir)/IESoR",
                "<(iesor_dir)/IESoR/Construction/Bodies",
                "<(iesor_dir)/IESoR/Construction/GPU",
                "<(iesor_dir)/IESoR/Network",
                "<(iesor_dir)/IESoR/Simulation",
                "<(iesor_dir)/IESoR/Simulation/Initialize",
                "<(iesor_dir)/IESoR/Simulation/Record",
            ],
            "sources": [
                "<(iesor_dir)/IESoR/iesor.h",

                "<(iesor_dir)/IESoR/Construction/Bodies/outputsToBodies.h",
                "<(iesor_dir)/IESoR/Construction/GPU/gpuBodyBuilder.h",
                "<(iesor_dir)/IESoR/Network/network.h",
                "<(iesor_dir)/IESoR/Network/iesorBody.h",
                "<(iesor_dir)/IESoR/Simulation/iesorWorld.h",
                "<(iesor_dir)/IESoR/Simulation/iesorDirector.h",
                "<(iesor_dir)/IESoR/Simulation/Initialize/initializeWorld.h",
                "<(iesor_dir)/IESoR/Simulation/Record/recordWorld.h",


                "<(iesor_dir)/IESoR/Construction/Bodies/outputsToBodies.cpp",
                "<(iesor_dir)/IESoR/Construction/GPU/gpuBodyBuilder.cpp",
                "<(iesor_dir)/IESoR/Network/network.cpp",
                "<(iesor_dir)/IESoR/Network/iesorBody.cpp",
                "<(iesor_dir)/IESoR/Simulation/iesorWorld.cpp",
                "<(iesor_dir)/IESoR/Simulation/iesorDirector.cpp",
                "<(iesor_dir)/IESoR/Simulation/Initialize/initializeWorld.cpp",
                "<(iesor_dir)/IESoR/Simulation/Record/recordWorld.cpp"
            ],           
            "all_dependent_settings": {
            	"include_dirs": [ 
                "<(iesor_dir)/",
                "<(iesor_dir)/IESoR" 
                ]
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