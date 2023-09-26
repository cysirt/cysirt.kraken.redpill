#pragma once

#include "xPropertyEnum.h"
#include "xReferenceCount.h"

#include "xCoreInterface.h"

struct json_t;



class xJsonStorageProvider : public xPropertyEnum<IxStorageProvider>
{
typedef xPropertyEnum<IxStorageProvider> _Base;

X_INTERFACE_MAP_BEGIN
	X_INTERFACE_MAP_ENTRY(IxStorageProvider)
	X_INTERFACE_MAP_CHAIN(_Base)
X_INTERFACE_MAP_END

X_PROPERTY_MAP_BEGIN
	X_PROPERTY_MAP_SET_GET(FileName, xChar*)
X_PROPERTY_MAP_END

public:
	xJsonStorageProvider();
	virtual ~xJsonStorageProvider();

	X_INTERFACE_METHOD(xResult, Save)(IxDataContainer* dataContainer);

private:
	xResult Save(IxPropertyEnum* propertyEnum, json_t** jsonResult);
	xResult Save(IxProperty* property, json_t** jsonResult);
	xResult Save(IxStringEnum* stringEnum, json_t** jsonResult);
};

