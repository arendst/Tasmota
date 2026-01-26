/*********************************************************************************\
* Compact form for events
*
\*********************************************************************************/
#include <stddef.h>
#include <stdint.h>

typedef struct {
  uint16_t    cluster;
  uint8_t     event;
  const char* name;
} matter_event_t;

// Must be sorted, cluster first, then attribute

const matter_event_t matter_Events[] = {
  // 0x001F Access Control Cluster
  { 0x001F, 0x00, "AccessControlEntryChanged" },
  { 0x001F, 0x00, "AccessControlExtensionChanged" },
  // 0x0025 Actions Cluster
  { 0x0025, 0x00, "StateChanged" },
  { 0x0025, 0x01, "ActionFailed" },
  // 0x0028 Basic Information Cluster
  { 0x0028, 0x00, "StartUp" },
  { 0x0028, 0x01, "ShutDown" },
  { 0x0028, 0x02, "Leave" },
  { 0x0028, 0x03, "ReachableChanged" },
  // 0x002F Power Source Cluster
  { 0x002F, 0x00, "WiredFaultChange" },
  { 0x002F, 0x01, "BatFaultChange" },
  { 0x002F, 0x02, "BatChargeFaultChange" },
  // 0x0033 General Diagnostics Cluster
  { 0x0033, 0x00, "HardwareFaultChange" },
  { 0x0033, 0x01, "RadioFaultChange" },
  { 0x0033, 0x02, "NetworkFaultChange" },
  { 0x0033, 0x03, "BootReason" },
  // 0x0034 Software Diagnostics Cluster
  { 0x0034, 0x00, "SoftwareFault" },
  // 0x0035 Thread Network Diagnostics Cluster
  // { 0x0035, 0x00, "ConnectionStatus" },
  // { 0x0035, 0x01, "NetworkFaultChange" },
  // 0x0036 Wi-Fi Network Diagnostics Cluster
  { 0x0036, 0x00, "Disconnection" },
  { 0x0036, 0x01, "AssociationFailure" },
  { 0x0036, 0x02, "ConnectionStatus" },
  // 0x0038 Time Synchronization Cluster
  { 0x0038, 0x00, "DSTTableEmpty" },
  { 0x0038, 0x01, "DSTStatus" },
  { 0x0038, 0x02, "TimeZoneStatus" },
  { 0x0038, 0x03, "TimeFailure" },
  { 0x0038, 0x04, "MissingTrustedTimeSource" },
  // 0x0039 Bridged Device Basic Information Cluster
  { 0x0039, 0x00, "StartUp" },
  { 0x0039, 0x01, "ShutDown" },
  { 0x0039, 0x02, "Leave" },
  { 0x0039, 0x03, "ReachableChanged" },
  { 0xFFFF, 0xFF, NULL }
};
