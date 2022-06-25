#include "SegmentedVector.hpp"

int main() {
    SegmentedVector sg = {1, 2, 3};
    sg.print_me();

    sg.push_back(2);
    sg.push_back(2);
    sg.push_back(2);
    sg.push_back(2);
    sg.push_back(2);
    sg.push_back(2);
    sg.push_back(2);
    sg.push_back(2);

    sg.print_me();

    return 0;
}