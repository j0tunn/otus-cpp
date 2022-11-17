#include "utils.h"

#include <vector>
#include <iostream>
#include <algorithm>
#include <exception>

using namespace std;

int main(int, char **) {
	vector<IPAddress> addresses;

	for(;;) {
		string line;
		getline(cin, line);
		if (line.empty()) {
			break;
		}

		try {
			addresses.push_back(parseIPAddress(line));
		} catch(exception& e) {
			cerr << e.what() << ": " << line << endl;
			return -1;
		}
	}

	sort(addresses.begin(), addresses.end(), [](const IPAddress& a, const IPAddress& b) {
		return toUInt32(a) > toUInt32(b);
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
