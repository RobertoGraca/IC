#include <iostream>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
using namespace std;
using namespace cv;

Mat calculateHistogram(Mat image)
{

    vector<Mat> bgr_planes;
    split(image, bgr_planes);

    int histSize = 256;

    float range[] = {0, 256}; // the upper boundary is exclusive
    const float *histRange[] = {range};

    bool uniform = true, accumulate = false;
    Mat b_hist, g_hist, r_hist;
    calcHist(&bgr_planes[0], 1, 0, Mat(), b_hist, 1, &histSize, histRange, uniform, accumulate);

    int hist_w = 512, hist_h = 400;
    int bin_w = cvRound((double)hist_w / histSize);
    Mat histImage(hist_h, hist_w, CV_8UC3, Scalar(0, 0, 0));
    normalize(b_hist, b_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat());

    for (int i = 1; i < histSize; i++)
    {
        line(histImage, Point(bin_w * (i - 1), hist_h - cvRound(b_hist.at<float>(i - 1))),
             Point(bin_w * (i), hist_h - cvRound(b_hist.at<float>(i))),
             Scalar(255, 0, 0), 2, 8, 0);
    }
    cout << "boas" << endl;
    resize(image, image, Size(1080 / 2, 720 / 2));
    imshow("Image", image);
    imshow("Histogram", histImage);
}

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        cout << "USAGE: ./program image_name" << endl;
        return -1;
    }

    Mat image;
    image = imread(argv[1], IMREAD_GRAYSCALE);

    if (image.empty())
        return -1;

    cout << image.channels() << endl;

    // imshow("image", image);

    calculateHistogram(image);

    waitKey();

    return 0;
}
