#include <iostream>
#include <mutex>
#include <thread>
#include <queue>

struct Buyer {
	void buy() {
		std::cout << "Buyer - buying now." << std::endl;
	}
};	

std::mutex mutex;
std::queue<Buyer> buyers;

void cashier() {
	const int pageCount = 30;

	std::cout << "cashier started!" << std::endl;
	for(int pageNumber=0; pageNumber<pageCount; ++pageNumber) {
		std::cout << "Reading page " << pageNumber << std::endl;
		std::this_thread::sleep_for(std::chrono::milliseconds(500));

		{
			std::lock_guard<std::mutex> guard{mutex};
			if (!buyers.empty()) {
				auto& buyer = buyers.front();
				buyers.pop();
				buyer.buy();
			}
			else
				std::cout << "No buyers" << std::endl;
		}
	}
	std::cout << "cashier finished!" << std::endl;
}


int main() {
	std::thread cashierThread{cashier};

	const int buyerCount = 10;

	for(int i=0; i<buyerCount; ++i) {
		{
			std::lock_guard<std::mutex> guard{mutex};
			buyers.push(Buyer{});
			std::cout << "Push buyer" << std::endl;
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}

	cashierThread.join();

	return 0;
}