# Matter 1.4.1 Application Cluster Specification - AI Compact Reference
Version: 1.4.1 (March 2025)

## Document Structure
- Ch1: General Clusters
- Ch2: Measurement/Sensing
- Ch3: Lighting
- Ch4: HVAC
- Ch5: Closures
- Ch6: Media
- Ch7: Robots
- Ch8: Home Appliances
- Ch9: Energy Management
- Ch10: Network Infrastructure

## Notation
- M=Mandatory, O=Optional, P=Provisional, F=Fixed, X=Nullable, N=NonVolatile
- Q=Quieter reporting, S=Scene, P=Periodic reporting
- R=Read, W=Write, V=View privilege, O=Operate, M=Manage, A=Administer

---
# CHAPTER 1: GENERAL CLUSTERS

## 1.2 Identify Cluster (0x0003)
Rev:5 | Role:Utility | Scope:Endpoint | PICS:I

### Data Types
```
IdentifyTypeEnum(enum8): None=0,LightOutput=1,VisibleIndicator=2,AudibleBeep=3,Display=4,Actuator=5
EffectIdentifierEnum(enum8): Blink=0,Breathe=1,Okay=2,ChannelChange=0x0B,FinishEffect=0xFE,StopEffect=0xFF
EffectVariantEnum(enum8): Default=0
```

### Attributes
| ID | Name | Type | Constraint | Quality | Default | Access | Conf |
|----|------|------|------------|---------|---------|--------|------|
|0x0000|IdentifyTime|uint16|all||0|RW VO|M|
|0x0001|IdentifyType|IdentifyTypeEnum|desc|MS||R V|M|

### Commands
| ID | Name | Dir | Response | Access | Conf |
|----|------|-----|----------|--------|------|
|0x00|Identify|C→S|Y|M|M|
|0x40|TriggerEffect|C→S|Y|M|O|

Identify: {IdentifyTime:uint16}
TriggerEffect: {EffectIdentifier:EffectIdentifierEnum, EffectVariant:EffectVariantEnum}

---
## 1.3 Groups Cluster (0x0004)
Rev:4 | Role:Utility | Scope:Endpoint | PICS:G

### Features
| Bit | Code | Feature | Summary |
|-----|------|---------|---------|
|0|GN|GroupNames|Store name for group|

### Data Types
```
NameSupportBitmap(map8): Bit7=GroupNames
```

### Attributes
| ID | Name | Type | Constraint | Quality | Default | Access | Conf |
|----|------|------|------------|---------|---------|--------|------|
|0x0000|NameSupport|NameSupportBitmap|desc|F|0|R V|M|

### Commands
| ID | Name | Dir | Response | Access | Conf |
|----|------|-----|----------|--------|------|
|0x00|AddGroup|C→S|AddGroupResponse|M F|M|
|0x01|ViewGroup|C→S|ViewGroupResponse|O F|M|
|0x02|GetGroupMembership|C→S|GetGroupMembershipResponse|O F|M|
|0x03|RemoveGroup|C→S|RemoveGroupResponse|M F|M|
|0x04|RemoveAllGroups|C→S|Y|M F|M|
|0x05|AddGroupIfIdentifying|C→S|Y|M F|M|

AddGroup: {GroupID:group-id(min1), GroupName:string(max16)}
ViewGroup: {GroupID:group-id(min1)}
GetGroupMembership: {GroupList:list[group-id](min1)}
RemoveGroup: {GroupID:group-id(min1)}
AddGroupIfIdentifying: {GroupID:group-id(min1), GroupName:string(max16)}

Response Fields:
- AddGroupResponse: {Status:enum8, GroupID:group-id}
- ViewGroupResponse: {Status:enum8, GroupID:group-id, GroupName:string(max16)}
- GetGroupMembershipResponse: {Capacity:uint8|X, GroupList:list[group-id]}
- RemoveGroupResponse: {Status:enum8, GroupID:group-id}

---
## 1.4 Scenes Management Cluster (0x0062) [PROVISIONAL]
Rev:1 | Role:Application | Scope:Endpoint | PICS:S

### Features
| Bit | Code | Feature | Conf | Summary |
|-----|------|---------|------|---------|
|0|SN|SceneNames|O|Store scene names|

### Data Types
```
CopyModeBitmap(map8): CopyAllScenes=Bit0
SceneInfoStruct: {SceneCount:uint8, CurrentScene:uint8(0xFF=undef), CurrentGroup:group-id, SceneValid:bool, RemainingCapacity:uint8(max253)}
AttributeValuePairStruct: {AttributeID:attribute-id, ValueUnsigned8/Signed8/16/32/64:varies}
ExtensionFieldSetStruct: {ClusterID:cluster-id, AttributeValueList:list[AttributeValuePairStruct]}
```

### Attributes
| ID | Name | Type | Constraint | Quality | Default | Access | Conf |
|----|------|------|------------|---------|---------|--------|------|
|0x0000|LastConfiguredBy|node-id|all|X|null|R V|O|
|0x0001|SceneTableSize|uint16|all|F||R V|M|
|0x0002|FabricSceneInfo|list[SceneInfoStruct]|all|||R V|M|

### Commands
| ID | Name | Dir | Response | Access | Conf |
|----|------|-----|----------|--------|------|
|0x00|AddScene|C→S|AddSceneResponse|M F|M|
|0x01|ViewScene|C→S|ViewSceneResponse|O F|M|
|0x02|RemoveScene|C→S|RemoveSceneResponse|M F|M|
|0x03|RemoveAllScenes|C→S|RemoveAllScenesResponse|M F|M|
|0x04|StoreScene|C→S|StoreSceneResponse|M F|M|
|0x05|RecallScene|C→S|Y|O|M|
|0x06|GetSceneMembership|C→S|GetSceneMembershipResponse|O F|M|
|0x40|CopyScene|C→S|CopySceneResponse|M F|O|

---
## 1.5 On/Off Cluster (0x0006)
Rev:6 | Role:Application | Scope:Endpoint | PICS:OO

### Features
| Bit | Code | Feature | Conf | Summary |
|-----|------|---------|------|---------|
|0|LT|Lighting|[!OFFONLY]|Lighting behavior|
|1|DF|DeadFrontBehavior|[!OFFONLY]|Dead front when off|
|2|OFFONLY|OffOnly|[!(LT\|DF)]|Only Off command|

### Data Types
```
OnOffControlBitmap(map8): AcceptOnlyWhenOn=Bit0
StartUpOnOffEnum(enum8): Off=0,On=1,Toggle=2
EffectIdentifierEnum(enum8): DelayedAllOff=0,DyingLight=1
DelayedAllOffEffectVariantEnum(enum8): DelayedOffFastFade=0,NoFade=1,DelayedOffSlowFade=2
DyingLightEffectVariantEnum(enum8): DyingLightFadeOff=0
```

### Attributes
| ID | Name | Type | Constraint | Quality | Default | Access | Conf |
|----|------|------|------------|---------|---------|--------|------|
|0x0000|OnOff|bool|all|SN|FALSE|R V|M|
|0x4000|GlobalSceneControl|bool|all||TRUE|R V|LT|
|0x4001|OnTime|uint16|all||0|RW VO|LT|
|0x4002|OffWaitTime|uint16|all||0|RW VO|LT|
|0x4003|StartUpOnOff|StartUpOnOffEnum|desc|XN|MS|RW VM|LT|

### Commands
| ID | Name | Dir | Response | Access | Conf |
|----|------|-----|----------|--------|------|
|0x00|Off|C→S|Y|O|M|
|0x01|On|C→S|Y|O|!OFFONLY|
|0x02|Toggle|C→S|Y|O|!OFFONLY|
|0x40|OffWithEffect|C→S|Y|O|LT|
|0x41|OnWithRecallGlobalScene|C→S|Y|O|LT|
|0x42|OnWithTimedOff|C→S|Y|O|LT|

OffWithEffect: {EffectIdentifier:EffectIdentifierEnum, EffectVariant:enum8}
OnWithTimedOff: {OnOffControl:OnOffControlBitmap(0-1), OnTime:uint16(max0xFFFE), OffWaitTime:uint16(max0xFFFE)}

---
## 1.6 Level Control Cluster (0x0008)
Rev:6 | Role:Application | Scope:Endpoint | PICS:LVL

### Features
| Bit | Code | Feature | Conf | Summary |
|-----|------|---------|------|---------|
|0|OO|OnOff|M|Dependency on On/Off|
|1|LT|Lighting|O|Lighting behavior|
|2|FQ|Frequency|P|Frequency control|

### Data Types
```
OptionsBitmap(map8): ExecuteIfOff=Bit0,CoupleColorTempToLevel=Bit1
MoveModeEnum(enum8): Up=0,Down=1
StepModeEnum(enum8): Up=0,Down=1
```

### Attributes
| ID | Name | Type | Constraint | Quality | Default | Access | Conf |
|----|------|------|------------|---------|---------|--------|------|
|0x0000|CurrentLevel|uint8|1-254|XQN|null|R V|M|
|0x0001|RemainingTime|uint16|all|Q|0|R V|LT|
|0x0002|MinLevel|uint8|1-254||1/0|R V|O|
|0x0003|MaxLevel|uint8|MinLevel-254||254|R V|O|
|0x0004|CurrentFrequency|uint16|MinFreq-MaxFreq|QP|0|R V|FQ|
|0x0005|MinFrequency|uint16|0-MaxFreq|F|0|R V|FQ|
|0x0006|MaxFrequency|uint16|MinFreq-65535|F|0|R V|FQ|
|0x000F|Options|OptionsBitmap|all||0|RW VO|M|
|0x0010|OnOffTransitionTime|uint16|all||0|RW VO|O|
|0x0011|OnLevel|uint8|MinLevel-MaxLevel|XN|null|RW VO|M|
|0x0012|OnTransitionTime|uint16|all|XN|null|RW VO|O|
|0x0013|OffTransitionTime|uint16|all|XN|null|RW VO|O|
|0x0014|DefaultMoveRate|uint8|all|XN|null|RW VO|O|
|0x4000|StartUpCurrentLevel|uint8|1-254|XN|MS|RW VM|LT|

### Commands
| ID | Name | Dir | Response | Access | Conf |
|----|------|-----|----------|--------|------|
|0x00|MoveToLevel|C→S|Y|O|M|
|0x01|Move|C→S|Y|O|M|
|0x02|Step|C→S|Y|O|M|
|0x03|Stop|C→S|Y|O|M|
|0x04|MoveToLevelWithOnOff|C→S|Y|O|M|
|0x05|MoveWithOnOff|C→S|Y|O|M|
|0x06|StepWithOnOff|C→S|Y|O|M|
|0x07|StopWithOnOff|C→S|Y|O|M|
|0x08|MoveToClosestFrequency|C→S|Y|O|FQ|

MoveToLevel: {Level:uint8(0-254), TransitionTime:uint16|X, OptionsMask:OptionsBitmap, OptionsOverride:OptionsBitmap}
Move: {MoveMode:MoveModeEnum, Rate:uint8|X, OptionsMask:OptionsBitmap, OptionsOverride:OptionsBitmap}
Step: {StepMode:StepModeEnum, StepSize:uint8, TransitionTime:uint16|X, OptionsMask:OptionsBitmap, OptionsOverride:OptionsBitmap}
Stop: {OptionsMask:OptionsBitmap, OptionsOverride:OptionsBitmap}
MoveToClosestFrequency: {Frequency:uint16}

---
## 1.7 Boolean State Cluster (0x0045)
Rev:1 | Role:Application | Scope:Endpoint | PICS:BOOL

### Attributes
| ID | Name | Type | Constraint | Quality | Default | Access | Conf |
|----|------|------|------------|---------|---------|--------|------|
|0x0000|StateValue|bool||P||R V|M|

### Events
| ID | Name | Priority | Access | Conf |
|----|------|----------|--------|------|
|0x00|StateChange|INFO|V|O|

StateChange: {StateValue:bool}

---
## 1.8 Boolean State Configuration Cluster (0x0080)
Rev:1 | Role:Application | Scope:Endpoint | PICS:BOOLCFG

### Features
| Bit | Code | Feature | Conf | Summary |
|-----|------|---------|------|---------|
|0|VIS|Visual|O|Visual alarms|
|1|AUD|Audible|O|Audible alarms|
|2|SPRS|AlarmSuppress|[VIS\|AUD]|Suppress alarms|
|3|SENSLVL|SensitivityLevel|O|Sensitivity setting|

### Data Types
```
AlarmModeBitmap(map8): Visual=Bit0,Audible=Bit1
SensorFaultBitmap(map16): GeneralFault=Bit0
```

### Attributes
| ID | Name | Type | Constraint | Quality | Default | Access | Conf |
|----|------|------|------------|---------|---------|--------|------|
|0x0000|CurrentSensitivityLevel|uint8|max(SupportedSensitivityLevels-1)|N|MS|RW VO|SENSLVL|
|0x0001|SupportedSensitivityLevels|uint8|2-10|F|MS|R V|SENSLVL|
|0x0002|DefaultSensitivityLevel|uint8|max(SupportedSensitivityLevels-1)|F|MS|R V|[SENSLVL]|
|0x0003|AlarmsActive|AlarmModeBitmap|all||0|R V|VIS\|AUD|
|0x0004|AlarmsSuppressed|AlarmModeBitmap|all||0|R V|SPRS|
|0x0005|AlarmsEnabled|AlarmModeBitmap|all|N|MS|R V|[VIS\|AUD]|
|0x0006|AlarmsSupported|AlarmModeBitmap|all|F|0|R V|VIS\|AUD|
|0x0007|SensorFault|SensorFaultBitmap|all||0|R V|O|

### Commands
| ID | Name | Dir | Response | Access | Conf |
|----|------|-----|----------|--------|------|
|0x00|SuppressAlarm|C→S|Y|O|SPRS|
|0x01|EnableDisableAlarm|C→S|Y|O|VIS\|AUD|

SuppressAlarm: {AlarmsToSuppress:AlarmModeBitmap}
EnableDisableAlarm: {AlarmsToEnableDisable:AlarmModeBitmap}

### Events
| ID | Name | Priority | Access | Conf |
|----|------|----------|--------|------|
|0x00|AlarmsStateChanged|INFO|V|VIS\|AUD|
|0x01|SensorFault|INFO|V|O|

---
## 1.9 Mode Select Cluster (0x0050)
Rev:2 | Role:Application | Scope:Endpoint | PICS:MOD

### Features
| Bit | Code | Feature | Summary |
|-----|------|---------|---------|
|0|DEPONOFF|OnOff|Dependency on On/Off|

### Data Types
```
SemanticTagStruct: {MfgCode:vendor-id(F), Value:enum16(F)}
ModeOptionStruct: {Label:string(max64,F), Mode:uint8(F), SemanticTags:list[SemanticTagStruct](max64,F)}
```

### Attributes
| ID | Name | Type | Constraint | Quality | Default | Access | Conf |
|----|------|------|------------|---------|---------|--------|------|
|0x0000|Description|string|max64|F|MS|R V|M|
|0x0001|StandardNamespace|enum16|desc|FX|null|R V|M|
|0x0002|SupportedModes|list[ModeOptionStruct]|max255|F|MS|R V|M|
|0x0003|CurrentMode|uint8|desc|N|MS|R V|M|
|0x0004|StartUpMode|uint8|desc|NX|MS|RW VO|O|
|0x0005|OnMode|uint8|desc|NX|null|RW VO|DEPONOFF|

### Commands
| ID | Name | Dir | Response | Access | Conf |
|----|------|-----|----------|--------|------|
|0x00|ChangeToMode|C→S|Y|O|M|

ChangeToMode: {NewMode:uint8}

---
## 1.10 Mode Base Cluster (Base for derived clusters)
Rev:2 | Role:Application | Scope:Endpoint | PICS:MODB

### Features
| Bit | Code | Feature | Summary |
|-----|------|---------|---------|
|0|DEPONOFF|OnOff|Dependency on On/Off|

### Data Types
```
ModeTagStruct: {MfgCode:vendor-id(O), Value:enum16}
ModeOptionStruct: {Label:string(max64,F), Mode:uint8(F), ModeTags:list[ModeTagStruct](max8,F)}
```

### Attributes
| ID | Name | Type | Constraint | Quality | Default | Access | Conf |
|----|------|------|------------|---------|---------|--------|------|
|0x0000|SupportedModes|list[ModeOptionStruct]|2-255|F|MS|R V|M|
|0x0001|CurrentMode|uint8|desc|N|MS|R V|M|
|0x0002|StartUpMode|uint8|desc|NX|MS|RW VO|O|
|0x0003|OnMode|uint8|desc|NX|null|RW VO|DEPONOFF|

### Commands
| ID | Name | Dir | Response | Access | Conf |
|----|------|-----|----------|--------|------|
|0x00|ChangeToMode|C→S|ChangeToModeResponse|O|M|

ChangeToMode: {NewMode:uint8}
ChangeToModeResponse: {Status:ModeChangeStatus, StatusText:string(max64,O)}

### Mode Tag Values (0x0000-0x003F reserved for base)
| Value | Name | Summary |
|-------|------|---------|
|0x0000|Auto|Auto mode|
|0x0001|Quick|Quick mode|
|0x0002|Quiet|Quiet mode|
|0x0003|LowNoise|Low noise|
|0x0004|LowEnergy|Low energy|
|0x0005|Vacation|Vacation mode|
|0x0006|Min|Minimum setting|
|0x0007|Max|Maximum setting|
|0x0008|Night|Night mode|
|0x0009|Day|Day mode|

---
## 1.11 Low Power Cluster (0x0508)
Rev:1 | Role:Application | Scope:Endpoint | PICS:LOWPOWER

### Commands
| ID | Name | Dir | Response | Access | Conf |
|----|------|-----|----------|--------|------|
|0x00|Sleep|C→S|Y|O|M|

---
## 1.12 Wake On LAN Cluster (0x0503)
Rev:1 | Role:Application | Scope:Endpoint | PICS:WAKEONLAN

### Attributes
| ID | Name | Type | Constraint | Quality | Default | Access | Conf |
|----|------|------|------------|---------|---------|--------|------|
|0x0000|MACAddress|string|max32|F||R V|O|
|0x0001|LinkLocalAddress|ipv6adr||F||R V|O|

---
## 1.13 Switch Cluster (0x003B)
Rev:2 | Role:Application | Scope:Endpoint | PICS:SWTCH

### Features
| Bit | Code | Feature | Conf | Summary |
|-----|------|---------|------|---------|
|0|LS|LatchingSwitch|[!MS]|Latching switch|
|1|MS|MomentarySwitch|[!LS]|Momentary switch|
|2|MSR|MomentarySwitchRelease|[MS]|Release detection|
|3|MSL|MomentarySwitchLongPress|[MS&MSR]|Long press detection|
|4|MSM|MomentarySwitchMultiPress|[MS&MSR]|Multi-press detection|
|5|AS|ActionSwitch|[MSM&!MSL]|Reduced events|

### Attributes
| ID | Name | Type | Constraint | Quality | Default | Access | Conf |
|----|------|------|------------|---------|---------|--------|------|
|0x0000|NumberOfPositions|uint8|min2|F|2|R V|M|
|0x0001|CurrentPosition|uint8|0-(NumberOfPositions-1)||0|R V|M|
|0x0002|MultiPressMax|uint8|min2|F|2|R V|MSM|

### Events
| ID | Name | Priority | Access | Conf |
|----|------|----------|--------|------|
|0x00|SwitchLatched|INFO|V|LS|
|0x01|InitialPress|INFO|V|MS|
|0x02|LongPress|INFO|V|MSL|
|0x03|ShortRelease|INFO|V|MSR&!AS|
|0x04|LongRelease|INFO|V|MSL|
|0x05|MultiPressOngoing|INFO|V|MSM&!AS|
|0x06|MultiPressComplete|INFO|V|MSM|

Event Fields:
- SwitchLatched: {NewPosition:uint8}
- InitialPress: {NewPosition:uint8}
- LongPress: {NewPosition:uint8}
- ShortRelease: {PreviousPosition:uint8}
- LongRelease: {PreviousPosition:uint8}
- MultiPressOngoing: {NewPosition:uint8, CurrentNumberOfPressesCounted:uint8}
- MultiPressComplete: {PreviousPosition:uint8, TotalNumberOfPressesCounted:uint8}

---
## 1.14 Operational State Cluster (0x0060)
Rev:3 | Role:Application | Scope:Endpoint | PICS:OPSTATE

### Data Types
```
OperationalStateEnum(enum8): Stopped=0,Running=1,Paused=2,Error=3 (0x40-0x7F=Derived,0x80-0xBF=MfgSpecific)
OperationalStateStruct: {OperationalStateID:OperationalStateEnum, OperationalStateLabel:string(max64,desc)}
ErrorStateEnum(enum8): NoError=0,UnableToStartOrResume=1,UnableToCompleteOperation=2,CommandInvalidInState=3
ErrorStateStruct: {ErrorStateID:ErrorStateEnum, ErrorStateLabel:string(max64,desc), ErrorStateDetails:string(max64,O)}
```

### Attributes
| ID | Name | Type | Constraint | Quality | Default | Access | Conf |
|----|------|------|------------|---------|---------|--------|------|
|0x0000|PhaseList|list[string]|max32[max64]|X|MS|R V|M|
|0x0001|CurrentPhase|uint8|max(PhaseList-1)|X|null|R V|M|
|0x0002|CountdownTime|elapsed-s|all|XQ|null|R V|O|
|0x0003|OperationalStateList|list[OperationalStateStruct]|all|||R V|M|
|0x0004|OperationalState|OperationalStateEnum|desc|||R V|M|
|0x0005|OperationalError|ErrorStateStruct|desc|||R V|M|

### Commands
| ID | Name | Dir | Response | Access | Conf |
|----|------|-----|----------|--------|------|
|0x00|Pause|C→S|OperationalCommandResponse|O|O|
|0x01|Stop|C→S|OperationalCommandResponse|O|O|
|0x02|Start|C→S|OperationalCommandResponse|O|O|
|0x03|Resume|C→S|OperationalCommandResponse|O|O|

OperationalCommandResponse: {CommandResponseState:ErrorStateStruct}

### Events
| ID | Name | Priority | Access | Conf |
|----|------|----------|--------|------|
|0x00|OperationalError|CRIT|V|M|
|0x01|OperationCompletion|INFO|V|O|

---
## 1.15 Alarm Base Cluster (Base for derived clusters)
Rev:1 | Role:Application | Scope:Endpoint | PICS:ALARM

### Features
| Bit | Code | Feature | Summary |
|-----|------|---------|---------|
|0|RESET|Reset|Supports Reset command|

### Data Types
```
AlarmBitmap(map32): Derived cluster defines bits
```

### Attributes
| ID | Name | Type | Constraint | Quality | Default | Access | Conf |
|----|------|------|------------|---------|---------|--------|------|
|0x0000|Mask|AlarmBitmap|all||0|RW VO|M|
|0x0001|Latch|AlarmBitmap|all|F|0|R V|O|
|0x0002|State|AlarmBitmap|all||0|R V|M|
|0x0003|Supported|AlarmBitmap|all|F|0|R V|M|

### Commands
| ID | Name | Dir | Response | Access | Conf |
|----|------|-----|----------|--------|------|
|0x00|Reset|C→S|Y|O|RESET|

Reset: {Alarms:AlarmBitmap}

### Events
| ID | Name | Priority | Access | Conf |
|----|------|----------|--------|------|
|0x00|Notify|INFO|V|M|

---
## 1.16 Messages Cluster (0x0097)
Rev:1 | Role:Application | Scope:Endpoint | PICS:MESS

### Features
| Bit | Code | Feature | Conf | Summary |
|-----|------|---------|------|---------|
|0|CONF|ReceivedConfirmation|O|Confirmation support|
|1|RESP|Response|[CONF]|Response support|
|2|RPLY|Reply|O|Reply support|
|3|PROT|ProtectedMessages|O|Protected messages|

### Data Types
```
FutureBitmap(map32): Reserved
MessagePriorityEnum(enum8): Low=0,Medium=1,High=2,Critical=3
MessageControlBitmap(map8): ConfirmationRequired=Bit0,ResponseRequired=Bit1,ReplyMessage=Bit2,MessageConfirmed=Bit3,MessageProtected=Bit4
MessageResponseOptionStruct: {MessageResponseID:uint32(O), Label:string(max32,O)}
MessageStruct: {MessageID:octstr(max16), Priority:MessagePriorityEnum, MessageControl:MessageControlBitmap, StartTime:epoch-s|X, Duration:elapsed-ms|X, MessageText:string(max256), Responses:list[MessageResponseOptionStruct](max4,O)}
```

### Attributes
| ID | Name | Type | Constraint | Quality | Default | Access | Conf |
|----|------|------|------------|---------|---------|--------|------|
|0x0000|Messages|list[MessageStruct]|max8||empty|R V|M|
|0x0001|ActiveMessageIDs|list[octstr]|max8||empty|R V|M|

### Commands
| ID | Name | Dir | Response | Access | Conf |
|----|------|-----|----------|--------|------|
|0x00|PresentMessagesRequest|C→S|Y|O|M|
|0x01|CancelMessagesRequest|C→S|Y|O|M|

### Events
| ID | Name | Priority | Access | Conf |
|----|------|----------|--------|------|
|0x00|MessageQueued|INFO|V|M|
|0x01|MessagePresented|INFO|V|M|
|0x02|MessageComplete|INFO|V|M|

---
## 1.17 Service Area Cluster (0x0150)
Rev:1 | Role:Application | Scope:Endpoint | PICS:SEAR

### Features
| Bit | Code | Feature | Conf | Summary |
|-----|------|---------|------|---------|
|0|SELRUN|SelectWhileRunning|O|Select areas while running|
|1|PROG|ProgressReporting|O|Progress reporting|
|2|MAPS|Maps|O|Map support|

### Data Types
```
OperationalStatusEnum(enum8): Pending=0,Operating=1,Skipped=2,Completed=3
AreaStruct: {AreaID:uint32, MapID:uint32|X(MAPS), AreaInfo:AreaInfoStruct}
MapStruct: {MapID:uint32, Name:string(max64)}
ProgressStruct: {AreaID:uint32, Status:OperationalStatusEnum, TotalOperationalTime:elapsed-s|X(O), InitialTimeEstimate:elapsed-s|X(O)}
```

### Attributes
| ID | Name | Type | Constraint | Quality | Default | Access | Conf |
|----|------|------|------------|---------|---------|--------|------|
|0x0000|SupportedAreas|list[AreaStruct]|max99|||R V|M|
|0x0001|SupportedMaps|list[MapStruct]|max99|X||R V|MAPS|
|0x0002|SelectedAreas|list[uint32]|max99|||R V|M|
|0x0003|CurrentArea|uint32||X|null|R V|O|
|0x0004|EstimatedEndTime|epoch-s||XQ|null|R V|O|
|0x0005|Progress|list[ProgressStruct]|max99|||R V|PROG|

### Commands
| ID | Name | Dir | Response | Access | Conf |
|----|------|-----|----------|--------|------|
|0x00|SelectAreas|C→S|SelectAreasResponse|O|M|
|0x02|SkipArea|C→S|SkipAreaResponse|O|SELRUN&PROG|

---
# CHAPTER 2: MEASUREMENT AND SENSING

## 2.2 Illuminance Measurement Cluster (0x0400)
Rev:3 | Role:Application | Scope:Endpoint | PICS:ILL

### Data Types
```
LightSensorTypeEnum(enum8): Photodiode=0,CMOS=1,64-254=MfgSpecific
```

### Attributes
| ID | Name | Type | Constraint | Quality | Default | Access | Conf |
|----|------|------|------------|---------|---------|--------|------|
|0x0000|MeasuredValue|uint16|0,MinMeasuredValue-MaxMeasuredValue|PX|0|R V|M|
|0x0001|MinMeasuredValue|uint16|1-65533|X||R V|M|
|0x0002|MaxMeasuredValue|uint16|min(MinMeasuredValue+1)|X||R V|M|
|0x0003|Tolerance|uint16|max2048|||R V|O|
|0x0004|LightSensorType|LightSensorTypeEnum|all|X|null|R V|O|

MeasuredValue = 10000 × log10(illuminance) + 1, where 1 lx ≤ illuminance ≤ 3.576 Mlx

---
## 2.3 Temperature Measurement Cluster (0x0402)
Rev:4 | Role:Application | Scope:Endpoint | PICS:TMP

### Attributes
| ID | Name | Type | Constraint | Quality | Default | Access | Conf |
|----|------|------|------------|---------|---------|--------|------|
|0x0000|MeasuredValue|temperature|MinMeasuredValue-MaxMeasuredValue|XP||R V|M|
|0x0001|MinMeasuredValue|temperature|-27315-32766|X||R V|M|
|0x0002|MaxMeasuredValue|temperature|min(MinMeasuredValue+1)|X||R V|M|
|0x0003|Tolerance|uint16|max2048||0|R V|O|

Temperature in 0.01°C units (e.g., 2500 = 25.00°C)

---
## 2.4 Pressure Measurement Cluster (0x0403)
Rev:3 | Role:Application | Scope:Endpoint | PICS:PRS

### Features
| Bit | Code | Feature | Conf | Summary |
|-----|------|---------|------|---------|
|0|EXT|Extended|O|Extended range/resolution|

### Attributes
| ID | Name | Type | Constraint | Quality | Default | Access | Conf |
|----|------|------|------------|---------|---------|--------|------|
|0x0000|MeasuredValue|int16|MinMeasuredValue-MaxMeasuredValue|XP||R V|M|
|0x0001|MinMeasuredValue|int16|max32766|X||R V|M|
|0x0002|MaxMeasuredValue|int16|(MinMeasuredValue+1)-32767|X||R V|M|
|0x0003|Tolerance|uint16|max2048||0|R V|O|
|0x0010|ScaledValue|int16|MinScaledValue-MaxScaledValue|X|0|R V|EXT|
|0x0011|MinScaledValue|int16|max32766|X|0|R V|EXT|
|0x0012|MaxScaledValue|int16|(MinScaledValue+1)-32767|X|0|R V|EXT|
|0x0013|ScaledTolerance|uint16|max2048||0|R V|[EXT]|
|0x0014|Scale|int8|min-127||0|R V|EXT|

MeasuredValue = 10 × Pressure[kPa]; ScaledValue = 10^Scale × Pressure[Pa]

---
## 2.5 Flow Measurement Cluster (0x0404)
Rev:3 | Role:Application | Scope:Endpoint | PICS:FLW

### Attributes
| ID | Name | Type | Constraint | Quality | Default | Access | Conf |
|----|------|------|------------|---------|---------|--------|------|
|0x0000|MeasuredValue|uint16|MinMeasuredValue-MaxMeasuredValue|XP|null|R V|M|
|0x0001|MinMeasuredValue|uint16|max65533|X||R V|M|
|0x0002|MaxMeasuredValue|uint16|min(MinMeasuredValue+1)|X||R V|M|
|0x0003|Tolerance|uint16|max2048||0|R V|O|

MeasuredValue = 10 × Flow[m³/h]

---
## 2.6 Relative Humidity Measurement Cluster (0x0405)
Rev:3 | Role:Application | Scope:Endpoint | PICS:RH

### Attributes
| ID | Name | Type | Constraint | Quality | Default | Access | Conf |
|----|------|------|------------|---------|---------|--------|------|
|0x0000|MeasuredValue|uint16|MinMeasuredValue-MaxMeasuredValue|XP||R V|M|
|0x0001|MinMeasuredValue|uint16|max9999|X||R V|M|
|0x0002|MaxMeasuredValue|uint16|(MinMeasuredValue+1)-10000|X||R V|M|
|0x0003|Tolerance|uint16|max2048|||R V|O|

MeasuredValue = 100 × humidity[%] (0-10000 = 0-100%)

---
## 2.7 Occupancy Sensing Cluster (0x0406)
Rev:5 | Role:Application | Scope:Endpoint | PICS:OCC

### Features
| Bit | Code | Feature | Conf | Summary |
|-----|------|---------|------|---------|
|0|OTHER|Other|O.a+|Other sensing|
|1|PIR|PIR|O.a+|PIR sensing|
|2|US|Ultrasonic|O.a+|Ultrasonic sensing|
|3|PHY|PhysicalContact|O.a+|Physical contact|
|4|AIR|ActiveInfrared|O|Active infrared|
|5|RAD|Radar|O|Radar sensing|
|6|RFS|RFSensing|O|RF sensing|
|7|VIS|Vision|O|Vision-based|

### Data Types
```
OccupancyBitmap(map8): Occupied=Bit0
OccupancySensorTypeEnum(enum8): PIR=0,Ultrasonic=1,PIRAndUltrasonic=2,PhysicalContact=3
OccupancySensorTypeBitmap(map8): PIR=Bit0,Ultrasonic=Bit1,PhysicalContact=Bit2
```

### Attributes
| ID | Name | Type | Constraint | Quality | Default | Access | Conf |
|----|------|------|------------|---------|---------|--------|------|
|0x0000|Occupancy|OccupancyBitmap|all|P||R V|M|
|0x0001|OccupancySensorType|OccupancySensorTypeEnum|desc|F||R V|M|
|0x0002|OccupancySensorTypeBitmap|OccupancySensorTypeBitmap|desc|F||R V|M|
|0x0003|HoldTime|uint16|all|N||RW VM|O|
|0x0004|HoldTimeLimits|HoldTimeLimitsStruct||F||R V|[HoldTime]|
|0x0010|PIROccupiedToUnoccupiedDelay|uint16|all|N|0|RW VM|[PIR&HoldTime]|
|0x0011|PIRUnoccupiedToOccupiedDelay|uint16|all|N|0|RW VM|[PIR&HoldTime]|
|0x0012|PIRUnoccupiedToOccupiedThreshold|uint8|1-254|N|1|RW VM|[PIR&HoldTime]|
|0x0020|UltrasonicOccupiedToUnoccupiedDelay|uint16|all|N|0|RW VM|[US&HoldTime]|
|0x0021|UltrasonicUnoccupiedToOccupiedDelay|uint16|all|N|0|RW VM|[US&HoldTime]|
|0x0022|UltrasonicUnoccupiedToOccupiedThreshold|uint8|1-254|N|1|RW VM|[US&HoldTime]|
|0x0030|PhysicalContactOccupiedToUnoccupiedDelay|uint16|all|N|0|RW VM|[PHY&HoldTime]|
|0x0031|PhysicalContactUnoccupiedToOccupiedDelay|uint16|all|N|0|RW VM|[PHY&HoldTime]|
|0x0032|PhysicalContactUnoccupiedToOccupiedThreshold|uint8|1-254|N|1|RW VM|[PHY&HoldTime]|

### Events
| ID | Name | Priority | Access | Conf |
|----|------|----------|--------|------|
|0x00|OccupancyChanged|INFO|V|O|

---
## 2.8 Resource Monitoring Clusters
Base cluster for filter/resource monitoring. Derived clusters:
- HEPA Filter Monitoring (0x0071)
- Activated Carbon Filter Monitoring (0x0072)

Rev:1 | Role:Application | Scope:Endpoint

### Features
| Bit | Code | Feature | Conf | Summary |
|-----|------|---------|------|---------|
|0|CON|Condition|O.a+|Condition monitoring|
|1|WRN|Warning|O.a+|Warning threshold|
|2|REP|ReplacementProductList|O|Replacement products|

### Data Types
```
ChangeIndicationEnum(enum8): OK=0,Warning=1,Critical=2
DegradationDirectionEnum(enum8): Up=0,Down=1
ProductIdentifierTypeEnum(enum8): UPC=0,GTIN8=1,EAN=2,GTIN14=3,OEM=4
ReplacementProductStruct: {ProductIdentifierType:ProductIdentifierTypeEnum, ProductIdentifierValue:string(max20)}
```

### Attributes
| ID | Name | Type | Constraint | Quality | Default | Access | Conf |
|----|------|------|------------|---------|---------|--------|------|
|0x0000|Condition|percent|all|P||R V|CON|
|0x0001|DegradationDirection|DegradationDirectionEnum|desc|F||R V|CON|
|0x0002|ChangeIndication|ChangeIndicationEnum|desc||0|R V|M|
|0x0003|InPlaceIndicator|bool|all|||R V|O|
|0x0004|LastChangedTime|epoch-s|all|XN|null|RW VM|O|
|0x0005|ReplacementProductList|list[ReplacementProductStruct]|max5|F||R V|REP|

### Commands
| ID | Name | Dir | Response | Access | Conf |
|----|------|-----|----------|--------|------|
|0x00|ResetCondition|C→S|Y|O|O|

---
## 2.9 Air Quality Cluster (0x005B)
Rev:1 | Role:Application | Scope:Endpoint | PICS:AIRQUAL

### Features
| Bit | Code | Feature | Conf | Summary |
|-----|------|---------|------|---------|
|0|FAIR|Fair|O|Fair level|
|1|MOD|Moderate|O|Moderate level|
|2|VPOOR|VeryPoor|O|Very poor level|
|3|XPOOR|ExtremelyPoor|O|Extremely poor level|

### Data Types
```
AirQualityEnum(enum8): Unknown=0,Good=1,Fair=2,Moderate=3,Poor=4,VeryPoor=5,ExtremelyPoor=6
```

### Attributes
| ID | Name | Type | Constraint | Quality | Default | Access | Conf |
|----|------|------|------------|---------|---------|--------|------|
|0x0000|AirQuality|AirQualityEnum|desc|||R V|M|

---
## 2.10 Concentration Measurement Clusters
Base cluster for concentration measurements. Derived clusters:
- Carbon Monoxide (0x040C), Carbon Dioxide (0x040D), Nitrogen Dioxide (0x0413)
- Ozone (0x0415), Formaldehyde (0x042B), PM1 (0x042C), PM2.5 (0x042A), PM10 (0x042D)
- Radon (0x042F), Total VOC (0x042E)

Rev:3 | Role:Application | Scope:Endpoint

### Features
| Bit | Code | Feature | Conf | Summary |
|-----|------|---------|------|---------|
|0|MEA|NumericMeasurement|O.a+|Numeric measurement|
|1|LEV|LevelIndication|O.a+|Level indication|
|2|MED|MediumLevel|[LEV]|Medium level|
|3|CRI|CriticalLevel|[LEV]|Critical level|
|4|PEA|PeakMeasurement|[MEA]|Peak measurement|
|5|AVG|AverageMeasurement|[MEA]|Average measurement|

### Data Types
```
MeasurementUnitEnum(enum8): PPM=0,PPB=1,PPT=2,MGM3=3,UGM3=4,NGM3=5,PM3=6,BQM3=7,PCIL=8
MeasurementMediumEnum(enum8): Air=0,Water=1,Soil=2
LevelValueEnum(enum8): Unknown=0,Low=1,Medium=2,High=3,Critical=4
```

### Attributes
| ID | Name | Type | Constraint | Quality | Default | Access | Conf |
|----|------|------|------------|---------|---------|--------|------|
|0x0000|MeasuredValue|single|MinMeasuredValue-MaxMeasuredValue|XPQ|null|R V|MEA|
|0x0001|MinMeasuredValue|single|all|X|null|R V|MEA|
|0x0002|MaxMeasuredValue|single|all|X|null|R V|MEA|
|0x0003|PeakMeasuredValue|single|MinMeasuredValue-MaxMeasuredValue|XQ|null|R V|PEA|
|0x0004|PeakMeasuredValueWindow|elapsed-s|max604800|F|1|R V|PEA|
|0x0005|AverageMeasuredValue|single|MinMeasuredValue-MaxMeasuredValue|XQ|null|R V|AVG|
|0x0006|AverageMeasuredValueWindow|elapsed-s|max604800|F|1|R V|AVG|
|0x0007|Uncertainty|single|all||0|R V|MEA|
|0x0008|MeasurementUnit|MeasurementUnitEnum|desc|F||R V|MEA|
|0x0009|MeasurementMedium|MeasurementMediumEnum|desc|F||R V|MEA|
|0x000A|LevelValue|LevelValueEnum|desc|||R V|LEV|

---
## 2.11 Smoke CO Alarm Cluster (0x005C)
Rev:1 | Role:Application | Scope:Endpoint | PICS:SMOKECO

### Features
| Bit | Code | Feature | Conf | Summary |
|-----|------|---------|------|---------|
|0|SMOKE|SmokeAlarm|O.a+|Smoke alarm|
|1|CO|COAlarm|O.a+|CO alarm|

### Data Types
```
AlarmStateEnum(enum8): Normal=0,Warning=1,Critical=2
SensitivityEnum(enum8): High=0,Standard=1,Low=2
MuteStateEnum(enum8): NotMuted=0,Muted=1
EndOfServiceEnum(enum8): Normal=0,Expired=1
ContaminationStateEnum(enum8): Normal=0,Low=1,Warning=2,Critical=3
ExpressedStateEnum(enum8): Normal=0,SmokeAlarm=1,COAlarm=2,BatteryAlert=3,Testing=4,HardwareFault=5,EndOfService=6,InterconnectSmoke=7,InterconnectCO=8
```

### Attributes
| ID | Name | Type | Constraint | Quality | Default | Access | Conf |
|----|------|------|------------|---------|---------|--------|------|
|0x0000|ExpressedState|ExpressedStateEnum|desc|||R V|M|
|0x0001|SmokeState|AlarmStateEnum|desc|||R V|SMOKE|
|0x0002|COState|AlarmStateEnum|desc|||R V|CO|
|0x0003|BatteryAlert|AlarmStateEnum|desc|||R V|M|
|0x0004|DeviceMuted|MuteStateEnum|desc|||R V|O|
|0x0005|TestInProgress|bool|all|||R V|M|
|0x0006|HardwareFaultAlert|bool|all|||R V|M|
|0x0007|EndOfServiceAlert|EndOfServiceEnum|desc|||R V|M|
|0x0008|InterconnectSmokeAlarm|AlarmStateEnum|desc|||R V|O|
|0x0009|InterconnectCOAlarm|AlarmStateEnum|desc|||R V|O|
|0x000A|ContaminationState|ContaminationStateEnum|desc|||R V|O|
|0x000B|SmokeSensitivityLevel|SensitivityEnum|desc|N||RW VO|O|
|0x000C|ExpiryDate|epoch-s|all|F||R V|O|

### Commands
| ID | Name | Dir | Response | Access | Conf |
|----|------|-----|----------|--------|------|
|0x00|SelfTestRequest|C→S|Y|O|M|

### Events
| ID | Name | Priority | Access | Conf |
|----|------|----------|--------|------|
|0x00|SmokeAlarm|CRIT|V|SMOKE|
|0x01|COAlarm|CRIT|V|CO|
|0x02|LowBattery|INFO|V|M|
|0x03|HardwareFault|INFO|V|M|
|0x04|EndOfService|INFO|V|M|
|0x05|SelfTestComplete|INFO|V|M|
|0x06|AlarmMuted|INFO|V|O|
|0x07|MuteEnded|INFO|V|O|
|0x08|InterconnectSmokeAlarm|INFO|V|O|
|0x09|InterconnectCOAlarm|INFO|V|O|
|0x0A|AllClear|INFO|V|M|

---
## 2.12 Electrical Energy Measurement Cluster (0x0091)
Rev:1 | Role:Application | Scope:Endpoint | PICS:EEM

### Features
| Bit | Code | Feature | Conf | Summary |
|-----|------|---------|------|---------|
|0|IMPE|ImportedEnergy|O.a+|Imported energy|
|1|EXPE|ExportedEnergy|O.a+|Exported energy|
|2|CUME|CumulativeEnergy|O.b+|Cumulative measurement|
|3|PERE|PeriodicEnergy|O.b+|Periodic measurement|

### Data Types
```
EnergyMeasurementStruct: {Energy:energy-mWh, StartTimestamp:epoch-s(O), EndTimestamp:epoch-s(O), StartSystime:systime-ms(O), EndSystime:systime-ms(O)}
CumulativeEnergyResetStruct: {ImportedResetTimestamp:epoch-s|X(O), ExportedResetTimestamp:epoch-s|X(O), ImportedResetSystime:systime-ms|X(O), ExportedResetSystime:systime-ms|X(O)}
```

### Attributes
| ID | Name | Type | Constraint | Quality | Default | Access | Conf |
|----|------|------|------------|---------|---------|--------|------|
|0x0000|Accuracy|MeasurementAccuracyStruct||F||R V|M|
|0x0001|CumulativeEnergyImported|EnergyMeasurementStruct||XQ|null|R V|CUME&IMPE|
|0x0002|CumulativeEnergyExported|EnergyMeasurementStruct||XQ|null|R V|CUME&EXPE|
|0x0003|PeriodicEnergyImported|EnergyMeasurementStruct||XQ|null|R V|PERE&IMPE|
|0x0004|PeriodicEnergyExported|EnergyMeasurementStruct||XQ|null|R V|PERE&EXPE|
|0x0005|CumulativeEnergyReset|CumulativeEnergyResetStruct||X|null|R V|[CUME]|

### Events
| ID | Name | Priority | Access | Conf |
|----|------|----------|--------|------|
|0x00|CumulativeEnergyMeasured|INFO|V|CUME|
|0x01|PeriodicEnergyMeasured|INFO|V|PERE|

---
## 2.13 Electrical Power Measurement Cluster (0x0090)
Rev:1 | Role:Application | Scope:Endpoint | PICS:EPM

### Features
| Bit | Code | Feature | Conf | Summary |
|-----|------|---------|------|---------|
|0|DIRC|DirectCurrent|O.a+|DC measurement|
|1|ALTC|AlternatingCurrent|O.a+|AC measurement|
|2|POLY|PolyphasePower|[ALTC]|Polyphase|
|3|HARM|Harmonics|[ALTC]|Harmonics|
|4|PWRQ|PowerQuality|[ALTC]|Power quality|

### Data Types
```
PowerModeEnum(enum8): Unknown=0,DC=1,AC=2
MeasurementRangeStruct: {MeasurementType:MeasurementTypeEnum, Min:int64, Max:int64, StartTimestamp:epoch-s(O), EndTimestamp:epoch-s(O), MinTimestamp:epoch-s(O), MaxTimestamp:epoch-s(O), StartSystime:systime-ms(O), EndSystime:systime-ms(O), MinSystime:systime-ms(O), MaxSystime:systime-ms(O)}
HarmonicMeasurementStruct: {Order:uint8(min1), Measurement:int64|X}
```

### Attributes
| ID | Name | Type | Constraint | Quality | Default | Access | Conf |
|----|------|------|------------|---------|---------|--------|------|
|0x0000|PowerMode|PowerModeEnum|||||R V|M|
|0x0001|NumberOfMeasurementTypes|uint8|min1|F||R V|M|
|0x0002|Accuracy|list[MeasurementAccuracyStruct]|1-NumberOfMeasurementTypes|F||R V|M|
|0x0003|Ranges|list[MeasurementRangeStruct]|0-NumberOfMeasurementTypes|Q|empty|R V|O|
|0x0004|Voltage|voltage-mV|-2^62-2^62|XQ|null|R V|O|
|0x0005|ActiveCurrent|amperage-mA|-2^62-2^62|XQ|null|R V|O|
|0x0006|ReactiveCurrent|amperage-mA|-2^62-2^62|XQ|null|R V|[ALTC]|
|0x0007|ApparentCurrent|amperage-mA|0-2^62|XQ|null|R V|[ALTC]|
|0x0008|ActivePower|power-mW|-2^62-2^62|XQ|null|R V|M|
|0x0009|ReactivePower|power-mW|-2^62-2^62|XQ|null|R V|[ALTC]|
|0x000A|ApparentPower|power-mW|-2^62-2^62|XQ|null|R V|[ALTC]|
|0x000B|RMSVoltage|voltage-mV|-2^62-2^62|XQ|null|R V|[ALTC]|
|0x000C|RMSCurrent|amperage-mA|-2^62-2^62|XQ|null|R V|[ALTC]|
|0x000D|RMSPower|power-mW|-2^62-2^62|XQ|null|R V|[ALTC]|
|0x000E|Frequency|int64|0-1000000|XQ|null|R V|[ALTC]|
|0x000F|HarmonicCurrents|list[HarmonicMeasurementStruct]|desc|XQ|null|R V|HARM|
|0x0010|HarmonicPhases|list[HarmonicMeasurementStruct]|desc|XQ|null|R V|PWRQ|
|0x0011|PowerFactor|int64|-10000-10000|XQ|null|R V|[ALTC]|
|0x0012|NeutralCurrent|amperage-mA|-2^62-2^62|XQ|null|R V|[POLY]|

### Events
| ID | Name | Priority | Access | Conf |
|----|------|----------|--------|------|
|0x00|MeasurementPeriodRanges|INFO|V|Ranges|

---
# CHAPTER 3: LIGHTING

## 3.2 Color Control Cluster (0x0300)
Rev:7 | Role:Application | Scope:Endpoint | PICS:CC

### Features
| Bit | Code | Feature | Conf | Summary |
|-----|------|---------|------|---------|
|0|HS|HueSaturation|O.a+|Hue/Saturation|
|1|EHUE|EnhancedHue|[HS]|Enhanced hue|
|2|CL|ColorLoop|[EHUE]|Color loop|
|3|XY|XY|O.a+|CIE 1931 XY|
|4|CT|ColorTemperature|O.a+|Color temperature|

### Data Types
```
HueDirectionEnum(enum8): ShortestDistance=0,LongestDistance=1,Up=2,Down=3
HueMoveDirectionEnum(enum8): Stop=0,Up=1,Down=2
SaturationMoveDirectionEnum(enum8): Stop=0,Up=1,Down=2
ColorModeEnum(enum8): CurrentHueAndCurrentSaturation=0,CurrentXAndCurrentY=1,ColorTemperatureMireds=2
EnhancedColorModeEnum(enum8): CurrentHueAndCurrentSaturation=0,CurrentXAndCurrentY=1,ColorTemperatureMireds=2,EnhancedCurrentHueAndCurrentSaturation=3
ColorLoopDirectionEnum(enum8): DecrementHue=0,IncrementHue=1
ColorLoopActionEnum(enum8): Deactivate=0,ActivateFromColorLoopStartEnhancedHue=1,ActivateFromEnhancedCurrentHue=2
ColorLoopUpdateFlagsBitmap(map8): UpdateAction=Bit0,UpdateDirection=Bit1,UpdateTime=Bit2,UpdateStartHue=Bit3
OptionsBitmap(map8): ExecuteIfOff=Bit0
```

### Attributes
| ID | Name | Type | Constraint | Quality | Default | Access | Conf |
|----|------|------|------------|---------|---------|--------|------|
|0x0000|CurrentHue|uint8|0-254|QNS|0|R V|HS|
|0x0001|CurrentSaturation|uint8|0-254|QNS|0|R V|HS|
|0x0002|RemainingTime|uint16|all|Q|0|R V|O|
|0x0003|CurrentX|uint16|0-0xFEFF|QS|0x616B|R V|XY|
|0x0004|CurrentY|uint16|0-0xFEFF|QS|0x607D|R V|XY|
|0x0005|DriftCompensation|enum8|0-4|||R V|O|
|0x0006|CompensationText|string|max254|||R V|O|
|0x0007|ColorTemperatureMireds|uint16|ColorTempPhysicalMinMireds-ColorTempPhysicalMaxMireds|QNS||R V|CT|
|0x0008|ColorMode|ColorModeEnum|desc||1|R V|M|
|0x000F|Options|OptionsBitmap|all||0|RW VO|M|
|0x0010|NumberOfPrimaries|uint8|0-6|XF|null|R V|O|
|0x0011-0x0019|Primary1-3X/Y/Intensity|uint16/uint8|varies|F||R V|O|
|0x0020-0x0028|Primary4-6X/Y/Intensity|uint16/uint8|varies|F||R V|O|
|0x0030|WhitePointX|uint16|0-0xFEFF|||RW VM|O|
|0x0031|WhitePointY|uint16|0-0xFEFF|||RW VM|O|
|0x0032-0x0037|ColorPointR/G/BX/Y/Intensity|uint16/uint8|varies|||RW VM|O|
|0x4000|EnhancedCurrentHue|uint16|all|QS|0|R V|EHUE|
|0x4001|EnhancedColorMode|EnhancedColorModeEnum|desc|S|1|R V|M|
|0x4002|ColorLoopActive|uint8|all|S|0|R V|CL|
|0x4003|ColorLoopDirection|uint8|all|S|0|R V|CL|
|0x4004|ColorLoopTime|uint16|all|S|0x0019|R V|CL|
|0x4005|ColorLoopStartEnhancedHue|uint16|all||0x2300|R V|CL|
|0x4006|ColorLoopStoredEnhancedHue|uint16|all||0|R V|CL|
|0x400A|ColorCapabilities|map16|all|F|0|R V|M|
|0x400B|ColorTempPhysicalMinMireds|uint16|all|F|0|R V|CT|
|0x400C|ColorTempPhysicalMaxMireds|uint16|all|F|0xFEFF|R V|CT|
|0x400D|CoupleColorTempToLevelMinMireds|uint16|ColorTempPhysicalMinMireds-ColorTemperatureMireds|||R V|[CT&LT]|
|0x4010|StartUpColorTemperatureMireds|uint16|all|XN|MS|RW VM|[CT&LT]|

### Commands
| ID | Name | Dir | Response | Access | Conf |
|----|------|-----|----------|--------|------|
|0x00|MoveToHue|C→S|Y|O|HS|
|0x01|MoveHue|C→S|Y|O|HS|
|0x02|StepHue|C→S|Y|O|HS|
|0x03|MoveToSaturation|C→S|Y|O|HS|
|0x04|MoveSaturation|C→S|Y|O|HS|
|0x05|StepSaturation|C→S|Y|O|HS|
|0x06|MoveToHueAndSaturation|C→S|Y|O|HS|
|0x07|MoveToColor|C→S|Y|O|XY|
|0x08|MoveColor|C→S|Y|O|XY|
|0x09|StepColor|C→S|Y|O|XY|
|0x0A|MoveToColorTemperature|C→S|Y|O|CT|
|0x40|EnhancedMoveToHue|C→S|Y|O|EHUE|
|0x41|EnhancedMoveHue|C→S|Y|O|EHUE|
|0x42|EnhancedStepHue|C→S|Y|O|EHUE|
|0x43|EnhancedMoveToHueAndSaturation|C→S|Y|O|EHUE|
|0x44|ColorLoopSet|C→S|Y|O|CL|
|0x47|StopMoveStep|C→S|Y|O|HS\|XY\|CT|
|0x4B|MoveColorTemperature|C→S|Y|O|CT|
|0x4C|StepColorTemperature|C→S|Y|O|CT|

---
## 3.3 Ballast Configuration Cluster (0x0301) [PROVISIONAL]
Rev:5 | Role:Application | Scope:Endpoint | PICS:BC

### Attributes
| ID | Name | Type | Constraint | Quality | Default | Access | Conf |
|----|------|------|------------|---------|---------|--------|------|
|0x0000|PhysicalMinLevel|uint8|1-254|F|1|R V|M|
|0x0001|PhysicalMaxLevel|uint8|1-254|F|254|R V|M|
|0x0002|BallastStatus|map8|all||0|R V|O|
|0x0010|MinLevel|uint8|1-254|N|PhysicalMinLevel|RW VM|M|
|0x0011|MaxLevel|uint8|1-254|N|PhysicalMaxLevel|RW VM|M|
|0x0014|IntrinsicBallastFactor|uint8|all|XN|null|RW VM|O|
|0x0015|BallastFactorAdjustment|uint8|100-255|XN|null|RW VM|O|
|0x0020|LampQuantity|uint8|all|F||R V|O|
|0x0030|LampType|string|max16|N||RW VM|O|
|0x0031|LampManufacturer|string|max16|N||RW VM|O|
|0x0032|LampRatedHours|uint24|all|XN|null|RW VM|O|
|0x0033|LampBurnHours|uint24|all|XN|null|RW VM|O|
|0x0034|LampAlarmMode|map8|all|N|0|RW VM|O|
|0x0035|LampBurnHoursTripPoint|uint24|all|XN|null|RW VM|O|

---
# CHAPTER 4: HVAC

## 4.2 Pump Configuration and Control Cluster (0x0200)
Rev:4 | Role:Application | Scope:Endpoint | PICS:PCC

### Features
| Bit | Code | Feature | Conf | Summary |
|-----|------|---------|------|---------|
|0|PRSCONST|ConstantPressure|O.a+|Constant pressure|
|1|PRSCOMP|CompensatedPressure|O.a+|Compensated pressure|
|2|FLW|ConstantFlow|O.a+|Constant flow|
|3|SPD|ConstantSpeed|O.a+|Constant speed|
|4|TEMP|ConstantTemperature|O.a+|Constant temperature|
|5|AUTO|Automatic|O|Automatic mode|
|6|LOCAL|LocalOperation|O|Local operation|

### Data Types
```
PumpStatusBitmap(map16): DeviceFault=Bit0,SupplyFault=Bit1,SpeedLow=Bit2,SpeedHigh=Bit3,LocalOverride=Bit4,Running=Bit5,RemotePressure=Bit6,RemoteFlow=Bit7,RemoteTemperature=Bit8
OperationModeEnum(enum8): Normal=0,Minimum=1,Maximum=2,Local=3
ControlModeEnum(enum8): ConstantSpeed=0,ConstantPressure=1,ProportionalPressure=2,ConstantFlow=3,ConstantTemperature=5,Automatic=7
```

### Attributes
| ID | Name | Type | Constraint | Quality | Default | Access | Conf |
|----|------|------|------------|---------|---------|--------|------|
|0x0000|MaxPressure|int16|all|XF|null|R V|M|
|0x0001|MaxSpeed|uint16|all|XF|null|R V|M|
|0x0002|MaxFlow|uint16|all|XF|null|R V|M|
|0x0003|MinConstPressure|int16|all|XF|null|R V|[PRSCONST]|
|0x0004|MaxConstPressure|int16|all|XF|null|R V|[PRSCONST]|
|0x0005|MinCompPressure|int16|all|XF|null|R V|[PRSCOMP]|
|0x0006|MaxCompPressure|int16|all|XF|null|R V|[PRSCOMP]|
|0x0007|MinConstSpeed|uint16|all|XF|null|R V|[SPD]|
|0x0008|MaxConstSpeed|uint16|all|XF|null|R V|[SPD]|
|0x0009|MinConstFlow|uint16|all|XF|null|R V|[FLW]|
|0x000A|MaxConstFlow|uint16|all|XF|null|R V|[FLW]|
|0x000B|MinConstTemp|int16|all|XF|null|R V|[TEMP]|
|0x000C|MaxConstTemp|int16|all|XF|null|R V|[TEMP]|
|0x0010|PumpStatus|PumpStatusBitmap|all|P|0|R V|O|
|0x0011|EffectiveOperationMode|OperationModeEnum|desc|||R V|M|
|0x0012|EffectiveControlMode|ControlModeEnum|desc|||R V|M|
|0x0013|Capacity|int16|all|XPQ|null|R V|M|
|0x0014|Speed|uint16|all|X|null|R V|O|
|0x0015|LifetimeRunningHours|uint24|all|XNQ|0|RW VM|O|
|0x0016|Power|uint24|all|XQ|null|R V|O|
|0x0017|LifetimeEnergyConsumed|uint32|all|XNQ|0|RW VM|O|
|0x0020|OperationMode|OperationModeEnum|desc|N|0|RW VM|M|
|0x0021|ControlMode|ControlModeEnum|desc|N|0|RW VM|O|

### Events
| ID | Name | Priority | Access | Conf |
|----|------|----------|--------|------|
|0x00|SupplyVoltageLow|INFO|V|O|
|0x01|SupplyVoltageHigh|INFO|V|O|
|0x02|PowerMissingPhase|INFO|V|O|
|0x03|SystemPressureLow|INFO|V|O|
|0x04|SystemPressureHigh|INFO|V|O|
|0x05|DryRunning|CRIT|V|O|
|0x06|MotorTemperatureHigh|INFO|V|O|
|0x07|PumpMotorFatalFailure|CRIT|V|O|
|0x08|ElectronicTemperatureHigh|INFO|V|O|
|0x09|PumpBlocked|CRIT|V|O|
|0x0A|SensorFailure|INFO|V|O|
|0x0B|ElectronicNonFatalFailure|INFO|V|O|
|0x0C|ElectronicFatalFailure|CRIT|V|O|
|0x0D|GeneralFault|INFO|V|O|
|0x0E|Leakage|INFO|V|O|
|0x0F|AirDetection|INFO|V|O|
|0x10|TurbineOperation|INFO|V|O|

---
## 4.3 Thermostat Cluster (0x0201)
Rev:7 | Role:Application | Scope:Endpoint | PICS:TSTAT

### Features
| Bit | Code | Feature | Conf | Summary |
|-----|------|---------|------|---------|
|0|HEAT|Heating|O.a+|Heating|
|1|COOL|Cooling|O.a+|Cooling|
|2|OCC|Occupancy|O|Occupancy sensing|
|3|SCH|ScheduleConfiguration|O|Weekly schedule|
|4|SB|Setback|O|Setback support|
|5|AUTO|AutoMode|O|Auto mode|
|6|LTNE|LocalTemperatureNotExposed|O|Local temp not exposed|
|7|MSCH|MatterScheduleConfiguration|O|Matter schedules|
|8|PRES|Presets|O|Presets support|

### Data Types
```
SystemModeEnum(enum8): Off=0,Auto=1,Cool=3,Heat=4,EmergencyHeat=5,Precooling=6,FanOnly=7,Dry=8,Sleep=9
ThermostatRunningModeEnum(enum8): Off=0,Cool=3,Heat=4
ControlSequenceOfOperationEnum(enum8): CoolingOnly=0,CoolingWithReheat=1,HeatingOnly=2,HeatingWithReheat=3,CoolingAndHeating=4,CoolingAndHeatingWithReheat=5
SetpointRaiseLowerModeEnum(enum8): Heat=0,Cool=1,Both=2
TemperatureSetpointHoldEnum(enum8): SetpointHoldOff=0,SetpointHoldOn=1
PresetScenarioEnum(enum8): Occupied=1,Unoccupied=2,Sleep=3,Wake=4,Vacation=5,GoingToSleep=6,UserDefined=254
OccupancyBitmap(map8): Occupied=Bit0
ScheduleDayOfWeekBitmap(map8): Sunday-Saturday=Bit0-6,Away=Bit7
ScheduleModeBitmap(map8): HeatSetpointPresent=Bit0,CoolSetpointPresent=Bit1
PresetStruct: {PresetHandle:octstr(max16)|X, PresetScenario:PresetScenarioEnum, Name:string(max64)|X(O), CoolingSetpoint:temperature(COOL), HeatingSetpoint:temperature(HEAT), BuiltIn:bool|X}
ScheduleStruct: {ScheduleHandle:octstr(max16)|X, SystemMode:SystemModeEnum, Name:string(max64,O), PresetHandle:octstr(max16,O), Transitions:list[ScheduleTransitionStruct], BuiltIn:bool|X}
WeeklyScheduleTransitionStruct: {TransitionTime:uint16(max1439), HeatSetpoint:temperature|X, CoolSetpoint:temperature|X}
```

### Attributes (Key attributes)
| ID | Name | Type | Constraint | Quality | Default | Access | Conf |
|----|------|------|------------|---------|---------|--------|------|
|0x0000|LocalTemperature|temperature|all|XP|null|R V|M|
|0x0001|OutdoorTemperature|temperature|all|X|null|R V|O|
|0x0002|Occupancy|OccupancyBitmap|all||1|R V|OCC|
|0x0003|AbsMinHeatSetpointLimit|temperature|desc|F|7°C|R V|[HEAT]|
|0x0004|AbsMaxHeatSetpointLimit|temperature|desc|F|30°C|R V|[HEAT]|
|0x0005|AbsMinCoolSetpointLimit|temperature|desc|F|16°C|R V|[COOL]|
|0x0006|AbsMaxCoolSetpointLimit|temperature|desc|F|32°C|R V|[COOL]|
|0x0007|PICoolingDemand|uint8|0%-100%|P|-|R V|[COOL]|
|0x0008|PIHeatingDemand|uint8|0%-100%|P|-|R V|[HEAT]|
|0x0011|OccupiedCoolingSetpoint|temperature|desc|N|26°C|RW VO|COOL|
|0x0012|OccupiedHeatingSetpoint|temperature|desc|N|20°C|RW VO|HEAT|
|0x0013|UnoccupiedCoolingSetpoint|temperature|desc|N|26°C|RW VO|COOL&OCC|
|0x0014|UnoccupiedHeatingSetpoint|temperature|desc|N|20°C|RW VO|HEAT&OCC|
|0x0015|MinHeatSetpointLimit|temperature|desc|N|AbsMinHeatSetpointLimit|RW VM|[HEAT]|
|0x0016|MaxHeatSetpointLimit|temperature|desc|N|AbsMaxHeatSetpointLimit|RW VM|[HEAT]|
|0x0017|MinCoolSetpointLimit|temperature|desc|N|AbsMinCoolSetpointLimit|RW VM|[COOL]|
|0x0018|MaxCoolSetpointLimit|temperature|desc|N|AbsMaxCoolSetpointLimit|RW VM|[COOL]|
|0x0019|MinSetpointDeadBand|SignedTemperature|0-12.7°C|N|2.5°C|R[W] VM|AUTO|
|0x001B|ControlSequenceOfOperation|ControlSequenceOfOperationEnum|desc|N|4|RW VM|M|
|0x001C|SystemMode|SystemModeEnum|desc|N|1|RW VM|M|
|0x001E|ThermostatRunningMode|ThermostatRunningModeEnum|desc||0|R V|[AUTO]|
|0x0020|StartOfWeek|StartOfWeekEnum|desc|F|–|R V|SCH|
|0x0021|NumberOfWeeklyTransitions|uint8|all|F|0|R V|SCH|
|0x0022|NumberOfDailyTransitions|uint8|all|F|0|R V|SCH|
|0x0029|ThermostatRunningState|RelayStateBitmap|desc|-||R V|O|
|0x0048|PresetTypes|list[PresetTypeStruct]|desc|F|MS|R V|PRES|
|0x0049|ScheduleTypes|list[ScheduleTypeStruct]|desc|F|MS|R V|MSCH|
|0x004E|ActivePresetHandle|octstr|max16|XN|null|R V|PRES|
|0x004F|ActiveScheduleHandle|octstr|max16|XN|null|R V|MSCH|
|0x0050|Presets|list[PresetStruct]|max NumberOfPresets|NT|empty|RW VM|PRES|
|0x0051|Schedules|list[ScheduleStruct]|desc|NT|empty|RW VM|MSCH|

### Commands
| ID | Name | Dir | Response | Access | Conf |
|----|------|-----|----------|--------|------|
|0x00|SetpointRaiseLower|C→S|Y|O|M|
|0x01|SetWeeklySchedule|C→S|Y|M|SCH|
|0x02|GetWeeklySchedule|C→S|GetWeeklyScheduleResponse|O|SCH|
|0x03|ClearWeeklySchedule|C→S|Y|M|SCH|
|0x05|SetActiveScheduleRequest|C→S|Y|O|MSCH|
|0x06|SetActivePresetRequest|C→S|Y|O|PRES|
|0x07|AtomicRequest|C→S|AtomicResponse|O|PRES\|MSCH|

SetpointRaiseLower: {Mode:SetpointRaiseLowerModeEnum, Amount:int8}
SetWeeklySchedule: {NumberOfTransitionsForSequence:uint8, DayOfWeekForSequence:ScheduleDayOfWeekBitmap, ModeForSequence:ScheduleModeBitmap, Transitions:list[WeeklyScheduleTransitionStruct]}

---
## 4.4 Fan Control Cluster (0x0202)
Rev:4 | Role:Application | Scope:Endpoint | PICS:FAN

### Features
| Bit | Code | Feature | Conf | Summary |
|-----|------|---------|------|---------|
|0|SPD|MultiSpeed|O|Multi-speed|
|1|AUT|Auto|O|Auto mode|
|2|RCK|Rocking|O|Rocking|
|3|WND|Wind|O|Wind modes|
|4|STEP|Step|O|Step command|
|5|DIR|AirflowDirection|O|Airflow direction|

### Data Types
```
FanModeEnum(enum8): Off=0,Low=1,Medium=2,High=3,On=4,Auto=5,Smart=6
FanModeSequenceEnum(enum8): OffLowMedHigh=0,OffLowHigh=1,OffLowMedHighAuto=2,OffLowHighAuto=3,OffHighAuto=4,OffHigh=5
StepDirectionEnum(enum8): Increase=0,Decrease=1
AirflowDirectionEnum(enum8): Forward=0,Reverse=1
RockBitmap(map8): RockLeftRight=Bit0,RockUpDown=Bit1,RockRound=Bit2
WindBitmap(map8): SleepWind=Bit0,NaturalWind=Bit1
```

### Attributes
| ID | Name | Type | Constraint | Quality | Default | Access | Conf |
|----|------|------|------------|---------|---------|--------|------|
|0x0000|FanMode|FanModeEnum|desc|N|0|RW VO|M|
|0x0001|FanModeSequence|FanModeSequenceEnum|desc|F|2|R V|M|
|0x0002|PercentSetting|percent|all|XN|0|RW VO|M|
|0x0003|PercentCurrent|percent|all|P|0|R V|M|
|0x0004|SpeedMax|uint8|1-100|F|1|R V|SPD|
|0x0005|SpeedSetting|uint8|0-SpeedMax|XN|0|RW VO|SPD|
|0x0006|SpeedCurrent|uint8|0-SpeedMax|P|0|R V|SPD|
|0x0007|RockSupport|RockBitmap|all|F|0|R V|RCK|
|0x0008|RockSetting|RockBitmap|all|N|0|RW VO|RCK|
|0x0009|WindSupport|WindBitmap|all|F|0|R V|WND|
|0x000A|WindSetting|WindBitmap|all|N|0|RW VO|WND|
|0x000B|AirflowDirection|AirflowDirectionEnum|desc|N|0|RW VO|DIR|

### Commands
| ID | Name | Dir | Response | Access | Conf |
|----|------|-----|----------|--------|------|
|0x00|Step|C→S|Y|O|STEP|

Step: {Direction:StepDirectionEnum, Wrap:bool(O), LowestOff:bool(O)}

---
## 4.5 Thermostat User Interface Configuration Cluster (0x0204)
Rev:2 | Role:Application | Scope:Endpoint | PICS:TSUIC

### Data Types
```
TemperatureDisplayModeEnum(enum8): Celsius=0,Fahrenheit=1
KeypadLockoutEnum(enum8): NoLockout=0,Level1Lockout=1,Level2Lockout=2,Level3Lockout=3,Level4Lockout=4,Level5Lockout=5
ScheduleProgrammingVisibilityEnum(enum8): ScheduleProgrammingPermitted=0,ScheduleProgrammingDenied=1
```

### Attributes
| ID | Name | Type | Constraint | Quality | Default | Access | Conf |
|----|------|------|------------|---------|---------|--------|------|
|0x0000|TemperatureDisplayMode|TemperatureDisplayModeEnum|desc|N|0|RW VO|M|
|0x0001|KeypadLockout|KeypadLockoutEnum|desc|N|0|RW VM|M|
|0x0002|ScheduleProgrammingVisibility|ScheduleProgrammingVisibilityEnum|desc|N|0|RW VM|O|

---
## 4.6 Valve Configuration and Control Cluster (0x0081)
Rev:1 | Role:Application | Scope:Endpoint | PICS:VALCC

### Features
| Bit | Code | Feature | Conf | Summary |
|-----|------|---------|------|---------|
|0|TS|TimeSync|O|Time sync|
|1|LVL|Level|O|Level control|

### Data Types
```
ValveStateEnum(enum8): Closed=0,Open=1,Transitioning=2
StatusCodeEnum(enum8): FailureDueToFault=2
ValveFaultBitmap(map16): GeneralFault=Bit0,Blocked=Bit1,Leaking=Bit2,NotConnected=Bit3,ShortCircuit=Bit4,CurrentExceeded=Bit5
```

### Attributes
| ID | Name | Type | Constraint | Quality | Default | Access | Conf |
|----|------|------|------------|---------|---------|--------|------|
|0x0000|OpenDuration|elapsed-s|all|XN|null|RW VO|O|
|0x0001|DefaultOpenDuration|elapsed-s|all|XN|null|RW VM|O|
|0x0002|AutoCloseTime|epoch-us|all|X|null|R V|TS|
|0x0003|RemainingDuration|elapsed-s|all|XQ|null|R V|O|
|0x0004|CurrentState|ValveStateEnum|desc|X|null|R V|M|
|0x0005|TargetState|ValveStateEnum|desc|X|null|R V|M|
|0x0006|CurrentLevel|percent|all|XQ|null|R V|LVL|
|0x0007|TargetLevel|percent|all|X|null|R V|LVL|
|0x0008|DefaultOpenLevel|percent|1-100|N|100|RW VM|LVL|
|0x0009|ValveFault|ValveFaultBitmap|all||0|R V|O|
|0x000A|LevelStep|uint8|1-50|F|1|R V|LVL|

### Commands
| ID | Name | Dir | Response | Access | Conf |
|----|------|-----|----------|--------|------|
|0x00|Open|C→S|Y|O|M|
|0x01|Close|C→S|Y|O|M|

Open: {OpenDuration:elapsed-s|X(O), TargetLevel:percent(LVL)}

### Events
| ID | Name | Priority | Access | Conf |
|----|------|----------|--------|------|
|0x00|ValveStateChanged|INFO|V|O|
|0x01|ValveFault|INFO|V|O|

---
# CHAPTER 5: CLOSURES

## 5.2 Door Lock Cluster (0x0101)
Rev:7 | Role:Application | Scope:Endpoint | PICS:DRLK

### Features
| Bit | Code | Feature | Conf | Summary |
|-----|------|---------|------|---------|
|0|PIN|PINCredential|O|PIN credentials|
|1|RID|RFIDCredential|O|RFID credentials|
|2|FGP|FingerCredentials|O|Finger credentials|
|3|LOG|Logging|O|Audit logging|
|4|WDSCH|WeekDayAccessSchedules|O|Week day schedules|
|5|DPS|DoorPositionSensor|O|Door position sensor|
|6|FACE|FaceCredentials|O|Face credentials|
|7|COTA|CredentialOverTheAirAccess|O|OTA credential access|
|8|USR|User|[PIN\|RID\|FGP\|FACE]|User management|
|9|NOT|Notification|O|Notifications|
|10|YDSCH|YearDayAccessSchedules|O|Year day schedules|
|11|HDSCH|HolidaySchedules|O|Holiday schedules|
|12|UBOLT|Unbolt|O|Unbolt command|
|13|ALIRO|AliroProvisioning|O|Aliro support|
|14|ALBU|AliroBLEUWB|[ALIRO]|Aliro BLE/UWB|

### Data Types
```
AlarmCodeEnum(enum8): LockJammed=0,LockFactoryReset=1,LockRadioPowerCycled=3,WrongCodeEntryLimit=4,FrontEscutcheonRemoved=5,DoorForcedOpen=6,DoorAjar=7,ForcedUser=8
CredentialRuleEnum(enum8): Single=0,Dual=1,Tri=2
CredentialTypeEnum(enum8): ProgrammingPIN=0,PIN=1,RFID=2,Fingerprint=3,FingerVein=4,Face=5,AliroCredentialIssuerKey=6,AliroEvictableEndpointKey=7,AliroNonEvictableEndpointKey=8
DataOperationTypeEnum(enum8): Add=0,Clear=1,Modify=2
DlLockState(enum8): NotFullyLocked=0,Locked=1,Unlocked=2,Unlatched=3
DlLockType(enum8): DeadBolt=0,Magnetic=1,Other=2,Mortise=3,Rim=4,LatchBolt=5,CylindricalLock=6,TubularLock=7,InterconnectedLock=8,DeadLatch=9,DoorFurniture=10,Eurocylinder=11
DoorStateEnum(enum8): DoorOpen=0,DoorClosed=1,DoorJammed=2,DoorForcedOpen=3,DoorUnspecifiedError=4,DoorAjar=5
LockDataTypeEnum(enum8): Unspecified=0,ProgrammingCode=1,UserIndex=2,WeekDaySchedule=3,YearDaySchedule=4,HolidaySchedule=5,PIN=6,RFID=7,Fingerprint=8,FingerVein=9,Face=10,AliroCredentialIssuerKey=11,AliroEvictableEndpointKey=12,AliroNonEvictableEndpointKey=13
LockOperationTypeEnum(enum8): Lock=0,Unlock=1,NonAccessUserEvent=2,ForcedUserEvent=3,Unlatch=4
OperatingModeEnum(enum8): Normal=0,Vacation=1,Privacy=2,NoRemoteLockUnlock=3,Passage=4
OperationErrorEnum(enum8): Unspecified=0,InvalidCredential=1,DisabledUserDenied=2,Restricted=3,InsufficientBattery=4
OperationSourceEnum(enum8): Unspecified=0,Manual=1,ProprietaryRemote=2,Keypad=3,Auto=4,Button=5,Schedule=6,Remote=7,RFID=8,Biometric=9,Aliro=10
UserStatusEnum(enum8): Available=0,OccupiedEnabled=1,OccupiedDisabled=3
UserTypeEnum(enum8): UnrestrictedUser=0,YearDayScheduleUser=1,WeekDayScheduleUser=2,ProgrammingUser=3,NonAccessUser=4,ForcedUser=5,DisposableUser=6,ExpiringUser=7,ScheduleRestrictedUser=8,RemoteOnlyUser=9
CredentialStruct: {CredentialType:CredentialTypeEnum, CredentialIndex:uint16}
```

### Attributes (Key attributes)
| ID | Name | Type | Constraint | Quality | Default | Access | Conf |
|----|------|------|------------|---------|---------|--------|------|
|0x0000|LockState|DlLockState|desc|XP|null|R V|M|
|0x0001|LockType|DlLockType|desc|F||R V|M|
|0x0002|ActuatorEnabled|bool|all|||R V|M|
|0x0003|DoorState|DoorStateEnum|desc|XP|null|R V|DPS|
|0x0004|DoorOpenEvents|uint32|all|N||RW VM|[DPS]|
|0x0005|DoorClosedEvents|uint32|all|N||RW VM|[DPS]|
|0x0006|OpenPeriod|uint16|all|N||RW VM|[DPS]|
|0x0011|NumberOfTotalUsersSupported|uint16|all|F|0|R V|USR|
|0x0012|NumberOfPINUsersSupported|uint16|all|F|0|R V|PIN|
|0x0013|NumberOfRFIDUsersSupported|uint16|all|F|0|R V|RID|
|0x0014|NumberOfWeekDaySchedulesSupportedPerUser|uint8|all|F|0|R V|WDSCH|
|0x0015|NumberOfYearDaySchedulesSupportedPerUser|uint8|all|F|0|R V|YDSCH|
|0x0016|NumberOfHolidaySchedulesSupported|uint8|all|F|0|R V|HDSCH|
|0x0017|MaxPINCodeLength|uint8|all|F||R V|PIN|
|0x0018|MinPINCodeLength|uint8|all|F||R V|PIN|
|0x0019|MaxRFIDCodeLength|uint8|all|F||R V|RID|
|0x001A|MinRFIDCodeLength|uint8|all|F||R V|RID|
|0x001B|CredentialRulesSupport|CredentialRulesBitmap|all|F|1|R V|USR|
|0x001C|NumberOfCredentialsSupportedPerUser|uint8|all|F|0|R V|USR|
|0x0021|Language|string|max3|N||RW VM|O|
|0x0023|AutoRelockTime|uint32|all|N||RW VM|O|
|0x0024|SoundVolume|uint8|all|N||RW VM|O|
|0x0025|OperatingMode|OperatingModeEnum|desc|N||RW VM|M|
|0x0026|SupportedOperatingModes|OperatingModesBitmap|all|F|0xFFF6|R V|M|
|0x0027|DefaultConfigurationRegister|ConfigurationRegisterBitmap|all|P|0|R V|O|
|0x0028|EnableLocalProgramming|bool|all|N|true|RW VA|O|
|0x0029|EnableOneTouchLocking|bool|all|N|false|RW VM|O|
|0x002A|EnableInsideStatusLED|bool|all|N|false|RW VM|O|
|0x002B|EnablePrivacyModeButton|bool|all|N|false|RW VM|O|
|0x002C|LocalProgrammingFeatures|LocalProgrammingFeaturesBitmap|all|N|0|RW VA|O|
|0x0030|WrongCodeEntryLimit|uint8|1-255|N||RW VA|PIN\|RID|
|0x0031|UserCodeTemporaryDisableTime|uint8|1-255|N||RW VA|PIN\|RID|
|0x0032|SendPINOverTheAir|bool|all|N|false|RW VA|COTA&PIN|
|0x0033|RequirePINforRemoteOperation|bool|all|N|false|RW VA|COTA&PIN|
|0x0035|ExpiringUserTimeout|uint16|1-2880|N||RW VA|[USR]|

### Commands
| ID | Name | Dir | Response | Access | Conf |
|----|------|-----|----------|--------|------|
|0x00|LockDoor|C→S|Y|T|M|
|0x01|UnlockDoor|C→S|Y|T|M|
|0x03|UnlockWithTimeout|C→S|Y|T|O|
|0x0B|SetWeekDaySchedule|C→S|Y|A|WDSCH|
|0x0C|GetWeekDaySchedule|C→S|GetWeekDayScheduleResponse|A|WDSCH|
|0x0D|ClearWeekDaySchedule|C→S|Y|A|WDSCH|
|0x0E|SetYearDaySchedule|C→S|Y|A|YDSCH|
|0x0F|GetYearDaySchedule|C→S|GetYearDayScheduleResponse|A|YDSCH|
|0x10|ClearYearDaySchedule|C→S|Y|A|YDSCH|
|0x11|SetHolidaySchedule|C→S|Y|A|HDSCH|
|0x12|GetHolidaySchedule|C→S|GetHolidayScheduleResponse|A|HDSCH|
|0x13|ClearHolidaySchedule|C→S|Y|A|HDSCH|
|0x1A|SetUser|C→S|Y|A|USR|
|0x1B|GetUser|C→S|GetUserResponse|A|USR|
|0x1D|ClearUser|C→S|Y|A|USR|
|0x22|SetCredential|C→S|SetCredentialResponse|A T|USR|
|0x24|GetCredentialStatus|C→S|GetCredentialStatusResponse|A|USR|
|0x26|ClearCredential|C→S|Y|A T|USR|
|0x27|UnboltDoor|C→S|Y|T|UBOLT|

LockDoor/UnlockDoor/UnboltDoor: {PINCode:octstr(O)}

### Events
| ID | Name | Priority | Access | Conf |
|----|------|----------|--------|------|
|0x00|DoorLockAlarm|CRIT|V|M|
|0x01|DoorStateChange|CRIT|V|DPS|
|0x02|LockOperation|CRIT|V|M|
|0x03|LockOperationError|CRIT|V|M|
|0x04|LockUserChange|INFO|A|USR|

---
## 5.3 Window Covering Cluster (0x0102)
Rev:5 | Role:Application | Scope:Endpoint | PICS:WNCV

### Features
| Bit | Code | Feature | Conf | Summary |
|-----|------|---------|------|---------|
|0|LF|Lift|O.a+|Lift control|
|1|TL|Tilt|O.a+|Tilt control|
|2|PA_LF|PositionAwareLift|[LF]|Position aware lift|
|3|ABS|AbsolutePosition|[PA_LF\|PA_TL]|Absolute positioning|
|4|PA_TL|PositionAwareTilt|[TL]|Position aware tilt|

### Data Types
```
TypeEnum(enum8): Rollershade=0,Rollershade2Motor=1,RollershadeExterior=2,RollershadeExterior2Motor=3,Drapery=4,Awning=5,Shutter=6,TiltBlindTiltOnly=7,TiltBlindLiftAndTilt=8,ProjectorScreen=9,Unknown=255
EndProductTypeEnum(enum8): RollerShade=0,RomanShade=1,BalloonShade=2,WovenWood=3,PleatedShade=4,CellularShade=5,LayeredShade=6,LayeredShade2D=7,SheerShade=8,TiltOnlyInteriorBlind=9,InteriorBlind=10,VerticalBlindStripCurtain=11,InteriorVenetianBlind=12,ExteriorVenetianBlind=13,LateralLeftCurtain=14,LateralRightCurtain=15,CentralCurtain=16,RollerShutter=17,ExteriorVerticalScreen=18,AwningTerracePatio=19,AwningVerticalScreen=20,TiltOnlyPergola=21,SwingingShutter=22,SlidingShutter=23,Unknown=255
ConfigStatusBitmap(map8): Operational=Bit0,OnlineReserved=Bit1,LiftMovementReversed=Bit2,LiftPositionAware=Bit3,TiltPositionAware=Bit4,LiftEncoderControlled=Bit5,TiltEncoderControlled=Bit6
ModeBitmap(map8): MotorDirectionReversed=Bit0,CalibrationMode=Bit1,MaintenanceMode=Bit2,LEDFeedback=Bit3
OperationalStatusBitmap(map8): Global=Bits0-1,Lift=Bits2-3,Tilt=Bits4-5
SafetyStatusBitmap(map16): RemoteLockout=Bit0,TamperDetection=Bit1,FailedCommunication=Bit2,PositionFailure=Bit3,ThermalProtection=Bit4,ObstacleDetected=Bit5,Power=Bit6,StopInput=Bit7,MotorJammed=Bit8,HardwareFailure=Bit9,ManualOperation=Bit10,Protection=Bit11
```

### Attributes
| ID | Name | Type | Constraint | Quality | Default | Access | Conf |
|----|------|------|------------|---------|---------|--------|------|
|0x0000|Type|TypeEnum|desc|F|0|R V|M|
|0x0001|PhysicalClosedLimitLift|uint16|all|F|0|R V|[LF&PA_LF]|
|0x0002|PhysicalClosedLimitTilt|uint16|all|F|0|R V|[TL&PA_TL]|
|0x0003|CurrentPositionLift|uint16|all|XP|null|R V|[LF&PA_LF]|
|0x0004|CurrentPositionTilt|uint16|all|XP|null|R V|[TL&PA_TL]|
|0x0005|NumberOfActuationsLift|uint16|all||0|R V|[LF]|
|0x0006|NumberOfActuationsTilt|uint16|all||0|R V|[TL]|
|0x0007|ConfigStatus|ConfigStatusBitmap|all|P|0|R V|M|
|0x0008|CurrentPositionLiftPercentage|percent|all|XPQ|null|R V|LF&PA_LF|
|0x0009|CurrentPositionTiltPercentage|percent|all|XPQ|null|R V|TL&PA_TL|
|0x000A|OperationalStatus|OperationalStatusBitmap|all|P|0|R V|M|
|0x000B|TargetPositionLiftPercent100ths|percent100ths|all|XPQ|null|R V|LF&PA_LF|
|0x000C|TargetPositionTiltPercent100ths|percent100ths|all|XPQ|null|R V|TL&PA_TL|
|0x000D|EndProductType|EndProductTypeEnum|desc|F|0|R V|M|
|0x000E|CurrentPositionLiftPercent100ths|percent100ths|all|XPQ|null|R V|LF&PA_LF|
|0x000F|CurrentPositionTiltPercent100ths|percent100ths|all|XPQ|null|R V|TL&PA_TL|
|0x0010|InstalledOpenLimitLift|uint16|all|F|0|R V|LF&PA_LF|
|0x0011|InstalledClosedLimitLift|uint16|all|F|65534|R V|LF&PA_LF|
|0x0012|InstalledOpenLimitTilt|uint16|all|F|0|R V|TL&PA_TL|
|0x0013|InstalledClosedLimitTilt|uint16|all|F|65534|R V|TL&PA_TL|
|0x0017|Mode|ModeBitmap|all|N|0|RW VM|M|
|0x001A|SafetyStatus|SafetyStatusBitmap|all|P|0|R V|O|

### Commands
| ID | Name | Dir | Response | Access | Conf |
|----|------|-----|----------|--------|------|
|0x00|UpOrOpen|C→S|Y|O|M|
|0x01|DownOrClose|C→S|Y|O|M|
|0x02|StopMotion|C→S|Y|O|M|
|0x04|GoToLiftValue|C→S|Y|O|LF&ABS|
|0x05|GoToLiftPercentage|C→S|Y|O|LF&PA_LF|
|0x07|GoToTiltValue|C→S|Y|O|TL&ABS|
|0x08|GoToTiltPercentage|C→S|Y|O|TL&PA_TL|

GoToLiftValue: {LiftValue:uint16}
GoToLiftPercentage: {LiftPercent100thsValue:percent100ths}
GoToTiltValue: {TiltValue:uint16}
GoToTiltPercentage: {TiltPercent100thsValue:percent100ths}

---
# CHAPTER 6: MEDIA

## 6.2 Account Login Cluster (0x050E)
Rev:2 | Role:Application | Scope:Endpoint | PICS:ALOGIN

### Commands
| ID | Name | Dir | Response | Access | Conf |
|----|------|-----|----------|--------|------|
|0x00|GetSetupPIN|C→S|GetSetupPINResponse|A T|M|
|0x02|Login|C→S|Y|A T|M|
|0x03|Logout|C→S|Y|O T|M|

GetSetupPIN: {TempAccountIdentifier:string(max100)}
Login: {TempAccountIdentifier:string(max100), SetupPIN:string(max32), Node:node-id(O)}
GetSetupPINResponse: {SetupPIN:string(max32)}

### Events
| ID | Name | Priority | Access | Conf |
|----|------|----------|--------|------|
|0x00|LoggedOut|CRIT|V|O|

---
## 6.3 Application Basic Cluster (0x050D)
Rev:1 | Role:Application | Scope:Endpoint | PICS:APBSC

### Data Types
```
ApplicationStatusEnum(enum8): Stopped=0,ActiveVisibleFocus=1,ActiveHidden=2,ActiveVisibleNotFocus=3
ApplicationStruct: {CatalogVendorID:uint16, ApplicationID:string}
```

### Attributes
| ID | Name | Type | Constraint | Quality | Default | Access | Conf |
|----|------|------|------------|---------|---------|--------|------|
|0x0000|VendorName|string|max32|F||R V|O|
|0x0001|VendorID|vendor-id|all|F||R V|O|
|0x0002|ApplicationName|string|max256|F||R V|M|
|0x0003|ProductID|uint16|all|F||R V|O|
|0x0004|Application|ApplicationStruct|all|F||R V|M|
|0x0005|Status|ApplicationStatusEnum|desc|||R V|M|
|0x0006|ApplicationVersion|string|max32|F||R V|M|
|0x0007|AllowedVendorList|list[vendor-id]|all|F||R A|M|

---
## 6.4 Application Launcher Cluster (0x050C)
Rev:1 | Role:Application | Scope:Endpoint | PICS:APPLAUNCHER

### Features
| Bit | Code | Feature | Conf | Summary |
|-----|------|---------|------|---------|
|0|AP|ApplicationPlatform|O|Application platform|

### Data Types
```
StatusEnum(enum8): Success=0,AppNotAvailable=1,SystemBusy=2,PendingUserApproval=3,Downloading=4,Installing=5
ApplicationStruct: {CatalogVendorID:uint16, ApplicationID:string}
ApplicationEPStruct: {Application:ApplicationStruct, Endpoint:endpoint-no(O)}
```

### Attributes
| ID | Name | Type | Constraint | Quality | Default | Access | Conf |
|----|------|------|------------|---------|---------|--------|------|
|0x0000|CatalogList|list[uint16]|all|||R V|AP|
|0x0001|CurrentApp|ApplicationEPStruct|all|X|null|R V|O|

### Commands
| ID | Name | Dir | Response | Access | Conf |
|----|------|-----|----------|--------|------|
|0x00|LaunchApp|C→S|LauncherResponse|O|M|
|0x01|StopApp|C→S|LauncherResponse|O|M|
|0x02|HideApp|C→S|LauncherResponse|O|M|

LaunchApp: {Application:ApplicationStruct(O), Data:octstr(O)}
StopApp: {Application:ApplicationStruct(O)}
HideApp: {Application:ApplicationStruct(O)}
LauncherResponse: {Status:StatusEnum, Data:octstr(O)}

---
## 6.5 Audio Output Cluster (0x050B)
Rev:1 | Role:Application | Scope:Endpoint | PICS:AUDIOOUTPUT

### Features
| Bit | Code | Feature | Conf | Summary |
|-----|------|---------|------|---------|
|0|NU|NameUpdates|O|Name updates|

### Data Types
```
OutputTypeEnum(enum8): HDMI=0,BT=1,Optical=2,Headphone=3,Internal=4,Other=5
OutputInfoStruct: {Index:uint8, OutputType:OutputTypeEnum, Name:string(max32)}
```

### Attributes
| ID | Name | Type | Constraint | Quality | Default | Access | Conf |
|----|------|------|------------|---------|---------|--------|------|
|0x0000|OutputList|list[OutputInfoStruct]|all|||R V|M|
|0x0001|CurrentOutput|uint8|all||0|R V|M|

### Commands
| ID | Name | Dir | Response | Access | Conf |
|----|------|-----|----------|--------|------|
|0x00|SelectOutput|C→S|Y|O|M|
|0x01|RenameOutput|C→S|Y|M|NU|

SelectOutput: {Index:uint8}
RenameOutput: {Index:uint8, Name:string(max32)}

---
## 6.6 Channel Cluster (0x0504)
Rev:2 | Role:Application | Scope:Endpoint | PICS:CHANNEL

### Features
| Bit | Code | Feature | Conf | Summary |
|-----|------|---------|------|---------|
|0|CL|ChannelList|O|Channel list|
|1|LI|LineupInfo|O|Lineup info|
|2|EG|ElectronicGuide|O|Electronic guide|
|3|RP|RecordProgram|O|Record program|

### Data Types
```
StatusEnum(enum8): Success=0,MultipleMatches=1,NoMatches=2
LineupInfoTypeEnum(enum8): MSO=0
ChannelTypeEnum(enum8): Satellite=0,Cable=1,Terrestrial=2,OTT=3
ChannelInfoStruct: {MajorNumber:uint16, MinorNumber:uint16, Name:string(max32,O), CallSign:string(max32,O), AffiliateCallSign:string(max32,O), Identifier:string(max32,O), Type:ChannelTypeEnum(O)}
LineupInfoStruct: {OperatorName:string, LineupName:string(O), PostalCode:string(O), LineupInfoType:LineupInfoTypeEnum}
ProgramStruct: {Identifier:string, Channel:ChannelInfoStruct, StartTime:epoch-s, EndTime:epoch-s, Title:string, Subtitle:string(O), Description:string(O), ...}
```

### Attributes
| ID | Name | Type | Constraint | Quality | Default | Access | Conf |
|----|------|------|------------|---------|---------|--------|------|
|0x0000|ChannelList|list[ChannelInfoStruct]|all|||R V|CL|
|0x0001|Lineup|LineupInfoStruct|all|X|null|R V|LI|
|0x0002|CurrentChannel|ChannelInfoStruct|all|X|null|R V|O|

### Commands
| ID | Name | Dir | Response | Access | Conf |
|----|------|-----|----------|--------|------|
|0x00|ChangeChannel|C→S|ChangeChannelResponse|O|CL\|LI|
|0x02|ChangeChannelByNumber|C→S|Y|O|M|
|0x03|SkipChannel|C→S|Y|O|M|
|0x04|GetProgramGuide|C→S|ProgramGuideResponse|O|EG|
|0x06|RecordProgram|C→S|Y|O|RP|
|0x07|CancelRecordProgram|C→S|Y|O|RP|

ChangeChannel: {Match:string(max255)}
ChangeChannelByNumber: {MajorNumber:uint16, MinorNumber:uint16}
SkipChannel: {Count:int16}

---
## 6.7 Content Launcher Cluster (0x050A)
Rev:2 | Role:Application | Scope:Endpoint | PICS:CONTENTLAUNCHER

### Features
| Bit | Code | Feature | Conf | Summary |
|-----|------|---------|------|---------|
|0|CS|ContentSearch|O|Content search|
|1|UP|URLPlayback|O|URL playback|
|2|AS|AdvancedSeek|O|Advanced seek|
|3|TT|TextTracks|O|Text tracks|
|4|AT|AudioTracks|O|Audio tracks|

### Data Types
```
StatusEnum(enum8): Success=0,URLNotAvailable=1,AuthFailed=2,TextTrackNotAvailable=3,AudioTrackNotAvailable=4
MetricTypeEnum(enum8): Pixels=0,Percentage=1
ParameterEnum(enum8): Actor=0,Channel=1,Character=2,Director=3,Event=4,Franchise=5,Genre=6,League=7,Popularity=8,Provider=9,Sport=10,SportsTeam=11,Type=12,Video=13,Season=14,Episode=15,Any=16
SupportedProtocolsBitmap(map32): DASH=Bit0,HLS=Bit1
ContentSearchStruct: {ParameterList:list[ParameterStruct]}
ParameterStruct: {Type:ParameterEnum, Value:string, ExternalIDList:list[AdditionalInfoStruct](O)}
```

### Attributes
| ID | Name | Type | Constraint | Quality | Default | Access | Conf |
|----|------|------|------------|---------|---------|--------|------|
|0x0000|AcceptHeader|list[string]|max100[max1024]|||R V|UP|
|0x0001|SupportedStreamingProtocols|SupportedProtocolsBitmap|all||0|R V|UP|

### Commands
| ID | Name | Dir | Response | Access | Conf |
|----|------|-----|----------|--------|------|
|0x00|LaunchContent|C→S|LauncherResponse|O|CS|
|0x01|LaunchURL|C→S|LauncherResponse|O|UP|

LaunchContent: {Search:ContentSearchStruct, AutoPlay:bool, Data:string(O), PlaybackPreferences:PlaybackPreferencesStruct(O), UseCurrentContext:bool(O)}
LaunchURL: {ContentURL:string(max8192), DisplayString:string(O), BrandingInformation:BrandingInformationStruct(O)}
LauncherResponse: {Status:StatusEnum, Data:string(O)}

---
## 6.8 Keypad Input Cluster (0x0509)
Rev:1 | Role:Application | Scope:Endpoint | PICS:KEYPADINPUT

### Features
| Bit | Code | Feature | Conf | Summary |
|-----|------|---------|------|---------|
|0|NV|NavigationKeyCodes|O|Navigation keys|
|1|LK|LocationKeys|O|Location keys|
|2|NK|NumberKeys|O|Number keys|

### Data Types
```
StatusEnum(enum8): Success=0,UnsupportedKey=1,InvalidKeyInCurrentState=2
CECKeyCodeEnum(enum8): Select=0x00,Up=0x01,Down=0x02,Left=0x03,Right=0x04,RightUp=0x05,RightDown=0x06,LeftUp=0x07,LeftDown=0x08,RootMenu=0x09,SetupMenu=0x0A,ContentsMenu=0x0B,FavoriteMenu=0x0C,Exit=0x0D,MediaTopMenu=0x10,MediaContextSensitiveMenu=0x11,NumberEntryMode=0x1D,Number11=0x1E,Number12=0x1F,Number0OrNumber10=0x20,Numbers1-9=0x21-0x29,Dot=0x2A,Enter=0x2B,Clear=0x2C,NextFavorite=0x2F,ChannelUp=0x30,ChannelDown=0x31,PreviousChannel=0x32,SoundSelect=0x33,InputSelect=0x34,DisplayInformation=0x35,Help=0x36,PageUp=0x37,PageDown=0x38,Power=0x40,VolumeUp=0x41,VolumeDown=0x42,Mute=0x43,Play=0x44,Stop=0x45,Pause=0x46,Record=0x47,Rewind=0x48,FastForward=0x49,Eject=0x4A,Forward=0x4B,Backward=0x4C,StopRecord=0x4D,PauseRecord=0x4E,Reserved=0x4F,Angle=0x50,SubPicture=0x51,VideoOnDemand=0x52,ElectronicProgramGuide=0x53,TimerProgramming=0x54,InitialConfiguration=0x55,SelectBroadcastType=0x56,SelectSoundPresentation=0x57,PlayFunction=0x60,PausePlayFunction=0x61,RecordFunction=0x62,PauseRecordFunction=0x63,StopFunction=0x64,MuteFunction=0x65,RestoreVolumeFunction=0x66,TuneFunction=0x67,SelectMediaFunction=0x68,SelectAvInputFunction=0x69,SelectAudioInputFunction=0x6A,PowerToggleFunction=0x6B,PowerOffFunction=0x6C,PowerOnFunction=0x6D,F1Blue=0x71,F2Red=0x72,F3Green=0x73,F4Yellow=0x74,F5=0x75,Data=0x76
```

### Commands
| ID | Name | Dir | Response | Access | Conf |
|----|------|-----|----------|--------|------|
|0x00|SendKey|C→S|SendKeyResponse|O|M|

SendKey: {KeyCode:CECKeyCodeEnum}
SendKeyResponse: {Status:StatusEnum}

---
## 6.9 Media Input Cluster (0x0507)
Rev:1 | Role:Application | Scope:Endpoint | PICS:MEDIAINPUT

### Features
| Bit | Code | Feature | Conf | Summary |
|-----|------|---------|------|---------|
|0|NU|NameUpdates|O|Name updates|

### Data Types
```
InputTypeEnum(enum8): Internal=0,Aux=1,Coax=2,Composite=3,HDMI=4,Input=5,Line=6,Optical=7,Video=8,SCART=9,USB=10,Other=11
InputInfoStruct: {Index:uint8, InputType:InputTypeEnum, Name:string(max32), Description:string(max32)}
```

### Attributes
| ID | Name | Type | Constraint | Quality | Default | Access | Conf |
|----|------|------|------------|---------|---------|--------|------|
|0x0000|InputList|list[InputInfoStruct]|all|||R V|M|
|0x0001|CurrentInput|uint8|all||0|R V|M|

### Commands
| ID | Name | Dir | Response | Access | Conf |
|----|------|-----|----------|--------|------|
|0x00|SelectInput|C→S|Y|O|M|
|0x01|ShowInputStatus|C→S|Y|O|M|
|0x02|HideInputStatus|C→S|Y|O|M|
|0x03|RenameInput|C→S|Y|M|NU|

SelectInput: {Index:uint8}
RenameInput: {Index:uint8, Name:string(max32)}

---
## 6.10 Media Playback Cluster (0x0506)
Rev:2 | Role:Application | Scope:Endpoint | PICS:MEDIAPLAYBACK

### Features
| Bit | Code | Feature | Conf | Summary |
|-----|------|---------|------|---------|
|0|AS|AdvancedSeek|O|Advanced seek|
|1|VS|VariableSpeed|O|Variable speed|
|2|TT|TextTracks|O|Text tracks|
|3|AT|AudioTracks|O|Audio tracks|
|4|AA|AudioAdvance|O|Audio advance|

### Data Types
```
PlaybackStateEnum(enum8): Playing=0,Paused=1,NotPlaying=2,Buffering=3
StatusEnum(enum8): Success=0,InvalidStateForCommand=1,NotAllowed=2,NotActive=3,SpeedOutOfRange=4,SeekOutOfRange=5
CharacteristicEnum(enum8): ForcedSubtitles=0,DescribesVideo=1,EasyToRead=2,FrameBased=3,MainProgram=4,OriginalContent=5,VoiceOverTranslation=6,Caption=7,Subtitle=8,Alternate=9,Supplementary=10,Commentary=11,DubbedTranslation=12
PlaybackPositionStruct: {UpdatedAt:epoch-us, Position:uint64|X}
TrackStruct: {ID:string(max32), TrackAttributes:TrackAttributesStruct|X}
TrackAttributesStruct: {LanguageCode:string(max32), Characteristics:list[CharacteristicEnum](O), DisplayName:string(max256,O)}
```

### Attributes
| ID | Name | Type | Constraint | Quality | Default | Access | Conf |
|----|------|------|------------|---------|---------|--------|------|
|0x0000|CurrentState|PlaybackStateEnum|desc|||R V|M|
|0x0001|StartTime|epoch-us|all|X|null|R V|AS|
|0x0002|Duration|uint64|all|X|null|R V|AS|
|0x0003|SampledPosition|PlaybackPositionStruct|all|X|null|R V|AS|
|0x0004|PlaybackSpeed|single|all||1|R V|AS|
|0x0005|SeekRangeEnd|uint64|all|X|null|R V|AS|
|0x0006|SeekRangeStart|uint64|all|X|null|R V|AS|
|0x0007|ActiveAudioTrack|TrackStruct|all|X|null|R V|AT|
|0x0008|AvailableAudioTracks|list[TrackStruct]|all|X|null|R V|AT|
|0x0009|ActiveTextTrack|TrackStruct|all|X|null|R V|TT|
|0x000A|AvailableTextTracks|list[TrackStruct]|all|X|null|R V|TT|

### Commands
| ID | Name | Dir | Response | Access | Conf |
|----|------|-----|----------|--------|------|
|0x00|Play|C→S|PlaybackResponse|O|M|
|0x01|Pause|C→S|PlaybackResponse|O|M|
|0x02|Stop|C→S|PlaybackResponse|O|M|
|0x03|StartOver|C→S|PlaybackResponse|O|O|
|0x04|Previous|C→S|PlaybackResponse|O|O|
|0x05|Next|C→S|PlaybackResponse|O|O|
|0x06|Rewind|C→S|PlaybackResponse|O|VS|
|0x07|FastForward|C→S|PlaybackResponse|O|VS|
|0x08|SkipForward|C→S|PlaybackResponse|O|AS|
|0x09|SkipBackward|C→S|PlaybackResponse|O|AS|
|0x0B|Seek|C→S|PlaybackResponse|O|AS|
|0x0C|ActivateAudioTrack|C→S|Y|O|AT|
|0x0D|ActivateTextTrack|C→S|Y|O|TT|
|0x0E|DeactivateTextTrack|C→S|Y|O|TT|

Rewind/FastForward: {AudioAdvanceUnmuted:bool(AA)}
SkipForward/SkipBackward: {DeltaPositionMilliseconds:uint64}
Seek: {Position:uint64}
ActivateAudioTrack: {TrackID:string(max32), AudioOutputIndex:uint8|X(AT)}
ActivateTextTrack: {TrackID:string(max32)}
PlaybackResponse: {Status:StatusEnum, Data:string(O)}

### Events
| ID | Name | Priority | Access | Conf |
|----|------|----------|--------|------|
|0x00|StateChanged|INFO|V|O|

---
## 6.11 Target Navigator Cluster (0x0505)
Rev:2 | Role:Application | Scope:Endpoint | PICS:TGTNAV

### Data Types
```
StatusEnum(enum8): Success=0,TargetNotFound=1,NotAllowed=2
TargetInfoStruct: {Identifier:uint8(max254), Name:string}
```

### Attributes
| ID | Name | Type | Constraint | Quality | Default | Access | Conf |
|----|------|------|------------|---------|---------|--------|------|
|0x0000|TargetList|list[TargetInfoStruct]|all|||R V|M|
|0x0001|CurrentTarget|uint8|desc||0xFF|R V|O|

### Commands
| ID | Name | Dir | Response | Access | Conf |
|----|------|-----|----------|--------|------|
|0x00|NavigateTarget|C→S|NavigateTargetResponse|O|M|

NavigateTarget: {Target:uint8, Data:string(O)}
NavigateTargetResponse: {Status:StatusEnum, Data:string(O)}

### Events
| ID | Name | Priority | Access | Conf |
|----|------|----------|--------|------|
|0x00|TargetUpdated|INFO|V|O|

---
## 6.12 Content App Observer Cluster (0x0510)
Rev:1 | Role:Application | Scope:Endpoint | PICS:APPOBSERVER

### Data Types
```
StatusEnum(enum8): Success=0,UnexpectedData=1
```

### Commands
| ID | Name | Dir | Response | Access | Conf |
|----|------|-----|----------|--------|------|
|0x00|ContentAppMessage|C→S|ContentAppMessageResponse|O|M|

ContentAppMessage: {Data:string(max500), EncodingHint:string(max100,O)}
ContentAppMessageResponse: {Status:StatusEnum, Data:string(max500,O), EncodingHint:string(max100,O)}

---
## 6.13 Content Control Cluster (0x050F) [PROVISIONAL]
Rev:1 | Role:Application | Scope:Endpoint | PICS:CONCON

### Features
| Bit | Code | Feature | Conf | Summary |
|-----|------|---------|------|---------|
|0|ST|ScreenTime|O|Screen time limits|
|1|PM|PINManagement|O|PIN management|
|2|BU|BlockUnrated|O|Block unrated content|
|3|OCR|OnDemandContentRating|O|On-demand rating|
|4|SCR|ScheduledContentRating|O|Scheduled rating|
|5|BC|BlockChannels|O|Block channels|
|6|BA|BlockApplications|O|Block applications|
|7|BTW|BlockContentTimeWindow|O|Block time windows|

### Data Types
```
DayOfWeekBitmap(map8): Sunday-Saturday=Bit0-6
RatingNameStruct: {RatingName:string(max8), RatingNameDesc:string(max64,O)}
BlockChannelStruct: {BlockChannelIndex:uint16|X, MajorNumber:uint16, MinorNumber:uint16, Identifier:string(O)}
AppInfoStruct: {CatalogVendorID:uint16, ApplicationID:string}
TimeWindowStruct: {TimeWindowIndex:uint16|X, DayOfWeek:DayOfWeekBitmap, TimePeriod:list[TimePeriodStruct]}
TimePeriodStruct: {StartHour:uint8(0-23), StartMinute:uint8(0-59), EndHour:uint8(0-23), EndMinute:uint8(0-59)}
```

### Attributes
| ID | Name | Type | Constraint | Quality | Default | Access | Conf |
|----|------|------|------------|---------|---------|--------|------|
|0x0000|Enabled|bool|all|||R V|M|
|0x0001|OnDemandRatings|list[RatingNameStruct]|all|||R V|OCR|
|0x0002|OnDemandRatingThreshold|string|max8|||R V|OCR|
|0x0003|ScheduledContentRatings|list[RatingNameStruct]|all|||R V|SCR|
|0x0004|ScheduledContentRatingThreshold|string|max8|||R V|SCR|
|0x0005|ScreenDailyTime|elapsed-s|max86400|||R V|ST|
|0x0006|RemainingScreenTime|elapsed-s|max86400|||R V|ST|
|0x0007|BlockUnrated|bool|all|||R V|BU|
|0x0008|BlockChannelList|list[BlockChannelStruct]|all|||R V|BC|
|0x0009|BlockApplicationList|list[AppInfoStruct]|all|||R V|BA|
|0x000A|BlockContentTimeWindow|list[TimeWindowStruct]|max7|||R V|BTW|

### Commands
| ID | Name | Dir | Response | Access | Conf |
|----|------|-----|----------|--------|------|
|0x00|UpdatePIN|C→S|Y|M|PM|
|0x01|ResetPIN|C→S|ResetPINResponse|A|PM|
|0x02|Enable|C→S|Y|M|M|
|0x03|Disable|C→S|Y|M|M|
|0x04|AddBonusTime|C→S|Y|M|ST|
|0x05|SetScreenDailyTime|C→S|Y|M|ST|
|0x06|BlockUnratedContent|C→S|Y|M|BU|
|0x07|UnblockUnratedContent|C→S|Y|M|BU|
|0x08|SetOnDemandRatingThreshold|C→S|Y|M|OCR|
|0x09|SetScheduledContentRatingThreshold|C→S|Y|M|SCR|
|0x0A|AddBlockChannels|C→S|Y|M|BC|
|0x0B|RemoveBlockChannels|C→S|Y|M|BC|
|0x0C|AddBlockApplications|C→S|Y|M|BA|
|0x0D|RemoveBlockApplications|C→S|Y|M|BA|
|0x0E|SetBlockContentTimeWindow|C→S|Y|M|BTW|
|0x0F|RemoveBlockContentTimeWindow|C→S|Y|M|BTW|

### Events
| ID | Name | Priority | Access | Conf |
|----|------|----------|--------|------|
|0x00|RemainingScreenTimeExpired|INFO|V|ST|
|0x01|EnteringBlockContentTimeWindow|INFO|V|BTW|

---
# CHAPTER 7: ROBOTS

## 7.2 RVC Run Mode Cluster (0x0054)
Rev:3 | Role:Application | Scope:Endpoint | PICS:RVCRUNM
Derived from: Mode Base

### Features
| Bit | Code | Feature | Conf | Summary |
|-----|------|---------|------|---------|
|0|DEPONOFF|OnOff|X|Disallowed|

### Mode Tags (Derived)
| Value | Name | Summary |
|-------|------|---------|
|0x4000|Idle|Not performing main operations|
|0x4001|Cleaning|Cleaning mode|
|0x4002|Mapping|Mapping mode|

### ChangeToModeResponse Status Codes
| Value | Name |
|-------|------|
|0x41|Stuck|
|0x42|DustBinMissing|
|0x43|DustBinFull|
|0x44|WaterTankEmpty|
|0x45|WaterTankMissing|
|0x46|WaterTankLidOpen|
|0x47|MopCleaningPadMissing|
|0x48|BatteryLow|

SupportedModes: Must include at least one Idle mode and one Cleaning mode. Idle/Cleaning/Mapping are mutually exclusive.

---
## 7.3 RVC Clean Mode Cluster (0x0055)
Rev:3 | Role:Application | Scope:Endpoint | PICS:RVCCLEANM
Derived from: Mode Base

### Features
| Bit | Code | Feature | Conf | Summary |
|-----|------|---------|------|---------|
|0|DEPONOFF|OnOff|X|Disallowed|

### Mode Tags (Derived)
| Value | Name | Summary |
|-------|------|---------|
|0x4000|DeepClean|Improved cleaning|
|0x4001|Vacuum|Vacuuming enabled|
|0x4002|Mop|Mopping enabled|

### ChangeToModeResponse Status Codes
| Value | Name |
|-------|------|
|0x40|CleaningInProgress|

SupportedModes: Must include at least one mode with Vacuum and/or Mop tag.

---
## 7.4 RVC Operational State Cluster (0x0061)
Rev:2 | Role:Application | Scope:Endpoint | PICS:RVCOPSTATE
Derived from: Operational State

### OperationalStateEnum (Derived)
| Value | Name | Pause-Compatible | Resume-Compatible |
|-------|------|------------------|-------------------|
|0x40|SeekingCharger|Y|N|
|0x41|Charging|N|Y|
|0x42|Docked|N|Y|

### ErrorStateEnum (Derived)
| Value | Name |
|-------|------|
|0x40|FailedToFindChargingDock|
|0x41|Stuck|
|0x42|DustBinMissing|
|0x43|DustBinFull|
|0x44|WaterTankEmpty|
|0x45|WaterTankMissing|
|0x46|WaterTankLidOpen|
|0x47|MopCleaningPadMissing|

### Commands
| ID | Name | Dir | Response | Access | Conf |
|----|------|-----|----------|--------|------|
|0x00|Pause|C→S|OperationalCommandResponse|O|M|
|0x01|Stop|C→S|OperationalCommandResponse|O|X|
|0x02|Start|C→S|OperationalCommandResponse|O|X|
|0x03|Resume|C→S|OperationalCommandResponse|O|M|
|0x80|GoHome|C→S|OperationalCommandResponse|O|O|

---
# CHAPTER 8: HOME APPLIANCES

## 8.2 Temperature Control Cluster (0x0056)
Rev:1 | Role:Application | Scope:Endpoint | PICS:TCTL

### Features
| Bit | Code | Feature | Conf | Summary |
|-----|------|---------|------|---------|
|0|TN|TemperatureNumber|O.a+|Actual temperature|
|1|TL|TemperatureLevel|O.a+|Temperature levels|
|2|STEP|TemperatureStep|[TN]|Step control|

### Attributes
| ID | Name | Type | Constraint | Quality | Default | Access | Conf |
|----|------|------|------------|---------|---------|--------|------|
|0x0000|TemperatureSetpoint|temperature|MinTemperature-MaxTemperature|||R V|TN|
|0x0001|MinTemperature|temperature|max(MaxTemperature-1)|F||R V|TN|
|0x0002|MaxTemperature|temperature|desc|F||R V|TN|
|0x0003|Step|temperature|max(MaxTemperature-MinTemperature)|F||R V|STEP|
|0x0004|SelectedTemperatureLevel|uint8|max31|||R V|TL|
|0x0005|SupportedTemperatureLevels|list[string]|max32[max16]|||R V|TL|

### Commands
| ID | Name | Dir | Response | Access | Conf |
|----|------|-----|----------|--------|------|
|0x00|SetTemperature|C→S|Y|O|M|

SetTemperature: {TargetTemperature:temperature(TN,O), TargetTemperatureLevel:uint8(TL,O)}

---
## 8.3 Dishwasher Mode Cluster (0x0059)
Rev:2 | Role:Application | Scope:Endpoint | PICS:DISHM
Derived from: Mode Base

### Features
| Bit | Code | Feature | Conf | Summary |
|-----|------|---------|------|---------|
|0|DEPONOFF|OnOff|X|Disallowed|

### Mode Tags (Derived)
| Value | Name |
|-------|------|
|0x4000|Normal|
|0x4001|Heavy|
|0x4002|Light|

SupportedModes: Must include at least one mode with Normal tag.

---
## 8.4 Dishwasher Alarm Cluster (0x005D)
Rev:1 | Role:Application | Scope:Endpoint | PICS:DISHALM
Derived from: Alarm Base

### Features
| Bit | Code | Feature | Conf | Summary |
|-----|------|---------|------|---------|
|0|RESET|Reset|X|Disallowed|

### AlarmBitmap (Derived)
| Bit | Name |
|-----|------|
|0|InflowError|
|1|DrainError|
|2|DoorError|
|3|TempTooLow|
|4|TempTooHigh|
|5|WaterLevelError|

---
## 8.5 Laundry Washer Mode Cluster (0x0051)
Rev:3 | Role:Application | Scope:Endpoint | PICS:LWM
Derived from: Mode Base

### Features
| Bit | Code | Feature | Conf | Summary |
|-----|------|---------|------|---------|
|0|DEPONOFF|OnOff|X|Disallowed|

### Mode Tags (Derived)
| Value | Name |
|-------|------|
|0x4000|Normal|
|0x4001|Delicate|
|0x4002|Heavy|
|0x4003|Whites|

SupportedModes: Must include at least one mode with Normal tag.

---
## 8.6 Laundry Washer Controls Cluster (0x0053)
Rev:2 | Role:Application | Scope:Endpoint | PICS:WASHERCTRL

### Features
| Bit | Code | Feature | Conf | Summary |
|-----|------|---------|------|---------|
|0|SPIN|Spin|O.a+|Multiple spin speeds|
|1|RINSE|Rinse|O.a+|Multiple rinse cycles|

### Data Types
```
NumberOfRinsesEnum(enum8): None=0,Normal=1,Extra=2,Max=3
```

### Attributes
| ID | Name | Type | Constraint | Quality | Default | Access | Conf |
|----|------|------|------------|---------|---------|--------|------|
|0x0000|SpinSpeeds|list[string]|max16[max64]|||R V|SPIN|
|0x0001|SpinSpeedCurrent|uint8|max15|X|desc|RW VO|SPIN|
|0x0002|NumberOfRinses|NumberOfRinsesEnum|desc||1|RW VO|RINSE|
|0x0003|SupportedRinses|list[NumberOfRinsesEnum]|max4|||R V|RINSE|

---
## 8.7 Refrigerator And Temperature Controlled Cabinet Mode Cluster (0x0052)
Rev:3 | Role:Application | Scope:Endpoint | PICS:TCCM
Derived from: Mode Base

### Features
| Bit | Code | Feature | Conf | Summary |
|-----|------|---------|------|---------|
|0|DEPONOFF|OnOff|X|Disallowed|

### Mode Tags (Derived)
| Value | Name |
|-------|------|
|0x4000|RapidCool|
|0x4001|RapidFreeze|

SupportedModes: Must include at least one mode with Auto tag.

---
## 8.8 Refrigerator Alarm Cluster (0x0057)
Rev:1 | Role:Application | Scope:Endpoint | PICS:REFALM
Derived from: Alarm Base

### Features
| Bit | Code | Feature | Conf | Summary |
|-----|------|---------|------|---------|
|0|RESET|Reset|X|Disallowed|

### AlarmBitmap (Derived)
| Bit | Name |
|-----|------|
|0|DoorOpen|

---
## 8.9 Laundry Dryer Controls Cluster (0x004A)
Rev:1 | Role:Application | Scope:Endpoint | PICS:DRYERCTRL

### Data Types
```
DrynessLevelEnum(enum8): Low=0,Normal=1,Extra=2,Max=3
```

### Attributes
| ID | Name | Type | Constraint | Quality | Default | Access | Conf |
|----|------|------|------------|---------|---------|--------|------|
|0x0000|SupportedDrynessLevels|list[DrynessLevelEnum]|1-4|||R V|M|
|0x0001|SelectedDrynessLevel|DrynessLevelEnum|desc|X|desc|RW VO|M|

---
## 8.10 Oven Cavity Operational State Cluster (0x0048)
Rev:2 | Role:Application | Scope:Endpoint | PICS:OVENOPSTATE
Derived from: Operational State

PhaseList: Only "pre-heating", "pre-heated", "cooling down" allowed.

### Commands
| ID | Name | Dir | Response | Access | Conf |
|----|------|-----|----------|--------|------|
|0x00|Pause|C→S|OperationalCommandResponse|O|X|
|0x01|Stop|C→S|OperationalCommandResponse|O|M|
|0x02|Start|C→S|OperationalCommandResponse|O|M|
|0x03|Resume|C→S|OperationalCommandResponse|O|X|

---
## 8.11 Oven Mode Cluster (0x0049)
Rev:2 | Role:Application | Scope:Endpoint | PICS:OTCCM
Derived from: Mode Base

### Features
| Bit | Code | Feature | Conf | Summary |
|-----|------|---------|------|---------|
|0|DEPONOFF|OnOff|X|Disallowed|

### Mode Tags (Derived)
| Value | Name |
|-------|------|
|0x4000|Bake|
|0x4001|Convection|
|0x4002|Grill|
|0x4003|Roast|
|0x4004|Clean|
|0x4005|ConvectionBake|
|0x4006|ConvectionRoast|
|0x4007|Warming|
|0x4008|Proofing|
|0x4009|Steam|

SupportedModes: Must include at least one mode with Bake tag.

---
## 8.12 Microwave Oven Mode Cluster (0x005E)
Rev:2 | Role:Application | Scope:Endpoint | PICS:MWOM
Derived from: Mode Base

### Features
| Bit | Code | Feature | Conf | Summary |
|-----|------|---------|------|---------|
|0|DEPONOFF|OnOff|X|Disallowed|

### Mode Tags (Derived)
| Value | Name |
|-------|------|
|0x4000|Normal|
|0x4001|Defrost|

SupportedModes: Exactly one mode with Normal tag. Normal and Defrost are mutually exclusive.

### Commands (Disallowed)
ChangeToMode and ChangeToModeResponse are disallowed.

---
## 8.13 Microwave Oven Control Cluster (0x005F)
Rev:1 | Role:Application | Scope:Endpoint | PICS:MWOCTRL

### Features
| Bit | Code | Feature | Conf | Summary |
|-----|------|---------|------|---------|
|0|PWRNUM|PowerAsNumber|O.a+|Power as number/percent|
|1|WATTS|PowerInWatts|P,O.a+|Power in Watts|
|2|PWRLMTS|PowerNumberLimits|[PWRNUM]|Power limit attributes|

### Attributes
| ID | Name | Type | Constraint | Quality | Default | Access | Conf |
|----|------|------|------------|---------|---------|--------|------|
|0x0000|CookTime|elapsed-s|1-MaxCookTime||30|R V|M|
|0x0001|MaxCookTime|elapsed-s|1-86400|F|MS|R V|M|
|0x0002|PowerSetting|uint8|desc||desc|R V|PWRNUM|
|0x0003|MinPower|uint8|1-99|F|10|R V|PWRLMTS|
|0x0004|MaxPower|uint8|(MinPower+1)-100|F|100|R V|PWRLMTS|
|0x0005|PowerStep|uint8|desc|F|10|R V|PWRLMTS|
|0x0006|SupportedWatts|list[uint16]|1-10|F|MS|R V|WATTS|
|0x0007|SelectedWattIndex|uint8|desc||MS|R V|WATTS|
|0x0008|WattRating|uint16|all|F|MS|R V|O|

### Commands
| ID | Name | Dir | Response | Access | Conf |
|----|------|-----|----------|--------|------|
|0x00|SetCookingParameters|C→S|Y|O|M|
|0x01|AddMoreTime|C→S|Y|O|O|

SetCookingParameters: {CookMode:uint8(O), CookTime:elapsed-s(O), PowerSetting:uint8([PWRNUM],O), WattSettingIndex:uint8([WATTS],O), StartAfterSetting:bool(O)}
AddMoreTime: {TimeToAdd:elapsed-s}

---
# CHAPTER 9: ENERGY MANAGEMENT

## 9.2 Device Energy Management Cluster (0x0098)
Rev:4 | Role:Application | Scope:Endpoint | PICS:DEM

### Features
| Bit | Code | Feature | Conf | Summary |
|-----|------|---------|------|---------|
|0|PA|PowerAdjustment|O|Power adjustment|
|1|PFR|PowerForecastReporting|O|Power forecast|
|2|SFR|StateForecastReporting|O|State forecast|
|3|STA|StartTimeAdjustment|[PFR\|SFR]|Start time adjust|
|4|PAU|Pausable|[PFR\|SFR]|Pausable operation|
|5|FA|ForecastAdjustment|[PFR]|Forecast adjustment|
|6|CON|ConstraintBasedAdjustment|[PFR\|SFR]|Constraint-based|

### Data Types
```
ESATypeEnum(enum8): EVSE=0,SpaceHeating=1,WaterHeating=2,SpaceCooling=3,SpaceHeatingCooling=4,BatteryStorage=5,SolarPV=6,FridgeFreezer=7,WashingMachine=8,Dishwasher=9,Cooking=10,HomeWaterPump=11,IrrigationWaterPump=12,PoolPump=13,Other=255
ESAStateEnum(enum8): Offline=0,Online=1,Fault=2,PowerAdjustActive=3,Paused=4
CauseEnum(enum8): NormalCompletion=0,Offline=1,Fault=2,UserOptOut=3,Cancelled=4
AdjustmentCauseEnum(enum8): LocalOptimization=0,GridOptimization=1
OptOutStateEnum(enum8): NoOptOut=0,LocalOptOut=1,GridOptOut=2,OptOut=3
ForecastUpdateReasonEnum(enum8): InternalOptimization=0,LocalOptimization=1,GridOptimization=2
PowerAdjustReasonEnum(enum8): NoAdjustment=0,LocalOptimizationAdjustment=1,GridOptimizationAdjustment=2
CostTypeEnum(enum8): Financial=0,GHGEmissions=1,Comfort=2,Temperature=3
ForecastStruct: {ForecastID:uint32, ActiveSlotNumber:uint16|X, StartTime:epoch-s, EndTime:epoch-s, EarliestStartTime:epoch-s|X(O), LatestEndTime:epoch-s|X(O), IsPausable:bool, Slots:list[SlotStruct], ForecastUpdateReason:ForecastUpdateReasonEnum}
SlotStruct: {MinDuration:elapsed-s, MaxDuration:elapsed-s, DefaultDuration:elapsed-s, ElapsedSlotTime:elapsed-s, RemainingSlotTime:elapsed-s, SlotIsPausable:bool(O), MinPauseDuration:elapsed-s(O), MaxPauseDuration:elapsed-s(O), ManufacturerESAState:uint16(O), NominalPower:power-mW(O), MinPower:power-mW(O), MaxPower:power-mW(O), NominalEnergy:energy-mWh(O), Costs:list[CostStruct](O), MinPowerAdjustment:power-mW(O), MaxPowerAdjustment:power-mW(O), MinDurationAdjustment:elapsed-s(O), MaxDurationAdjustment:elapsed-s(O)}
PowerAdjustStruct: {MinPower:power-mW, MaxPower:power-mW, MinDuration:elapsed-s, MaxDuration:elapsed-s}
PowerAdjustCapabilityStruct: {PowerAdjustCapability:list[PowerAdjustStruct]|X, Cause:PowerAdjustReasonEnum}
```

### Attributes
| ID | Name | Type | Constraint | Quality | Default | Access | Conf |
|----|------|------|------------|---------|---------|--------|------|
|0x0000|ESAType|ESATypeEnum|desc|F||R V|M|
|0x0001|ESACanGenerate|bool|all|F||R V|M|
|0x0002|ESAState|ESAStateEnum|desc|||R V|M|
|0x0003|AbsMinPower|power-mW|all|F||R V|M|
|0x0004|AbsMaxPower|power-mW|all|F||R V|M|
|0x0005|PowerAdjustmentCapability|PowerAdjustCapabilityStruct|all|X|null|R V|PA|
|0x0006|Forecast|ForecastStruct|all|X|null|R V|PFR\|SFR|
|0x0007|OptOutState|OptOutStateEnum|desc||0|R V|M|

### Commands
| ID | Name | Dir | Response | Access | Conf |
|----|------|-----|----------|--------|------|
|0x00|PowerAdjustRequest|C→S|Y|O|PA|
|0x01|CancelPowerAdjustRequest|C→S|Y|O|PA|
|0x02|StartTimeAdjustRequest|C→S|Y|O|STA|
|0x03|PauseRequest|C→S|Y|O|PAU|
|0x04|ResumeRequest|C→S|Y|O|PAU|
|0x05|ModifyForecastRequest|C→S|Y|O|FA|
|0x06|RequestConstraintBasedForecast|C→S|Y|O|CON|
|0x07|CancelRequest|C→S|Y|O|STA\|FA\|CON|

PowerAdjustRequest: {Power:power-mW, Duration:elapsed-s, Cause:AdjustmentCauseEnum}
StartTimeAdjustRequest: {RequestedStartTime:epoch-s, Cause:AdjustmentCauseEnum}
PauseRequest: {Duration:elapsed-s, Cause:AdjustmentCauseEnum}
ModifyForecastRequest: {ForecastID:uint32, SlotAdjustments:list[SlotAdjustmentStruct](max10), Cause:AdjustmentCauseEnum}
RequestConstraintBasedForecast: {Constraints:list[ConstraintsStruct](max10), Cause:AdjustmentCauseEnum}

### Events
| ID | Name | Priority | Access | Conf |
|----|------|----------|--------|------|
|0x00|PowerAdjustStart|INFO|V|PA|
|0x01|PowerAdjustEnd|INFO|V|PA|
|0x02|Paused|INFO|V|PAU|
|0x03|Resumed|INFO|V|PAU|

---
## 9.3 Energy EVSE Cluster (0x0099)
Rev:3 | Role:Application | Scope:Endpoint | PICS:EEVSE

### Features
| Bit | Code | Feature | Conf | Summary |
|-----|------|---------|------|---------|
|0|PREF|ChargingPreferences|O|Charging preferences|
|1|SOC|SoCReporting|O|State of charge|
|2|PNC|PlugAndCharge|O|Plug and charge|
|3|RFID|RFID|O|RFID support|
|4|V2X|V2X|O|Vehicle to grid|

### Data Types
```
StateEnum(enum8): NotPluggedIn=0,PluggedInNoDemand=1,PluggedInDemand=2,PluggedInCharging=3,PluggedInDischarging=4,SessionEnding=5,Fault=6
SupplyStateEnum(enum8): Disabled=0,ChargingEnabled=1,DischargingEnabled=2,DisabledError=3,DisabledDiagnostics=4,Enabled=5
FaultStateEnum(enum8): NoError=0,MeterFailure=1,OverVoltage=2,UnderVoltage=3,OverCurrent=4,ContactWetFailure=5,ContactDryFailure=6,GroundFault=7,PowerLoss=8,PowerQuality=9,PilotShortCircuit=10,EmergencyStop=11,EVDisconnected=12,WrongPowerSupply=13,LiveNeutralSwap=14,OverTemperature=15,Other=255
EnergyTransferStoppedReasonEnum(enum8): EVStopped=0,EVSEStopped=1,Other=2
TargetDayOfWeekBitmap(map8): Sunday-Saturday=Bit0-6
ChargingTargetStruct: {TargetTimeMinutesPastMidnight:uint16(max1439), TargetSoC:percent(O), AddedEnergy:energy-mWh(O)}
ChargingTargetScheduleStruct: {DayOfWeekForSequence:TargetDayOfWeekBitmap, ChargingTargets:list[ChargingTargetStruct](max10)}
```

### Attributes
| ID | Name | Type | Constraint | Quality | Default | Access | Conf |
|----|------|------|------------|---------|---------|--------|------|
|0x0000|State|StateEnum|desc|X|null|R V|M|
|0x0001|SupplyState|SupplyStateEnum|desc|||R V|M|
|0x0002|FaultState|FaultStateEnum|desc|||R V|M|
|0x0003|ChargingEnabledUntil|epoch-s|all|XN|null|R V|M|
|0x0004|DischargingEnabledUntil|epoch-s|all|XN|null|R V|V2X|
|0x0005|CircuitCapacity|amperage-mA|all||0|R V|M|
|0x0006|MinimumChargeCurrent|amperage-mA|all||6000|R V|M|
|0x0007|MaximumChargeCurrent|amperage-mA|all||0|R V|M|
|0x0008|MaximumDischargeCurrent|amperage-mA|all||0|R V|V2X|
|0x0009|UserMaximumChargeCurrent|amperage-mA|all|N|0|RW VM|O|
|0x000A|RandomizationDelayWindow|elapsed-s|max86400|N|600|RW VM|O|
|0x0023|NextChargeStartTime|epoch-s|all|X|null|R V|PREF|
|0x0024|NextChargeTargetTime|epoch-s|all|X|null|R V|PREF|
|0x0025|NextChargeRequiredEnergy|energy-mWh|all|X|null|R V|PREF|
|0x0026|NextChargeTargetSoC|percent|all|X|null|R V|PREF&SOC|
|0x0030|ApproximateEVEfficiency|uint16|all|XN|null|RW VM|PREF|
|0x0031|StateOfCharge|percent|all|X|null|R V|SOC|
|0x0032|BatteryCapacity|energy-mWh|all|X|null|R V|SOC|
|0x0033|VehicleID|string|max32|X|null|R V|PNC|
|0x0040|SessionID|uint32|all|XN|null|R V|M|
|0x0041|SessionDuration|elapsed-s|all|N|0|R V|M|
|0x0042|SessionEnergyCharged|energy-mWh|all|N|0|R V|M|
|0x0043|SessionEnergyDischarged|energy-mWh|all|N|0|R V|V2X|

### Commands
| ID | Name | Dir | Response | Access | Conf |
|----|------|-----|----------|--------|------|
|0x01|Disable|C→S|Y|O|M|
|0x02|EnableCharging|C→S|Y|O|M|
|0x03|EnableDischarging|C→S|Y|O|V2X|
|0x04|StartDiagnostics|C→S|Y|O|O|
|0x05|SetTargets|C→S|Y|O|PREF|
|0x06|GetTargets|C→S|GetTargetsResponse|O|PREF|
|0x07|ClearTargets|C→S|Y|O|PREF|

EnableCharging: {ChargingEnabledUntil:epoch-s|X, MinimumChargeCurrent:amperage-mA, MaximumChargeCurrent:amperage-mA}
EnableDischarging: {DischargingEnabledUntil:epoch-s|X, MaximumDischargeCurrent:amperage-mA}
SetTargets: {ChargingTargetSchedules:list[ChargingTargetScheduleStruct](max7)}

### Events
| ID | Name | Priority | Access | Conf |
|----|------|----------|--------|------|
|0x00|EVConnected|INFO|V|M|
|0x01|EVNotDetected|INFO|V|M|
|0x02|EnergyTransferStarted|INFO|V|M|
|0x03|EnergyTransferStopped|INFO|V|M|
|0x04|Fault|CRIT|V|M|
|0x05|RFID|INFO|V|RFID|

---
## 9.5 Water Heater Management Cluster (0x0094)
Rev:1 | Role:Application | Scope:Endpoint | PICS:WHM

### Features
| Bit | Code | Feature | Conf | Summary |
|-----|------|---------|------|---------|
|0|EM|EnergyManagement|O|Energy management|
|1|TP|TankPercent|O|Tank percentage|

### Data Types
```
WaterHeaterHeatSourceBitmap(map8): ImmersionElement1=Bit0,ImmersionElement2=Bit1,HeatPump=Bit2,Boiler=Bit3,Other=Bit4
BoostStateEnum(enum8): Inactive=0,Active=1
WaterHeaterBoostInfoStruct: {Duration:elapsed-s, OneShot:bool(O), EmergencyBoost:bool(O), TemporarySetpoint:temperature(O), TargetPercentage:percent(O), TargetReheat:percent(O)}
```

### Attributes
| ID | Name | Type | Constraint | Quality | Default | Access | Conf |
|----|------|------|------------|---------|---------|--------|------|
|0x0000|HeaterTypes|WaterHeaterHeatSourceBitmap|all|F|0|R V|M|
|0x0001|HeatDemand|WaterHeaterHeatSourceBitmap|all||0|R V|M|
|0x0002|TankVolume|uint16|all||0|R V|EM|
|0x0003|EstimatedHeatRequired|energy-mWh|min0||0|R V|EM|
|0x0004|TankPercentage|percent|all||0|R V|TP|
|0x0005|BoostState|BoostStateEnum|all||Inactive|R V|M|

### Commands
| ID | Name | Dir | Response | Access | Conf |
|----|------|-----|----------|--------|------|
|0x00|Boost|C→S|Y|M|M|
|0x01|CancelBoost|C→S|Y|M|M|

Boost: {BoostInfo:WaterHeaterBoostInfoStruct}

### Events
| ID | Name | Priority | Access | Conf |
|----|------|----------|--------|------|
|0x00|BoostStarted|INFO|V|M|
|0x01|BoostEnded|INFO|V|M|

---
## 9.6 Water Heater Mode Cluster (0x009E)
Rev:1 | Role:Application | Scope:Endpoint | PICS:WHM
Derived from: Mode Base

### Features
| Bit | Code | Feature | Conf | Summary |
|-----|------|---------|------|---------|
|0|DEPONOFF|OnOff|X|Disallowed|

### Mode Tags (Derived)
| Value | Name |
|-------|------|
|0x4000|Off|
|0x4001|Manual|
|0x4002|Timed|

SupportedModes: Must include at least one mode with Manual tag and one with Off tag. Off/Manual/Timed are mutually exclusive.

---
## 9.7 Energy Preference Cluster (0x009B) [PROVISIONAL]
Rev:1 | Role:Application | Scope:Endpoint | PICS:EPREF

### Features
| Bit | Code | Feature | Conf | Summary |
|-----|------|---------|------|---------|
|0|BALA|EnergyBalance|O.a+|Energy balance|
|1|LPMS|LowPowerModeSensitivity|O.a+|Low power mode|

### Data Types
```
EnergyPriorityEnum(enum8): Comfort=0,Speed=1,Efficiency=2,WaterConsumption=3
BalanceStruct: {Step:percent(F), Label:string(max64,F,O)}
```

### Attributes
| ID | Name | Type | Constraint | Quality | Default | Access | Conf |
|----|------|------|------------|---------|---------|--------|------|
|0x0000|EnergyBalances|list[BalanceStruct]|2-10|F||R V|BALA|
|0x0001|CurrentEnergyBalance|uint8|all|N||RW VO|BALA|
|0x0002|EnergyPriorities|list[EnergyPriorityEnum]|2|F||R V|BALA|
|0x0003|LowPowerModeSensitivities|list[BalanceStruct]|2-10|F||R V|LPMS|
|0x0004|CurrentLowPowerModeSensitivity|uint8|all|N||RW VO|LPMS|

---
## 9.8 Device Energy Management Mode Cluster (0x009F)
Rev:2 | Role:Application | Scope:Endpoint | PICS:DEMM
Derived from: Mode Base

### Features
| Bit | Code | Feature | Conf | Summary |
|-----|------|---------|------|---------|
|0|DEPONOFF|OnOff|X|Disallowed|

### Mode Tags (Derived)
| Value | Name |
|-------|------|
|0x4000|NoOptimization|
|0x4001|DeviceOptimization|
|0x4002|LocalOptimization|
|0x4003|GridOptimization|

SupportedModes: Must include NoOptimization, LocalOptimization, and GridOptimization modes. NoOptimization cannot be combined with other optimization tags.

---
# CHAPTER 10: NETWORK INFRASTRUCTURE

## 10.2 Wi-Fi Network Management Cluster (0x0451)
Rev:1 | Role:Application | Scope:Endpoint | PICS:WIFINM

### Attributes
| ID | Name | Type | Constraint | Quality | Default | Access | Conf |
|----|------|------|------------|---------|---------|--------|------|
|0x0000|SSID|octstr|1-32|XN|null|R V|M|
|0x0001|PassphraseSurrogate|uint64|all|XN|null|R M|M|

### Commands
| ID | Name | Dir | Response | Access | Conf |
|----|------|-----|----------|--------|------|
|0x00|NetworkPassphraseRequest|C→S|NetworkPassphraseResponse|M|M|

NetworkPassphraseResponse: {Passphrase:octstr(max64)}

Note: Passphrase format: 8-63 bytes (WPA passphrase) or 64 bytes (hex PSK).

---
## 10.3 Thread Border Router Management Cluster (0x0452)
Rev:1 | Role:Application | Scope:Endpoint | PICS:TBRM

### Features
| Bit | Code | Feature | Conf | Summary |
|-----|------|---------|------|---------|
|0|PC|PANChange|O|PAN change capability|

### Attributes
| ID | Name | Type | Constraint | Quality | Default | Access | Conf |
|----|------|------|------------|---------|---------|--------|------|
|0x0000|BorderRouterName|string|1-63|||R V|M|
|0x0001|BorderAgentID|octstr|16|||R V|M|
|0x0002|ThreadVersion|uint16|all|F|MS|R V|M|
|0x0003|InterfaceEnabled|bool|all|N|false|R V|M|
|0x0004|ActiveDatasetTimestamp|uint64|all|XN|0|R V|M|
|0x0005|PendingDatasetTimestamp|uint64|all|XN|0|R V|M|

### Commands
| ID | Name | Dir | Response | Access | Conf |
|----|------|-----|----------|--------|------|
|0x00|GetActiveDatasetRequest|C→S|DatasetResponse|O|M|
|0x01|GetPendingDatasetRequest|C→S|DatasetResponse|O|M|
|0x03|SetActiveDatasetRequest|C→S|Y|M|M|
|0x04|SetPendingDatasetRequest|C→S|Y|M|PC|

SetActiveDatasetRequest: {ActiveDataset:octstr(max254), Breadcrumb:uint64(O)}
SetPendingDatasetRequest: {PendingDataset:octstr(max254)}
DatasetResponse: {Dataset:octstr(max254)}

---
## 10.4 Thread Network Directory Cluster (0x0453)
Rev:1 | Role:Application | Scope:Endpoint | PICS:THNETDIR

### Data Types
```
ThreadNetworkStruct: {ExtendedPanID:octstr(8), NetworkName:string(1-16), Channel:uint16, ActiveTimestamp:uint64}
```

### Attributes
| ID | Name | Type | Constraint | Quality | Default | Access | Conf |
|----|------|------|------------|---------|---------|--------|------|
|0x0000|PreferredExtendedPanID|octstr|8|XN|null|RW VM|M|
|0x0001|ThreadNetworks|list[ThreadNetworkStruct]|max20|||R V|M|
|0x0002|ThreadNetworkTableSize|uint8|all|F||R V|M|

### Commands
| ID | Name | Dir | Response | Access | Conf |
|----|------|-----|----------|--------|------|
|0x00|AddNetwork|C→S|Y|M|M|
|0x01|RemoveNetwork|C→S|Y|M|M|
|0x02|GetOperationalDataset|C→S|OperationalDatasetResponse|O|M|

AddNetwork: {OperationalDataset:octstr(max254)}
RemoveNetwork: {ExtendedPanID:octstr(8)}
GetOperationalDataset: {ExtendedPanID:octstr(8)}
OperationalDatasetResponse: {OperationalDataset:octstr(max254)}

---
# END OF DOCUMENT

## Cluster ID Quick Reference
| ID | Name | Chapter |
|----|------|---------|
|0x0003|Identify|1|
|0x0004|Groups|1|
|0x0005|On/Off|1|
|0x0006|On/Off|1|
|0x0008|Level Control|1|
|0x003B|Switch|1|
|0x0045|Boolean State|1|
|0x0048|Oven Cavity Operational State|8|
|0x0049|Oven Mode|8|
|0x004A|Laundry Dryer Controls|8|
|0x0050|Mode Select|1|
|0x0051|Laundry Washer Mode|8|
|0x0052|Refrigerator And TCC Mode|8|
|0x0053|Laundry Washer Controls|8|
|0x0054|RVC Run Mode|7|
|0x0055|RVC Clean Mode|7|
|0x0056|Temperature Control|8|
|0x0057|Refrigerator Alarm|8|
|0x0059|Dishwasher Mode|8|
|0x005B|Air Quality|2|
|0x005C|Smoke CO Alarm|2|
|0x005D|Dishwasher Alarm|8|
|0x005E|Microwave Oven Mode|8|
|0x005F|Microwave Oven Control|8|
|0x0060|Operational State|1|
|0x0061|RVC Operational State|7|
|0x0062|Scenes Management|1|
|0x0071|HEPA Filter Monitoring|2|
|0x0072|Activated Carbon Filter Monitoring|2|
|0x0080|Boolean State Configuration|1|
|0x0081|Valve Configuration and Control|4|
|0x0090|Electrical Power Measurement|2|
|0x0091|Electrical Energy Measurement|2|
|0x0094|Water Heater Management|9|
|0x0097|Messages|1|
|0x0098|Device Energy Management|9|
|0x0099|Energy EVSE|9|
|0x009B|Energy Preference|9|
|0x009E|Water Heater Mode|9|
|0x009F|Device Energy Management Mode|9|
|0x0101|Door Lock|5|
|0x0102|Window Covering|5|
|0x0150|Service Area|1|
|0x0200|Pump Configuration and Control|4|
|0x0201|Thermostat|4|
|0x0202|Fan Control|4|
|0x0204|Thermostat User Interface Configuration|4|
|0x0300|Color Control|3|
|0x0301|Ballast Configuration|3|
|0x0400|Illuminance Measurement|2|
|0x0402|Temperature Measurement|2|
|0x0403|Pressure Measurement|2|
|0x0404|Flow Measurement|2|
|0x0405|Relative Humidity Measurement|2|
|0x0406|Occupancy Sensing|2|
|0x040C|Carbon Monoxide Concentration|2|
|0x040D|Carbon Dioxide Concentration|2|
|0x0413|Nitrogen Dioxide Concentration|2|
|0x0415|Ozone Concentration|2|
|0x042A|PM2.5 Concentration|2|
|0x042B|Formaldehyde Concentration|2|
|0x042C|PM1 Concentration|2|
|0x042D|PM10 Concentration|2|
|0x042E|Total VOC Concentration|2|
|0x042F|Radon Concentration|2|
|0x0451|Wi-Fi Network Management|10|
|0x0452|Thread Border Router Management|10|
|0x0453|Thread Network Directory|10|
|0x0503|Wake On LAN|1|
|0x0504|Channel|6|
|0x0505|Target Navigator|6|
|0x0506|Media Playback|6|
|0x0507|Media Input|6|
|0x0508|Low Power|1|
|0x0509|Keypad Input|6|
|0x050A|Content Launcher|6|
|0x050B|Audio Output|6|
|0x050C|Application Launcher|6|
|0x050D|Application Basic|6|
|0x050E|Account Login|6|
|0x050F|Content Control|6|
|0x0510|Content App Observer|6|
