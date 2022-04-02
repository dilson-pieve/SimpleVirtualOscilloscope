#include "VirtualOscilloscope.hpp"

int main()
{
    VirtualOscilloscope vc;
    vc.Construct("Virtual Oscilloscope", 1000, 600);
    vc.Start();

    return 0;
}