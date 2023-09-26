#pragma once

#include "xBaseInterface.h"



X_INTERFACE IxCore;

X_INTERFACE IxCommandManager;
X_INTERFACE IxCommandManagerOption;

X_INTERFACE IxDataCollector;

// Data
X_INTERFACE IxDnsCacheData;
X_INTERFACE IxDriverData;
X_INTERFACE IxFileData;
X_INTERFACE IxFileVersionData;
X_INTERFACE IxHashData;
X_INTERFACE IxImportTableData;
X_INTERFACE IxMetaData;
X_INTERFACE IxModuleData;
X_INTERFACE IxNetworkConnectionData;
X_INTERFACE IxOperatingSystemData;
X_INTERFACE IxPrefetchData;
X_INTERFACE IxPrefetchItemData;
X_INTERFACE IxProcessData;
X_INTERFACE IxRegistryKeyData;
X_INTERFACE IxRegistryValueData;
X_INTERFACE IxSignatureData;
X_INTERFACE IxTimeData;
X_INTERFACE IxUserAccountData;

// Collector
X_INTERFACE IxDnsCacheDataCollector;
X_INTERFACE IxDriverDataCollector;
X_INTERFACE IxFileDataCollector;
X_INTERFACE IxFileVersionDataCollector;
X_INTERFACE IxHashDataCollector;
X_INTERFACE IxImportTableDataCollector;
X_INTERFACE IxMetaDataCollector;
X_INTERFACE IxModuleDataCollector;
X_INTERFACE IxNetworkConnectionDataCollector;
X_INTERFACE IxOperatingSystemDataCollector;
X_INTERFACE IxPrefetchDataCollector;
X_INTERFACE IxProcessDataCollector;
X_INTERFACE IxRegistryDataCollector;
X_INTERFACE IxSignatureDataCollector;
X_INTERFACE IxTimeDataCollector;
X_INTERFACE IxUserAccountDataCollector;

// Enum
X_INTERFACE IxDnsCacheDataEnum;
X_INTERFACE IxDriverDataEnum;
X_INTERFACE IxImportTableDataEnum;
X_INTERFACE IxModuleDataEnum;
X_INTERFACE IxNetworkConnectionDataEnum;
X_INTERFACE IxPrefetchDataEnum;
X_INTERFACE IxPrefetchItemDataEnum;
X_INTERFACE IxProcessDataEnum;
X_INTERFACE IxRegistryKeyDataEnum;
X_INTERFACE IxRegistryValueDataEnum;
X_INTERFACE IxUserAccountDataEnum;

X_INTERFACE IxFormatter;

X_INTERFACE IxStorageProvider;




static const xGuid IID_IxCore =
{ 0x666dc0de, 0x2894, 0x4c51, { 0x95, 0x01, 0x83, 0x0a, 0x25, 0x7e, 0x4e, 0x8a } };

X_INTERFACE_DECLARE(IxCore)
{
	X_INTERFACE_METHOD(xResult, Init)() X_PURE;
	X_INTERFACE_METHOD(xResult, Shutdown)() X_PURE;

	X_INTERFACE_METHOD(xResult, AddLogWriter)(IxLogWriter* logWriter) X_PURE;

	X_INTERFACE_METHOD(xResult, CreateCommandManagerOption)(IxCommandManagerOption** commandManagerOption) X_PURE;
	X_INTERFACE_METHOD(xResult, CreateCommandManager)(IxCommandManager** commandManager) X_PURE;

	X_INTERFACE_METHOD(xResult, CreateDataCollector)(IxDataCollector** dataCollector) X_PURE;
};



static const xGuid IID_IxCommandManager =
{ 0x666dc0de, 0x6392, 0x4412, { 0xaf, 0x92, 0x2d, 0xa7, 0x31, 0x62, 0x23, 0x54 } };

X_INTERFACE_DECLARE(IxCommandManager)
{
	X_INTERFACE_METHOD(xResult, Execute)(IxCommandManagerOption* option) X_PURE;
};



enum xCommandManagerOptionFlag
{
	xCommandManagerOptionFlagCollect		= 0x00000001,
	xCommandManagerOptionFlagCollectQuick	= 0x00000002,
	xCommandManagerOptionFlagCollectFull	= 0x00000004,
	xCommandManagerOptionFlagLogMode		= 0x00000008,
	xCommandManagerOptionFlagLogFile		= 0x00000010,
	xCommandManagerOptionFlagOutput			= 0x00000020,
	xCommandManagerOptionFlagUpload			= 0x00000040,
	xCommandManagerOptionFlagNoNetwork		= 0x00000080,
	xCommandManagerOptionFlagHelp			= 0x00000100,
	xCommandManagerOptionFlagAbout			= 0x00000200
};

enum xCommandManagerCollectFlag
{
	// General
	xCommandManagerCollectFlagConnectionData		= 0x00000001,
	xCommandManagerCollectFlagDnsCacheData			= 0x00000002,
	xCommandManagerCollectFlagDriverData			= 0x00000004,
	xCommandManagerCollectFlagOperatingSystemData	= 0x00000008,
	xCommandManagerCollectFlagPrefetchData			= 0x00000010,
	xCommandManagerCollectFlagProcessData			= 0x00000020,
	xCommandManagerCollectFlagRegistryData			= 0x00000040,
	xCommandManagerCollectFlagUserAccountData		= 0x00000080,
	// Amalgamation
	xCommandManagerCollectFlagQuick					= xCommandManagerCollectFlagOperatingSystemData | xCommandManagerCollectFlagUserAccountData | xCommandManagerCollectFlagProcessData | xCommandManagerCollectFlagDriverData | xCommandManagerCollectFlagRegistryData,
	xCommandManagerCollectFlagFull					= xCommandManagerCollectFlagQuick | xCommandManagerCollectFlagPrefetchData | xCommandManagerCollectFlagConnectionData | xCommandManagerCollectFlagDnsCacheData
};

static const xGuid IID_IxCommandManagerOption =
{ 0x666dc0de, 0xc3c2, 0x4f35, { 0x89, 0xa6, 0xe7, 0xa5, 0x2d, 0x1a, 0xcb, 0x93 } };

X_INTERFACE_DECLARE_BASE(IxCommandManagerOption, IxPropertyEnum)
{
	X_INTERFACE_METHOD(xResult, SetFlags)(const uint32_t falgs) X_PURE;
	X_INTERFACE_METHOD(xResult, GetFlags)(uint32_t* flags) X_PURE;
	X_INTERFACE_METHOD(xResult, SetCollectFlags)(const uint32_t collectFlags) X_PURE;
	X_INTERFACE_METHOD(xResult, GetCollectFlags)(uint32_t* collectFlags) X_PURE;
	X_INTERFACE_METHOD(xResult, SetLogMode)(const uint32_t logMode) X_PURE;
	X_INTERFACE_METHOD(xResult, GetLogMode)(uint32_t* logMode) X_PURE;
	X_INTERFACE_METHOD(xResult, SetLogFileName)(const xChar* fileName) X_PURE;
	X_INTERFACE_METHOD(xResult, GetLogFileName)(xChar** fileName) X_PURE;
	X_INTERFACE_METHOD(xResult, SetOutputFileName)(const xChar* fileName) X_PURE;
	X_INTERFACE_METHOD(xResult, GetOutputFileName)(xChar** fileName) X_PURE;
	X_INTERFACE_METHOD(xResult, SetUploadUrl)(const xChar* url) X_PURE;
	X_INTERFACE_METHOD(xResult, GetUploadUrl)(xChar** url) X_PURE;
};



static const xGuid IID_IxDataCollector =
{ 0x666dc0de, 0x9b23, 0x4651, { 0x98, 0x73, 0x15, 0xea, 0x04, 0xb2, 0xea, 0x90 } };

X_INTERFACE_DECLARE(IxDataCollector)
{
	X_INTERFACE_METHOD(xResult, CollectDnsCacheData)(IxDnsCacheDataEnum** dnsCacheDataEnum, const bool offline) X_PURE;
	X_INTERFACE_METHOD(xResult, CollectDriverData)(IxDriverDataEnum** driverDataEnum) X_PURE;
	X_INTERFACE_METHOD(xResult, CollectFileVersionData)(const xChar* fileName, IxFileVersionData** fileVersionData) X_PURE;
	X_INTERFACE_METHOD(xResult, CollectMetaData)(IxMetaData** metaData) X_PURE;
	X_INTERFACE_METHOD(xResult, CollectNetworkConnectionData)(IxNetworkConnectionDataEnum** networkConnectionDataEnum) X_PURE;
	X_INTERFACE_METHOD(xResult, CollectPrefetchData)(IxPrefetchDataEnum** prefetchDataEnum) X_PURE;
	X_INTERFACE_METHOD(xResult, CollectProcessData)(IxProcessDataEnum** processDataEnum) X_PURE;
	X_INTERFACE_METHOD(xResult, CollectRegistryData)(IxRegistryKeyDataEnum** registryKeyDataEnum) X_PURE;
	X_INTERFACE_METHOD(xResult, CollectOperatingSystemData)(IxOperatingSystemData** operatingSystemData) X_PURE;
	X_INTERFACE_METHOD(xResult, CollectUserAccountData)(IxUserAccountDataEnum** userAccountDataEnum) X_PURE;
};



static const xGuid IID_IxDataContainer =
{ 0x666dc0de, 0xe5d2, 0x4a3e, { 0xb1, 0x39, 0x1a, 0x98, 0xc0, 0xf0, 0x96, 0x9b } };

X_INTERFACE_DECLARE_BASE(IxDataContainer, IxPropertyEnum)
{
	X_INTERFACE_METHOD(xResult, GetDnsCacheData)(IxDnsCacheDataEnum** dnsCacheDataEnum) X_PURE;
	X_INTERFACE_METHOD(xResult, GetDriverData)(IxDriverDataEnum** driverDataEnum) X_PURE;
	X_INTERFACE_METHOD(xResult, GetMetaData)(IxMetaData** metaData) X_PURE;
	X_INTERFACE_METHOD(xResult, GetNetworkConnectionData)(IxNetworkConnectionDataEnum** networkConnectionData) X_PURE;
	X_INTERFACE_METHOD(xResult, GetPrefetchData)(IxPrefetchDataEnum** prefetchDataEnum) X_PURE;
	X_INTERFACE_METHOD(xResult, GetProcessData)(IxProcessDataEnum** processDataEnum) X_PURE;
	X_INTERFACE_METHOD(xResult, GetRegistryData)(IxRegistryKeyDataEnum** registryDataEnum) X_PURE;
	X_INTERFACE_METHOD(xResult, GetOperatingSystemData)(IxOperatingSystemData** operatingSystemData) X_PURE;
	X_INTERFACE_METHOD(xResult, GetUserAccountData)(IxUserAccountDataEnum** userAccountDataEnum) X_PURE;
};



static const xGuid IID_IxApplicationCompatibilityCacheData =
{ 0x666dc0de, 0x66a5, 0x4fae, { 0xad, 0x95, 0xf0, 0x9e, 0x12, 0x8e, 0xc1, 0xe1 } };

X_INTERFACE_DECLARE_BASE(IxApplicationCompatibilityCacheData, IxPropertyEnum)
{
};

static const xGuid IID_IxDnsCacheData =
{ 0x666dc0de, 0x833a, 0x4f34, { 0x98, 0x01, 0x1e, 0x41, 0x60, 0x71, 0xcf, 0xef } };

X_INTERFACE_DECLARE_BASE(IxDnsCacheData, IxPropertyEnum)
{
	X_INTERFACE_METHOD(xResult, GetName)(xChar** name) X_PURE;
	X_INTERFACE_METHOD(xResult, GetType)(uint16_t* type) X_PURE;
	X_INTERFACE_METHOD(xResult, GetDataLength)(uint16_t* dataLength) X_PURE;
	X_INTERFACE_METHOD(xResult, GetFlags)(uint32_t* flags) X_PURE;
	X_INTERFACE_METHOD(xResult, GetAddress)(xChar** address) X_PURE;
	X_INTERFACE_METHOD(xResult, GetTimeToLive)(uint32_t* timeToLive) X_PURE;
};

static const xGuid IID_IxModuleData =
{ 0x666dc0de, 0xbd03, 0x4f37, { 0x9b, 0x70, 0x94, 0xc4, 0x98, 0xc0, 0x54, 0x29 } };

X_INTERFACE_DECLARE_BASE(IxModuleData, IxPropertyEnum)
{
	X_INTERFACE_METHOD(xResult, GetFileData)(IxFileData** fileData) X_PURE;
	X_INTERFACE_METHOD(xResult, GetFileVersionData)(IxFileVersionData** fileVersionData) X_PURE;
	X_INTERFACE_METHOD(xResult, GetImportTableData)(IxImportTableDataEnum** importTableDataEnum) X_PURE;
	X_INTERFACE_METHOD(xResult, GetSignatureData)(IxSignatureData** signatureData) X_PURE;
};

static const xGuid IID_IxDriverData =
{ 0x666dc0de, 0xc4a2, 0x456f, { 0x8a, 0xb9, 0x02, 0xb4, 0xb6, 0xfb, 0x7a, 0x1f } };

X_INTERFACE_DECLARE_BASE(IxDriverData, IxModuleData)
{
	X_INTERFACE_METHOD(xResult, GetNativeFileName)(xChar** nativeFileName) X_PURE;
};

static const xGuid IID_IxFileData =
{ 0x666dc0de, 0xb65f, 0x4726, { 0x90, 0xd3, 0x85, 0x0b, 0xa8, 0xad, 0xf2, 0x18 } };

X_INTERFACE_DECLARE_BASE(IxFileData, IxPropertyEnum)
{
	X_INTERFACE_METHOD(xResult, GetName)(xChar** name) X_PURE;
	X_INTERFACE_METHOD(xResult, GetFileName)(xChar** fileName) X_PURE;
	X_INTERFACE_METHOD(xResult, GetSize)(uint64_t* size) X_PURE;
	X_INTERFACE_METHOD(xResult, GetTimeData)(IxTimeData** time) X_PURE;
	X_INTERFACE_METHOD(xResult, GetHashData)(IxHashData** hash) X_PURE;
};

static const xGuid IID_IxFileVersionData =
{ 0x666dc0de, 0x2987, 0x4646, { 0x99, 0xfe, 0x99, 0x4d, 0xaa, 0x54, 0xc1, 0x50 } };

X_INTERFACE_DECLARE_BASE(IxFileVersionData, IxPropertyEnum)
{
	X_INTERFACE_METHOD(xResult, GetComments)(xChar** comments) X_PURE;
	X_INTERFACE_METHOD(xResult, GetInternalName)(xChar** internalName) X_PURE;
	X_INTERFACE_METHOD(xResult, GetProductName)(xChar** productName) X_PURE;
	X_INTERFACE_METHOD(xResult, GetCompanyName)(xChar** companyName) X_PURE;
	X_INTERFACE_METHOD(xResult, GetLegalCopyright)(xChar** legalCopyright) X_PURE;
	X_INTERFACE_METHOD(xResult, GetProductVersion)(xChar** productVersion) X_PURE;
	X_INTERFACE_METHOD(xResult, GetFileDescription)(xChar** fileDescription) X_PURE;
	X_INTERFACE_METHOD(xResult, GetLegalTrademarks)(xChar** legalTrademarks) X_PURE;
	X_INTERFACE_METHOD(xResult, GetPrivateBuild)(xChar** privateBuild) X_PURE;
	X_INTERFACE_METHOD(xResult, GetFileVersion)(xChar** fileVersion) X_PURE;
	X_INTERFACE_METHOD(xResult, GetOriginalFileName)(xChar** originalFileName) X_PURE;
	X_INTERFACE_METHOD(xResult, GetSpecialBuild)(xChar** specialBuild) X_PURE;
};

static const xGuid IID_IxHashData =
{ 0x666dc0de, 0xfc2e, 0x43bb, { 0x95, 0xc6, 0x64, 0x5b, 0x4f, 0xbb, 0x45, 0x1a } };

X_INTERFACE_DECLARE_BASE(IxHashData, IxPropertyEnum)
{
	X_INTERFACE_METHOD(xResult, GetMd5)(xChar** md5) X_PURE;
	X_INTERFACE_METHOD(xResult, GetSha1)(xChar** sha1) X_PURE;
	X_INTERFACE_METHOD(xResult, GetSha256)(xChar** sha256) X_PURE;
};

static const xGuid IID_IxImportTableData =
{ 0x666dc0de, 0x8ab7, 0x4233, { 0xaa, 0x1c, 0x2f, 0x48, 0xf4, 0xdd, 0x81, 0x50 } };

X_INTERFACE_DECLARE_BASE(IxImportTableData, IxPropertyEnum)
{
	X_INTERFACE_METHOD(xResult, GetModuleName)(xChar** moduleName) X_PURE;
	X_INTERFACE_METHOD(xResult, GetFunctions)(IxStringEnum** functions) X_PURE;
};

static const xGuid IID_IxMetaData =
{ 0x666dc0de, 0x1f85, 0x4c56, { 0x99, 0x77, 0x9f, 0x43, 0x77, 0x34, 0x87, 0x3e } };

X_INTERFACE_DECLARE_BASE(IxMetaData, IxPropertyEnum)
{
	X_INTERFACE_METHOD(xResult, GetCreated)(time_t* created) X_PURE;
	X_INTERFACE_METHOD(xResult, GetElevated)(bool* elevated) X_PURE;
	X_INTERFACE_METHOD(xResult, GetGuid)(xGuid* guid) X_PURE;
	X_INTERFACE_METHOD(xResult, GetVersion)(uint32_t* version) X_PURE;
};

enum xNetworkConnectionType
{
	xNetworkConnectionTypeUnknown	= 0x00,
	xNetworkConnectionTypeTcp4		= 0x01,
	xNetworkConnectionTypeTcp6		= 0x02,
	xNetworkConnectionTypeUdp4		= 0x03,
	xNetworkConnectionTypeUdp6		= 0x04
};

static const xGuid IID_IxNetworkConnectionData =
{ 0x666dc0de, 0xc4a2, 0x456f, { 0x8a, 0xb9, 0x02, 0xb4, 0xb6, 0xfb, 0x7a, 0x1f } };

X_INTERFACE_DECLARE_BASE(IxNetworkConnectionData, IxPropertyEnum)
{
	X_INTERFACE_METHOD(xResult, GetType)(uint32_t* type) X_PURE;
	X_INTERFACE_METHOD(xResult, GetTypeString)(xChar** type) X_PURE;
	X_INTERFACE_METHOD(xResult, GetState)(xChar** state) X_PURE;
	X_INTERFACE_METHOD(xResult, GetLocalAddress)(xChar** address) X_PURE;
	X_INTERFACE_METHOD(xResult, GetLocalHost)(xChar** host) X_PURE;
	X_INTERFACE_METHOD(xResult, GetLocalPort)(uint16_t* port) X_PURE;
	X_INTERFACE_METHOD(xResult, GetRemoteAddress)(xChar** address) X_PURE;
	X_INTERFACE_METHOD(xResult, GetRemoteHost)(xChar** host) X_PURE;
	X_INTERFACE_METHOD(xResult, GetRemotePort)(uint16_t* port) X_PURE;
	X_INTERFACE_METHOD(xResult, GetOwningProcess)(IxProcessData** processData) X_PURE;
};

static const xGuid IID_IxOperatingSystemData =
{ 0x666dc0de, 0x1b2c, 0x4c15, { 0x8a, 0x50, 0x8f, 0xd3, 0x38, 0xbe, 0xdb, 0x03 } };

X_INTERFACE_DECLARE_BASE(IxOperatingSystemData, IxPropertyEnum)
{
	X_INTERFACE_METHOD(xResult, GetFullyQualifiedDomainName)(xChar** fullyQualifiedDomainName) X_PURE;
	X_INTERFACE_METHOD(xResult, GetBootDevice)(xChar** bootDevice) X_PURE;
	X_INTERFACE_METHOD(xResult, GetBuildNumber)(xChar** buildNumber) X_PURE;
	X_INTERFACE_METHOD(xResult, GetBuildType)(xChar** buildType) X_PURE;
	X_INTERFACE_METHOD(xResult, GetCaption)(xChar** caption) X_PURE;
	X_INTERFACE_METHOD(xResult, GetCodeSet)(xChar** codeSet) X_PURE;
	X_INTERFACE_METHOD(xResult, GetCountryCode)(xChar** countryCode) X_PURE;
	X_INTERFACE_METHOD(xResult, GetCsdVersion)(xChar** csdVersion) X_PURE;
	X_INTERFACE_METHOD(xResult, GetCurrentTimeZone)(int16_t* currentTimeZone) X_PURE;
	X_INTERFACE_METHOD(xResult, GetDataExecutionPrevention32BitApplications)(bool* dataExecutionPrevention32BitApplications) X_PURE;
	X_INTERFACE_METHOD(xResult, GetDataExecutionPreventionAvailable)(bool* dataExecutionPreventionAvailable) X_PURE;
	X_INTERFACE_METHOD(xResult, GetDataExecutionPreventionSupportPolicy)(uint8_t* dataExecutionPreventionSupportPolicy) X_PURE;
	X_INTERFACE_METHOD(xResult, GetDataExecutionPreventionDrivers)(bool* dataExecutionPreventionDrivers) X_PURE;
	X_INTERFACE_METHOD(xResult, GetDataExecutionPreventionSupportPolicyString)(xChar** dataExecutionPreventionSupportPolicyString) X_PURE;
	X_INTERFACE_METHOD(xResult, GetDistributed)(bool* distributed) X_PURE;
	X_INTERFACE_METHOD(xResult, GetEncryptionLevel)(uint32_t* encryptionLevel) X_PURE;
	X_INTERFACE_METHOD(xResult, GetInstallDate)(time_t* installDate) X_PURE;
	X_INTERFACE_METHOD(xResult, GetLastBootUpTime)(time_t* lastBootTime) X_PURE;
	X_INTERFACE_METHOD(xResult, GetLocale)(xChar** locale) X_PURE;
	X_INTERFACE_METHOD(xResult, GetName)(xChar** name) X_PURE;
	X_INTERFACE_METHOD(xResult, GetNumberOfUsers)(uint32_t* numberOfUsers) X_PURE;
	X_INTERFACE_METHOD(xResult, GetOperatingSystemSku)(uint32_t* edition) X_PURE;
	X_INTERFACE_METHOD(xResult, GetOperatingSystemSkuString)(xChar** edition) X_PURE;
	X_INTERFACE_METHOD(xResult, GetOrganization)(xChar** organization) X_PURE;
	X_INTERFACE_METHOD(xResult, GetOsArchitecture)(xChar** osArchitecture) X_PURE;
	X_INTERFACE_METHOD(xResult, GetPortableOperatingSystem)(bool* portableOperatingSystem) X_PURE;
	X_INTERFACE_METHOD(xResult, GetProductType)(uint32_t* productType) X_PURE;
	X_INTERFACE_METHOD(xResult, GetProductTypeString)(xChar** productTypeString) X_PURE;
	X_INTERFACE_METHOD(xResult, GetPrimary)(bool* primary) X_PURE;
	X_INTERFACE_METHOD(xResult, GetRegisteredUser)(xChar** registeredUser) X_PURE;
	X_INTERFACE_METHOD(xResult, GetSerialNumber)(xChar** serialNumber) X_PURE;
	X_INTERFACE_METHOD(xResult, GetServicePackMajorVersion)(uint16_t* servicePackMajorVersion) X_PURE;
	X_INTERFACE_METHOD(xResult, GetServicePackMinorVersion)(uint16_t* servicePackMinorVersion) X_PURE;
	X_INTERFACE_METHOD(xResult, GetSuiteMask)(uint32_t* suiteMask) X_PURE;
	X_INTERFACE_METHOD(xResult, GetSystemDevice)(xChar** systemDevice) X_PURE;
	X_INTERFACE_METHOD(xResult, GetSystemDrive)(xChar** systemDrive) X_PURE;
	X_INTERFACE_METHOD(xResult, GetSystemDirectory)(xChar** systemDirectory) X_PURE;
	X_INTERFACE_METHOD(xResult, GetVersion)(xChar** version) X_PURE;
	X_INTERFACE_METHOD(xResult, GetWindowsDirectory)(xChar** windowsDirectory) X_PURE;
};

static const xGuid IID_IxPrefetchData =
{ 0x666dc0de, 0xd1f2, 0x42fe, { 0x83, 0x65, 0xea, 0x28, 0x79, 0xda, 0x27, 0x38 } };

X_INTERFACE_DECLARE_BASE(IxPrefetchData, IxPropertyEnum)
{
	X_INTERFACE_METHOD(xResult, GetFileName)(xChar** fileName) X_PURE;
	X_INTERFACE_METHOD(xResult, GetExecutableFileName)(xChar** executableFileName) X_PURE;
	X_INTERFACE_METHOD(xResult, GetHash)(xChar** hash) X_PURE;
	X_INTERFACE_METHOD(xResult, GetLastRunTimes)(IxTimeEnum** lastRunTimes) X_PURE;
	X_INTERFACE_METHOD(xResult, GetItemData)(IxPrefetchItemDataEnum** prefetchItemDataEnum) X_PURE;
};

static const xGuid IID_IxPrefetchItemData =
{ 0x666dc0de, 0xe84e, 0x4cea,{ 0xb5, 0x18, 0x02, 0x59, 0xaa, 0x02, 0x9f, 0x98 } };

X_INTERFACE_DECLARE_BASE(IxPrefetchItemData, IxPropertyEnum)
{
	X_INTERFACE_METHOD(xResult, GetNativeFileName)(xChar** nativeFleName) X_PURE;
	X_INTERFACE_METHOD(xResult, GetItem)(IxInterface** item) X_PURE;
};

static const xGuid IID_IxProcessData =
{ 0x666dc0de, 0x2c14, 0x43ed, { 0x88, 0x76, 0x93, 0xbf, 0xde, 0xee, 0xe7, 0x07 } };

X_INTERFACE_DECLARE_BASE(IxProcessData, IxModuleData)
{
	X_INTERFACE_METHOD(xResult, GetId)(uint32_t* id) X_PURE;
	X_INTERFACE_METHOD(xResult, GetSubsystem)(xChar** subsystem) X_PURE;
	X_INTERFACE_METHOD(xResult, GetParentId)(uint32_t* parentId) X_PURE;
	X_INTERFACE_METHOD(xResult, GetParent)(xChar** fileName) X_PURE;
	X_INTERFACE_METHOD(xResult, GetModuleData)(IxModuleDataEnum** moduleDataEnum) X_PURE;
};

static const xGuid IID_IxRegistryKeyData =
{ 0x666dc0de, 0x1f64, 0x4b65, { 0x8d, 0xd9, 0x90, 0x34, 0x9e, 0xa4, 0x2f, 0x86 } };

X_INTERFACE_DECLARE_BASE(IxRegistryKeyData, IxPropertyEnum)
{
	X_INTERFACE_METHOD(xResult, GetRootKey)(xChar** rootKey) X_PURE;
	X_INTERFACE_METHOD(xResult, GetPath)(xChar** path) X_PURE;
	X_INTERFACE_METHOD(xResult, GetLastModified)(time_t* lastModified) X_PURE;
	X_INTERFACE_METHOD(xResult, GetLastModifiedString)(xChar** lastModifiedString) X_PURE;
	X_INTERFACE_METHOD(xResult, GetKeyData)(IxRegistryKeyDataEnum** registryKeyDataEnum) X_PURE;
	X_INTERFACE_METHOD(xResult, GetValueData)(IxRegistryValueDataEnum** registryValueDataEnum) X_PURE;
};

static const xGuid IID_IxRegistryValueData =
{ 0x666dc0de, 0x3166, 0x4142, { 0xb3, 0x52, 0x0a, 0x18, 0x40, 0xce, 0x34, 0xd8 } };

X_INTERFACE_DECLARE_BASE(IxRegistryValueData, IxPropertyEnum)
{
	X_INTERFACE_METHOD(xResult, GetRootKey)(xChar** rootKey) X_PURE;
	X_INTERFACE_METHOD(xResult, GetPath)(xChar** path) X_PURE;
	X_INTERFACE_METHOD(xResult, GetName)(xChar** name) X_PURE;
	X_INTERFACE_METHOD(xResult, GetType)(uint32_t* type) X_PURE;
	X_INTERFACE_METHOD(xResult, GetTypeString)(xChar** typeString) X_PURE;
	X_INTERFACE_METHOD(xResult, GetBinaryValue)(xChar** value) X_PURE;
	X_INTERFACE_METHOD(xResult, GetDwordValue)(uint32_t* value) X_PURE;
	X_INTERFACE_METHOD(xResult, GetBigEndianDwordValue)(uint32_t* value) X_PURE;
	X_INTERFACE_METHOD(xResult, GetEnvironmentStringValue)(xChar** value) X_PURE;
	X_INTERFACE_METHOD(xResult, GetLinkValue)(xChar** value) X_PURE;
	X_INTERFACE_METHOD(xResult, GetMultiStringValue)(IxStringEnum** value) X_PURE;
	X_INTERFACE_METHOD(xResult, GetQwordValue)(uint64_t* value) X_PURE;
	X_INTERFACE_METHOD(xResult, GetStringValue)(xChar** value) X_PURE;
	X_INTERFACE_METHOD(xResult, GetFileData)(IxFileData** fileData) X_PURE;
	X_INTERFACE_METHOD(xResult, GetModuleData)(IxModuleData** moduleData) X_PURE;
};


static const xGuid IID_IxSignatureData =
{ 0x666dc0de, 0xe41d, 0x431d, { 0xa1, 0x26, 0xab, 0x3f, 0xed, 0x77, 0x22, 0x3d } };

X_INTERFACE_DECLARE_BASE(IxSignatureData, IxPropertyEnum)
{
	X_INTERFACE_METHOD(xResult, GetState)(uint32_t* state) X_PURE;
	X_INTERFACE_METHOD(xResult, GetStateString)(xChar** state) X_PURE;
	X_INTERFACE_METHOD(xResult, GetSigner)(xChar** signer) X_PURE;
};

static const xGuid IID_IxTimeData =
{ 0x666dc0de, 0x0a68, 0x46ab, { 0x93, 0xb0, 0x62, 0x2d, 0x08, 0x5e, 0xf0, 0x66 } };

X_INTERFACE_DECLARE_BASE(IxTimeData, IxPropertyEnum)
{
	X_INTERFACE_METHOD(xResult, GetCreated)(time_t* created) X_PURE;
	X_INTERFACE_METHOD(xResult, GetLastModified)(time_t* lastModified) X_PURE;
	X_INTERFACE_METHOD(xResult, GetLastAccessed)(time_t* lastAccessed) X_PURE;
};

static const xGuid IID_IxUserAccountData =
{ 0x666dc0de, 0x9073, 0x475d, { 0x91, 0x45, 0xc9, 0x88, 0xf1, 0x1c, 0xc9, 0xd4 } };

X_INTERFACE_DECLARE_BASE(IxUserAccountData, IxPropertyEnum)
{
	X_INTERFACE_METHOD(xResult, GetName)(xChar** name) X_PURE;
	X_INTERFACE_METHOD(xResult, GetPasswordAge)(uint32_t* passwordAge) X_PURE;
	X_INTERFACE_METHOD(xResult, GetPrivileges)(uint32_t* privileges) X_PURE;
	X_INTERFACE_METHOD(xResult, GetPrivilegesString)(xChar** privilegesString) X_PURE;
	X_INTERFACE_METHOD(xResult, GetHomeDirectory)(xChar** homeDirectory) X_PURE;
	X_INTERFACE_METHOD(xResult, GetControlFlags)(uint32_t* controlFlags) X_PURE;
	X_INTERFACE_METHOD(xResult, GetControlFlagsStringEnum)(IxStringEnum** controlFlagsStringEnum) X_PURE;
	X_INTERFACE_METHOD(xResult, GetScriptPath)(xChar** scriptPath) X_PURE;
	X_INTERFACE_METHOD(xResult, GetAuthenticationFlags)(uint32_t* authenticationFlags) X_PURE;
	X_INTERFACE_METHOD(xResult, GetAuthenticationFlagsStringEnum)(IxStringEnum** authenticationFlagsStringEnum) X_PURE;
	X_INTERFACE_METHOD(xResult, GetFullName)(xChar** fullName) X_PURE;
	X_INTERFACE_METHOD(xResult, GetLastLogon)(time_t* lastLogon) X_PURE;
	X_INTERFACE_METHOD(xResult, GetAccountExpires)(time_t* accountExpires) X_PURE;
	X_INTERFACE_METHOD(xResult, GetBadPasswordCount)(uint32_t* badPasswordCount) X_PURE;
	X_INTERFACE_METHOD(xResult, GetLogonCount)(uint32_t* logonCount) X_PURE;
	X_INTERFACE_METHOD(xResult, GetLogonServer)(xChar** logonServer) X_PURE;
	X_INTERFACE_METHOD(xResult, GetCountryCode)(uint32_t* countryCode) X_PURE;
	X_INTERFACE_METHOD(xResult, GetCodePage)(uint32_t* codePage) X_PURE;
	X_INTERFACE_METHOD(xResult, GetUserId)(uint32_t* userId) X_PURE;
	X_INTERFACE_METHOD(xResult, GetPrimaryGroupId)(uint32_t* primaryGroupId) X_PURE;
	X_INTERFACE_METHOD(xResult, GetProfile)(xChar** userProfile) X_PURE;
	X_INTERFACE_METHOD(xResult, GetHomeDirectoryDrive)(xChar** homeDirectoryDrive) X_PURE;
	X_INTERFACE_METHOD(xResult, GetPasswordExpired)(uint32_t* passwordExpired) X_PURE;
};



/*
static const xGuid IID_IxNetworkData =
{ 0x666dc0de, 0x48b5, 0x4305, { 0x91, 0x16, 0x76, 0xdf, 0xab, 0xa0, 0xf2, 0x69 } };

X_INTERFACE_DECLARE(IxNetworkData)
{
	X_INTERFACE_METHOD(xResult, GetHost)(xChar** host) X_PURE;
	X_INTERFACE_METHOD(xResult, GetDomain)(xChar** domain) X_PURE;
	X_INTERFACE_METHOD(xResult, GetNodeType)(uint32_t* nodeType) X_PURE;
	X_INTERFACE_METHOD(xResult, GetNodeType)(xChar** nodeType) X_PURE;
	X_INTERFACE_METHOD(xResult, GetScopeId)(xChar** scopeId) X_PURE;
	X_INTERFACE_METHOD(xResult, GetRoutingEnabled)(bool* routingEnabled) X_PURE;
	X_INTERFACE_METHOD(xResult, GetArpProxyEnabled)(bool* arpProxyEnabled) X_PURE;
	X_INTERFACE_METHOD(xResult, GetDnsEnabled)(bool* arpProxyEnabled) X_PURE;
	X_INTERFACE_METHOD(xResult, GetDnsServerData)(IxDnsServerDataEnum** dnsServerDataEnum) X_PURE;
};
*/


static const xGuid IID_IxApplicationCompatibilityCacheDataCollector =
{ 0x666dc0de, 0x979d, 0x429a, { 0xa1, 0x83, 0xc2, 0x12, 0x83, 0xdc, 0x6d, 0x37 } };

X_INTERFACE_DECLARE(IxApplicationCompatibilityCacheDataCollector)
{
	X_INTERFACE_METHOD(xResult, Collect)(IxApplicationCompatibilityCacheData** applicationCompatibilityCacheData) X_PURE;
};

static const xGuid IID_IxDnsCacheDataCollector =
{ 0x666dc0de, 0x58fb, 0x422c, { 0xa0, 0x22, 0xb7, 0x6c, 0x2a, 0xb, 0x31, 0xa3 } };

X_INTERFACE_DECLARE(IxDnsCacheDataCollector)
{
	X_INTERFACE_METHOD(xResult, Collect)(IxDnsCacheDataEnum** dnsCacheDataEnum, const bool offline) X_PURE;
};

static const xGuid IID_IxDriverDataCollector =
{ 0x666dc0de, 0x2371, 0x4f85, { 0xb6, 0xf6, 0xaf, 0x72, 0xbb, 0x6c, 0xf2, 0x46 } };

X_INTERFACE_DECLARE(IxDriverDataCollector)
{
	X_INTERFACE_METHOD(xResult, Collect)(IxDriverDataEnum** driverDataEnum) X_PURE;
};

static const xGuid IID_IxFileDataCollector =
{ 0x666dc0de, 0x901f, 0x4901, { 0x9b, 0xbc, 0x69, 0x23, 0x2e, 0xf3, 0xc9, 0xb7 } };

X_INTERFACE_DECLARE(IxFileDataCollector)
{
	X_INTERFACE_METHOD(xResult, Collect)(const xChar* fileName, IxFileData** fileVersionData) X_PURE;
};

static const xGuid IID_IxFileVersionDataCollector =
{ 0x666dc0de, 0x302d, 0x45e4, { 0xb9, 0x23, 0x10, 0x8d, 0xcd, 0x24, 0x35, 0x4f } };

X_INTERFACE_DECLARE(IxFileVersionDataCollector)
{
	X_INTERFACE_METHOD(xResult, Collect)(const xChar* fileName, IxFileVersionData** fileVersionData) X_PURE;
};

static const xGuid IID_IxHashDataCollector =
{ 0x666dc0de, 0xa299, 0x4b2d, { 0x8d, 0x55, 0x55, 0x7d, 0x71, 0x7c, 0xa5, 0x41 } };

X_INTERFACE_DECLARE(IxHashDataCollector)
{
	X_INTERFACE_METHOD(xResult, Collect)(const xChar* fileName, IxHashData** hashData) X_PURE;
};

static const xGuid IID_IxImportTableDataCollector =
{ 0x666dc0de, 0xa228, 0x448e, { 0x90, 0x96, 0x86, 0x52, 0x68, 0x82, 0x23, 0xa8 } };

X_INTERFACE_DECLARE(IxImportTableDataCollector)
{
	X_INTERFACE_METHOD(xResult, Collect)(const xChar* fileName, IxImportTableDataEnum** importTableDataEnum) X_PURE;
};

static const xGuid IID_IxMetaDataCollector =
{ 0x666dc0de, 0xdfad, 0x4230, { 0x87, 0x5c, 0xef, 0xec, 0x18, 0x82, 0x79, 0x2 } };

X_INTERFACE_DECLARE(IxMetaDataCollector)
{
	X_INTERFACE_METHOD(xResult, Collect)(IxMetaData** metaData) X_PURE;
};

static const xGuid IID_IxModuleDataCollector =
{ 0x666dc0de, 0xf63a, 0x49b0, { 0xbd, 0xf0, 0xdb, 0x4e, 0x23, 0x8b, 0xf3, 0x5c } };

X_INTERFACE_DECLARE(IxModuleDataCollector)
{
	X_INTERFACE_METHOD(xResult, Collect)(const xChar* fileName, IxModuleData** moduleData) X_PURE;
};

static const xGuid IID_IxNetworkConnectionDataCollector =
{ 0x666dc0de, 0x9cb0, 0x4b95, { 0x94, 0x19, 0x9c, 0xe4, 0x61, 0xae, 0xb4, 0x2f } };

X_INTERFACE_DECLARE(IxNetworkConnectionDataCollector)
{
	X_INTERFACE_METHOD(xResult, Collect)(IxNetworkConnectionDataEnum** networkConnectionDataEnum) X_PURE;
};

static const xGuid IID_IxOperatingSystemDataCollector =
{ 0x666dc0de, 0x2a68, 0x4dd3, { 0xb5, 0x5e, 0x36, 0x85, 0x2c, 0x57, 0xc0, 0xb8 } };

X_INTERFACE_DECLARE(IxOperatingSystemDataCollector)
{
	X_INTERFACE_METHOD(xResult, Collect)(IxOperatingSystemData** operatingSystemData) X_PURE;
};

static const xGuid IID_IxPrefetchDataCollector =
{ 0x666dc0de, 0x5afa, 0x4881, { 0x90, 0x28, 0x28, 0x6f, 0xe5, 0x08, 0x27, 0xcb } };

X_INTERFACE_DECLARE(IxPrefetchDataCollector)
{
	X_INTERFACE_METHOD(xResult, Collect)(IxPrefetchDataEnum** prefetchDataEnum) X_PURE;
};

static const xGuid IID_IxProcessDataCollector =
{ 0x666dc0de, 0xf7a5, 0x4fd6, { 0x9f, 0x4f, 0x62, 0x75, 0x3a, 0xa0, 0x9f, 0x6c } };

X_INTERFACE_DECLARE(IxProcessDataCollector)
{
	X_INTERFACE_METHOD(xResult, Collect)(IxProcessDataEnum** processDataEnum) X_PURE;
	X_INTERFACE_METHOD(xResult, Collect)(const uint32_t processId, IxProcessData** processData) X_PURE;
};

static const xGuid IID_IxRegistryDataCollector =
{ 0x666dc0de, 0xb650, 0x44bc, { 0xb8, 0x90, 0x48, 0x6, 0xa7, 0x5f, 0x7a, 0x27 } };

X_INTERFACE_DECLARE(IxRegistryDataCollector)
{
	X_INTERFACE_METHOD(xResult, Collect)(IxRegistryKeyDataEnum** registryKeyDataEnum) X_PURE;
};

static const xGuid IID_IxSignatureDataCollector =
{ 0x666dc0de, 0x689a, 0x434b, { 0xbe, 0x4a, 0xfb, 0x73, 0x1a, 0x1, 0xea, 0x9b } };

X_INTERFACE_DECLARE(IxSignatureDataCollector)
{
	X_INTERFACE_METHOD(xResult, Collect)(const xChar* fileName, IxSignatureData** signatureData) X_PURE;
};

static const xGuid IID_IxTimeDataCollector =
{ 0x666dc0de, 0xa299, 0x4b2d, { 0x8d, 0x55, 0x55, 0x7d, 0x71, 0x7c, 0xa5, 0x41 } };

X_INTERFACE_DECLARE(IxTimeDataCollector)
{
	X_INTERFACE_METHOD(xResult, Collect)(const xChar* fileName, IxTimeData** timeData) X_PURE;
};

static const xGuid IID_IxUserAccountDataCollector =
{ 0x666dc0de, 0x7d6d, 0x43fe, { 0xbc, 0x44, 0xa6, 0x77, 0x89, 0x6e, 0xc2, 0xff } };

X_INTERFACE_DECLARE(IxUserAccountDataCollector)
{
	X_INTERFACE_METHOD(xResult, Collect)(IxUserAccountDataEnum** userAccountDataEnum) X_PURE;
};



static const xGuid IID_IxDnsCacheDataEnum =
{ 0x666dc0de, 0x8aa2, 0x4b0f, { 0xa0, 0x9d, 0x83, 0x94, 0x5c, 0x22, 0x2b, 0xe8 } };

X_INTERFACE_DECLARE_BASE(IxDnsCacheDataEnum, IxPropertyEnum)
{
	X_INTERFACE_METHOD(xResult, Get)(const uint32_t index, IxDnsCacheData** dnsCacheData) X_PURE;
};

static const xGuid IID_IxDriverDataEnum =
{ 0x666dc0de, 0x4696, 0x45e1, { 0xbf, 0x76, 0xc0, 0xfc, 0x22, 0xdf, 0x5c, 0x40 } };

X_INTERFACE_DECLARE_BASE(IxDriverDataEnum, IxPropertyEnum)
{
	X_INTERFACE_METHOD(xResult, Get)(const uint32_t index, IxDriverData** driverData) X_PURE;
};

static const xGuid IID_IxImportTableDataEnum =
{ 0x666dc0de, 0x100e, 0x4e94, { 0xb2, 0x10, 0xf1, 0x1c, 0x17, 0xf0, 0x0c, 0x0a } };

X_INTERFACE_DECLARE_BASE(IxImportTableDataEnum, IxPropertyEnum)
{
	X_INTERFACE_METHOD(xResult, Get)(const uint32_t index, IxImportTableData** împortTableData) X_PURE;
};

static const xGuid IID_IxModuleDataEnum =
{ 0x666dc0de, 0xb2fa, 0x47a2, { 0xb6, 0x9e, 0x01, 0xa9, 0xdb, 0x56, 0xdf, 0x48 } };

X_INTERFACE_DECLARE_BASE(IxModuleDataEnum, IxPropertyEnum)
{
	X_INTERFACE_METHOD(xResult, Get)(const uint32_t index, IxModuleData** moduleData) X_PURE;
};
/*
static const xGuid IID_IxNetworkAdapterDataEnum =
{ 0x666dc0de, 0x6af2, 0x43e4, { 0x95, 0xa8, 0x21, 0x1c, 0xe9, 0x87, 0xf6, 0x80 } };

X_INTERFACE_DECLARE_BASE(IxNetworkAdapterDataEnum, IxInterfaceEnum)
{
	X_INTERFACE_METHOD(xResult, Get)(const uint32_t index, IxNetworkAdapterData** networkAdapterData) X_PURE;
};
*/
static const xGuid IID_IxNetworkConnectionDataEnum =
{ 0x666dc0de, 0x3ca9, 0x40c7, { 0xb0, 0x5d, 0xa2, 0x24, 0x2d, 0xb3, 0x89, 0x39 } };

X_INTERFACE_DECLARE_BASE(IxNetworkConnectionDataEnum, IxPropertyEnum)
{
	X_INTERFACE_METHOD(xResult, Get)(const uint32_t index, IxNetworkConnectionData** networkConnectionData) X_PURE;
};

static const xGuid IID_IxPrefetchDataEnum =
{ 0x666dc0de, 0x56a8, 0x428c, { 0x8a, 0xf7, 0xe1, 0x27, 0x50, 0x86, 0xef, 0x51 } };

X_INTERFACE_DECLARE_BASE(IxPrefetchDataEnum, IxPropertyEnum)
{
	X_INTERFACE_METHOD(xResult, Get)(const uint32_t index, IxPrefetchData** prefetchData) X_PURE;
	X_INTERFACE_METHOD(bool, Contains)(const xChar* fileName, IxPrefetchItemData** prefetchItemData) X_PURE;
};

static const xGuid IID_IxPrefetchItemDataEnum =
{ 0x666dc0de, 0xd6f7, 0x4d19,{ 0x91, 0xbc, 0xba, 0xb0, 0xcb, 0x57, 0x79, 0x2f } };

X_INTERFACE_DECLARE_BASE(IxPrefetchItemDataEnum, IxPropertyEnum)
{
	X_INTERFACE_METHOD(xResult, Get)(const uint32_t index, IxPrefetchItemData** prefetchItemData) X_PURE;
	X_INTERFACE_METHOD(bool, Contains)(const xChar* fileName, IxPrefetchItemData** prefetchItemData) X_PURE;
};

static const xGuid IID_IxProcessDataEnum =
{ 0x666dc0de, 0xb162, 0x4b30, { 0x87, 0x80, 0x75, 0xe5, 0x58, 0xe9, 0x4b, 0x0a } };

X_INTERFACE_DECLARE_BASE(IxProcessDataEnum, IxPropertyEnum)
{
	X_INTERFACE_METHOD(xResult, Get)(const uint32_t index, IxProcessData** processData) X_PURE;
};

static const xGuid IID_IxRegistryKeyDataEnum =
{ 0x666dc0de, 0x8651, 0x4a12, { 0x82, 0x82, 0xbd, 0x65, 0xa0, 0xcf, 0x3d, 0x56 } };

X_INTERFACE_DECLARE_BASE(IxRegistryKeyDataEnum, IxPropertyEnum)
{
	X_INTERFACE_METHOD(xResult, Get)(const uint32_t index, IxRegistryKeyData** registryKeyData) X_PURE;
};

static const xGuid IID_IxRegistryValueDataEnum =
{ 0x666dc0de, 0x0cfa, 0x43e7, { 0x8f, 0x69, 0x3b, 0xde, 0x97, 0xbb, 0xc4, 0xf0 } };

X_INTERFACE_DECLARE_BASE(IxRegistryValueDataEnum, IxPropertyEnum)
{
	X_INTERFACE_METHOD(xResult, Get)(const uint32_t index, IxRegistryValueData** registryValueData) X_PURE;
};

static const xGuid IID_IxUserAccountDataEnum =
{ 0x666dc0de, 0xccc0, 0x40d5, { 0xb3, 0x8c, 0x16, 0x55, 0x6c, 0x19, 0x37, 0xed } };

X_INTERFACE_DECLARE_BASE(IxUserAccountDataEnum, IxPropertyEnum)
{
	X_INTERFACE_METHOD(xResult, Get)(const uint32_t index, IxUserAccountData** userAccountData) X_PURE;
};




/*
static const xGuid IID_IxDnsServerDataEnum =
{ 0x666dc0de, 0x9439, 0x4aff, { 0x84, 0x93, 0x59, 0xfe, 0x92, 0xa8, 0x6e, 0xef } };

X_INTERFACE_DECLARE_BASE(IxDnsServerDataEnum, IxInterfaceEnum)
{
	X_INTERFACE_METHOD(xResult, Get)(const uint32_t index, IxDnsServerData** dnsServerData) X_PURE;
};

*/




/*
static const xGuid IID_IxDnsServerData =
{ 0x666dc0de, 0x61bb, 0x49f0, { 0x8c, 0x20, 0x1c, 0xd1, 0x12, 0x1e, 0xdc, 0xe3 } };

X_INTERFACE_DECLARE(IxDnsServerData)
{
	X_INTERFACE_METHOD(xResult, GetIp)(xChar** host) X_PURE;
	X_INTERFACE_METHOD(xResult, GetMask)(xChar** mask) X_PURE;
};
*/

/*
static const xGuid IID_IxNetworkAdapterData =
{ 0x666dc0de, 0x5934, 0x4240, { 0x95, 0xef, 0x8f, 0x02, 0xde, 0xed, 0x68, 0xd2 } };

X_INTERFACE_DECLARE(IxNetworkAdapterData)
{
};
*/


static const xGuid IID_IxStorageProvider =
{ 0x666dc0de, 0x2abc, 0x48ed, { 0x9f, 0x92, 0x08, 0xbb, 0x86, 0x50, 0x65, 0x86 } };

X_INTERFACE_DECLARE_BASE(IxStorageProvider, IxPropertyEnum)
{
	X_INTERFACE_METHOD(xResult, SetFileName)(const xChar* fileName) X_PURE;
	X_INTERFACE_METHOD(xResult, GetFileName)(xChar** fileName) X_PURE;
	X_INTERFACE_METHOD(xResult, Save)(IxDataContainer* dataContainer) X_PURE;
};



static const GUID IID_IxFormatter =
{ 0x666dc0de, 0x9925, 0x4b90, { 0x97, 0x20, 0x23, 0xa1, 0x67, 0x8f, 0x4c, 0xfc } };

X_INTERFACE_DECLARE(IxFormatter)
{
	X_INTERFACE_METHOD(xResult, Format)(IxDataContainer* dataContainer, void* context) X_PURE;
};
