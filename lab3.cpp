#include <windows.h>
#include <setupapi.h>
#include <devguid.h>   
#include <iostream>

#pragma comment(lib, "setupapi.lib")

int main()
{
    HDEVINFO deviceInfoSet = SetupDiGetClassDevs(
        &GUID_DEVCLASS_USB,
        NULL,
        NULL,
        DIGCF_PRESENT
    );
    if (deviceInfoSet == INVALID_HANDLE_VALUE) {
        std::cerr << "Eroare: Nu pot obține deviceInfoSet.\n";
        return 1;
    }

    SP_DEVINFO_DATA deviceInfoData;
    deviceInfoData.cbSize = sizeof(SP_DEVINFO_DATA);
    DWORD index = 0;

    while (SetupDiEnumDeviceInfo(deviceInfoSet, index, &deviceInfoData))
    {
        std::wcout << L"Dispozitiv " << index << L": ";

        TCHAR description[1024];
        if (SetupDiGetDeviceRegistryProperty(
            deviceInfoSet,
            &deviceInfoData,
            SPDRP_DEVICEDESC,
            NULL,
            (PBYTE)description,
            sizeof(description),
            NULL))
        {
            std::wcout << description << std::endl;
        }
        else
        {
            std::wcout << L"Descriere indisponibilă.\n";
        }

        index++;
    }

    SetupDiDestroyDeviceInfoList(deviceInfoSet);
    return 0;
}
