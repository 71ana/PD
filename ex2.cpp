//Să se implementeze o aplicaţie ce afişează driverele înregistrate în systemul de
//operare urmând următoarele indicații :
//Aplicația va fi scrisă urmând convenția de codare indicată
//Se afișează ImagePath doar a subcheilor ce au valoarea variabilei Type
//corespunzătoare unui driver

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
            DWORD type;
            DWORD typeSize = sizeof(type);
            if(RegQueryValueEx(hSubKey, L"Type", NULL, NULL, (LPBYTE)&type, &typeSize) == ERROR_SUCCESS)
                if (type == 0x01 || type == 0x02)
                {
                    WCHAR imagePath[512];
                    DWORD imagePathSize = sizeof(imagePath);
                    DWORD imagePathType;
                    if (RegQueryValueEx(hSubKey, L"ImagePath", NULL, &imagePathType, (LPBYTE)imagePath, &imagePathSize) == ERROR_SUCCESS) {                            
                        wprintf(L"[%s] -> %s\n", subKeyName, imagePath);
                    }
                }

            RegCloseKey(hSubKey);
        }

        subKeyNameSize = 256;
        index++;
    }

    RegCloseKey(sysKey);

	return 0;
}
