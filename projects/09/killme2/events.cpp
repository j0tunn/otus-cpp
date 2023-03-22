#include <queue>
#include <thread>
#include <vector>

#include <cassert>

enum class EventCode {
    start, 
    new_doc, 
    quit
};

struct Event {
    EventCode code;
};


class Handler {
public:
    virtual bool event(Event &) = 0;
};


class EventLoop {
public:
    void send(EventCode code) {
        m_event_queue.push({code});
    }

    void add(Handler *handler) {
        m_handlers.push_back(handler);
    }

    int exec() {
        while (!m_quit) {
            if (!m_event_queue.empty()) {
                auto ev = m_event_queue.front();
                m_event_queue.pop();

                switch (ev.code) {
                	// Special event for stopping
                    case EventCode::quit:
                        m_quit = true;
                        break;
                    // All other events are handled by handlers
                    default:
                        for (auto handler : m_handlers) {
                        	assert(handler);
                        	if (handler->event(ev))
                        		break;
                        }
                }
            } else {
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
            }
        }

        return 0;
    }
private:
	bool m_quit = false;
    std::queue<Event> m_event_queue;
    std::vector<Handler *> m_handlers;
    std::thread m_eventLoopThread;
};

int main() {
	return 0;
}
