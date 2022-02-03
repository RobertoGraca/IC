#include "lang.hpp"
#include <dirent.h>

// USAGE:
// $ g++ findlang.cpp -o findlang
// $ ./findlang cache example.txt 4 50 - to search using cache
// $ ./findlang langs example.txt 4 50 - to search using the fcm

int main(int argc, char **argv)
{
    if (argc != 5)
    {
        cout << "USAGE: ./findlang <reference_texts_directory> <target_text_file> <context_size> <smoothing_percentage>" << endl;
        exit(1);
    }

    DIR *dir;
    struct dirent *ent;
    string path;
    string directory;
    map<float, string> langs;

    directory = argv[1];
    if (directory == "cache")
    {
        directory += "/k";
        directory += argv[3];
    }

    if ((dir = opendir(directory.c_str())) != NULL)
    {
        /* print all the files and directories within directory */
        while ((ent = readdir(dir)) != NULL)
        {
            // gets the path for the current file/language dataset
            path = ent->d_name;

            if (path == ".." || path == ".")
                continue;

            cout << "File: " << path << endl;

            // creates the new 'lang' object
            lang temp(directory + "/" + path, argv[2], atoi(argv[3]), (float)atoi(argv[4]));

            // calculates and stores the estimated number of bits and the corresponding language on the map
            langs.emplace(temp.get_estimated_bits(), path.substr(0, path.find_last_of(".")).substr(path.find("/") + 1));
        }
        closedir(dir);
    }
    else
    {
        /* could not open directory */
        perror("Could not open directory!");
        return EXIT_FAILURE;
    }

    /* map<float, string> langs;
    string path;

    // iterates over all files in the folder passed as argument
    for (auto const &entry : filesystem::directory_iterator(argv[1]))
    {
        // gets the path for the current file/language dataset
        path = entry.path();

        cout << "File: " << path << endl;

        // creates the new 'lang' object
        lang temp(path, argv[2], atoi(argv[3]), (float)atoi(argv[4]));

        // calculates and stores the estimated number of bits and the corresponding language on the map
        langs.emplace(temp.get_estimated_bits(), path.substr(0, path.find_last_of(".")).substr(path.find("/") + 1));
    } */

    // prints the estimated number of bits for every language
    cout << "" << endl;
    cout << "Results:" << endl;
    for (auto item : langs)
    {
        cout << item.second << " -> " << item.first << endl;
    }
    cout << "" << endl;

    // gets the language with the lowest estimated number of bits
    auto min_key = langs.begin()->second;

    cout << "This text is most likely written in " << min_key << endl;

    return 0;
}