#include <iostream>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <cmath>
using namespace std;
using namespace cv;

int main(int argc, char **argv)
{
    if (argc != 3)
    {
        cout << "USAGE: ./program <image_input> <image2_input>" << endl;
        return -1;
    }

    Mat image;
    string path = "../images/";
    image = imread(path + argv[1], IMREAD_COLOR);

    if (image.empty())
    {
        cout << "Could not load or find image. Please try again!" << endl;
        return -2;
    }

    Mat image2 = imread(path + argv[2], IMREAD_COLOR);

    if (image2.empty())
    {
        cout << "Could not load or find image2. Please try again!" << endl;
        return -2;
    }

    float mse = 0;

    for (int y = 0; y < image.cols; y++)
    {
        for (int x = 0; x < image.rows; x++)
        {
            mse += pow(image.at<Vec3b>(y, x)[0] - image2.at<Vec3b>(y, x)[0], 2) +
                   pow(image.at<Vec3b>(y, x)[1] - image2.at<Vec3b>(y, x)[1], 2) +
                   pow(image.at<Vec3b>(y, x)[2] - image2.at<Vec3b>(y, x)[2], 2);
        }
    }

    mse = mse / (image.cols * image.rows);

    float psnr = (20 * log10(255)) - (10 * log10(mse));
    string input = argv[1];
    string compare = argv[2];
    cout << "The PSNR (Peak Signal-to-Noise Ratio) between " + input + " and " + compare + " is " + to_string(psnr) + " dB" << endl;

    return 0;
}