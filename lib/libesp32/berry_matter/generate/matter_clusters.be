# Matter clusters
#
# Needs to be reword, this structure is huge...
#
# imported from Python definition


var _Matter_clusters = 
{
    'Identify': {
        'clusterName': 'Identify',
        'clusterId': 3,
        'commands': {
            0: {
                'commandId': 0,
                'commandName': 'Identify',
                'args': {
                    'identifyTime': 'int'
                }
            },
            64: {
                'commandId': 64,
                'commandName': 'TriggerEffect',
                'args': {
                    'effectIdentifier': 'int',
                    'effectVariant': 'int'
                }
            }
        },
        'attributes': {
            0: {
                'attributeName': 'IdentifyTime',
                'attributeId': 0,
                'type': 'int',
                'reportable': true,
                'writable': true
            },
            1: {
                'attributeName': 'IdentifyType',
                'attributeId': 1,
                'type': 'int',
                'reportable': true
            },
            65528: {
                'attributeName': 'GeneratedCommandList',
                'attributeId': 65528,
                'type': 'int',
                'reportable': true
            },
            65529: {
                'attributeName': 'AcceptedCommandList',
                'attributeId': 65529,
                'type': 'int',
                'reportable': true
            },
            65531: {
                'attributeName': 'AttributeList',
                'attributeId': 65531,
                'type': 'int',
                'reportable': true
            },
            65532: {
                'attributeName': 'FeatureMap',
                'attributeId': 65532,
                'type': 'int',
                'reportable': true
            },
            65533: {
                'attributeName': 'ClusterRevision',
                'attributeId': 65533,
                'type': 'int',
                'reportable': true
            }
        }
    },
    'Groups': {
        'clusterName': 'Groups',
        'clusterId': 4,
        'commands': {
            0: {
                'commandId': 0,
                'commandName': 'AddGroup',
                'args': {
                    'groupId': 'int',
                    'groupName': 'str'
                }
            },
            1: {
                'commandId': 1,
                'commandName': 'ViewGroup',
                'args': {
                    'groupId': 'int'
                }
            },
            2: {
                'commandId': 2,
                'commandName': 'GetGroupMembership',
                'args': {
                    'groupList': 'int'
                }
            },
            3: {
                'commandId': 3,
                'commandName': 'RemoveGroup',
                'args': {
                    'groupId': 'int'
                }
            },
            4: {
                'commandId': 4,
                'commandName': 'RemoveAllGroups',
                'args': {}
            },
            5: {
                'commandId': 5,
                'commandName': 'AddGroupIfIdentifying',
                'args': {
                    'groupId': 'int',
                    'groupName': 'str'
                }
            }
        },
        'attributes': {
            0: {
                'attributeName': 'NameSupport',
                'attributeId': 0,
                'type': 'int',
                'reportable': true
            },
            65528: {
                'attributeName': 'GeneratedCommandList',
                'attributeId': 65528,
                'type': 'int',
                'reportable': true
            },
            65529: {
                'attributeName': 'AcceptedCommandList',
                'attributeId': 65529,
                'type': 'int',
                'reportable': true
            },
            65531: {
                'attributeName': 'AttributeList',
                'attributeId': 65531,
                'type': 'int',
                'reportable': true
            },
            65532: {
                'attributeName': 'FeatureMap',
                'attributeId': 65532,
                'type': 'int',
                'reportable': true
            },
            65533: {
                'attributeName': 'ClusterRevision',
                'attributeId': 65533,
                'type': 'int',
                'reportable': true
            }
        }
    },
    'Scenes': {
        'clusterName': 'Scenes',
        'clusterId': 5,
        'commands': {
            0: {
                'commandId': 0,
                'commandName': 'AddScene',
                'args': {
                    'groupId': 'int',
                    'sceneId': 'int',
                    'transitionTime': 'int',
                    'sceneName': 'str',
                    'clusterId': 'int',
                    'attributeValueList': ''
                }
            },
            1: {
                'commandId': 1,
                'commandName': 'ViewScene',
                'args': {
                    'groupId': 'int',
                    'sceneId': 'int'
                }
            },
            2: {
                'commandId': 2,
                'commandName': 'RemoveScene',
                'args': {
                    'groupId': 'int',
                    'sceneId': 'int'
                }
            },
            3: {
                'commandId': 3,
                'commandName': 'RemoveAllScenes',
                'args': {
                    'groupId': 'int'
                }
            },
            4: {
                'commandId': 4,
                'commandName': 'StoreScene',
                'args': {
                    'groupId': 'int',
                    'sceneId': 'int'
                }
            },
            5: {
                'commandId': 5,
                'commandName': 'RecallScene',
                'args': {
                    'groupId': 'int',
                    'sceneId': 'int',
                    'transitionTime': 'int'
                }
            },
            6: {
                'commandId': 6,
                'commandName': 'GetSceneMembership',
                'args': {
                    'groupId': 'int'
                }
            }
        },
        'attributes': {
            0: {
                'attributeName': 'SceneCount',
                'attributeId': 0,
                'type': 'int',
                'reportable': true
            },
            1: {
                'attributeName': 'CurrentScene',
                'attributeId': 1,
                'type': 'int',
                'reportable': true
            },
            2: {
                'attributeName': 'CurrentGroup',
                'attributeId': 2,
                'type': 'int',
                'reportable': true
            },
            3: {
                'attributeName': 'SceneValid',
                'attributeId': 3,
                'type': 'bool',
                'reportable': true
            },
            4: {
                'attributeName': 'NameSupport',
                'attributeId': 4,
                'type': 'int',
                'reportable': true
            },
            65528: {
                'attributeName': 'GeneratedCommandList',
                'attributeId': 65528,
                'type': 'int',
                'reportable': true
            },
            65529: {
                'attributeName': 'AcceptedCommandList',
                'attributeId': 65529,
                'type': 'int',
                'reportable': true
            },
            65531: {
                'attributeName': 'AttributeList',
                'attributeId': 65531,
                'type': 'int',
                'reportable': true
            },
            65532: {
                'attributeName': 'FeatureMap',
                'attributeId': 65532,
                'type': 'int',
                'reportable': true
            },
            65533: {
                'attributeName': 'ClusterRevision',
                'attributeId': 65533,
                'type': 'int',
                'reportable': true
            }
        }
    },
    'OnOff': {
        'clusterName': 'OnOff',
        'clusterId': 6,
        'commands': {
            0: {
                'commandId': 0,
                'commandName': 'Off',
                'args': {}
            },
            1: {
                'commandId': 1,
                'commandName': 'On',
                'args': {}
            },
            2: {
                'commandId': 2,
                'commandName': 'Toggle',
                'args': {}
            },
            64: {
                'commandId': 64,
                'commandName': 'OffWithEffect',
                'args': {
                    'effectId': 'int',
                    'effectVariant': 'int'
                }
            },
            65: {
                'commandId': 65,
                'commandName': 'OnWithRecallGlobalScene',
                'args': {}
            },
            66: {
                'commandId': 66,
                'commandName': 'OnWithTimedOff',
                'args': {
                    'onOffControl': 'int',
                    'onTime': 'int',
                    'offWaitTime': 'int'
                }
            }
        },
        'attributes': {
            0: {
                'attributeName': 'OnOff',
                'attributeId': 0,
                'type': 'bool',
                'reportable': true
            },
            16384: {
                'attributeName': 'GlobalSceneControl',
                'attributeId': 16384,
                'type': 'bool',
                'reportable': true
            },
            16385: {
                'attributeName': 'OnTime',
                'attributeId': 16385,
                'type': 'int',
                'reportable': true,
                'writable': true
            },
            16386: {
                'attributeName': 'OffWaitTime',
                'attributeId': 16386,
                'type': 'int',
                'reportable': true,
                'writable': true
            },
            16387: {
                'attributeName': 'StartUpOnOff',
                'attributeId': 16387,
                'type': 'int',
                'reportable': true,
                'writable': true
            },
            65528: {
                'attributeName': 'GeneratedCommandList',
                'attributeId': 65528,
                'type': 'int',
                'reportable': true
            },
            65529: {
                'attributeName': 'AcceptedCommandList',
                'attributeId': 65529,
                'type': 'int',
                'reportable': true
            },
            65531: {
                'attributeName': 'AttributeList',
                'attributeId': 65531,
                'type': 'int',
                'reportable': true
            },
            65532: {
                'attributeName': 'FeatureMap',
                'attributeId': 65532,
                'type': 'int',
                'reportable': true
            },
            65533: {
                'attributeName': 'ClusterRevision',
                'attributeId': 65533,
                'type': 'int',
                'reportable': true
            }
        }
    },
    'OnOffSwitchConfiguration': {
        'clusterName': 'OnOffSwitchConfiguration',
        'clusterId': 7,
        'commands': {},
        'attributes': {
            0: {
                'attributeName': 'SwitchType',
                'attributeId': 0,
                'type': 'int',
                'reportable': true
            },
            16: {
                'attributeName': 'SwitchActions',
                'attributeId': 16,
                'type': 'int',
                'reportable': true,
                'writable': true
            },
            65528: {
                'attributeName': 'GeneratedCommandList',
                'attributeId': 65528,
                'type': 'int',
                'reportable': true
            },
            65529: {
                'attributeName': 'AcceptedCommandList',
                'attributeId': 65529,
                'type': 'int',
                'reportable': true
            },
            65531: {
                'attributeName': 'AttributeList',
                'attributeId': 65531,
                'type': 'int',
                'reportable': true
            },
            65532: {
                'attributeName': 'FeatureMap',
                'attributeId': 65532,
                'type': 'int',
                'reportable': true
            },
            65533: {
                'attributeName': 'ClusterRevision',
                'attributeId': 65533,
                'type': 'int',
                'reportable': true
            }
        }
    },
    'LevelControl': {
        'clusterName': 'LevelControl',
        'clusterId': 8,
        'commands': {
            0: {
                'commandId': 0,
                'commandName': 'MoveToLevel',
                'args': {
                    'level': 'int',
                    'transitionTime': 'int',
                    'optionsMask': 'int',
                    'optionsOverride': 'int'
                }
            },
            1: {
                'commandId': 1,
                'commandName': 'Move',
                'args': {
                    'moveMode': 'int',
                    'rate': 'int',
                    'optionsMask': 'int',
                    'optionsOverride': 'int'
                }
            },
            2: {
                'commandId': 2,
                'commandName': 'Step',
                'args': {
                    'stepMode': 'int',
                    'stepSize': 'int',
                    'transitionTime': 'int',
                    'optionsMask': 'int',
                    'optionsOverride': 'int'
                }
            },
            3: {
                'commandId': 3,
                'commandName': 'Stop',
                'args': {
                    'optionsMask': 'int',
                    'optionsOverride': 'int'
                }
            },
            4: {
                'commandId': 4,
                'commandName': 'MoveToLevelWithOnOff',
                'args': {
                    'level': 'int',
                    'transitionTime': 'int',
                    'optionsMask': 'int',
                    'optionsOverride': 'int'
                }
            },
            5: {
                'commandId': 5,
                'commandName': 'MoveWithOnOff',
                'args': {
                    'moveMode': 'int',
                    'rate': 'int',
                    'optionsMask': 'int',
                    'optionsOverride': 'int'
                }
            },
            6: {
                'commandId': 6,
                'commandName': 'StepWithOnOff',
                'args': {
                    'stepMode': 'int',
                    'stepSize': 'int',
                    'transitionTime': 'int',
                    'optionsMask': 'int',
                    'optionsOverride': 'int'
                }
            },
            7: {
                'commandId': 7,
                'commandName': 'StopWithOnOff',
                'args': {
                    'optionsMask': 'int',
                    'optionsOverride': 'int'
                }
            }
        },
        'attributes': {
            0: {
                'attributeName': 'CurrentLevel',
                'attributeId': 0,
                'type': 'int',
                'reportable': true
            },
            1: {
                'attributeName': 'RemainingTime',
                'attributeId': 1,
                'type': 'int',
                'reportable': true
            },
            2: {
                'attributeName': 'MinLevel',
                'attributeId': 2,
                'type': 'int',
                'reportable': true
            },
            3: {
                'attributeName': 'MaxLevel',
                'attributeId': 3,
                'type': 'int',
                'reportable': true
            },
            4: {
                'attributeName': 'CurrentFrequency',
                'attributeId': 4,
                'type': 'int',
                'reportable': true
            },
            5: {
                'attributeName': 'MinFrequency',
                'attributeId': 5,
                'type': 'int',
                'reportable': true
            },
            6: {
                'attributeName': 'MaxFrequency',
                'attributeId': 6,
                'type': 'int',
                'reportable': true
            },
            15: {
                'attributeName': 'Options',
                'attributeId': 15,
                'type': 'int',
                'reportable': true,
                'writable': true
            },
            16: {
                'attributeName': 'OnOffTransitionTime',
                'attributeId': 16,
                'type': 'int',
                'reportable': true,
                'writable': true
            },
            17: {
                'attributeName': 'OnLevel',
                'attributeId': 17,
                'type': 'int',
                'reportable': true,
                'writable': true
            },
            18: {
                'attributeName': 'OnTransitionTime',
                'attributeId': 18,
                'type': 'int',
                'reportable': true,
                'writable': true
            },
            19: {
                'attributeName': 'OffTransitionTime',
                'attributeId': 19,
                'type': 'int',
                'reportable': true,
                'writable': true
            },
            20: {
                'attributeName': 'DefaultMoveRate',
                'attributeId': 20,
                'type': 'int',
                'reportable': true,
                'writable': true
            },
            16384: {
                'attributeName': 'StartUpCurrentLevel',
                'attributeId': 16384,
                'type': 'int',
                'reportable': true,
                'writable': true
            },
            65528: {
                'attributeName': 'GeneratedCommandList',
                'attributeId': 65528,
                'type': 'int',
                'reportable': true
            },
            65529: {
                'attributeName': 'AcceptedCommandList',
                'attributeId': 65529,
                'type': 'int',
                'reportable': true
            },
            65531: {
                'attributeName': 'AttributeList',
                'attributeId': 65531,
                'type': 'int',
                'reportable': true
            },
            65532: {
                'attributeName': 'FeatureMap',
                'attributeId': 65532,
                'type': 'int',
                'reportable': true
            },
            65533: {
                'attributeName': 'ClusterRevision',
                'attributeId': 65533,
                'type': 'int',
                'reportable': true
            }
        }
    },
    'BinaryInputBasic': {
        'clusterName': 'BinaryInputBasic',
        'clusterId': 15,
        'commands': {},
        'attributes': {
            81: {
                'attributeName': 'OutOfService',
                'attributeId': 81,
                'type': 'bool',
                'reportable': true,
                'writable': true
            },
            85: {
                'attributeName': 'PresentValue',
                'attributeId': 85,
                'type': 'bool',
                'reportable': true,
                'writable': true
            },
            111: {
                'attributeName': 'StatusFlags',
                'attributeId': 111,
                'type': 'int',
                'reportable': true
            },
            65528: {
                'attributeName': 'GeneratedCommandList',
                'attributeId': 65528,
                'type': 'int',
                'reportable': true
            },
            65529: {
                'attributeName': 'AcceptedCommandList',
                'attributeId': 65529,
                'type': 'int',
                'reportable': true
            },
            65531: {
                'attributeName': 'AttributeList',
                'attributeId': 65531,
                'type': 'int',
                'reportable': true
            },
            65532: {
                'attributeName': 'FeatureMap',
                'attributeId': 65532,
                'type': 'int',
                'reportable': true
            },
            65533: {
                'attributeName': 'ClusterRevision',
                'attributeId': 65533,
                'type': 'int',
                'reportable': true
            }
        }
    },
    'Descriptor': {
        'clusterName': 'Descriptor',
        'clusterId': 29,
        'commands': {},
        'attributes': {
            0: {
                'attributeName': 'DeviceTypeList',
                'attributeId': 0,
                'type': '',
                'reportable': true
            },
            1: {
                'attributeName': 'ServerList',
                'attributeId': 1,
                'type': 'int',
                'reportable': true
            },
            2: {
                'attributeName': 'ClientList',
                'attributeId': 2,
                'type': 'int',
                'reportable': true
            },
            3: {
                'attributeName': 'PartsList',
                'attributeId': 3,
                'type': 'int',
                'reportable': true
            },
            65528: {
                'attributeName': 'GeneratedCommandList',
                'attributeId': 65528,
                'type': 'int',
                'reportable': true
            },
            65529: {
                'attributeName': 'AcceptedCommandList',
                'attributeId': 65529,
                'type': 'int',
                'reportable': true
            },
            65531: {
                'attributeName': 'AttributeList',
                'attributeId': 65531,
                'type': 'int',
                'reportable': true
            },
            65532: {
                'attributeName': 'FeatureMap',
                'attributeId': 65532,
                'type': 'int',
                'reportable': true
            },
            65533: {
                'attributeName': 'ClusterRevision',
                'attributeId': 65533,
                'type': 'int',
                'reportable': true
            }
        }
    },
    'Binding': {
        'clusterName': 'Binding',
        'clusterId': 30,
        'commands': {},
        'attributes': {
            0: {
                'attributeName': 'Binding',
                'attributeId': 0,
                'type': '',
                'reportable': true,
                'writable': true
            },
            65528: {
                'attributeName': 'GeneratedCommandList',
                'attributeId': 65528,
                'type': 'int',
                'reportable': true
            },
            65529: {
                'attributeName': 'AcceptedCommandList',
                'attributeId': 65529,
                'type': 'int',
                'reportable': true
            },
            65531: {
                'attributeName': 'AttributeList',
                'attributeId': 65531,
                'type': 'int',
                'reportable': true
            },
            65532: {
                'attributeName': 'FeatureMap',
                'attributeId': 65532,
                'type': 'int',
                'reportable': true
            },
            65533: {
                'attributeName': 'ClusterRevision',
                'attributeId': 65533,
                'type': 'int',
                'reportable': true
            }
        }
    },
    'AccessControl': {
        'clusterName': 'AccessControl',
        'clusterId': 31,
        'commands': {},
        'attributes': {
            0: {
                'attributeName': 'Acl',
                'attributeId': 0,
                'type': '',
                'reportable': true,
                'writable': true
            },
            1: {
                'attributeName': 'Extension',
                'attributeId': 1,
                'type': '',
                'reportable': true,
                'writable': true
            },
            2: {
                'attributeName': 'SubjectsPerAccessControlEntry',
                'attributeId': 2,
                'type': 'int',
                'reportable': true
            },
            3: {
                'attributeName': 'TargetsPerAccessControlEntry',
                'attributeId': 3,
                'type': 'int',
                'reportable': true
            },
            4: {
                'attributeName': 'AccessControlEntriesPerFabric',
                'attributeId': 4,
                'type': 'int',
                'reportable': true
            },
            65528: {
                'attributeName': 'GeneratedCommandList',
                'attributeId': 65528,
                'type': 'int',
                'reportable': true
            },
            65529: {
                'attributeName': 'AcceptedCommandList',
                'attributeId': 65529,
                'type': 'int',
                'reportable': true
            },
            65531: {
                'attributeName': 'AttributeList',
                'attributeId': 65531,
                'type': 'int',
                'reportable': true
            },
            65532: {
                'attributeName': 'FeatureMap',
                'attributeId': 65532,
                'type': 'int',
                'reportable': true
            },
            65533: {
                'attributeName': 'ClusterRevision',
                'attributeId': 65533,
                'type': 'int',
                'reportable': true
            }
        }
    },
    'Actions': {
        'clusterName': 'Actions',
        'clusterId': 37,
        'commands': {
            0: {
                'commandId': 0,
                'commandName': 'InstantAction',
                'args': {
                    'actionID': 'int',
                    'invokeID': 'int'
                }
            },
            1: {
                'commandId': 1,
                'commandName': 'InstantActionWithTransition',
                'args': {
                    'actionID': 'int',
                    'invokeID': 'int',
                    'transitionTime': 'int'
                }
            },
            2: {
                'commandId': 2,
                'commandName': 'StartAction',
                'args': {
                    'actionID': 'int',
                    'invokeID': 'int'
                }
            },
            3: {
                'commandId': 3,
                'commandName': 'StartActionWithDuration',
                'args': {
                    'actionID': 'int',
                    'invokeID': 'int',
                    'duration': 'int'
                }
            },
            4: {
                'commandId': 4,
                'commandName': 'StopAction',
                'args': {
                    'actionID': 'int',
                    'invokeID': 'int'
                }
            },
            5: {
                'commandId': 5,
                'commandName': 'PauseAction',
                'args': {
                    'actionID': 'int',
                    'invokeID': 'int'
                }
            },
            6: {
                'commandId': 6,
                'commandName': 'PauseActionWithDuration',
                'args': {
                    'actionID': 'int',
                    'invokeID': 'int',
                    'duration': 'int'
                }
            },
            7: {
                'commandId': 7,
                'commandName': 'ResumeAction',
                'args': {
                    'actionID': 'int',
                    'invokeID': 'int'
                }
            },
            8: {
                'commandId': 8,
                'commandName': 'EnableAction',
                'args': {
                    'actionID': 'int',
                    'invokeID': 'int'
                }
            },
            9: {
                'commandId': 9,
                'commandName': 'EnableActionWithDuration',
                'args': {
                    'actionID': 'int',
                    'invokeID': 'int',
                    'duration': 'int'
                }
            },
            10: {
                'commandId': 10,
                'commandName': 'DisableAction',
                'args': {
                    'actionID': 'int',
                    'invokeID': 'int'
                }
            },
            11: {
                'commandId': 11,
                'commandName': 'DisableActionWithDuration',
                'args': {
                    'actionID': 'int',
                    'invokeID': 'int',
                    'duration': 'int'
                }
            }
        },
        'attributes': {
            0: {
                'attributeName': 'ActionList',
                'attributeId': 0,
                'type': '',
                'reportable': true
            },
            1: {
                'attributeName': 'EndpointLists',
                'attributeId': 1,
                'type': '',
                'reportable': true
            },
            2: {
                'attributeName': 'SetupURL',
                'attributeId': 2,
                'type': 'str',
                'reportable': true
            },
            65528: {
                'attributeName': 'GeneratedCommandList',
                'attributeId': 65528,
                'type': 'int',
                'reportable': true
            },
            65529: {
                'attributeName': 'AcceptedCommandList',
                'attributeId': 65529,
                'type': 'int',
                'reportable': true
            },
            65531: {
                'attributeName': 'AttributeList',
                'attributeId': 65531,
                'type': 'int',
                'reportable': true
            },
            65532: {
                'attributeName': 'FeatureMap',
                'attributeId': 65532,
                'type': 'int',
                'reportable': true
            },
            65533: {
                'attributeName': 'ClusterRevision',
                'attributeId': 65533,
                'type': 'int',
                'reportable': true
            }
        }
    },
    'Basic': {
        'clusterName': 'Basic',
        'clusterId': 40,
        'commands': {},
        'attributes': {
            0: {
                'attributeName': 'DataModelRevision',
                'attributeId': 0,
                'type': 'int',
                'reportable': true
            },
            1: {
                'attributeName': 'VendorName',
                'attributeId': 1,
                'type': 'str',
                'reportable': true
            },
            2: {
                'attributeName': 'VendorID',
                'attributeId': 2,
                'type': 'int',
                'reportable': true
            },
            3: {
                'attributeName': 'ProductName',
                'attributeId': 3,
                'type': 'str',
                'reportable': true
            },
            4: {
                'attributeName': 'ProductID',
                'attributeId': 4,
                'type': 'int',
                'reportable': true
            },
            5: {
                'attributeName': 'NodeLabel',
                'attributeId': 5,
                'type': 'str',
                'reportable': true,
                'writable': true
            },
            6: {
                'attributeName': 'Location',
                'attributeId': 6,
                'type': 'str',
                'reportable': true,
                'writable': true
            },
            7: {
                'attributeName': 'HardwareVersion',
                'attributeId': 7,
                'type': 'int',
                'reportable': true
            },
            8: {
                'attributeName': 'HardwareVersionString',
                'attributeId': 8,
                'type': 'str',
                'reportable': true
            },
            9: {
                'attributeName': 'SoftwareVersion',
                'attributeId': 9,
                'type': 'int',
                'reportable': true
            },
            10: {
                'attributeName': 'SoftwareVersionString',
                'attributeId': 10,
                'type': 'str',
                'reportable': true
            },
            11: {
                'attributeName': 'ManufacturingDate',
                'attributeId': 11,
                'type': 'str',
                'reportable': true
            },
            12: {
                'attributeName': 'PartNumber',
                'attributeId': 12,
                'type': 'str',
                'reportable': true
            },
            13: {
                'attributeName': 'ProductURL',
                'attributeId': 13,
                'type': 'str',
                'reportable': true
            },
            14: {
                'attributeName': 'ProductLabel',
                'attributeId': 14,
                'type': 'str',
                'reportable': true
            },
            15: {
                'attributeName': 'SerialNumber',
                'attributeId': 15,
                'type': 'str',
                'reportable': true
            },
            16: {
                'attributeName': 'LocalConfigDisabled',
                'attributeId': 16,
                'type': 'bool',
                'reportable': true,
                'writable': true
            },
            17: {
                'attributeName': 'Reachable',
                'attributeId': 17,
                'type': 'bool',
                'reportable': true
            },
            18: {
                'attributeName': 'UniqueID',
                'attributeId': 18,
                'type': 'str',
                'reportable': true
            },
            19: {
                'attributeName': 'CapabilityMinima',
                'attributeId': 19,
                'type': '',
                'reportable': true
            },
            65528: {
                'attributeName': 'GeneratedCommandList',
                'attributeId': 65528,
                'type': 'int',
                'reportable': true
            },
            65529: {
                'attributeName': 'AcceptedCommandList',
                'attributeId': 65529,
                'type': 'int',
                'reportable': true
            },
            65531: {
                'attributeName': 'AttributeList',
                'attributeId': 65531,
                'type': 'int',
                'reportable': true
            },
            65532: {
                'attributeName': 'FeatureMap',
                'attributeId': 65532,
                'type': 'int',
                'reportable': true
            },
            65533: {
                'attributeName': 'ClusterRevision',
                'attributeId': 65533,
                'type': 'int',
                'reportable': true
            }
        }
    },
    'OtaSoftwareUpdateProvider': {
        'clusterName': 'OtaSoftwareUpdateProvider',
        'clusterId': 41,
        'commands': {
            0: {
                'commandId': 0,
                'commandName': 'QueryImage',
                'args': {
                    'vendorId': 'int',
                    'productId': 'int',
                    'softwareVersion': 'int',
                    'protocolsSupported': 'int',
                    'hardwareVersion': 'int',
                    'location': 'str',
                    'requestorCanConsent': 'bool',
                    'metadataForProvider': 'bytes'
                }
            },
            2: {
                'commandId': 2,
                'commandName': 'ApplyUpdateRequest',
                'args': {
                    'updateToken': 'bytes',
                    'newVersion': 'int'
                }
            },
            4: {
                'commandId': 4,
                'commandName': 'NotifyUpdateApplied',
                'args': {
                    'updateToken': 'bytes',
                    'softwareVersion': 'int'
                }
            }
        },
        'attributes': {
            65531: {
                'attributeName': 'AttributeList',
                'attributeId': 65531,
                'type': 'int',
                'reportable': true
            },
            65532: {
                'attributeName': 'FeatureMap',
                'attributeId': 65532,
                'type': 'int',
                'reportable': true
            },
            65533: {
                'attributeName': 'ClusterRevision',
                'attributeId': 65533,
                'type': 'int',
                'reportable': true
            }
        }
    },
    'OtaSoftwareUpdateRequestor': {
        'clusterName': 'OtaSoftwareUpdateRequestor',
        'clusterId': 42,
        'commands': {
            0: {
                'commandId': 0,
                'commandName': 'AnnounceOtaProvider',
                'args': {
                    'providerNodeId': 'int',
                    'vendorId': 'int',
                    'announcementReason': 'int',
                    'metadataForNode': 'bytes',
                    'endpoint': 'int'
                }
            }
        },
        'attributes': {
            0: {
                'attributeName': 'DefaultOtaProviders',
                'attributeId': 0,
                'type': '',
                'reportable': true,
                'writable': true
            },
            1: {
                'attributeName': 'UpdatePossible',
                'attributeId': 1,
                'type': 'bool',
                'reportable': true
            },
            2: {
                'attributeName': 'UpdateState',
                'attributeId': 2,
                'type': 'int',
                'reportable': true
            },
            3: {
                'attributeName': 'UpdateStateProgress',
                'attributeId': 3,
                'type': 'int',
                'reportable': true
            },
            65531: {
                'attributeName': 'AttributeList',
                'attributeId': 65531,
                'type': 'int',
                'reportable': true
            },
            65532: {
                'attributeName': 'FeatureMap',
                'attributeId': 65532,
                'type': 'int',
                'reportable': true
            },
            65533: {
                'attributeName': 'ClusterRevision',
                'attributeId': 65533,
                'type': 'int',
                'reportable': true
            }
        }
    },
    'LocalizationConfiguration': {
        'clusterName': 'LocalizationConfiguration',
        'clusterId': 43,
        'commands': {},
        'attributes': {
            0: {
                'attributeName': 'ActiveLocale',
                'attributeId': 0,
                'type': 'str',
                'reportable': true,
                'writable': true
            },
            1: {
                'attributeName': 'SupportedLocales',
                'attributeId': 1,
                'type': 'str',
                'reportable': true
            },
            65528: {
                'attributeName': 'GeneratedCommandList',
                'attributeId': 65528,
                'type': 'int',
                'reportable': true
            },
            65529: {
                'attributeName': 'AcceptedCommandList',
                'attributeId': 65529,
                'type': 'int',
                'reportable': true
            },
            65532: {
                'attributeName': 'FeatureMap',
                'attributeId': 65532,
                'type': 'int',
                'reportable': true
            },
            65533: {
                'attributeName': 'ClusterRevision',
                'attributeId': 65533,
                'type': 'int',
                'reportable': true
            }
        }
    },
    'TimeFormatLocalization': {
        'clusterName': 'TimeFormatLocalization',
        'clusterId': 44,
        'commands': {},
        'attributes': {
            0: {
                'attributeName': 'HourFormat',
                'attributeId': 0,
                'type': 'int',
                'reportable': true,
                'writable': true
            },
            1: {
                'attributeName': 'ActiveCalendarType',
                'attributeId': 1,
                'type': 'int',
                'reportable': true,
                'writable': true
            },
            2: {
                'attributeName': 'SupportedCalendarTypes',
                'attributeId': 2,
                'type': 'int',
                'reportable': true
            },
            65528: {
                'attributeName': 'GeneratedCommandList',
                'attributeId': 65528,
                'type': 'int',
                'reportable': true
            },
            65529: {
                'attributeName': 'AcceptedCommandList',
                'attributeId': 65529,
                'type': 'int',
                'reportable': true
            },
            65532: {
                'attributeName': 'FeatureMap',
                'attributeId': 65532,
                'type': 'int',
                'reportable': true
            },
            65533: {
                'attributeName': 'ClusterRevision',
                'attributeId': 65533,
                'type': 'int',
                'reportable': true
            }
        }
    },
    'UnitLocalization': {
        'clusterName': 'UnitLocalization',
        'clusterId': 45,
        'commands': {},
        'attributes': {
            0: {
                'attributeName': 'TemperatureUnit',
                'attributeId': 0,
                'type': 'int',
                'reportable': true,
                'writable': true
            },
            65531: {
                'attributeName': 'AttributeList',
                'attributeId': 65531,
                'type': 'int',
                'reportable': true
            },
            65532: {
                'attributeName': 'FeatureMap',
                'attributeId': 65532,
                'type': 'int',
                'reportable': true
            },
            65533: {
                'attributeName': 'ClusterRevision',
                'attributeId': 65533,
                'type': 'int',
                'reportable': true
            }
        }
    },
    'PowerSourceConfiguration': {
        'clusterName': 'PowerSourceConfiguration',
        'clusterId': 46,
        'commands': {},
        'attributes': {
            0: {
                'attributeName': 'Sources',
                'attributeId': 0,
                'type': 'int',
                'reportable': true
            },
            65528: {
                'attributeName': 'GeneratedCommandList',
                'attributeId': 65528,
                'type': 'int',
                'reportable': true
            },
            65529: {
                'attributeName': 'AcceptedCommandList',
                'attributeId': 65529,
                'type': 'int',
                'reportable': true
            },
            65531: {
                'attributeName': 'AttributeList',
                'attributeId': 65531,
                'type': 'int',
                'reportable': true
            },
            65532: {
                'attributeName': 'FeatureMap',
                'attributeId': 65532,
                'type': 'int',
                'reportable': true
            },
            65533: {
                'attributeName': 'ClusterRevision',
                'attributeId': 65533,
                'type': 'int',
                'reportable': true
            }
        }
    },
    'PowerSource': {
        'clusterName': 'PowerSource',
        'clusterId': 47,
        'commands': {},
        'attributes': {
            0: {
                'attributeName': 'Status',
                'attributeId': 0,
                'type': 'int',
                'reportable': true
            },
            1: {
                'attributeName': 'Order',
                'attributeId': 1,
                'type': 'int',
                'reportable': true
            },
            2: {
                'attributeName': 'Description',
                'attributeId': 2,
                'type': 'str',
                'reportable': true
            },
            3: {
                'attributeName': 'WiredAssessedInputVoltage',
                'attributeId': 3,
                'type': 'int',
                'reportable': true
            },
            4: {
                'attributeName': 'WiredAssessedInputFrequency',
                'attributeId': 4,
                'type': 'int',
                'reportable': true
            },
            5: {
                'attributeName': 'WiredCurrentType',
                'attributeId': 5,
                'type': 'int',
                'reportable': true
            },
            6: {
                'attributeName': 'WiredAssessedCurrent',
                'attributeId': 6,
                'type': 'int',
                'reportable': true
            },
            7: {
                'attributeName': 'WiredNominalVoltage',
                'attributeId': 7,
                'type': 'int',
                'reportable': true
            },
            8: {
                'attributeName': 'WiredMaximumCurrent',
                'attributeId': 8,
                'type': 'int',
                'reportable': true
            },
            9: {
                'attributeName': 'WiredPresent',
                'attributeId': 9,
                'type': 'bool',
                'reportable': true
            },
            10: {
                'attributeName': 'ActiveWiredFaults',
                'attributeId': 10,
                'type': 'int',
                'reportable': true
            },
            11: {
                'attributeName': 'BatVoltage',
                'attributeId': 11,
                'type': 'int',
                'reportable': true
            },
            12: {
                'attributeName': 'BatPercentRemaining',
                'attributeId': 12,
                'type': 'int',
                'reportable': true
            },
            13: {
                'attributeName': 'BatTimeRemaining',
                'attributeId': 13,
                'type': 'int',
                'reportable': true
            },
            14: {
                'attributeName': 'BatChargeLevel',
                'attributeId': 14,
                'type': 'int',
                'reportable': true
            },
            15: {
                'attributeName': 'BatReplacementNeeded',
                'attributeId': 15,
                'type': 'bool',
                'reportable': true
            },
            16: {
                'attributeName': 'BatReplaceability',
                'attributeId': 16,
                'type': 'int',
                'reportable': true
            },
            17: {
                'attributeName': 'BatPresent',
                'attributeId': 17,
                'type': 'bool',
                'reportable': true
            },
            18: {
                'attributeName': 'ActiveBatFaults',
                'attributeId': 18,
                'type': 'int',
                'reportable': true
            },
            19: {
                'attributeName': 'BatReplacementDescription',
                'attributeId': 19,
                'type': 'str',
                'reportable': true
            },
            20: {
                'attributeName': 'BatCommonDesignation',
                'attributeId': 20,
                'type': 'int',
                'reportable': true
            },
            21: {
                'attributeName': 'BatANSIDesignation',
                'attributeId': 21,
                'type': 'str',
                'reportable': true
            },
            22: {
                'attributeName': 'BatIECDesignation',
                'attributeId': 22,
                'type': 'str',
                'reportable': true
            },
            23: {
                'attributeName': 'BatApprovedChemistry',
                'attributeId': 23,
                'type': 'int',
                'reportable': true
            },
            24: {
                'attributeName': 'BatCapacity',
                'attributeId': 24,
                'type': 'int',
                'reportable': true
            },
            25: {
                'attributeName': 'BatQuantity',
                'attributeId': 25,
                'type': 'int',
                'reportable': true
            },
            26: {
                'attributeName': 'BatChargeState',
                'attributeId': 26,
                'type': 'int',
                'reportable': true
            },
            27: {
                'attributeName': 'BatTimeToFullCharge',
                'attributeId': 27,
                'type': 'int',
                'reportable': true
            },
            28: {
                'attributeName': 'BatFunctionalWhileCharging',
                'attributeId': 28,
                'type': 'bool',
                'reportable': true
            },
            29: {
                'attributeName': 'BatChargingCurrent',
                'attributeId': 29,
                'type': 'int',
                'reportable': true
            },
            30: {
                'attributeName': 'ActiveBatChargeFaults',
                'attributeId': 30,
                'type': 'int',
                'reportable': true
            },
            65528: {
                'attributeName': 'GeneratedCommandList',
                'attributeId': 65528,
                'type': 'int',
                'reportable': true
            },
            65529: {
                'attributeName': 'AcceptedCommandList',
                'attributeId': 65529,
                'type': 'int',
                'reportable': true
            },
            65531: {
                'attributeName': 'AttributeList',
                'attributeId': 65531,
                'type': 'int',
                'reportable': true
            },
            65532: {
                'attributeName': 'FeatureMap',
                'attributeId': 65532,
                'type': 'int',
                'reportable': true
            },
            65533: {
                'attributeName': 'ClusterRevision',
                'attributeId': 65533,
                'type': 'int',
                'reportable': true
            }
        }
    },
    'GeneralCommissioning': {
        'clusterName': 'GeneralCommissioning',
        'clusterId': 48,
        'commands': {
            0: {
                'commandId': 0,
                'commandName': 'ArmFailSafe',
                'args': {
                    'expiryLengthSeconds': 'int',
                    'breadcrumb': 'int'
                }
            },
            2: {
                'commandId': 2,
                'commandName': 'SetRegulatoryConfig',
                'args': {
                    'newRegulatoryConfig': 'int',
                    'countryCode': 'str',
                    'breadcrumb': 'int'
                }
            },
            4: {
                'commandId': 4,
                'commandName': 'CommissioningComplete',
                'args': {}
            }
        },
        'attributes': {
            0: {
                'attributeName': 'Breadcrumb',
                'attributeId': 0,
                'type': 'int',
                'reportable': true,
                'writable': true
            },
            1: {
                'attributeName': 'BasicCommissioningInfo',
                'attributeId': 1,
                'type': '',
                'reportable': true
            },
            2: {
                'attributeName': 'RegulatoryConfig',
                'attributeId': 2,
                'type': 'int',
                'reportable': true
            },
            3: {
                'attributeName': 'LocationCapability',
                'attributeId': 3,
                'type': 'int',
                'reportable': true
            },
            4: {
                'attributeName': 'SupportsConcurrentConnection',
                'attributeId': 4,
                'type': 'bool',
                'reportable': true
            },
            65528: {
                'attributeName': 'GeneratedCommandList',
                'attributeId': 65528,
                'type': 'int',
                'reportable': true
            },
            65529: {
                'attributeName': 'AcceptedCommandList',
                'attributeId': 65529,
                'type': 'int',
                'reportable': true
            },
            65531: {
                'attributeName': 'AttributeList',
                'attributeId': 65531,
                'type': 'int',
                'reportable': true
            },
            65532: {
                'attributeName': 'FeatureMap',
                'attributeId': 65532,
                'type': 'int',
                'reportable': true
            },
            65533: {
                'attributeName': 'ClusterRevision',
                'attributeId': 65533,
                'type': 'int',
                'reportable': true
            }
        }
    },
    'NetworkCommissioning': {
        'clusterName': 'NetworkCommissioning',
        'clusterId': 49,
        'commands': {
            0: {
                'commandId': 0,
                'commandName': 'ScanNetworks',
                'args': {
                    'ssid': 'bytes',
                    'breadcrumb': 'int'
                }
            },
            2: {
                'commandId': 2,
                'commandName': 'AddOrUpdateWiFiNetwork',
                'args': {
                    'ssid': 'bytes',
                    'credentials': 'bytes',
                    'breadcrumb': 'int'
                }
            },
            3: {
                'commandId': 3,
                'commandName': 'AddOrUpdateThreadNetwork',
                'args': {
                    'operationalDataset': 'bytes',
                    'breadcrumb': 'int'
                }
            },
            4: {
                'commandId': 4,
                'commandName': 'RemoveNetwork',
                'args': {
                    'networkID': 'bytes',
                    'breadcrumb': 'int'
                }
            },
            6: {
                'commandId': 6,
                'commandName': 'ConnectNetwork',
                'args': {
                    'networkID': 'bytes',
                    'breadcrumb': 'int'
                }
            },
            8: {
                'commandId': 8,
                'commandName': 'ReorderNetwork',
                'args': {
                    'networkID': 'bytes',
                    'networkIndex': 'int',
                    'breadcrumb': 'int'
                }
            }
        },
        'attributes': {
            0: {
                'attributeName': 'MaxNetworks',
                'attributeId': 0,
                'type': 'int',
                'reportable': true
            },
            1: {
                'attributeName': 'Networks',
                'attributeId': 1,
                'type': '',
                'reportable': true
            },
            2: {
                'attributeName': 'ScanMaxTimeSeconds',
                'attributeId': 2,
                'type': 'int',
                'reportable': true
            },
            3: {
                'attributeName': 'ConnectMaxTimeSeconds',
                'attributeId': 3,
                'type': 'int',
                'reportable': true
            },
            4: {
                'attributeName': 'InterfaceEnabled',
                'attributeId': 4,
                'type': 'bool',
                'reportable': true,
                'writable': true
            },
            5: {
                'attributeName': 'LastNetworkingStatus',
                'attributeId': 5,
                'type': 'int',
                'reportable': true
            },
            6: {
                'attributeName': 'LastNetworkID',
                'attributeId': 6,
                'type': 'bytes',
                'reportable': true
            },
            7: {
                'attributeName': 'LastConnectErrorValue',
                'attributeId': 7,
                'type': 'int',
                'reportable': true
            },
            65528: {
                'attributeName': 'GeneratedCommandList',
                'attributeId': 65528,
                'type': 'int',
                'reportable': true
            },
            65529: {
                'attributeName': 'AcceptedCommandList',
                'attributeId': 65529,
                'type': 'int',
                'reportable': true
            },
            65532: {
                'attributeName': 'FeatureMap',
                'attributeId': 65532,
                'type': 'int',
                'reportable': true
            },
            65533: {
                'attributeName': 'ClusterRevision',
                'attributeId': 65533,
                'type': 'int',
                'reportable': true
            }
        }
    },
    'DiagnosticLogs': {
        'clusterName': 'DiagnosticLogs',
        'clusterId': 50,
        'commands': {
            0: {
                'commandId': 0,
                'commandName': 'RetrieveLogsRequest',
                'args': {
                    'intent': 'int',
                    'requestedProtocol': 'int',
                    'transferFileDesignator': 'bytes'
                }
            }
        },
        'attributes': {
            65528: {
                'attributeName': 'GeneratedCommandList',
                'attributeId': 65528,
                'type': 'int',
                'reportable': true
            },
            65529: {
                'attributeName': 'AcceptedCommandList',
                'attributeId': 65529,
                'type': 'int',
                'reportable': true
            },
            65531: {
                'attributeName': 'AttributeList',
                'attributeId': 65531,
                'type': 'int',
                'reportable': true
            },
            65532: {
                'attributeName': 'FeatureMap',
                'attributeId': 65532,
                'type': 'int',
                'reportable': true
            },
            65533: {
                'attributeName': 'ClusterRevision',
                'attributeId': 65533,
                'type': 'int',
                'reportable': true
            }
        }
    },
    'GeneralDiagnostics': {
        'clusterName': 'GeneralDiagnostics',
        'clusterId': 51,
        'commands': {
            0: {
                'commandId': 0,
                'commandName': 'TestEventTrigger',
                'args': {
                    'enableKey': 'bytes',
                    'eventTrigger': 'int'
                }
            }
        },
        'attributes': {
            0: {
                'attributeName': 'NetworkInterfaces',
                'attributeId': 0,
                'type': '',
                'reportable': true
            },
            1: {
                'attributeName': 'RebootCount',
                'attributeId': 1,
                'type': 'int',
                'reportable': true
            },
            2: {
                'attributeName': 'UpTime',
                'attributeId': 2,
                'type': 'int',
                'reportable': true
            },
            3: {
                'attributeName': 'TotalOperationalHours',
                'attributeId': 3,
                'type': 'int',
                'reportable': true
            },
            4: {
                'attributeName': 'BootReasons',
                'attributeId': 4,
                'type': 'int',
                'reportable': true
            },
            5: {
                'attributeName': 'ActiveHardwareFaults',
                'attributeId': 5,
                'type': 'int',
                'reportable': true
            },
            6: {
                'attributeName': 'ActiveRadioFaults',
                'attributeId': 6,
                'type': 'int',
                'reportable': true
            },
            7: {
                'attributeName': 'ActiveNetworkFaults',
                'attributeId': 7,
                'type': 'int',
                'reportable': true
            },
            8: {
                'attributeName': 'TestEventTriggersEnabled',
                'attributeId': 8,
                'type': 'bool',
                'reportable': true
            },
            65528: {
                'attributeName': 'GeneratedCommandList',
                'attributeId': 65528,
                'type': 'int',
                'reportable': true
            },
            65529: {
                'attributeName': 'AcceptedCommandList',
                'attributeId': 65529,
                'type': 'int',
                'reportable': true
            },
            65531: {
                'attributeName': 'AttributeList',
                'attributeId': 65531,
                'type': 'int',
                'reportable': true
            },
            65532: {
                'attributeName': 'FeatureMap',
                'attributeId': 65532,
                'type': 'int',
                'reportable': true
            },
            65533: {
                'attributeName': 'ClusterRevision',
                'attributeId': 65533,
                'type': 'int',
                'reportable': true
            }
        }
    },
    'SoftwareDiagnostics': {
        'clusterName': 'SoftwareDiagnostics',
        'clusterId': 52,
        'commands': {
            0: {
                'commandId': 0,
                'commandName': 'ResetWatermarks',
                'args': {}
            }
        },
        'attributes': {
            0: {
                'attributeName': 'ThreadMetrics',
                'attributeId': 0,
                'type': '',
                'reportable': true
            },
            1: {
                'attributeName': 'CurrentHeapFree',
                'attributeId': 1,
                'type': 'int',
                'reportable': true
            },
            2: {
                'attributeName': 'CurrentHeapUsed',
                'attributeId': 2,
                'type': 'int',
                'reportable': true
            },
            3: {
                'attributeName': 'CurrentHeapHighWatermark',
                'attributeId': 3,
                'type': 'int',
                'reportable': true
            },
            65528: {
                'attributeName': 'GeneratedCommandList',
                'attributeId': 65528,
                'type': 'int',
                'reportable': true
            },
            65529: {
                'attributeName': 'AcceptedCommandList',
                'attributeId': 65529,
                'type': 'int',
                'reportable': true
            },
            65531: {
                'attributeName': 'AttributeList',
                'attributeId': 65531,
                'type': 'int',
                'reportable': true
            },
            65532: {
                'attributeName': 'FeatureMap',
                'attributeId': 65532,
                'type': 'int',
                'reportable': true
            },
            65533: {
                'attributeName': 'ClusterRevision',
                'attributeId': 65533,
                'type': 'int',
                'reportable': true
            }
        }
    },
    'ThreadNetworkDiagnostics': {
        'clusterName': 'ThreadNetworkDiagnostics',
        'clusterId': 53,
        'commands': {
            0: {
                'commandId': 0,
                'commandName': 'ResetCounts',
                'args': {}
            }
        },
        'attributes': {
            0: {
                'attributeName': 'Channel',
                'attributeId': 0,
                'type': 'int',
                'reportable': true
            },
            1: {
                'attributeName': 'RoutingRole',
                'attributeId': 1,
                'type': 'int',
                'reportable': true
            },
            2: {
                'attributeName': 'NetworkName',
                'attributeId': 2,
                'type': 'str',
                'reportable': true
            },
            3: {
                'attributeName': 'PanId',
                'attributeId': 3,
                'type': 'int',
                'reportable': true
            },
            4: {
                'attributeName': 'ExtendedPanId',
                'attributeId': 4,
                'type': 'int',
                'reportable': true
            },
            5: {
                'attributeName': 'MeshLocalPrefix',
                'attributeId': 5,
                'type': 'bytes',
                'reportable': true
            },
            6: {
                'attributeName': 'OverrunCount',
                'attributeId': 6,
                'type': 'int',
                'reportable': true
            },
            7: {
                'attributeName': 'NeighborTableList',
                'attributeId': 7,
                'type': '',
                'reportable': true
            },
            8: {
                'attributeName': 'RouteTableList',
                'attributeId': 8,
                'type': '',
                'reportable': true
            },
            9: {
                'attributeName': 'PartitionId',
                'attributeId': 9,
                'type': 'int',
                'reportable': true
            },
            10: {
                'attributeName': 'Weighting',
                'attributeId': 10,
                'type': 'int',
                'reportable': true
            },
            11: {
                'attributeName': 'DataVersion',
                'attributeId': 11,
                'type': 'int',
                'reportable': true
            },
            12: {
                'attributeName': 'StableDataVersion',
                'attributeId': 12,
                'type': 'int',
                'reportable': true
            },
            13: {
                'attributeName': 'LeaderRouterId',
                'attributeId': 13,
                'type': 'int',
                'reportable': true
            },
            14: {
                'attributeName': 'DetachedRoleCount',
                'attributeId': 14,
                'type': 'int',
                'reportable': true
            },
            15: {
                'attributeName': 'ChildRoleCount',
                'attributeId': 15,
                'type': 'int',
                'reportable': true
            },
            16: {
                'attributeName': 'RouterRoleCount',
                'attributeId': 16,
                'type': 'int',
                'reportable': true
            },
            17: {
                'attributeName': 'LeaderRoleCount',
                'attributeId': 17,
                'type': 'int',
                'reportable': true
            },
            18: {
                'attributeName': 'AttachAttemptCount',
                'attributeId': 18,
                'type': 'int',
                'reportable': true
            },
            19: {
                'attributeName': 'PartitionIdChangeCount',
                'attributeId': 19,
                'type': 'int',
                'reportable': true
            },
            20: {
                'attributeName': 'BetterPartitionAttachAttemptCount',
                'attributeId': 20,
                'type': 'int',
                'reportable': true
            },
            21: {
                'attributeName': 'ParentChangeCount',
                'attributeId': 21,
                'type': 'int',
                'reportable': true
            },
            22: {
                'attributeName': 'TxTotalCount',
                'attributeId': 22,
                'type': 'int',
                'reportable': true
            },
            23: {
                'attributeName': 'TxUnicastCount',
                'attributeId': 23,
                'type': 'int',
                'reportable': true
            },
            24: {
                'attributeName': 'TxBroadcastCount',
                'attributeId': 24,
                'type': 'int',
                'reportable': true
            },
            25: {
                'attributeName': 'TxAckRequestedCount',
                'attributeId': 25,
                'type': 'int',
                'reportable': true
            },
            26: {
                'attributeName': 'TxAckedCount',
                'attributeId': 26,
                'type': 'int',
                'reportable': true
            },
            27: {
                'attributeName': 'TxNoAckRequestedCount',
                'attributeId': 27,
                'type': 'int',
                'reportable': true
            },
            28: {
                'attributeName': 'TxDataCount',
                'attributeId': 28,
                'type': 'int',
                'reportable': true
            },
            29: {
                'attributeName': 'TxDataPollCount',
                'attributeId': 29,
                'type': 'int',
                'reportable': true
            },
            30: {
                'attributeName': 'TxBeaconCount',
                'attributeId': 30,
                'type': 'int',
                'reportable': true
            },
            31: {
                'attributeName': 'TxBeaconRequestCount',
                'attributeId': 31,
                'type': 'int',
                'reportable': true
            },
            32: {
                'attributeName': 'TxOtherCount',
                'attributeId': 32,
                'type': 'int',
                'reportable': true
            },
            33: {
                'attributeName': 'TxRetryCount',
                'attributeId': 33,
                'type': 'int',
                'reportable': true
            },
            34: {
                'attributeName': 'TxDirectMaxRetryExpiryCount',
                'attributeId': 34,
                'type': 'int',
                'reportable': true
            },
            35: {
                'attributeName': 'TxIndirectMaxRetryExpiryCount',
                'attributeId': 35,
                'type': 'int',
                'reportable': true
            },
            36: {
                'attributeName': 'TxErrCcaCount',
                'attributeId': 36,
                'type': 'int',
                'reportable': true
            },
            37: {
                'attributeName': 'TxErrAbortCount',
                'attributeId': 37,
                'type': 'int',
                'reportable': true
            },
            38: {
                'attributeName': 'TxErrBusyChannelCount',
                'attributeId': 38,
                'type': 'int',
                'reportable': true
            },
            39: {
                'attributeName': 'RxTotalCount',
                'attributeId': 39,
                'type': 'int',
                'reportable': true
            },
            40: {
                'attributeName': 'RxUnicastCount',
                'attributeId': 40,
                'type': 'int',
                'reportable': true
            },
            41: {
                'attributeName': 'RxBroadcastCount',
                'attributeId': 41,
                'type': 'int',
                'reportable': true
            },
            42: {
                'attributeName': 'RxDataCount',
                'attributeId': 42,
                'type': 'int',
                'reportable': true
            },
            43: {
                'attributeName': 'RxDataPollCount',
                'attributeId': 43,
                'type': 'int',
                'reportable': true
            },
            44: {
                'attributeName': 'RxBeaconCount',
                'attributeId': 44,
                'type': 'int',
                'reportable': true
            },
            45: {
                'attributeName': 'RxBeaconRequestCount',
                'attributeId': 45,
                'type': 'int',
                'reportable': true
            },
            46: {
                'attributeName': 'RxOtherCount',
                'attributeId': 46,
                'type': 'int',
                'reportable': true
            },
            47: {
                'attributeName': 'RxAddressFilteredCount',
                'attributeId': 47,
                'type': 'int',
                'reportable': true
            },
            48: {
                'attributeName': 'RxDestAddrFilteredCount',
                'attributeId': 48,
                'type': 'int',
                'reportable': true
            },
            49: {
                'attributeName': 'RxDuplicatedCount',
                'attributeId': 49,
                'type': 'int',
                'reportable': true
            },
            50: {
                'attributeName': 'RxErrNoFrameCount',
                'attributeId': 50,
                'type': 'int',
                'reportable': true
            },
            51: {
                'attributeName': 'RxErrUnknownNeighborCount',
                'attributeId': 51,
                'type': 'int',
                'reportable': true
            },
            52: {
                'attributeName': 'RxErrInvalidSrcAddrCount',
                'attributeId': 52,
                'type': 'int',
                'reportable': true
            },
            53: {
                'attributeName': 'RxErrSecCount',
                'attributeId': 53,
                'type': 'int',
                'reportable': true
            },
            54: {
                'attributeName': 'RxErrFcsCount',
                'attributeId': 54,
                'type': 'int',
                'reportable': true
            },
            55: {
                'attributeName': 'RxErrOtherCount',
                'attributeId': 55,
                'type': 'int',
                'reportable': true
            },
            56: {
                'attributeName': 'ActiveTimestamp',
                'attributeId': 56,
                'type': 'int',
                'reportable': true
            },
            57: {
                'attributeName': 'PendingTimestamp',
                'attributeId': 57,
                'type': 'int',
                'reportable': true
            },
            58: {
                'attributeName': 'Delay',
                'attributeId': 58,
                'type': 'int',
                'reportable': true
            },
            59: {
                'attributeName': 'SecurityPolicy',
                'attributeId': 59,
                'type': '',
                'reportable': true
            },
            60: {
                'attributeName': 'ChannelPage0Mask',
                'attributeId': 60,
                'type': 'bytes',
                'reportable': true
            },
            61: {
                'attributeName': 'OperationalDatasetComponents',
                'attributeId': 61,
                'type': '',
                'reportable': true
            },
            62: {
                'attributeName': 'ActiveNetworkFaultsList',
                'attributeId': 62,
                'type': 'int',
                'reportable': true
            },
            65528: {
                'attributeName': 'GeneratedCommandList',
                'attributeId': 65528,
                'type': 'int',
                'reportable': true
            },
            65529: {
                'attributeName': 'AcceptedCommandList',
                'attributeId': 65529,
                'type': 'int',
                'reportable': true
            },
            65531: {
                'attributeName': 'AttributeList',
                'attributeId': 65531,
                'type': 'int',
                'reportable': true
            },
            65532: {
                'attributeName': 'FeatureMap',
                'attributeId': 65532,
                'type': 'int',
                'reportable': true
            },
            65533: {
                'attributeName': 'ClusterRevision',
                'attributeId': 65533,
                'type': 'int',
                'reportable': true
            }
        }
    },
    'WiFiNetworkDiagnostics': {
        'clusterName': 'WiFiNetworkDiagnostics',
        'clusterId': 54,
        'commands': {
            0: {
                'commandId': 0,
                'commandName': 'ResetCounts',
                'args': {}
            }
        },
        'attributes': {
            0: {
                'attributeName': 'Bssid',
                'attributeId': 0,
                'type': 'bytes',
                'reportable': true
            },
            1: {
                'attributeName': 'SecurityType',
                'attributeId': 1,
                'type': 'int',
                'reportable': true
            },
            2: {
                'attributeName': 'WiFiVersion',
                'attributeId': 2,
                'type': 'int',
                'reportable': true
            },
            3: {
                'attributeName': 'ChannelNumber',
                'attributeId': 3,
                'type': 'int',
                'reportable': true
            },
            4: {
                'attributeName': 'Rssi',
                'attributeId': 4,
                'type': 'int',
                'reportable': true
            },
            5: {
                'attributeName': 'BeaconLostCount',
                'attributeId': 5,
                'type': 'int',
                'reportable': true
            },
            6: {
                'attributeName': 'BeaconRxCount',
                'attributeId': 6,
                'type': 'int',
                'reportable': true
            },
            7: {
                'attributeName': 'PacketMulticastRxCount',
                'attributeId': 7,
                'type': 'int',
                'reportable': true
            },
            8: {
                'attributeName': 'PacketMulticastTxCount',
                'attributeId': 8,
                'type': 'int',
                'reportable': true
            },
            9: {
                'attributeName': 'PacketUnicastRxCount',
                'attributeId': 9,
                'type': 'int',
                'reportable': true
            },
            10: {
                'attributeName': 'PacketUnicastTxCount',
                'attributeId': 10,
                'type': 'int',
                'reportable': true
            },
            11: {
                'attributeName': 'CurrentMaxRate',
                'attributeId': 11,
                'type': 'int',
                'reportable': true
            },
            12: {
                'attributeName': 'OverrunCount',
                'attributeId': 12,
                'type': 'int',
                'reportable': true
            },
            65528: {
                'attributeName': 'GeneratedCommandList',
                'attributeId': 65528,
                'type': 'int',
                'reportable': true
            },
            65529: {
                'attributeName': 'AcceptedCommandList',
                'attributeId': 65529,
                'type': 'int',
                'reportable': true
            },
            65531: {
                'attributeName': 'AttributeList',
                'attributeId': 65531,
                'type': 'int',
                'reportable': true
            },
            65532: {
                'attributeName': 'FeatureMap',
                'attributeId': 65532,
                'type': 'int',
                'reportable': true
            },
            65533: {
                'attributeName': 'ClusterRevision',
                'attributeId': 65533,
                'type': 'int',
                'reportable': true
            }
        }
    },
    'EthernetNetworkDiagnostics': {
        'clusterName': 'EthernetNetworkDiagnostics',
        'clusterId': 55,
        'commands': {
            0: {
                'commandId': 0,
                'commandName': 'ResetCounts',
                'args': {}
            }
        },
        'attributes': {
            0: {
                'attributeName': 'PHYRate',
                'attributeId': 0,
                'type': 'int',
                'reportable': true
            },
            1: {
                'attributeName': 'FullDuplex',
                'attributeId': 1,
                'type': 'bool',
                'reportable': true
            },
            2: {
                'attributeName': 'PacketRxCount',
                'attributeId': 2,
                'type': 'int',
                'reportable': true
            },
            3: {
                'attributeName': 'PacketTxCount',
                'attributeId': 3,
                'type': 'int',
                'reportable': true
            },
            4: {
                'attributeName': 'TxErrCount',
                'attributeId': 4,
                'type': 'int',
                'reportable': true
            },
            5: {
                'attributeName': 'CollisionCount',
                'attributeId': 5,
                'type': 'int',
                'reportable': true
            },
            6: {
                'attributeName': 'OverrunCount',
                'attributeId': 6,
                'type': 'int',
                'reportable': true
            },
            7: {
                'attributeName': 'CarrierDetect',
                'attributeId': 7,
                'type': 'bool',
                'reportable': true
            },
            8: {
                'attributeName': 'TimeSinceReset',
                'attributeId': 8,
                'type': 'int',
                'reportable': true
            },
            65528: {
                'attributeName': 'GeneratedCommandList',
                'attributeId': 65528,
                'type': 'int',
                'reportable': true
            },
            65529: {
                'attributeName': 'AcceptedCommandList',
                'attributeId': 65529,
                'type': 'int',
                'reportable': true
            },
            65531: {
                'attributeName': 'AttributeList',
                'attributeId': 65531,
                'type': 'int',
                'reportable': true
            },
            65532: {
                'attributeName': 'FeatureMap',
                'attributeId': 65532,
                'type': 'int',
                'reportable': true
            },
            65533: {
                'attributeName': 'ClusterRevision',
                'attributeId': 65533,
                'type': 'int',
                'reportable': true
            }
        }
    },
    'BridgedDeviceBasic': {
        'clusterName': 'BridgedDeviceBasic',
        'clusterId': 57,
        'commands': {},
        'attributes': {
            1: {
                'attributeName': 'VendorName',
                'attributeId': 1,
                'type': 'str',
                'reportable': true
            },
            2: {
                'attributeName': 'VendorID',
                'attributeId': 2,
                'type': 'int',
                'reportable': true
            },
            3: {
                'attributeName': 'ProductName',
                'attributeId': 3,
                'type': 'str',
                'reportable': true
            },
            5: {
                'attributeName': 'NodeLabel',
                'attributeId': 5,
                'type': 'str',
                'reportable': true,
                'writable': true
            },
            7: {
                'attributeName': 'HardwareVersion',
                'attributeId': 7,
                'type': 'int',
                'reportable': true
            },
            8: {
                'attributeName': 'HardwareVersionString',
                'attributeId': 8,
                'type': 'str',
                'reportable': true
            },
            9: {
                'attributeName': 'SoftwareVersion',
                'attributeId': 9,
                'type': 'int',
                'reportable': true
            },
            10: {
                'attributeName': 'SoftwareVersionString',
                'attributeId': 10,
                'type': 'str',
                'reportable': true
            },
            11: {
                'attributeName': 'ManufacturingDate',
                'attributeId': 11,
                'type': 'str',
                'reportable': true
            },
            12: {
                'attributeName': 'PartNumber',
                'attributeId': 12,
                'type': 'str',
                'reportable': true
            },
            13: {
                'attributeName': 'ProductURL',
                'attributeId': 13,
                'type': 'str',
                'reportable': true
            },
            14: {
                'attributeName': 'ProductLabel',
                'attributeId': 14,
                'type': 'str',
                'reportable': true
            },
            15: {
                'attributeName': 'SerialNumber',
                'attributeId': 15,
                'type': 'str',
                'reportable': true
            },
            17: {
                'attributeName': 'Reachable',
                'attributeId': 17,
                'type': 'bool',
                'reportable': true
            },
            18: {
                'attributeName': 'UniqueID',
                'attributeId': 18,
                'type': 'str',
                'reportable': true
            },
            65528: {
                'attributeName': 'GeneratedCommandList',
                'attributeId': 65528,
                'type': 'int',
                'reportable': true
            },
            65529: {
                'attributeName': 'AcceptedCommandList',
                'attributeId': 65529,
                'type': 'int',
                'reportable': true
            },
            65531: {
                'attributeName': 'AttributeList',
                'attributeId': 65531,
                'type': 'int',
                'reportable': true
            },
            65532: {
                'attributeName': 'FeatureMap',
                'attributeId': 65532,
                'type': 'int',
                'reportable': true
            },
            65533: {
                'attributeName': 'ClusterRevision',
                'attributeId': 65533,
                'type': 'int',
                'reportable': true
            }
        }
    },
    'Switch': {
        'clusterName': 'Switch',
        'clusterId': 59,
        'commands': {},
        'attributes': {
            0: {
                'attributeName': 'NumberOfPositions',
                'attributeId': 0,
                'type': 'int',
                'reportable': true
            },
            1: {
                'attributeName': 'CurrentPosition',
                'attributeId': 1,
                'type': 'int',
                'reportable': true
            },
            2: {
                'attributeName': 'MultiPressMax',
                'attributeId': 2,
                'type': 'int',
                'reportable': true
            },
            65528: {
                'attributeName': 'GeneratedCommandList',
                'attributeId': 65528,
                'type': 'int',
                'reportable': true
            },
            65529: {
                'attributeName': 'AcceptedCommandList',
                'attributeId': 65529,
                'type': 'int',
                'reportable': true
            },
            65531: {
                'attributeName': 'AttributeList',
                'attributeId': 65531,
                'type': 'int',
                'reportable': true
            },
            65532: {
                'attributeName': 'FeatureMap',
                'attributeId': 65532,
                'type': 'int',
                'reportable': true
            },
            65533: {
                'attributeName': 'ClusterRevision',
                'attributeId': 65533,
                'type': 'int',
                'reportable': true
            }
        }
    },
    'AdministratorCommissioning': {
        'clusterName': 'AdministratorCommissioning',
        'clusterId': 60,
        'commands': {
            0: {
                'commandId': 0,
                'commandName': 'OpenCommissioningWindow',
                'args': {
                    'commissioningTimeout': 'int',
                    'PAKEVerifier': 'bytes',
                    'discriminator': 'int',
                    'iterations': 'int',
                    'salt': 'bytes'
                }
            },
            1: {
                'commandId': 1,
                'commandName': 'OpenBasicCommissioningWindow',
                'args': {
                    'commissioningTimeout': 'int'
                }
            },
            2: {
                'commandId': 2,
                'commandName': 'RevokeCommissioning',
                'args': {}
            }
        },
        'attributes': {
            0: {
                'attributeName': 'WindowStatus',
                'attributeId': 0,
                'type': 'int',
                'reportable': true
            },
            1: {
                'attributeName': 'AdminFabricIndex',
                'attributeId': 1,
                'type': 'int',
                'reportable': true
            },
            2: {
                'attributeName': 'AdminVendorId',
                'attributeId': 2,
                'type': 'int',
                'reportable': true
            },
            65528: {
                'attributeName': 'GeneratedCommandList',
                'attributeId': 65528,
                'type': 'int',
                'reportable': true
            },
            65529: {
                'attributeName': 'AcceptedCommandList',
                'attributeId': 65529,
                'type': 'int',
                'reportable': true
            },
            65531: {
                'attributeName': 'AttributeList',
                'attributeId': 65531,
                'type': 'int',
                'reportable': true
            },
            65532: {
                'attributeName': 'FeatureMap',
                'attributeId': 65532,
                'type': 'int',
                'reportable': true
            },
            65533: {
                'attributeName': 'ClusterRevision',
                'attributeId': 65533,
                'type': 'int',
                'reportable': true
            }
        }
    },
    'OperationalCredentials': {
        'clusterName': 'OperationalCredentials',
        'clusterId': 62,
        'commands': {
            0: {
                'commandId': 0,
                'commandName': 'AttestationRequest',
                'args': {
                    'attestationNonce': 'bytes'
                }
            },
            2: {
                'commandId': 2,
                'commandName': 'CertificateChainRequest',
                'args': {
                    'certificateType': 'int'
                }
            },
            4: {
                'commandId': 4,
                'commandName': 'CSRRequest',
                'args': {
                    'CSRNonce': 'bytes',
                    'isForUpdateNOC': 'bool'
                }
            },
            6: {
                'commandId': 6,
                'commandName': 'AddNOC',
                'args': {
                    'NOCValue': 'bytes',
                    'ICACValue': 'bytes',
                    'IPKValue': 'bytes',
                    'caseAdminSubject': 'int',
                    'adminVendorId': 'int'
                }
            },
            7: {
                'commandId': 7,
                'commandName': 'UpdateNOC',
                'args': {
                    'NOCValue': 'bytes',
                    'ICACValue': 'bytes'
                }
            },
            9: {
                'commandId': 9,
                'commandName': 'UpdateFabricLabel',
                'args': {
                    'label': 'str'
                }
            },
            10: {
                'commandId': 10,
                'commandName': 'RemoveFabric',
                'args': {
                    'fabricIndex': 'int'
                }
            },
            11: {
                'commandId': 11,
                'commandName': 'AddTrustedRootCertificate',
                'args': {
                    'rootCertificate': 'bytes'
                }
            }
        },
        'attributes': {
            0: {
                'attributeName': 'NOCs',
                'attributeId': 0,
                'type': '',
                'reportable': true
            },
            1: {
                'attributeName': 'Fabrics',
                'attributeId': 1,
                'type': '',
                'reportable': true
            },
            2: {
                'attributeName': 'SupportedFabrics',
                'attributeId': 2,
                'type': 'int',
                'reportable': true
            },
            3: {
                'attributeName': 'CommissionedFabrics',
                'attributeId': 3,
                'type': 'int',
                'reportable': true
            },
            4: {
                'attributeName': 'TrustedRootCertificates',
                'attributeId': 4,
                'type': 'bytes',
                'reportable': true
            },
            5: {
                'attributeName': 'CurrentFabricIndex',
                'attributeId': 5,
                'type': 'int',
                'reportable': true
            },
            65528: {
                'attributeName': 'GeneratedCommandList',
                'attributeId': 65528,
                'type': 'int',
                'reportable': true
            },
            65529: {
                'attributeName': 'AcceptedCommandList',
                'attributeId': 65529,
                'type': 'int',
                'reportable': true
            },
            65531: {
                'attributeName': 'AttributeList',
                'attributeId': 65531,
                'type': 'int',
                'reportable': true
            },
            65532: {
                'attributeName': 'FeatureMap',
                'attributeId': 65532,
                'type': 'int',
                'reportable': true
            },
            65533: {
                'attributeName': 'ClusterRevision',
                'attributeId': 65533,
                'type': 'int',
                'reportable': true
            }
        }
    },
    'GroupKeyManagement': {
        'clusterName': 'GroupKeyManagement',
        'clusterId': 63,
        'commands': {
            0: {
                'commandId': 0,
                'commandName': 'KeySetWrite',
                'args': {
                    'groupKeySetID': 'int',
                    'groupKeySecurityPolicy': 'int',
                    'epochKey0': 'bytes',
                    'epochStartTime0': 'int',
                    'epochKey1': 'bytes',
                    'epochStartTime1': 'int',
                    'epochKey2': 'bytes',
                    'epochStartTime2': 'int'
                }
            },
            1: {
                'commandId': 1,
                'commandName': 'KeySetRead',
                'args': {
                    'groupKeySetID': 'int'
                }
            },
            3: {
                'commandId': 3,
                'commandName': 'KeySetRemove',
                'args': {
                    'groupKeySetID': 'int'
                }
            },
            4: {
                'commandId': 4,
                'commandName': 'KeySetReadAllIndices',
                'args': {
                    'groupKeySetIDs': 'int'
                }
            }
        },
        'attributes': {
            0: {
                'attributeName': 'GroupKeyMap',
                'attributeId': 0,
                'type': '',
                'reportable': true,
                'writable': true
            },
            1: {
                'attributeName': 'GroupTable',
                'attributeId': 1,
                'type': '',
                'reportable': true
            },
            2: {
                'attributeName': 'MaxGroupsPerFabric',
                'attributeId': 2,
                'type': 'int',
                'reportable': true
            },
            3: {
                'attributeName': 'MaxGroupKeysPerFabric',
                'attributeId': 3,
                'type': 'int',
                'reportable': true
            },
            65528: {
                'attributeName': 'GeneratedCommandList',
                'attributeId': 65528,
                'type': 'int',
                'reportable': true
            },
            65529: {
                'attributeName': 'AcceptedCommandList',
                'attributeId': 65529,
                'type': 'int',
                'reportable': true
            },
            65531: {
                'attributeName': 'AttributeList',
                'attributeId': 65531,
                'type': 'int',
                'reportable': true
            },
            65532: {
                'attributeName': 'FeatureMap',
                'attributeId': 65532,
                'type': 'int',
                'reportable': true
            },
            65533: {
                'attributeName': 'ClusterRevision',
                'attributeId': 65533,
                'type': 'int',
                'reportable': true
            }
        }
    },
    'FixedLabel': {
        'clusterName': 'FixedLabel',
        'clusterId': 64,
        'commands': {},
        'attributes': {
            0: {
                'attributeName': 'LabelList',
                'attributeId': 0,
                'type': '',
                'reportable': true
            },
            65528: {
                'attributeName': 'GeneratedCommandList',
                'attributeId': 65528,
                'type': 'int',
                'reportable': true
            },
            65529: {
                'attributeName': 'AcceptedCommandList',
                'attributeId': 65529,
                'type': 'int',
                'reportable': true
            },
            65531: {
                'attributeName': 'AttributeList',
                'attributeId': 65531,
                'type': 'int',
                'reportable': true
            },
            65532: {
                'attributeName': 'FeatureMap',
                'attributeId': 65532,
                'type': 'int',
                'reportable': true
            },
            65533: {
                'attributeName': 'ClusterRevision',
                'attributeId': 65533,
                'type': 'int',
                'reportable': true
            }
        }
    },
    'UserLabel': {
        'clusterName': 'UserLabel',
        'clusterId': 65,
        'commands': {},
        'attributes': {
            0: {
                'attributeName': 'LabelList',
                'attributeId': 0,
                'type': '',
                'reportable': true,
                'writable': true
            },
            65528: {
                'attributeName': 'GeneratedCommandList',
                'attributeId': 65528,
                'type': 'int',
                'reportable': true
            },
            65529: {
                'attributeName': 'AcceptedCommandList',
                'attributeId': 65529,
                'type': 'int',
                'reportable': true
            },
            65532: {
                'attributeName': 'FeatureMap',
                'attributeId': 65532,
                'type': 'int',
                'reportable': true
            },
            65533: {
                'attributeName': 'ClusterRevision',
                'attributeId': 65533,
                'type': 'int',
                'reportable': true
            }
        }
    },
    'BooleanState': {
        'clusterName': 'BooleanState',
        'clusterId': 69,
        'commands': {},
        'attributes': {
            0: {
                'attributeName': 'StateValue',
                'attributeId': 0,
                'type': 'bool',
                'reportable': true
            },
            65528: {
                'attributeName': 'GeneratedCommandList',
                'attributeId': 65528,
                'type': 'int',
                'reportable': true
            },
            65529: {
                'attributeName': 'AcceptedCommandList',
                'attributeId': 65529,
                'type': 'int',
                'reportable': true
            },
            65531: {
                'attributeName': 'AttributeList',
                'attributeId': 65531,
                'type': 'int',
                'reportable': true
            },
            65532: {
                'attributeName': 'FeatureMap',
                'attributeId': 65532,
                'type': 'int',
                'reportable': true
            },
            65533: {
                'attributeName': 'ClusterRevision',
                'attributeId': 65533,
                'type': 'int',
                'reportable': true
            }
        }
    },
    'ModeSelect': {
        'clusterName': 'ModeSelect',
        'clusterId': 80,
        'commands': {
            0: {
                'commandId': 0,
                'commandName': 'ChangeToMode',
                'args': {
                    'newMode': 'int'
                }
            }
        },
        'attributes': {
            0: {
                'attributeName': 'Description',
                'attributeId': 0,
                'type': 'str',
                'reportable': true
            },
            1: {
                'attributeName': 'StandardNamespace',
                'attributeId': 1,
                'type': 'int',
                'reportable': true
            },
            2: {
                'attributeName': 'SupportedModes',
                'attributeId': 2,
                'type': '',
                'reportable': true
            },
            3: {
                'attributeName': 'CurrentMode',
                'attributeId': 3,
                'type': 'int',
                'reportable': true
            },
            4: {
                'attributeName': 'StartUpMode',
                'attributeId': 4,
                'type': 'int',
                'reportable': true,
                'writable': true
            },
            5: {
                'attributeName': 'OnMode',
                'attributeId': 5,
                'type': 'int',
                'reportable': true,
                'writable': true
            },
            65528: {
                'attributeName': 'GeneratedCommandList',
                'attributeId': 65528,
                'type': 'int',
                'reportable': true
            },
            65529: {
                'attributeName': 'AcceptedCommandList',
                'attributeId': 65529,
                'type': 'int',
                'reportable': true
            },
            65531: {
                'attributeName': 'AttributeList',
                'attributeId': 65531,
                'type': 'int',
                'reportable': true
            },
            65532: {
                'attributeName': 'FeatureMap',
                'attributeId': 65532,
                'type': 'int',
                'reportable': true
            },
            65533: {
                'attributeName': 'ClusterRevision',
                'attributeId': 65533,
                'type': 'int',
                'reportable': true
            }
        }
    },
    'DoorLock': {
        'clusterName': 'DoorLock',
        'clusterId': 257,
        'commands': {
            0: {
                'commandId': 0,
                'commandName': 'LockDoor',
                'args': {
                    'pinCode': 'bytes'
                }
            },
            1: {
                'commandId': 1,
                'commandName': 'UnlockDoor',
                'args': {
                    'pinCode': 'bytes'
                }
            },
            3: {
                'commandId': 3,
                'commandName': 'UnlockWithTimeout',
                'args': {
                    'timeout': 'int',
                    'pinCode': 'bytes'
                }
            },
            11: {
                'commandId': 11,
                'commandName': 'SetWeekDaySchedule',
                'args': {
                    'weekDayIndex': 'int',
                    'userIndex': 'int',
                    'daysMask': 'int',
                    'startHour': 'int',
                    'startMinute': 'int',
                    'endHour': 'int',
                    'endMinute': 'int'
                }
            },
            12: {
                'commandId': 12,
                'commandName': 'GetWeekDaySchedule',
                'args': {
                    'weekDayIndex': 'int',
                    'userIndex': 'int'
                }
            },
            13: {
                'commandId': 13,
                'commandName': 'ClearWeekDaySchedule',
                'args': {
                    'weekDayIndex': 'int',
                    'userIndex': 'int'
                }
            },
            14: {
                'commandId': 14,
                'commandName': 'SetYearDaySchedule',
                'args': {
                    'yearDayIndex': 'int',
                    'userIndex': 'int',
                    'localStartTime': 'int',
                    'localEndTime': 'int'
                }
            },
            15: {
                'commandId': 15,
                'commandName': 'GetYearDaySchedule',
                'args': {
                    'yearDayIndex': 'int',
                    'userIndex': 'int'
                }
            },
            16: {
                'commandId': 16,
                'commandName': 'ClearYearDaySchedule',
                'args': {
                    'yearDayIndex': 'int',
                    'userIndex': 'int'
                }
            },
            17: {
                'commandId': 17,
                'commandName': 'SetHolidaySchedule',
                'args': {
                    'holidayIndex': 'int',
                    'localStartTime': 'int',
                    'localEndTime': 'int',
                    'operatingMode': 'int'
                }
            },
            18: {
                'commandId': 18,
                'commandName': 'GetHolidaySchedule',
                'args': {
                    'holidayIndex': 'int'
                }
            },
            19: {
                'commandId': 19,
                'commandName': 'ClearHolidaySchedule',
                'args': {
                    'holidayIndex': 'int'
                }
            },
            26: {
                'commandId': 26,
                'commandName': 'SetUser',
                'args': {
                    'operationType': 'int',
                    'userIndex': 'int',
                    'userName': 'str',
                    'userUniqueId': 'int',
                    'userStatus': 'int',
                    'userType': 'int',
                    'credentialRule': 'int'
                }
            },
            27: {
                'commandId': 27,
                'commandName': 'GetUser',
                'args': {
                    'userIndex': 'int'
                }
            },
            29: {
                'commandId': 29,
                'commandName': 'ClearUser',
                'args': {
                    'userIndex': 'int'
                }
            },
            34: {
                'commandId': 34,
                'commandName': 'SetCredential',
                'args': {
                    'operationType': 'int',
                    'credentialType': 'int',
                    'credentialIndex': 'int',
                    'credentialData': 'bytes',
                    'userIndex': 'int',
                    'userStatus': 'int',
                    'userType': 'int'
                }
            },
            36: {
                'commandId': 36,
                'commandName': 'GetCredentialStatus',
                'args': {
                    'credentialType': 'int',
                    'credentialIndex': 'int'
                }
            },
            38: {
                'commandId': 38,
                'commandName': 'ClearCredential',
                'args': {
                    'credentialType': 'int',
                    'credentialIndex': 'int'
                }
            }
        },
        'attributes': {
            0: {
                'attributeName': 'LockState',
                'attributeId': 0,
                'type': 'int',
                'reportable': true
            },
            1: {
                'attributeName': 'LockType',
                'attributeId': 1,
                'type': 'int',
                'reportable': true
            },
            2: {
                'attributeName': 'ActuatorEnabled',
                'attributeId': 2,
                'type': 'bool',
                'reportable': true
            },
            3: {
                'attributeName': 'DoorState',
                'attributeId': 3,
                'type': 'int',
                'reportable': true
            },
            17: {
                'attributeName': 'NumberOfTotalUsersSupported',
                'attributeId': 17,
                'type': 'int',
                'reportable': true
            },
            18: {
                'attributeName': 'NumberOfPINUsersSupported',
                'attributeId': 18,
                'type': 'int',
                'reportable': true
            },
            19: {
                'attributeName': 'NumberOfRFIDUsersSupported',
                'attributeId': 19,
                'type': 'int',
                'reportable': true
            },
            20: {
                'attributeName': 'NumberOfWeekDaySchedulesSupportedPerUser',
                'attributeId': 20,
                'type': 'int',
                'reportable': true
            },
            21: {
                'attributeName': 'NumberOfYearDaySchedulesSupportedPerUser',
                'attributeId': 21,
                'type': 'int',
                'reportable': true
            },
            22: {
                'attributeName': 'NumberOfHolidaySchedulesSupported',
                'attributeId': 22,
                'type': 'int',
                'reportable': true
            },
            23: {
                'attributeName': 'MaxPINCodeLength',
                'attributeId': 23,
                'type': 'int',
                'reportable': true
            },
            24: {
                'attributeName': 'MinPINCodeLength',
                'attributeId': 24,
                'type': 'int',
                'reportable': true
            },
            25: {
                'attributeName': 'MaxRFIDCodeLength',
                'attributeId': 25,
                'type': 'int',
                'reportable': true
            },
            26: {
                'attributeName': 'MinRFIDCodeLength',
                'attributeId': 26,
                'type': 'int',
                'reportable': true
            },
            28: {
                'attributeName': 'NumberOfCredentialsSupportedPerUser',
                'attributeId': 28,
                'type': 'int',
                'reportable': true
            },
            33: {
                'attributeName': 'Language',
                'attributeId': 33,
                'type': 'str',
                'reportable': true,
                'writable': true
            },
            35: {
                'attributeName': 'AutoRelockTime',
                'attributeId': 35,
                'type': 'int',
                'reportable': true,
                'writable': true
            },
            36: {
                'attributeName': 'SoundVolume',
                'attributeId': 36,
                'type': 'int',
                'reportable': true,
                'writable': true
            },
            37: {
                'attributeName': 'OperatingMode',
                'attributeId': 37,
                'type': 'int',
                'reportable': true,
                'writable': true
            },
            38: {
                'attributeName': 'SupportedOperatingModes',
                'attributeId': 38,
                'type': 'int',
                'reportable': true
            },
            41: {
                'attributeName': 'EnableOneTouchLocking',
                'attributeId': 41,
                'type': 'bool',
                'reportable': true,
                'writable': true
            },
            43: {
                'attributeName': 'EnablePrivacyModeButton',
                'attributeId': 43,
                'type': 'bool',
                'reportable': true,
                'writable': true
            },
            48: {
                'attributeName': 'WrongCodeEntryLimit',
                'attributeId': 48,
                'type': 'int',
                'reportable': true,
                'writable': true
            },
            49: {
                'attributeName': 'UserCodeTemporaryDisableTime',
                'attributeId': 49,
                'type': 'int',
                'reportable': true,
                'writable': true
            },
            51: {
                'attributeName': 'RequirePINforRemoteOperation',
                'attributeId': 51,
                'type': 'bool',
                'writable': true
            },
            65528: {
                'attributeName': 'GeneratedCommandList',
                'attributeId': 65528,
                'type': 'int',
                'reportable': true
            },
            65529: {
                'attributeName': 'AcceptedCommandList',
                'attributeId': 65529,
                'type': 'int',
                'reportable': true
            },
            65531: {
                'attributeName': 'AttributeList',
                'attributeId': 65531,
                'type': 'int',
                'reportable': true
            },
            65532: {
                'attributeName': 'FeatureMap',
                'attributeId': 65532,
                'type': 'int',
                'reportable': true
            },
            65533: {
                'attributeName': 'ClusterRevision',
                'attributeId': 65533,
                'type': 'int',
                'reportable': true
            }
        }
    },
    'WindowCovering': {
        'clusterName': 'WindowCovering',
        'clusterId': 258,
        'commands': {
            0: {
                'commandId': 0,
                'commandName': 'UpOrOpen',
                'args': {}
            },
            1: {
                'commandId': 1,
                'commandName': 'DownOrClose',
                'args': {}
            },
            2: {
                'commandId': 2,
                'commandName': 'StopMotion',
                'args': {}
            },
            4: {
                'commandId': 4,
                'commandName': 'GoToLiftValue',
                'args': {
                    'liftValue': 'int'
                }
            },
            5: {
                'commandId': 5,
                'commandName': 'GoToLiftPercentage',
                'args': {
                    'liftPercent100thsValue': 'int'
                }
            },
            7: {
                'commandId': 7,
                'commandName': 'GoToTiltValue',
                'args': {
                    'tiltValue': 'int'
                }
            },
            8: {
                'commandId': 8,
                'commandName': 'GoToTiltPercentage',
                'args': {
                    'tiltPercent100thsValue': 'int'
                }
            }
        },
        'attributes': {
            0: {
                'attributeName': 'Type',
                'attributeId': 0,
                'type': 'int',
                'reportable': true
            },
            1: {
                'attributeName': 'PhysicalClosedLimitLift',
                'attributeId': 1,
                'type': 'int',
                'reportable': true
            },
            2: {
                'attributeName': 'PhysicalClosedLimitTilt',
                'attributeId': 2,
                'type': 'int',
                'reportable': true
            },
            3: {
                'attributeName': 'CurrentPositionLift',
                'attributeId': 3,
                'type': 'int',
                'reportable': true
            },
            4: {
                'attributeName': 'CurrentPositionTilt',
                'attributeId': 4,
                'type': 'int',
                'reportable': true
            },
            5: {
                'attributeName': 'NumberOfActuationsLift',
                'attributeId': 5,
                'type': 'int',
                'reportable': true
            },
            6: {
                'attributeName': 'NumberOfActuationsTilt',
                'attributeId': 6,
                'type': 'int',
                'reportable': true
            },
            7: {
                'attributeName': 'ConfigStatus',
                'attributeId': 7,
                'type': 'int',
                'reportable': true
            },
            8: {
                'attributeName': 'CurrentPositionLiftPercentage',
                'attributeId': 8,
                'type': 'int',
                'reportable': true
            },
            9: {
                'attributeName': 'CurrentPositionTiltPercentage',
                'attributeId': 9,
                'type': 'int',
                'reportable': true
            },
            10: {
                'attributeName': 'OperationalStatus',
                'attributeId': 10,
                'type': 'int',
                'reportable': true
            },
            11: {
                'attributeName': 'TargetPositionLiftPercent100ths',
                'attributeId': 11,
                'type': 'int',
                'reportable': true
            },
            12: {
                'attributeName': 'TargetPositionTiltPercent100ths',
                'attributeId': 12,
                'type': 'int',
                'reportable': true
            },
            13: {
                'attributeName': 'EndProductType',
                'attributeId': 13,
                'type': 'int',
                'reportable': true
            },
            14: {
                'attributeName': 'CurrentPositionLiftPercent100ths',
                'attributeId': 14,
                'type': 'int',
                'reportable': true
            },
            15: {
                'attributeName': 'CurrentPositionTiltPercent100ths',
                'attributeId': 15,
                'type': 'int',
                'reportable': true
            },
            16: {
                'attributeName': 'InstalledOpenLimitLift',
                'attributeId': 16,
                'type': 'int',
                'reportable': true
            },
            17: {
                'attributeName': 'InstalledClosedLimitLift',
                'attributeId': 17,
                'type': 'int',
                'reportable': true
            },
            18: {
                'attributeName': 'InstalledOpenLimitTilt',
                'attributeId': 18,
                'type': 'int',
                'reportable': true
            },
            19: {
                'attributeName': 'InstalledClosedLimitTilt',
                'attributeId': 19,
                'type': 'int',
                'reportable': true
            },
            23: {
                'attributeName': 'Mode',
                'attributeId': 23,
                'type': 'int',
                'reportable': true,
                'writable': true
            },
            26: {
                'attributeName': 'SafetyStatus',
                'attributeId': 26,
                'type': 'int',
                'reportable': true
            },
            65528: {
                'attributeName': 'GeneratedCommandList',
                'attributeId': 65528,
                'type': 'int',
                'reportable': true
            },
            65529: {
                'attributeName': 'AcceptedCommandList',
                'attributeId': 65529,
                'type': 'int',
                'reportable': true
            },
            65531: {
                'attributeName': 'AttributeList',
                'attributeId': 65531,
                'type': 'int',
                'reportable': true
            },
            65532: {
                'attributeName': 'FeatureMap',
                'attributeId': 65532,
                'type': 'int',
                'reportable': true
            },
            65533: {
                'attributeName': 'ClusterRevision',
                'attributeId': 65533,
                'type': 'int',
                'reportable': true
            }
        }
    },
    'BarrierControl': {
        'clusterName': 'BarrierControl',
        'clusterId': 259,
        'commands': {
            0: {
                'commandId': 0,
                'commandName': 'BarrierControlGoToPercent',
                'args': {
                    'percentOpen': 'int'
                }
            },
            1: {
                'commandId': 1,
                'commandName': 'BarrierControlStop',
                'args': {}
            }
        },
        'attributes': {
            1: {
                'attributeName': 'BarrierMovingState',
                'attributeId': 1,
                'type': 'int',
                'reportable': true
            },
            2: {
                'attributeName': 'BarrierSafetyStatus',
                'attributeId': 2,
                'type': 'int',
                'reportable': true
            },
            3: {
                'attributeName': 'BarrierCapabilities',
                'attributeId': 3,
                'type': 'int',
                'reportable': true
            },
            10: {
                'attributeName': 'BarrierPosition',
                'attributeId': 10,
                'type': 'int',
                'reportable': true
            },
            65528: {
                'attributeName': 'GeneratedCommandList',
                'attributeId': 65528,
                'type': 'int',
                'reportable': true
            },
            65529: {
                'attributeName': 'AcceptedCommandList',
                'attributeId': 65529,
                'type': 'int',
                'reportable': true
            },
            65531: {
                'attributeName': 'AttributeList',
                'attributeId': 65531,
                'type': 'int',
                'reportable': true
            },
            65532: {
                'attributeName': 'FeatureMap',
                'attributeId': 65532,
                'type': 'int',
                'reportable': true
            },
            65533: {
                'attributeName': 'ClusterRevision',
                'attributeId': 65533,
                'type': 'int',
                'reportable': true
            }
        }
    },
    'PumpConfigurationAndControl': {
        'clusterName': 'PumpConfigurationAndControl',
        'clusterId': 512,
        'commands': {},
        'attributes': {
            0: {
                'attributeName': 'MaxPressure',
                'attributeId': 0,
                'type': 'int',
                'reportable': true
            },
            1: {
                'attributeName': 'MaxSpeed',
                'attributeId': 1,
                'type': 'int',
                'reportable': true
            },
            2: {
                'attributeName': 'MaxFlow',
                'attributeId': 2,
                'type': 'int',
                'reportable': true
            },
            3: {
                'attributeName': 'MinConstPressure',
                'attributeId': 3,
                'type': 'int',
                'reportable': true
            },
            4: {
                'attributeName': 'MaxConstPressure',
                'attributeId': 4,
                'type': 'int',
                'reportable': true
            },
            5: {
                'attributeName': 'MinCompPressure',
                'attributeId': 5,
                'type': 'int',
                'reportable': true
            },
            6: {
                'attributeName': 'MaxCompPressure',
                'attributeId': 6,
                'type': 'int',
                'reportable': true
            },
            7: {
                'attributeName': 'MinConstSpeed',
                'attributeId': 7,
                'type': 'int',
                'reportable': true
            },
            8: {
                'attributeName': 'MaxConstSpeed',
                'attributeId': 8,
                'type': 'int',
                'reportable': true
            },
            9: {
                'attributeName': 'MinConstFlow',
                'attributeId': 9,
                'type': 'int',
                'reportable': true
            },
            10: {
                'attributeName': 'MaxConstFlow',
                'attributeId': 10,
                'type': 'int',
                'reportable': true
            },
            11: {
                'attributeName': 'MinConstTemp',
                'attributeId': 11,
                'type': 'int',
                'reportable': true
            },
            12: {
                'attributeName': 'MaxConstTemp',
                'attributeId': 12,
                'type': 'int',
                'reportable': true
            },
            16: {
                'attributeName': 'PumpStatus',
                'attributeId': 16,
                'type': 'int',
                'reportable': true
            },
            17: {
                'attributeName': 'EffectiveOperationMode',
                'attributeId': 17,
                'type': 'int',
                'reportable': true
            },
            18: {
                'attributeName': 'EffectiveControlMode',
                'attributeId': 18,
                'type': 'int',
                'reportable': true
            },
            19: {
                'attributeName': 'Capacity',
                'attributeId': 19,
                'type': 'int',
                'reportable': true
            },
            20: {
                'attributeName': 'Speed',
                'attributeId': 20,
                'type': 'int',
                'reportable': true
            },
            21: {
                'attributeName': 'LifetimeRunningHours',
                'attributeId': 21,
                'type': 'int',
                'reportable': true,
                'writable': true
            },
            22: {
                'attributeName': 'Power',
                'attributeId': 22,
                'type': 'int',
                'reportable': true
            },
            23: {
                'attributeName': 'LifetimeEnergyConsumed',
                'attributeId': 23,
                'type': 'int',
                'reportable': true,
                'writable': true
            },
            32: {
                'attributeName': 'OperationMode',
                'attributeId': 32,
                'type': 'int',
                'reportable': true,
                'writable': true
            },
            33: {
                'attributeName': 'ControlMode',
                'attributeId': 33,
                'type': 'int',
                'reportable': true,
                'writable': true
            },
            65528: {
                'attributeName': 'GeneratedCommandList',
                'attributeId': 65528,
                'type': 'int',
                'reportable': true
            },
            65529: {
                'attributeName': 'AcceptedCommandList',
                'attributeId': 65529,
                'type': 'int',
                'reportable': true
            },
            65531: {
                'attributeName': 'AttributeList',
                'attributeId': 65531,
                'type': 'int',
                'reportable': true
            },
            65532: {
                'attributeName': 'FeatureMap',
                'attributeId': 65532,
                'type': 'int',
                'reportable': true
            },
            65533: {
                'attributeName': 'ClusterRevision',
                'attributeId': 65533,
                'type': 'int',
                'reportable': true
            }
        }
    },
    'Thermostat': {
        'clusterName': 'Thermostat',
        'clusterId': 513,
        'commands': {
            0: {
                'commandId': 0,
                'commandName': 'SetpointRaiseLower',
                'args': {
                    'mode': 'int',
                    'amount': 'int'
                }
            },
            1: {
                'commandId': 1,
                'commandName': 'SetWeeklySchedule',
                'args': {
                    'numberOfTransitionsForSequence': 'int',
                    'dayOfWeekForSequence': 'int',
                    'modeForSequence': 'int',
                    'transitionTime': 'int',
                    'heatSetpoint': 'int',
                    'coolSetpoint': 'int'
                }
            },
            2: {
                'commandId': 2,
                'commandName': 'GetWeeklySchedule',
                'args': {
                    'daysToReturn': 'int',
                    'modeToReturn': 'int'
                }
            },
            3: {
                'commandId': 3,
                'commandName': 'ClearWeeklySchedule',
                'args': {}
            }
        },
        'attributes': {
            0: {
                'attributeName': 'LocalTemperature',
                'attributeId': 0,
                'type': 'int',
                'reportable': true
            },
            3: {
                'attributeName': 'AbsMinHeatSetpointLimit',
                'attributeId': 3,
                'type': 'int',
                'reportable': true
            },
            4: {
                'attributeName': 'AbsMaxHeatSetpointLimit',
                'attributeId': 4,
                'type': 'int',
                'reportable': true
            },
            5: {
                'attributeName': 'AbsMinCoolSetpointLimit',
                'attributeId': 5,
                'type': 'int',
                'reportable': true
            },
            6: {
                'attributeName': 'AbsMaxCoolSetpointLimit',
                'attributeId': 6,
                'type': 'int',
                'reportable': true
            },
            17: {
                'attributeName': 'OccupiedCoolingSetpoint',
                'attributeId': 17,
                'type': 'int',
                'reportable': true,
                'writable': true
            },
            18: {
                'attributeName': 'OccupiedHeatingSetpoint',
                'attributeId': 18,
                'type': 'int',
                'reportable': true,
                'writable': true
            },
            21: {
                'attributeName': 'MinHeatSetpointLimit',
                'attributeId': 21,
                'type': 'int',
                'reportable': true,
                'writable': true
            },
            22: {
                'attributeName': 'MaxHeatSetpointLimit',
                'attributeId': 22,
                'type': 'int',
                'reportable': true,
                'writable': true
            },
            23: {
                'attributeName': 'MinCoolSetpointLimit',
                'attributeId': 23,
                'type': 'int',
                'reportable': true,
                'writable': true
            },
            24: {
                'attributeName': 'MaxCoolSetpointLimit',
                'attributeId': 24,
                'type': 'int',
                'reportable': true,
                'writable': true
            },
            25: {
                'attributeName': 'MinSetpointDeadBand',
                'attributeId': 25,
                'type': 'int',
                'reportable': true,
                'writable': true
            },
            27: {
                'attributeName': 'ControlSequenceOfOperation',
                'attributeId': 27,
                'type': 'int',
                'reportable': true,
                'writable': true
            },
            28: {
                'attributeName': 'SystemMode',
                'attributeId': 28,
                'type': 'int',
                'reportable': true,
                'writable': true
            },
            32: {
                'attributeName': 'StartOfWeek',
                'attributeId': 32,
                'type': 'int',
                'reportable': true
            },
            33: {
                'attributeName': 'NumberOfWeeklyTransitions',
                'attributeId': 33,
                'type': 'int',
                'reportable': true
            },
            34: {
                'attributeName': 'NumberOfDailyTransitions',
                'attributeId': 34,
                'type': 'int',
                'reportable': true
            },
            65531: {
                'attributeName': 'AttributeList',
                'attributeId': 65531,
                'type': 'int',
                'reportable': true
            },
            65532: {
                'attributeName': 'FeatureMap',
                'attributeId': 65532,
                'type': 'int',
                'reportable': true
            },
            65533: {
                'attributeName': 'ClusterRevision',
                'attributeId': 65533,
                'type': 'int',
                'reportable': true
            }
        }
    },
    'FanControl': {
        'clusterName': 'FanControl',
        'clusterId': 514,
        'commands': {},
        'attributes': {
            0: {
                'attributeName': 'FanMode',
                'attributeId': 0,
                'type': 'int',
                'reportable': true,
                'writable': true
            },
            1: {
                'attributeName': 'FanModeSequence',
                'attributeId': 1,
                'type': 'int',
                'reportable': true,
                'writable': true
            },
            2: {
                'attributeName': 'PercentSetting',
                'attributeId': 2,
                'type': 'int',
                'reportable': true,
                'writable': true
            },
            3: {
                'attributeName': 'PercentCurrent',
                'attributeId': 3,
                'type': 'int',
                'reportable': true
            },
            4: {
                'attributeName': 'SpeedMax',
                'attributeId': 4,
                'type': 'int',
                'reportable': true
            },
            5: {
                'attributeName': 'SpeedSetting',
                'attributeId': 5,
                'type': 'int',
                'reportable': true,
                'writable': true
            },
            6: {
                'attributeName': 'SpeedCurrent',
                'attributeId': 6,
                'type': 'int',
                'reportable': true
            },
            7: {
                'attributeName': 'RockSupport',
                'attributeId': 7,
                'type': 'int',
                'reportable': true
            },
            8: {
                'attributeName': 'RockSetting',
                'attributeId': 8,
                'type': 'int',
                'reportable': true,
                'writable': true
            },
            9: {
                'attributeName': 'WindSupport',
                'attributeId': 9,
                'type': 'int',
                'reportable': true
            },
            10: {
                'attributeName': 'WindSetting',
                'attributeId': 10,
                'type': 'int',
                'reportable': true,
                'writable': true
            },
            65528: {
                'attributeName': 'GeneratedCommandList',
                'attributeId': 65528,
                'type': 'int',
                'reportable': true
            },
            65529: {
                'attributeName': 'AcceptedCommandList',
                'attributeId': 65529,
                'type': 'int',
                'reportable': true
            },
            65531: {
                'attributeName': 'AttributeList',
                'attributeId': 65531,
                'type': 'int',
                'reportable': true
            },
            65532: {
                'attributeName': 'FeatureMap',
                'attributeId': 65532,
                'type': 'int',
                'reportable': true
            },
            65533: {
                'attributeName': 'ClusterRevision',
                'attributeId': 65533,
                'type': 'int',
                'reportable': true
            }
        }
    },
    'ThermostatUserInterfaceConfiguration': {
        'clusterName': 'ThermostatUserInterfaceConfiguration',
        'clusterId': 516,
        'commands': {},
        'attributes': {
            0: {
                'attributeName': 'TemperatureDisplayMode',
                'attributeId': 0,
                'type': 'int',
                'reportable': true,
                'writable': true
            },
            1: {
                'attributeName': 'KeypadLockout',
                'attributeId': 1,
                'type': 'int',
                'reportable': true,
                'writable': true
            },
            2: {
                'attributeName': 'ScheduleProgrammingVisibility',
                'attributeId': 2,
                'type': 'int',
                'reportable': true,
                'writable': true
            },
            65528: {
                'attributeName': 'GeneratedCommandList',
                'attributeId': 65528,
                'type': 'int',
                'reportable': true
            },
            65529: {
                'attributeName': 'AcceptedCommandList',
                'attributeId': 65529,
                'type': 'int',
                'reportable': true
            },
            65531: {
                'attributeName': 'AttributeList',
                'attributeId': 65531,
                'type': 'int',
                'reportable': true
            },
            65532: {
                'attributeName': 'FeatureMap',
                'attributeId': 65532,
                'type': 'int',
                'reportable': true
            },
            65533: {
                'attributeName': 'ClusterRevision',
                'attributeId': 65533,
                'type': 'int',
                'reportable': true
            }
        }
    },
    'ColorControl': {
        'clusterName': 'ColorControl',
        'clusterId': 768,
        'commands': {
            0: {
                'commandId': 0,
                'commandName': 'MoveToHue',
                'args': {
                    'hue': 'int',
                    'direction': 'int',
                    'transitionTime': 'int',
                    'optionsMask': 'int',
                    'optionsOverride': 'int'
                }
            },
            1: {
                'commandId': 1,
                'commandName': 'MoveHue',
                'args': {
                    'moveMode': 'int',
                    'rate': 'int',
                    'optionsMask': 'int',
                    'optionsOverride': 'int'
                }
            },
            2: {
                'commandId': 2,
                'commandName': 'StepHue',
                'args': {
                    'stepMode': 'int',
                    'stepSize': 'int',
                    'transitionTime': 'int',
                    'optionsMask': 'int',
                    'optionsOverride': 'int'
                }
            },
            3: {
                'commandId': 3,
                'commandName': 'MoveToSaturation',
                'args': {
                    'saturation': 'int',
                    'transitionTime': 'int',
                    'optionsMask': 'int',
                    'optionsOverride': 'int'
                }
            },
            4: {
                'commandId': 4,
                'commandName': 'MoveSaturation',
                'args': {
                    'moveMode': 'int',
                    'rate': 'int',
                    'optionsMask': 'int',
                    'optionsOverride': 'int'
                }
            },
            5: {
                'commandId': 5,
                'commandName': 'StepSaturation',
                'args': {
                    'stepMode': 'int',
                    'stepSize': 'int',
                    'transitionTime': 'int',
                    'optionsMask': 'int',
                    'optionsOverride': 'int'
                }
            },
            6: {
                'commandId': 6,
                'commandName': 'MoveToHueAndSaturation',
                'args': {
                    'hue': 'int',
                    'saturation': 'int',
                    'transitionTime': 'int',
                    'optionsMask': 'int',
                    'optionsOverride': 'int'
                }
            },
            7: {
                'commandId': 7,
                'commandName': 'MoveToColor',
                'args': {
                    'colorX': 'int',
                    'colorY': 'int',
                    'transitionTime': 'int',
                    'optionsMask': 'int',
                    'optionsOverride': 'int'
                }
            },
            8: {
                'commandId': 8,
                'commandName': 'MoveColor',
                'args': {
                    'rateX': 'int',
                    'rateY': 'int',
                    'optionsMask': 'int',
                    'optionsOverride': 'int'
                }
            },
            9: {
                'commandId': 9,
                'commandName': 'StepColor',
                'args': {
                    'stepX': 'int',
                    'stepY': 'int',
                    'transitionTime': 'int',
                    'optionsMask': 'int',
                    'optionsOverride': 'int'
                }
            },
            10: {
                'commandId': 10,
                'commandName': 'MoveToColorTemperature',
                'args': {
                    'colorTemperature': 'int',
                    'transitionTime': 'int',
                    'optionsMask': 'int',
                    'optionsOverride': 'int'
                }
            },
            64: {
                'commandId': 64,
                'commandName': 'EnhancedMoveToHue',
                'args': {
                    'enhancedHue': 'int',
                    'direction': 'int',
                    'transitionTime': 'int',
                    'optionsMask': 'int',
                    'optionsOverride': 'int'
                }
            },
            65: {
                'commandId': 65,
                'commandName': 'EnhancedMoveHue',
                'args': {
                    'moveMode': 'int',
                    'rate': 'int',
                    'optionsMask': 'int',
                    'optionsOverride': 'int'
                }
            },
            66: {
                'commandId': 66,
                'commandName': 'EnhancedStepHue',
                'args': {
                    'stepMode': 'int',
                    'stepSize': 'int',
                    'transitionTime': 'int',
                    'optionsMask': 'int',
                    'optionsOverride': 'int'
                }
            },
            67: {
                'commandId': 67,
                'commandName': 'EnhancedMoveToHueAndSaturation',
                'args': {
                    'enhancedHue': 'int',
                    'saturation': 'int',
                    'transitionTime': 'int',
                    'optionsMask': 'int',
                    'optionsOverride': 'int'
                }
            },
            68: {
                'commandId': 68,
                'commandName': 'ColorLoopSet',
                'args': {
                    'updateFlags': 'int',
                    'action': 'int',
                    'direction': 'int',
                    'time': 'int',
                    'startHue': 'int',
                    'optionsMask': 'int',
                    'optionsOverride': 'int'
                }
            },
            71: {
                'commandId': 71,
                'commandName': 'StopMoveStep',
                'args': {
                    'optionsMask': 'int',
                    'optionsOverride': 'int'
                }
            },
            75: {
                'commandId': 75,
                'commandName': 'MoveColorTemperature',
                'args': {
                    'moveMode': 'int',
                    'rate': 'int',
                    'colorTemperatureMinimumMireds': 'int',
                    'colorTemperatureMaximumMireds': 'int',
                    'optionsMask': 'int',
                    'optionsOverride': 'int'
                }
            },
            76: {
                'commandId': 76,
                'commandName': 'StepColorTemperature',
                'args': {
                    'stepMode': 'int',
                    'stepSize': 'int',
                    'transitionTime': 'int',
                    'colorTemperatureMinimumMireds': 'int',
                    'colorTemperatureMaximumMireds': 'int',
                    'optionsMask': 'int',
                    'optionsOverride': 'int'
                }
            }
        },
        'attributes': {
            0: {
                'attributeName': 'CurrentHue',
                'attributeId': 0,
                'type': 'int',
                'reportable': true
            },
            1: {
                'attributeName': 'CurrentSaturation',
                'attributeId': 1,
                'type': 'int',
                'reportable': true
            },
            2: {
                'attributeName': 'RemainingTime',
                'attributeId': 2,
                'type': 'int',
                'reportable': true
            },
            3: {
                'attributeName': 'CurrentX',
                'attributeId': 3,
                'type': 'int',
                'reportable': true
            },
            4: {
                'attributeName': 'CurrentY',
                'attributeId': 4,
                'type': 'int',
                'reportable': true
            },
            5: {
                'attributeName': 'DriftCompensation',
                'attributeId': 5,
                'type': 'int',
                'reportable': true
            },
            6: {
                'attributeName': 'CompensationText',
                'attributeId': 6,
                'type': 'str',
                'reportable': true
            },
            7: {
                'attributeName': 'ColorTemperatureMireds',
                'attributeId': 7,
                'type': 'int',
                'reportable': true
            },
            8: {
                'attributeName': 'ColorMode',
                'attributeId': 8,
                'type': 'int',
                'reportable': true
            },
            15: {
                'attributeName': 'Options',
                'attributeId': 15,
                'type': 'int',
                'reportable': true,
                'writable': true
            },
            16: {
                'attributeName': 'NumberOfPrimaries',
                'attributeId': 16,
                'type': 'int',
                'reportable': true
            },
            17: {
                'attributeName': 'Primary1X',
                'attributeId': 17,
                'type': 'int',
                'reportable': true
            },
            18: {
                'attributeName': 'Primary1Y',
                'attributeId': 18,
                'type': 'int',
                'reportable': true
            },
            19: {
                'attributeName': 'Primary1Intensity',
                'attributeId': 19,
                'type': 'int',
                'reportable': true
            },
            21: {
                'attributeName': 'Primary2X',
                'attributeId': 21,
                'type': 'int',
                'reportable': true
            },
            22: {
                'attributeName': 'Primary2Y',
                'attributeId': 22,
                'type': 'int',
                'reportable': true
            },
            23: {
                'attributeName': 'Primary2Intensity',
                'attributeId': 23,
                'type': 'int',
                'reportable': true
            },
            25: {
                'attributeName': 'Primary3X',
                'attributeId': 25,
                'type': 'int',
                'reportable': true
            },
            26: {
                'attributeName': 'Primary3Y',
                'attributeId': 26,
                'type': 'int',
                'reportable': true
            },
            27: {
                'attributeName': 'Primary3Intensity',
                'attributeId': 27,
                'type': 'int',
                'reportable': true
            },
            32: {
                'attributeName': 'Primary4X',
                'attributeId': 32,
                'type': 'int',
                'reportable': true
            },
            33: {
                'attributeName': 'Primary4Y',
                'attributeId': 33,
                'type': 'int',
                'reportable': true
            },
            34: {
                'attributeName': 'Primary4Intensity',
                'attributeId': 34,
                'type': 'int',
                'reportable': true
            },
            36: {
                'attributeName': 'Primary5X',
                'attributeId': 36,
                'type': 'int',
                'reportable': true
            },
            37: {
                'attributeName': 'Primary5Y',
                'attributeId': 37,
                'type': 'int',
                'reportable': true
            },
            38: {
                'attributeName': 'Primary5Intensity',
                'attributeId': 38,
                'type': 'int',
                'reportable': true
            },
            40: {
                'attributeName': 'Primary6X',
                'attributeId': 40,
                'type': 'int',
                'reportable': true
            },
            41: {
                'attributeName': 'Primary6Y',
                'attributeId': 41,
                'type': 'int',
                'reportable': true
            },
            42: {
                'attributeName': 'Primary6Intensity',
                'attributeId': 42,
                'type': 'int',
                'reportable': true
            },
            48: {
                'attributeName': 'WhitePointX',
                'attributeId': 48,
                'type': 'int',
                'reportable': true,
                'writable': true
            },
            49: {
                'attributeName': 'WhitePointY',
                'attributeId': 49,
                'type': 'int',
                'reportable': true,
                'writable': true
            },
            50: {
                'attributeName': 'ColorPointRX',
                'attributeId': 50,
                'type': 'int',
                'reportable': true,
                'writable': true
            },
            51: {
                'attributeName': 'ColorPointRY',
                'attributeId': 51,
                'type': 'int',
                'reportable': true,
                'writable': true
            },
            52: {
                'attributeName': 'ColorPointRIntensity',
                'attributeId': 52,
                'type': 'int',
                'reportable': true,
                'writable': true
            },
            54: {
                'attributeName': 'ColorPointGX',
                'attributeId': 54,
                'type': 'int',
                'reportable': true,
                'writable': true
            },
            55: {
                'attributeName': 'ColorPointGY',
                'attributeId': 55,
                'type': 'int',
                'reportable': true,
                'writable': true
            },
            56: {
                'attributeName': 'ColorPointGIntensity',
                'attributeId': 56,
                'type': 'int',
                'reportable': true,
                'writable': true
            },
            58: {
                'attributeName': 'ColorPointBX',
                'attributeId': 58,
                'type': 'int',
                'reportable': true,
                'writable': true
            },
            59: {
                'attributeName': 'ColorPointBY',
                'attributeId': 59,
                'type': 'int',
                'reportable': true,
                'writable': true
            },
            60: {
                'attributeName': 'ColorPointBIntensity',
                'attributeId': 60,
                'type': 'int',
                'reportable': true,
                'writable': true
            },
            16384: {
                'attributeName': 'EnhancedCurrentHue',
                'attributeId': 16384,
                'type': 'int',
                'reportable': true
            },
            16385: {
                'attributeName': 'EnhancedColorMode',
                'attributeId': 16385,
                'type': 'int',
                'reportable': true
            },
            16386: {
                'attributeName': 'ColorLoopActive',
                'attributeId': 16386,
                'type': 'int',
                'reportable': true
            },
            16387: {
                'attributeName': 'ColorLoopDirection',
                'attributeId': 16387,
                'type': 'int',
                'reportable': true
            },
            16388: {
                'attributeName': 'ColorLoopTime',
                'attributeId': 16388,
                'type': 'int',
                'reportable': true
            },
            16389: {
                'attributeName': 'ColorLoopStartEnhancedHue',
                'attributeId': 16389,
                'type': 'int',
                'reportable': true
            },
            16390: {
                'attributeName': 'ColorLoopStoredEnhancedHue',
                'attributeId': 16390,
                'type': 'int',
                'reportable': true
            },
            16394: {
                'attributeName': 'ColorCapabilities',
                'attributeId': 16394,
                'type': 'int',
                'reportable': true
            },
            16395: {
                'attributeName': 'ColorTempPhysicalMinMireds',
                'attributeId': 16395,
                'type': 'int',
                'reportable': true
            },
            16396: {
                'attributeName': 'ColorTempPhysicalMaxMireds',
                'attributeId': 16396,
                'type': 'int',
                'reportable': true
            },
            16397: {
                'attributeName': 'CoupleColorTempToLevelMinMireds',
                'attributeId': 16397,
                'type': 'int',
                'reportable': true
            },
            16400: {
                'attributeName': 'StartUpColorTemperatureMireds',
                'attributeId': 16400,
                'type': 'int',
                'reportable': true,
                'writable': true
            },
            65528: {
                'attributeName': 'GeneratedCommandList',
                'attributeId': 65528,
                'type': 'int',
                'reportable': true
            },
            65529: {
                'attributeName': 'AcceptedCommandList',
                'attributeId': 65529,
                'type': 'int',
                'reportable': true
            },
            65531: {
                'attributeName': 'AttributeList',
                'attributeId': 65531,
                'type': 'int',
                'reportable': true
            },
            65532: {
                'attributeName': 'FeatureMap',
                'attributeId': 65532,
                'type': 'int',
                'reportable': true
            },
            65533: {
                'attributeName': 'ClusterRevision',
                'attributeId': 65533,
                'type': 'int',
                'reportable': true
            }
        }
    },
    'BallastConfiguration': {
        'clusterName': 'BallastConfiguration',
        'clusterId': 769,
        'commands': {},
        'attributes': {
            0: {
                'attributeName': 'PhysicalMinLevel',
                'attributeId': 0,
                'type': 'int',
                'reportable': true
            },
            1: {
                'attributeName': 'PhysicalMaxLevel',
                'attributeId': 1,
                'type': 'int',
                'reportable': true
            },
            2: {
                'attributeName': 'BallastStatus',
                'attributeId': 2,
                'type': 'int',
                'reportable': true
            },
            16: {
                'attributeName': 'MinLevel',
                'attributeId': 16,
                'type': 'int',
                'reportable': true,
                'writable': true
            },
            17: {
                'attributeName': 'MaxLevel',
                'attributeId': 17,
                'type': 'int',
                'reportable': true,
                'writable': true
            },
            20: {
                'attributeName': 'IntrinsicBalanceFactor',
                'attributeId': 20,
                'type': 'int',
                'reportable': true,
                'writable': true
            },
            21: {
                'attributeName': 'BallastFactorAdjustment',
                'attributeId': 21,
                'type': 'int',
                'reportable': true,
                'writable': true
            },
            32: {
                'attributeName': 'LampQuantity',
                'attributeId': 32,
                'type': 'int',
                'reportable': true
            },
            48: {
                'attributeName': 'LampType',
                'attributeId': 48,
                'type': 'str',
                'reportable': true,
                'writable': true
            },
            49: {
                'attributeName': 'LampManufacturer',
                'attributeId': 49,
                'type': 'str',
                'reportable': true,
                'writable': true
            },
            50: {
                'attributeName': 'LampRatedHours',
                'attributeId': 50,
                'type': 'int',
                'reportable': true,
                'writable': true
            },
            51: {
                'attributeName': 'LampBurnHours',
                'attributeId': 51,
                'type': 'int',
                'reportable': true,
                'writable': true
            },
            52: {
                'attributeName': 'LampAlarmMode',
                'attributeId': 52,
                'type': 'int',
                'reportable': true,
                'writable': true
            },
            53: {
                'attributeName': 'LampBurnHoursTripPoint',
                'attributeId': 53,
                'type': 'int',
                'reportable': true,
                'writable': true
            },
            65528: {
                'attributeName': 'GeneratedCommandList',
                'attributeId': 65528,
                'type': 'int',
                'reportable': true
            },
            65529: {
                'attributeName': 'AcceptedCommandList',
                'attributeId': 65529,
                'type': 'int',
                'reportable': true
            },
            65531: {
                'attributeName': 'AttributeList',
                'attributeId': 65531,
                'type': 'int',
                'reportable': true
            },
            65532: {
                'attributeName': 'FeatureMap',
                'attributeId': 65532,
                'type': 'int',
                'reportable': true
            },
            65533: {
                'attributeName': 'ClusterRevision',
                'attributeId': 65533,
                'type': 'int',
                'reportable': true
            }
        }
    },
    'IlluminanceMeasurement': {
        'clusterName': 'IlluminanceMeasurement',
        'clusterId': 1024,
        'commands': {},
        'attributes': {
            0: {
                'attributeName': 'MeasuredValue',
                'attributeId': 0,
                'type': 'int',
                'reportable': true
            },
            1: {
                'attributeName': 'MinMeasuredValue',
                'attributeId': 1,
                'type': 'int',
                'reportable': true
            },
            2: {
                'attributeName': 'MaxMeasuredValue',
                'attributeId': 2,
                'type': 'int',
                'reportable': true
            },
            3: {
                'attributeName': 'Tolerance',
                'attributeId': 3,
                'type': 'int',
                'reportable': true
            },
            4: {
                'attributeName': 'LightSensorType',
                'attributeId': 4,
                'type': 'int',
                'reportable': true
            },
            65528: {
                'attributeName': 'GeneratedCommandList',
                'attributeId': 65528,
                'type': 'int',
                'reportable': true
            },
            65529: {
                'attributeName': 'AcceptedCommandList',
                'attributeId': 65529,
                'type': 'int',
                'reportable': true
            },
            65531: {
                'attributeName': 'AttributeList',
                'attributeId': 65531,
                'type': 'int',
                'reportable': true
            },
            65532: {
                'attributeName': 'FeatureMap',
                'attributeId': 65532,
                'type': 'int',
                'reportable': true
            },
            65533: {
                'attributeName': 'ClusterRevision',
                'attributeId': 65533,
                'type': 'int',
                'reportable': true
            }
        }
    },
    'TemperatureMeasurement': {
        'clusterName': 'TemperatureMeasurement',
        'clusterId': 1026,
        'commands': {},
        'attributes': {
            0: {
                'attributeName': 'MeasuredValue',
                'attributeId': 0,
                'type': 'int',
                'reportable': true
            },
            1: {
                'attributeName': 'MinMeasuredValue',
                'attributeId': 1,
                'type': 'int',
                'reportable': true
            },
            2: {
                'attributeName': 'MaxMeasuredValue',
                'attributeId': 2,
                'type': 'int',
                'reportable': true
            },
            3: {
                'attributeName': 'Tolerance',
                'attributeId': 3,
                'type': 'int',
                'reportable': true
            },
            65531: {
                'attributeName': 'AttributeList',
                'attributeId': 65531,
                'type': 'int',
                'reportable': true
            },
            65532: {
                'attributeName': 'FeatureMap',
                'attributeId': 65532,
                'type': 'int',
                'reportable': true
            },
            65533: {
                'attributeName': 'ClusterRevision',
                'attributeId': 65533,
                'type': 'int',
                'reportable': true
            }
        }
    },
    'PressureMeasurement': {
        'clusterName': 'PressureMeasurement',
        'clusterId': 1027,
        'commands': {},
        'attributes': {
            0: {
                'attributeName': 'MeasuredValue',
                'attributeId': 0,
                'type': 'int',
                'reportable': true
            },
            1: {
                'attributeName': 'MinMeasuredValue',
                'attributeId': 1,
                'type': 'int',
                'reportable': true
            },
            2: {
                'attributeName': 'MaxMeasuredValue',
                'attributeId': 2,
                'type': 'int',
                'reportable': true
            },
            3: {
                'attributeName': 'Tolerance',
                'attributeId': 3,
                'type': 'int',
                'reportable': true
            },
            16: {
                'attributeName': 'ScaledValue',
                'attributeId': 16,
                'type': 'int',
                'reportable': true
            },
            17: {
                'attributeName': 'MinScaledValue',
                'attributeId': 17,
                'type': 'int',
                'reportable': true
            },
            18: {
                'attributeName': 'MaxScaledValue',
                'attributeId': 18,
                'type': 'int',
                'reportable': true
            },
            19: {
                'attributeName': 'ScaledTolerance',
                'attributeId': 19,
                'type': 'int',
                'reportable': true
            },
            20: {
                'attributeName': 'Scale',
                'attributeId': 20,
                'type': 'int',
                'reportable': true
            },
            65531: {
                'attributeName': 'AttributeList',
                'attributeId': 65531,
                'type': 'int',
                'reportable': true
            },
            65532: {
                'attributeName': 'FeatureMap',
                'attributeId': 65532,
                'type': 'int',
                'reportable': true
            },
            65533: {
                'attributeName': 'ClusterRevision',
                'attributeId': 65533,
                'type': 'int',
                'reportable': true
            }
        }
    },
    'FlowMeasurement': {
        'clusterName': 'FlowMeasurement',
        'clusterId': 1028,
        'commands': {},
        'attributes': {
            0: {
                'attributeName': 'MeasuredValue',
                'attributeId': 0,
                'type': 'int',
                'reportable': true
            },
            1: {
                'attributeName': 'MinMeasuredValue',
                'attributeId': 1,
                'type': 'int',
                'reportable': true
            },
            2: {
                'attributeName': 'MaxMeasuredValue',
                'attributeId': 2,
                'type': 'int',
                'reportable': true
            },
            3: {
                'attributeName': 'Tolerance',
                'attributeId': 3,
                'type': 'int',
                'reportable': true
            },
            65528: {
                'attributeName': 'GeneratedCommandList',
                'attributeId': 65528,
                'type': 'int',
                'reportable': true
            },
            65529: {
                'attributeName': 'AcceptedCommandList',
                'attributeId': 65529,
                'type': 'int',
                'reportable': true
            },
            65531: {
                'attributeName': 'AttributeList',
                'attributeId': 65531,
                'type': 'int',
                'reportable': true
            },
            65532: {
                'attributeName': 'FeatureMap',
                'attributeId': 65532,
                'type': 'int',
                'reportable': true
            },
            65533: {
                'attributeName': 'ClusterRevision',
                'attributeId': 65533,
                'type': 'int',
                'reportable': true
            }
        }
    },
    'RelativeHumidityMeasurement': {
        'clusterName': 'RelativeHumidityMeasurement',
        'clusterId': 1029,
        'commands': {},
        'attributes': {
            0: {
                'attributeName': 'MeasuredValue',
                'attributeId': 0,
                'type': 'int',
                'reportable': true
            },
            1: {
                'attributeName': 'MinMeasuredValue',
                'attributeId': 1,
                'type': 'int',
                'reportable': true
            },
            2: {
                'attributeName': 'MaxMeasuredValue',
                'attributeId': 2,
                'type': 'int',
                'reportable': true
            },
            3: {
                'attributeName': 'Tolerance',
                'attributeId': 3,
                'type': 'int',
                'reportable': true
            },
            65528: {
                'attributeName': 'GeneratedCommandList',
                'attributeId': 65528,
                'type': 'int',
                'reportable': true
            },
            65529: {
                'attributeName': 'AcceptedCommandList',
                'attributeId': 65529,
                'type': 'int',
                'reportable': true
            },
            65531: {
                'attributeName': 'AttributeList',
                'attributeId': 65531,
                'type': 'int',
                'reportable': true
            },
            65532: {
                'attributeName': 'FeatureMap',
                'attributeId': 65532,
                'type': 'int',
                'reportable': true
            },
            65533: {
                'attributeName': 'ClusterRevision',
                'attributeId': 65533,
                'type': 'int',
                'reportable': true
            }
        }
    },
    'OccupancySensing': {
        'clusterName': 'OccupancySensing',
        'clusterId': 1030,
        'commands': {},
        'attributes': {
            0: {
                'attributeName': 'Occupancy',
                'attributeId': 0,
                'type': 'int',
                'reportable': true
            },
            1: {
                'attributeName': 'OccupancySensorType',
                'attributeId': 1,
                'type': 'int',
                'reportable': true
            },
            2: {
                'attributeName': 'OccupancySensorTypeBitmap',
                'attributeId': 2,
                'type': 'int',
                'reportable': true
            },
            65528: {
                'attributeName': 'GeneratedCommandList',
                'attributeId': 65528,
                'type': 'int',
                'reportable': true
            },
            65529: {
                'attributeName': 'AcceptedCommandList',
                'attributeId': 65529,
                'type': 'int',
                'reportable': true
            },
            65531: {
                'attributeName': 'AttributeList',
                'attributeId': 65531,
                'type': 'int',
                'reportable': true
            },
            65532: {
                'attributeName': 'FeatureMap',
                'attributeId': 65532,
                'type': 'int',
                'reportable': true
            },
            65533: {
                'attributeName': 'ClusterRevision',
                'attributeId': 65533,
                'type': 'int',
                'reportable': true
            }
        }
    },
    'WakeOnLan': {
        'clusterName': 'WakeOnLan',
        'clusterId': 1283,
        'commands': {},
        'attributes': {
            0: {
                'attributeName': 'MACAddress',
                'attributeId': 0,
                'type': 'str',
                'reportable': true
            },
            65528: {
                'attributeName': 'GeneratedCommandList',
                'attributeId': 65528,
                'type': 'int',
                'reportable': true
            },
            65529: {
                'attributeName': 'AcceptedCommandList',
                'attributeId': 65529,
                'type': 'int',
                'reportable': true
            },
            65531: {
                'attributeName': 'AttributeList',
                'attributeId': 65531,
                'type': 'int',
                'reportable': true
            },
            65532: {
                'attributeName': 'FeatureMap',
                'attributeId': 65532,
                'type': 'int',
                'reportable': true
            },
            65533: {
                'attributeName': 'ClusterRevision',
                'attributeId': 65533,
                'type': 'int',
                'reportable': true
            }
        }
    },
    'Channel': {
        'clusterName': 'Channel',
        'clusterId': 1284,
        'commands': {
            0: {
                'commandId': 0,
                'commandName': 'ChangeChannel',
                'args': {
                    'match': 'str'
                }
            },
            2: {
                'commandId': 2,
                'commandName': 'ChangeChannelByNumber',
                'args': {
                    'majorNumber': 'int',
                    'minorNumber': 'int'
                }
            },
            3: {
                'commandId': 3,
                'commandName': 'SkipChannel',
                'args': {
                    'count': 'int'
                }
            }
        },
        'attributes': {
            0: {
                'attributeName': 'ChannelList',
                'attributeId': 0,
                'type': '',
                'reportable': true
            },
            1: {
                'attributeName': 'Lineup',
                'attributeId': 1,
                'type': '',
                'reportable': true
            },
            2: {
                'attributeName': 'CurrentChannel',
                'attributeId': 2,
                'type': '',
                'reportable': true
            },
            65528: {
                'attributeName': 'GeneratedCommandList',
                'attributeId': 65528,
                'type': 'int',
                'reportable': true
            },
            65529: {
                'attributeName': 'AcceptedCommandList',
                'attributeId': 65529,
                'type': 'int',
                'reportable': true
            },
            65531: {
                'attributeName': 'AttributeList',
                'attributeId': 65531,
                'type': 'int',
                'reportable': true
            },
            65532: {
                'attributeName': 'FeatureMap',
                'attributeId': 65532,
                'type': 'int',
                'reportable': true
            },
            65533: {
                'attributeName': 'ClusterRevision',
                'attributeId': 65533,
                'type': 'int',
                'reportable': true
            }
        }
    },
    'TargetNavigator': {
        'clusterName': 'TargetNavigator',
        'clusterId': 1285,
        'commands': {
            0: {
                'commandId': 0,
                'commandName': 'NavigateTarget',
                'args': {
                    'target': 'int',
                    'data': 'str'
                }
            }
        },
        'attributes': {
            0: {
                'attributeName': 'TargetList',
                'attributeId': 0,
                'type': '',
                'reportable': true
            },
            1: {
                'attributeName': 'CurrentTarget',
                'attributeId': 1,
                'type': 'int',
                'reportable': true
            },
            65528: {
                'attributeName': 'GeneratedCommandList',
                'attributeId': 65528,
                'type': 'int',
                'reportable': true
            },
            65529: {
                'attributeName': 'AcceptedCommandList',
                'attributeId': 65529,
                'type': 'int',
                'reportable': true
            },
            65531: {
                'attributeName': 'AttributeList',
                'attributeId': 65531,
                'type': 'int',
                'reportable': true
            },
            65532: {
                'attributeName': 'FeatureMap',
                'attributeId': 65532,
                'type': 'int',
                'reportable': true
            },
            65533: {
                'attributeName': 'ClusterRevision',
                'attributeId': 65533,
                'type': 'int',
                'reportable': true
            }
        }
    },
    'MediaPlayback': {
        'clusterName': 'MediaPlayback',
        'clusterId': 1286,
        'commands': {
            0: {
                'commandId': 0,
                'commandName': 'Play',
                'args': {}
            },
            1: {
                'commandId': 1,
                'commandName': 'Pause',
                'args': {}
            },
            2: {
                'commandId': 2,
                'commandName': 'StopPlayback',
                'args': {}
            },
            3: {
                'commandId': 3,
                'commandName': 'StartOver',
                'args': {}
            },
            4: {
                'commandId': 4,
                'commandName': 'Previous',
                'args': {}
            },
            5: {
                'commandId': 5,
                'commandName': 'Next',
                'args': {}
            },
            6: {
                'commandId': 6,
                'commandName': 'Rewind',
                'args': {}
            },
            7: {
                'commandId': 7,
                'commandName': 'FastForward',
                'args': {}
            },
            8: {
                'commandId': 8,
                'commandName': 'SkipForward',
                'args': {
                    'deltaPositionMilliseconds': 'int'
                }
            },
            9: {
                'commandId': 9,
                'commandName': 'SkipBackward',
                'args': {
                    'deltaPositionMilliseconds': 'int'
                }
            },
            11: {
                'commandId': 11,
                'commandName': 'Seek',
                'args': {
                    'position': 'int'
                }
            }
        },
        'attributes': {
            0: {
                'attributeName': 'CurrentState',
                'attributeId': 0,
                'type': 'int',
                'reportable': true
            },
            1: {
                'attributeName': 'StartTime',
                'attributeId': 1,
                'type': 'int',
                'reportable': true
            },
            2: {
                'attributeName': 'Duration',
                'attributeId': 2,
                'type': 'int',
                'reportable': true
            },
            3: {
                'attributeName': 'SampledPosition',
                'attributeId': 3,
                'type': '',
                'reportable': true
            },
            4: {
                'attributeName': 'PlaybackSpeed',
                'attributeId': 4,
                'type': '',
                'reportable': true
            },
            5: {
                'attributeName': 'SeekRangeEnd',
                'attributeId': 5,
                'type': 'int',
                'reportable': true
            },
            6: {
                'attributeName': 'SeekRangeStart',
                'attributeId': 6,
                'type': 'int',
                'reportable': true
            },
            65528: {
                'attributeName': 'GeneratedCommandList',
                'attributeId': 65528,
                'type': 'int',
                'reportable': true
            },
            65529: {
                'attributeName': 'AcceptedCommandList',
                'attributeId': 65529,
                'type': 'int',
                'reportable': true
            },
            65531: {
                'attributeName': 'AttributeList',
                'attributeId': 65531,
                'type': 'int',
                'reportable': true
            },
            65532: {
                'attributeName': 'FeatureMap',
                'attributeId': 65532,
                'type': 'int',
                'reportable': true
            },
            65533: {
                'attributeName': 'ClusterRevision',
                'attributeId': 65533,
                'type': 'int',
                'reportable': true
            }
        }
    },
    'MediaInput': {
        'clusterName': 'MediaInput',
        'clusterId': 1287,
        'commands': {
            0: {
                'commandId': 0,
                'commandName': 'SelectInput',
                'args': {
                    'index': 'int'
                }
            },
            1: {
                'commandId': 1,
                'commandName': 'ShowInputStatus',
                'args': {}
            },
            2: {
                'commandId': 2,
                'commandName': 'HideInputStatus',
                'args': {}
            },
            3: {
                'commandId': 3,
                'commandName': 'RenameInput',
                'args': {
                    'index': 'int',
                    'name': 'str'
                }
            }
        },
        'attributes': {
            0: {
                'attributeName': 'InputList',
                'attributeId': 0,
                'type': '',
                'reportable': true
            },
            1: {
                'attributeName': 'CurrentInput',
                'attributeId': 1,
                'type': 'int',
                'reportable': true
            },
            65528: {
                'attributeName': 'GeneratedCommandList',
                'attributeId': 65528,
                'type': 'int',
                'reportable': true
            },
            65529: {
                'attributeName': 'AcceptedCommandList',
                'attributeId': 65529,
                'type': 'int',
                'reportable': true
            },
            65531: {
                'attributeName': 'AttributeList',
                'attributeId': 65531,
                'type': 'int',
                'reportable': true
            },
            65532: {
                'attributeName': 'FeatureMap',
                'attributeId': 65532,
                'type': 'int',
                'reportable': true
            },
            65533: {
                'attributeName': 'ClusterRevision',
                'attributeId': 65533,
                'type': 'int',
                'reportable': true
            }
        }
    },
    'LowPower': {
        'clusterName': 'LowPower',
        'clusterId': 1288,
        'commands': {
            0: {
                'commandId': 0,
                'commandName': 'Sleep',
                'args': {}
            }
        },
        'attributes': {
            65528: {
                'attributeName': 'GeneratedCommandList',
                'attributeId': 65528,
                'type': 'int',
                'reportable': true
            },
            65529: {
                'attributeName': 'AcceptedCommandList',
                'attributeId': 65529,
                'type': 'int',
                'reportable': true
            },
            65531: {
                'attributeName': 'AttributeList',
                'attributeId': 65531,
                'type': 'int',
                'reportable': true
            },
            65532: {
                'attributeName': 'FeatureMap',
                'attributeId': 65532,
                'type': 'int',
                'reportable': true
            },
            65533: {
                'attributeName': 'ClusterRevision',
                'attributeId': 65533,
                'type': 'int',
                'reportable': true
            }
        }
    },
    'KeypadInput': {
        'clusterName': 'KeypadInput',
        'clusterId': 1289,
        'commands': {
            0: {
                'commandId': 0,
                'commandName': 'SendKey',
                'args': {
                    'keyCode': 'int'
                }
            }
        },
        'attributes': {
            65528: {
                'attributeName': 'GeneratedCommandList',
                'attributeId': 65528,
                'type': 'int',
                'reportable': true
            },
            65529: {
                'attributeName': 'AcceptedCommandList',
                'attributeId': 65529,
                'type': 'int',
                'reportable': true
            },
            65531: {
                'attributeName': 'AttributeList',
                'attributeId': 65531,
                'type': 'int',
                'reportable': true
            },
            65532: {
                'attributeName': 'FeatureMap',
                'attributeId': 65532,
                'type': 'int',
                'reportable': true
            },
            65533: {
                'attributeName': 'ClusterRevision',
                'attributeId': 65533,
                'type': 'int',
                'reportable': true
            }
        }
    },
    'ContentLauncher': {
        'clusterName': 'ContentLauncher',
        'clusterId': 1290,
        'commands': {
            0: {
                'commandId': 0,
                'commandName': 'LaunchContent',
                'args': {
                    'parameterList': '',
                    'autoPlay': 'bool',
                    'data': 'str'
                }
            },
            1: {
                'commandId': 1,
                'commandName': 'LaunchURL',
                'args': {
                    'contentURL': 'str',
                    'displayString': 'str',
                    'providerName': 'str',
                    'background': '',
                    'logo': '',
                    'progressBar': '',
                    'splash': '',
                    'waterMark': ''
                }
            }
        },
        'attributes': {
            0: {
                'attributeName': 'AcceptHeader',
                'attributeId': 0,
                'type': 'str',
                'reportable': true
            },
            1: {
                'attributeName': 'SupportedStreamingProtocols',
                'attributeId': 1,
                'type': 'int',
                'reportable': true,
                'writable': true
            },
            65528: {
                'attributeName': 'GeneratedCommandList',
                'attributeId': 65528,
                'type': 'int',
                'reportable': true
            },
            65529: {
                'attributeName': 'AcceptedCommandList',
                'attributeId': 65529,
                'type': 'int',
                'reportable': true
            },
            65531: {
                'attributeName': 'AttributeList',
                'attributeId': 65531,
                'type': 'int',
                'reportable': true
            },
            65532: {
                'attributeName': 'FeatureMap',
                'attributeId': 65532,
                'type': 'int',
                'reportable': true
            },
            65533: {
                'attributeName': 'ClusterRevision',
                'attributeId': 65533,
                'type': 'int',
                'reportable': true
            }
        }
    },
    'AudioOutput': {
        'clusterName': 'AudioOutput',
        'clusterId': 1291,
        'commands': {
            0: {
                'commandId': 0,
                'commandName': 'SelectOutput',
                'args': {
                    'index': 'int'
                }
            },
            1: {
                'commandId': 1,
                'commandName': 'RenameOutput',
                'args': {
                    'index': 'int',
                    'name': 'str'
                }
            }
        },
        'attributes': {
            0: {
                'attributeName': 'OutputList',
                'attributeId': 0,
                'type': '',
                'reportable': true
            },
            1: {
                'attributeName': 'CurrentOutput',
                'attributeId': 1,
                'type': 'int',
                'reportable': true
            },
            65528: {
                'attributeName': 'GeneratedCommandList',
                'attributeId': 65528,
                'type': 'int',
                'reportable': true
            },
            65529: {
                'attributeName': 'AcceptedCommandList',
                'attributeId': 65529,
                'type': 'int',
                'reportable': true
            },
            65531: {
                'attributeName': 'AttributeList',
                'attributeId': 65531,
                'type': 'int',
                'reportable': true
            },
            65532: {
                'attributeName': 'FeatureMap',
                'attributeId': 65532,
                'type': 'int',
                'reportable': true
            },
            65533: {
                'attributeName': 'ClusterRevision',
                'attributeId': 65533,
                'type': 'int',
                'reportable': true
            }
        }
    },
    'ApplicationLauncher': {
        'clusterName': 'ApplicationLauncher',
        'clusterId': 1292,
        'commands': {
            0: {
                'commandId': 0,
                'commandName': 'LaunchApp',
                'args': {
                    'catalogVendorId': 'int',
                    'applicationId': 'str',
                    'data': 'bytes'
                }
            },
            1: {
                'commandId': 1,
                'commandName': 'StopApp',
                'args': {
                    'catalogVendorId': 'int',
                    'applicationId': 'str'
                }
            },
            2: {
                'commandId': 2,
                'commandName': 'HideApp',
                'args': {
                    'catalogVendorId': 'int',
                    'applicationId': 'str'
                }
            }
        },
        'attributes': {
            0: {
                'attributeName': 'CatalogList',
                'attributeId': 0,
                'type': 'int',
                'reportable': true
            },
            1: {
                'attributeName': 'CurrentApp',
                'attributeId': 1,
                'type': '',
                'reportable': true,
                'writable': true
            },
            65528: {
                'attributeName': 'GeneratedCommandList',
                'attributeId': 65528,
                'type': 'int',
                'reportable': true
            },
            65529: {
                'attributeName': 'AcceptedCommandList',
                'attributeId': 65529,
                'type': 'int',
                'reportable': true
            },
            65531: {
                'attributeName': 'AttributeList',
                'attributeId': 65531,
                'type': 'int',
                'reportable': true
            },
            65532: {
                'attributeName': 'FeatureMap',
                'attributeId': 65532,
                'type': 'int',
                'reportable': true
            },
            65533: {
                'attributeName': 'ClusterRevision',
                'attributeId': 65533,
                'type': 'int',
                'reportable': true
            }
        }
    },
    'ApplicationBasic': {
        'clusterName': 'ApplicationBasic',
        'clusterId': 1293,
        'commands': {},
        'attributes': {
            0: {
                'attributeName': 'VendorName',
                'attributeId': 0,
                'type': 'str',
                'reportable': true
            },
            1: {
                'attributeName': 'VendorID',
                'attributeId': 1,
                'type': 'int',
                'reportable': true
            },
            2: {
                'attributeName': 'ApplicationName',
                'attributeId': 2,
                'type': 'str',
                'reportable': true
            },
            3: {
                'attributeName': 'ProductID',
                'attributeId': 3,
                'type': 'int',
                'reportable': true
            },
            4: {
                'attributeName': 'Application',
                'attributeId': 4,
                'type': '',
                'reportable': true
            },
            5: {
                'attributeName': 'Status',
                'attributeId': 5,
                'type': 'int',
                'reportable': true
            },
            6: {
                'attributeName': 'ApplicationVersion',
                'attributeId': 6,
                'type': 'str',
                'reportable': true
            },
            7: {
                'attributeName': 'AllowedVendorList',
                'attributeId': 7,
                'type': 'int',
                'reportable': true
            },
            65528: {
                'attributeName': 'GeneratedCommandList',
                'attributeId': 65528,
                'type': 'int',
                'reportable': true
            },
            65529: {
                'attributeName': 'AcceptedCommandList',
                'attributeId': 65529,
                'type': 'int',
                'reportable': true
            },
            65531: {
                'attributeName': 'AttributeList',
                'attributeId': 65531,
                'type': 'int',
                'reportable': true
            },
            65532: {
                'attributeName': 'FeatureMap',
                'attributeId': 65532,
                'type': 'int',
                'reportable': true
            },
            65533: {
                'attributeName': 'ClusterRevision',
                'attributeId': 65533,
                'type': 'int',
                'reportable': true
            }
        }
    },
    'AccountLogin': {
        'clusterName': 'AccountLogin',
        'clusterId': 1294,
        'commands': {
            0: {
                'commandId': 0,
                'commandName': 'GetSetupPIN',
                'args': {
                    'tempAccountIdentifier': 'str'
                }
            },
            2: {
                'commandId': 2,
                'commandName': 'Login',
                'args': {
                    'tempAccountIdentifier': 'str',
                    'setupPIN': 'str'
                }
            },
            3: {
                'commandId': 3,
                'commandName': 'Logout',
                'args': {}
            }
        },
        'attributes': {
            65528: {
                'attributeName': 'GeneratedCommandList',
                'attributeId': 65528,
                'type': 'int',
                'reportable': true
            },
            65529: {
                'attributeName': 'AcceptedCommandList',
                'attributeId': 65529,
                'type': 'int',
                'reportable': true
            },
            65531: {
                'attributeName': 'AttributeList',
                'attributeId': 65531,
                'type': 'int',
                'reportable': true
            },
            65532: {
                'attributeName': 'FeatureMap',
                'attributeId': 65532,
                'type': 'int',
                'reportable': true
            },
            65533: {
                'attributeName': 'ClusterRevision',
                'attributeId': 65533,
                'type': 'int',
                'reportable': true
            }
        }
    },
    'ElectricalMeasurement': {
        'clusterName': 'ElectricalMeasurement',
        'clusterId': 2820,
        'commands': {},
        'attributes': {
            0: {
                'attributeName': 'MeasurementType',
                'attributeId': 0,
                'type': 'int',
                'reportable': true
            },
            772: {
                'attributeName': 'TotalActivePower',
                'attributeId': 772,
                'type': 'int',
                'reportable': true
            },
            1285: {
                'attributeName': 'RmsVoltage',
                'attributeId': 1285,
                'type': 'int',
                'reportable': true
            },
            1286: {
                'attributeName': 'RmsVoltageMin',
                'attributeId': 1286,
                'type': 'int',
                'reportable': true
            },
            1287: {
                'attributeName': 'RmsVoltageMax',
                'attributeId': 1287,
                'type': 'int',
                'reportable': true
            },
            1288: {
                'attributeName': 'RmsCurrent',
                'attributeId': 1288,
                'type': 'int',
                'reportable': true
            },
            1289: {
                'attributeName': 'RmsCurrentMin',
                'attributeId': 1289,
                'type': 'int',
                'reportable': true
            },
            1290: {
                'attributeName': 'RmsCurrentMax',
                'attributeId': 1290,
                'type': 'int',
                'reportable': true
            },
            1291: {
                'attributeName': 'ActivePower',
                'attributeId': 1291,
                'type': 'int',
                'reportable': true
            },
            1292: {
                'attributeName': 'ActivePowerMin',
                'attributeId': 1292,
                'type': 'int',
                'reportable': true
            },
            1293: {
                'attributeName': 'ActivePowerMax',
                'attributeId': 1293,
                'type': 'int',
                'reportable': true
            },
            65528: {
                'attributeName': 'GeneratedCommandList',
                'attributeId': 65528,
                'type': 'int',
                'reportable': true
            },
            65529: {
                'attributeName': 'AcceptedCommandList',
                'attributeId': 65529,
                'type': 'int',
                'reportable': true
            },
            65531: {
                'attributeName': 'AttributeList',
                'attributeId': 65531,
                'type': 'int',
                'reportable': true
            },
            65532: {
                'attributeName': 'FeatureMap',
                'attributeId': 65532,
                'type': 'int',
                'reportable': true
            },
            65533: {
                'attributeName': 'ClusterRevision',
                'attributeId': 65533,
                'type': 'int',
                'reportable': true
            }
        }
    },
    # 'UnitTesting': {
    #     'clusterName': 'UnitTesting',
    #     'clusterId': 4294048773,
    #     'commands': {
    #         0: {
    #             'commandId': 0,
    #             'commandName': 'Test',
    #             'args': {}
    #         },
    #         1: {
    #             'commandId': 1,
    #             'commandName': 'TestNotHandled',
    #             'args': {}
    #         },
    #         2: {
    #             'commandId': 2,
    #             'commandName': 'TestSpecific',
    #             'args': {}
    #         },
    #         3: {
    #             'commandId': 3,
    #             'commandName': 'TestUnknownCommand',
    #             'args': {}
    #         },
    #         4: {
    #             'commandId': 4,
    #             'commandName': 'TestAddArguments',
    #             'args': {
    #                 'arg1': 'int',
    #                 'arg2': 'int'
    #             }
    #         },
    #         7: {
    #             'commandId': 7,
    #             'commandName': 'TestStructArgumentRequest',
    #             'args': {
    #                 'a': 'int',
    #                 'b': 'bool',
    #                 'c': 'int',
    #                 'd': 'bytes',
    #                 'e': 'str',
    #                 'f': 'int',
    #                 'g': '',
    #                 'h': ''
    #             }
    #         },
    #         8: {
    #             'commandId': 8,
    #             'commandName': 'TestNestedStructArgumentRequest',
    #             'args': {
    #                 'a': 'int',
    #                 'b': 'bool',
    #                 'c': ''
    #             }
    #         },
    #         9: {
    #             'commandId': 9,
    #             'commandName': 'TestListStructArgumentRequest',
    #             'args': {
    #                 'a': 'int',
    #                 'b': 'bool',
    #                 'c': 'int',
    #                 'd': 'bytes',
    #                 'e': 'str',
    #                 'f': 'int',
    #                 'g': '',
    #                 'h': ''
    #             }
    #         },
    #         10: {
    #             'commandId': 10,
    #             'commandName': 'TestListInt8UArgumentRequest',
    #             'args': {
    #                 'arg1': 'int'
    #             }
    #         },
    #         11: {
    #             'commandId': 11,
    #             'commandName': 'TestNestedStructListArgumentRequest',
    #             'args': {
    #                 'a': 'int',
    #                 'b': 'bool',
    #                 'c': '',
    #                 'd': '',
    #                 'e': 'int',
    #                 'f': 'bytes',
    #                 'g': 'int'
    #             }
    #         },
    #         12: {
    #             'commandId': 12,
    #             'commandName': 'TestListNestedStructListArgumentRequest',
    #             'args': {
    #                 'a': 'int',
    #                 'b': 'bool',
    #                 'c': '',
    #                 'd': '',
    #                 'e': 'int',
    #                 'f': 'bytes',
    #                 'g': 'int'
    #             }
    #         },
    #         13: {
    #             'commandId': 13,
    #             'commandName': 'TestListInt8UReverseRequest',
    #             'args': {
    #                 'arg1': 'int'
    #             }
    #         },
    #         14: {
    #             'commandId': 14,
    #             'commandName': 'TestEnumsRequest',
    #             'args': {
    #                 'arg1': 'int',
    #                 'arg2': 'int'
    #             }
    #         },
    #         15: {
    #             'commandId': 15,
    #             'commandName': 'TestNullableOptionalRequest',
    #             'args': {
    #                 'arg1': 'int'
    #             }
    #         },
    #         17: {
    #             'commandId': 17,
    #             'commandName': 'SimpleStructEchoRequest',
    #             'args': {
    #                 'a': 'int',
    #                 'b': 'bool',
    #                 'c': 'int',
    #                 'd': 'bytes',
    #                 'e': 'str',
    #                 'f': 'int',
    #                 'g': '',
    #                 'h': ''
    #             }
    #         },
    #         18: {
    #             'commandId': 18,
    #             'commandName': 'TimedInvokeRequest',
    #             'args': {}
    #         },
    #         19: {
    #             'commandId': 19,
    #             'commandName': 'TestSimpleOptionalArgumentRequest',
    #             'args': {
    #                 'arg1': 'bool'
    #             }
    #         },
    #         20: {
    #             'commandId': 20,
    #             'commandName': 'TestEmitTestEventRequest',
    #             'args': {
    #                 'arg1': 'int',
    #                 'arg2': 'int',
    #                 'arg3': 'bool'
    #             }
    #         }
    #     },
    #     'attributes': {
    #         0: {
    #             'attributeName': 'Boolean',
    #             'attributeId': 0,
    #             'type': 'bool',
    #             'reportable': true,
    #             'writable': true
    #         },
    #         1: {
    #             'attributeName': 'Bitmap8',
    #             'attributeId': 1,
    #             'type': 'int',
    #             'reportable': true,
    #             'writable': true
    #         },
    #         2: {
    #             'attributeName': 'Bitmap16',
    #             'attributeId': 2,
    #             'type': 'int',
    #             'reportable': true,
    #             'writable': true
    #         },
    #         3: {
    #             'attributeName': 'Bitmap32',
    #             'attributeId': 3,
    #             'type': 'int',
    #             'reportable': true,
    #             'writable': true
    #         },
    #         4: {
    #             'attributeName': 'Bitmap64',
    #             'attributeId': 4,
    #             'type': 'int',
    #             'reportable': true,
    #             'writable': true
    #         },
    #         5: {
    #             'attributeName': 'Int8u',
    #             'attributeId': 5,
    #             'type': 'int',
    #             'reportable': true,
    #             'writable': true
    #         },
    #         6: {
    #             'attributeName': 'Int16u',
    #             'attributeId': 6,
    #             'type': 'int',
    #             'reportable': true,
    #             'writable': true
    #         },
    #         7: {
    #             'attributeName': 'Int24u',
    #             'attributeId': 7,
    #             'type': 'int',
    #             'reportable': true,
    #             'writable': true
    #         },
    #         8: {
    #             'attributeName': 'Int32u',
    #             'attributeId': 8,
    #             'type': 'int',
    #             'reportable': true,
    #             'writable': true
    #         },
    #         9: {
    #             'attributeName': 'Int40u',
    #             'attributeId': 9,
    #             'type': 'int',
    #             'reportable': true,
    #             'writable': true
    #         },
    #         10: {
    #             'attributeName': 'Int48u',
    #             'attributeId': 10,
    #             'type': 'int',
    #             'reportable': true,
    #             'writable': true
    #         },
    #         11: {
    #             'attributeName': 'Int56u',
    #             'attributeId': 11,
    #             'type': 'int',
    #             'reportable': true,
    #             'writable': true
    #         },
    #         12: {
    #             'attributeName': 'Int64u',
    #             'attributeId': 12,
    #             'type': 'int',
    #             'reportable': true,
    #             'writable': true
    #         },
    #         13: {
    #             'attributeName': 'Int8s',
    #             'attributeId': 13,
    #             'type': 'int',
    #             'reportable': true,
    #             'writable': true
    #         },
    #         14: {
    #             'attributeName': 'Int16s',
    #             'attributeId': 14,
    #             'type': 'int',
    #             'reportable': true,
    #             'writable': true
    #         },
    #         15: {
    #             'attributeName': 'Int24s',
    #             'attributeId': 15,
    #             'type': 'int',
    #             'reportable': true,
    #             'writable': true
    #         },
    #         16: {
    #             'attributeName': 'Int32s',
    #             'attributeId': 16,
    #             'type': 'int',
    #             'reportable': true,
    #             'writable': true
    #         },
    #         17: {
    #             'attributeName': 'Int40s',
    #             'attributeId': 17,
    #             'type': 'int',
    #             'reportable': true,
    #             'writable': true
    #         },
    #         18: {
    #             'attributeName': 'Int48s',
    #             'attributeId': 18,
    #             'type': 'int',
    #             'reportable': true,
    #             'writable': true
    #         },
    #         19: {
    #             'attributeName': 'Int56s',
    #             'attributeId': 19,
    #             'type': 'int',
    #             'reportable': true,
    #             'writable': true
    #         },
    #         20: {
    #             'attributeName': 'Int64s',
    #             'attributeId': 20,
    #             'type': 'int',
    #             'reportable': true,
    #             'writable': true
    #         },
    #         21: {
    #             'attributeName': 'Enum8',
    #             'attributeId': 21,
    #             'type': 'int',
    #             'reportable': true,
    #             'writable': true
    #         },
    #         22: {
    #             'attributeName': 'Enum16',
    #             'attributeId': 22,
    #             'type': 'int',
    #             'reportable': true,
    #             'writable': true
    #         },
    #         23: {
    #             'attributeName': 'FloatSingle',
    #             'attributeId': 23,
    #             'type': '',
    #             'reportable': true,
    #             'writable': true
    #         },
    #         24: {
    #             'attributeName': 'FloatDouble',
    #             'attributeId': 24,
    #             'type': '',
    #             'reportable': true,
    #             'writable': true
    #         },
    #         25: {
    #             'attributeName': 'OctetString',
    #             'attributeId': 25,
    #             'type': 'bytes',
    #             'reportable': true,
    #             'writable': true
    #         },
    #         26: {
    #             'attributeName': 'ListInt8u',
    #             'attributeId': 26,
    #             'type': 'int',
    #             'reportable': true,
    #             'writable': true
    #         },
    #         27: {
    #             'attributeName': 'ListOctetString',
    #             'attributeId': 27,
    #             'type': 'bytes',
    #             'reportable': true,
    #             'writable': true
    #         },
    #         28: {
    #             'attributeName': 'ListStructOctetString',
    #             'attributeId': 28,
    #             'type': '',
    #             'reportable': true,
    #             'writable': true
    #         },
    #         29: {
    #             'attributeName': 'LongOctetString',
    #             'attributeId': 29,
    #             'type': 'bytes',
    #             'reportable': true,
    #             'writable': true
    #         },
    #         30: {
    #             'attributeName': 'CharString',
    #             'attributeId': 30,
    #             'type': 'str',
    #             'reportable': true,
    #             'writable': true
    #         },
    #         31: {
    #             'attributeName': 'LongCharString',
    #             'attributeId': 31,
    #             'type': 'str',
    #             'reportable': true,
    #             'writable': true
    #         },
    #         32: {
    #             'attributeName': 'EpochUs',
    #             'attributeId': 32,
    #             'type': 'int',
    #             'reportable': true,
    #             'writable': true
    #         },
    #         33: {
    #             'attributeName': 'EpochS',
    #             'attributeId': 33,
    #             'type': 'int',
    #             'reportable': true,
    #             'writable': true
    #         },
    #         34: {
    #             'attributeName': 'VendorId',
    #             'attributeId': 34,
    #             'type': 'int',
    #             'reportable': true,
    #             'writable': true
    #         },
    #         35: {
    #             'attributeName': 'ListNullablesAndOptionalsStruct',
    #             'attributeId': 35,
    #             'type': '',
    #             'reportable': true,
    #             'writable': true
    #         },
    #         36: {
    #             'attributeName': 'EnumAttr',
    #             'attributeId': 36,
    #             'type': 'int',
    #             'reportable': true,
    #             'writable': true
    #         },
    #         37: {
    #             'attributeName': 'StructAttr',
    #             'attributeId': 37,
    #             'type': '',
    #             'reportable': true,
    #             'writable': true
    #         },
    #         38: {
    #             'attributeName': 'RangeRestrictedInt8u',
    #             'attributeId': 38,
    #             'type': 'int',
    #             'reportable': true,
    #             'writable': true
    #         },
    #         39: {
    #             'attributeName': 'RangeRestrictedInt8s',
    #             'attributeId': 39,
    #             'type': 'int',
    #             'reportable': true,
    #             'writable': true
    #         },
    #         40: {
    #             'attributeName': 'RangeRestrictedInt16u',
    #             'attributeId': 40,
    #             'type': 'int',
    #             'reportable': true,
    #             'writable': true
    #         },
    #         41: {
    #             'attributeName': 'RangeRestrictedInt16s',
    #             'attributeId': 41,
    #             'type': 'int',
    #             'reportable': true,
    #             'writable': true
    #         },
    #         42: {
    #             'attributeName': 'ListLongOctetString',
    #             'attributeId': 42,
    #             'type': 'bytes',
    #             'reportable': true,
    #             'writable': true
    #         },
    #         43: {
    #             'attributeName': 'ListFabricScoped',
    #             'attributeId': 43,
    #             'type': '',
    #             'reportable': true,
    #             'writable': true
    #         },
    #         48: {
    #             'attributeName': 'TimedWriteBoolean',
    #             'attributeId': 48,
    #             'type': 'bool',
    #             'reportable': true,
    #             'writable': true
    #         },
    #         49: {
    #             'attributeName': 'GeneralErrorBoolean',
    #             'attributeId': 49,
    #             'type': 'bool',
    #             'reportable': true,
    #             'writable': true
    #         },
    #         50: {
    #             'attributeName': 'ClusterErrorBoolean',
    #             'attributeId': 50,
    #             'type': 'bool',
    #             'reportable': true,
    #             'writable': true
    #         },
    #         255: {
    #             'attributeName': 'Unsupported',
    #             'attributeId': 255,
    #             'type': 'bool',
    #             'reportable': true,
    #             'writable': true
    #         },
    #         16384: {
    #             'attributeName': 'NullableBoolean',
    #             'attributeId': 16384,
    #             'type': 'bool',
    #             'reportable': true,
    #             'writable': true
    #         },
    #         16385: {
    #             'attributeName': 'NullableBitmap8',
    #             'attributeId': 16385,
    #             'type': 'int',
    #             'reportable': true,
    #             'writable': true
    #         },
    #         16386: {
    #             'attributeName': 'NullableBitmap16',
    #             'attributeId': 16386,
    #             'type': 'int',
    #             'reportable': true,
    #             'writable': true
    #         },
    #         16387: {
    #             'attributeName': 'NullableBitmap32',
    #             'attributeId': 16387,
    #             'type': 'int',
    #             'reportable': true,
    #             'writable': true
    #         },
    #         16388: {
    #             'attributeName': 'NullableBitmap64',
    #             'attributeId': 16388,
    #             'type': 'int',
    #             'reportable': true,
    #             'writable': true
    #         },
    #         16389: {
    #             'attributeName': 'NullableInt8u',
    #             'attributeId': 16389,
    #             'type': 'int',
    #             'reportable': true,
    #             'writable': true
    #         },
    #         16390: {
    #             'attributeName': 'NullableInt16u',
    #             'attributeId': 16390,
    #             'type': 'int',
    #             'reportable': true,
    #             'writable': true
    #         },
    #         16391: {
    #             'attributeName': 'NullableInt24u',
    #             'attributeId': 16391,
    #             'type': 'int',
    #             'reportable': true,
    #             'writable': true
    #         },
    #         16392: {
    #             'attributeName': 'NullableInt32u',
    #             'attributeId': 16392,
    #             'type': 'int',
    #             'reportable': true,
    #             'writable': true
    #         },
    #         16393: {
    #             'attributeName': 'NullableInt40u',
    #             'attributeId': 16393,
    #             'type': 'int',
    #             'reportable': true,
    #             'writable': true
    #         },
    #         16394: {
    #             'attributeName': 'NullableInt48u',
    #             'attributeId': 16394,
    #             'type': 'int',
    #             'reportable': true,
    #             'writable': true
    #         },
    #         16395: {
    #             'attributeName': 'NullableInt56u',
    #             'attributeId': 16395,
    #             'type': 'int',
    #             'reportable': true,
    #             'writable': true
    #         },
    #         16396: {
    #             'attributeName': 'NullableInt64u',
    #             'attributeId': 16396,
    #             'type': 'int',
    #             'reportable': true,
    #             'writable': true
    #         },
    #         16397: {
    #             'attributeName': 'NullableInt8s',
    #             'attributeId': 16397,
    #             'type': 'int',
    #             'reportable': true,
    #             'writable': true
    #         },
    #         16398: {
    #             'attributeName': 'NullableInt16s',
    #             'attributeId': 16398,
    #             'type': 'int',
    #             'reportable': true,
    #             'writable': true
    #         },
    #         16399: {
    #             'attributeName': 'NullableInt24s',
    #             'attributeId': 16399,
    #             'type': 'int',
    #             'reportable': true,
    #             'writable': true
    #         },
    #         16400: {
    #             'attributeName': 'NullableInt32s',
    #             'attributeId': 16400,
    #             'type': 'int',
    #             'reportable': true,
    #             'writable': true
    #         },
    #         16401: {
    #             'attributeName': 'NullableInt40s',
    #             'attributeId': 16401,
    #             'type': 'int',
    #             'reportable': true,
    #             'writable': true
    #         },
    #         16402: {
    #             'attributeName': 'NullableInt48s',
    #             'attributeId': 16402,
    #             'type': 'int',
    #             'reportable': true,
    #             'writable': true
    #         },
    #         16403: {
    #             'attributeName': 'NullableInt56s',
    #             'attributeId': 16403,
    #             'type': 'int',
    #             'reportable': true,
    #             'writable': true
    #         },
    #         16404: {
    #             'attributeName': 'NullableInt64s',
    #             'attributeId': 16404,
    #             'type': 'int',
    #             'reportable': true,
    #             'writable': true
    #         },
    #         16405: {
    #             'attributeName': 'NullableEnum8',
    #             'attributeId': 16405,
    #             'type': 'int',
    #             'reportable': true,
    #             'writable': true
    #         },
    #         16406: {
    #             'attributeName': 'NullableEnum16',
    #             'attributeId': 16406,
    #             'type': 'int',
    #             'reportable': true,
    #             'writable': true
    #         },
    #         16407: {
    #             'attributeName': 'NullableFloatSingle',
    #             'attributeId': 16407,
    #             'type': '',
    #             'reportable': true,
    #             'writable': true
    #         },
    #         16408: {
    #             'attributeName': 'NullableFloatDouble',
    #             'attributeId': 16408,
    #             'type': '',
    #             'reportable': true,
    #             'writable': true
    #         },
    #         16409: {
    #             'attributeName': 'NullableOctetString',
    #             'attributeId': 16409,
    #             'type': 'bytes',
    #             'reportable': true,
    #             'writable': true
    #         },
    #         16414: {
    #             'attributeName': 'NullableCharString',
    #             'attributeId': 16414,
    #             'type': 'str',
    #             'reportable': true,
    #             'writable': true
    #         },
    #         16420: {
    #             'attributeName': 'NullableEnumAttr',
    #             'attributeId': 16420,
    #             'type': 'int',
    #             'reportable': true,
    #             'writable': true
    #         },
    #         16421: {
    #             'attributeName': 'NullableStruct',
    #             'attributeId': 16421,
    #             'type': '',
    #             'reportable': true,
    #             'writable': true
    #         },
    #         16422: {
    #             'attributeName': 'NullableRangeRestrictedInt8u',
    #             'attributeId': 16422,
    #             'type': 'int',
    #             'reportable': true,
    #             'writable': true
    #         },
    #         16423: {
    #             'attributeName': 'NullableRangeRestrictedInt8s',
    #             'attributeId': 16423,
    #             'type': 'int',
    #             'reportable': true,
    #             'writable': true
    #         },
    #         16424: {
    #             'attributeName': 'NullableRangeRestrictedInt16u',
    #             'attributeId': 16424,
    #             'type': 'int',
    #             'reportable': true,
    #             'writable': true
    #         },
    #         16425: {
    #             'attributeName': 'NullableRangeRestrictedInt16s',
    #             'attributeId': 16425,
    #             'type': 'int',
    #             'reportable': true,
    #             'writable': true
    #         },
    #         65528: {
    #             'attributeName': 'GeneratedCommandList',
    #             'attributeId': 65528,
    #             'type': 'int',
    #             'reportable': true
    #         },
    #         65529: {
    #             'attributeName': 'AcceptedCommandList',
    #             'attributeId': 65529,
    #             'type': 'int',
    #             'reportable': true
    #         },
    #         65531: {
    #             'attributeName': 'AttributeList',
    #             'attributeId': 65531,
    #             'type': 'int',
    #             'reportable': true
    #         },
    #         65532: {
    #             'attributeName': 'FeatureMap',
    #             'attributeId': 65532,
    #             'type': 'int',
    #             'reportable': true
    #         },
    #         65533: {
    #             'attributeName': 'ClusterRevision',
    #             'attributeId': 65533,
    #             'type': 'int',
    #             'reportable': true
    #         }
    #     }
    # }
}

return _Matter_clusters
