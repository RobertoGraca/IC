#include <iostream>
#include "fcm.hpp"

void show_menu()
{
    cout << endl;
    cout << "________________Menu________________" << endl;
    cout << "| 1- Read file from path------------|" << endl;
    cout << "| 2- Show model index---------------|" << endl;
    cout << "| 3- Show alphabet------------------|" << endl;
    cout << "| 4- Show model entropy-------------|" << endl;
    cout << "| 5- Load model index from file-----|" << endl;
    cout << "| 6- Write model index to file------|" << endl;
    cout << "| 7- Set parameters values(k, alpha)|" << endl;
    cout << "| 0- Exit---------------------------|" << endl;
    cout << "‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾" << endl;
    cout << endl;
}

int main(int argc, char **argv)
{
    int option;
    int k, alpha = 0;
    float entropy;
    string path;
    FCM fcm(1, 1);
    do
    {
        show_menu();
        do
        {
            cout << "Choice: ";
            cin >> option;
        } while (option < 0 || option > 7);

        switch (option)
        {
        case 0:
            cout << "Exiting..." << endl;
            exit(0);
        case 1:
            if (alpha == 0)
            {
                cout << "Parameters values not set. Try running option 7 first!" << endl;
                break;
            }
            cout << "Path to file: ";
            cin >> path;
            fcm = FCM(k, alpha);
            fcm.read_file(path);
            break;
        case 2:
            if (fcm.get_alphabet_size() == 0)
            {
                cout << "Finite context model is empty. Try running options 1 or 5 first!" << endl;
                break;
            }
            fcm.show_index();
            break;
        case 3:
            if (fcm.get_alphabet_size() == 0)
            {
                cout << "Finite context model is empty. Try running options 1 or 5 first!" << endl;
                break;
            }
            fcm.show_alphabet();
            break;
        case 4:
            if (fcm.get_alphabet_size() == 0)
            {
                cout << "Finite context model is empty. Try running options 1 or 5 first!" << endl;
                break;
            }
            cout << "This may take a while depending on the size of the model!" << endl;
            entropy = fcm.get_model_entropy();
            cout << "Entropy of the model is " << entropy << endl;
            break;
        case 5:
            if (alpha == 0)
            {
                cout << "Parameters values not set. Try running option 7 first!" << endl;
                break;
            }
            cout << "Path to source file: ";
            cin >> path;
            fcm.load_index(path);
            cout << "File " << path << " was read and indexed" << endl;
            break;
        case 6:
            if (fcm.get_alphabet_size() == 0)
            {
                cout << "Finite context model is empty. Try running options 1 or 5 first!" << endl;
                break;
            }
            cout << "Path to destination file: ";
            cin >> path;
            fcm.write_index(path);
            cout << "File " << path << " was written" << endl;
            break;
        case 7:
            cout << "Parameters to create Finite Context Model" << endl;
            do
            {
                cout << "Context Size (1 to 5): ";
                cin >> k;
            } while (k < 0 || k > 5);

            do
            {
                cout << "Smoothing parameter (1 to 100): ";
                cin >> alpha;
            } while (alpha < 1 || alpha > 100);
            cout << "Parameters context size and smoothing parameter were set." << endl;
        }
    } while (option != 0);

    return 0;
}