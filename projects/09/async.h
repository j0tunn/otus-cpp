#pragma once

#include <cstddef>

namespace async {

class Handle;

Handle* connect(std::size_t bulkSize);
void receive(Handle* h, const char *data, std::size_t size);
void disconnect(Handle* h);

}
