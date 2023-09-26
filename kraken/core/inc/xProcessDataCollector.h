#pragma once

#include "xReferenceCount.h"

#include "xCoreInterface.h"

class xDllData;
class xProcessData;



class xProcessDataCollector: public xRefCount<IxProcessDataCollector>
{
X_INTERFACE_MAP_BEGIN
	X_INTERFACE_MAP_ENTRY(IxProcessDataCollector)
X_INTERFACE_MAP_END

public:
	xProcessDataCollector();
	virtual ~xProcessDataCollector();

	X_INTERFACE_METHOD(xResult, Collect)(IxProcessDataEnum** processDataEnum);
	X_INTERFACE_METHOD(xResult, Collect)(const uint32_t processId, IxProcessData** processData);

private:
	xResult GetProcessData(const uint32_t processId, xProcessData** processData);
	xResult GetProcessData(HANDLE process, HMODULE module, xProcessData** processData);
	const xChar* GetProcessSubsystem(HANDLE process);
	xResult GetParentProcessData(HANDLE child, uint32_t* parentProcessId, xChar** parentFileName);
	uint32_t GetParentProcessId(HANDLE child);
	xResult GetModuleData(HANDLE process, HMODULE module, IxModuleData** moduleData);
};

