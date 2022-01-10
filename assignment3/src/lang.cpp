#include "fcm.hpp"

int main(int argc, char **argv)
{
    if (argc == 3)
    {
        // no order context model and no probability estimator given
    }
    else if (argc == 4)
    {
        // no probability estimator given
    }
    else if (argc == 5)
    {
        // all arguments given
    }
    else
    {
        cout << "USAGE: ./lang <file_1> <file_2> [order_model] [probability_estimator]" << endl;
        exit(1);
    }

    return 0;
}