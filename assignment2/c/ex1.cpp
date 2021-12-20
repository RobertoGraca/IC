#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <stdio.h>
#include <string>
#include <cmath>
using namespace std;
using namespace cv;

int jpeg_ls_prediction(int a, int b, int c)
{
    if (c >= max(a, b))
        return min(a, b);
    else if (c <= min(a, b))
        return max(a, b);
    else
        return a + b - c;
}

int main(int argc, char **argv)
{
    Mat rgb;
    rgb = imread(argv[1], IMREAD_COLOR);

    Mat yuv;
    cvtColor(rgb, yuv, COLOR_BGR2YUV_I420);
    // string name = argv[1];
    // name = name.substr(0, name.length() - 4);
    // imwrite(name + "_yuv.jpg", yuv);

    int matrix[yuv.cols][yuv.rows][3]; // prediction matrix
    vector<int> Y, U, V;
    for (int y = 0; y < yuv.cols; y++)
    {
        for (int x = 0; x < yuv.rows; x++)
        {
            if (y == 0 || x == 0)
            {
                Y.push_back((int)yuv.at<Vec3b>(y, x)[0]);
                U.push_back((int)yuv.at<Vec3b>(y, x)[1]);
                V.push_back((int)yuv.at<Vec3b>(y, x)[2]);
                continue;
            }

            Y.push_back(jpeg_ls_prediction((int)yuv.at<Vec3b>(y - 1, x)[0], (int)yuv.at<Vec3b>(y, x - 1)[0], (int)yuv.at<Vec3b>(y - 1, x - 1)[0]));
            U.push_back(jpeg_ls_prediction((int)yuv.at<Vec3b>(y - 1, x)[1], (int)yuv.at<Vec3b>(y, x - 1)[1], (int)yuv.at<Vec3b>(y - 1, x - 1)[1]));
            V.push_back(jpeg_ls_prediction((int)yuv.at<Vec3b>(y - 1, x)[2], (int)yuv.at<Vec3b>(y, x - 1)[2], (int)yuv.at<Vec3b>(y - 1, x - 1)[2]));

            // cout << (int)yuv.at<Vec3b>(y, x)[i] << "\t";
            // cout << endl;
        }
    }
    /*for (int y = 0; y < yuv.cols; y++)
    {
        for (int x = 0; x < yuv.rows; x++)
        {
            cout << yuv.at<Vec3b>(y, x)[0] - matrix[y][x][0] << "\t";
            cout << yuv.at<Vec3b>(y, x)[1] - matrix[y][x][1] << "\t";
            cout << yuv.at<Vec3b>(y, x)[2] - matrix[y][x][2] << endl;
            ;
        }
    }*/
    // waitKey(0);
    return 0;
}