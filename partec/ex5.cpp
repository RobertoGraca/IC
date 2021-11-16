#include <iostream>
#include <fstream>
#include <map>
#include <opencv2/plot.hpp>
#include <opencv2/opencv.hpp>
using namespace std;
using namespace cv;

template <typename T>
cv::Mat plotGraph(std::vector<T>& vals, int YRange[2])
{

    auto it = minmax_element(vals.begin(), vals.end());
    float scale = 1./ceil(*it.second - *it.first); 
    float bias = *it.first;
    int rows = YRange[1] - YRange[0] + 1;
    cv::Mat image = Mat::zeros( rows, vals.size(), CV_8UC3 );
    image.setTo(0);
    for (int i = 0; i < (int)vals.size()-1; i++)
    {
        cv::line(image, cv::Point(i, rows - 1 - (vals[i] - bias)*scale*YRange[1]), cv::Point(i+1, rows - 1 - (vals[i+1] - bias)*scale*YRange[1]), Scalar(255, 0, 0), 1);
    }
    return image;
}
int main(){
    ifstream ifs("lusiadas.txt");

    map<char, int> m;

    while(!ifs.eof()){
        string s;
        ifs >> s;
        for(int i=0;i<s.length();i++){
            if(m.count(s[i]) == 0) m[s[i]] = 1;
            else m[s[i]]++;
        }
    }
    
    int arrx[m.size()];
    int arry[m.size()];

    for(int i = 0; i < sizeof(arrx); i++)
    {
        arrx[i] = m[i];
    }
    vector<int> x(arrx,arrx + sizeof(arrx) / sizeof(int));
    vector<int> y(arry,arry + sizeof(arry) / sizeof(int));
    int range[2] = {0,(int)sizeof(arry)};
    cv::Mat lineGraph = plotGraph(x,range);
    imshow("Lusiadas Histogram",lineGraph);
    waitKey();

}