#include <opencv2/highgui.hpp>
#include <iostream>
using namespace cv;

int main(int argc, char **argv)
{
    if (argc != 3)
        return -1;

    Mat img;
    img = imread(argv[1], IMREAD_COLOR);

    if (!img.data)
    {
        std::cout << "Image not found or unable to open" << std::endl;
        return -2;
    }

    Mat img2 = Mat::zeros(cv::Size(img.cols, img.rows), CV_8UC3);

    for (int y = 0; y < img.rows; y++)
    {
        for (int x = 0; x < img.cols; x++)
        {
            img2.at<Vec3b>(y, x) = img.at<Vec3b>(y, x);
        }
    }

    imwrite(argv[2], img2);

    return 0;
}