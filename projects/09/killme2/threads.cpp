#include <future>
#include <iostream>
#include <thread>

void not_multy_not_async_not_parallel() {
	std::cout << "\nnot_multy_not_async_not_parallel\n";

	std::cout << "Hello from worker!" << std::endl;
	const int count = 10;
	for (int i=0; i<count; ++i) {
		std::cout << "Executing task " << i << std::endl;
		std::this_thread::sleep_for(std::chrono::milliseconds(200));
	}
	std::cout << "Finished" << std::endl;

	std::cout << std::endl;
}


void multy_not_async_not_parallel() {
	std::cout << "\nnot_async_not_parallel\n";

	std::mutex m;
	auto worker = [&m](int count) {
		std::lock_guard<std::mutex> lck{m};
		std::cout << "Hello from worker!" << std::endl;
		for (int i=0; i<count; ++i) {
			std::cout << "Executing task " << i << std::endl;
			std::this_thread::sleep_for(std::chrono::milliseconds(200));
		}
		std::cout << "Finished" << std::endl;
	};

	std::thread workerThread{worker, 10};
	std::thread workerThread2{worker, 10};
	
	workerThread.join();
	workerThread2.join();

	std::cout << std::endl;
}


void multy_not_async_parallel() {
	std::cout << "\nmulty_not_async_parallel\n";
	
	auto worker = [](int count) {
		std::cout << "Hello from worker!" << std::endl;
		for (int i=0; i<count; ++i) {
			std::cout << "Executing task " << i << std::endl;
			std::this_thread::sleep_for(std::chrono::milliseconds(200));
		}
		std::cout << "Finished" << std::endl;
	};

	std::thread workerThread1{worker, 10};
	std::thread workerThread2{worker, 10};

	worker(10);

	workerThread1.join();
	workerThread2.join();


	std::cout << std::endl;
}


void multy_async_paraller() {
	std::cout << "\nmulty_async_not_paraller\n";

	auto worker = [](int count) {
		std::cout << "Hello from worker!" << std::endl;
		for (int i=0; i<count; ++i) {
			std::cout << "Executing task " << i << std::endl;
			std::this_thread::sleep_for(std::chrono::milliseconds(200));
		}
		std::cout << "Finished" << std::endl;
	};

	std::cout << "Preparation" << std::endl;
	auto future1 = std::async(std::launch::async, worker, 10);
	auto future2 = std::async(std::launch::async, worker, 10);

	std::cout << "Execution" << std::endl;
	future1.get();
	future2.get();

	std::cout << std::endl;
}




int main() {
	not_multy_not_async_not_parallel();
	multy_not_async_not_parallel();
	multy_not_async_parallel();
	multy_async_paraller();

	return 0;
}