#include "xUserAccountDataCollector.h"

#include "xDataEnum.h"
#include "xUserAccountData.h"

#include "xStringEnum.h"



xUserAccountDataCollector::xUserAccountDataCollector()
{

}

xUserAccountDataCollector::~xUserAccountDataCollector()
{

}

xResult xUserAccountDataCollector::Collect(IxUserAccountDataEnum** userAccountDataEnum)
{
	X_ASSERT(userAccountDataEnum);

	if (!userAccountDataEnum)
	{
		return X_E_INVALID_ARGUMENT;
	}

	xInterfacePtr<xUserAccountDataEnum> uade(new xUserAccountDataEnum);

	if (!uade)
	{
		return X_E_OUT_OF_MEMORY;
	}

	xResult r = X_S_OK;

	uint8_t* buffer = NULL;

	DWORD entries = 0;

	DWORD totalEntries;

	DWORD result = NetUserEnum(NULL, 3, 0, &buffer, MAX_PREFERRED_LENGTH, &entries, &totalEntries, NULL);

	if (result == NERR_Success || result == ERROR_MORE_DATA)
	{
		USER_INFO_3* ui3 = (USER_INFO_3*) buffer;

		for (uint32_t ui = 0; ui < entries; ui++)
		{
			xInterfacePtr<IxUserAccountData> uad;

			if (xSucceeded(r = Collect(ui3, &uad)))
			{
				uade->Add(uad);
			}

			ui3++;
		}

		NetApiBufferFree(buffer);
	}

	*userAccountDataEnum = uade.Detach();

	return X_S_OK;
}

xResult xUserAccountDataCollector::Collect(const USER_INFO_3* userInfo, IxUserAccountData** userAccountData)
{
	X_ASSERT(userAccountData);

	if (!userAccountData)
	{
		return X_E_INVALID_ARGUMENT;
	}

	xInterfacePtr<xUserAccountData> uad(new xUserAccountData);

	if (!uad)
	{
		return X_E_INVALID_ARGUMENT;
	}

	if (userInfo->usri3_name && xCharLength(userInfo->usri3_name))
	{
		uad->SetName(userInfo->usri3_name);
	}

	uad->SetPasswordAge(userInfo->usri3_password_age);

	uad->SetPrivileges(userInfo->usri3_priv);

	if (userInfo->usri3_home_dir && xCharLength(userInfo->usri3_home_dir))
	{
		uad->SetHomeDirectory(userInfo->usri3_home_dir);
	}

	uad->SetControlFlags(userInfo->usri3_flags);

	if (userInfo->usri3_script_path && xCharLength(userInfo->usri3_script_path))
	{
		uad->SetScriptPath(userInfo->usri3_script_path);
	}

	uad->SetAuthenticationFlags(userInfo->usri3_auth_flags);

	if (userInfo->usri3_full_name && xCharLength(userInfo->usri3_full_name))
	{
		uad->SetFullName(userInfo->usri3_full_name);
	}

	uad->SetLastLogon(userInfo->usri3_last_logon);

	uad->SetAccountExpires(userInfo->usri3_acct_expires);

	uad->SetBadPasswordCount(userInfo->usri3_bad_pw_count);

	uad->SetLogonCount(userInfo->usri3_num_logons);

	if (userInfo->usri3_logon_server && xCharLength(userInfo->usri3_logon_server))
	{
		uad->SetLogonServer(userInfo->usri3_logon_server);
	}

	uad->SetCountryCode(userInfo->usri3_country_code);

	uad->SetCodePage(userInfo->usri3_code_page);

	uad->SetUserId(userInfo->usri3_user_id);

	uad->SetPrimaryGroupId(userInfo->usri3_primary_group_id);

	if (userInfo->usri3_profile && xCharLength(userInfo->usri3_profile))
	{
		uad->SetProfile(userInfo->usri3_profile);
	}

	if (userInfo->usri3_home_dir_drive && xCharLength(userInfo->usri3_home_dir_drive))
	{
		uad->SetHomeDirectoryDrive(userInfo->usri3_home_dir_drive);
	}

	uad->SetPasswordExpired(userInfo->usri3_password_expired);

	*userAccountData = uad.Detach();

	return X_S_OK;
}
