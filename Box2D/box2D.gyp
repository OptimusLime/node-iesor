{
    "targets": [
       {
        'target_name': 'box2D',
        "product_name": "box2D",

          "variables": {
            "box2d_dir%": "../"
          },
          'type': 'static_library',
          'include_dirs': [
            '<(box2d_dir)/',
            '<(box2d_dir)/Box2D',
            '<(box2d_dir)/Box2D/Dynamics/Contacts',
            '<(box2d_dir)/Box2D/Dynamics/Joints',
            '<(box2d_dir)/Box2D/Rope',
            '<(box2d_dir)/Box2D/Common',
            '<(box2d_dir)/Box2D/Collision',
            '<(box2d_dir)/Box2D/Dynamics',
            '<(box2d_dir)/Box2D/Collision/Shapes',
          ],
          'sources': [
            '<(box2d_dir)/Box2D/Box2D.h',
            '<(box2d_dir)/Box2D/Collision/Shapes/b2ChainShape.cpp',
            '<(box2d_dir)/Box2D/Collision/Shapes/b2ChainShape.h',
            '<(box2d_dir)/Box2D/Collision/Shapes/b2CircleShape.cpp',
            '<(box2d_dir)/Box2D/Collision/Shapes/b2CircleShape.h',
            '<(box2d_dir)/Box2D/Collision/Shapes/b2EdgeShape.cpp',
            '<(box2d_dir)/Box2D/Collision/Shapes/b2EdgeShape.h',
            '<(box2d_dir)/Box2D/Collision/Shapes/b2PolygonShape.cpp',
            '<(box2d_dir)/Box2D/Collision/Shapes/b2PolygonShape.h',
            '<(box2d_dir)/Box2D/Collision/Shapes/b2Shape.h',
            '<(box2d_dir)/Box2D/Collision/b2BroadPhase.cpp',
            '<(box2d_dir)/Box2D/Collision/b2BroadPhase.h',
            '<(box2d_dir)/Box2D/Collision/b2CollideCircle.cpp',
            '<(box2d_dir)/Box2D/Collision/b2CollideEdge.cpp',
            '<(box2d_dir)/Box2D/Collision/b2CollidePolygon.cpp',
            '<(box2d_dir)/Box2D/Collision/b2Collision.cpp',
            '<(box2d_dir)/Box2D/Collision/b2Collision.h',
            '<(box2d_dir)/Box2D/Collision/b2Distance.cpp',
            '<(box2d_dir)/Box2D/Collision/b2Distance.h',
            '<(box2d_dir)/Box2D/Collision/b2DynamicTree.cpp',
            '<(box2d_dir)/Box2D/Collision/b2DynamicTree.h',
            '<(box2d_dir)/Box2D/Collision/b2TimeOfImpact.cpp',
            '<(box2d_dir)/Box2D/Collision/b2TimeOfImpact.h',
            '<(box2d_dir)/Box2D/Common/b2BlockAllocator.cpp',
            '<(box2d_dir)/Box2D/Common/b2BlockAllocator.h',
            '<(box2d_dir)/Box2D/Common/b2Draw.cpp',
            '<(box2d_dir)/Box2D/Common/b2Draw.h',
            '<(box2d_dir)/Box2D/Common/b2GrowableStack.h',
            '<(box2d_dir)/Box2D/Common/b2Math.cpp',
            '<(box2d_dir)/Box2D/Common/b2Math.h',
            '<(box2d_dir)/Box2D/Common/b2Settings.cpp',
            '<(box2d_dir)/Box2D/Common/b2Settings.h',
            '<(box2d_dir)/Box2D/Common/b2StackAllocator.cpp',
            '<(box2d_dir)/Box2D/Common/b2StackAllocator.h',
            '<(box2d_dir)/Box2D/Common/b2Timer.cpp',
            '<(box2d_dir)/Box2D/Common/b2Timer.h',
            '<(box2d_dir)/Box2D/Dynamics/Contacts/b2ChainAndCircleContact.cpp',
            '<(box2d_dir)/Box2D/Dynamics/Contacts/b2ChainAndCircleContact.h',
            '<(box2d_dir)/Box2D/Dynamics/Contacts/b2ChainAndPolygonContact.cpp',
            '<(box2d_dir)/Box2D/Dynamics/Contacts/b2ChainAndPolygonContact.h',
            '<(box2d_dir)/Box2D/Dynamics/Contacts/b2CircleContact.cpp',
            '<(box2d_dir)/Box2D/Dynamics/Contacts/b2CircleContact.h',
            '<(box2d_dir)/Box2D/Dynamics/Contacts/b2Contact.cpp',
            '<(box2d_dir)/Box2D/Dynamics/Contacts/b2Contact.h',
            '<(box2d_dir)/Box2D/Dynamics/Contacts/b2ContactSolver.cpp',
            '<(box2d_dir)/Box2D/Dynamics/Contacts/b2ContactSolver.h',
            '<(box2d_dir)/Box2D/Dynamics/Contacts/b2EdgeAndCircleContact.cpp',
            '<(box2d_dir)/Box2D/Dynamics/Contacts/b2EdgeAndCircleContact.h',
            '<(box2d_dir)/Box2D/Dynamics/Contacts/b2EdgeAndPolygonContact.cpp',
            '<(box2d_dir)/Box2D/Dynamics/Contacts/b2EdgeAndPolygonContact.h',
            '<(box2d_dir)/Box2D/Dynamics/Contacts/b2PolygonAndCircleContact.cpp',
            '<(box2d_dir)/Box2D/Dynamics/Contacts/b2PolygonAndCircleContact.h',
            '<(box2d_dir)/Box2D/Dynamics/Contacts/b2PolygonContact.cpp',
            '<(box2d_dir)/Box2D/Dynamics/Contacts/b2PolygonContact.h',
            '<(box2d_dir)/Box2D/Dynamics/Joints/b2DistanceJoint.cpp',
            '<(box2d_dir)/Box2D/Dynamics/Joints/b2DistanceJoint.h',
            '<(box2d_dir)/Box2D/Dynamics/Joints/b2FrictionJoint.cpp',
            '<(box2d_dir)/Box2D/Dynamics/Joints/b2FrictionJoint.h',
            '<(box2d_dir)/Box2D/Dynamics/Joints/b2GearJoint.cpp',
            '<(box2d_dir)/Box2D/Dynamics/Joints/b2GearJoint.h',
            '<(box2d_dir)/Box2D/Dynamics/Joints/b2Joint.cpp',
            '<(box2d_dir)/Box2D/Dynamics/Joints/b2Joint.h',
            '<(box2d_dir)/Box2D/Dynamics/Joints/b2MouseJoint.cpp',
            '<(box2d_dir)/Box2D/Dynamics/Joints/b2MouseJoint.h',
            '<(box2d_dir)/Box2D/Dynamics/Joints/b2PrismaticJoint.cpp',
            '<(box2d_dir)/Box2D/Dynamics/Joints/b2PrismaticJoint.h',
            '<(box2d_dir)/Box2D/Dynamics/Joints/b2PulleyJoint.cpp',
            '<(box2d_dir)/Box2D/Dynamics/Joints/b2PulleyJoint.h',
            '<(box2d_dir)/Box2D/Dynamics/Joints/b2RevoluteJoint.cpp',
            '<(box2d_dir)/Box2D/Dynamics/Joints/b2RevoluteJoint.h',
            '<(box2d_dir)/Box2D/Dynamics/Joints/b2RopeJoint.cpp',
            '<(box2d_dir)/Box2D/Dynamics/Joints/b2RopeJoint.h',
            '<(box2d_dir)/Box2D/Dynamics/Joints/b2WeldJoint.cpp',
            '<(box2d_dir)/Box2D/Dynamics/Joints/b2WeldJoint.h',
            '<(box2d_dir)/Box2D/Dynamics/Joints/b2WheelJoint.cpp',
            '<(box2d_dir)/Box2D/Dynamics/Joints/b2WheelJoint.h',
            '<(box2d_dir)/Box2D/Dynamics/b2Body.cpp',
            '<(box2d_dir)/Box2D/Dynamics/b2Body.h',
            '<(box2d_dir)/Box2D/Dynamics/b2ContactManager.cpp',
            '<(box2d_dir)/Box2D/Dynamics/b2ContactManager.h',
            '<(box2d_dir)/Box2D/Dynamics/b2Fixture.cpp',
            '<(box2d_dir)/Box2D/Dynamics/b2Fixture.h',
            '<(box2d_dir)/Box2D/Dynamics/b2Island.cpp',
            '<(box2d_dir)/Box2D/Dynamics/b2Island.h',
            '<(box2d_dir)/Box2D/Dynamics/b2TimeStep.h',
            '<(box2d_dir)/Box2D/Dynamics/b2World.cpp',
            '<(box2d_dir)/Box2D/Dynamics/b2World.h',
            '<(box2d_dir)/Box2D/Dynamics/b2WorldCallbacks.cpp',
            '<(box2d_dir)/Box2D/Dynamics/b2WorldCallbacks.h',
            '<(box2d_dir)/Box2D/Rope/b2Rope.cpp',
            '<(box2d_dir)/Box2D/Rope/b2Rope.h',
          ],
          'all_dependent_settings': {
                'include_dirs': [
                    "<(box2d_dir)/Box2D"
                ],  # dependents need to find Box2D.h.
            },
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