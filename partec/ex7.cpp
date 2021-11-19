#include <iostream>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
using namespace std;
using namespace cv;

float entropy(Mat seq, Size size, int index)
{
    int cnt = 0;
    float entr = 0;
    float total_size = size.height * size.width;

    for (int i = 0; i < index; i++)
    {
        float sym_occur = seq.at<float>(0, i);

        if (sym_occur > 0)
        {
            cnt++;
            entr += (sym_occur / total_size) * (log2(sym_occur / total_size));
        }
    }
    return -entr;
}

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
            {
                if (img_channels.size() == 3)
                    color = Scalar(255, 0, 0);
                else if (img_channels.size() == 1)
                    color = Scalar(255, 255, 255);
            }
            else if (j == 1)
            {
                color = Scalar(0, 255, 0);
            }
            else if (j == 2)
            {
                color = Scalar(0, 0, 255);
            }
            line(histImage, Point(bin_w * (i - 1), hist_h - cvRound(channel_hist[j].at<float>(i - 1))),
                 Point(bin_w * (i), hist_h - cvRound(channel_hist[j].at<float>(i))),
                 color, 2, 8, 0);
        }
    }

    resize(image, image, Size(1080 / 2, 720 / 2));
    imshow("Image", image);
    imshow("Histogram", histImage);
    string channel_name[img_channels.size()];
    if (img_channels.size() == 3)
    {
        channel_name[0] = "Blue";
        channel_name[1] = "Green";
        channel_name[2] = "Red";
    }
    else if (img_channels.size() == 1)
    {
        channel_name[0] = "Gray";
    }
    for (int i = 0; i < img_channels.size(); i++)
    {
        cout << channel_name[i] << " channel entropy is " << entropy(channel_hist[i], image.size(), histSize) << endl;
    }
    cout << "-------------------" << endl;
    waitKey();
}

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        cout << "USAGE: ./program <image_name>" << endl;
        return -1;
    }

    Mat image;

    image = imread(argv[1], IMREAD_COLOR);

    if (image.empty())
    {
        cout << "Could not load or find image. Please try again!" << endl;
        return -1;
    }

    calculateHistogram(image);

    Mat image2;
    image2 = imread(argv[1], IMREAD_GRAYSCALE);

    if (image2.empty())
    {
        cout << "Could not load or find image. Please try again!" << endl;
        return -1;
    }

    calculateHistogram(image2);

    return 0;
}
