#include "subset.h"

#include "randomized_queue.h"

void subset(unsigned long k, std::istream & in, std::ostream & out)
{
    randomized_queue<std::string> distributor;
    std::string line;
    while (std::getline(in, line)) {
        distributor.enqueue(line);
    }
    while (k > 0 && !distributor.empty()) {
        out << distributor.dequeue() << std::endl;
        --k;
    }
}