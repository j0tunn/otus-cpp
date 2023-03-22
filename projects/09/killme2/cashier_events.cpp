#include <iostream>
#include <mutex>
#include <thread>
#include <queue>

#include <cassert>

// 0. Declare events here
enum class Event {
	Reading,
	Buyer,
	Stop
};

// 1. Buer class
struct Buyer {
	void buy() {
		std::cout << "Buyer - buying now." << std::endl;
	}
};

// 2. Cashier class
class Cashier {
public:
	Cashier() : m_currentPage{0} {
		// Start thread in the constructor (instead of start method)
		m_workerThread = std::thread(&Cashier::workerThread, this);
	}

	~Cashier() {
		// Stop-and-join idiom
		stop();
		m_workerThread.join();
	}

	// Async read
	void read() {
		std::lock_guard<std::mutex> guard{m_mutex};
		m_events.push(Event::Reading);
	}

	// Async buy
	void buyer(const Buyer& buyer) {
		std::lock_guard<std::mutex> guard{m_mutex};
		m_events.push(Event::Buyer);
		m_buyers.push(buyer);
	}

	// Async stop
	void stop() {
		std::lock_guard<std::mutex> guard{m_mutex};
		m_events.push(Event::Stop);

		// Join here if blocking stop is expected
	}

private:
	void workerThread() {
		Event event;
		// Main worker cycle
		do {
			{
				std::unique_lock<std::mutex> guard{m_mutex};
				if (m_events.empty()) {
					guard.unlock();
					std::this_thread::sleep_for(std::chrono::milliseconds(1));
					// std::this_thread::yield();
					continue;
				}

				event = m_events.front();
				m_events.pop();
			}

			// handle each event without lock if possible
			switch(event) {
				case Event::Reading: {
					if (m_currentPage < m_totalPages) {
						std::cout << "Reading..." << std::endl;
						++m_currentPage;
						std::this_thread::sleep_for(std::chrono::milliseconds(100));
					}
					else {
						std::cout << "No more pages!" << std::endl;
					}
				}
				break;

				case Event::Buyer: {
					Buyer buyer;
					{
						// if no possible, use small lock
						std::lock_guard<std::mutex> lockGuard{m_mutex};
						assert(!m_buyers.empty());
						buyer = m_buyers.front();
						m_buyers.pop();
					}
					std::cout << "Next buyer" << std::endl;
					buyer.buy();
				}
				break;

				case Event::Stop:
					std::cout << "Stopping" << std::endl;
					break;

				default:
					assert(0);
			}

		} while(event != Event::Stop);
	}

	std::thread m_workerThread;
	std::mutex m_mutex;

	// events queue
	std::queue<Event> m_events;

	// buyers context
	std::queue<Buyer> m_buyers;


	// reading context
	const int m_totalPages = 10;
	int m_currentPage;
};


int main() {
	Cashier cashier;

	cashier.buyer(Buyer{});
	cashier.buyer(Buyer{});
	cashier.buyer(Buyer{});
	cashier.read();
	cashier.buyer(Buyer{});
	cashier.read();
	cashier.buyer(Buyer{});
	cashier.stop();

	return 0;
}
