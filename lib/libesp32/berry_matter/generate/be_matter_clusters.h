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
  { 0x0000, 0, 0x03, "IdentifyTime" },
  { 0x0001, 0, 0x02, "IdentifyType" },
  { 0xFFFF, 0, 0x00, NULL },
};

const matter_command_t matter_Commands_0003[] = {
  { 0x0000, "Identify" },
  { 0x0040, "TriggerEffect" },
  { 0xFFFF, NULL },
};

const matter_attribute_t matter_Attributes_0004[] = {
  { 0x0000, 0, 0x02, "NameSupport" },
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
  { 0x0000, 0, 0x02, "SceneCount" },
  { 0x0001, 0, 0x02, "CurrentScene" },
  { 0x0002, 0, 0x02, "CurrentGroup" },
  { 0x0003, 0, 0x02, "SceneValid" },
  { 0x0004, 0, 0x02, "NameSupport" },
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
  { 0x0000, 0, 0x02, "OnOff" },
  { 0x4000, 0, 0x02, "GlobalSceneControl" },
  { 0x4001, 0, 0x03, "OnTime" },
  { 0x4002, 0, 0x03, "OffWaitTime" },
  { 0x4003, 0, 0x03, "StartUpOnOff" },
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
  { 0x0000, 0, 0x02, "SwitchType" },
  { 0x0010, 0, 0x03, "SwitchActions" },
  { 0xFFFF, 0, 0x00, NULL },
};

const matter_command_t matter_Commands_0007[] = {
  { 0xFFFF, NULL },
};

const matter_attribute_t matter_Attributes_0008[] = {
  { 0x0000, 0, 0x02, "CurrentLevel" },
  { 0x0001, 0, 0x02, "RemainingTime" },
  { 0x0002, 0, 0x02, "MinLevel" },
  { 0x0003, 0, 0x02, "MaxLevel" },
  { 0x0004, 0, 0x02, "CurrentFrequency" },
  { 0x0005, 0, 0x02, "MinFrequency" },
  { 0x0006, 0, 0x02, "MaxFrequency" },
  { 0x000F, 0, 0x03, "Options" },
  { 0x0010, 0, 0x03, "OnOffTransitionTime" },
  { 0x0011, 0, 0x03, "OnLevel" },
  { 0x0012, 0, 0x03, "OnTransitionTime" },
  { 0x0013, 0, 0x03, "OffTransitionTime" },
  { 0x0014, 0, 0x03, "DefaultMoveRate" },
  { 0x4000, 0, 0x03, "StartUpCurrentLevel" },
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
  { 0x0051, 0, 0x03, "OutOfService" },
  { 0x0055, 0, 0x03, "PresentValue" },
  { 0x006F, 0, 0x02, "StatusFlags" },
  { 0xFFFF, 0, 0x00, NULL },
};

const matter_command_t matter_Commands_000F[] = {
  { 0xFFFF, NULL },
};

const matter_attribute_t matter_Attributes_001D[] = {
  { 0x0000, 0, 0x02, "DeviceTypeList" },
  { 0x0001, 0, 0x02, "ServerList" },
  { 0x0002, 0, 0x02, "ClientList" },
  { 0x0003, 0, 0x02, "PartsList" },
  { 0x0004, 0, 0x02, "TagList" },
  { 0xFFFF, 0, 0x00, NULL },
};

const matter_command_t matter_Commands_001D[] = {
  { 0xFFFF, NULL },
};

const matter_attribute_t matter_Attributes_001E[] = {
  { 0x0000, 0, 0x03, "Binding" },
  { 0xFFFF, 0, 0x00, NULL },
};

const matter_command_t matter_Commands_001E[] = {
  { 0xFFFF, NULL },
};

const matter_attribute_t matter_Attributes_001F[] = {
  { 0x0000, 0, 0x03, "Acl" },
  { 0x0001, 0, 0x03, "Extension" },
  { 0x0002, 0, 0x02, "SubjectsPerAccessControlEntry" },
  { 0x0003, 0, 0x02, "TargetsPerAccessControlEntry" },
  { 0x0004, 0, 0x02, "AccessControlEntriesPerFabric" },
  { 0xFFFF, 0, 0x00, NULL },
};

const matter_command_t matter_Commands_001F[] = {
  { 0xFFFF, NULL },
};

const matter_attribute_t matter_Attributes_0025[] = {
  { 0x0000, 0, 0x02, "ActionList" },
  { 0x0001, 0, 0x02, "EndpointLists" },
  { 0x0002, 0, 0x02, "SetupURL" },
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
  { 0x0000, 0, 0x02, "DataModelRevision" },
  { 0x0001, 0, 0x02, "VendorName" },
  { 0x0002, 0, 0x02, "VendorID" },
  { 0x0003, 0, 0x02, "ProductName" },
  { 0x0004, 0, 0x02, "ProductID" },
  { 0x0005, 0, 0x03, "NodeLabel" },
  { 0x0006, 0, 0x03, "Location" },
  { 0x0007, 0, 0x02, "HardwareVersion" },
  { 0x0008, 0, 0x02, "HardwareVersionString" },
  { 0x0009, 0, 0x02, "SoftwareVersion" },
  { 0x000A, 0, 0x02, "SoftwareVersionString" },
  { 0x000B, 0, 0x02, "ManufacturingDate" },
  { 0x000C, 0, 0x02, "PartNumber" },
  { 0x000D, 0, 0x02, "ProductURL" },
  { 0x000E, 0, 0x02, "ProductLabel" },
  { 0x000F, 0, 0x02, "SerialNumber" },
  { 0x0010, 0, 0x03, "LocalConfigDisabled" },
  { 0x0011, 0, 0x02, "Reachable" },
  { 0x0012, 0, 0x02, "UniqueID" },
  { 0x0013, 0, 0x02, "CapabilityMinima" },
  { 0xFFFF, 0, 0x00, NULL },
};

const matter_command_t matter_Commands_0028[] = {
  { 0xFFFF, NULL },
};

const matter_attribute_t matter_Attributes_0029[] = {
  { 0xFFFB, 0, 0x02, "AttributeList" },
  { 0xFFFC, 0, 0x02, "FeatureMap" },
  { 0xFFFD, 0, 0x02, "ClusterRevision" },
  { 0xFFFF, 0, 0x00, NULL },
};

const matter_command_t matter_Commands_0029[] = {
  { 0x0000, "QueryImage" },
  { 0x0002, "ApplyUpdateRequest" },
  { 0x0004, "NotifyUpdateApplied" },
  { 0xFFFF, NULL },
};

const matter_attribute_t matter_Attributes_002A[] = {
  { 0x0000, 0, 0x03, "DefaultOtaProviders" },
  { 0x0001, 0, 0x02, "UpdatePossible" },
  { 0x0002, 0, 0x02, "UpdateState" },
  { 0x0003, 0, 0x02, "UpdateStateProgress" },
  { 0xFFFB, 0, 0x02, "AttributeList" },
  { 0xFFFC, 0, 0x02, "FeatureMap" },
  { 0xFFFD, 0, 0x02, "ClusterRevision" },
  { 0xFFFF, 0, 0x00, NULL },
};

const matter_command_t matter_Commands_002A[] = {
  { 0x0000, "AnnounceOtaProvider" },
  { 0xFFFF, NULL },
};

const matter_attribute_t matter_Attributes_002B[] = {
  { 0x0000, 0, 0x03, "ActiveLocale" },
  { 0x0001, 0, 0x02, "SupportedLocales" },
  { 0xFFFF, 0, 0x00, NULL },
};

const matter_command_t matter_Commands_002B[] = {
  { 0xFFFF, NULL },
};

const matter_attribute_t matter_Attributes_002C[] = {
  { 0x0000, 0, 0x03, "HourFormat" },
  { 0x0001, 0, 0x03, "ActiveCalendarType" },
  { 0x0002, 0, 0x02, "SupportedCalendarTypes" },
  { 0xFFFF, 0, 0x00, NULL },
};

const matter_command_t matter_Commands_002C[] = {
  { 0xFFFF, NULL },
};

const matter_attribute_t matter_Attributes_002D[] = {
  { 0x0000, 0, 0x03, "TemperatureUnit" },
  { 0xFFFB, 0, 0x02, "AttributeList" },
  { 0xFFFC, 0, 0x02, "FeatureMap" },
  { 0xFFFD, 0, 0x02, "ClusterRevision" },
  { 0xFFFF, 0, 0x00, NULL },
};

const matter_command_t matter_Commands_002D[] = {
  { 0xFFFF, NULL },
};

const matter_attribute_t matter_Attributes_002E[] = {
  { 0x0000, 0, 0x02, "Sources" },
  { 0xFFFF, 0, 0x00, NULL },
};

const matter_command_t matter_Commands_002E[] = {
  { 0xFFFF, NULL },
};

const matter_attribute_t matter_Attributes_002F[] = {
  { 0x0000, 0, 0x02, "Status" },
  { 0x0001, 0, 0x02, "Order" },
  { 0x0002, 0, 0x02, "Description" },
  { 0x0003, 0, 0x02, "WiredAssessedInputVoltage" },
  { 0x0004, 0, 0x02, "WiredAssessedInputFrequency" },
  { 0x0005, 0, 0x02, "WiredCurrentType" },
  { 0x0006, 0, 0x02, "WiredAssessedCurrent" },
  { 0x0007, 0, 0x02, "WiredNominalVoltage" },
  { 0x0008, 0, 0x02, "WiredMaximumCurrent" },
  { 0x0009, 0, 0x02, "WiredPresent" },
  { 0x000A, 0, 0x02, "ActiveWiredFaults" },
  { 0x000B, 0, 0x02, "BatVoltage" },
  { 0x000C, 0, 0x02, "BatPercentRemaining" },
  { 0x000D, 0, 0x02, "BatTimeRemaining" },
  { 0x000E, 0, 0x02, "BatChargeLevel" },
  { 0x000F, 0, 0x02, "BatReplacementNeeded" },
  { 0x0010, 0, 0x02, "BatReplaceability" },
  { 0x0011, 0, 0x02, "BatPresent" },
  { 0x0012, 0, 0x02, "ActiveBatFaults" },
  { 0x0013, 0, 0x02, "BatReplacementDescription" },
  { 0x0014, 0, 0x02, "BatCommonDesignation" },
  { 0x0015, 0, 0x02, "BatANSIDesignation" },
  { 0x0016, 0, 0x02, "BatIECDesignation" },
  { 0x0017, 0, 0x02, "BatApprovedChemistry" },
  { 0x0018, 0, 0x02, "BatCapacity" },
  { 0x0019, 0, 0x02, "BatQuantity" },
  { 0x001A, 0, 0x02, "BatChargeState" },
  { 0x001B, 0, 0x02, "BatTimeToFullCharge" },
  { 0x001C, 0, 0x02, "BatFunctionalWhileCharging" },
  { 0x001D, 0, 0x02, "BatChargingCurrent" },
  { 0x001E, 0, 0x02, "ActiveBatChargeFaults" },
  { 0xFFFF, 0, 0x00, NULL },
};

const matter_command_t matter_Commands_002F[] = {
  { 0xFFFF, NULL },
};

const matter_attribute_t matter_Attributes_0030[] = {
  { 0x0000, 0, 0x03, "Breadcrumb" },
  { 0x0001, 0, 0x02, "BasicCommissioningInfo" },
  { 0x0002, 0, 0x02, "RegulatoryConfig" },
  { 0x0003, 0, 0x02, "LocationCapability" },
  { 0x0004, 0, 0x02, "SupportsConcurrentConnection" },
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
  { 0x0000, 0, 0x02, "MaxNetworks" },
  { 0x0001, 0, 0x02, "Networks" },
  { 0x0002, 0, 0x02, "ScanMaxTimeSeconds" },
  { 0x0003, 0, 0x02, "ConnectMaxTimeSeconds" },
  { 0x0004, 0, 0x03, "InterfaceEnabled" },
  { 0x0005, 0, 0x02, "LastNetworkingStatus" },
  { 0x0006, 0, 0x02, "LastNetworkID" },
  { 0x0007, 0, 0x02, "LastConnectErrorValue" },
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
  { 0xFFFF, 0, 0x00, NULL },
};

const matter_command_t matter_Commands_0032[] = {
  { 0x0000, "RetrieveLogsRequest" },
  { 0xFFFF, NULL },
};

const matter_attribute_t matter_Attributes_0033[] = {
  { 0x0000, 0, 0x02, "NetworkInterfaces" },
  { 0x0001, 0, 0x02, "RebootCount" },
  { 0x0002, 0, 0x02, "UpTime" },
  { 0x0003, 0, 0x02, "TotalOperationalHours" },
  { 0x0004, 0, 0x02, "BootReasons" },
  { 0x0005, 0, 0x02, "ActiveHardwareFaults" },
  { 0x0006, 0, 0x02, "ActiveRadioFaults" },
  { 0x0007, 0, 0x02, "ActiveNetworkFaults" },
  { 0x0008, 0, 0x02, "TestEventTriggersEnabled" },
  { 0xFFFF, 0, 0x00, NULL },
};

const matter_command_t matter_Commands_0033[] = {
  { 0x0000, "TestEventTrigger" },
  { 0xFFFF, NULL },
};

const matter_attribute_t matter_Attributes_0034[] = {
  { 0x0000, 0, 0x02, "ThreadMetrics" },
  { 0x0001, 0, 0x02, "CurrentHeapFree" },
  { 0x0002, 0, 0x02, "CurrentHeapUsed" },
  { 0x0003, 0, 0x02, "CurrentHeapHighWatermark" },
  { 0xFFFF, 0, 0x00, NULL },
};

const matter_command_t matter_Commands_0034[] = {
  { 0x0000, "ResetWatermarks" },
  { 0xFFFF, NULL },
};

const matter_attribute_t matter_Attributes_0035[] = {
  { 0x0000, 0, 0x02, "Channel" },
  { 0x0001, 0, 0x02, "RoutingRole" },
  { 0x0002, 0, 0x02, "NetworkName" },
  { 0x0003, 0, 0x02, "PanId" },
  { 0x0004, 0, 0x02, "ExtendedPanId" },
  { 0x0005, 0, 0x02, "MeshLocalPrefix" },
  { 0x0006, 0, 0x02, "OverrunCount" },
  { 0x0007, 0, 0x02, "NeighborTableList" },
  { 0x0008, 0, 0x02, "RouteTableList" },
  { 0x0009, 0, 0x02, "PartitionId" },
  { 0x000A, 0, 0x02, "Weighting" },
  { 0x000B, 0, 0x02, "DataVersion" },
  { 0x000C, 0, 0x02, "StableDataVersion" },
  { 0x000D, 0, 0x02, "LeaderRouterId" },
  { 0x000E, 0, 0x02, "DetachedRoleCount" },
  { 0x000F, 0, 0x02, "ChildRoleCount" },
  { 0x0010, 0, 0x02, "RouterRoleCount" },
  { 0x0011, 0, 0x02, "LeaderRoleCount" },
  { 0x0012, 0, 0x02, "AttachAttemptCount" },
  { 0x0013, 0, 0x02, "PartitionIdChangeCount" },
  { 0x0014, 0, 0x02, "BetterPartitionAttachAttemptCount" },
  { 0x0015, 0, 0x02, "ParentChangeCount" },
  { 0x0016, 0, 0x02, "TxTotalCount" },
  { 0x0017, 0, 0x02, "TxUnicastCount" },
  { 0x0018, 0, 0x02, "TxBroadcastCount" },
  { 0x0019, 0, 0x02, "TxAckRequestedCount" },
  { 0x001A, 0, 0x02, "TxAckedCount" },
  { 0x001B, 0, 0x02, "TxNoAckRequestedCount" },
  { 0x001C, 0, 0x02, "TxDataCount" },
  { 0x001D, 0, 0x02, "TxDataPollCount" },
  { 0x001E, 0, 0x02, "TxBeaconCount" },
  { 0x001F, 0, 0x02, "TxBeaconRequestCount" },
  { 0x0020, 0, 0x02, "TxOtherCount" },
  { 0x0021, 0, 0x02, "TxRetryCount" },
  { 0x0022, 0, 0x02, "TxDirectMaxRetryExpiryCount" },
  { 0x0023, 0, 0x02, "TxIndirectMaxRetryExpiryCount" },
  { 0x0024, 0, 0x02, "TxErrCcaCount" },
  { 0x0025, 0, 0x02, "TxErrAbortCount" },
  { 0x0026, 0, 0x02, "TxErrBusyChannelCount" },
  { 0x0027, 0, 0x02, "RxTotalCount" },
  { 0x0028, 0, 0x02, "RxUnicastCount" },
  { 0x0029, 0, 0x02, "RxBroadcastCount" },
  { 0x002A, 0, 0x02, "RxDataCount" },
  { 0x002B, 0, 0x02, "RxDataPollCount" },
  { 0x002C, 0, 0x02, "RxBeaconCount" },
  { 0x002D, 0, 0x02, "RxBeaconRequestCount" },
  { 0x002E, 0, 0x02, "RxOtherCount" },
  { 0x002F, 0, 0x02, "RxAddressFilteredCount" },
  { 0x0030, 0, 0x02, "RxDestAddrFilteredCount" },
  { 0x0031, 0, 0x02, "RxDuplicatedCount" },
  { 0x0032, 0, 0x02, "RxErrNoFrameCount" },
  { 0x0033, 0, 0x02, "RxErrUnknownNeighborCount" },
  { 0x0034, 0, 0x02, "RxErrInvalidSrcAddrCount" },
  { 0x0035, 0, 0x02, "RxErrSecCount" },
  { 0x0036, 0, 0x02, "RxErrFcsCount" },
  { 0x0037, 0, 0x02, "RxErrOtherCount" },
  { 0x0038, 0, 0x02, "ActiveTimestamp" },
  { 0x0039, 0, 0x02, "PendingTimestamp" },
  { 0x003A, 0, 0x02, "Delay" },
  { 0x003B, 0, 0x02, "SecurityPolicy" },
  { 0x003C, 0, 0x02, "ChannelPage0Mask" },
  { 0x003D, 0, 0x02, "OperationalDatasetComponents" },
  { 0x003E, 0, 0x02, "ActiveNetworkFaultsList" },
  { 0xFFFF, 0, 0x00, NULL },
};

const matter_command_t matter_Commands_0035[] = {
  { 0x0000, "ResetCounts" },
  { 0xFFFF, NULL },
};

const matter_attribute_t matter_Attributes_0036[] = {
  { 0x0000, 0, 0x02, "Bssid" },
  { 0x0001, 0, 0x02, "SecurityType" },
  { 0x0002, 0, 0x02, "WiFiVersion" },
  { 0x0003, 0, 0x02, "ChannelNumber" },
  { 0x0004, 0, 0x02, "Rssi" },
  { 0x0005, 0, 0x02, "BeaconLostCount" },
  { 0x0006, 0, 0x02, "BeaconRxCount" },
  { 0x0007, 0, 0x02, "PacketMulticastRxCount" },
  { 0x0008, 0, 0x02, "PacketMulticastTxCount" },
  { 0x0009, 0, 0x02, "PacketUnicastRxCount" },
  { 0x000A, 0, 0x02, "PacketUnicastTxCount" },
  { 0x000B, 0, 0x02, "CurrentMaxRate" },
  { 0x000C, 0, 0x02, "OverrunCount" },
  { 0xFFFF, 0, 0x00, NULL },
};

const matter_command_t matter_Commands_0036[] = {
  { 0x0000, "ResetCounts" },
  { 0xFFFF, NULL },
};

const matter_attribute_t matter_Attributes_0037[] = {
  { 0x0000, 0, 0x02, "PHYRate" },
  { 0x0001, 0, 0x02, "FullDuplex" },
  { 0x0002, 0, 0x02, "PacketRxCount" },
  { 0x0003, 0, 0x02, "PacketTxCount" },
  { 0x0004, 0, 0x02, "TxErrCount" },
  { 0x0005, 0, 0x02, "CollisionCount" },
  { 0x0006, 0, 0x02, "OverrunCount" },
  { 0x0007, 0, 0x02, "CarrierDetect" },
  { 0x0008, 0, 0x02, "TimeSinceReset" },
  { 0xFFFF, 0, 0x00, NULL },
};

const matter_command_t matter_Commands_0037[] = {
  { 0x0000, "ResetCounts" },
  { 0xFFFF, NULL },
};

const matter_attribute_t matter_Attributes_0038[] = {
  { 0x0000, 0, 0x02, "UTCTime" },
  { 0x0001, 0, 0x02, "Granularity" },
  { 0x0002, 0, 0x02, "TimeSource" },
  { 0x0003, 0, 0x02, "TrustedTimeSource" },
  { 0x0004, 0, 0x02, "DefaultNTP" },
  { 0x0005, 0, 0x02, "TimeZone" },
  { 0x0006, 0, 0x02, "DSTOffset" },
  { 0x0007, 0, 0x02, "LocalTime" },
  { 0x0008, 0, 0x02, "TimeZoneDatabase" },
  { 0x0009, 0, 0x02, "NTPServerAvailable" },
  { 0x000A, 0, 0x02, "TimeZoneListMaxSize" },
  { 0x000B, 0, 0x02, "DSTOffsetListMaxSize" },
  { 0x000C, 0, 0x02, "SupportsDNSResolve" },
  { 0xFFFF, 0, 0x00, NULL },
};

const matter_command_t matter_Commands_0038[] = {
  { 0x0000, "SetUTCTime" },
  { 0x0001, "SetTrustedTimeSource" },
  { 0x0002, "SetTimeZone" },
  { 0x0003, "SetTimzZoneResponse" },
  { 0x0004, "SetDSTOffset" },
  { 0x0005, "SetDefaultNTP" },
  { 0xFFFF, NULL },
};

const matter_attribute_t matter_Attributes_0039[] = {
  { 0x0001, 0, 0x02, "VendorName" },
  { 0x0002, 0, 0x02, "VendorID" },
  { 0x0003, 0, 0x02, "ProductName" },
  { 0x0005, 0, 0x03, "NodeLabel" },
  { 0x0007, 0, 0x02, "HardwareVersion" },
  { 0x0008, 0, 0x02, "HardwareVersionString" },
  { 0x0009, 0, 0x02, "SoftwareVersion" },
  { 0x000A, 0, 0x02, "SoftwareVersionString" },
  { 0x000B, 0, 0x02, "ManufacturingDate" },
  { 0x000C, 0, 0x02, "PartNumber" },
  { 0x000D, 0, 0x02, "ProductURL" },
  { 0x000E, 0, 0x02, "ProductLabel" },
  { 0x000F, 0, 0x02, "SerialNumber" },
  { 0x0011, 0, 0x02, "Reachable" },
  { 0x0012, 0, 0x02, "UniqueID" },
  { 0xFFFF, 0, 0x00, NULL },
};

const matter_command_t matter_Commands_0039[] = {
  { 0xFFFF, NULL },
};

const matter_attribute_t matter_Attributes_003B[] = {
  { 0x0000, 0, 0x02, "NumberOfPositions" },
  { 0x0001, 0, 0x02, "CurrentPosition" },
  { 0x0002, 0, 0x02, "MultiPressMax" },
  { 0xFFFF, 0, 0x00, NULL },
};

const matter_command_t matter_Commands_003B[] = {
  { 0xFFFF, NULL },
};

const matter_attribute_t matter_Attributes_003C[] = {
  { 0x0000, 0, 0x02, "WindowStatus" },
  { 0x0001, 0, 0x02, "AdminFabricIndex" },
  { 0x0002, 0, 0x02, "AdminVendorId" },
  { 0xFFFF, 0, 0x00, NULL },
};

const matter_command_t matter_Commands_003C[] = {
  { 0x0000, "OpenCommissioningWindow" },
  { 0x0001, "OpenBasicCommissioningWindow" },
  { 0x0002, "RevokeCommissioning" },
  { 0xFFFF, NULL },
};

const matter_attribute_t matter_Attributes_003E[] = {
  { 0x0000, 0, 0x02, "NOCs" },
  { 0x0001, 0, 0x02, "Fabrics" },
  { 0x0002, 0, 0x02, "SupportedFabrics" },
  { 0x0003, 0, 0x02, "CommissionedFabrics" },
  { 0x0004, 0, 0x02, "TrustedRootCertificates" },
  { 0x0005, 0, 0x02, "CurrentFabricIndex" },
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
  { 0x0000, 0, 0x03, "GroupKeyMap" },
  { 0x0001, 0, 0x02, "GroupTable" },
  { 0x0002, 0, 0x02, "MaxGroupsPerFabric" },
  { 0x0003, 0, 0x02, "MaxGroupKeysPerFabric" },
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
  { 0x0000, 0, 0x02, "LabelList" },
  { 0xFFFF, 0, 0x00, NULL },
};

const matter_command_t matter_Commands_0040[] = {
  { 0xFFFF, NULL },
};

const matter_attribute_t matter_Attributes_0041[] = {
  { 0x0000, 0, 0x03, "LabelList" },
  { 0xFFFF, 0, 0x00, NULL },
};

const matter_command_t matter_Commands_0041[] = {
  { 0xFFFF, NULL },
};

const matter_attribute_t matter_Attributes_0045[] = {
  { 0x0000, 0, 0x02, "StateValue" },
  { 0xFFFF, 0, 0x00, NULL },
};

const matter_command_t matter_Commands_0045[] = {
  { 0xFFFF, NULL },
};

const matter_attribute_t matter_Attributes_0050[] = {
  { 0x0000, 0, 0x02, "Description" },
  { 0x0001, 0, 0x02, "StandardNamespace" },
  { 0x0002, 0, 0x02, "SupportedModes" },
  { 0x0003, 0, 0x02, "CurrentMode" },
  { 0x0004, 0, 0x03, "StartUpMode" },
  { 0x0005, 0, 0x03, "OnMode" },
  { 0xFFFF, 0, 0x00, NULL },
};

const matter_command_t matter_Commands_0050[] = {
  { 0x0000, "ChangeToMode" },
  { 0xFFFF, NULL },
};

const matter_attribute_t matter_Attributes_0101[] = {
  { 0x0000, 0, 0x02, "LockState" },
  { 0x0001, 0, 0x02, "LockType" },
  { 0x0002, 0, 0x02, "ActuatorEnabled" },
  { 0x0003, 0, 0x02, "DoorState" },
  { 0x0011, 0, 0x02, "NumberOfTotalUsersSupported" },
  { 0x0012, 0, 0x02, "NumberOfPINUsersSupported" },
  { 0x0013, 0, 0x02, "NumberOfRFIDUsersSupported" },
  { 0x0014, 0, 0x02, "NumberOfWeekDaySchedulesSupportedPerUser" },
  { 0x0015, 0, 0x02, "NumberOfYearDaySchedulesSupportedPerUser" },
  { 0x0016, 0, 0x02, "NumberOfHolidaySchedulesSupported" },
  { 0x0017, 0, 0x02, "MaxPINCodeLength" },
  { 0x0018, 0, 0x02, "MinPINCodeLength" },
  { 0x0019, 0, 0x02, "MaxRFIDCodeLength" },
  { 0x001A, 0, 0x02, "MinRFIDCodeLength" },
  { 0x001C, 0, 0x02, "NumberOfCredentialsSupportedPerUser" },
  { 0x0021, 0, 0x03, "Language" },
  { 0x0023, 0, 0x03, "AutoRelockTime" },
  { 0x0024, 0, 0x03, "SoundVolume" },
  { 0x0025, 0, 0x03, "OperatingMode" },
  { 0x0026, 0, 0x02, "SupportedOperatingModes" },
  { 0x0029, 0, 0x03, "EnableOneTouchLocking" },
  { 0x002B, 0, 0x03, "EnablePrivacyModeButton" },
  { 0x0030, 0, 0x03, "WrongCodeEntryLimit" },
  { 0x0031, 0, 0x03, "UserCodeTemporaryDisableTime" },
  { 0x0033, 0, 0x01, "RequirePINforRemoteOperation" },
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
  { 0x0000, 0, 0x02, "Type" },
  { 0x0001, 0, 0x02, "PhysicalClosedLimitLift" },
  { 0x0002, 0, 0x02, "PhysicalClosedLimitTilt" },
  { 0x0003, 0, 0x02, "CurrentPositionLift" },
  { 0x0004, 0, 0x02, "CurrentPositionTilt" },
  { 0x0005, 0, 0x02, "NumberOfActuationsLift" },
  { 0x0006, 0, 0x02, "NumberOfActuationsTilt" },
  { 0x0007, 0, 0x02, "ConfigStatus" },
  { 0x0008, 0, 0x02, "CurrentPositionLiftPercentage" },
  { 0x0009, 0, 0x02, "CurrentPositionTiltPercentage" },
  { 0x000A, 0, 0x02, "OperationalStatus" },
  { 0x000B, 0, 0x02, "TargetPositionLiftPercent100ths" },
  { 0x000C, 0, 0x02, "TargetPositionTiltPercent100ths" },
  { 0x000D, 0, 0x02, "EndProductType" },
  { 0x000E, 0, 0x02, "CurrentPositionLiftPercent100ths" },
  { 0x000F, 0, 0x02, "CurrentPositionTiltPercent100ths" },
  { 0x0010, 0, 0x02, "InstalledOpenLimitLift" },
  { 0x0011, 0, 0x02, "InstalledClosedLimitLift" },
  { 0x0012, 0, 0x02, "InstalledOpenLimitTilt" },
  { 0x0013, 0, 0x02, "InstalledClosedLimitTilt" },
  { 0x0017, 0, 0x03, "Mode" },
  { 0x001A, 0, 0x02, "SafetyStatus" },
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
  { 0x0001, 0, 0x02, "BarrierMovingState" },
  { 0x0002, 0, 0x02, "BarrierSafetyStatus" },
  { 0x0003, 0, 0x02, "BarrierCapabilities" },
  { 0x000A, 0, 0x02, "BarrierPosition" },
  { 0xFFFF, 0, 0x00, NULL },
};

const matter_command_t matter_Commands_0103[] = {
  { 0x0000, "BarrierControlGoToPercent" },
  { 0x0001, "BarrierControlStop" },
  { 0xFFFF, NULL },
};

const matter_attribute_t matter_Attributes_0200[] = {
  { 0x0000, 0, 0x02, "MaxPressure" },
  { 0x0001, 0, 0x02, "MaxSpeed" },
  { 0x0002, 0, 0x02, "MaxFlow" },
  { 0x0003, 0, 0x02, "MinConstPressure" },
  { 0x0004, 0, 0x02, "MaxConstPressure" },
  { 0x0005, 0, 0x02, "MinCompPressure" },
  { 0x0006, 0, 0x02, "MaxCompPressure" },
  { 0x0007, 0, 0x02, "MinConstSpeed" },
  { 0x0008, 0, 0x02, "MaxConstSpeed" },
  { 0x0009, 0, 0x02, "MinConstFlow" },
  { 0x000A, 0, 0x02, "MaxConstFlow" },
  { 0x000B, 0, 0x02, "MinConstTemp" },
  { 0x000C, 0, 0x02, "MaxConstTemp" },
  { 0x0010, 0, 0x02, "PumpStatus" },
  { 0x0011, 0, 0x02, "EffectiveOperationMode" },
  { 0x0012, 0, 0x02, "EffectiveControlMode" },
  { 0x0013, 0, 0x02, "Capacity" },
  { 0x0014, 0, 0x02, "Speed" },
  { 0x0015, 0, 0x03, "LifetimeRunningHours" },
  { 0x0016, 0, 0x02, "Power" },
  { 0x0017, 0, 0x03, "LifetimeEnergyConsumed" },
  { 0x0020, 0, 0x03, "OperationMode" },
  { 0x0021, 0, 0x03, "ControlMode" },
  { 0xFFFF, 0, 0x00, NULL },
};

const matter_command_t matter_Commands_0200[] = {
  { 0xFFFF, NULL },
};

const matter_attribute_t matter_Attributes_0201[] = {
  { 0x0000, 0, 0x02, "LocalTemperature" },
  { 0x0003, 0, 0x02, "AbsMinHeatSetpointLimit" },
  { 0x0004, 0, 0x02, "AbsMaxHeatSetpointLimit" },
  { 0x0005, 0, 0x02, "AbsMinCoolSetpointLimit" },
  { 0x0006, 0, 0x02, "AbsMaxCoolSetpointLimit" },
  { 0x0011, 0, 0x03, "OccupiedCoolingSetpoint" },
  { 0x0012, 0, 0x03, "OccupiedHeatingSetpoint" },
  { 0x0015, 0, 0x03, "MinHeatSetpointLimit" },
  { 0x0016, 0, 0x03, "MaxHeatSetpointLimit" },
  { 0x0017, 0, 0x03, "MinCoolSetpointLimit" },
  { 0x0018, 0, 0x03, "MaxCoolSetpointLimit" },
  { 0x0019, 0, 0x03, "MinSetpointDeadBand" },
  { 0x001B, 0, 0x03, "ControlSequenceOfOperation" },
  { 0x001C, 0, 0x03, "SystemMode" },
  { 0x0020, 0, 0x02, "StartOfWeek" },
  { 0x0021, 0, 0x02, "NumberOfWeeklyTransitions" },
  { 0x0022, 0, 0x02, "NumberOfDailyTransitions" },
  { 0xFFFB, 0, 0x02, "AttributeList" },
  { 0xFFFC, 0, 0x02, "FeatureMap" },
  { 0xFFFD, 0, 0x02, "ClusterRevision" },
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
  { 0x0000, 0, 0x03, "FanMode" },
  { 0x0001, 0, 0x03, "FanModeSequence" },
  { 0x0002, 0, 0x03, "PercentSetting" },
  { 0x0003, 0, 0x02, "PercentCurrent" },
  { 0x0004, 0, 0x02, "SpeedMax" },
  { 0x0005, 0, 0x03, "SpeedSetting" },
  { 0x0006, 0, 0x02, "SpeedCurrent" },
  { 0x0007, 0, 0x02, "RockSupport" },
  { 0x0008, 0, 0x03, "RockSetting" },
  { 0x0009, 0, 0x02, "WindSupport" },
  { 0x000A, 0, 0x03, "WindSetting" },
  { 0xFFFF, 0, 0x00, NULL },
};

const matter_command_t matter_Commands_0202[] = {
  { 0xFFFF, NULL },
};

const matter_attribute_t matter_Attributes_0204[] = {
  { 0x0000, 0, 0x03, "TemperatureDisplayMode" },
  { 0x0001, 0, 0x03, "KeypadLockout" },
  { 0x0002, 0, 0x03, "ScheduleProgrammingVisibility" },
  { 0xFFFF, 0, 0x00, NULL },
};

const matter_command_t matter_Commands_0204[] = {
  { 0xFFFF, NULL },
};

const matter_attribute_t matter_Attributes_0300[] = {
  { 0x0000, 0, 0x02, "CurrentHue" },
  { 0x0001, 0, 0x02, "CurrentSaturation" },
  { 0x0002, 0, 0x02, "RemainingTime" },
  { 0x0003, 0, 0x02, "CurrentX" },
  { 0x0004, 0, 0x02, "CurrentY" },
  { 0x0005, 0, 0x02, "DriftCompensation" },
  { 0x0006, 0, 0x02, "CompensationText" },
  { 0x0007, 0, 0x02, "ColorTemperatureMireds" },
  { 0x0008, 0, 0x02, "ColorMode" },
  { 0x000F, 0, 0x03, "Options" },
  { 0x0010, 0, 0x02, "NumberOfPrimaries" },
  { 0x0011, 0, 0x02, "Primary1X" },
  { 0x0012, 0, 0x02, "Primary1Y" },
  { 0x0013, 0, 0x02, "Primary1Intensity" },
  { 0x0015, 0, 0x02, "Primary2X" },
  { 0x0016, 0, 0x02, "Primary2Y" },
  { 0x0017, 0, 0x02, "Primary2Intensity" },
  { 0x0019, 0, 0x02, "Primary3X" },
  { 0x001A, 0, 0x02, "Primary3Y" },
  { 0x001B, 0, 0x02, "Primary3Intensity" },
  { 0x0020, 0, 0x02, "Primary4X" },
  { 0x0021, 0, 0x02, "Primary4Y" },
  { 0x0022, 0, 0x02, "Primary4Intensity" },
  { 0x0024, 0, 0x02, "Primary5X" },
  { 0x0025, 0, 0x02, "Primary5Y" },
  { 0x0026, 0, 0x02, "Primary5Intensity" },
  { 0x0028, 0, 0x02, "Primary6X" },
  { 0x0029, 0, 0x02, "Primary6Y" },
  { 0x002A, 0, 0x02, "Primary6Intensity" },
  { 0x0030, 0, 0x03, "WhitePointX" },
  { 0x0031, 0, 0x03, "WhitePointY" },
  { 0x0032, 0, 0x03, "ColorPointRX" },
  { 0x0033, 0, 0x03, "ColorPointRY" },
  { 0x0034, 0, 0x03, "ColorPointRIntensity" },
  { 0x0036, 0, 0x03, "ColorPointGX" },
  { 0x0037, 0, 0x03, "ColorPointGY" },
  { 0x0038, 0, 0x03, "ColorPointGIntensity" },
  { 0x003A, 0, 0x03, "ColorPointBX" },
  { 0x003B, 0, 0x03, "ColorPointBY" },
  { 0x003C, 0, 0x03, "ColorPointBIntensity" },
  { 0x4000, 0, 0x02, "EnhancedCurrentHue" },
  { 0x4001, 0, 0x02, "EnhancedColorMode" },
  { 0x4002, 0, 0x02, "ColorLoopActive" },
  { 0x4003, 0, 0x02, "ColorLoopDirection" },
  { 0x4004, 0, 0x02, "ColorLoopTime" },
  { 0x4005, 0, 0x02, "ColorLoopStartEnhancedHue" },
  { 0x4006, 0, 0x02, "ColorLoopStoredEnhancedHue" },
  { 0x400A, 0, 0x02, "ColorCapabilities" },
  { 0x400B, 0, 0x02, "ColorTempPhysicalMinMireds" },
  { 0x400C, 0, 0x02, "ColorTempPhysicalMaxMireds" },
  { 0x400D, 0, 0x02, "CoupleColorTempToLevelMinMireds" },
  { 0x4010, 0, 0x03, "StartUpColorTemperatureMireds" },
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
  { 0x0000, 0, 0x02, "PhysicalMinLevel" },
  { 0x0001, 0, 0x02, "PhysicalMaxLevel" },
  { 0x0002, 0, 0x02, "BallastStatus" },
  { 0x0010, 0, 0x03, "MinLevel" },
  { 0x0011, 0, 0x03, "MaxLevel" },
  { 0x0014, 0, 0x03, "IntrinsicBalanceFactor" },
  { 0x0015, 0, 0x03, "BallastFactorAdjustment" },
  { 0x0020, 0, 0x02, "LampQuantity" },
  { 0x0030, 0, 0x03, "LampType" },
  { 0x0031, 0, 0x03, "LampManufacturer" },
  { 0x0032, 0, 0x03, "LampRatedHours" },
  { 0x0033, 0, 0x03, "LampBurnHours" },
  { 0x0034, 0, 0x03, "LampAlarmMode" },
  { 0x0035, 0, 0x03, "LampBurnHoursTripPoint" },
  { 0xFFFF, 0, 0x00, NULL },
};

const matter_command_t matter_Commands_0301[] = {
  { 0xFFFF, NULL },
};

const matter_attribute_t matter_Attributes_0400[] = {
  { 0x0000, 0, 0x02, "MeasuredValue" },
  { 0x0001, 0, 0x02, "MinMeasuredValue" },
  { 0x0002, 0, 0x02, "MaxMeasuredValue" },
  { 0x0003, 0, 0x02, "Tolerance" },
  { 0x0004, 0, 0x02, "LightSensorType" },
  { 0xFFFF, 0, 0x00, NULL },
};

const matter_command_t matter_Commands_0400[] = {
  { 0xFFFF, NULL },
};

const matter_attribute_t matter_Attributes_0402[] = {
  { 0x0000, 0, 0x02, "MeasuredValue" },
  { 0x0001, 0, 0x02, "MinMeasuredValue" },
  { 0x0002, 0, 0x02, "MaxMeasuredValue" },
  { 0x0003, 0, 0x02, "Tolerance" },
  { 0xFFFB, 0, 0x02, "AttributeList" },
  { 0xFFFC, 0, 0x02, "FeatureMap" },
  { 0xFFFD, 0, 0x02, "ClusterRevision" },
  { 0xFFFF, 0, 0x00, NULL },
};

const matter_command_t matter_Commands_0402[] = {
  { 0xFFFF, NULL },
};

const matter_attribute_t matter_Attributes_0403[] = {
  { 0x0000, 0, 0x02, "MeasuredValue" },
  { 0x0001, 0, 0x02, "MinMeasuredValue" },
  { 0x0002, 0, 0x02, "MaxMeasuredValue" },
  { 0x0003, 0, 0x02, "Tolerance" },
  { 0x0010, 0, 0x02, "ScaledValue" },
  { 0x0011, 0, 0x02, "MinScaledValue" },
  { 0x0012, 0, 0x02, "MaxScaledValue" },
  { 0x0013, 0, 0x02, "ScaledTolerance" },
  { 0x0014, 0, 0x02, "Scale" },
  { 0xFFFB, 0, 0x02, "AttributeList" },
  { 0xFFFC, 0, 0x02, "FeatureMap" },
  { 0xFFFD, 0, 0x02, "ClusterRevision" },
  { 0xFFFF, 0, 0x00, NULL },
};

const matter_command_t matter_Commands_0403[] = {
  { 0xFFFF, NULL },
};

const matter_attribute_t matter_Attributes_0404[] = {
  { 0x0000, 0, 0x02, "MeasuredValue" },
  { 0x0001, 0, 0x02, "MinMeasuredValue" },
  { 0x0002, 0, 0x02, "MaxMeasuredValue" },
  { 0x0003, 0, 0x02, "Tolerance" },
  { 0xFFFF, 0, 0x00, NULL },
};

const matter_command_t matter_Commands_0404[] = {
  { 0xFFFF, NULL },
};

const matter_attribute_t matter_Attributes_0405[] = {
  { 0x0000, 0, 0x02, "MeasuredValue" },
  { 0x0001, 0, 0x02, "MinMeasuredValue" },
  { 0x0002, 0, 0x02, "MaxMeasuredValue" },
  { 0x0003, 0, 0x02, "Tolerance" },
  { 0xFFFF, 0, 0x00, NULL },
};

const matter_command_t matter_Commands_0405[] = {
  { 0xFFFF, NULL },
};

const matter_attribute_t matter_Attributes_0406[] = {
  { 0x0000, 0, 0x02, "Occupancy" },
  { 0x0001, 0, 0x02, "OccupancySensorType" },
  { 0x0002, 0, 0x02, "OccupancySensorTypeBitmap" },
  { 0xFFFF, 0, 0x00, NULL },
};

const matter_command_t matter_Commands_0406[] = {
  { 0xFFFF, NULL },
};

const matter_attribute_t matter_Attributes_0503[] = {
  { 0x0000, 0, 0x02, "MACAddress" },
  { 0xFFFF, 0, 0x00, NULL },
};

const matter_command_t matter_Commands_0503[] = {
  { 0xFFFF, NULL },
};

const matter_attribute_t matter_Attributes_0504[] = {
  { 0x0000, 0, 0x02, "ChannelList" },
  { 0x0001, 0, 0x02, "Lineup" },
  { 0x0002, 0, 0x02, "CurrentChannel" },
  { 0xFFFF, 0, 0x00, NULL },
};

const matter_command_t matter_Commands_0504[] = {
  { 0x0000, "ChangeChannel" },
  { 0x0002, "ChangeChannelByNumber" },
  { 0x0003, "SkipChannel" },
  { 0xFFFF, NULL },
};

const matter_attribute_t matter_Attributes_0505[] = {
  { 0x0000, 0, 0x02, "TargetList" },
  { 0x0001, 0, 0x02, "CurrentTarget" },
  { 0xFFFF, 0, 0x00, NULL },
};

const matter_command_t matter_Commands_0505[] = {
  { 0x0000, "NavigateTarget" },
  { 0xFFFF, NULL },
};

const matter_attribute_t matter_Attributes_0506[] = {
  { 0x0000, 0, 0x02, "CurrentState" },
  { 0x0001, 0, 0x02, "StartTime" },
  { 0x0002, 0, 0x02, "Duration" },
  { 0x0003, 0, 0x02, "SampledPosition" },
  { 0x0004, 0, 0x02, "PlaybackSpeed" },
  { 0x0005, 0, 0x02, "SeekRangeEnd" },
  { 0x0006, 0, 0x02, "SeekRangeStart" },
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
  { 0x0000, 0, 0x02, "InputList" },
  { 0x0001, 0, 0x02, "CurrentInput" },
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
  { 0xFFFF, 0, 0x00, NULL },
};

const matter_command_t matter_Commands_0508[] = {
  { 0x0000, "Sleep" },
  { 0xFFFF, NULL },
};

const matter_attribute_t matter_Attributes_0509[] = {
  { 0xFFFF, 0, 0x00, NULL },
};

const matter_command_t matter_Commands_0509[] = {
  { 0x0000, "SendKey" },
  { 0xFFFF, NULL },
};

const matter_attribute_t matter_Attributes_050A[] = {
  { 0x0000, 0, 0x02, "AcceptHeader" },
  { 0x0001, 0, 0x03, "SupportedStreamingProtocols" },
  { 0xFFFF, 0, 0x00, NULL },
};

const matter_command_t matter_Commands_050A[] = {
  { 0x0000, "LaunchContent" },
  { 0x0001, "LaunchURL" },
  { 0xFFFF, NULL },
};

const matter_attribute_t matter_Attributes_050B[] = {
  { 0x0000, 0, 0x02, "OutputList" },
  { 0x0001, 0, 0x02, "CurrentOutput" },
  { 0xFFFF, 0, 0x00, NULL },
};

const matter_command_t matter_Commands_050B[] = {
  { 0x0000, "SelectOutput" },
  { 0x0001, "RenameOutput" },
  { 0xFFFF, NULL },
};

const matter_attribute_t matter_Attributes_050C[] = {
  { 0x0000, 0, 0x02, "CatalogList" },
  { 0x0001, 0, 0x03, "CurrentApp" },
  { 0xFFFF, 0, 0x00, NULL },
};

const matter_command_t matter_Commands_050C[] = {
  { 0x0000, "LaunchApp" },
  { 0x0001, "StopApp" },
  { 0x0002, "HideApp" },
  { 0xFFFF, NULL },
};

const matter_attribute_t matter_Attributes_050D[] = {
  { 0x0000, 0, 0x02, "VendorName" },
  { 0x0001, 0, 0x02, "VendorID" },
  { 0x0002, 0, 0x02, "ApplicationName" },
  { 0x0003, 0, 0x02, "ProductID" },
  { 0x0004, 0, 0x02, "Application" },
  { 0x0005, 0, 0x02, "Status" },
  { 0x0006, 0, 0x02, "ApplicationVersion" },
  { 0x0007, 0, 0x02, "AllowedVendorList" },
  { 0xFFFF, 0, 0x00, NULL },
};

const matter_command_t matter_Commands_050D[] = {
  { 0xFFFF, NULL },
};

const matter_attribute_t matter_Attributes_050E[] = {
  { 0xFFFF, 0, 0x00, NULL },
};

const matter_command_t matter_Commands_050E[] = {
  { 0x0000, "GetSetupPIN" },
  { 0x0002, "Login" },
  { 0x0003, "Logout" },
  { 0xFFFF, NULL },
};

const matter_attribute_t matter_Attributes_0B04[] = {
  { 0x0000, 0, 0x02, "MeasurementType" },
  { 0x0304, 0, 0x02, "TotalActivePower" },
  { 0x0505, 0, 0x02, "RmsVoltage" },
  { 0x0506, 0, 0x02, "RmsVoltageMin" },
  { 0x0507, 0, 0x02, "RmsVoltageMax" },
  { 0x0508, 0, 0x02, "RmsCurrent" },
  { 0x0509, 0, 0x02, "RmsCurrentMin" },
  { 0x050A, 0, 0x02, "RmsCurrentMax" },
  { 0x050B, 0, 0x02, "ActivePower" },
  { 0x050C, 0, 0x02, "ActivePowerMin" },
  { 0x050D, 0, 0x02, "ActivePowerMax" },
  { 0xFFFF, 0, 0x00, NULL },
};

const matter_command_t matter_Commands_0B04[] = {
  { 0xFFFF, NULL },
};

// specifal cluster for attributes common to all clusters
const matter_attribute_t matter_Attributes_Common[] = {
  { 0xFFF8, 0, 0x02, "GeneratedCommandList" },
  { 0xFFF9, 0, 0x02, "AcceptedCommandList" },
  { 0xFFFA, 0, 0x02, "EventList" },
  { 0xFFFB, 0, 0x02, "AttributeList" },
  { 0xFFFC, 0, 0x02, "FeatureMap" },
  { 0xFFFD, 0, 0x02, "ClusterRevision" },
  { 0xFFFF, 0, 0x00, NULL },
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
  // { 0x0035, "ThreadNetworkDiagnostics", matter_Attributes_0035, matter_Commands_0035 },
  { 0x0036, "WiFiNetworkDiagnostics", matter_Attributes_0036, matter_Commands_0036 },
  { 0x0037, "EthernetNetworkDiagnostics", matter_Attributes_0037, matter_Commands_0037 },
  { 0x0038, "TimeSync", matter_Attributes_0038, matter_Commands_0038 },
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
  // { 0x0103, "BarrierControl", matter_Attributes_0103, matter_Commands_0103 },
  { 0x0200, "PumpConfigurationAndControl", matter_Attributes_0200, matter_Commands_0200 },
  { 0x0201, "Thermostat", matter_Attributes_0201, matter_Commands_0201 },
  { 0x0202, "FanControl", matter_Attributes_0202, matter_Commands_0202 },
  { 0x0204, "ThermostatUserInterfaceConfiguration", matter_Attributes_0204, matter_Commands_0204 },
  { 0x0300, "ColorControl", matter_Attributes_0300, matter_Commands_0300 },
  // { 0x0301, "BallastConfiguration", matter_Attributes_0301, matter_Commands_0301 },
  { 0x0400, "IlluminanceMeasurement", matter_Attributes_0400, matter_Commands_0400 },
  { 0x0402, "TemperatureMeasurement", matter_Attributes_0402, matter_Commands_0402 },
  { 0x0403, "PressureMeasurement", matter_Attributes_0403, matter_Commands_0403 },
  { 0x0404, "FlowMeasurement", matter_Attributes_0404, matter_Commands_0404 },
  { 0x0405, "RelativeHumidityMeasurement", matter_Attributes_0405, matter_Commands_0405 },
  { 0x0406, "OccupancySensing", matter_Attributes_0406, matter_Commands_0406 },
  // { 0x0503, "WakeOnLan", matter_Attributes_0503, matter_Commands_0503 },
  // { 0x0504, "Channel", matter_Attributes_0504, matter_Commands_0504 },
  // { 0x0505, "TargetNavigator", matter_Attributes_0505, matter_Commands_0505 },
  // { 0x0506, "MediaPlayback", matter_Attributes_0506, matter_Commands_0506 },
  // { 0x0507, "MediaInput", matter_Attributes_0507, matter_Commands_0507 },
  // { 0x0508, "LowPower", matter_Attributes_0508, matter_Commands_0508 },
  // { 0x0509, "KeypadInput", matter_Attributes_0509, matter_Commands_0509 },
  // { 0x050A, "ContentLauncher", matter_Attributes_050A, matter_Commands_050A },
  // { 0x050B, "AudioOutput", matter_Attributes_050B, matter_Commands_050B },
  // { 0x050C, "ApplicationLauncher", matter_Attributes_050C, matter_Commands_050C },
  // { 0x050D, "ApplicationBasic", matter_Attributes_050D, matter_Commands_050D },
  // { 0x050E, "AccountLogin", matter_Attributes_050E, matter_Commands_050E },
  // { 0x0B04, "ElectricalMeasurement", matter_Attributes_0B04, matter_Commands_0B04 },
  { 0xFFFF, NULL, NULL },
};
