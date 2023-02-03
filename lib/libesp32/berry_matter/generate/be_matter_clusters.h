/*********************************************************************************\
* Compact form for attributes and clusters
*
* Generated content, do not edit
\*********************************************************************************/
#include <stddef.h>
#include <stdint.h>

typedef struct {
  uint16_t id;
  uint8_t  type;
  uint8_t  flags;
  const char* name;
} matter_attribute_t;

typedef struct {
  uint16_t id;
  const char* name;
} matter_command_t;

typedef struct {
  uint16_t id;
  const char* name;
  const matter_attribute_t* attributes;
  const matter_command_t* commands;
} matter_cluster_t;

const matter_attribute_t matter_Attributes_0003[] = {
  { 0x0000, 0, 0x00, "IdentifyTime" },
  { 0x0001, 0, 0x00, "IdentifyType" },
  { 0xFFF8, 0, 0x00, "GeneratedCommandList" },
  { 0xFFF9, 0, 0x00, "AcceptedCommandList" },
  { 0xFFFB, 0, 0x00, "AttributeList" },
  { 0xFFFC, 0, 0x00, "FeatureMap" },
  { 0xFFFD, 0, 0x00, "ClusterRevision" },
  { 0xFFFF, 0, 0x00, NULL },
};

const matter_command_t matter_Commands_0003[] = {
  { 0x0000, "Identify" },
  { 0x0040, "TriggerEffect" },
  { 0xFFFF, NULL },
};

const matter_attribute_t matter_Attributes_0004[] = {
  { 0x0000, 0, 0x00, "NameSupport" },
  { 0xFFF8, 0, 0x00, "GeneratedCommandList" },
  { 0xFFF9, 0, 0x00, "AcceptedCommandList" },
  { 0xFFFB, 0, 0x00, "AttributeList" },
  { 0xFFFC, 0, 0x00, "FeatureMap" },
  { 0xFFFD, 0, 0x00, "ClusterRevision" },
  { 0xFFFF, 0, 0x00, NULL },
};

const matter_command_t matter_Commands_0004[] = {
  { 0x0000, "AddGroup" },
  { 0x0001, "ViewGroup" },
  { 0x0002, "GetGroupMembership" },
  { 0x0003, "RemoveGroup" },
  { 0x0004, "RemoveAllGroups" },
  { 0x0005, "AddGroupIfIdentifying" },
  { 0xFFFF, NULL },
};

const matter_attribute_t matter_Attributes_0005[] = {
  { 0x0000, 0, 0x00, "SceneCount" },
  { 0x0001, 0, 0x00, "CurrentScene" },
  { 0x0002, 0, 0x00, "CurrentGroup" },
  { 0x0003, 0, 0x00, "SceneValid" },
  { 0x0004, 0, 0x00, "NameSupport" },
  { 0xFFF8, 0, 0x00, "GeneratedCommandList" },
  { 0xFFF9, 0, 0x00, "AcceptedCommandList" },
  { 0xFFFB, 0, 0x00, "AttributeList" },
  { 0xFFFC, 0, 0x00, "FeatureMap" },
  { 0xFFFD, 0, 0x00, "ClusterRevision" },
  { 0xFFFF, 0, 0x00, NULL },
};

const matter_command_t matter_Commands_0005[] = {
  { 0x0000, "AddScene" },
  { 0x0001, "ViewScene" },
  { 0x0002, "RemoveScene" },
  { 0x0003, "RemoveAllScenes" },
  { 0x0004, "StoreScene" },
  { 0x0005, "RecallScene" },
  { 0x0006, "GetSceneMembership" },
  { 0xFFFF, NULL },
};

const matter_attribute_t matter_Attributes_0006[] = {
  { 0x0000, 0, 0x00, "OnOff" },
  { 0x4000, 0, 0x00, "GlobalSceneControl" },
  { 0x4001, 0, 0x00, "OnTime" },
  { 0x4002, 0, 0x00, "OffWaitTime" },
  { 0x4003, 0, 0x00, "StartUpOnOff" },
  { 0xFFF8, 0, 0x00, "GeneratedCommandList" },
  { 0xFFF9, 0, 0x00, "AcceptedCommandList" },
  { 0xFFFB, 0, 0x00, "AttributeList" },
  { 0xFFFC, 0, 0x00, "FeatureMap" },
  { 0xFFFD, 0, 0x00, "ClusterRevision" },
  { 0xFFFF, 0, 0x00, NULL },
};

const matter_command_t matter_Commands_0006[] = {
  { 0x0000, "Off" },
  { 0x0001, "On" },
  { 0x0002, "Toggle" },
  { 0x0040, "OffWithEffect" },
  { 0x0041, "OnWithRecallGlobalScene" },
  { 0x0042, "OnWithTimedOff" },
  { 0xFFFF, NULL },
};

const matter_attribute_t matter_Attributes_0007[] = {
  { 0x0000, 0, 0x00, "SwitchType" },
  { 0x0010, 0, 0x00, "SwitchActions" },
  { 0xFFF8, 0, 0x00, "GeneratedCommandList" },
  { 0xFFF9, 0, 0x00, "AcceptedCommandList" },
  { 0xFFFB, 0, 0x00, "AttributeList" },
  { 0xFFFC, 0, 0x00, "FeatureMap" },
  { 0xFFFD, 0, 0x00, "ClusterRevision" },
  { 0xFFFF, 0, 0x00, NULL },
};

const matter_command_t matter_Commands_0007[] = {
  { 0xFFFF, NULL },
};

const matter_attribute_t matter_Attributes_0008[] = {
  { 0x0000, 0, 0x00, "CurrentLevel" },
  { 0x0001, 0, 0x00, "RemainingTime" },
  { 0x0002, 0, 0x00, "MinLevel" },
  { 0x0003, 0, 0x00, "MaxLevel" },
  { 0x0004, 0, 0x00, "CurrentFrequency" },
  { 0x0005, 0, 0x00, "MinFrequency" },
  { 0x0006, 0, 0x00, "MaxFrequency" },
  { 0x000F, 0, 0x00, "Options" },
  { 0x0010, 0, 0x00, "OnOffTransitionTime" },
  { 0x0011, 0, 0x00, "OnLevel" },
  { 0x0012, 0, 0x00, "OnTransitionTime" },
  { 0x0013, 0, 0x00, "OffTransitionTime" },
  { 0x0014, 0, 0x00, "DefaultMoveRate" },
  { 0x4000, 0, 0x00, "StartUpCurrentLevel" },
  { 0xFFF8, 0, 0x00, "GeneratedCommandList" },
  { 0xFFF9, 0, 0x00, "AcceptedCommandList" },
  { 0xFFFB, 0, 0x00, "AttributeList" },
  { 0xFFFC, 0, 0x00, "FeatureMap" },
  { 0xFFFD, 0, 0x00, "ClusterRevision" },
  { 0xFFFF, 0, 0x00, NULL },
};

const matter_command_t matter_Commands_0008[] = {
  { 0x0000, "MoveToLevel" },
  { 0x0001, "Move" },
  { 0x0002, "Step" },
  { 0x0003, "Stop" },
  { 0x0004, "MoveToLevelWithOnOff" },
  { 0x0005, "MoveWithOnOff" },
  { 0x0006, "StepWithOnOff" },
  { 0x0007, "StopWithOnOff" },
  { 0xFFFF, NULL },
};

const matter_attribute_t matter_Attributes_000F[] = {
  { 0x0051, 0, 0x00, "OutOfService" },
  { 0x0055, 0, 0x00, "PresentValue" },
  { 0x006F, 0, 0x00, "StatusFlags" },
  { 0xFFF8, 0, 0x00, "GeneratedCommandList" },
  { 0xFFF9, 0, 0x00, "AcceptedCommandList" },
  { 0xFFFB, 0, 0x00, "AttributeList" },
  { 0xFFFC, 0, 0x00, "FeatureMap" },
  { 0xFFFD, 0, 0x00, "ClusterRevision" },
  { 0xFFFF, 0, 0x00, NULL },
};

const matter_command_t matter_Commands_000F[] = {
  { 0xFFFF, NULL },
};

const matter_attribute_t matter_Attributes_001D[] = {
  { 0x0000, 0, 0x00, "DeviceTypeList" },
  { 0x0001, 0, 0x00, "ServerList" },
  { 0x0002, 0, 0x00, "ClientList" },
  { 0x0003, 0, 0x00, "PartsList" },
  { 0xFFF8, 0, 0x00, "GeneratedCommandList" },
  { 0xFFF9, 0, 0x00, "AcceptedCommandList" },
  { 0xFFFB, 0, 0x00, "AttributeList" },
  { 0xFFFC, 0, 0x00, "FeatureMap" },
  { 0xFFFD, 0, 0x00, "ClusterRevision" },
  { 0xFFFF, 0, 0x00, NULL },
};

const matter_command_t matter_Commands_001D[] = {
  { 0xFFFF, NULL },
};

const matter_attribute_t matter_Attributes_001E[] = {
  { 0x0000, 0, 0x00, "Binding" },
  { 0xFFF8, 0, 0x00, "GeneratedCommandList" },
  { 0xFFF9, 0, 0x00, "AcceptedCommandList" },
  { 0xFFFB, 0, 0x00, "AttributeList" },
  { 0xFFFC, 0, 0x00, "FeatureMap" },
  { 0xFFFD, 0, 0x00, "ClusterRevision" },
  { 0xFFFF, 0, 0x00, NULL },
};

const matter_command_t matter_Commands_001E[] = {
  { 0xFFFF, NULL },
};

const matter_attribute_t matter_Attributes_001F[] = {
  { 0x0000, 0, 0x00, "Acl" },
  { 0x0001, 0, 0x00, "Extension" },
  { 0x0002, 0, 0x00, "SubjectsPerAccessControlEntry" },
  { 0x0003, 0, 0x00, "TargetsPerAccessControlEntry" },
  { 0x0004, 0, 0x00, "AccessControlEntriesPerFabric" },
  { 0xFFF8, 0, 0x00, "GeneratedCommandList" },
  { 0xFFF9, 0, 0x00, "AcceptedCommandList" },
  { 0xFFFB, 0, 0x00, "AttributeList" },
  { 0xFFFC, 0, 0x00, "FeatureMap" },
  { 0xFFFD, 0, 0x00, "ClusterRevision" },
  { 0xFFFF, 0, 0x00, NULL },
};

const matter_command_t matter_Commands_001F[] = {
  { 0xFFFF, NULL },
};

const matter_attribute_t matter_Attributes_0025[] = {
  { 0x0000, 0, 0x00, "ActionList" },
  { 0x0001, 0, 0x00, "EndpointLists" },
  { 0x0002, 0, 0x00, "SetupURL" },
  { 0xFFF8, 0, 0x00, "GeneratedCommandList" },
  { 0xFFF9, 0, 0x00, "AcceptedCommandList" },
  { 0xFFFB, 0, 0x00, "AttributeList" },
  { 0xFFFC, 0, 0x00, "FeatureMap" },
  { 0xFFFD, 0, 0x00, "ClusterRevision" },
  { 0xFFFF, 0, 0x00, NULL },
};

const matter_command_t matter_Commands_0025[] = {
  { 0x0000, "InstantAction" },
  { 0x0001, "InstantActionWithTransition" },
  { 0x0002, "StartAction" },
  { 0x0003, "StartActionWithDuration" },
  { 0x0004, "StopAction" },
  { 0x0005, "PauseAction" },
  { 0x0006, "PauseActionWithDuration" },
  { 0x0007, "ResumeAction" },
  { 0x0008, "EnableAction" },
  { 0x0009, "EnableActionWithDuration" },
  { 0x000A, "DisableAction" },
  { 0x000B, "DisableActionWithDuration" },
  { 0xFFFF, NULL },
};

const matter_attribute_t matter_Attributes_0028[] = {
  { 0x0000, 0, 0x00, "DataModelRevision" },
  { 0x0001, 0, 0x00, "VendorName" },
  { 0x0002, 0, 0x00, "VendorID" },
  { 0x0003, 0, 0x00, "ProductName" },
  { 0x0004, 0, 0x00, "ProductID" },
  { 0x0005, 0, 0x00, "NodeLabel" },
  { 0x0006, 0, 0x00, "Location" },
  { 0x0007, 0, 0x00, "HardwareVersion" },
  { 0x0008, 0, 0x00, "HardwareVersionString" },
  { 0x0009, 0, 0x00, "SoftwareVersion" },
  { 0x000A, 0, 0x00, "SoftwareVersionString" },
  { 0x000B, 0, 0x00, "ManufacturingDate" },
  { 0x000C, 0, 0x00, "PartNumber" },
  { 0x000D, 0, 0x00, "ProductURL" },
  { 0x000E, 0, 0x00, "ProductLabel" },
  { 0x000F, 0, 0x00, "SerialNumber" },
  { 0x0010, 0, 0x00, "LocalConfigDisabled" },
  { 0x0011, 0, 0x00, "Reachable" },
  { 0x0012, 0, 0x00, "UniqueID" },
  { 0x0013, 0, 0x00, "CapabilityMinima" },
  { 0xFFF8, 0, 0x00, "GeneratedCommandList" },
  { 0xFFF9, 0, 0x00, "AcceptedCommandList" },
  { 0xFFFB, 0, 0x00, "AttributeList" },
  { 0xFFFC, 0, 0x00, "FeatureMap" },
  { 0xFFFD, 0, 0x00, "ClusterRevision" },
  { 0xFFFF, 0, 0x00, NULL },
};

const matter_command_t matter_Commands_0028[] = {
  { 0xFFFF, NULL },
};

const matter_attribute_t matter_Attributes_0029[] = {
  { 0xFFFB, 0, 0x00, "AttributeList" },
  { 0xFFFC, 0, 0x00, "FeatureMap" },
  { 0xFFFD, 0, 0x00, "ClusterRevision" },
  { 0xFFFF, 0, 0x00, NULL },
};

const matter_command_t matter_Commands_0029[] = {
  { 0x0000, "QueryImage" },
  { 0x0002, "ApplyUpdateRequest" },
  { 0x0004, "NotifyUpdateApplied" },
  { 0xFFFF, NULL },
};

const matter_attribute_t matter_Attributes_002A[] = {
  { 0x0000, 0, 0x00, "DefaultOtaProviders" },
  { 0x0001, 0, 0x00, "UpdatePossible" },
  { 0x0002, 0, 0x00, "UpdateState" },
  { 0x0003, 0, 0x00, "UpdateStateProgress" },
  { 0xFFFB, 0, 0x00, "AttributeList" },
  { 0xFFFC, 0, 0x00, "FeatureMap" },
  { 0xFFFD, 0, 0x00, "ClusterRevision" },
  { 0xFFFF, 0, 0x00, NULL },
};

const matter_command_t matter_Commands_002A[] = {
  { 0x0000, "AnnounceOtaProvider" },
  { 0xFFFF, NULL },
};

const matter_attribute_t matter_Attributes_002B[] = {
  { 0x0000, 0, 0x00, "ActiveLocale" },
  { 0x0001, 0, 0x00, "SupportedLocales" },
  { 0xFFF8, 0, 0x00, "GeneratedCommandList" },
  { 0xFFF9, 0, 0x00, "AcceptedCommandList" },
  { 0xFFFC, 0, 0x00, "FeatureMap" },
  { 0xFFFD, 0, 0x00, "ClusterRevision" },
  { 0xFFFF, 0, 0x00, NULL },
};

const matter_command_t matter_Commands_002B[] = {
  { 0xFFFF, NULL },
};

const matter_attribute_t matter_Attributes_002C[] = {
  { 0x0000, 0, 0x00, "HourFormat" },
  { 0x0001, 0, 0x00, "ActiveCalendarType" },
  { 0x0002, 0, 0x00, "SupportedCalendarTypes" },
  { 0xFFF8, 0, 0x00, "GeneratedCommandList" },
  { 0xFFF9, 0, 0x00, "AcceptedCommandList" },
  { 0xFFFC, 0, 0x00, "FeatureMap" },
  { 0xFFFD, 0, 0x00, "ClusterRevision" },
  { 0xFFFF, 0, 0x00, NULL },
};

const matter_command_t matter_Commands_002C[] = {
  { 0xFFFF, NULL },
};

const matter_attribute_t matter_Attributes_002D[] = {
  { 0x0000, 0, 0x00, "TemperatureUnit" },
  { 0xFFFB, 0, 0x00, "AttributeList" },
  { 0xFFFC, 0, 0x00, "FeatureMap" },
  { 0xFFFD, 0, 0x00, "ClusterRevision" },
  { 0xFFFF, 0, 0x00, NULL },
};

const matter_command_t matter_Commands_002D[] = {
  { 0xFFFF, NULL },
};

const matter_attribute_t matter_Attributes_002E[] = {
  { 0x0000, 0, 0x00, "Sources" },
  { 0xFFF8, 0, 0x00, "GeneratedCommandList" },
  { 0xFFF9, 0, 0x00, "AcceptedCommandList" },
  { 0xFFFB, 0, 0x00, "AttributeList" },
  { 0xFFFC, 0, 0x00, "FeatureMap" },
  { 0xFFFD, 0, 0x00, "ClusterRevision" },
  { 0xFFFF, 0, 0x00, NULL },
};

const matter_command_t matter_Commands_002E[] = {
  { 0xFFFF, NULL },
};

const matter_attribute_t matter_Attributes_002F[] = {
  { 0x0000, 0, 0x00, "Status" },
  { 0x0001, 0, 0x00, "Order" },
  { 0x0002, 0, 0x00, "Description" },
  { 0x0003, 0, 0x00, "WiredAssessedInputVoltage" },
  { 0x0004, 0, 0x00, "WiredAssessedInputFrequency" },
  { 0x0005, 0, 0x00, "WiredCurrentType" },
  { 0x0006, 0, 0x00, "WiredAssessedCurrent" },
  { 0x0007, 0, 0x00, "WiredNominalVoltage" },
  { 0x0008, 0, 0x00, "WiredMaximumCurrent" },
  { 0x0009, 0, 0x00, "WiredPresent" },
  { 0x000A, 0, 0x00, "ActiveWiredFaults" },
  { 0x000B, 0, 0x00, "BatVoltage" },
  { 0x000C, 0, 0x00, "BatPercentRemaining" },
  { 0x000D, 0, 0x00, "BatTimeRemaining" },
  { 0x000E, 0, 0x00, "BatChargeLevel" },
  { 0x000F, 0, 0x00, "BatReplacementNeeded" },
  { 0x0010, 0, 0x00, "BatReplaceability" },
  { 0x0011, 0, 0x00, "BatPresent" },
  { 0x0012, 0, 0x00, "ActiveBatFaults" },
  { 0x0013, 0, 0x00, "BatReplacementDescription" },
  { 0x0014, 0, 0x00, "BatCommonDesignation" },
  { 0x0015, 0, 0x00, "BatANSIDesignation" },
  { 0x0016, 0, 0x00, "BatIECDesignation" },
  { 0x0017, 0, 0x00, "BatApprovedChemistry" },
  { 0x0018, 0, 0x00, "BatCapacity" },
  { 0x0019, 0, 0x00, "BatQuantity" },
  { 0x001A, 0, 0x00, "BatChargeState" },
  { 0x001B, 0, 0x00, "BatTimeToFullCharge" },
  { 0x001C, 0, 0x00, "BatFunctionalWhileCharging" },
  { 0x001D, 0, 0x00, "BatChargingCurrent" },
  { 0x001E, 0, 0x00, "ActiveBatChargeFaults" },
  { 0xFFF8, 0, 0x00, "GeneratedCommandList" },
  { 0xFFF9, 0, 0x00, "AcceptedCommandList" },
  { 0xFFFB, 0, 0x00, "AttributeList" },
  { 0xFFFC, 0, 0x00, "FeatureMap" },
  { 0xFFFD, 0, 0x00, "ClusterRevision" },
  { 0xFFFF, 0, 0x00, NULL },
};

const matter_command_t matter_Commands_002F[] = {
  { 0xFFFF, NULL },
};

const matter_attribute_t matter_Attributes_0030[] = {
  { 0x0000, 0, 0x00, "Breadcrumb" },
  { 0x0001, 0, 0x00, "BasicCommissioningInfo" },
  { 0x0002, 0, 0x00, "RegulatoryConfig" },
  { 0x0003, 0, 0x00, "LocationCapability" },
  { 0x0004, 0, 0x00, "SupportsConcurrentConnection" },
  { 0xFFF8, 0, 0x00, "GeneratedCommandList" },
  { 0xFFF9, 0, 0x00, "AcceptedCommandList" },
  { 0xFFFB, 0, 0x00, "AttributeList" },
  { 0xFFFC, 0, 0x00, "FeatureMap" },
  { 0xFFFD, 0, 0x00, "ClusterRevision" },
  { 0xFFFF, 0, 0x00, NULL },
};

const matter_command_t matter_Commands_0030[] = {
  { 0x0000, "ArmFailSafe" },
  { 0x0001, "ArmFailSafeResponse" },
  { 0x0002, "SetRegulatoryConfig" },
  { 0x0003, "SetRegulatoryConfigResponse" },
  { 0x0004, "CommissioningComplete" },
  { 0x0005, "CommissioningCompleteResponse" },
  { 0xFFFF, NULL },
};

const matter_attribute_t matter_Attributes_0031[] = {
  { 0x0000, 0, 0x00, "MaxNetworks" },
  { 0x0001, 0, 0x00, "Networks" },
  { 0x0002, 0, 0x00, "ScanMaxTimeSeconds" },
  { 0x0003, 0, 0x00, "ConnectMaxTimeSeconds" },
  { 0x0004, 0, 0x00, "InterfaceEnabled" },
  { 0x0005, 0, 0x00, "LastNetworkingStatus" },
  { 0x0006, 0, 0x00, "LastNetworkID" },
  { 0x0007, 0, 0x00, "LastConnectErrorValue" },
  { 0xFFF8, 0, 0x00, "GeneratedCommandList" },
  { 0xFFF9, 0, 0x00, "AcceptedCommandList" },
  { 0xFFFC, 0, 0x00, "FeatureMap" },
  { 0xFFFD, 0, 0x00, "ClusterRevision" },
  { 0xFFFF, 0, 0x00, NULL },
};

const matter_command_t matter_Commands_0031[] = {
  { 0x0000, "ScanNetworks" },
  { 0x0002, "AddOrUpdateWiFiNetwork" },
  { 0x0003, "AddOrUpdateThreadNetwork" },
  { 0x0004, "RemoveNetwork" },
  { 0x0006, "ConnectNetwork" },
  { 0x0008, "ReorderNetwork" },
  { 0xFFFF, NULL },
};

const matter_attribute_t matter_Attributes_0032[] = {
  { 0xFFF8, 0, 0x00, "GeneratedCommandList" },
  { 0xFFF9, 0, 0x00, "AcceptedCommandList" },
  { 0xFFFB, 0, 0x00, "AttributeList" },
  { 0xFFFC, 0, 0x00, "FeatureMap" },
  { 0xFFFD, 0, 0x00, "ClusterRevision" },
  { 0xFFFF, 0, 0x00, NULL },
};

const matter_command_t matter_Commands_0032[] = {
  { 0x0000, "RetrieveLogsRequest" },
  { 0xFFFF, NULL },
};

const matter_attribute_t matter_Attributes_0033[] = {
  { 0x0000, 0, 0x00, "NetworkInterfaces" },
  { 0x0001, 0, 0x00, "RebootCount" },
  { 0x0002, 0, 0x00, "UpTime" },
  { 0x0003, 0, 0x00, "TotalOperationalHours" },
  { 0x0004, 0, 0x00, "BootReasons" },
  { 0x0005, 0, 0x00, "ActiveHardwareFaults" },
  { 0x0006, 0, 0x00, "ActiveRadioFaults" },
  { 0x0007, 0, 0x00, "ActiveNetworkFaults" },
  { 0x0008, 0, 0x00, "TestEventTriggersEnabled" },
  { 0xFFF8, 0, 0x00, "GeneratedCommandList" },
  { 0xFFF9, 0, 0x00, "AcceptedCommandList" },
  { 0xFFFB, 0, 0x00, "AttributeList" },
  { 0xFFFC, 0, 0x00, "FeatureMap" },
  { 0xFFFD, 0, 0x00, "ClusterRevision" },
  { 0xFFFF, 0, 0x00, NULL },
};

const matter_command_t matter_Commands_0033[] = {
  { 0x0000, "TestEventTrigger" },
  { 0xFFFF, NULL },
};

const matter_attribute_t matter_Attributes_0034[] = {
  { 0x0000, 0, 0x00, "ThreadMetrics" },
  { 0x0001, 0, 0x00, "CurrentHeapFree" },
  { 0x0002, 0, 0x00, "CurrentHeapUsed" },
  { 0x0003, 0, 0x00, "CurrentHeapHighWatermark" },
  { 0xFFF8, 0, 0x00, "GeneratedCommandList" },
  { 0xFFF9, 0, 0x00, "AcceptedCommandList" },
  { 0xFFFB, 0, 0x00, "AttributeList" },
  { 0xFFFC, 0, 0x00, "FeatureMap" },
  { 0xFFFD, 0, 0x00, "ClusterRevision" },
  { 0xFFFF, 0, 0x00, NULL },
};

const matter_command_t matter_Commands_0034[] = {
  { 0x0000, "ResetWatermarks" },
  { 0xFFFF, NULL },
};

const matter_attribute_t matter_Attributes_0035[] = {
  { 0x0000, 0, 0x00, "Channel" },
  { 0x0001, 0, 0x00, "RoutingRole" },
  { 0x0002, 0, 0x00, "NetworkName" },
  { 0x0003, 0, 0x00, "PanId" },
  { 0x0004, 0, 0x00, "ExtendedPanId" },
  { 0x0005, 0, 0x00, "MeshLocalPrefix" },
  { 0x0006, 0, 0x00, "OverrunCount" },
  { 0x0007, 0, 0x00, "NeighborTableList" },
  { 0x0008, 0, 0x00, "RouteTableList" },
  { 0x0009, 0, 0x00, "PartitionId" },
  { 0x000A, 0, 0x00, "Weighting" },
  { 0x000B, 0, 0x00, "DataVersion" },
  { 0x000C, 0, 0x00, "StableDataVersion" },
  { 0x000D, 0, 0x00, "LeaderRouterId" },
  { 0x000E, 0, 0x00, "DetachedRoleCount" },
  { 0x000F, 0, 0x00, "ChildRoleCount" },
  { 0x0010, 0, 0x00, "RouterRoleCount" },
  { 0x0011, 0, 0x00, "LeaderRoleCount" },
  { 0x0012, 0, 0x00, "AttachAttemptCount" },
  { 0x0013, 0, 0x00, "PartitionIdChangeCount" },
  { 0x0014, 0, 0x00, "BetterPartitionAttachAttemptCount" },
  { 0x0015, 0, 0x00, "ParentChangeCount" },
  { 0x0016, 0, 0x00, "TxTotalCount" },
  { 0x0017, 0, 0x00, "TxUnicastCount" },
  { 0x0018, 0, 0x00, "TxBroadcastCount" },
  { 0x0019, 0, 0x00, "TxAckRequestedCount" },
  { 0x001A, 0, 0x00, "TxAckedCount" },
  { 0x001B, 0, 0x00, "TxNoAckRequestedCount" },
  { 0x001C, 0, 0x00, "TxDataCount" },
  { 0x001D, 0, 0x00, "TxDataPollCount" },
  { 0x001E, 0, 0x00, "TxBeaconCount" },
  { 0x001F, 0, 0x00, "TxBeaconRequestCount" },
  { 0x0020, 0, 0x00, "TxOtherCount" },
  { 0x0021, 0, 0x00, "TxRetryCount" },
  { 0x0022, 0, 0x00, "TxDirectMaxRetryExpiryCount" },
  { 0x0023, 0, 0x00, "TxIndirectMaxRetryExpiryCount" },
  { 0x0024, 0, 0x00, "TxErrCcaCount" },
  { 0x0025, 0, 0x00, "TxErrAbortCount" },
  { 0x0026, 0, 0x00, "TxErrBusyChannelCount" },
  { 0x0027, 0, 0x00, "RxTotalCount" },
  { 0x0028, 0, 0x00, "RxUnicastCount" },
  { 0x0029, 0, 0x00, "RxBroadcastCount" },
  { 0x002A, 0, 0x00, "RxDataCount" },
  { 0x002B, 0, 0x00, "RxDataPollCount" },
  { 0x002C, 0, 0x00, "RxBeaconCount" },
  { 0x002D, 0, 0x00, "RxBeaconRequestCount" },
  { 0x002E, 0, 0x00, "RxOtherCount" },
  { 0x002F, 0, 0x00, "RxAddressFilteredCount" },
  { 0x0030, 0, 0x00, "RxDestAddrFilteredCount" },
  { 0x0031, 0, 0x00, "RxDuplicatedCount" },
  { 0x0032, 0, 0x00, "RxErrNoFrameCount" },
  { 0x0033, 0, 0x00, "RxErrUnknownNeighborCount" },
  { 0x0034, 0, 0x00, "RxErrInvalidSrcAddrCount" },
  { 0x0035, 0, 0x00, "RxErrSecCount" },
  { 0x0036, 0, 0x00, "RxErrFcsCount" },
  { 0x0037, 0, 0x00, "RxErrOtherCount" },
  { 0x0038, 0, 0x00, "ActiveTimestamp" },
  { 0x0039, 0, 0x00, "PendingTimestamp" },
  { 0x003A, 0, 0x00, "Delay" },
  { 0x003B, 0, 0x00, "SecurityPolicy" },
  { 0x003C, 0, 0x00, "ChannelPage0Mask" },
  { 0x003D, 0, 0x00, "OperationalDatasetComponents" },
  { 0x003E, 0, 0x00, "ActiveNetworkFaultsList" },
  { 0xFFF8, 0, 0x00, "GeneratedCommandList" },
  { 0xFFF9, 0, 0x00, "AcceptedCommandList" },
  { 0xFFFB, 0, 0x00, "AttributeList" },
  { 0xFFFC, 0, 0x00, "FeatureMap" },
  { 0xFFFD, 0, 0x00, "ClusterRevision" },
  { 0xFFFF, 0, 0x00, NULL },
};

const matter_command_t matter_Commands_0035[] = {
  { 0x0000, "ResetCounts" },
  { 0xFFFF, NULL },
};

const matter_attribute_t matter_Attributes_0036[] = {
  { 0x0000, 0, 0x00, "Bssid" },
  { 0x0001, 0, 0x00, "SecurityType" },
  { 0x0002, 0, 0x00, "WiFiVersion" },
  { 0x0003, 0, 0x00, "ChannelNumber" },
  { 0x0004, 0, 0x00, "Rssi" },
  { 0x0005, 0, 0x00, "BeaconLostCount" },
  { 0x0006, 0, 0x00, "BeaconRxCount" },
  { 0x0007, 0, 0x00, "PacketMulticastRxCount" },
  { 0x0008, 0, 0x00, "PacketMulticastTxCount" },
  { 0x0009, 0, 0x00, "PacketUnicastRxCount" },
  { 0x000A, 0, 0x00, "PacketUnicastTxCount" },
  { 0x000B, 0, 0x00, "CurrentMaxRate" },
  { 0x000C, 0, 0x00, "OverrunCount" },
  { 0xFFF8, 0, 0x00, "GeneratedCommandList" },
  { 0xFFF9, 0, 0x00, "AcceptedCommandList" },
  { 0xFFFB, 0, 0x00, "AttributeList" },
  { 0xFFFC, 0, 0x00, "FeatureMap" },
  { 0xFFFD, 0, 0x00, "ClusterRevision" },
  { 0xFFFF, 0, 0x00, NULL },
};

const matter_command_t matter_Commands_0036[] = {
  { 0x0000, "ResetCounts" },
  { 0xFFFF, NULL },
};

const matter_attribute_t matter_Attributes_0037[] = {
  { 0x0000, 0, 0x00, "PHYRate" },
  { 0x0001, 0, 0x00, "FullDuplex" },
  { 0x0002, 0, 0x00, "PacketRxCount" },
  { 0x0003, 0, 0x00, "PacketTxCount" },
  { 0x0004, 0, 0x00, "TxErrCount" },
  { 0x0005, 0, 0x00, "CollisionCount" },
  { 0x0006, 0, 0x00, "OverrunCount" },
  { 0x0007, 0, 0x00, "CarrierDetect" },
  { 0x0008, 0, 0x00, "TimeSinceReset" },
  { 0xFFF8, 0, 0x00, "GeneratedCommandList" },
  { 0xFFF9, 0, 0x00, "AcceptedCommandList" },
  { 0xFFFB, 0, 0x00, "AttributeList" },
  { 0xFFFC, 0, 0x00, "FeatureMap" },
  { 0xFFFD, 0, 0x00, "ClusterRevision" },
  { 0xFFFF, 0, 0x00, NULL },
};

const matter_command_t matter_Commands_0037[] = {
  { 0x0000, "ResetCounts" },
  { 0xFFFF, NULL },
};

const matter_attribute_t matter_Attributes_0039[] = {
  { 0x0001, 0, 0x00, "VendorName" },
  { 0x0002, 0, 0x00, "VendorID" },
  { 0x0003, 0, 0x00, "ProductName" },
  { 0x0005, 0, 0x00, "NodeLabel" },
  { 0x0007, 0, 0x00, "HardwareVersion" },
  { 0x0008, 0, 0x00, "HardwareVersionString" },
  { 0x0009, 0, 0x00, "SoftwareVersion" },
  { 0x000A, 0, 0x00, "SoftwareVersionString" },
  { 0x000B, 0, 0x00, "ManufacturingDate" },
  { 0x000C, 0, 0x00, "PartNumber" },
  { 0x000D, 0, 0x00, "ProductURL" },
  { 0x000E, 0, 0x00, "ProductLabel" },
  { 0x000F, 0, 0x00, "SerialNumber" },
  { 0x0011, 0, 0x00, "Reachable" },
  { 0x0012, 0, 0x00, "UniqueID" },
  { 0xFFF8, 0, 0x00, "GeneratedCommandList" },
  { 0xFFF9, 0, 0x00, "AcceptedCommandList" },
  { 0xFFFB, 0, 0x00, "AttributeList" },
  { 0xFFFC, 0, 0x00, "FeatureMap" },
  { 0xFFFD, 0, 0x00, "ClusterRevision" },
  { 0xFFFF, 0, 0x00, NULL },
};

const matter_command_t matter_Commands_0039[] = {
  { 0xFFFF, NULL },
};

const matter_attribute_t matter_Attributes_003B[] = {
  { 0x0000, 0, 0x00, "NumberOfPositions" },
  { 0x0001, 0, 0x00, "CurrentPosition" },
  { 0x0002, 0, 0x00, "MultiPressMax" },
  { 0xFFF8, 0, 0x00, "GeneratedCommandList" },
  { 0xFFF9, 0, 0x00, "AcceptedCommandList" },
  { 0xFFFB, 0, 0x00, "AttributeList" },
  { 0xFFFC, 0, 0x00, "FeatureMap" },
  { 0xFFFD, 0, 0x00, "ClusterRevision" },
  { 0xFFFF, 0, 0x00, NULL },
};

const matter_command_t matter_Commands_003B[] = {
  { 0xFFFF, NULL },
};

const matter_attribute_t matter_Attributes_003C[] = {
  { 0x0000, 0, 0x00, "WindowStatus" },
  { 0x0001, 0, 0x00, "AdminFabricIndex" },
  { 0x0002, 0, 0x00, "AdminVendorId" },
  { 0xFFF8, 0, 0x00, "GeneratedCommandList" },
  { 0xFFF9, 0, 0x00, "AcceptedCommandList" },
  { 0xFFFB, 0, 0x00, "AttributeList" },
  { 0xFFFC, 0, 0x00, "FeatureMap" },
  { 0xFFFD, 0, 0x00, "ClusterRevision" },
  { 0xFFFF, 0, 0x00, NULL },
};

const matter_command_t matter_Commands_003C[] = {
  { 0x0000, "OpenCommissioningWindow" },
  { 0x0001, "OpenBasicCommissioningWindow" },
  { 0x0002, "RevokeCommissioning" },
  { 0xFFFF, NULL },
};

const matter_attribute_t matter_Attributes_003E[] = {
  { 0x0000, 0, 0x00, "NOCs" },
  { 0x0001, 0, 0x00, "Fabrics" },
  { 0x0002, 0, 0x00, "SupportedFabrics" },
  { 0x0003, 0, 0x00, "CommissionedFabrics" },
  { 0x0004, 0, 0x00, "TrustedRootCertificates" },
  { 0x0005, 0, 0x00, "CurrentFabricIndex" },
  { 0xFFF8, 0, 0x00, "GeneratedCommandList" },
  { 0xFFF9, 0, 0x00, "AcceptedCommandList" },
  { 0xFFFB, 0, 0x00, "AttributeList" },
  { 0xFFFC, 0, 0x00, "FeatureMap" },
  { 0xFFFD, 0, 0x00, "ClusterRevision" },
  { 0xFFFF, 0, 0x00, NULL },
};

const matter_command_t matter_Commands_003E[] = {
  { 0x0000, "AttestationRequest" },
  { 0x0001, "AttestationResponse" },
  { 0x0002, "CertificateChainRequest" },
  { 0x0003, "CertificateChainResponse" },
  { 0x0004, "CSRRequest" },
  { 0x0005, "CSRResponse" },
  { 0x0006, "AddNOC" },
  { 0x0007, "UpdateNOC" },
  { 0x0008, "NOCResponse" },
  { 0x0009, "UpdateFabricLabel" },
  { 0x000A, "RemoveFabric" },
  { 0x000B, "AddTrustedRootCertificate" },
  { 0xFFFF, NULL },
};

const matter_attribute_t matter_Attributes_003F[] = {
  { 0x0000, 0, 0x00, "GroupKeyMap" },
  { 0x0001, 0, 0x00, "GroupTable" },
  { 0x0002, 0, 0x00, "MaxGroupsPerFabric" },
  { 0x0003, 0, 0x00, "MaxGroupKeysPerFabric" },
  { 0xFFF8, 0, 0x00, "GeneratedCommandList" },
  { 0xFFF9, 0, 0x00, "AcceptedCommandList" },
  { 0xFFFB, 0, 0x00, "AttributeList" },
  { 0xFFFC, 0, 0x00, "FeatureMap" },
  { 0xFFFD, 0, 0x00, "ClusterRevision" },
  { 0xFFFF, 0, 0x00, NULL },
};

const matter_command_t matter_Commands_003F[] = {
  { 0x0000, "KeySetWrite" },
  { 0x0001, "KeySetRead" },
  { 0x0003, "KeySetRemove" },
  { 0x0004, "KeySetReadAllIndices" },
  { 0xFFFF, NULL },
};

const matter_attribute_t matter_Attributes_0040[] = {
  { 0x0000, 0, 0x00, "LabelList" },
  { 0xFFF8, 0, 0x00, "GeneratedCommandList" },
  { 0xFFF9, 0, 0x00, "AcceptedCommandList" },
  { 0xFFFB, 0, 0x00, "AttributeList" },
  { 0xFFFC, 0, 0x00, "FeatureMap" },
  { 0xFFFD, 0, 0x00, "ClusterRevision" },
  { 0xFFFF, 0, 0x00, NULL },
};

const matter_command_t matter_Commands_0040[] = {
  { 0xFFFF, NULL },
};

const matter_attribute_t matter_Attributes_0041[] = {
  { 0x0000, 0, 0x00, "LabelList" },
  { 0xFFF8, 0, 0x00, "GeneratedCommandList" },
  { 0xFFF9, 0, 0x00, "AcceptedCommandList" },
  { 0xFFFC, 0, 0x00, "FeatureMap" },
  { 0xFFFD, 0, 0x00, "ClusterRevision" },
  { 0xFFFF, 0, 0x00, NULL },
};

const matter_command_t matter_Commands_0041[] = {
  { 0xFFFF, NULL },
};

const matter_attribute_t matter_Attributes_0045[] = {
  { 0x0000, 0, 0x00, "StateValue" },
  { 0xFFF8, 0, 0x00, "GeneratedCommandList" },
  { 0xFFF9, 0, 0x00, "AcceptedCommandList" },
  { 0xFFFB, 0, 0x00, "AttributeList" },
  { 0xFFFC, 0, 0x00, "FeatureMap" },
  { 0xFFFD, 0, 0x00, "ClusterRevision" },
  { 0xFFFF, 0, 0x00, NULL },
};

const matter_command_t matter_Commands_0045[] = {
  { 0xFFFF, NULL },
};

const matter_attribute_t matter_Attributes_0050[] = {
  { 0x0000, 0, 0x00, "Description" },
  { 0x0001, 0, 0x00, "StandardNamespace" },
  { 0x0002, 0, 0x00, "SupportedModes" },
  { 0x0003, 0, 0x00, "CurrentMode" },
  { 0x0004, 0, 0x00, "StartUpMode" },
  { 0x0005, 0, 0x00, "OnMode" },
  { 0xFFF8, 0, 0x00, "GeneratedCommandList" },
  { 0xFFF9, 0, 0x00, "AcceptedCommandList" },
  { 0xFFFB, 0, 0x00, "AttributeList" },
  { 0xFFFC, 0, 0x00, "FeatureMap" },
  { 0xFFFD, 0, 0x00, "ClusterRevision" },
  { 0xFFFF, 0, 0x00, NULL },
};

const matter_command_t matter_Commands_0050[] = {
  { 0x0000, "ChangeToMode" },
  { 0xFFFF, NULL },
};

const matter_attribute_t matter_Attributes_0101[] = {
  { 0x0000, 0, 0x00, "LockState" },
  { 0x0001, 0, 0x00, "LockType" },
  { 0x0002, 0, 0x00, "ActuatorEnabled" },
  { 0x0003, 0, 0x00, "DoorState" },
  { 0x0011, 0, 0x00, "NumberOfTotalUsersSupported" },
  { 0x0012, 0, 0x00, "NumberOfPINUsersSupported" },
  { 0x0013, 0, 0x00, "NumberOfRFIDUsersSupported" },
  { 0x0014, 0, 0x00, "NumberOfWeekDaySchedulesSupportedPerUser" },
  { 0x0015, 0, 0x00, "NumberOfYearDaySchedulesSupportedPerUser" },
  { 0x0016, 0, 0x00, "NumberOfHolidaySchedulesSupported" },
  { 0x0017, 0, 0x00, "MaxPINCodeLength" },
  { 0x0018, 0, 0x00, "MinPINCodeLength" },
  { 0x0019, 0, 0x00, "MaxRFIDCodeLength" },
  { 0x001A, 0, 0x00, "MinRFIDCodeLength" },
  { 0x001C, 0, 0x00, "NumberOfCredentialsSupportedPerUser" },
  { 0x0021, 0, 0x00, "Language" },
  { 0x0023, 0, 0x00, "AutoRelockTime" },
  { 0x0024, 0, 0x00, "SoundVolume" },
  { 0x0025, 0, 0x00, "OperatingMode" },
  { 0x0026, 0, 0x00, "SupportedOperatingModes" },
  { 0x0029, 0, 0x00, "EnableOneTouchLocking" },
  { 0x002B, 0, 0x00, "EnablePrivacyModeButton" },
  { 0x0030, 0, 0x00, "WrongCodeEntryLimit" },
  { 0x0031, 0, 0x00, "UserCodeTemporaryDisableTime" },
  { 0x0033, 0, 0x00, "RequirePINforRemoteOperation" },
  { 0xFFF8, 0, 0x00, "GeneratedCommandList" },
  { 0xFFF9, 0, 0x00, "AcceptedCommandList" },
  { 0xFFFB, 0, 0x00, "AttributeList" },
  { 0xFFFC, 0, 0x00, "FeatureMap" },
  { 0xFFFD, 0, 0x00, "ClusterRevision" },
  { 0xFFFF, 0, 0x00, NULL },
};

const matter_command_t matter_Commands_0101[] = {
  { 0x0000, "LockDoor" },
  { 0x0001, "UnlockDoor" },
  { 0x0003, "UnlockWithTimeout" },
  { 0x000B, "SetWeekDaySchedule" },
  { 0x000C, "GetWeekDaySchedule" },
  { 0x000D, "ClearWeekDaySchedule" },
  { 0x000E, "SetYearDaySchedule" },
  { 0x000F, "GetYearDaySchedule" },
  { 0x0010, "ClearYearDaySchedule" },
  { 0x0011, "SetHolidaySchedule" },
  { 0x0012, "GetHolidaySchedule" },
  { 0x0013, "ClearHolidaySchedule" },
  { 0x001A, "SetUser" },
  { 0x001B, "GetUser" },
  { 0x001D, "ClearUser" },
  { 0x0022, "SetCredential" },
  { 0x0024, "GetCredentialStatus" },
  { 0x0026, "ClearCredential" },
  { 0xFFFF, NULL },
};

const matter_attribute_t matter_Attributes_0102[] = {
  { 0x0000, 0, 0x00, "Type" },
  { 0x0001, 0, 0x00, "PhysicalClosedLimitLift" },
  { 0x0002, 0, 0x00, "PhysicalClosedLimitTilt" },
  { 0x0003, 0, 0x00, "CurrentPositionLift" },
  { 0x0004, 0, 0x00, "CurrentPositionTilt" },
  { 0x0005, 0, 0x00, "NumberOfActuationsLift" },
  { 0x0006, 0, 0x00, "NumberOfActuationsTilt" },
  { 0x0007, 0, 0x00, "ConfigStatus" },
  { 0x0008, 0, 0x00, "CurrentPositionLiftPercentage" },
  { 0x0009, 0, 0x00, "CurrentPositionTiltPercentage" },
  { 0x000A, 0, 0x00, "OperationalStatus" },
  { 0x000B, 0, 0x00, "TargetPositionLiftPercent100ths" },
  { 0x000C, 0, 0x00, "TargetPositionTiltPercent100ths" },
  { 0x000D, 0, 0x00, "EndProductType" },
  { 0x000E, 0, 0x00, "CurrentPositionLiftPercent100ths" },
  { 0x000F, 0, 0x00, "CurrentPositionTiltPercent100ths" },
  { 0x0010, 0, 0x00, "InstalledOpenLimitLift" },
  { 0x0011, 0, 0x00, "InstalledClosedLimitLift" },
  { 0x0012, 0, 0x00, "InstalledOpenLimitTilt" },
  { 0x0013, 0, 0x00, "InstalledClosedLimitTilt" },
  { 0x0017, 0, 0x00, "Mode" },
  { 0x001A, 0, 0x00, "SafetyStatus" },
  { 0xFFF8, 0, 0x00, "GeneratedCommandList" },
  { 0xFFF9, 0, 0x00, "AcceptedCommandList" },
  { 0xFFFB, 0, 0x00, "AttributeList" },
  { 0xFFFC, 0, 0x00, "FeatureMap" },
  { 0xFFFD, 0, 0x00, "ClusterRevision" },
  { 0xFFFF, 0, 0x00, NULL },
};

const matter_command_t matter_Commands_0102[] = {
  { 0x0000, "UpOrOpen" },
  { 0x0001, "DownOrClose" },
  { 0x0002, "StopMotion" },
  { 0x0004, "GoToLiftValue" },
  { 0x0005, "GoToLiftPercentage" },
  { 0x0007, "GoToTiltValue" },
  { 0x0008, "GoToTiltPercentage" },
  { 0xFFFF, NULL },
};

const matter_attribute_t matter_Attributes_0103[] = {
  { 0x0001, 0, 0x00, "BarrierMovingState" },
  { 0x0002, 0, 0x00, "BarrierSafetyStatus" },
  { 0x0003, 0, 0x00, "BarrierCapabilities" },
  { 0x000A, 0, 0x00, "BarrierPosition" },
  { 0xFFF8, 0, 0x00, "GeneratedCommandList" },
  { 0xFFF9, 0, 0x00, "AcceptedCommandList" },
  { 0xFFFB, 0, 0x00, "AttributeList" },
  { 0xFFFC, 0, 0x00, "FeatureMap" },
  { 0xFFFD, 0, 0x00, "ClusterRevision" },
  { 0xFFFF, 0, 0x00, NULL },
};

const matter_command_t matter_Commands_0103[] = {
  { 0x0000, "BarrierControlGoToPercent" },
  { 0x0001, "BarrierControlStop" },
  { 0xFFFF, NULL },
};

const matter_attribute_t matter_Attributes_0200[] = {
  { 0x0000, 0, 0x00, "MaxPressure" },
  { 0x0001, 0, 0x00, "MaxSpeed" },
  { 0x0002, 0, 0x00, "MaxFlow" },
  { 0x0003, 0, 0x00, "MinConstPressure" },
  { 0x0004, 0, 0x00, "MaxConstPressure" },
  { 0x0005, 0, 0x00, "MinCompPressure" },
  { 0x0006, 0, 0x00, "MaxCompPressure" },
  { 0x0007, 0, 0x00, "MinConstSpeed" },
  { 0x0008, 0, 0x00, "MaxConstSpeed" },
  { 0x0009, 0, 0x00, "MinConstFlow" },
  { 0x000A, 0, 0x00, "MaxConstFlow" },
  { 0x000B, 0, 0x00, "MinConstTemp" },
  { 0x000C, 0, 0x00, "MaxConstTemp" },
  { 0x0010, 0, 0x00, "PumpStatus" },
  { 0x0011, 0, 0x00, "EffectiveOperationMode" },
  { 0x0012, 0, 0x00, "EffectiveControlMode" },
  { 0x0013, 0, 0x00, "Capacity" },
  { 0x0014, 0, 0x00, "Speed" },
  { 0x0015, 0, 0x00, "LifetimeRunningHours" },
  { 0x0016, 0, 0x00, "Power" },
  { 0x0017, 0, 0x00, "LifetimeEnergyConsumed" },
  { 0x0020, 0, 0x00, "OperationMode" },
  { 0x0021, 0, 0x00, "ControlMode" },
  { 0xFFF8, 0, 0x00, "GeneratedCommandList" },
  { 0xFFF9, 0, 0x00, "AcceptedCommandList" },
  { 0xFFFB, 0, 0x00, "AttributeList" },
  { 0xFFFC, 0, 0x00, "FeatureMap" },
  { 0xFFFD, 0, 0x00, "ClusterRevision" },
  { 0xFFFF, 0, 0x00, NULL },
};

const matter_command_t matter_Commands_0200[] = {
  { 0xFFFF, NULL },
};

const matter_attribute_t matter_Attributes_0201[] = {
  { 0x0000, 0, 0x00, "LocalTemperature" },
  { 0x0003, 0, 0x00, "AbsMinHeatSetpointLimit" },
  { 0x0004, 0, 0x00, "AbsMaxHeatSetpointLimit" },
  { 0x0005, 0, 0x00, "AbsMinCoolSetpointLimit" },
  { 0x0006, 0, 0x00, "AbsMaxCoolSetpointLimit" },
  { 0x0011, 0, 0x00, "OccupiedCoolingSetpoint" },
  { 0x0012, 0, 0x00, "OccupiedHeatingSetpoint" },
  { 0x0015, 0, 0x00, "MinHeatSetpointLimit" },
  { 0x0016, 0, 0x00, "MaxHeatSetpointLimit" },
  { 0x0017, 0, 0x00, "MinCoolSetpointLimit" },
  { 0x0018, 0, 0x00, "MaxCoolSetpointLimit" },
  { 0x0019, 0, 0x00, "MinSetpointDeadBand" },
  { 0x001B, 0, 0x00, "ControlSequenceOfOperation" },
  { 0x001C, 0, 0x00, "SystemMode" },
  { 0x0020, 0, 0x00, "StartOfWeek" },
  { 0x0021, 0, 0x00, "NumberOfWeeklyTransitions" },
  { 0x0022, 0, 0x00, "NumberOfDailyTransitions" },
  { 0xFFFB, 0, 0x00, "AttributeList" },
  { 0xFFFC, 0, 0x00, "FeatureMap" },
  { 0xFFFD, 0, 0x00, "ClusterRevision" },
  { 0xFFFF, 0, 0x00, NULL },
};

const matter_command_t matter_Commands_0201[] = {
  { 0x0000, "SetpointRaiseLower" },
  { 0x0001, "SetWeeklySchedule" },
  { 0x0002, "GetWeeklySchedule" },
  { 0x0003, "ClearWeeklySchedule" },
  { 0xFFFF, NULL },
};

const matter_attribute_t matter_Attributes_0202[] = {
  { 0x0000, 0, 0x00, "FanMode" },
  { 0x0001, 0, 0x00, "FanModeSequence" },
  { 0x0002, 0, 0x00, "PercentSetting" },
  { 0x0003, 0, 0x00, "PercentCurrent" },
  { 0x0004, 0, 0x00, "SpeedMax" },
  { 0x0005, 0, 0x00, "SpeedSetting" },
  { 0x0006, 0, 0x00, "SpeedCurrent" },
  { 0x0007, 0, 0x00, "RockSupport" },
  { 0x0008, 0, 0x00, "RockSetting" },
  { 0x0009, 0, 0x00, "WindSupport" },
  { 0x000A, 0, 0x00, "WindSetting" },
  { 0xFFF8, 0, 0x00, "GeneratedCommandList" },
  { 0xFFF9, 0, 0x00, "AcceptedCommandList" },
  { 0xFFFB, 0, 0x00, "AttributeList" },
  { 0xFFFC, 0, 0x00, "FeatureMap" },
  { 0xFFFD, 0, 0x00, "ClusterRevision" },
  { 0xFFFF, 0, 0x00, NULL },
};

const matter_command_t matter_Commands_0202[] = {
  { 0xFFFF, NULL },
};

const matter_attribute_t matter_Attributes_0204[] = {
  { 0x0000, 0, 0x00, "TemperatureDisplayMode" },
  { 0x0001, 0, 0x00, "KeypadLockout" },
  { 0x0002, 0, 0x00, "ScheduleProgrammingVisibility" },
  { 0xFFF8, 0, 0x00, "GeneratedCommandList" },
  { 0xFFF9, 0, 0x00, "AcceptedCommandList" },
  { 0xFFFB, 0, 0x00, "AttributeList" },
  { 0xFFFC, 0, 0x00, "FeatureMap" },
  { 0xFFFD, 0, 0x00, "ClusterRevision" },
  { 0xFFFF, 0, 0x00, NULL },
};

const matter_command_t matter_Commands_0204[] = {
  { 0xFFFF, NULL },
};

const matter_attribute_t matter_Attributes_0300[] = {
  { 0x0000, 0, 0x00, "CurrentHue" },
  { 0x0001, 0, 0x00, "CurrentSaturation" },
  { 0x0002, 0, 0x00, "RemainingTime" },
  { 0x0003, 0, 0x00, "CurrentX" },
  { 0x0004, 0, 0x00, "CurrentY" },
  { 0x0005, 0, 0x00, "DriftCompensation" },
  { 0x0006, 0, 0x00, "CompensationText" },
  { 0x0007, 0, 0x00, "ColorTemperatureMireds" },
  { 0x0008, 0, 0x00, "ColorMode" },
  { 0x000F, 0, 0x00, "Options" },
  { 0x0010, 0, 0x00, "NumberOfPrimaries" },
  { 0x0011, 0, 0x00, "Primary1X" },
  { 0x0012, 0, 0x00, "Primary1Y" },
  { 0x0013, 0, 0x00, "Primary1Intensity" },
  { 0x0015, 0, 0x00, "Primary2X" },
  { 0x0016, 0, 0x00, "Primary2Y" },
  { 0x0017, 0, 0x00, "Primary2Intensity" },
  { 0x0019, 0, 0x00, "Primary3X" },
  { 0x001A, 0, 0x00, "Primary3Y" },
  { 0x001B, 0, 0x00, "Primary3Intensity" },
  { 0x0020, 0, 0x00, "Primary4X" },
  { 0x0021, 0, 0x00, "Primary4Y" },
  { 0x0022, 0, 0x00, "Primary4Intensity" },
  { 0x0024, 0, 0x00, "Primary5X" },
  { 0x0025, 0, 0x00, "Primary5Y" },
  { 0x0026, 0, 0x00, "Primary5Intensity" },
  { 0x0028, 0, 0x00, "Primary6X" },
  { 0x0029, 0, 0x00, "Primary6Y" },
  { 0x002A, 0, 0x00, "Primary6Intensity" },
  { 0x0030, 0, 0x00, "WhitePointX" },
  { 0x0031, 0, 0x00, "WhitePointY" },
  { 0x0032, 0, 0x00, "ColorPointRX" },
  { 0x0033, 0, 0x00, "ColorPointRY" },
  { 0x0034, 0, 0x00, "ColorPointRIntensity" },
  { 0x0036, 0, 0x00, "ColorPointGX" },
  { 0x0037, 0, 0x00, "ColorPointGY" },
  { 0x0038, 0, 0x00, "ColorPointGIntensity" },
  { 0x003A, 0, 0x00, "ColorPointBX" },
  { 0x003B, 0, 0x00, "ColorPointBY" },
  { 0x003C, 0, 0x00, "ColorPointBIntensity" },
  { 0x4000, 0, 0x00, "EnhancedCurrentHue" },
  { 0x4001, 0, 0x00, "EnhancedColorMode" },
  { 0x4002, 0, 0x00, "ColorLoopActive" },
  { 0x4003, 0, 0x00, "ColorLoopDirection" },
  { 0x4004, 0, 0x00, "ColorLoopTime" },
  { 0x4005, 0, 0x00, "ColorLoopStartEnhancedHue" },
  { 0x4006, 0, 0x00, "ColorLoopStoredEnhancedHue" },
  { 0x400A, 0, 0x00, "ColorCapabilities" },
  { 0x400B, 0, 0x00, "ColorTempPhysicalMinMireds" },
  { 0x400C, 0, 0x00, "ColorTempPhysicalMaxMireds" },
  { 0x400D, 0, 0x00, "CoupleColorTempToLevelMinMireds" },
  { 0x4010, 0, 0x00, "StartUpColorTemperatureMireds" },
  { 0xFFF8, 0, 0x00, "GeneratedCommandList" },
  { 0xFFF9, 0, 0x00, "AcceptedCommandList" },
  { 0xFFFB, 0, 0x00, "AttributeList" },
  { 0xFFFC, 0, 0x00, "FeatureMap" },
  { 0xFFFD, 0, 0x00, "ClusterRevision" },
  { 0xFFFF, 0, 0x00, NULL },
};

const matter_command_t matter_Commands_0300[] = {
  { 0x0000, "MoveToHue" },
  { 0x0001, "MoveHue" },
  { 0x0002, "StepHue" },
  { 0x0003, "MoveToSaturation" },
  { 0x0004, "MoveSaturation" },
  { 0x0005, "StepSaturation" },
  { 0x0006, "MoveToHueAndSaturation" },
  { 0x0007, "MoveToColor" },
  { 0x0008, "MoveColor" },
  { 0x0009, "StepColor" },
  { 0x000A, "MoveToColorTemperature" },
  { 0x0040, "EnhancedMoveToHue" },
  { 0x0041, "EnhancedMoveHue" },
  { 0x0042, "EnhancedStepHue" },
  { 0x0043, "EnhancedMoveToHueAndSaturation" },
  { 0x0044, "ColorLoopSet" },
  { 0x0047, "StopMoveStep" },
  { 0x004B, "MoveColorTemperature" },
  { 0x004C, "StepColorTemperature" },
  { 0xFFFF, NULL },
};

const matter_attribute_t matter_Attributes_0301[] = {
  { 0x0000, 0, 0x00, "PhysicalMinLevel" },
  { 0x0001, 0, 0x00, "PhysicalMaxLevel" },
  { 0x0002, 0, 0x00, "BallastStatus" },
  { 0x0010, 0, 0x00, "MinLevel" },
  { 0x0011, 0, 0x00, "MaxLevel" },
  { 0x0014, 0, 0x00, "IntrinsicBalanceFactor" },
  { 0x0015, 0, 0x00, "BallastFactorAdjustment" },
  { 0x0020, 0, 0x00, "LampQuantity" },
  { 0x0030, 0, 0x00, "LampType" },
  { 0x0031, 0, 0x00, "LampManufacturer" },
  { 0x0032, 0, 0x00, "LampRatedHours" },
  { 0x0033, 0, 0x00, "LampBurnHours" },
  { 0x0034, 0, 0x00, "LampAlarmMode" },
  { 0x0035, 0, 0x00, "LampBurnHoursTripPoint" },
  { 0xFFF8, 0, 0x00, "GeneratedCommandList" },
  { 0xFFF9, 0, 0x00, "AcceptedCommandList" },
  { 0xFFFB, 0, 0x00, "AttributeList" },
  { 0xFFFC, 0, 0x00, "FeatureMap" },
  { 0xFFFD, 0, 0x00, "ClusterRevision" },
  { 0xFFFF, 0, 0x00, NULL },
};

const matter_command_t matter_Commands_0301[] = {
  { 0xFFFF, NULL },
};

const matter_attribute_t matter_Attributes_0400[] = {
  { 0x0000, 0, 0x00, "MeasuredValue" },
  { 0x0001, 0, 0x00, "MinMeasuredValue" },
  { 0x0002, 0, 0x00, "MaxMeasuredValue" },
  { 0x0003, 0, 0x00, "Tolerance" },
  { 0x0004, 0, 0x00, "LightSensorType" },
  { 0xFFF8, 0, 0x00, "GeneratedCommandList" },
  { 0xFFF9, 0, 0x00, "AcceptedCommandList" },
  { 0xFFFB, 0, 0x00, "AttributeList" },
  { 0xFFFC, 0, 0x00, "FeatureMap" },
  { 0xFFFD, 0, 0x00, "ClusterRevision" },
  { 0xFFFF, 0, 0x00, NULL },
};

const matter_command_t matter_Commands_0400[] = {
  { 0xFFFF, NULL },
};

const matter_attribute_t matter_Attributes_0402[] = {
  { 0x0000, 0, 0x00, "MeasuredValue" },
  { 0x0001, 0, 0x00, "MinMeasuredValue" },
  { 0x0002, 0, 0x00, "MaxMeasuredValue" },
  { 0x0003, 0, 0x00, "Tolerance" },
  { 0xFFFB, 0, 0x00, "AttributeList" },
  { 0xFFFC, 0, 0x00, "FeatureMap" },
  { 0xFFFD, 0, 0x00, "ClusterRevision" },
  { 0xFFFF, 0, 0x00, NULL },
};

const matter_command_t matter_Commands_0402[] = {
  { 0xFFFF, NULL },
};

const matter_attribute_t matter_Attributes_0403[] = {
  { 0x0000, 0, 0x00, "MeasuredValue" },
  { 0x0001, 0, 0x00, "MinMeasuredValue" },
  { 0x0002, 0, 0x00, "MaxMeasuredValue" },
  { 0x0003, 0, 0x00, "Tolerance" },
  { 0x0010, 0, 0x00, "ScaledValue" },
  { 0x0011, 0, 0x00, "MinScaledValue" },
  { 0x0012, 0, 0x00, "MaxScaledValue" },
  { 0x0013, 0, 0x00, "ScaledTolerance" },
  { 0x0014, 0, 0x00, "Scale" },
  { 0xFFFB, 0, 0x00, "AttributeList" },
  { 0xFFFC, 0, 0x00, "FeatureMap" },
  { 0xFFFD, 0, 0x00, "ClusterRevision" },
  { 0xFFFF, 0, 0x00, NULL },
};

const matter_command_t matter_Commands_0403[] = {
  { 0xFFFF, NULL },
};

const matter_attribute_t matter_Attributes_0404[] = {
  { 0x0000, 0, 0x00, "MeasuredValue" },
  { 0x0001, 0, 0x00, "MinMeasuredValue" },
  { 0x0002, 0, 0x00, "MaxMeasuredValue" },
  { 0x0003, 0, 0x00, "Tolerance" },
  { 0xFFF8, 0, 0x00, "GeneratedCommandList" },
  { 0xFFF9, 0, 0x00, "AcceptedCommandList" },
  { 0xFFFB, 0, 0x00, "AttributeList" },
  { 0xFFFC, 0, 0x00, "FeatureMap" },
  { 0xFFFD, 0, 0x00, "ClusterRevision" },
  { 0xFFFF, 0, 0x00, NULL },
};

const matter_command_t matter_Commands_0404[] = {
  { 0xFFFF, NULL },
};

const matter_attribute_t matter_Attributes_0405[] = {
  { 0x0000, 0, 0x00, "MeasuredValue" },
  { 0x0001, 0, 0x00, "MinMeasuredValue" },
  { 0x0002, 0, 0x00, "MaxMeasuredValue" },
  { 0x0003, 0, 0x00, "Tolerance" },
  { 0xFFF8, 0, 0x00, "GeneratedCommandList" },
  { 0xFFF9, 0, 0x00, "AcceptedCommandList" },
  { 0xFFFB, 0, 0x00, "AttributeList" },
  { 0xFFFC, 0, 0x00, "FeatureMap" },
  { 0xFFFD, 0, 0x00, "ClusterRevision" },
  { 0xFFFF, 0, 0x00, NULL },
};

const matter_command_t matter_Commands_0405[] = {
  { 0xFFFF, NULL },
};

const matter_attribute_t matter_Attributes_0406[] = {
  { 0x0000, 0, 0x00, "Occupancy" },
  { 0x0001, 0, 0x00, "OccupancySensorType" },
  { 0x0002, 0, 0x00, "OccupancySensorTypeBitmap" },
  { 0xFFF8, 0, 0x00, "GeneratedCommandList" },
  { 0xFFF9, 0, 0x00, "AcceptedCommandList" },
  { 0xFFFB, 0, 0x00, "AttributeList" },
  { 0xFFFC, 0, 0x00, "FeatureMap" },
  { 0xFFFD, 0, 0x00, "ClusterRevision" },
  { 0xFFFF, 0, 0x00, NULL },
};

const matter_command_t matter_Commands_0406[] = {
  { 0xFFFF, NULL },
};

const matter_attribute_t matter_Attributes_0503[] = {
  { 0x0000, 0, 0x00, "MACAddress" },
  { 0xFFF8, 0, 0x00, "GeneratedCommandList" },
  { 0xFFF9, 0, 0x00, "AcceptedCommandList" },
  { 0xFFFB, 0, 0x00, "AttributeList" },
  { 0xFFFC, 0, 0x00, "FeatureMap" },
  { 0xFFFD, 0, 0x00, "ClusterRevision" },
  { 0xFFFF, 0, 0x00, NULL },
};

const matter_command_t matter_Commands_0503[] = {
  { 0xFFFF, NULL },
};

const matter_attribute_t matter_Attributes_0504[] = {
  { 0x0000, 0, 0x00, "ChannelList" },
  { 0x0001, 0, 0x00, "Lineup" },
  { 0x0002, 0, 0x00, "CurrentChannel" },
  { 0xFFF8, 0, 0x00, "GeneratedCommandList" },
  { 0xFFF9, 0, 0x00, "AcceptedCommandList" },
  { 0xFFFB, 0, 0x00, "AttributeList" },
  { 0xFFFC, 0, 0x00, "FeatureMap" },
  { 0xFFFD, 0, 0x00, "ClusterRevision" },
  { 0xFFFF, 0, 0x00, NULL },
};

const matter_command_t matter_Commands_0504[] = {
  { 0x0000, "ChangeChannel" },
  { 0x0002, "ChangeChannelByNumber" },
  { 0x0003, "SkipChannel" },
  { 0xFFFF, NULL },
};

const matter_attribute_t matter_Attributes_0505[] = {
  { 0x0000, 0, 0x00, "TargetList" },
  { 0x0001, 0, 0x00, "CurrentTarget" },
  { 0xFFF8, 0, 0x00, "GeneratedCommandList" },
  { 0xFFF9, 0, 0x00, "AcceptedCommandList" },
  { 0xFFFB, 0, 0x00, "AttributeList" },
  { 0xFFFC, 0, 0x00, "FeatureMap" },
  { 0xFFFD, 0, 0x00, "ClusterRevision" },
  { 0xFFFF, 0, 0x00, NULL },
};

const matter_command_t matter_Commands_0505[] = {
  { 0x0000, "NavigateTarget" },
  { 0xFFFF, NULL },
};

const matter_attribute_t matter_Attributes_0506[] = {
  { 0x0000, 0, 0x00, "CurrentState" },
  { 0x0001, 0, 0x00, "StartTime" },
  { 0x0002, 0, 0x00, "Duration" },
  { 0x0003, 0, 0x00, "SampledPosition" },
  { 0x0004, 0, 0x00, "PlaybackSpeed" },
  { 0x0005, 0, 0x00, "SeekRangeEnd" },
  { 0x0006, 0, 0x00, "SeekRangeStart" },
  { 0xFFF8, 0, 0x00, "GeneratedCommandList" },
  { 0xFFF9, 0, 0x00, "AcceptedCommandList" },
  { 0xFFFB, 0, 0x00, "AttributeList" },
  { 0xFFFC, 0, 0x00, "FeatureMap" },
  { 0xFFFD, 0, 0x00, "ClusterRevision" },
  { 0xFFFF, 0, 0x00, NULL },
};

const matter_command_t matter_Commands_0506[] = {
  { 0x0000, "Play" },
  { 0x0001, "Pause" },
  { 0x0002, "StopPlayback" },
  { 0x0003, "StartOver" },
  { 0x0004, "Previous" },
  { 0x0005, "Next" },
  { 0x0006, "Rewind" },
  { 0x0007, "FastForward" },
  { 0x0008, "SkipForward" },
  { 0x0009, "SkipBackward" },
  { 0x000B, "Seek" },
  { 0xFFFF, NULL },
};

const matter_attribute_t matter_Attributes_0507[] = {
  { 0x0000, 0, 0x00, "InputList" },
  { 0x0001, 0, 0x00, "CurrentInput" },
  { 0xFFF8, 0, 0x00, "GeneratedCommandList" },
  { 0xFFF9, 0, 0x00, "AcceptedCommandList" },
  { 0xFFFB, 0, 0x00, "AttributeList" },
  { 0xFFFC, 0, 0x00, "FeatureMap" },
  { 0xFFFD, 0, 0x00, "ClusterRevision" },
  { 0xFFFF, 0, 0x00, NULL },
};

const matter_command_t matter_Commands_0507[] = {
  { 0x0000, "SelectInput" },
  { 0x0001, "ShowInputStatus" },
  { 0x0002, "HideInputStatus" },
  { 0x0003, "RenameInput" },
  { 0xFFFF, NULL },
};

const matter_attribute_t matter_Attributes_0508[] = {
  { 0xFFF8, 0, 0x00, "GeneratedCommandList" },
  { 0xFFF9, 0, 0x00, "AcceptedCommandList" },
  { 0xFFFB, 0, 0x00, "AttributeList" },
  { 0xFFFC, 0, 0x00, "FeatureMap" },
  { 0xFFFD, 0, 0x00, "ClusterRevision" },
  { 0xFFFF, 0, 0x00, NULL },
};

const matter_command_t matter_Commands_0508[] = {
  { 0x0000, "Sleep" },
  { 0xFFFF, NULL },
};

const matter_attribute_t matter_Attributes_0509[] = {
  { 0xFFF8, 0, 0x00, "GeneratedCommandList" },
  { 0xFFF9, 0, 0x00, "AcceptedCommandList" },
  { 0xFFFB, 0, 0x00, "AttributeList" },
  { 0xFFFC, 0, 0x00, "FeatureMap" },
  { 0xFFFD, 0, 0x00, "ClusterRevision" },
  { 0xFFFF, 0, 0x00, NULL },
};

const matter_command_t matter_Commands_0509[] = {
  { 0x0000, "SendKey" },
  { 0xFFFF, NULL },
};

const matter_attribute_t matter_Attributes_050A[] = {
  { 0x0000, 0, 0x00, "AcceptHeader" },
  { 0x0001, 0, 0x00, "SupportedStreamingProtocols" },
  { 0xFFF8, 0, 0x00, "GeneratedCommandList" },
  { 0xFFF9, 0, 0x00, "AcceptedCommandList" },
  { 0xFFFB, 0, 0x00, "AttributeList" },
  { 0xFFFC, 0, 0x00, "FeatureMap" },
  { 0xFFFD, 0, 0x00, "ClusterRevision" },
  { 0xFFFF, 0, 0x00, NULL },
};

const matter_command_t matter_Commands_050A[] = {
  { 0x0000, "LaunchContent" },
  { 0x0001, "LaunchURL" },
  { 0xFFFF, NULL },
};

const matter_attribute_t matter_Attributes_050B[] = {
  { 0x0000, 0, 0x00, "OutputList" },
  { 0x0001, 0, 0x00, "CurrentOutput" },
  { 0xFFF8, 0, 0x00, "GeneratedCommandList" },
  { 0xFFF9, 0, 0x00, "AcceptedCommandList" },
  { 0xFFFB, 0, 0x00, "AttributeList" },
  { 0xFFFC, 0, 0x00, "FeatureMap" },
  { 0xFFFD, 0, 0x00, "ClusterRevision" },
  { 0xFFFF, 0, 0x00, NULL },
};

const matter_command_t matter_Commands_050B[] = {
  { 0x0000, "SelectOutput" },
  { 0x0001, "RenameOutput" },
  { 0xFFFF, NULL },
};

const matter_attribute_t matter_Attributes_050C[] = {
  { 0x0000, 0, 0x00, "CatalogList" },
  { 0x0001, 0, 0x00, "CurrentApp" },
  { 0xFFF8, 0, 0x00, "GeneratedCommandList" },
  { 0xFFF9, 0, 0x00, "AcceptedCommandList" },
  { 0xFFFB, 0, 0x00, "AttributeList" },
  { 0xFFFC, 0, 0x00, "FeatureMap" },
  { 0xFFFD, 0, 0x00, "ClusterRevision" },
  { 0xFFFF, 0, 0x00, NULL },
};

const matter_command_t matter_Commands_050C[] = {
  { 0x0000, "LaunchApp" },
  { 0x0001, "StopApp" },
  { 0x0002, "HideApp" },
  { 0xFFFF, NULL },
};

const matter_attribute_t matter_Attributes_050D[] = {
  { 0x0000, 0, 0x00, "VendorName" },
  { 0x0001, 0, 0x00, "VendorID" },
  { 0x0002, 0, 0x00, "ApplicationName" },
  { 0x0003, 0, 0x00, "ProductID" },
  { 0x0004, 0, 0x00, "Application" },
  { 0x0005, 0, 0x00, "Status" },
  { 0x0006, 0, 0x00, "ApplicationVersion" },
  { 0x0007, 0, 0x00, "AllowedVendorList" },
  { 0xFFF8, 0, 0x00, "GeneratedCommandList" },
  { 0xFFF9, 0, 0x00, "AcceptedCommandList" },
  { 0xFFFB, 0, 0x00, "AttributeList" },
  { 0xFFFC, 0, 0x00, "FeatureMap" },
  { 0xFFFD, 0, 0x00, "ClusterRevision" },
  { 0xFFFF, 0, 0x00, NULL },
};

const matter_command_t matter_Commands_050D[] = {
  { 0xFFFF, NULL },
};

const matter_attribute_t matter_Attributes_050E[] = {
  { 0xFFF8, 0, 0x00, "GeneratedCommandList" },
  { 0xFFF9, 0, 0x00, "AcceptedCommandList" },
  { 0xFFFB, 0, 0x00, "AttributeList" },
  { 0xFFFC, 0, 0x00, "FeatureMap" },
  { 0xFFFD, 0, 0x00, "ClusterRevision" },
  { 0xFFFF, 0, 0x00, NULL },
};

const matter_command_t matter_Commands_050E[] = {
  { 0x0000, "GetSetupPIN" },
  { 0x0002, "Login" },
  { 0x0003, "Logout" },
  { 0xFFFF, NULL },
};

const matter_attribute_t matter_Attributes_0B04[] = {
  { 0x0000, 0, 0x00, "MeasurementType" },
  { 0x0304, 0, 0x00, "TotalActivePower" },
  { 0x0505, 0, 0x00, "RmsVoltage" },
  { 0x0506, 0, 0x00, "RmsVoltageMin" },
  { 0x0507, 0, 0x00, "RmsVoltageMax" },
  { 0x0508, 0, 0x00, "RmsCurrent" },
  { 0x0509, 0, 0x00, "RmsCurrentMin" },
  { 0x050A, 0, 0x00, "RmsCurrentMax" },
  { 0x050B, 0, 0x00, "ActivePower" },
  { 0x050C, 0, 0x00, "ActivePowerMin" },
  { 0x050D, 0, 0x00, "ActivePowerMax" },
  { 0xFFF8, 0, 0x00, "GeneratedCommandList" },
  { 0xFFF9, 0, 0x00, "AcceptedCommandList" },
  { 0xFFFB, 0, 0x00, "AttributeList" },
  { 0xFFFC, 0, 0x00, "FeatureMap" },
  { 0xFFFD, 0, 0x00, "ClusterRevision" },
  { 0xFFFF, 0, 0x00, NULL },
};

const matter_command_t matter_Commands_0B04[] = {
  { 0xFFFF, NULL },
};

const matter_cluster_t matterAllClusters[] = {
  { 0x0003, "Identify", matter_Attributes_0003, matter_Commands_0003 },
  { 0x0004, "Groups", matter_Attributes_0004, matter_Commands_0004 },
  { 0x0005, "Scenes", matter_Attributes_0005, matter_Commands_0005 },
  { 0x0006, "OnOff", matter_Attributes_0006, matter_Commands_0006 },
  { 0x0007, "OnOffSwitchConfiguration", matter_Attributes_0007, matter_Commands_0007 },
  { 0x0008, "LevelControl", matter_Attributes_0008, matter_Commands_0008 },
  { 0x000F, "BinaryInputBasic", matter_Attributes_000F, matter_Commands_000F },
  { 0x001D, "Descriptor", matter_Attributes_001D, matter_Commands_001D },
  { 0x001E, "Binding", matter_Attributes_001E, matter_Commands_001E },
  { 0x001F, "AccessControl", matter_Attributes_001F, matter_Commands_001F },
  { 0x0025, "Actions", matter_Attributes_0025, matter_Commands_0025 },
  { 0x0028, "Basic", matter_Attributes_0028, matter_Commands_0028 },
  { 0x0029, "OtaSoftwareUpdateProvider", matter_Attributes_0029, matter_Commands_0029 },
  { 0x002A, "OtaSoftwareUpdateRequestor", matter_Attributes_002A, matter_Commands_002A },
  { 0x002B, "LocalizationConfiguration", matter_Attributes_002B, matter_Commands_002B },
  { 0x002C, "TimeFormatLocalization", matter_Attributes_002C, matter_Commands_002C },
  { 0x002D, "UnitLocalization", matter_Attributes_002D, matter_Commands_002D },
  { 0x002E, "PowerSourceConfiguration", matter_Attributes_002E, matter_Commands_002E },
  { 0x002F, "PowerSource", matter_Attributes_002F, matter_Commands_002F },
  { 0x0030, "GeneralCommissioning", matter_Attributes_0030, matter_Commands_0030 },
  { 0x0031, "NetworkCommissioning", matter_Attributes_0031, matter_Commands_0031 },
  { 0x0032, "DiagnosticLogs", matter_Attributes_0032, matter_Commands_0032 },
  { 0x0033, "GeneralDiagnostics", matter_Attributes_0033, matter_Commands_0033 },
  { 0x0034, "SoftwareDiagnostics", matter_Attributes_0034, matter_Commands_0034 },
  { 0x0035, "ThreadNetworkDiagnostics", matter_Attributes_0035, matter_Commands_0035 },
  { 0x0036, "WiFiNetworkDiagnostics", matter_Attributes_0036, matter_Commands_0036 },
  { 0x0037, "EthernetNetworkDiagnostics", matter_Attributes_0037, matter_Commands_0037 },
  { 0x0039, "BridgedDeviceBasic", matter_Attributes_0039, matter_Commands_0039 },
  { 0x003B, "Switch", matter_Attributes_003B, matter_Commands_003B },
  { 0x003C, "AdministratorCommissioning", matter_Attributes_003C, matter_Commands_003C },
  { 0x003E, "OperationalCredentials", matter_Attributes_003E, matter_Commands_003E },
  { 0x003F, "GroupKeyManagement", matter_Attributes_003F, matter_Commands_003F },
  { 0x0040, "FixedLabel", matter_Attributes_0040, matter_Commands_0040 },
  { 0x0041, "UserLabel", matter_Attributes_0041, matter_Commands_0041 },
  { 0x0045, "BooleanState", matter_Attributes_0045, matter_Commands_0045 },
  { 0x0050, "ModeSelect", matter_Attributes_0050, matter_Commands_0050 },
  { 0x0101, "DoorLock", matter_Attributes_0101, matter_Commands_0101 },
  { 0x0102, "WindowCovering", matter_Attributes_0102, matter_Commands_0102 },
  { 0x0103, "BarrierControl", matter_Attributes_0103, matter_Commands_0103 },
  { 0x0200, "PumpConfigurationAndControl", matter_Attributes_0200, matter_Commands_0200 },
  { 0x0201, "Thermostat", matter_Attributes_0201, matter_Commands_0201 },
  { 0x0202, "FanControl", matter_Attributes_0202, matter_Commands_0202 },
  { 0x0204, "ThermostatUserInterfaceConfiguration", matter_Attributes_0204, matter_Commands_0204 },
  { 0x0300, "ColorControl", matter_Attributes_0300, matter_Commands_0300 },
  { 0x0301, "BallastConfiguration", matter_Attributes_0301, matter_Commands_0301 },
  { 0x0400, "IlluminanceMeasurement", matter_Attributes_0400, matter_Commands_0400 },
  { 0x0402, "TemperatureMeasurement", matter_Attributes_0402, matter_Commands_0402 },
  { 0x0403, "PressureMeasurement", matter_Attributes_0403, matter_Commands_0403 },
  { 0x0404, "FlowMeasurement", matter_Attributes_0404, matter_Commands_0404 },
  { 0x0405, "RelativeHumidityMeasurement", matter_Attributes_0405, matter_Commands_0405 },
  { 0x0406, "OccupancySensing", matter_Attributes_0406, matter_Commands_0406 },
  { 0x0503, "WakeOnLan", matter_Attributes_0503, matter_Commands_0503 },
  { 0x0504, "Channel", matter_Attributes_0504, matter_Commands_0504 },
  { 0x0505, "TargetNavigator", matter_Attributes_0505, matter_Commands_0505 },
  { 0x0506, "MediaPlayback", matter_Attributes_0506, matter_Commands_0506 },
  { 0x0507, "MediaInput", matter_Attributes_0507, matter_Commands_0507 },
  { 0x0508, "LowPower", matter_Attributes_0508, matter_Commands_0508 },
  { 0x0509, "KeypadInput", matter_Attributes_0509, matter_Commands_0509 },
  { 0x050A, "ContentLauncher", matter_Attributes_050A, matter_Commands_050A },
  { 0x050B, "AudioOutput", matter_Attributes_050B, matter_Commands_050B },
  { 0x050C, "ApplicationLauncher", matter_Attributes_050C, matter_Commands_050C },
  { 0x050D, "ApplicationBasic", matter_Attributes_050D, matter_Commands_050D },
  { 0x050E, "AccountLogin", matter_Attributes_050E, matter_Commands_050E },
  { 0x0B04, "ElectricalMeasurement", matter_Attributes_0B04, matter_Commands_0B04 },
  { 0xFFFF, NULL, NULL },
};
