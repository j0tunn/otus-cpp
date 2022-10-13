#include "utils.h"

#include <iostream>
#include <algorithm>

using namespace std;

int main(int, char **) {
	vector<IPAddress> addresses;

	for(;;) {
		string line;
		getline(cin, line);
		if (line.empty()) {
			break;
		}

		addresses.push_back(parseIPAddress(line));
	}

	sort(addresses.begin(), addresses.end(), [](const IPAddress& a, const IPAddress& b) {
		return a[0] > b[0]
			|| a[0] == b[0] && a[1] > b[1]
			|| a[0] == b[0] && a[1] == b[1] && a[2] > b[2]
			|| a[0] == b[0] && a[1] == b[1] && a[2] == b[2] && a[3] > b[3];
	});

	for (auto addr: addresses)
		cout << toString(addr) << endl;

	vector<IPAddress> filteredAddresses;

	copy_if(addresses.begin(), addresses.end(), back_inserter(filteredAddresses),
		[](const IPAddress& ip) { return ip[0] == 1; });
	copy_if(addresses.begin(), addresses.end(), back_inserter(filteredAddresses),
		[](const IPAddress& ip) { return ip[0] == 46 && ip[1] == 70; });
	copy_if(addresses.begin(), addresses.end(), back_inserter(filteredAddresses),
		[](const IPAddress& ip) { return ip[0] == 46 || ip[1] == 46 || ip[2] == 46 || ip[3] == 46; });

	for (auto addr: filteredAddresses)
		cout << toString(addr) << endl;

	return 0;
}
