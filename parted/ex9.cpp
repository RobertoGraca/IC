#include <iostream>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
using namespace std;
using namespace cv;

int main(int argc, char **argv)
{
    if (argc != 3)
    {
        cout << "USAGE: ./program <image_input> <bits_to_reduce>" << endl;
        return -1;
    }

    Mat image;
    image = imread(argv[1], IMREAD_COLOR);

    if (image.empty())
    {
        cout << "Could not load or find image. Please try again!" << endl;
        return -2;
    }

    Mat image2 = Mat::zeros(Size(image.cols, image.rows), CV_8UC3);

    for (int y = 0; y < image.cols; y++)
    {
        for (int x = 0; x < image.rows; x++)
        {
            image2.at<Vec3b>(y, x)[0] = image.at<Vec3b>(y, x)[0] >> stoi(argv[2]);
            image2.at<Vec3b>(y, x)[0] = image2.at<Vec3b>(y, x)[0] << stoi(argv[2]);
            image2.at<Vec3b>(y, x)[1] = image.at<Vec3b>(y, x)[1] >> stoi(argv[2]);
            image2.at<Vec3b>(y, x)[1] = image2.at<Vec3b>(y, x)[1] << stoi(argv[2]);
            image2.at<Vec3b>(y, x)[2] = image.at<Vec3b>(y, x)[2] >> stoi(argv[2]);
            image2.at<Vec3b>(y, x)[2] = image2.at<Vec3b>(y, x)[2] << stoi(argv[2]);
        }
    }
    imshow("Original", image);
    imshow("Reduced", image2);
    waitKey();
    return 0;
}