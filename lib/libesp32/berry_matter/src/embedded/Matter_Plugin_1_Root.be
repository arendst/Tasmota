#
# Matter_Plugin_1_Root.be - implements the core features that a Matter device must implement
#
# Copyright (C) 2023  Stephan Hadinger & Theo Arends
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
#

#################################################################################
# Matter 1.4.1 Root Node Device Specification
#################################################################################
# Device Type: Root Node (0x0016)
# Device Type Revision: 3 (Matter 1.4.1)
# Class: Simple | Scope: Node
#
# The Root Node device type represents the base functionality required by all
# Matter nodes. It resides on Endpoint 0 and provides commissioning, security,
# network management, and diagnostic capabilities.
#
# MANDATORY CLUSTERS (Server):
# - 0x001D: Descriptor (M) - Endpoint composition
# - 0x001F: Access Control (M) - ACL management
# - 0x0028: Basic Information (M) - Device identification
# - 0x0030: General Commissioning (M) - Commissioning workflow
# - 0x0031: Network Commissioning (M) - Network configuration
# - 0x0033: General Diagnostics (M) - System diagnostics
# - 0x003C: Administrator Commissioning (M) - Commissioning window control
# - 0x003E: Node Operational Credentials (M) - Certificate management
# - 0x003F: Group Key Management (M) - Group communication keys
#
# OPTIONAL CLUSTERS (Server):
# - 0x002A: OTA Software Update Requestor (O) - Firmware updates
# - 0x002B: Localization Configuration (O) - Locale settings
# - 0x002C: Time Format Localization (O) - Time/date formatting
# - 0x002D: Unit Localization (O) - Unit preferences (Matter 1.3+)
# - 0x0032: Diagnostic Logs (O) - Log retrieval
# - 0x0034: Software Diagnostics (O) - Software metrics
# - 0x0036: Wi-Fi Network Diagnostics (O) - Wi-Fi statistics
# - 0x0038: Time Synchronization (O) - Time sync
# - 0x0046: ICD Management (O) - Intermittently Connected Device management
#
# MANDATORY CLUSTERS (Client):
# - 0x001F: Access Control (M) - For ACL operations
#
# NOTES:
# - All Matter devices MUST implement Root Node on Endpoint 0
# - Root Node provides the foundation for device commissioning and operation
# - Network Commissioning cluster varies by network type (Wi-Fi/Thread/Ethernet)
# - ICD Management cluster required for battery-powered/sleepy devices
#################################################################################

import matter

# Matter plug-in for root behavior

#@ solidify:Matter_Plugin_Root,weak

class Matter_Plugin_Root : Matter_Plugin
#################################################################################
# Matter 1.4.1 Basic Information Cluster (0x0028)
#################################################################################
# Cluster Revision: 3 (Matter 1.4.1)
# Role: Utility | Scope: Node
#
# Provides device identification, versioning, and capability information.
#
# ATTRIBUTES:
# ID     | Name                    | Type    | Constraint | Quality | Default | Access | Conf
# -------|-------------------------|---------|------------|---------|---------|--------|-----
# 0x0000 | DataModelRevision       | uint16  | all        | F       | 18      | R V    | M
# 0x0001 | VendorName              | string  | max 32     | F       | -       | R V    | M
# 0x0002 | VendorID                | uint16  | all        | F       | -       | R V    | M
# 0x0003 | ProductName             | string  | max 32     | F       | -       | R V    | M
# 0x0004 | ProductID               | uint16  | all        | F       | -       | R V    | O
# 0x0005 | NodeLabel               | string  | max 32     | N       | ""      | RW VM  | M
# 0x0006 | Location                | string  | 2          | N       | "XX"    | RW VA  | M
# 0x0007 | HardwareVersion         | uint16  | all        | F       | 0       | R V    | M
# 0x0008 | HardwareVersionString   | string  | 1-64       | F       | -       | R V    | M
# 0x0009 | SoftwareVersion         | uint32  | all        | F       | -       | R V    | M
# 0x000A | SoftwareVersionString   | string  | 1-64       | F       | -       | R V    | M
# 0x000F | SerialNumber            | string  | max 32     | F       | -       | R V    | O
# 0x0011 | Reachable               | bool    | all        | -       | true    | R V    | O
# 0x0012 | UniqueID                | string  | max 32     | F       | -       | R V    | O
# 0x0013 | CapabilityMinima        | struct  | -          | F       | -       | R V    | M
#
# CapabilityMinima Structure:
#   - CaseSessionsPerFabric (uint16): Min 3
#   - SubscriptionsPerFabric (uint16): Min 3
#
# EVENTS:
# ID   | Name    | Priority | Conf
# -----|---------|----------|-----
# 0x00 | StartUp | CRITICAL | M
#
# NOTES:
# - DataModelRevision: 18 = Matter 1.3+ (compatible with 1.4.1)
# - VendorID: 0xFFF1-0xFFF4 for test vendors
# - NodeLabel: User-friendly name, writable
# - Location: ISO 3166-1 alpha-2 country code
# - Reachable: Always true for always-on devices
#################################################################################

#################################################################################
# Matter 1.4.1 General Commissioning Cluster (0x0030)
#################################################################################
# Cluster Revision: 1 (Matter 1.4.1)
# Role: Utility | Scope: Node
#
# Manages commissioning workflow including fail-safe, regulatory config, and
# commissioning completion.
#
# ATTRIBUTES:
# ID     | Name                          | Type   | Constraint | Quality | Default | Access | Conf
# -------|-------------------------------|--------|------------|---------|---------|--------|-----
# 0x0000 | Breadcrumb                    | uint64 | all        | N       | 0       | RW VA  | M
# 0x0001 | BasicCommissioningInfo        | struct | -          | F       | -       | R V    | M
# 0x0002 | RegulatoryConfig              | enum8  | 0-2        | F       | -       | R V    | M
# 0x0003 | LocationCapability            | enum8  | 0-2        | F       | -       | R V    | M
# 0x0004 | SupportsConcurrentConnection  | bool   | all        | F       | false   | R V    | M
#
# BasicCommissioningInfo Structure:
#   - FailSafeExpiryLengthSeconds (uint16): Max fail-safe duration (60s)
#   - MaxCumulativeFailsafeSeconds (uint16): Max cumulative fail-safe (900s)
#
# RegulatoryLocationType Enum:
#   0 = Indoor, 1 = Outdoor, 2 = IndoorOutdoor
#
# COMMANDS:
# ID   | Name                          | Dir  | Response | Access | Conf
# -----|-------------------------------|------|----------|--------|-----
# 0x00 | ArmFailSafe                   | C→S  | Y        | A      | M
# 0x01 | ArmFailSafeResponse           | S→C  | -        | -      | M
# 0x02 | SetRegulatoryConfig           | C→S  | Y        | A      | M
# 0x03 | SetRegulatoryConfigResponse   | S→C  | -        | -      | M
# 0x04 | CommissioningComplete         | C→S  | Y        | A      | M
# 0x05 | CommissioningCompleteResponse | S→C  | -        | -      | M
#
# NOTES:
# - Breadcrumb: Tracks commissioning progress across commands
# - Fail-safe: Automatic rollback if commissioning fails
# - RegulatoryConfig: Indoor/Outdoor operation restrictions
#################################################################################

#################################################################################
# Matter 1.4.1 Network Commissioning Cluster (0x0031)
#################################################################################
# Cluster Revision: 1 (Matter 1.4.1)
# Role: Utility | Scope: Node
#
# Configures network connectivity (Wi-Fi, Thread, or Ethernet).
#
# FEATURES:
# - Bit 0 (WI): Wi-Fi Network Interface
# - Bit 1 (TH): Thread Network Interface
# - Bit 2 (ET): Ethernet Network Interface
#
# ATTRIBUTES:
# ID     | Name                  | Type   | Constraint | Quality | Default | Access | Conf
# -------|----------------------|--------|------------|---------|---------|--------|-----
# 0x0003 | ConnectMaxTimeSeconds | uint8  | all        | F       | 30      | R V    | M
# 0x0004 | InterfaceEnabled      | bool   | all        | N       | true    | RW VA  | M
#
# COMMANDS (Wi-Fi):
# - ScanNetworks, AddOrUpdateWiFiNetwork, RemoveNetwork, ConnectNetwork
#
# NOTES:
# - Tasmota implements Wi-Fi variant (WI feature)
# - ConnectMaxTimeSeconds: Max time to establish network connection
# - InterfaceEnabled: Enable/disable network interface
#################################################################################

#################################################################################
# Matter 1.4.1 General Diagnostics Cluster (0x0033)
#################################################################################
# Cluster Revision: 1 (Matter 1.4.1)
# Role: Utility | Scope: Node
#
# Provides system-level diagnostic information.
#
# ATTRIBUTES:
# ID     | Name                     | Type        | Constraint | Quality | Default | Access | Conf
# -------|--------------------------|-------------|------------|---------|---------|--------|-----
# 0x0000 | NetworkInterfaces        | list[struct]| -          | -       | []      | R V    | M
# 0x0001 | RebootCount              | uint16      | all        | N       | 0       | R V    | M
# 0x0002 | UpTime                   | uint64      | all        | -       | 0       | R V    | O
# 0x0008 | TestEventTriggersEnabled | bool        | all        | F       | false   | R V    | M
#
# NetworkInterface Structure:
#   - Name (string): Interface name ("wifi", "ethernet")
#   - IsOperational (bool): Interface operational status
#   - OffPremiseServicesReachableIPv4 (bool): Internet reachable via IPv4
#   - OffPremiseServicesReachableIPv6 (bool): Internet reachable via IPv6
#   - HardwareAddress (octstr): MAC address
#   - IPv4Addresses (list[octstr]): IPv4 addresses
#   - IPv6Addresses (list[octstr]): IPv6 addresses
#   - InterfaceType (enum8): 1=Wi-Fi, 2=Ethernet, 3=Thread
#
# EVENTS:
# ID   | Name       | Priority | Conf
# -----|------------|----------|-----
# 0x03 | BootReason | CRITICAL | M
#
# BootReason Enum:
#   0=Unspecified, 1=PowerOnReboot, 2=BrownOutReset, 3=SoftwareWatchdogReset,
#   4=HardwareWatchdogReset, 5=SoftwareUpdateCompleted, 6=SoftwareReset
#
# NOTES:
# - NetworkInterfaces: Lists all active network interfaces
# - RebootCount: Increments on each boot
# - UpTime: Seconds since last boot
#################################################################################

#################################################################################
# Matter 1.4.1 Time Synchronization Cluster (0x0038)
#################################################################################
# Cluster Revision: 2 (Matter 1.4.1)
# Role: Utility | Scope: Node
#
# Provides time synchronization capabilities.
#
# ATTRIBUTES:
# ID     | Name        | Type     | Constraint | Quality | Default | Access | Conf
# -------|-------------|----------|------------|---------|---------|--------|-----
# 0x0000 | UTCTime     | epoch-us | all        | X       | null    | R V    | O
# 0x0001 | Granularity | enum8    | 0-5        | -       | -       | R V    | M
# 0x0007 | LocalTime   | epoch-us | all        | X       | null    | R V    | O
#
# Granularity Enum:
#   0=NoTimeGranularity, 1=MinutesGranularity, 2=SecondsGranularity,
#   3=MillisecondsGranularity, 4=MicrosecondsGranularity
#
# NOTES:
# - UTCTime: Current UTC time in microseconds since Unix epoch
# - LocalTime: Current local time with timezone offset
# - Granularity: Time precision (3=Milliseconds for NTP)
# - Tasmota uses NTP for time synchronization
#################################################################################

#################################################################################
# Matter 1.4.1 Node Operational Credentials Cluster (0x003E)
#################################################################################
# Cluster Revision: 1 (Matter 1.4.1)
# Role: Utility | Scope: Node
#
# Manages operational certificates, fabrics, and trust anchors.
#
# ATTRIBUTES:
# ID     | Name                      | Type         | Constraint | Quality | Default | Access | Conf
# -------|---------------------------|--------------|------------|---------|---------|--------|-----
# 0x0000 | NOCs                      | list[struct] | max 5      | NF      | []      | R A    | M
# 0x0001 | Fabrics                   | list[struct] | max 5      | NF      | []      | R V    | M
# 0x0002 | SupportedFabrics          | uint8        | min 5      | F       | 5       | R V    | M
# 0x0003 | CommissionedFabrics       | uint8        | all        | -       | 0       | R V    | M
# 0x0004 | TrustedRootCertificates   | list[octstr] | -          | N       | []      | R V    | M
# 0x0005 | CurrentFabricIndex        | uint8        | 1-254      | -       | -       | R V    | M
#
# NOCStruct:
#   - NOC (octstr): Node Operational Certificate
#   - ICAC (octstr, nullable): Intermediate CA Certificate
#   - FabricIndex (uint8): Fabric identifier (1-254)
#
# FabricDescriptorStruct:
#   - RootPublicKey (octstr): Root CA public key
#   - VendorID (uint16): Administrator vendor ID
#   - FabricID (uint64): Fabric identifier
#   - NodeID (uint64): Node identifier within fabric
#   - Label (string): User-assigned fabric label
#   - FabricIndex (uint8): Fabric index
#
# COMMANDS:
# ID   | Name                        | Dir  | Response | Access | Conf
# -----|----------------------------|------|----------|--------|-----
# 0x00 | AttestationRequest          | C→S  | Y        | A      | M
# 0x01 | AttestationResponse         | S→C  | -        | -      | M
# 0x02 | CertificateChainRequest     | C→S  | Y        | A      | M
# 0x03 | CertificateChainResponse    | S→C  | -        | -      | M
# 0x04 | CSRRequest                  | C→S  | Y        | A      | M
# 0x05 | CSRResponse                 | S→C  | -        | -      | M
# 0x06 | AddNOC                      | C→S  | Y        | A      | M
# 0x07 | UpdateNOC                   | C→S  | Y        | A      | O
# 0x08 | NOCResponse                 | S→C  | -        | -      | M
# 0x09 | UpdateFabricLabel           | C→S  | Y        | A      | M
# 0x0A | RemoveFabric                | C→S  | Y        | A      | M
# 0x0B | AddTrustedRootCertificate   | C→S  | N        | A      | M
#
# NOTES:
# - NOCs: Operational certificates for each fabric
# - Fabrics: List of commissioned fabrics
# - SupportedFabrics: Max 5 fabrics minimum per spec
# - CurrentFabricIndex: Fabric index of current session
# - Certificate chain: PAA → PAI → DAC (Device Attestation)
# - Certificate chain: RCAC → ICAC → NOC (Operational)
#################################################################################

#################################################################################
# Matter 1.4.1 Administrator Commissioning Cluster (0x003C)
#################################################################################
# Cluster Revision: 1 (Matter 1.4.1)
# Role: Utility | Scope: Node
#
# Controls commissioning window for adding new administrators.
#
# ATTRIBUTES:
# ID     | Name             | Type  | Constraint | Quality | Default | Access | Conf
# -------|------------------|-------|------------|---------|---------|--------|-----
# 0x0000 | WindowStatus     | enum8 | 0-2        | -       | 0       | R V    | M
# 0x0001 | AdminFabricIndex | uint8 | 1-254      | X       | null    | R V    | M
# 0x0002 | AdminVendorId    | uint16| all        | X       | null    | R V    | M
#
# WindowStatus Enum:
#   0 = WindowNotOpen
#   1 = EnhancedWindowOpen (with PAKE verifier)
#   2 = BasicWindowOpen (with device passcode)
#
# COMMANDS:
# ID   | Name                           | Dir  | Response | Access | Conf
# -----|--------------------------------|------|----------|--------|-----
# 0x00 | OpenCommissioningWindow        | C→S  | N        | A      | M
# 0x01 | OpenBasicCommissioningWindow   | C→S  | N        | A      | M
# 0x02 | RevokeCommissioning            | C→S  | N        | A      | M
#
# NOTES:
# - OpenCommissioningWindow: Enhanced method with dynamic passcode
# - OpenBasicCommissioningWindow: Uses device's built-in passcode
# - RevokeCommissioning: Closes commissioning window
# - AdminFabricIndex: Fabric that opened the window
# - AdminVendorId: Vendor ID of administrator
#################################################################################

#################################################################################
# Matter 1.4.1 Access Control Cluster (0x001F)
#################################################################################
# Cluster Revision: 2 (Matter 1.4.1)
# Role: Utility | Scope: Node
#
# Manages Access Control Lists (ACLs) for authorization.
#
# ATTRIBUTES:
# ID     | Name                              | Type         | Constraint | Quality | Default | Access | Conf
# -------|-----------------------------------|--------------|------------|---------|---------|--------|-----
# 0x0000 | ACL                               | list[struct] | max 4      | NF      | []      | RW FA  | M
# 0x0001 | Extension                         | list[struct] | max 4      | NF      | []      | RW FA  | O
# 0x0002 | SubjectsPerAccessControlEntry     | uint16       | min 4      | F       | 4       | R V    | M
# 0x0003 | TargetsPerAccessControlEntry      | uint16       | min 3      | F       | 3       | R V    | M
# 0x0004 | AccessControlEntriesPerFabric     | uint16       | min 4      | F       | 4       | R V    | M
#
# AccessControlEntryStruct:
#   - Privilege (enum8): 1=View, 2=ProxyView, 3=Operate, 4=Manage, 5=Administer
#   - AuthMode (enum8): 1=PASE, 2=CASE, 3=Group
#   - Subjects (list[uint64], nullable): Node IDs or CATs (max 4)
#   - Targets (list[struct], nullable): Cluster/Endpoint/DeviceType (max 3)
#   - FabricIndex (uint8): Fabric identifier
#
# Privilege Hierarchy:
#   Administer > Manage > Operate > ProxyView, View
#
# NOTES:
# - ACL: Per-fabric access control entries
# - Subjects: null = wildcard (all subjects)
# - Targets: null = wildcard (all targets)
# - PASE commissioning implicitly grants Administer privilege
# - Minimum 4 ACL entries per fabric
#################################################################################

#################################################################################
# Matter 1.4.1 Group Key Management Cluster (0x003F)
#################################################################################
# Cluster Revision: 1 (Matter 1.4.1)
# Role: Utility | Scope: Node
#
# Manages group communication encryption keys.
#
# ATTRIBUTES:
# ID     | Name                  | Type         | Constraint | Quality | Default | Access | Conf
# --------|-----------------------|--------------|------------|---------|---------|--------|-----
# 0x0000 | GroupKeyMap           | list[struct] | -          | NF      | []      | RW FA  | M
# 0x0001 | GroupTable            | list[struct] | -          | NF      | []      | R V    | M
# 0x0002 | MaxGroupsPerFabric    | uint16       | min 4      | F       | 4       | R V    | M
# 0x0003 | MaxGroupKeysPerFabric | uint16       | min 3      | F       | 3       | R V    | M
#
# GroupKeyMapStruct:
#   - GroupId (uint16): Group identifier
#   - GroupKeySetID (uint16): Key set identifier
#   - FabricIndex (uint8): Fabric identifier
#
# GroupKeySetStruct:
#   - GroupKeySetID (uint16): Key set identifier (0 = IPK)
#   - GroupKeySecurityPolicy (enum8): Security policy
#   - EpochKey0/1/2 (octstr, nullable): Epoch keys
#   - EpochStartTime0/1/2 (uint64, nullable): Epoch start times
#
# COMMANDS:
# ID   | Name                      | Dir  | Response | Access | Conf
# -----|---------------------------|------|----------|--------|-----
# 0x00 | KeySetWrite               | C→S  | N        | A      | M
# 0x01 | KeySetRead                | C→S  | Y        | A      | M
# 0x02 | KeySetReadResponse        | S→C  | -        | -      | M
# 0x03 | KeySetRemove              | C→S  | N        | A      | M
# 0x04 | KeySetReadAllIndices      | C→S  | Y        | A      | M
# 0x05 | KeySetReadAllIndicesResponse | S→C | -     | -      | M
#
# NOTES:
# - GroupKeySetID 0: Identity Protection Key (IPK) for CASE
# - Epoch keys: Up to 3 keys with rotation support
# - GroupKeyMap: Maps groups to key sets
# - Minimum 4 groups and 3 key sets per fabric
#################################################################################

#################################################################################
# Matter 1.4.1 Localization Configuration Cluster (0x002B)
#################################################################################
# Cluster Revision: 1 (Matter 1.4.1)
# Role: Utility | Scope: Node
#
# Configures locale settings.
#
# ATTRIBUTES:
# ID     | Name             | Type         | Constraint | Quality | Default | Access | Conf
# -------|------------------|--------------|------------|---------|---------|--------|-----
# 0x0000 | ActiveLocale     | string       | max 35     | N       | "en-US" | RW VM  | M
# 0x0001 | SupportedLocales | list[string] | -          | F       | -       | R V    | M
#
# NOTES:
# - ActiveLocale: BCP 47 language tag (e.g., "en-US", "fr-FR")
# - SupportedLocales: List of supported locales
# - Tasmota returns current locale from tasmota.locale()
#################################################################################

#################################################################################
# Matter 1.4.1 Time Format Localization Cluster (0x002C)
#################################################################################
# Cluster Revision: 1 (Matter 1.4.1)
# Role: Utility | Scope: Node
#
# Configures time and date formatting preferences.
#
# ATTRIBUTES:
# ID     | Name                   | Type         | Constraint | Quality | Default | Access | Conf
# -------|------------------------|--------------|------------|---------|---------|--------|-----
# 0x0000 | HourFormat             | enum8        | 0-1        | N       | 0       | RW VM  | M
# 0x0001 | ActiveCalendarType     | enum8        | 0-11       | N       | 0       | RW VM  | M
# 0x0002 | SupportedCalendarTypes | list[enum8]  | -          | F       | -       | R V    | M
#
# HourFormat Enum:
#   0 = 12hr (12-hour AM/PM)
#   1 = 24hr (24-hour)
#
# CalendarType Enum:
#   0=Buddhist, 1=Chinese, 2=Coptic, 3=Ethiopian, 4=Gregorian, 5=Hebrew,
#   6=Indian, 7=Islamic, 8=Japanese, 9=Korean, 10=Persian, 11=Taiwanese
#
# NOTES:
# - HourFormat: 12-hour vs 24-hour time display
# - ActiveCalendarType: Calendar system (4=Gregorian most common)
# - Tasmota defaults to 24hr and Gregorian calendar
#################################################################################

#################################################################################
# Matter 1.4.1 Unit Localization Cluster (0x002D)
#################################################################################
# Cluster Revision: 1 (Matter 1.3+)
# Role: Utility | Scope: Node
#
# Configures unit preferences (temperature, etc.).
#
# ATTRIBUTES:
# ID     | Name            | Type  | Constraint | Quality | Default | Access | Conf
# -------|-----------------|-------|------------|---------|---------|--------|-----
# 0x0000 | TemperatureUnit | enum8 | 0-2        | N       | 1       | RW VM  | O
#
# TemperatureUnit Enum:
#   0 = Fahrenheit
#   1 = Celsius
#   2 = Kelvin
#
# NOTES:
# - TemperatureUnit: Temperature display preference
# - Tasmota maps from SetOption8 (0=Celsius, 1=Fahrenheit)
# - Matter enum is inverted: 0=Fahrenheit, 1=Celsius
#################################################################################

#################################################################################
# Matter 1.4.1 ICD Management Cluster (0x0046)
#################################################################################
# Cluster Revision: 3 (Matter 1.4.1)
# Role: Utility | Scope: Node
#
# Manages Intermittently Connected Device (ICD) behavior for battery-powered
# or sleepy devices. Tasmota WiFi devices are always-on (SIT mode).
#
# FEATURES:
# - Bit 0 (CIP): Check-In Protocol Support
# - Bit 1 (UAT): User Active Mode Trigger
# - Bit 2 (LITS): Long Idle Time Support
# - Bit 3 (DSLS): Dynamic SIT/LIT Support
#
# ATTRIBUTES (Base SIT mode - no CIP/LITS):
# ID     | Name               | Type   | Constraint  | Quality | Default | Access | Conf
# -------|-------------------|--------|-------------|---------|---------|--------|-----
# 0x0000 | IdleModeDuration  | uint32 | 1-64800     | F       | 1       | R V    | M
# 0x0001 | ActiveModeDuration| uint32 | all         | F       | 300     | R V    | M
# 0x0002 | ActiveModeThreshold| uint16| all         | F       | 300     | R V    | M
#
# NOTES:
# - IdleModeDuration: Max seconds in idle mode (1s for always-on WiFi)
# - ActiveModeDuration: Min milliseconds in active mode (300ms)
# - ActiveModeThreshold: Min milliseconds active after network activity (300ms)
# - SIT Mode: IdleModeDuration ≤ 15 seconds (always-on devices)
# - LIT Mode: IdleModeDuration > 15 seconds (battery-powered devices)
# - Tasmota WiFi devices: Always-on, SIT mode, no CIP/LITS features
# - Attributes 0x0003-0x0005 require CIP feature (not implemented)
# - Attribute 0x0008 (OperatingMode) requires LITS feature (not implemented)
#################################################################################

#################################################################################
# Matter 1.4.1 OTA Software Update Requestor Cluster (0x002A)
#################################################################################
# Cluster Revision: 1 (Matter 1.4.1)
# Role: Utility | Scope: Node
#
# Requests and manages firmware updates (OTA).
#
# ATTRIBUTES:
# ID     | Name                | Type         | Constraint | Quality | Default | Access | Conf
# -------|---------------------|--------------|------------|---------|---------|--------|-----
# 0x0000 | DefaultOTAProviders | list[struct] | -          | N       | []      | RW VA  | M
# 0x0001 | UpdatePossible      | bool         | all        | -       | true    | R V    | M
# 0x0002 | UpdateState         | enum8        | 0-6        | -       | 0       | R V    | M
# 0x0003 | UpdateStateProgress | uint8        | 0-100      | X       | null    | R V    | M
#
# UpdateState Enum:
#   0=Unknown, 1=Idle, 2=Querying, 3=DelayedOnQuery, 4=Downloading,
#   5=Applying, 6=DelayedOnApply, 7=RollingBack, 8=DelayedOnUserConsent
#
# NOTES:
# - DefaultOTAProviders: List of OTA provider endpoints
# - UpdatePossible: Whether updates can be performed
# - UpdateState: Current OTA update state
# - Tasmota: Updates via web UI, not Matter OTA protocol
#################################################################################

#################################################################################
# Matter 1.4.1 Diagnostic Logs Cluster (0x0032)
#################################################################################
# Cluster Revision: 1 (Matter 1.4.1)
# Role: Utility | Scope: Node
#
# Provides access to diagnostic logs via BDX protocol.
#
# COMMANDS:
# ID   | Name                  | Dir  | Response | Access | Conf
# -----|----------------------|------|----------|--------|-----
# 0x00 | RetrieveLogsRequest  | C→S  | Y        | M      | M
# 0x01 | RetrieveLogsResponse | S→C  | -        | -      | M
#
# NOTES:
# - No mandatory attributes
# - Logs retrieved via Bulk Data Exchange (BDX) protocol
# - Intent: EndUserSupport, NetworkDiag, CrashLogs
#################################################################################

#################################################################################
# Matter 1.4.1 Software Diagnostics Cluster (0x0034)
#################################################################################
# Cluster Revision: 1 (Matter 1.4.1)
# Role: Utility | Scope: Node
#
# Provides software-level diagnostic information.
#
# ATTRIBUTES (All Optional):
# ID     | Name                  | Type   | Constraint | Quality | Default | Access | Conf
# -------|----------------------|--------|------------|---------|---------|--------|-----
# 0x0000 | ThreadMetrics        | list   | -          | -       | []      | R V    | O
# 0x0001 | CurrentHeapFree      | uint64 | all        | -       | 0       | R V    | O
# 0x0002 | CurrentHeapUsed      | uint64 | all        | -       | 0       | R V    | O
# 0x0003 | CurrentHeapHighWatermark | uint64 | all    | -       | 0       | R V    | O
#
# NOTES:
# - All attributes optional
# - ThreadMetrics: Per-thread stack usage
# - Heap metrics: Memory usage statistics
#################################################################################


  static var TYPE = "root"            # name of the plug-in in json
  static var DISPLAY_NAME = "Root node"       # display name of the plug-in
  static var CLUSTERS  = matter.consolidate_clusters(_class, {
    # 0x001D: inherited               # Descriptor Cluster 9.5 p.453
    0x001F: [0,2,3,4],                # Access Control Cluster, p.461
    0x0028: [0,1,2,3,4,5,6,7,8,9,0x0A,0x0F,0x11,0x12,0x13],# Basic Information Cluster cluster 11.1 p.565
    # 0x002A: [0,1,2,3],                # OTA Software Update Requestor Cluster Definition 11.19.7 p.762
    0x002B: [0,1],                    # Localization Configuration Cluster 11.3 p.580
    0x002C: [0,1,2],                  # Time Format Localization Cluster 11.4 p.581
    0x002D: [0],                      # Unit Localization Cluster 11.5 p.583 (Matter 1.3+)
    0x0030: [0,1,2,3,4],              # GeneralCommissioning cluster 11.9 p.627
    0x0031: [3,4],                    # Network Commissioning Cluster cluster 11.8 p.606
    0x0032: [],                       # Diagnostic Logs Cluster 11.10 p.637
    0x0033: [0,1,2,8],                # General Diagnostics Cluster 11.11 p.642
    0x0034: [],                       # Software Diagnostics Cluster 11.12 p.654
    0x0038: [0,1,7],                  # Time Synchronization 11.16 p.689
    0x003C: [0,1,2],                  # Administrator Commissioning Cluster 11.18 p.725
    0x003E: [0,1,2,3,4,5],            # Node Operational Credentials Cluster 11.17 p.704
    0x003F: [0],                      # Group Key Management Cluster 11.2 p.572
    # ICD Management Cluster (0x0046) - Section 9.17
    # For SIT (Short Idle Time) devices without CIP/LITS features:
    # - IdleModeDuration (0x0000): mandatory
    # - ActiveModeDuration (0x0001): mandatory
    # - ActiveModeThreshold (0x0002): mandatory
    # Note: ICDCounter(0x0004), ClientsSupportedPerFabric(0x0005) require CIP feature
    # Note: OperatingMode(0x0008) requires LITS feature
    0x0046: [0,1,2]                   # ICD Management Cluster - base SIT mode (no CIP/LITS features)
  })
  static var TYPES = { 0x0016: 3 }       # Root node - Matter 1.4.1 Device Library Rev 3

  #############################################################
  # Constructor
  def init(device, endpoint, config)
    super(self).init(device, endpoint, config)
    # publish mandatory events
    self.publish_event(0x0028, 0x00, matter.EVENT_CRITICAL, matter.TLV.Matter_TLV_item().set(matter.TLV.U4, tasmota.version()))   # Event StartUp - Software Version
    self.publish_event(0x0033, 0x03, matter.EVENT_CRITICAL, matter.TLV.Matter_TLV_item().set(matter.TLV.U1, 1))   # Event BootReason - PowerOnReboot - TODO if we need to refine
  end

  #############################################################
  # read an attribute
  #
  def read_attribute(session, ctx, tlv_solo)
    import string
    var TLV = matter.TLV
    var cluster = ctx.cluster
    var attribute = ctx.attribute

    if   cluster == 0x0030              # ========== GeneralCommissioning cluster 11.9 p.627 ==========

      if   attribute == 0x0000          # ---------- Breadcrumb ----------
        return tlv_solo.set(TLV.U8, session._breadcrumb)
      elif attribute == 0x0001          # ---------- BasicCommissioningInfo / BasicCommissioningInfo----------
        var bci = TLV.Matter_TLV_struct()
        bci.add_TLV(0, TLV.U2, 60)      # FailSafeExpiryLengthSeconds
        bci.add_TLV(1, TLV.U2, 900)     # MaxCumulativeFailsafeSeconds
        return bci
      elif attribute == 0x0002          # ---------- RegulatoryConfig / RegulatoryLocationType ----------
        return tlv_solo.set(TLV.U1, 2)    # 2 = IndoorOutdoor | esp-matter = 0
      elif attribute == 0x0003          # ---------- LocationCapability / RegulatoryLocationType----------
        return tlv_solo.set(TLV.U1, 2)    # 2 = IndoorOutdoor
      elif attribute == 0x0004          # ---------- SupportsConcurrentConnection / bool ----------
        return tlv_solo.set(TLV.BOOL, false)    # false - maybe can set to true
      end
    # ====================================================================================================
    elif cluster == 0x0032              # ========== Diagnostic Logs Cluster 11.10 p.637 ==========
      # no attributes

    # ====================================================================================================
    elif cluster == 0x0033              # ========== General Diagnostics Cluster 11.11 p.642 ==========

      if   attribute == 0x0000          #  ---------- NetworkInterfaces ----------
        var nwi = TLV.Matter_TLV_array() # list network interfaces, empty list for now, p.647

        var tas_eth = tasmota.eth()
        if (tas_eth['up'])
          var eth = nwi.add_struct(nil)
          eth.add_TLV(0, TLV.UTF1, 'ethernet')      # Name
          eth.add_TLV(1, TLV.BOOL, 1)               # IsOperational
          eth.add_TLV(2, TLV.BOOL, 1)               # OffPremiseServicesReachableIPv4
          eth.add_TLV(3, TLV.NULL, nil)             # OffPremiseServicesReachableIPv6
          var mac = bytes().fromhex(string.replace(tas_eth.find("mac", ""), ":", ""))
          eth.add_TLV(4, TLV.B1, mac)               # HardwareAddress
          var ip4 = eth.add_array(5)                # IPv4Addresses
          ip4.add_TLV(nil, TLV.B1, matter.get_ip_bytes(tas_eth.find("ip", "")))
          var ip6 = eth.add_array(6)                # IPv6Addresses
          ip6.add_TLV(nil, TLV.B1, matter.get_ip_bytes(tas_eth.find("ip6local", "")))
          ip6.add_TLV(nil, TLV.B1, matter.get_ip_bytes(tas_eth.find("ip6", "")))
          eth.add_TLV(7, TLV.U1, 2)                 # InterfaceType, p646
        end

        var tas_wif = tasmota.wifi()
        if (tas_wif['up'])
          var wif = nwi.add_struct(nil)
          wif.add_TLV(0, TLV.UTF1, 'wifi')          # Name
          wif.add_TLV(1, TLV.BOOL, 1)               # IsOperational
          wif.add_TLV(2, TLV.BOOL, 1)               # OffPremiseServicesReachableIPv4
          wif.add_TLV(3, TLV.NULL, nil)             # OffPremiseServicesReachableIPv6
          var mac = bytes().fromhex(string.replace(tas_wif.find("mac", ""), ":", ""))
          wif.add_TLV(4, TLV.B1, mac)               # HardwareAddress
          var ip4 = wif.add_array(5)                # IPv4Addresses
          ip4.add_TLV(nil, TLV.B1, matter.get_ip_bytes(tas_wif.find("ip", "")))
          var ip6 = wif.add_array(6)                # IPv6Addresses
          ip6.add_TLV(nil, TLV.B1, matter.get_ip_bytes(tas_wif.find("ip6local", "")))
          ip6.add_TLV(nil, TLV.B1, matter.get_ip_bytes(tas_wif.find("ip6", "")))
          wif.add_TLV(7, TLV.U1, 1)                 # InterfaceType, p646
        end
        return nwi
      elif attribute == 0x0001          #  ---------- RebootCount u16 ----------
        return tlv_solo.set(TLV.U2, tasmota.cmd("Status 1", true)['StatusPRM']['BootCount'])
      elif attribute == 0x0002          #  ---------- UpTime u16 ----------
        return tlv_solo.set(TLV.U4, tasmota.cmd("Status 11", true)['StatusSTS']['UptimeSec'])
      # TODO add later other attributes
      elif attribute == 0x0008          #  ---------- TestEventTriggersEnabled bool ----------
        return tlv_solo.set(TLV.BOOL, false)    # false - maybe can set to true
      end
        
    # ====================================================================================================
    elif cluster == 0x0034              # ========== Software Diagnostics Cluster 11.12 p.654 ==========
      # no mandatory attributes - to be added later (maybe)

    # ====================================================================================================
    elif cluster == 0x0038              # ========== Time Synchronization 11.16 p.689 ==========
      if   attribute == 0x0000          #  ---------- UTCTime / epoch_us ----------
        var epoch_us = int64(tasmota.rtc_utc()) * int64(1000000)
        return tlv_solo.set(TLV.U8, epoch_us)     # TODO test the conversion of int64()
      elif attribute == 0x0001          #  ---------- Granularity / enum ----------
        return tlv_solo.set(TLV.U1, 3)     # MillisecondsGranularity (NTP every hour, i.e. 36ms max drift)
      # TODO add some missing args
      elif attribute == 0x0007          #  ---------- LocalTime / epoch_us ----------
        var epoch_us = int64(tasmota.rtc('local')) * int64(1000000)
        return tlv_solo.set(TLV.U8, epoch_us)     # TODO test the conversion of int64()
      end

    # ====================================================================================================
    elif cluster == 0x003E              # ========== Node Operational Credentials Cluster 11.17 p.704 ==========
      self.ack_request(ctx)             # long operation, send Ack first

      if   attribute == 0x0000          #  ---------- NOCs / list[NOCStruct] ----------
        var nocl = TLV.Matter_TLV_array() # NOCs, p.711
        var fabs = ctx.fabric_filtered ? [session.get_fabric()] : self.device.sessions.active_fabrics()
        for loc_fabric: fabs
          if loc_fabric.is_marked_for_deletion()    continue end    # fabric is scheduled for deletion
          if loc_fabric == nil    continue  end
          var nocs = nocl.add_struct(nil)
          nocs.add_TLV(1, TLV.B2, loc_fabric.get_noc())      # NOC
          nocs.add_TLV(2, TLV.B2, loc_fabric.get_icac())     # ICAC
          nocs.add_TLV(0xFE, TLV.U2, loc_fabric.get_fabric_index())    # Label
        end
        return nocl
      elif attribute == 0x0001          #  ---------- Fabrics / list[FabricDescriptorStruct] ----------
        var fabrics = TLV.Matter_TLV_array() # Fabrics, p.711
        var fabs = ctx.fabric_filtered ? [session.get_fabric()] : self.device.sessions.active_fabrics()
        for loc_fabric: fabs
          if loc_fabric == nil    continue  end
          if loc_fabric.is_marked_for_deletion()    continue end    # fabric is scheduled for deletion
          var root_ca_tlv = TLV.parse(loc_fabric.get_ca())
          var fab = fabrics.add_struct(nil)            # encoding see p.303
          fab.add_TLV(1, TLV.B2, root_ca_tlv.findsubval(9)) # RootPublicKey
          fab.add_TLV(2, TLV.U2, loc_fabric.get_admin_vendor())      # VendorID
          fab.add_TLV(3, TLV.U8, loc_fabric.get_fabric_id_as_int64())            # FabricID
          fab.add_TLV(4, TLV.U8, loc_fabric.get_device_id_as_int64())          # NodeID
          fab.add_TLV(5, TLV.UTF1, loc_fabric.get_fabric_label())    # Label
          fab.add_TLV(0xFE, TLV.U2, loc_fabric.get_fabric_index())    # idx
        end
        return fabrics
      elif attribute == 0x0002          #  ---------- SupportedFabrics / u1 ----------
        return tlv_solo.set(TLV.U1, matter.Fabric._MAX_CASE)     # Max 5 fabrics
      elif attribute == 0x0003          #  ---------- CommissionedFabrics / u1 ----------
        var fabric_actice = self.device.sessions.count_active_fabrics()
        return tlv_solo.set(TLV.U1, fabric_actice)  # number of active fabrics
      elif attribute == 0x0004          #  ---------- TrustedRootCertificates / list[octstr] ----------
        # TODO
      elif attribute == 0x0005          #  ---------- Current FabricIndex / u1 ----------
        var fab_index = session._fabric.get_fabric_index()
        if fab_index == nil   fab_index = 0   end     # if PASE session, then the fabric index should be zero
        return tlv_solo.set(TLV.U1, fab_index)      # number of active sessions
      end

    # ====================================================================================================
    elif cluster == 0x003C              # ========== Administrator Commissioning Cluster 11.18 p.725 ==========
      if   attribute == 0x0000          #  ---------- WindowStatus / u8 ----------
        var commissioning_open = self.device.commissioning.is_commissioning_open()
        var basic_commissioning = self.device.commissioning.is_root_commissioning_open()
        var val = commissioning_open ? (basic_commissioning ? 2 #-BasicWindowOpen-# : 1 #-EnhancedWindowOpen-#) : 0 #-WindowNotOpen-#
        return tlv_solo.set(TLV.U1, val)
      elif attribute == 0x0001          #  ---------- AdminFabricIndex / u16 ----------
        var admin_fabric = self.device.commissioning.commissioning_admin_fabric
        if admin_fabric != nil
          return tlv_solo.set_or_nil(TLV.U2, admin_fabric.get_fabric_index())
        end
      elif attribute == 0x0002          #  ---------- AdminVendorId / u16 ----------
        var admin_fabric = self.device.commissioning.commissioning_admin_fabric
        if admin_fabric != nil
          return tlv_solo.set_or_nil(TLV.U2, admin_fabric.get_admin_vendor())
        end
      end
        
    # ====================================================================================================
    elif cluster == 0x0028              # ========== Basic Information Cluster cluster 11.1 p.565 ==========
      self.ack_request(ctx)             # long operation, send Ack first

      if   attribute == 0x0000          #  ---------- DataModelRevision ----------
        return tlv_solo.set(TLV.U2, 18)     # 18 = Matter 1.3+ (revision 18 of the Data Model, compatible with 1.4.1)
      elif attribute == 0x0001          #  ---------- VendorName / string ----------
        return tlv_solo.set(TLV.UTF1, "Tasmota")
      elif attribute == 0x0002          #  ---------- VendorID / vendor-id ----------
        return tlv_solo.set(TLV.U2, self.device.VENDOR_ID)    # Vendor ID reserved for development
      elif attribute == 0x0003          #  ---------- ProductName / string ----------
        return tlv_solo.set(TLV.UTF1, tasmota.cmd("DeviceName", true)['DeviceName'])
      elif attribute == 0x0004          #  ---------- ProductID / u16 (opt) ----------
        return tlv_solo.set(TLV.U2, 32768)    # taken from esp-matter example
      elif attribute == 0x0005          #  ---------- NodeLabel / string ----------
        return tlv_solo.set(TLV.UTF1, tasmota.cmd("FriendlyName", true)['FriendlyName1'])
      elif attribute == 0x0006          #  ---------- Location / string ----------
        return tlv_solo.set(TLV.UTF1, "XX")   # no location
      elif attribute == 0x0007          #  ---------- HardwareVersion / u16 ----------
        return tlv_solo.set(TLV.U2, 0)
      elif attribute == 0x0008          #  ---------- HardwareVersionString / string ----------
        return tlv_solo.set(TLV.UTF1, tasmota.cmd("Status 2", true)['StatusFWR']['Hardware'])
      elif attribute == 0x0009          #  ---------- SoftwareVersion / u32 ----------
        return tlv_solo.set(TLV.U4, tasmota.version())
      elif attribute == 0x000A          #  ---------- SoftwareVersionString / string ----------
        var version_full = tasmota.cmd("Status 2", true)['StatusFWR']['Version']
        var version_end = string.find(version_full, '(')
        if version_end > 0    version_full = version_full[0..version_end - 1]   end
        return tlv_solo.set(TLV.UTF1, version_full)
      elif attribute == 0x000F          #  ---------- SerialNumber / string ----------
        return tlv_solo.set(TLV.UTF1, tasmota.wifi().find("mac", ""))
      elif attribute == 0x0011          #  ---------- Reachable / bool ----------
        return tlv_solo.set(TLV.BOOL, 1)     # by default we are reachable
      elif attribute == 0x0012          #  ---------- UniqueID / string 32 max ----------
        return tlv_solo.set(TLV.UTF1, tasmota.wifi().find("mac", ""))
      elif attribute == 0x0013          #  ---------- CapabilityMinima / CapabilityMinimaStruct ----------
        var cps = TLV.Matter_TLV_struct()
        cps.add_TLV(0, TLV.U2, 3)       # CaseSessionsPerFabric = 3
        cps.add_TLV(1, TLV.U2, 3)       # SubscriptionsPerFabric = 5
        return cps
      end

    # ====================================================================================================
    elif cluster == 0x003F              # ========== Group Key Management Cluster 11.2 p.572 ==========
      
      if   attribute == 0x0000          #  ---------- GroupKeyMap / list[GroupKeyMapStruct] ----------
        # Return empty list for now - group keys managed internally
        return TLV.Matter_TLV_array()
      end

    # ====================================================================================================
    elif cluster == 0x002A              # ========== OTA Software Update Requestor Cluster Definition 11.19.7 p.762 ==========

      if   attribute == 0x0000          #  ---------- DefaultOTAProviders / list[ProviderLocationStruct] ----------
        return TLV.Matter_TLV_array()   # empty list for now TODO
      elif attribute == 0x0001          #  ---------- UpdatePossible / bool ----------
        return tlv_solo.set(TLV.BOOL, 0)  # we claim that update is not possible, would require to go to Tasmota UI
      elif attribute == 0x0002          #  ---------- UpdateState / UpdateStateEnum ----------
        return tlv_solo.set(TLV.U1, 1)  # Idle
      elif attribute == 0x0003          #  ---------- UpdateStateProgress / uint8 ----------
        return tlv_solo.set(TLV.NULL, nil)  # null, nothing in process
      end

    # ====================================================================================================
    elif cluster == 0x002B              # ========== Localization Configuration Cluster 11.3 p.580 ==========

      if   attribute == 0x0000          #  ---------- ActiveLocale / string ----------
        return tlv_solo.set(TLV.UTF1, tasmota.locale())
      elif attribute == 0x0001          #  ---------- SupportedLocales / list[string] ----------
        var locl = TLV.Matter_TLV_array()
        locl.add_TLV(nil, TLV.UTF1, tasmota.locale())
        return locl
      end

    # ====================================================================================================
    elif cluster == 0x002C              # ========== Time Format Localization Cluster 11.4 p.581 ==========

      if   attribute == 0x0000          #  ---------- HourFormat / HourFormat ----------
        return tlv_solo.set(TLV.U1, 1)  # 1 = 24hr
      elif attribute == 0x0001          #  ---------- ActiveCalendarType / CalendarType ----------
        return tlv_solo.set(TLV.U1, 4)  # 4 = Gregorian
      elif attribute == 0x0002          #  ---------- SupportedCalendarTypes / list[CalendarType] ----------
        var callist = TLV.Matter_TLV_array()
        callist.add_TLV(nil, TLV.create_TLV(TLV.U1, 4))
        return callist
      end

    # ====================================================================================================
    elif cluster == 0x002D              # ========== Unit Localization Cluster 11.5 p.583 (Matter 1.3+) ==========

      if   attribute == 0x0000          #  ---------- TemperatureUnit / TempUnitEnum ----------
        # 0=Fahrenheit, 1=Celsius, 2=Kelvin
        # SetOption8: 0=Celsius, 1=Fahrenheit -> Matter: 0=Fahrenheit, 1=Celsius (inverted)
        return tlv_solo.set(TLV.U1, tasmota.get_option(8) ? 0 : 1)
      end

    # ====================================================================================================
    elif cluster == 0x0031              # ========== Network Commissioning Cluster cluster 11.8 p.606 ==========
      if   attribute == 0x0003          #  ---------- ConnectMaxTimeSeconds / uint8 ----------
        return tlv_solo.set(TLV.U1, 30)    # 30 - value taking from example in esp-matter
      end

    elif cluster == 0x001D              # ========== Descriptor Cluster 9.5 p.453 ==========

      # overwrite ClientList
      if   attribute == 0x0002          # ---------- ClientList / list[cluster-id] ----------
        var pl = TLV.Matter_TLV_array()
        # from connectedhome reference implementation
        pl.add_TLV(nil, TLV.U2, 0x001F)     # Access Control Cluster
        return pl        
      # overwrite PartsList
      elif attribute == 0x0003          # ---------- PartsList / list[endpoint-no]----------
        var pl = TLV.Matter_TLV_array()
        var eps = self.device.get_active_endpoints(true)
        var disable_bridge_mode = self.device.disable_bridge_mode
        for ep: eps
          # if bridge mode is disabled, don't announce Aggregatore (above 0xFF00)
          if !disable_bridge_mode || ep != matter.AGGREGATOR_ENDPOINT
            pl.add_TLV(nil, TLV.U2, ep)     # add each endpoint
          end
        end
        return pl
      end

    # ====================================================================================================
    elif cluster == 0x0046              # ========== ICD Management Cluster 9.17 ==========
      # ICD = Intermittently Connected Device
      # For Tasmota WiFi devices: always-on, mains-powered, SIT mode
      # This tells controllers the device is always reachable
      # Per Matter 1.4.1 spec section 9.17.6:
      # - IdleModeDuration: max interval in seconds device stays in idle mode (constraint 1-64800, default 1)
      # - ActiveModeDuration: min interval in ms device stays in active mode (default 300)
      # - ActiveModeThreshold: min time in ms device stays active after network activity (default 300)

      if   attribute == 0x0000          #  ---------- IdleModeDuration / uint32 (seconds) ----------
        # For always-on WiFi device: 1 second (minimum per spec, device is always active)
        return tlv_solo.set(TLV.U4, 1)
      elif attribute == 0x0001          #  ---------- ActiveModeDuration / uint32 (milliseconds) ----------
        # For always-on WiFi device: 300ms (spec default, device never actually sleeps)
        return tlv_solo.set(TLV.U4, 300)
      elif attribute == 0x0002          #  ---------- ActiveModeThreshold / uint16 (milliseconds) ----------
        # For always-on WiFi device: 300ms (spec default)
        return tlv_solo.set(TLV.U2, 300)
      # Note: attributes 0x0003-0x0005 require CIP feature (Check-In Protocol)
      # Note: attribute 0x0008 (OperatingMode) requires LITS feature (Long Idle Time Support)
      # We don't implement CIP or LITS since this is an always-on WiFi device
      end

    end
    return super(self).read_attribute(session, ctx, tlv_solo)
  end

  #############################################################
  # Invoke a command
  #
  # returns a TLV object if successful, contains the response
  #   or an `int` to indicate a status
  def invoke_request(session, val, ctx)
    import crypto
    var TLV = matter.TLV
    var cluster = ctx.cluster
    var command = ctx.command
    if   cluster == 0x0030              # ========== GeneralCommissioning cluster 11.9 p.627 ==========

      if   command == 0x0000            # ---------- ArmFailSafe ----------
        # create ArmFailSafeResponse
        # ID=1
        #  0=ErrorCode (OK=0)
        #  1=DebugText
        var ExpiryLengthSeconds = val.findsubval(0, 900)
        var Breadcrumb = val.findsubval(1, 0)
        session._breadcrumb = Breadcrumb

        var afsr = TLV.Matter_TLV_struct()
        afsr.add_TLV(0, TLV.U1, 0)      # ErrorCode = OK
        afsr.add_TLV(1, TLV.UTF1, "")   # DebugText = ""
        ctx.command = 0x01              # ArmFailSafeResponse
        return afsr

      elif command == 0x0002            # ---------- SetRegulatoryConfig ----------
        var NewRegulatoryConfig = val.findsubval(0)     # RegulatoryLocationType Enum
        var CountryCode = val.findsubval(1, "XX")
        var Breadcrumb = val.findsubval(2, 0)
        session._breadcrumb = Breadcrumb
        # create SetRegulatoryConfigResponse
        # ID=1
        #  0=ErrorCode (OK=0)
        #  1=DebugText
        var srcr = TLV.Matter_TLV_struct()
        srcr.add_TLV(0, TLV.U1, 0)      # ErrorCode = OK
        srcr.add_TLV(1, TLV.UTF1, "")   # DebugText = ""
        ctx.command = 0x03              # SetRegulatoryConfigResponse
        return srcr

      elif command == 0x0004            # ---------- CommissioningComplete p.636 ----------
        self.ack_request(ctx)           # long operation, send Ack first
        # no data
        if session._fabric
          session._breadcrumb = 0          # clear breadcrumb
          session._fabric.fabric_completed()      # fabric information is complete, persist
          session.set_no_expiration()
          session.save()

          # create CommissioningCompleteResponse
          # ID=1
          #  0=ErrorCode (OK=0)
          #  1=DebugText
          var ccr = TLV.Matter_TLV_struct()
          ccr.add_TLV(0, TLV.U1, 0)      # ErrorCode = OK
          ccr.add_TLV(1, TLV.UTF1, "")   # DebugText = ""
          ctx.command = 0x05              # CommissioningCompleteResponse

          self.device.commissioning.start_commissioning_complete_deferred(session)
          return ccr
        else
          raise "context_error", "CommissioningComplete: no fabric attached"
        end
      end

    elif cluster == 0x003E              # ========== Node Operational Credentials Cluster 11.17 p.704 ==========

      if   command == 0x0002            # ---------- CertificateChainRequest ----------
        var CertificateType = val.findsubval(0)     # CertificateChainType Enum 1=DACCertificate 2=PAICertificate
        if CertificateType != 1 && CertificateType != 2
          ctx.status = matter.UNSUPPORTED_COMMAND
          return nil
        end
        # create CertificateChainResponse
        # ID=1
        #  0=Certificate (octstr)
        var ccr = TLV.Matter_TLV_struct()
        ccr.add_TLV(0, TLV.B2, CertificateType == 1 ? matter.Certs.DAC_Cert_FFF1_8000 : matter.Certs.PAI_Cert_FFF1)      # send DAC_Cert_FFF1_8000 or PAI_Cert_FFF1
        ctx.command = 0x03              # CertificateChainResponse
        return ccr

      elif command == 0x0000            # ---------- AttestationRequest ----------
        var AttestationNonce = val.findsubval(0)     # octstr
        if size(AttestationNonce) != 32   return nil end    # check size on nonce
        ctx.command = 0x01              # AttestationResponse

        # build Attestation Elements 11.17.5.4 p.707
        var att_elts = TLV.Matter_TLV_struct()
        att_elts.add_TLV(1, TLV.B2, matter.Certs.CD_FFF1_8000)    # certification_declaration
        att_elts.add_TLV(2, TLV.B1, AttestationNonce)         # attestation_nonce
        att_elts.add_TLV(3, TLV.U4, tasmota.rtc_utc())     # timestamp in epoch-s
        var attestation_message = att_elts.tlv2raw()

        var ac = session.get_ac()
        var attestation_tbs = attestation_message + ac
        # log("MTR: attestation_tbs=" + attestation_tbs.tohex(), 4)

        var attestation_signature = crypto.EC_P256().ecdsa_sign_sha256(matter.Certs.DAC_Priv_FFF1_8000, attestation_tbs)

        # create AttestationResponse
        # 0=AttestationElements (octstr max 900 bytes)
        # 1=AttestationSignature (octstr 64)
        var ar = TLV.Matter_TLV_struct()
        ar.add_TLV(0, TLV.B2, attestation_message)      # AttestationElements
        ar.add_TLV(1, TLV.B1, attestation_signature)    # AttestationElements
        ctx.command = 0x01              # AttestationResponse
        return ar

      elif command == 0x0004            # ---------- CSRRequest ----------
        self.ack_request(ctx)           # long operation, send Ack first
        var CSRNonce = val.findsubval(0)     # octstr 32
        if size(CSRNonce) != 32   return nil end    # check size on nonce
        var IsForUpdateNOC = val.findsubval(1, false)     # bool
        # log(format("MTR: CSRRequest CSRNonce=%s IsForUpdateNOC=%s", str(CSRNonce), str(IsForUpdateNOC)), 4)

        var csr = session.gen_CSR()

        var nocsr_elements = TLV.Matter_TLV_struct()
        nocsr_elements.add_TLV(1, TLV.B2, csr)
        nocsr_elements.add_TLV(2, TLV.B1, CSRNonce)
        var nocsr_elements_message = nocsr_elements.tlv2raw()
        # sign with attestation challenge
        var nocsr_tbs = nocsr_elements_message + session.get_ac()
        # log("MTR: nocsr_tbs=" + nocsr_tbs.tohex(), 4)
        var attestation_signature = crypto.EC_P256().ecdsa_sign_sha256(matter.Certs.DAC_Priv_FFF1_8000, nocsr_tbs)
        
        # create CSRResponse
        # 0=NOCSRElements (octstr max 900 bytes)
        # 1=AttestationSignature (octstr 64)
        var csrr = TLV.Matter_TLV_struct()
        csrr.add_TLV(0, TLV.B2, nocsr_elements_message)   # AttestationElements
        csrr.add_TLV(1, TLV.B1, attestation_signature)    # AttestationElements
        ctx.command = 0x05              # CSRResponse
        return csrr

      elif command == 0x000B            # ---------- AddTrustedRootCertificate ----------
        var RootCACertificate = val.findsubval(0)     # octstr 400 max
        # TODO - additional tests are expected according to 11.17.7.13. AddTrustedRootCertificate Command
        session.set_temp_ca(RootCACertificate)
        # log("MTR: received ca_root="+RootCACertificate.tohex(), 4)
        ctx.status = matter.SUCCESS                  # OK
        return nil                      # trigger a standalone ack

      elif command == 0x0006            # ---------- AddNOC ----------
        log("MTR: AddNoc Args=" + str(val), 4)
        var NOCValue = val.findsubval(0)        # octstr max 400
        var ICACValue = val.findsubval(1)       # octstr max 400
        # Apple sends an empty ICAC instead of a missing attribute, fix this
        if size(ICACValue) == 0   ICACValue = nil   end
        var IpkValue = val.findsubval(2)        # octstr max 16
        var CaseAdminSubject = val.findsubval(3)
        var AdminVendorId = val.findsubval(4)
        # log("MTR: AddNoc NOCValue=" + (NOCValue ? NOCValue.tohex() : ""), 3)
        # log("MTR: AddNoc ICACValue=" + (ICACValue ? ICACValue.tohex() : ""), 3)
        # log("MTR: AddNoc IpkValue=" + str(IpkValue), 3)
        # log("MTR: AddNoc CaseAdminSubject=" + str(CaseAdminSubject), 3)
        # log("MTR: AddNoc AdminVendorId=" + str(AdminVendorId), 3)

        if session.get_temp_ca() == nil
          log("MTR: Error: AdNOC without CA", 2)
          return nil
        end

        var new_fabric = self.device.sessions.create_fabric()
        new_fabric.set_ca(session.get_temp_ca())          # copy temporary CA to fabric
        new_fabric.set_noc_icac(NOCValue, ICACValue)
        new_fabric.set_ipk_epoch_key(IpkValue)
        new_fabric.set_admin_subject_vendor(CaseAdminSubject, AdminVendorId)
        new_fabric.set_pk(session.get_pk())               # copy the temporary commissioning PK to the fabric

        # extract important information from NOC
        var noc_cert = matter.TLV.parse(NOCValue)
        var dnlist = noc_cert.findsub(6)
        var fabric_id = dnlist.findsubval(21)
        var deviceid = dnlist.findsubval(17)
        # log("MTR: AddNoc noc_cert=" + str(noc_cert), 3)
        # log("MTR: AddNoc dnlist=" + str(dnlist), 3)

        if !fabric_id || !deviceid
          log("MTR: Error: no fabricid nor deviceid in NOC certificate", 2)
          return false
        end
        # convert fo bytes(8)
        if type(fabric_id) == 'int' fabric_id = int64.fromu32(fabric_id).tobytes()  else fabric_id = fabric_id.tobytes() end
        if type(deviceid) == 'int'  deviceid = int64.fromu32(deviceid).tobytes()    else deviceid = deviceid.tobytes() end

        # log("MTR: AddNoc fabric_id=" + str(fabric_id), 3)
        # log("MTR: AddNoc deviceid=" + str(deviceid), 3)

        var root_ca_pub = session.get_temp_ca_pub()
        # log("MTR: AddNoc root_ca_pub=" + str(root_ca_pub), 3)
        # log("MTR: AddNoc root_ca_pub=" + root_ca_pub.tohex(), 3)
        root_ca_pub = root_ca_pub[1..]            # remove first byte as per Matter specification
        var info = bytes().fromstring("CompressedFabric")   # as per spec, 4.3.2.2 p.99
        var hk = crypto.HKDF_SHA256()
        var fabric_rev = fabric_id.copy().reverse()
        var k_fabric = hk.derive(root_ca_pub, fabric_rev, info, 8)
        var parent_fabric = session._fabric ? session._fabric : self.device.commissioning.commissioning_admin_fabric    # get parent fabric whether CASE or PASE
        new_fabric.set_fabric_device(fabric_id, deviceid, k_fabric, parent_fabric)

        # log("MTR: AddNoc k_fabric=" + str(k_fabric), 3)
        # We have a candidate fabric, add it as expirable for 2 minutes
        new_fabric.fabric_candidate()

        # move to next step
        self.device.commissioning.start_operational_discovery_deferred(new_fabric)

        # we keep the PASE session for 1 minute
        if session.is_PASE()
          session.set_expire_in_seconds(60)
        end

        # log("MTR: ------------------------------------------", 3)
        # log("MTR: session=" + matter.inspect(session), 3)
        # log("MTR: fabric=" + matter.inspect(session._fabric), 3)
        # log("MTR: ------------------------------------------", 3)
        new_fabric.log_new_fabric()        # log that we registered a new fabric
        new_fabric.assign_fabric_index()
        # create NOCResponse
        # 0=StatusCode
        # 1=FabricIndex (1-254) (opt)
        # 2=DebugText (opt)
        var nocr = TLV.Matter_TLV_struct()
        nocr.add_TLV(0, TLV.U1, matter.SUCCESS)   # Status
        nocr.add_TLV(1, TLV.U1, new_fabric.get_fabric_index())   # fabric-index
        ctx.command = 0x08              # NOCResponse
        return nocr

      elif command == 0x0009            # ---------- UpdateFabricLabel ----------
        var label = val.findsubval(0)     # Label string max 32
        session.set_fabric_label(label)
        log(format("MTR: .          Update fabric '%s' label='%s'", session._fabric.get_fabric_id().copy().reverse().tohex(), str(label)), 3)
        
        # create NOCResponse
        # 0=StatusCode
        # 1=FabricIndex (1-254) (opt)
        # 2=DebugText (opt)
        var nocr = TLV.Matter_TLV_struct()
        nocr.add_TLV(0, TLV.U1, matter.SUCCESS)   # Status
        nocr.add_TLV(1, TLV.U1, session.get_fabric().get_fabric_index())   # fabric-index
        ctx.command = 0x08              # NOCResponse
        return nocr

      elif command == 0x000A            # ---------- RemoveFabric ----------
        var index = val.findsubval(0)     # FabricIndex
        ctx.log = "fabric_index:"+str(index)

        for fab: self.device.sessions.active_fabrics()
          if fab.get_fabric_index() == index
            # log("MTR: removing fabric " + fab.get_fabric_id().copy().reverse().tohex(), 2)
            # defer actual removal to send a response
            fab.mark_for_deletion()       # this should not appear anymore in the list
            tasmota.set_timer(2000, def () self.device.remove_fabric(fab) end)

            # create NOCResponse
            # 0=StatusCode
            # 1=FabricIndex (1-254) (opt)
            # 2=DebugText (opt)
            var nocr = TLV.Matter_TLV_struct()
            nocr.add_TLV(0, TLV.U1, matter.SUCCESS)   # Status
            nocr.add_TLV(1, TLV.U1, index)   # fabric-index
            ctx.command = 0x08              # NOCResponse
            return nocr
          end
        end
        log("MTR: RemoveFabric fabric("+str(index)+") not found", 2)
        ctx.status = matter.INVALID_ACTION
        return nil                      # trigger a standalone ack

      end

    # ====================================================================================================
    elif cluster == 0x003C              # ========== Administrator Commissioning Cluster 11.18 p.725 ==========

      if   command == 0x0000          #  ---------- OpenCommissioningWindow  ----------
        var timeout = val.findsubval(0)             # CommissioningTimeout u2
        var passcode_verifier = val.findsubval(1)   # PAKEPasscodeVerifier octstr
        var discriminator = val.findsubval(2)       # Discriminator u2
        var iterations = val.findsubval(3)          # Iterations u4
        var salt = val.findsubval(4)                # Salt octstr

        log(format("MTR: OpenCommissioningWindow(timeout=%i, passcode=%s, discriminator=%i, iterations=%i, salt=%s)",
                                  timeout, passcode_verifier.tohex(), discriminator, iterations, salt.tohex()), 4)

        # check values
        if timeout == nil || passcode_verifier == nil || discriminator == nil || iterations == nil || salt == nil
          ctx.status = matter.INVALID_DATA_TYPE
          return nil                      # trigger a standalone ack
        end
        if size(passcode_verifier) != 32+65 || size(salt) < 16 || size(salt) > 32
          log("MTR: wrong size for PAKE parameters", 2)
          ctx.status = matter.CONSTRAINT_ERROR
          return nil                      # trigger a standalone ack
        end

        var w0 = passcode_verifier[0..31]
        var L  = passcode_verifier[32..]

        self.device.commissioning.start_basic_commissioning(timeout, iterations, discriminator, salt, w0, #-w1,-# L, session.get_fabric())
        # TODO announce in MDNS
        return true                   # OK
      elif command == 0x0001          #  ---------- OpenBasicCommissioningWindow  ----------
        var commissioning_timeout = val.findsubval(0)     # CommissioningTimeout
        log("MTR: OpenBasicCommissioningWindow commissioning_timeout="+str(commissioning_timeout), 3)
        self.device.commissioning.start_root_basic_commissioning(commissioning_timeout)
        return true
      elif command == 0x0002          #  ---------- RevokeCommissioning  ----------
        # TODO add checks that the commissioning window was opened by the same fabric
        self.device.commissioning.stop_basic_commissioning()
        return true
      end

    # ====================================================================================================
    elif cluster == 0x002A              # ========== OTA Software Update Requestor Cluster Definition 11.19.7 p.762 ==========

      if   command == 0x0000          #  ---------- DefaultOTAProviders  ----------
        return true                   # OK
      end
    
    else
      return super(self).invoke_request(session, val, ctx)
    end

  end

  #############################################################
  # write an attribute
  #
  def write_attribute(session, ctx, write_data)
    var TLV = matter.TLV
    var cluster = ctx.cluster
    var attribute = ctx.attribute
    
    # 0x001D no writable attributes
    # 0x0032 no attributes
    # 0x0033 no writable attributes
    # 0x0034 no writable attributes
    # 0x0038 no mandatory writable attributes
    # 0x003C no writable attributes
    # 0x003E no writable attributes

    if   cluster == 0x0030              # ========== GeneralCommissioning cluster 11.9 p.627 ==========

      if   attribute == 0x0000          # ---------- Breadcrumb ----------
        if type(write_data) == 'int' || isinstance(write_data, int64)
          session._breadcrumb = write_data
          self.attribute_updated(ctx.cluster, ctx.attribute)    # TODO should we have a more generalized way each time a write_attribute is triggered, declare the attribute as changed?
          return true
        else
          ctx.status = matter.CONSTRAINT_ERROR
          return false
        end
      end

    # ====================================================================================================
    elif cluster == 0x001F              # ========== Access Control Cluster 9.10 p.461 ==========
      if   attribute == 0x0000          # ACL - list[AccessControlEntryStruct]
        return true
      end

    # ====================================================================================================
    elif cluster == 0x0028              # ========== Basic Information Cluster cluster 11.1 p.565 ==========

      if   attribute == 0x0005          #  ---------- NodeLabel / string ----------
        # TODO
        return true
      elif attribute == 0x0006          #  ---------- Location / string ----------
        # TODO
        return true
      end
    # ====================================================================================================
    elif cluster == 0x002A              # ========== OTA Software Update Requestor Cluster Definition 11.19.7 p.762 ==========

      if   attribute == 0x0000          #  ---------- DefaultOTAProviders / list[ProviderLocationStruct] ----------
        return true                     # silently ignore
      end
    # ====================================================================================================
    elif cluster == 0x002B              # ========== Localization Configuration Cluster 11.3 p.580 ==========

      if   attribute == 0x0000          #  ---------- ActiveLocale / string ----------
        ctx.status = matter.CONSTRAINT_ERROR    # changing locale is not possible
        return false
      end
    # ====================================================================================================
    elif cluster == 0x002C              # ========== Time Format Localization Cluster 11.4 p.581 ==========

      if   attribute == 0x0000          #  ---------- HourFormat / HourFormat ----------
        # TODO
        return true
      elif attribute == 0x0001          #  ---------- ActiveCalendarType / CalendarType ----------
        # TODO
        return true
      end
    # ====================================================================================================
    elif cluster == 0x0031              # ========== Network Commissioning Cluster cluster 11.8 p.606 ==========
      if   attribute == 0x0004          #  ---------- InterfaceEnabled / bool ----------
        ctx.status = matter.INVALID_ACTION
        return false
      end


    end
  end
end
matter.Plugin_Root = Matter_Plugin_Root
  
