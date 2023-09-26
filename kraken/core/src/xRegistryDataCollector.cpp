#include "xRegistryDataCollector.h"

#include <intrin.h>

#include "xConversion.h"
#include "xFile.h"
#include "xLog.h"
#include "xMemory.h"
#include "xPath.h"
#include "xStringEnum.h"

#include "xDataEnum.h"
#include "xFileDataCollector.h"
#include "xModuleDataCollector.h"
#include "xRegistryKeyData.h"
#include "xRegistryValueData.h"



xRegistryDataCollector::xRegistryDataCollector()
{

}

xRegistryDataCollector::~xRegistryDataCollector()
{

}

xResult xRegistryDataCollector::Collect(IxRegistryKeyDataEnum** registryKeyDataEnum)
{
	X_ASSERT(registryKeyDataEnum);

	if (!registryKeyDataEnum)
	{
		return X_E_INVALID_ARGUMENT;
	}

	typedef struct _registryKey
	{
		HKEY key;
		xChar* path;
	} registryKey;

	// Forensic hives we need
	const registryKey rk[] =
	{
		//  http://www.forensicswiki.org/wiki/Windows_Registry
		HKEY_LOCAL_MACHINE, X_CHAR("Software\\Microsoft\\Command Processor"),
		HKEY_LOCAL_MACHINE, X_CHAR("Software\\Wow6432Node\\Microsoft\\Command Processor"),
		HKEY_CURRENT_USER,	X_CHAR("Software\\Microsoft\\Command Processor"),
		HKEY_CURRENT_USER,	X_CHAR("Software\\Wow6432Node\\Microsoft\\Command Processor"),
		HKEY_LOCAL_MACHINE, X_CHAR("Software\\Microsoft\\Windows NT\\CurrentVersion\\AeDebug"),
		HKEY_LOCAL_MACHINE, X_CHAR("Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Browser Helper Objects"),
		HKEY_LOCAL_MACHINE, X_CHAR("Software\\Wow6432Node\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Browser Helper Objects"),
		HKEY_LOCAL_MACHINE, X_CHAR("System\\CurrentControlSet\\Control\\Lsa"),
		HKEY_LOCAL_MACHINE, X_CHAR("System\\CurrentControlSet\\Control\\Lsa\\OSConfig"),
		HKEY_LOCAL_MACHINE, X_CHAR("Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer\\Run"),
		HKEY_LOCAL_MACHINE, X_CHAR("Software\\Wow6432Node\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer\\Run"),
		HKEY_LOCAL_MACHINE, X_CHAR("Software\\Microsoft\\Windows\\CurrentVersion\\Run"),
		HKEY_LOCAL_MACHINE, X_CHAR("Software\\Wow6432Node\\Microsoft\\Windows\\CurrentVersion\\Run"),
		HKEY_LOCAL_MACHINE, X_CHAR("Software\\Microsoft\\Windows\\CurrentVersion\\RunOnce"),
		HKEY_LOCAL_MACHINE, X_CHAR("Software\\Wow6432Node\\Microsoft\\Windows\\CurrentVersion\\RunOnce"),
		HKEY_LOCAL_MACHINE, X_CHAR("Software\\Microsoft\\Windows\\CurrentVersion\\RunOnce\\Setup"),
		HKEY_LOCAL_MACHINE, X_CHAR("Software\\Wow6432Node\\Microsoft\\Windows\\CurrentVersion\\RunOnce\\Setup"),
		HKEY_LOCAL_MACHINE, X_CHAR("Software\\Microsoft\\Windows\\CurrentVersion\\RunOnceEx"),
		HKEY_LOCAL_MACHINE, X_CHAR("Software\\Wow6432Node\\Microsoft\\Windows\\CurrentVersion\\RunOnceEx"),
		HKEY_CURRENT_USER,	X_CHAR("Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer\\Run"),
		HKEY_CURRENT_USER,	X_CHAR("Software\\Wow6432Node\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer\\Run"),
		HKEY_CURRENT_USER,	X_CHAR("Software\\Microsoft\\Windows\\CurrentVersion\\Run"),
		HKEY_CURRENT_USER,	X_CHAR("Software\\Wow6432Node\\Microsoft\\Windows\\CurrentVersion\\Run"),
		HKEY_CURRENT_USER,	X_CHAR("Software\\Microsoft\\Windows\\CurrentVersion\\RunOnce"),
		HKEY_CURRENT_USER,	X_CHAR("Software\\Wow6432Node\\Microsoft\\Windows\\CurrentVersion\\RunOnce"),
		HKEY_CURRENT_USER,	X_CHAR("Software\\Microsoft\\Windows\\CurrentVersion\\RunOnce\\Setup"),
		HKEY_CURRENT_USER,	X_CHAR("Software\\Wow6432Node\\Microsoft\\Windows\\CurrentVersion\\RunOnce\\Setup"),
		HKEY_CURRENT_USER,	X_CHAR("Software\\Microsoft\\Windows\\CurrentVersion\\RunOnceEx"),
		HKEY_CURRENT_USER,	X_CHAR("Software\\Wow6432Node\\Microsoft\\Windows\\CurrentVersion\\RunOnceEx"),
		HKEY_LOCAL_MACHINE, X_CHAR("Software\\Microsoft\\Windows\\CurrentVersion\\RunServicesOnce"),
		HKEY_LOCAL_MACHINE, X_CHAR("Software\\Wow6432Node\\Microsoft\\Windows\\CurrentVersion\\RunServicesOnce"),
		HKEY_LOCAL_MACHINE, X_CHAR("Software\\Microsoft\\Windows\\CurrentVersion\\RunServices"),
		HKEY_LOCAL_MACHINE, X_CHAR("Software\\Wow6432Node\\Microsoft\\Windows\\CurrentVersion\\RunServices"),
		HKEY_LOCAL_MACHINE, X_CHAR("System\\CurrentControlSet\\Control\\Session Manager"),
		HKEY_LOCAL_MACHINE, X_CHAR("System\\CurrentControlSet\\Control\\Session Manager\\WOW"),
		HKEY_LOCAL_MACHINE, X_CHAR("System\\CurrentControlSet\\Control"),
		HKEY_LOCAL_MACHINE, X_CHAR("Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\ShellIconOverlayIdentifiers"),
		HKEY_LOCAL_MACHINE, X_CHAR("Software\\Wow6432Node\\Microsoft\\Windows\\CurrentVersion\\Explorer\\ShellIconOverlayIdentifiers"),
		HKEY_CURRENT_USER,	X_CHAR("Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\ShellIconOverlayIdentifiers"),
		HKEY_CURRENT_USER,	X_CHAR("Software\\Wow6432Node\\Microsoft\\Windows\\CurrentVersion\\Explorer\\ShellIconOverlayIdentifiers"),
		HKEY_LOCAL_MACHINE, X_CHAR("Software\\Microsoft\\Windows\\CurrentVersion\\Shell Extensions\\Approved"),
		HKEY_LOCAL_MACHINE, X_CHAR("Software\\Wow6432Node\\Microsoft\\Windows\\CurrentVersion\\Shell Extensions\\Approved"),
		HKEY_CURRENT_USER,	X_CHAR("Software\\Microsoft\\Windows\\CurrentVersion\\Shell Extensions\\Approved"),
		HKEY_CURRENT_USER,	X_CHAR("Software\\Wow6432Node\\Microsoft\\Windows\\CurrentVersion\\Shell Extensions\\Approved"),
		HKEY_LOCAL_MACHINE, X_CHAR("Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\ShellExecuteHooks"),
		HKEY_LOCAL_MACHINE, X_CHAR("Software\\Wow6432Node\\Microsoft\\Windows\\CurrentVersion\\Explorer\\ShellExecuteHooks"),
		HKEY_CURRENT_USER,	X_CHAR("Software\\Microsoft\\Windows NT\\CurrentVersion\\Windows"),
		HKEY_CURRENT_USER,	X_CHAR("Software\\Wow6432Node\\Microsoft\\Windows NT\\CurrentVersion\\Windows"),
		HKEY_LOCAL_MACHINE, X_CHAR("Software\\Microsoft\\Windows\\CurrentVersion\\ShellServiceObjectDelayLoad"),
		HKEY_LOCAL_MACHINE, X_CHAR("Software\\Wow6432Node\\Microsoft\\Windows\\CurrentVersion\\ShellServiceObjectDelayLoad"),
		HKEY_LOCAL_MACHINE, X_CHAR("Software\\Microsoft\\Windows\\CurrentVersion\\Authentication\\Credential Provider Filters"),
		HKEY_LOCAL_MACHINE, X_CHAR("Software\\Wow6432Node\\Microsoft\\Windows\\CurrentVersion\\Authentication\\Credential Provider Filters"),
		HKEY_LOCAL_MACHINE, X_CHAR("Software\\Microsoft\\Windows\\CurrentVersion\\Authentication\\Credential Providers"),
		HKEY_LOCAL_MACHINE, X_CHAR("Software\\Wow6432Node\\Microsoft\\Windows\\CurrentVersion\\Authentication\\Credential Providers"),
		HKEY_LOCAL_MACHINE, X_CHAR("Software\\Microsoft\\Windows\\CurrentVersion\\Authentication\\PLAP Providers"),
		HKEY_LOCAL_MACHINE, X_CHAR("Software\\Wow6432Node\\Microsoft\\Windows\\CurrentVersion\\Authentication\\PLAP Providers"),
		HKEY_LOCAL_MACHINE, X_CHAR("Software\\Microsoft\\Windows NT\\CurrentVersion\\Winlogon"),
		HKEY_CURRENT_USER,	X_CHAR("Software\\Microsoft\\Windows NT\\CurrentVersion\\Winlogon"),
		HKEY_LOCAL_MACHINE, X_CHAR("Software\\Microsoft\\Windows NT\\CurrentVersion\\Winlogon\\Notify"),
		HKEY_CURRENT_USER,	X_CHAR("Software\\Microsoft\\Windows NT\\CurrentVersion\\Winlogon\\Notify"),
		HKEY_LOCAL_MACHINE, X_CHAR("Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\System"),
		HKEY_LOCAL_MACHINE, X_CHAR("Software\\Wow6432Node\\Microsoft\\Windows\\CurrentVersion\\Policies\\System"),
		HKEY_LOCAL_MACHINE, X_CHAR("Software\\Microsoft\\Active Setup\\Installed Components"),
		HKEY_LOCAL_MACHINE, X_CHAR("Software\\Wow6432Node\\Microsoft\\Active Setup\\Installed Components"),
		HKEY_CURRENT_USER,	X_CHAR("Software\\Microsoft\\Active Setup\\Installed Components"),
		HKEY_CURRENT_USER,	X_CHAR("Software\\Wow6432Node\\Microsoft\\Active Setup\\Installed Components"),
		HKEY_LOCAL_MACHINE, X_CHAR("Software\\Microsoft\\Windows NT\\CurrentVersion\\Windows"),
		HKEY_LOCAL_MACHINE, X_CHAR("Software\\Wow6432Node\\Microsoft\\Windows NT\\CurrentVersion\\Windows"),
		HKEY_CURRENT_USER,	X_CHAR("Software\\Microsoft\\Windows NT\\CurrentVersion\\Windows"),
		HKEY_CURRENT_USER,	X_CHAR("Software\\Wow6432Node\\Microsoft\\Windows NT\\CurrentVersion\\Windows"),
		HKEY_LOCAL_MACHINE, X_CHAR("System\\CurrentControlSet\\Control\\SecurityProviders"),
		HKEY_LOCAL_MACHINE, X_CHAR("System\\CurrentControlSet\\Control\\SafeBoot"),
		HKEY_LOCAL_MACHINE, X_CHAR("System\\CurrentControlSet\\Control\\BootVerificationProgram")
	};

	xPtr<IxRegistryKeyDataEnum> result(new xRegistryKeyDataEnum);

	if (!result)
	{
		return X_E_OUT_OF_MEMORY;
	}

	for (uint32_t ui = 0; ui < sizeof(rk) / sizeof(registryKey); ui++)
	{
		xInterfacePtr<IxRegistryKeyData> rkd;

		xResult r = Collect(rk[ui].key, rk[ui].path, &rkd);

		if (xSucceeded(r))
		{
			result->Add(rkd);
		}
		else
		{
			if (r != X_E_NO_DATA)
			{
				xPtr<xChar> rks;

				r = GetRootKeyString(rk[ui].key, &rks);

				X_LOG_ERROR(X_CHAR("Failed to collect registry data at key \"%s\", path \"%s\""), xSucceeded(r) ? rks : X_CHAR("?"), rk[ui].path);
			}
		}
	}

	if (result->GetCount())
	{
		*registryKeyDataEnum = result.Detach();
	}

	return (*registryKeyDataEnum) ? X_S_OK : X_E_NO_DATA;
}

xResult xRegistryDataCollector::Collect(HKEY rootKey, const xChar* path, IxRegistryKeyData** registryKeyData) 
{ 
	X_ASSERT(rootKey && path && registryKeyData);

	if (!rootKey || !path || !registryKeyData)
	{
		return X_E_INVALID_ARGUMENT;
	}

	xInterfacePtr<xRegistryKeyData> rkd(new xRegistryKeyData);
	
	if (!rkd)
	{
		return X_E_OUT_OF_MEMORY;
	}

	HKEY currentKey = NULL;

	xResult r = xResultFromWin32(RegOpenKeyEx(rootKey, path, 0, KEY_READ, &currentKey));

	if (xSucceeded(r))
	{
		DWORD subKeyCount = 0;

		DWORD subKeyNameSizeMax = 0;

		DWORD valueCount = 0;

		DWORD valueNameSizeMax = 0;

		DWORD dataSizeMax = 0;

		FILETIME lastModified = {0, 0};

		if (xSucceeded(r = xResultFromWin32(RegQueryInfoKey(currentKey, NULL, NULL, NULL, &subKeyCount, &subKeyNameSizeMax, NULL, &valueCount, &valueNameSizeMax, &dataSizeMax, NULL, &lastModified))))
		{
			xPtr<xChar> rks;

			if (xSucceeded(r = GetRootKeyString(rootKey, &rks)))
			{
				rkd->SetRootKey(rks);
			}

			rkd->SetPath(path);

			rkd->SetLastModified(xConvertFileTimeToTime(lastModified));

			// Keys
			if (subKeyCount)
			{
				xInterfacePtr<xRegistryKeyDataEnum> rkde(new xRegistryKeyDataEnum);

				if (rkde)
				{
					for (uint32_t ui = 0; ui < subKeyCount; ui++)
					{
						DWORD subKeyNameSize = subKeyNameSizeMax + 1;

						xString keyName;

						xChar* keyNameBuffer = keyName.GetBuffer(subKeyNameSize);

						if (keyNameBuffer && 
							xSucceeded(r = xResultFromWin32(RegEnumKeyEx(currentKey, ui, keyNameBuffer, &subKeyNameSize, NULL, NULL, NULL, NULL))))
						{
							keyName.ReleaseBuffer();

							xString subKeyName;

							subKeyName.Format(X_CHAR("%s\\%s"), path, keyName);

							xInterfacePtr<IxRegistryKeyData> subKeyData;

							if (xSucceeded(r = Collect(rootKey, subKeyName, &subKeyData)))
							{
								rkde->Add(subKeyData);
							}
						}
					}

					if (rkde->GetCount())
					{
						rkd->SetKeyData(rkde);
					}
				}
			}

			// Values
			if (valueCount)
			{
				xInterfacePtr<xRegistryValueDataEnum> rvde(new xRegistryValueDataEnum);

				if (rvde)
				{
					for (uint32_t uj = 0; uj < valueCount; uj++)
					{
						DWORD valueNameSize = valueNameSizeMax + 1;

						xString valueName;

						xChar* valueNameBuffer = valueName.GetBuffer(valueNameSize);

						if (valueNameBuffer &&
							xSucceeded(r = xResultFromWin32(RegEnumValue(currentKey, uj, valueName.GetBuffer(valueNameSize), &valueNameSize, NULL, NULL, NULL, NULL))))
						{
							valueName.ReleaseBuffer();

							xInterfacePtr<IxRegistryValueData> rvd;

							if (xSucceeded(r = Collect(rootKey, path, valueName, &rvd)))
							{
								rvde->Add(rvd);
							}
						}
					}

					if (rvde->GetCount())
					{
						rkd->SetValueData(rvde);
					}
				}
			}

			RegCloseKey(currentKey);
		}
	}

	const uint32_t count = rkd->GetCount();

	if (count)
	{
		*registryKeyData = rkd.Detach();
	}

	return count ? X_S_OK : X_E_NO_DATA;
}

xResult xRegistryDataCollector::Collect(HKEY key, const xChar* path, const xChar* name, IxRegistryValueData** registryValueData)
{
	X_ASSERT(key && path && name && registryValueData);

	if (!key || !path || !name || !registryValueData)
	{
		return X_E_INVALID_ARGUMENT;
	}

	DWORD type = 0;

	DWORD size = 0;

	xResult r = xResultFromWin32(RegGetValue(key, path, name, RRF_RT_ANY, &type, NULL, &size));

	if (xFailed(r))
	{
		return r;
	}

	xPtr<uint8_t> buffer((uint8_t*) xMemoryAlloc(size));

	if (!buffer)
	{
		return X_E_OUT_OF_MEMORY;
	}

	if (xFailed(r = xResultFromWin32(RegGetValue(key, path, name, RRF_RT_ANY, &type, buffer, &size))))
	{
		X_LOG_WARNING(X_CHAR("Failed to get registry value for key \"%s\", path \"%s\", name \"%s\""), L"TBD", path, name);
	}

	xInterfacePtr<xRegistryValueData> rvd(new xRegistryValueData);

	if (!rvd)
	{
		return X_E_OUT_OF_MEMORY;
	}

	xPtr<xChar> rks;

	if (xSucceeded(r = GetRootKeyString(key, &rks)))
	{
		rvd->SetRootKey(rks);
	}
	
	rvd->SetPath(path);

	rvd->SetName(xCharIsEmpty(name) ? X_CHAR("Default") : name);

	rvd->SetType(type);

	if (size)
	{
		switch (type)
		{
			case REG_BINARY:
			case REG_RESOURCE_LIST:
			case REG_FULL_RESOURCE_DESCRIPTOR:
			case REG_RESOURCE_REQUIREMENTS_LIST:
			{
				xPtr<xChar> value(xConvertBinaryToHex(buffer, size));

				if (value)
				{
					rvd->SetBinaryValue(value);
				}

				break;
			}
			case REG_DWORD:
			{
				rvd->SetDwordValue(*buffer);

				break;
			}
			case REG_DWORD_BIG_ENDIAN:
			{
				rvd->SetBigEndianDwordValue(_byteswap_ulong(*buffer));

				break;
			}
			case REG_EXPAND_SZ:
			{
				const xChar* s = (xChar*) (uint8_t*) buffer;

				rvd->SetEnvironmentStringValue(s);

				xChar fileName[X_PATH_MAX] = {X_CHAR('\0')};

				// TODO: ExpandEnvironmentStringsForUser?
				if (ExpandEnvironmentStrings(s, fileName, X_PATH_MAX))
				{
					Collect(rvd, fileName);
				}

				break;
			}
			case REG_LINK:
			{
				const xChar* s = (xChar*) (uint8_t*) buffer;

				rvd->SetLinkValue(s);

				Collect(rvd, s);

				break;
			}
			case REG_MULTI_SZ:
			{
				xInterfacePtr<IxStringEnum> se;

				if (xSucceeded(r = xConvertMultiStringToStringEnum((xChar*) (uint8_t*) buffer, &se)))
				{
					// TODO: Check files in multi string?

					if (se->GetCount())
					{
						rvd->SetMultiStringValue(se);
					}
				}

				break;
			}
			case REG_QWORD:
			{
				rvd->SetQwordValue(*buffer);

				break;
			}
			case REG_SZ:
			{
				const xChar* s = (xChar*) (uint8_t*) buffer;

				rvd->SetStringValue(s);

				if (xFailed(r = Collect(rvd, s)))
				{
					xString sd;

					xChar* buffer = sd.GetBuffer(X_PATH_MAX);

					if (buffer &&
						GetSystemDirectory(buffer, X_PATH_MAX))
					{
						sd.ReleaseBuffer();

						xPtr<xChar> fnsd;

						if (xSucceeded(r = xPathAppend(sd, s, &fnsd)))
						{
							if (xFailed(r = Collect(rvd, fnsd)))
							{
								xString wd;

								buffer = wd.GetBuffer(X_PATH_MAX);

								if (buffer &&
									GetWindowsDirectory(buffer, X_PATH_MAX))
								{
									wd.ReleaseBuffer();

									xPtr<xChar> fnwd;

									if (xSucceeded(r = xPathAppend(wd, s, &fnwd)))
									{
										Collect(rvd, fnwd);
									}
								}
							}
						}
					}
				}

				break;
			}
			default:
			{
				X_ASSERT(false);

				return X_E_UNEXPECTED;
			}
		}
	}
	else
	{
		return X_E_NO_DATA;
	}

	*registryValueData = rvd.Detach();

	return X_S_OK;
}

xResult xRegistryDataCollector::Collect(xRegistryValueData* registryValueData, const xChar* fileName)
{
	X_ASSERT(registryValueData && fileName);

	if (!registryValueData || !fileName)
	{
		return X_E_INVALID_ARGUMENT;
	}

	xResult r = X_S_OK;

	const DWORD fa = GetFileAttributes(fileName);

	if (fa != INVALID_FILE_ATTRIBUTES && !(fa & FILE_ATTRIBUTE_DIRECTORY))
	{
		// Seems to be a valid file or directory
		if (xFileIsModule(fileName))
		{
			xModuleDataCollector mdc;

			xInterfacePtr<IxModuleData> md;

			if (xSucceeded(r = mdc.Collect(fileName, &md)))
			{
				registryValueData->SetModuleData(md);
			}
		}
		else
		{
			xFileDataCollector fdc;

			xInterfacePtr<IxFileData> fd;

			if (xSucceeded(r = fdc.Collect(fileName, &fd)))
			{
				registryValueData->SetFileData(fd);
			}
		}
	}
	else
	{
		r = X_E_INVALID_DATA;
	}

	return r;
}

xResult xRegistryDataCollector::GetRootKeyString(HKEY rootKey, xChar** rootKeyString)
{
	X_ASSERT(rootKey && rootKeyString);

	if (!rootKey || !rootKeyString)
	{
		return X_E_INVALID_ARGUMENT;
	}

	if (rootKey == HKEY_CLASSES_ROOT)
	{
		*rootKeyString = xCharClone(X_CHAR("HKEY_CLASSES_ROOT"));
	}
	else if (rootKey == HKEY_CURRENT_CONFIG)
	{
		*rootKeyString = xCharClone(X_CHAR("HKEY_CURRENT_CONFIG"));
	}
	else if (rootKey == HKEY_CURRENT_USER)
	{
		*rootKeyString = xCharClone(X_CHAR("HKEY_CURRENT_USER"));
	}
	else if (rootKey == HKEY_LOCAL_MACHINE)
	{
		*rootKeyString = xCharClone(X_CHAR("HKEY_LOCAL_MACHINE"));
	}
	else if (rootKey == HKEY_PERFORMANCE_DATA)
	{
		*rootKeyString = xCharClone(X_CHAR("HKEY_PERFORMANCE_DATA"));
	}
	else if (rootKey == HKEY_PERFORMANCE_NLSTEXT)
	{
		*rootKeyString = xCharClone(X_CHAR("HKEY_PERFORMANCE_NLSTEXT"));
	}
	else if (rootKey == HKEY_PERFORMANCE_TEXT)
	{
		*rootKeyString = xCharClone(X_CHAR("HKEY_PERFORMANCE_TEXT"));
	}
	else if (rootKey == HKEY_USERS)
	{
		*rootKeyString = xCharClone(X_CHAR("HKEY_USERS"));
	}
	else
	{
		X_ASSERT(false);

		return X_E_UNEXPECTED;
	}

	return (*rootKeyString) ? X_S_OK : X_E_OUT_OF_MEMORY;
}