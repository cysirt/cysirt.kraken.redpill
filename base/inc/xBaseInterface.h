#pragma once

#include <stdint.h>

#include "xChar.h"
#include "xDefinition.h"
#include "xGuid.h"
#include "xResult.h"



#define X_INTERFACE struct



#define X_INTERFACE_DECLARE_BASE(__Type, __Base)				X_INTERFACE __X_NOVTABLE __Type : public __Base
#define X_INTERFACE_DECLARE_BASE_2(__Type, __Base1, __Base2)	X_INTERFACE __X_NOVTABLE __Type : public __Base1, public __Base2
#define X_INTERFACE_DECLARE(__Type)								X_INTERFACE_DECLARE_BASE(__Type, IxInterface)
#define X_INTERFACE_METHOD(__Type, __Method)					virtual __Type __X_STDCALL __Method



X_INTERFACE IxFunctor;
X_INTERFACE IxInterface;
X_INTERFACE IxInterfaceEnum;
X_INTERFACE IxStringEnum;
X_INTERFACE IxStream;
X_INTERFACE IxStreamReader;
X_INTERFACE IxStreamWriter;
X_INTERFACE IxTimeEnum;



static const xGuid IID_IxInterface =
{ 0x666dc0de, 0xe499, 0x489d, { 0xb1, 0x19, 0x31, 0x75, 0xf3, 0x7b, 0x5d, 0x23 } };

X_INTERFACE __X_NOVTABLE IxInterface
{
	X_INTERFACE_METHOD(xResult, QueryInterface)(const xGuid& iid, void** iface) X_PURE;
	X_INTERFACE_METHOD(uint32_t, AddRef)() X_PURE;
	X_INTERFACE_METHOD(uint32_t, Release)() X_PURE;
};





enum xPropertyType
{
	xPropertyTypeInvalid	= 0x0000,
	xPropertyTypeInt8		= 0x0001,
	xPropertyTypeInt16		= 0x0002,
	xPropertyTypeInt32		= 0x0003,
	xPropertyTypeInt64		= 0x0004,
	xPropertyTypeUint8		= 0x0005,
	xPropertyTypeUint16		= 0x0006,
	xPropertyTypeUint32		= 0x0007,
	xPropertyTypeUint64		= 0x0008,
	xPropertyTypeString		= 0x000a,
	xPropertyTypeInterface	= 0x000b,
	xPropertyTypeGuid		= 0x000d
};

static const xGuid IID_IxProperty =
{ 0x666dc0de, 0x85ed, 0x4f3a, { 0xb0, 0xd4, 0x14, 0x69, 0x23, 0x8d, 0xce, 0xaf } };

X_INTERFACE_DECLARE(IxProperty)
{
	X_INTERFACE_METHOD(const char*, GetName)() X_PURE;
	X_INTERFACE_METHOD(xPropertyType, GetType)() X_PURE;

	X_INTERFACE_METHOD(xResult, SetValue)(const bool value) X_PURE;
	X_INTERFACE_METHOD(xResult, GetValue)(bool* value) X_PURE;

	X_INTERFACE_METHOD(xResult, SetValue)(const int8_t value) X_PURE;
	X_INTERFACE_METHOD(xResult, GetValue)(int8_t* value) X_PURE;

	X_INTERFACE_METHOD(xResult, SetValue)(const int16_t value) X_PURE;
	X_INTERFACE_METHOD(xResult, GetValue)(int16_t* value) X_PURE;

	X_INTERFACE_METHOD(xResult, SetValue)(const int32_t value) X_PURE;
	X_INTERFACE_METHOD(xResult, GetValue)(int32_t* value) X_PURE;

	X_INTERFACE_METHOD(xResult, SetValue)(const int64_t value) X_PURE;
	X_INTERFACE_METHOD(xResult, GetValue)(int64_t* value) X_PURE;

	X_INTERFACE_METHOD(xResult, SetValue)(const uint8_t value) X_PURE;
	X_INTERFACE_METHOD(xResult, GetValue)(uint8_t* value) X_PURE;

	X_INTERFACE_METHOD(xResult, SetValue)(const uint16_t value) X_PURE;
	X_INTERFACE_METHOD(xResult, GetValue)(uint16_t* value) X_PURE;

	X_INTERFACE_METHOD(xResult, SetValue)(const uint32_t value) X_PURE;
	X_INTERFACE_METHOD(xResult, GetValue)(uint32_t* value) X_PURE;

	X_INTERFACE_METHOD(xResult, SetValue)(const uint64_t value) X_PURE;
	X_INTERFACE_METHOD(xResult, GetValue)(uint64_t* value) X_PURE;

	X_INTERFACE_METHOD(xResult, SetValue)(const xChar* value) X_PURE;
	X_INTERFACE_METHOD(xResult, GetValue)(xChar** value) X_PURE;

	X_INTERFACE_METHOD(xResult, SetValue)(IxInterface* value) X_PURE;
	X_INTERFACE_METHOD(xResult, GetValue)(IxInterface** value) X_PURE;

	X_INTERFACE_METHOD(xResult, SetValue)(const xGuid& value) X_PURE;
	X_INTERFACE_METHOD(xResult, GetValue)(xGuid* value) X_PURE;
};



static const xGuid IID_IxFunctor =
{ 0x666dc0de, 0xa490, 0x4b67, { 0x8d, 0xf6, 0x52, 0xb0, 0xe4, 0x71, 0x81, 0x7f } };

X_INTERFACE_DECLARE(IxFunctor)
{
	X_INTERFACE_METHOD(xResult, Call)() X_PURE;
	X_INTERFACE_METHOD(xResult, Call)(void* data) X_PURE;
};



enum xLogCategory
{
	xLogCategoryInfo			= 0x00000001,
	xLogCategoryWarning			= 0x00000002,
	xLogCategoryError			= 0x00000004,
	xLogCategoryHighlightGreen	= 0x00000008,
	xLogCategoryHighlightRed	= 0x00000010,
	xLogCategoryHighlightBlue	= 0x00000020,
	xLogCategoryDebug			= 0x00000040,
	xLogCategorySilent			= 0x00000080
};

static const xGuid IID_IxLogWriter =
{ 0x666dc0de, 0xc2be, 0x4cab,{ 0x84, 0xb6, 0x23, 0x50, 0xb2, 0x2b, 0x61, 0xf8 } };

X_INTERFACE_DECLARE(IxLogWriter)
{
	X_INTERFACE_METHOD(xResult, Write)(xLogCategory category, const xChar* message) X_PURE;
};



static const xGuid IID_IxInterfaceEnum =
{ 0x666dc0de, 0x7ae2, 0x4ab1, { 0xa7, 0x9b, 0x4b, 0xb7, 0xd5, 0xa2, 0x3, 0x79 } };

X_INTERFACE_DECLARE(IxInterfaceEnum)
{
	X_INTERFACE_METHOD(uint32_t, GetCount)() X_PURE;
	X_INTERFACE_METHOD(xResult, Get)(const uint32_t index, IxInterface** iface) X_PURE;
	X_INTERFACE_METHOD(xResult, Get)(const uint32_t index, const xGuid& iid, IxInterface** iface) X_PURE;
	X_INTERFACE_METHOD(xResult, Add)(IxInterface* iface) X_PURE;
	X_INTERFACE_METHOD(xResult, Add)(IxInterfaceEnum* ifaceEnum, const bool deep) X_PURE;
	X_INTERFACE_METHOD(xResult, Delete)(const uint32_t index) X_PURE;
	X_INTERFACE_METHOD(xResult, DeleteAll)() X_PURE;
};

static const xGuid IID_IxPropertyEnum =
{ 0x666dc0de, 0x7468, 0x4a22, { 0xba, 0x45, 0xb5, 0xd8, 0x78, 0xb0, 0xe7, 0xd1 } };

X_INTERFACE_DECLARE_BASE(IxPropertyEnum, IxInterfaceEnum)
{
	X_INTERFACE_METHOD(xResult, Get)(const uint32_t index, IxProperty** property) X_PURE;
	X_INTERFACE_METHOD(xResult, Get)(const uint32_t index, IxPropertyEnum** propertyEnum) X_PURE;
};



static const xGuid IID_IxStringEnum =
{ 0x666dc0de, 0xdba0, 0x4c2f, { 0x98, 0x20, 0x1e, 0xc3, 0x3a, 0xa7, 0x6a, 0x45 } };

X_INTERFACE_DECLARE(IxStringEnum)
{
	X_INTERFACE_METHOD(uint32_t, GetCount)() X_PURE;
	X_INTERFACE_METHOD(xResult, Get)(const uint32_t index, xChar** str) X_PURE;
	X_INTERFACE_METHOD(xResult, Add)(const xChar* str) X_PURE;
	X_INTERFACE_METHOD(xResult, Add)(IxStringEnum* stringEnum) X_PURE;
	X_INTERFACE_METHOD(xResult, Delete)(const uint32_t index) X_PURE;
	X_INTERFACE_METHOD(xResult, DeleteAll)() X_PURE;
};



enum xStreamOriginFlag
{
	xStreamOriginFlagBegin		= 0x01,
	xStreamOriginFlagCurrent	= 0x02,
	xStreamOriginFlagEnd		= 0x03
};

static const xGuid IID_IxStream =
{ 0x666dc0de, 0x76a2, 0x490a, { 0x96, 0x64, 0x53, 0xd7, 0x66, 0xba, 0xd1, 0x65 } };

X_INTERFACE_DECLARE(IxStream)
{
	X_INTERFACE_METHOD(uint64_t, GetSize)() X_PURE;
	X_INTERFACE_METHOD(xResult, SetPosition)(const uint32_t offset, const xStreamOriginFlag origin) X_PURE;
	X_INTERFACE_METHOD(uint32_t, Write)(const uint8_t* buffer, const uint32_t size) X_PURE;
	X_INTERFACE_METHOD(uint32_t, Read)(uint8_t* buffer, const uint32_t size) X_PURE;
	X_INTERFACE_METHOD(uint32_t, Read)(uint8_t** buffer, const uint32_t size) X_PURE;
	X_INTERFACE_METHOD(xResult, Reset)() X_PURE;
};



static const xGuid IID_IxStreamReader =
{ 0x666dc0de, 0x3599, 0x4393, { 0x83, 0xe4, 0x10, 0x89, 0xec, 0x77, 0xce, 0x41 } };

X_INTERFACE_DECLARE(IxStreamReader)
{
	X_INTERFACE_METHOD(xResult, Set)(IxStream* stream) X_PURE;
	X_INTERFACE_METHOD(xResult, Read)(int32_t* value) X_PURE;
	X_INTERFACE_METHOD(xResult, Read)(int64_t* value) X_PURE;
	X_INTERFACE_METHOD(xResult, Read)(uint32_t* value) X_PURE;
	X_INTERFACE_METHOD(xResult, Read)(uint64_t* value) X_PURE;
	X_INTERFACE_METHOD(xResult, Read)(uint8_t* value, const uint32_t size) X_PURE;
	X_INTERFACE_METHOD(xResult, Read)(uint8_t** value, const uint32_t size) X_PURE;
	X_INTERFACE_METHOD(xResult, Read)(uint16_t* value) X_PURE;
	X_INTERFACE_METHOD(xResult, Read)(bool* value) X_PURE;
	X_INTERFACE_METHOD(xResult, Read)(xChar** value, const uint32_t count) X_PURE;
};



static const xGuid IID_IxStreamWriter =
{ 0x666dc0de, 0x2a7c, 0x4a06, { 0x9b, 0xaa, 0xd5, 0x54, 0x0b, 0xf1, 0xdd, 0x98 } };

X_INTERFACE_DECLARE(IxStreamWriter)
{
	X_INTERFACE_METHOD(xResult, Set)(IxStream* stream) X_PURE;
	X_INTERFACE_METHOD(xResult, Write)(int32_t value) X_PURE;
	X_INTERFACE_METHOD(xResult, Write)(int64_t value) X_PURE;
	X_INTERFACE_METHOD(xResult, Write)(uint32_t value) X_PURE;
	X_INTERFACE_METHOD(xResult, Write)(uint64_t value) X_PURE;
	X_INTERFACE_METHOD(xResult, Write)(const uint8_t* value, const uint32_t size) X_PURE;
	X_INTERFACE_METHOD(xResult, Write)(uint16_t value) X_PURE;
	X_INTERFACE_METHOD(xResult, Write)(bool value) X_PURE;
	X_INTERFACE_METHOD(xResult, Write)(const xChar* value) X_PURE;
};



static const xGuid IID_IxBoolEnum =
{ 0x666dc0de, 0x2a0a, 0x4712, { 0xbf, 0xce, 0x6d, 0x88, 0xa0, 0xc2, 0x91, 0x69 } };

X_INTERFACE_DECLARE(IxBoolEnum)
{
	X_INTERFACE_METHOD(uint32_t, GetCount)() X_PURE;
	X_INTERFACE_METHOD(xResult, Get)(const uint32_t index, bool* value) X_PURE;
	X_INTERFACE_METHOD(xResult, Add)(const bool value) X_PURE;
	X_INTERFACE_METHOD(xResult, Add)(IxBoolEnum* valueEnum) X_PURE;
	X_INTERFACE_METHOD(xResult, Delete)(const uint32_t index) X_PURE;
	X_INTERFACE_METHOD(xResult, DeleteAll)() X_PURE;
};



static const xGuid IID_IxInt8Enum =
{ 0x666dc0de, 0xa814, 0x4367, { 0x97, 0x9a, 0x06, 0xca, 0x0a, 0xe7, 0xc5, 0xc5 } };

X_INTERFACE_DECLARE(IxInt8Enum)
{
	X_INTERFACE_METHOD(uint32_t, GetCount)() X_PURE;
	X_INTERFACE_METHOD(xResult, Get)(const uint32_t index, int8_t* value) X_PURE;
	X_INTERFACE_METHOD(xResult, Add)(const int8_t value) X_PURE;
	X_INTERFACE_METHOD(xResult, Add)(IxInt8Enum* valueEnum) X_PURE;
	X_INTERFACE_METHOD(xResult, Delete)(const uint32_t index) X_PURE;
	X_INTERFACE_METHOD(xResult, DeleteAll)() X_PURE;
};



static const xGuid IID_IxInt16Enum =
{ 0x666dc0de, 0x6047, 0x4fd6, { 0x94, 0x9c, 0xe2, 0x14, 0x55, 0x5c, 0x88, 0x25 } };

X_INTERFACE_DECLARE(IxInt16Enum)
{
	X_INTERFACE_METHOD(uint32_t, GetCount)() X_PURE;
	X_INTERFACE_METHOD(xResult, Get)(const uint32_t index, int16_t* value) X_PURE;
	X_INTERFACE_METHOD(xResult, Add)(const int16_t value) X_PURE;
	X_INTERFACE_METHOD(xResult, Add)(IxInt16Enum* valueEnum) X_PURE;
	X_INTERFACE_METHOD(xResult, Delete)(const uint32_t index) X_PURE;
	X_INTERFACE_METHOD(xResult, DeleteAll)() X_PURE;
};



static const xGuid IID_IxInt32Enum =
{ 0x666dc0de, 0x7304, 0x483a, { 0x99, 0xd3, 0xa1, 0x8c, 0x90, 0x3e, 0x3c, 0x9 } };

X_INTERFACE_DECLARE(IxInt32Enum)
{
	X_INTERFACE_METHOD(uint32_t, GetCount)() X_PURE;
	X_INTERFACE_METHOD(xResult, Get)(const uint32_t index, int32_t* value) X_PURE;
	X_INTERFACE_METHOD(xResult, Add)(const int32_t value) X_PURE;
	X_INTERFACE_METHOD(xResult, Add)(IxInt32Enum* valueEnum) X_PURE;
	X_INTERFACE_METHOD(xResult, Delete)(const uint32_t index) X_PURE;
	X_INTERFACE_METHOD(xResult, DeleteAll)() X_PURE;
};



static const xGuid IID_IxInt64Enum =
{ 0x666dc0de, 0x7b05, 0x4171, { 0xb8, 0xc4, 0xa6, 0xfa, 0x83, 0x97, 0x37, 0x62 } };

X_INTERFACE_DECLARE(IxInt64Enum)
{
	X_INTERFACE_METHOD(uint32_t, GetCount)() X_PURE;
	X_INTERFACE_METHOD(xResult, Get)(const uint32_t index, int64_t* value) X_PURE;
	X_INTERFACE_METHOD(xResult, Add)(const int64_t value) X_PURE;
	X_INTERFACE_METHOD(xResult, Add)(IxInt64Enum* valueEnum) X_PURE;
	X_INTERFACE_METHOD(xResult, Delete)(const uint32_t index) X_PURE;
	X_INTERFACE_METHOD(xResult, DeleteAll)() X_PURE;
};



static const xGuid IID_IxUint8Enum =
{ 0x666dc0de, 0xae34, 0x436f, { 0xa9, 0xd6, 0x56, 0x82, 0x33, 0x5b, 0x27, 0x2f } };

X_INTERFACE_DECLARE(IxUint8Enum)
{
	X_INTERFACE_METHOD(uint32_t, GetCount)() X_PURE;
	X_INTERFACE_METHOD(xResult, Get)(const uint32_t index, uint8_t* value) X_PURE;
	X_INTERFACE_METHOD(xResult, Add)(const uint8_t value) X_PURE;
	X_INTERFACE_METHOD(xResult, Add)(IxUint8Enum* valueEnum) X_PURE;
	X_INTERFACE_METHOD(xResult, Delete)(const uint32_t index) X_PURE;
	X_INTERFACE_METHOD(xResult, DeleteAll)() X_PURE;
};



static const xGuid IID_IxUint16Enum =
{ 0x666dc0de, 0x79e7, 0x4612, { 0x87, 0xec, 0x33, 0x4f, 0x91, 0x60, 0xa7, 0x1a } };

X_INTERFACE_DECLARE(IxUint16Enum)
{
	X_INTERFACE_METHOD(uint32_t, GetCount)() X_PURE;
	X_INTERFACE_METHOD(xResult, Get)(const uint32_t index, uint16_t* value) X_PURE;
	X_INTERFACE_METHOD(xResult, Add)(const uint16_t value) X_PURE;
	X_INTERFACE_METHOD(xResult, Add)(IxUint16Enum* valueEnum) X_PURE;
	X_INTERFACE_METHOD(xResult, Delete)(const uint32_t index) X_PURE;
	X_INTERFACE_METHOD(xResult, DeleteAll)() X_PURE;
};



static const xGuid IID_IxUint32Enum =
{ 0x666dc0de, 0x1b01, 0x4c19, { 0xaa, 0xe2, 0xf3, 0xe8, 0x55, 0xc8, 0x9b, 0x49 } };

X_INTERFACE_DECLARE(IxUint32Enum)
{
	X_INTERFACE_METHOD(uint32_t, GetCount)() X_PURE;
	X_INTERFACE_METHOD(xResult, Get)(const uint32_t index, uint32_t* value) X_PURE;
	X_INTERFACE_METHOD(xResult, Add)(const uint32_t value) X_PURE;
	X_INTERFACE_METHOD(xResult, Add)(IxUint32Enum* valueEnum) X_PURE;
	X_INTERFACE_METHOD(xResult, Delete)(const uint32_t index) X_PURE;
	X_INTERFACE_METHOD(xResult, DeleteAll)() X_PURE;
};



static const xGuid IID_IxUint64Enum =
{ 0x666dc0de, 0xd04c, 0x4865, { 0xb2, 0xc7, 0x8b, 0x81, 0xdf, 0xdb, 0x21, 0x68 } };

X_INTERFACE_DECLARE(IxUint64Enum)
{
	X_INTERFACE_METHOD(uint32_t, GetCount)() X_PURE;
	X_INTERFACE_METHOD(xResult, Get)(const uint32_t index, uint64_t* value) X_PURE;
	X_INTERFACE_METHOD(xResult, Add)(const uint64_t value) X_PURE;
	X_INTERFACE_METHOD(xResult, Add)(IxUint64Enum* valueEnum) X_PURE;
	X_INTERFACE_METHOD(xResult, Delete)(const uint32_t index) X_PURE;
	X_INTERFACE_METHOD(xResult, DeleteAll)() X_PURE;
};



static const xGuid IID_IxTimeEnum =
{ 0x666dc0de, 0x6a26, 0x42bb,{ 0x86, 0xb6, 0x6c, 0x29, 0x81, 0x50, 0xc8, 0xdf } };

X_INTERFACE_DECLARE(IxTimeEnum)
{
	X_INTERFACE_METHOD(uint32_t, GetCount)() X_PURE;
	X_INTERFACE_METHOD(xResult, Get)(const uint32_t index, time_t* time) X_PURE;
	X_INTERFACE_METHOD(xResult, Add)(const time_t time) X_PURE;
	X_INTERFACE_METHOD(xResult, Add)(IxTimeEnum* timeEnum) X_PURE;
	X_INTERFACE_METHOD(xResult, Delete)(const uint32_t index) X_PURE;
	X_INTERFACE_METHOD(xResult, DeleteAll)() X_PURE;
};
