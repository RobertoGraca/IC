#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <stdio.h>
#include <string>
#include <cmath>
#include <map>
#include <vector>
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

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        cout << "USAGE: ./ex1 <image1>" << endl;
        exit(1);
    }

    Mat rgb;
    rgb = imread(argv[1], IMREAD_COLOR);

    Mat yuv;
    cvtColor(rgb, yuv, COLOR_BGR2YUV_I420);

    vector<int> Y, U, V;
    for (int y = 0; y < yuv.cols; y++)
    {
        for (int x = 0; x < yuv.rows; x++)
        {
            if (y == 0 || x == 0)
            {
                Y.push_back((int)yuv.at<Vec3b>(y, x)[0]);
                if (y % 2 == 0 && x % 2 == 0)
                {
                    U.push_back((int)yuv.at<Vec3b>(y, x)[1]);
                    V.push_back((int)yuv.at<Vec3b>(y, x)[2]);
                }

                continue;
            }

            Y.push_back(jpeg_ls_prediction((int)yuv.at<Vec3b>(y - 1, x)[0], (int)yuv.at<Vec3b>(y, x - 1)[0], (int)yuv.at<Vec3b>(y - 1, x - 1)[0]));
            if (y % 2 == 0 && x % 2 == 0)
            {
                U.push_back(jpeg_ls_prediction((int)yuv.at<Vec3b>(y - 2, x)[1], (int)yuv.at<Vec3b>(y, x - 2)[1], (int)yuv.at<Vec3b>(y - 2, x - 2)[1]));
                V.push_back(jpeg_ls_prediction((int)yuv.at<Vec3b>(y - 2, x)[2], (int)yuv.at<Vec3b>(y, x - 2)[2], (int)yuv.at<Vec3b>(y - 2, x - 2)[2]));
            }
        }
    }
    // Assume the channels have a Gaussian distribution
    float mean_y = 0;
    for (auto &i : Y)
    {
        mean_y += i;
    }
    mean_y /= Y.size(); // Y channel mean
    float var_y = 0;
    for (auto &i : Y)
    {
        var_y += pow(i - mean_y, 2);
    }
    var_y /= Y.size();
    var_y = sqrt(var_y); // Y channel standard deviation

    float mean_u = 0;
    for (auto &i : U)
    {
        mean_u += i;
    }
    mean_u /= U.size(); // U channel mean
    float var_u = 0;
    for (auto &i : Y)
    {
        var_u += pow(i - mean_u, 2);
    }
    var_u /= Y.size();
    var_u = sqrt(var_u); // U channel standard deviation

    float mean_v = 0;
    for (auto &i : V)
    {
        mean_v += i;
    }
    mean_v /= V.size(); // V channel mean
    float var_v = 0;
    for (auto &i : Y)
    {
        var_v += pow(i - mean_v, 2);
    }
    var_v /= Y.size();
    var_v = sqrt(var_v); // V channel standard deviation
    /*map<int, int> count_y, count_u, count_v;
    for (int i = 0; i < Y.size(); i++)
    {
        int tmp = Y.at(i);
        if (count_y.count(tmp) > 0)
        {
            count_y.at(tmp) += 1;
        }
        else
        {
            count_y.emplace(tmp, 1);
        }
    }
    for (int i = 0; i < U.size(); i++)
    {
        int tmp_u = U.at(i);
        if (count_u.count(tmp_u) > 0)
        {
            count_u.at(tmp_u) += 1;
        }
        else
        {
            count_u.emplace(tmp_u, 1);
        }

        int tmp_v = V.at(i);
        if (count_v.count(tmp_v) > 0)
        {
            count_v.at(tmp_v) += 1;
        }
        else
        {
            count_v.emplace(tmp_v, 1);
        }
    }
    sort_map(count_y);

    for (auto &it : count_y)
    {

        cout << it.first << ' '
             << it.second << endl;
    }

    vector<float> weight_y, weight_u, weight_v;
    for (auto i = Y.cbegin(); i != Y.cend(); i++)
    {
        weight_y.push_back(count_y.at(*i) / Y.size());
    }
    for (auto i = U.cbegin(); i != U.cend(); i++)
    {
        weight_u.push_back(count_u.at(*i) / U.size());
    }
    for (auto i = V.cbegin(); i != V.cend(); i++)
    {
        weight_v.push_back(count_v.at(*i) / V.size());
    }*/
    // waitKey(0);
    return 0;
}