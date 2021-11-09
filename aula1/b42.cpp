#include <chrono>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int main(int argc, char **argv)
{
    VideoCapture cap;

    if (argc == 2)
    {
        cap = VideoCapture(argv[1]);
    }
    else
    {
        cap = VideoCapture(0);
    }

    if (!cap.isOpened())
        return -1;

    Mat frame;
    int framePeriod{int(1000. / cap.get(CAP_PROP_FPS))};
    bool terminate = false;
    namedWindow("OpenCV Video");
    while (!terminate)
    {
        if (cap.read(frame))
        {
            imshow("OpenCV Video", frame);
        }
        else
        {
            terminate = true;
        }

        int key = waitKey(framePeriod);
        switch (key)
        {
        case 'q':
            terminate = true;
            break;
        }
    }

    return 0;
}