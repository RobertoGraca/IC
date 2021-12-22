#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <stdio.h>
#include <string>
#include <cmath>
#include <map>
#include <vector>
#include "../a/Golomb.h"
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

bool cmp(pair<int, int> &a, pair<int, int> &b)
{
    return (a.first < b.first);
}

void sort_map(map<int, int> &m)
{
    vector<pair<int, int>> v;
    for (auto &i : m)
    {
        v.push_back(i);
    }
    sort(v.begin(), v.end(), cmp);
    /*map<int, int> new_map;
    for (auto &i : v)
    {
        new_map.emplace(i.first, i.second);
    }
    return new_map;*/
}

_Float32 vector_mean(vector<int> x)
{
    _Float32 mean = 0;
    for (auto &i : x)
    {
        mean += i;
    }
    return mean / x.size();
}

_Float32 vector_std_dev(vector<int> x)
{
    _Float32 var = 0;
    _Float32 mean_x = vector_mean(x);
    for (auto &i : x)
    {
        var += pow(i - mean_x, 2);
    }
    var /= x.size();
    return sqrt(var);
}

void push(vector<int> &x, int *elem, int n)
{
    for (int i = 0; i < n; i++)
    {
        x.push_back(elem[i]);
    }
}

// g++ ex1.cpp -std=c++11 `pkg-config --cflags --libs opencv`
int main(int argc, char **argv)
{
    if (argc != 2)
    {
        cout << "USAGE: ./ex1 <image1>" << endl;
        exit(1);
    }
    Mat rgb;
    rgb = imread(argv[1], IMREAD_COLOR);

    if (rgb.empty())
    {
        cout << "Could not load or find image. Please try again!" << endl;
        exit(1);
    }

    Mat yuv;
    cvtColor(rgb, yuv, COLOR_BGR2YUV_I420);
    // imshow("yuv", yuv);
    //  cout << rgb.cols << "x" << rgb.rows << endl;
    //  cout << yuv.cols << "x" << yuv.rows << endl;

    vector<int> Y, U, V;
    for (int y = 0; y < yuv.cols; y++)
    {
        for (int x = 0; x < yuv.rows; x++)
        {
            cout << y << "\t" << x << endl;
            int elem[3] = {(int)yuv.at<Vec3b>(y, x)[0], (int)yuv.at<Vec3b>(y, x)[1], (int)yuv.at<Vec3b>(y, x)[2]};
            if (y == 0)
            {
                if (x < rgb.rows)
                {
                    push(Y, elem, 3);
                }

                else if (x >= rgb.rows && x < yuv.rows - (rgb.rows / 4))
                {
                    push(U, elem, 3);
                }

                else
                {
                    push(V, elem, 3);
                }
                continue;
            }
            else
            {
                if (x == 0)
                {
                    push(Y, elem, 3);
                    continue;
                }
                else if (x == rgb.rows)
                {
                    push(U, elem, 3);
                    continue;
                }
                else if (x == yuv.rows - (rgb.rows / 4))
                {
                    push(V, elem, 3);
                    continue;
                }
            }
            int a[3] = {(int)yuv.at<Vec3b>(y - 1, x)[0], (int)yuv.at<Vec3b>(y - 1, x)[1], (int)yuv.at<Vec3b>(y - 1, x)[2]};
            int b[3] = {(int)yuv.at<Vec3b>(y, x - 1)[0], (int)yuv.at<Vec3b>(y, x - 1)[1], (int)yuv.at<Vec3b>(y, x - 1)[2]};
            int c[3] = {(int)yuv.at<Vec3b>(y - 1, x - 1)[0], (int)yuv.at<Vec3b>(y - 1, x - 1)[1], (int)yuv.at<Vec3b>(y - 1, x - 1)[2]};
            if (x < rgb.rows)
            {
                int pred[3];
                pred[0] = jpeg_ls_prediction(a[0], b[0], c[0]);
                pred[1] = jpeg_ls_prediction(a[1], b[1], c[1]);
                pred[2] = jpeg_ls_prediction(a[2], b[2], c[2]);
                push(Y, pred, 3);
            }
            else if (x >= rgb.rows && x < yuv.rows - (rgb.rows / 4))
            {
                int pred[3];
                pred[0] = jpeg_ls_prediction(a[0], b[0], c[0]);
                pred[1] = jpeg_ls_prediction(a[1], b[1], c[1]);
                pred[2] = jpeg_ls_prediction(a[2], b[2], c[2]);
                push(U, pred, 3);
            }
            else
            {
                int pred[3];
                pred[0] = jpeg_ls_prediction(a[0], b[0], c[0]);
                pred[1] = jpeg_ls_prediction(a[1], b[1], c[1]);
                pred[2] = jpeg_ls_prediction(a[2], b[2], c[2]);
                push(V, pred, 3);
            }
        }
    }

    cout << Y.size() << endl;
    cout << U.size() << endl;
    cout << V.size() << endl;
    cout << (Y.size() / 4 == U.size()) << endl;
    ofstream ofs("yuv_values.txt");
    for (int i = 0; i < Y.size(); i++)
    {
        ofs << Y.at(i);
        ofs << "\t";
        if (i % 4 == 0)
        {
            ofs << U.at(i / 4);
            ofs << "\t";
            ofs << V.at(i / 4);
        }

        ofs << "\n";
    }
    ofs.close();
    // Assume the channels have a Gaussian distribution
    /* _Float32 mean_y = vector_mean(Y);   // Y channel mean
    _Float32 var_y = vector_std_dev(Y); // Y channel standard deviation

    _Float32 mean_u = vector_mean(U);   // U channel mean
    _Float32 var_u = vector_std_dev(U); // U channel standard deviation

    _Float32 mean_v = vector_mean(V);   // V channel mean
    _Float32 var_v = vector_std_dev(V); // V channel standard deviation */
    // waitKey(0);
    return 0;
}