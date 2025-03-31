#include <windows.h>
#include <iostream>

int main()
{
    SYSTEM_INFO sysInfo;
    GetSystemInfo(&sysInfo);
    DWORD pageSize = sysInfo.dwPageSize;
    std::cout << "Dimensiunea unei pagini: " << pageSize << " bytes" << std::endl;

    std::cout << "Introdu numarul N (numarul de pagini): ";
    int N;
    std::cin >> N;
    if (N <= 0) {
        std::cerr << "Eroare: N trebuie sa fie un numar pozitiv." << std::endl;
        return 1;
    }

    SIZE_T allocationSize = static_cast<SIZE_T>(N) * pageSize;

    LPVOID lpMem = VirtualAlloc(
        NULL,                           
        allocationSize,                 
        MEM_RESERVE | MEM_COMMIT,       
        PAGE_READWRITE                  
    );
    if (lpMem == NULL) {
        std::cerr << "VirtualAlloc a esuat." << std::endl;
        return 1;
    }
    std::cout << "Memorie alocata cu succes la adresa: " << lpMem << std::endl;

    ZeroMemory(lpMem, allocationSize);

    if (VirtualFree(lpMem, 0, MEM_RELEASE) == 0) {
        std::cerr << "VirtualFree a esuat." << std::endl;
        return 1;
    }
    std::cout << "Memorie eliberata cu succes." << std::endl;

    return 0;
}
