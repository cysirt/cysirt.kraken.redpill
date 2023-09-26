#include "xJsonStorageProvider.h"

#include "xConversion.h"
#include "xFile.h"
#include "xLog.h"

#include "jansson.h"



#define X_JSON_STORAGE_PROVIDER_SET_INTEGER_TYPE(__Type) \
	__Type value = 0; \
	if (xFailed(r = property->GetValue(&value))) \
	{ \
		return r; \
	} \
	if (!(json = json_integer(value))) \
	{ \
		r = X_E_OUT_OF_MEMORY; \
	}




xJsonStorageProvider::xJsonStorageProvider()
{
}

xJsonStorageProvider::~xJsonStorageProvider()
{
}

xResult xJsonStorageProvider::Save(IxDataContainer* dataContainer)
{
	X_ASSERT(dataContainer);

	if (!dataContainer)
	{
		return X_E_INVALID_ARGUMENT;
	}

	xPtr<xChar> fileName;

	xResult r = GetFileName(&fileName);

	if (xSucceeded(r))
	{
		xInterfacePtr<IxPropertyEnum> propertyEnum;

		json_t* jsonResult = NULL;

		if (xSucceeded(r = dataContainer->QueryInterface(IID_IxPropertyEnum, (void**) &propertyEnum)) &&
			xSucceeded(r = Save(propertyEnum, &jsonResult)))
		{
			FILE* f = xFileOpen(fileName, X_CHAR("wb"));

			if (f)
			{
				if (json_dumpf(jsonResult, f, JSON_COMPACT | JSON_SORT_KEYS) != 0)
				{
					r = X_E_FAIL;

					X_LOG_ERROR(X_CHAR("Failed to generate JSON output (code: ") X_RESULT_FORMAT_STRING X_CHAR(")"), r);
				}

				json_decref(jsonResult);

				xFileClose(f);
			}
			else
			{
				r = xResultFromErrNo();

				X_LOG_ERROR(X_CHAR("Failed to open file \"%s\" (code: ") X_RESULT_FORMAT_STRING X_CHAR(")"), fileName, r);
			}
		}
	}

	return r;
}

xResult xJsonStorageProvider::Save(IxPropertyEnum* propertyEnum, json_t** jsonResult)
{
	X_ASSERT(propertyEnum && jsonResult);

	if (!propertyEnum || !jsonResult)
	{
		return X_E_INVALID_ARGUMENT;
	}

	const uint32_t count = propertyEnum->GetCount();

	xResult r = count ? X_S_OK : X_E_NO_DATA;

	json_t* json = NULL;

	for (uint32_t ui = 0; ui < count; ui++)
	{
		xInterfacePtr<IxProperty> p;

		xInterfacePtr<IxPropertyEnum> pe;

		// Value or array?
		if (xSucceeded(r = propertyEnum->Get(ui, &p)))
		{
			const char* name = p->GetName();

			if (name)
			{
				if (!json)
				{
					json = json_object();

					if (!json)
					{
						return X_E_OUT_OF_MEMORY;
					}
				}

				// Sanity check
				const bool isObject = json_is_object(json);

				X_ASSERT(isObject);

				if (!isObject)
				{
					X_LOG_ERROR(X_CHAR("Internal object type error for property \"%s\" (code: ") X_RESULT_FORMAT_STRING X_CHAR(")"), name, X_E_INTERNAL);

					continue;
				}

				json_t* jsonTemp = NULL;

				if (xSucceeded(r = Save(p, &jsonTemp)))
				{
					int32_t jr = json_object_set_new(json, name, jsonTemp);

					X_ASSERT(jr == 0);

					if (jr != 0)
					{
						json_decref(jsonTemp);

						X_LOG_WARNING(X_CHAR("Failed to set JSON object for property \"%s\" (code: ") X_RESULT_FORMAT_STRING X_CHAR(")"), xPtr<xChar>(xConvertUtf8ToUtf16(name)), r);
					}
				}
				else
				{
					X_LOG_WARNING(X_CHAR("Failed to save property data for property \"%s\" (code: ") X_RESULT_FORMAT_STRING X_CHAR(")"), xPtr<xChar>(xConvertUtf8ToUtf16(name)), r);

					r = X_S_OK;
				}
			}
			else
			{
				X_ASSERT(false);

				X_LOG_WARNING(X_CHAR("Failed to get property name at index \"%d\""), ui);
			}
		}
		else if (xSucceeded(r = propertyEnum->Get(ui, &pe)))
		{
			if (!json)
			{
				json = json_array();

				if (!json)
				{
					return X_E_OUT_OF_MEMORY;
				}
			}

			// Sanity check
			const bool isArray = json_is_array(json);

			X_ASSERT(isArray);

			if (!isArray)
			{
				X_LOG_ERROR(X_CHAR("Internal object type error for property \"%d\" (code: ") X_RESULT_FORMAT_STRING X_CHAR(")"), ui, X_E_INTERNAL);

				continue;
			}

			json_t* jsonTemp = NULL;

			if (xSucceeded(r = Save(pe, &jsonTemp)))
			{
				int32_t jr = json_array_append_new(json, jsonTemp);

				X_ASSERT(jr == 0);

				if (jr != 0)
				{
					json_decref(jsonTemp);

					X_LOG_WARNING(X_CHAR("Failed to append to JSON array at index \"%d\""), ui);
				}
			}
			else
			{
				X_LOG_WARNING(X_CHAR("Failed to save property enum data at index \"%d\" (code: ") X_RESULT_FORMAT_STRING X_CHAR(")"), ui, r);

				r = X_S_OK;
			}
		}
		else
		{
			X_ASSERT(false);

			X_LOG_ERROR(X_CHAR("Internal unknown object error in property enum at index \"%d\" (code: ") X_RESULT_FORMAT_STRING X_CHAR(")"), ui, r);

			r = X_S_OK;
		}
	}

	if (json)
	{
		*jsonResult = json;
	}

	X_ASSERT((xSucceeded(r) && (json)) || (xFailed(r) && !(json)));

	return r;
}

xResult xJsonStorageProvider::Save(IxProperty* property, json_t** jsonResult)
{
	X_ASSERT(property && jsonResult);

	if (!property || !jsonResult)
	{
		return X_E_INVALID_ARGUMENT;
	}

	xResult r = X_S_OK;

	const xPropertyType propertyType = property->GetType();

	json_t* json = NULL;

	switch (propertyType)
	{
		case xPropertyTypeInt8:
		{
			X_JSON_STORAGE_PROVIDER_SET_INTEGER_TYPE(int8_t);

			break;
		}
		case xPropertyTypeInt16:
		{
			X_JSON_STORAGE_PROVIDER_SET_INTEGER_TYPE(int16_t);

			break;
		}
		case xPropertyTypeInt32:
		{
			X_JSON_STORAGE_PROVIDER_SET_INTEGER_TYPE(int32_t);

			break;
		}
		case xPropertyTypeInt64:
		{
			X_JSON_STORAGE_PROVIDER_SET_INTEGER_TYPE(int64_t);

			break;
		}
		case xPropertyTypeUint8:
		{
			X_JSON_STORAGE_PROVIDER_SET_INTEGER_TYPE(uint8_t);

			break;
		}
		case xPropertyTypeUint16:
		{
			X_JSON_STORAGE_PROVIDER_SET_INTEGER_TYPE(uint16_t);

			break;
		}
		case xPropertyTypeUint32:
		{
			X_JSON_STORAGE_PROVIDER_SET_INTEGER_TYPE(uint32_t);

			break;
		}
		case xPropertyTypeUint64:
		{
			X_JSON_STORAGE_PROVIDER_SET_INTEGER_TYPE(uint64_t);

			break;
		}
		case xPropertyTypeString:
		{
			xPtr<xChar> value;

			if (xFailed(r = property->GetValue(&value)))
			{
				return r;
			}

			if (!(json = json_string(xPtr<char>(xConvertUtf16ToUtf8(value)))))
			{
				r = X_E_OUT_OF_MEMORY;
			}

			break;
		}
		case xPropertyTypeInterface:
		{
			xInterfacePtr<IxInterface> p;

			if (xSucceeded(r = property->GetValue(&p)))
			{
				xInterfacePtr<IxPropertyEnum> propertyEnum;

				xInterfacePtr<IxStringEnum> stringEnum;

				if (xSucceeded(r = p->QueryInterface(IID_IxPropertyEnum, (void**) &propertyEnum)))
				{
					json_t* jsonPropertyEnum = NULL;

					if (xSucceeded(r = Save(propertyEnum, &jsonPropertyEnum)))
					{
						json = jsonPropertyEnum;
					}
				}
				else if (xSucceeded(r = p->QueryInterface(IID_IxStringEnum, (void**) &stringEnum)))
				{
					json_t* jsonStringEnum = NULL;

					if (xSucceeded(r = Save(stringEnum, &jsonStringEnum)))
					{
						json = jsonStringEnum;
					}
				}
				else
				{
					X_ASSERT(false);

					r = X_E_INTERNAL;
				}
			}
			else
			{
				X_ASSERT(false);
			}

			break;
		}
		case xPropertyTypeGuid:
		{
			xGuid value;

			xPtr<xChar> guid;

			if (xSucceeded(r = property->GetValue(&value)) && 
				xGuidToString(value, &guid))
			{
				if (!(json = json_string(xPtr<char>(xConvertUtf16ToUtf8(guid)))))
				{
					r = X_E_OUT_OF_MEMORY;
				}
			}
			else
			{
				X_ASSERT(false);
			}

			break;
		}
		default:
		{
			X_ASSERT(false);

			r = X_E_INTERNAL;
		}
	}

	if (json)
	{
		*jsonResult = json;
	}

	X_ASSERT(r != X_E_OUT_OF_MEMORY);
	X_ASSERT(r != X_E_INTERNAL);

	X_ASSERT((xSucceeded(r) && (json)) || (xFailed(r) && !(json)));

	return r;
}

xResult xJsonStorageProvider::Save(IxStringEnum* stringEnum, json_t** json)
{
	X_ASSERT(stringEnum && json);

	if (!stringEnum || !json)
	{
		return X_E_INVALID_ARGUMENT;
	}

	json_t* jsonArray = json_array();

	if (!jsonArray)
	{
		return X_E_OUT_OF_MEMORY;
	}

	xResult r = X_S_OK;

	const uint32_t count = stringEnum->GetCount();

	for (uint32_t ui = 0; ui < count; ui++)
	{
		xPtr<xChar> s;

		if (xSucceeded(r = stringEnum->Get(ui, &s)))
		{
			int32_t jr = json_array_append_new(jsonArray, json_string(xPtr<char>(xConvertUtf16ToUtf8(s))));

			X_ASSERT(jr == 0);
		}
		else
		{
			X_ASSERT(false);
		}
	}

	*json = jsonArray;

	return r;
}
