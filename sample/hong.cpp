#include <iostream>
using namespace std;
int main()
{
#if __cplusplus >= 201103L || (defined(_MSC_VER) && _MSC_VER >= 1900)
    cout << "C++11 is supported";
#else
    cout << "C++11 is not supported";
#endif
 
    return 0;
}
