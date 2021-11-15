#include <iostream>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
using namespace std;
using namespace cv;

void calculateHistogram(Mat image)
{
    vector<Mat> img_channels;
    split(image, img_channels);
    int histSize = 256;

    float range[] = {0, 256}; // the upper boundary is exclusive
    const float *histRange[] = {range};

    bool uniform = true, accumulate = false;

    Mat channel_hist[img_channels.size()];
    for (int i = 0; i < img_channels.size(); i++)
    {
        channel_hist[i] = Mat();
    }

    for (int i = 0; i < img_channels.size(); i++)
    {
        calcHist(&img_channels[i], 1, 0, Mat(), channel_hist[i], 1, &histSize, histRange, uniform, accumulate);
    }

    int hist_w = 512, hist_h = 400;
    int bin_w = cvRound((double)hist_w / histSize);
    Mat histImage(hist_h, hist_w, CV_8UC3, Scalar(0, 0, 0));
    for (int i = 0; i < img_channels.size(); i++)
    {
        normalize(channel_hist[i], channel_hist[i], 0, histImage.rows, NORM_MINMAX, -1, Mat());
    }

    for (int i = 1; i < histSize; i++)
    {
        for (int j = 0; j < img_channels.size(); j++)
        {
            Scalar color;
            if (j == 0)
                color = Scalar(255, 0, 0);
            else if (j == 1)
                color = Scalar(0, 255, 0);
            else if (j == 2)
                color = Scalar(0, 0, 255);
            line(histImage, Point(bin_w * (i - 1), hist_h - cvRound(channel_hist[j].at<float>(i - 1))),
                 Point(bin_w * (i), hist_h - cvRound(channel_hist[j].at<float>(i))),
                 color, 2, 8, 0);
        }
    }

    resize(image, image, Size(1080 / 2, 720 / 2));
    imshow("", image);
    imshow("Histogram", histImage);
    waitKey();
}

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        cout << "USAGE: ./program image_name" << endl;
        return -1;
    }

    Mat image;
    image = imread(argv[1], IMREAD_COLOR);

    if (image.empty())
        return -1;

    calculateHistogram(image);

    Mat image2;
    image2 = imread(argv[1], IMREAD_GRAYSCALE);

    if (image2.empty())
        return -1;

    calculateHistogram(image2);

    return 0;
}
