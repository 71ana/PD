/*1. Să se implementeze o aplicaţie ce afişează valoarea variabilei ImagePath a
tuturor subcheilor din HKLM\SYSTEM\CurrentControlSet\Services*/

#include <windows.h>
#include <winreg.h>
#include <stdio.h>

int wmain(void) {
	HKEY sysKey;
	LPCWSTR lpSubKey = L"SYSTEM\\CurrentControlSet\\Services";

    //deschidem cheia Services
    long lResult = RegOpenKeyEx(HKEY_LOCAL_MACHINE,
		                        lpSubKey,
		                        0,
		                        KEY_READ,
		                        &sysKey);

	if (lResult != ERROR_SUCCESS)
	{
		if (lResult == ERROR_FILE_NOT_FOUND) {
			printf("key not found.\n");
			return TRUE;
		}
		else {
			printf("Error opening key.\n");
			return FALSE;
		}
	}

    DWORD index = 0;
    WCHAR subKeyName[256];
    DWORD subKeyNameSize = 256;

    while (RegEnumKeyEx(sysKey, index, subKeyName, &subKeyNameSize, NULL, NULL, NULL, NULL) == ERROR_SUCCESS) {
        HKEY hSubKey;
        if (RegOpenKeyEx(sysKey, subKeyName, 0, KEY_READ, &hSubKey) == ERROR_SUCCESS) {
            WCHAR imagePath[512];
            DWORD imagePathSize = sizeof(imagePath);
            DWORD type;

            if (RegQueryValueEx(hSubKey, L"ImagePath", NULL, &type, (LPBYTE)imagePath, &imagePathSize) == ERROR_SUCCESS && (type == REG_EXPAND_SZ)) {
                wprintf(L"[%s] -> %s\n", subKeyName, imagePath);
            }

            RegCloseKey(hSubKey);
        }

        subKeyNameSize = 256;
        index++;
    }

    RegCloseKey(sysKey);

	return 0;
}