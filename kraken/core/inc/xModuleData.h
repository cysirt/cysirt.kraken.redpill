#pragma once

#include "xPropertyEnum.h"
#include "xReferenceCount.h"

#include "xCoreInterface.h"



template
<
	typename t_Base
>
class xModuleDataBase : public xPropertyEnum<t_Base>
{
typedef xPropertyEnum<t_Base> _Base;

X_INTERFACE_MAP_BEGIN
	X_INTERFACE_MAP_ENTRY(IxModuleData)
	X_INTERFACE_MAP_CHAIN(_Base)
X_INTERFACE_MAP_END

X_PROPERTY_MAP_BEGIN
	X_PROPERTY_MAP_SET_GET_INTERFACE(FileData, IxFileData)
	X_PROPERTY_MAP_SET_GET_INTERFACE(FileVersionData, IxFileVersionData)
	X_PROPERTY_MAP_SET_GET_INTERFACE(ImportTableData, IxImportTableDataEnum)
	X_PROPERTY_MAP_SET_GET_INTERFACE(SignatureData, IxSignatureData)
X_PROPERTY_MAP_END


public:
	xModuleDataBase()
	{
	}

	virtual ~xModuleDataBase()
	{
	}
};




class xModuleData : public xModuleDataBase<IxModuleData>
{
public:
	xModuleData()
	{
	}

	virtual ~xModuleData()
	{
	}
};