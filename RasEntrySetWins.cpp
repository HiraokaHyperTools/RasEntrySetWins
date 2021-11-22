// RasEntrySetWins.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#define WINVER 0x500

#include <winsock2.h>
#include <ip2string.h>
#include <ras.h>
#include <stdio.h>

#pragma comment(lib, "rasapi32")
#pragma comment(lib, "ntdll")

int wmain(int argc, wchar_t** argv)
{
	static char buffer[65536] = { 0 };

	LPCWSTR terminator = NULL;
	if (true
		&& argc == 5
		&& _wcsicmp(argv[2], L"set-wins") == 0
		)
	{
		LPRASENTRYW entry = reinterpret_cast<LPRASENTRYW>(buffer);
		entry->dwSize = sizeof(RASENTRYW);
		DWORD bufferSize = sizeof(buffer);
		DWORD res;

		res = RasGetEntryPropertiesW(NULL, argv[1], entry, &bufferSize, NULL, NULL);
		if (res != 0) {
			wprintf(L"RasGetEntryPropertiesW failed on error code %u\n", res);
			return 1;
		}

		entry->dwfOptions |= RASEO_SpecificNameServers;

		{
			DWORD newAddr;
			if (wcslen(argv[3]) == 0) {
				*reinterpret_cast<LPDWORD>(&entry->ipaddrWins) = 0;
			}
			else if (RtlIpv4StringToAddressW(argv[3], false, &terminator, reinterpret_cast<in_addr*>(&newAddr)) == 0) {
				*reinterpret_cast<LPDWORD>(&entry->ipaddrWins) = newAddr;
			}
		}

		{
			DWORD newAddr;
			if (wcslen(argv[4]) == 0) {
				*reinterpret_cast<LPDWORD>(&entry->ipaddrWinsAlt) = 0;
			}
			else if (RtlIpv4StringToAddressW(argv[4], false, &terminator, reinterpret_cast<in_addr*>(&newAddr)) == 0) {
				*reinterpret_cast<LPDWORD>(&entry->ipaddrWinsAlt) = newAddr;
			}
		}

		res = RasSetEntryPropertiesW(NULL, argv[1], entry, bufferSize, NULL, 0);
		if (res != 0) {
			wprintf(L"RasSetEntryPropertiesW failed on error code %u\n", res);
			return 1;
		}
		wprintf(L"Success.\n");
		return 0;
	}

	wprintf(
		L"RasEntrySetWins vpnName set-wins 1.2.3.4 \"\"\n"
	);
	return 1;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
