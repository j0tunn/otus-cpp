#include <future>
#include <iostream>
#include <thread>
#include <vector>

void async() {
	std::cout << "\n *** async *** \n";

	// Potential slow
	auto doSomeJob = [](const int context) {
		std::this_thread::sleep_for(std::chrono::seconds(2));
		return context + 1;
	};

	doSomeJob(1);
	doSomeJob(2); // 4s 

	int value = 42;
	// Fast
	std::future<int> future1 = std::async(std::launch::async, doSomeJob, value); // 2s
	// Fast
	std::future<int> future2 = std::async(std::launch::async, doSomeJob, value); // 2s

	std::async(std::launch::async, doSomeJob, value); // 2s 

	// do some other job
	// .....
	// ....
	
	int result1 = future1.get();
	int result2 = future2.get();
	// ......... // ???? 2 + 2 = 4?  2s

	std::async(std::launch::async, doSomeJob, value);

	int i;
	i = 0;
	for (int j = 0; j < 10; j++) {

	}

}

void async_is_useless() {
	std::cout << "\n *** async_is_useless *** \n";

	auto func = []() {
		std::cout << std::this_thread::get_id() << std::endl;
	};


	std::vector<std::future<void>> futures;
	for (int i = 0; i < 20; ++i) {
		auto future = std::async(std::launch::async, func);
		futures.emplace_back(std::move(future));
	}

	for (auto& f : futures) {
		f.get();
	}
}


template <typename Fut, typename Work>
auto then(Fut f, Work w) -> std::future<decltype(w(f.get()))> {
	return std::async(
		std::launch::async, 
		[f = std::move(f), w = std::move(w)]() mutable { 
			return w( f.get() ); 
		}
	);
}

void then_example() {
	std::cout << "\n *** then_example *** \n";

	auto first_step = [](int input) {
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
		return input * 2;
	};
	auto second_step = [](int input) {
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
		return input * 3;
	};
	auto third_step = [](int input) {
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
		return input * 4;
	};

	auto future1 = std::async(std::launch::async, first_step, 1);
	auto future2 = then(std::move(future1), second_step);
	auto future3 = then(std::move(future2), third_step);
	
	std::cout << "result = " << future3.get() << std::endl;
}


int main() {

	async();
	async_is_useless();
	then_example();

	return 0;
}