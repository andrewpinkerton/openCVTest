#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>

using namespace cv;
using namespace std;

// Function to detect open parking spaces
void detectParkingSpaces(const Mat& image) {
    Mat gray, edges, linesImage;

    // Convert image to grayscale
    cvtColor(image, gray, COLOR_BGR2GRAY);

    // Apply Gaussian blur to reduce noise
    GaussianBlur(gray, gray, Size(5, 5), 0);

    // Perform Canny edge detection to highlight lines
    Canny(gray, edges, 50, 150, 3);

    // Detect lines using Hough Line Transform
    vector<Vec4i> lines;
    HoughLinesP(edges, lines, 1, CV_PI / 180, 50, 50, 10); // Adjust thresholds as needed

    // Create a blank image to draw the detected lines
    linesImage = Mat::zeros(image.size(), image.type());

    // Draw the detected lines
    for (size_t i = 0; i < lines.size(); i++) {
        Vec4i l = lines[i];
        line(linesImage, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0, 255, 0), 2, LINE_AA);
    }

    // Combine the original image with the detected lines for visualization
    Mat result;
    addWeighted(image, 0.8, linesImage, 0.2, 0, result);

    // Find contours to detect parking spaces between the lines
    vector<vector<Point>> contours;
    findContours(edges, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

    int openSpaces = 0;
    for (size_t i = 0; i < contours.size(); i++) {
        Rect rect = boundingRect(contours[i]);

        // Filter small or large contours that are unlikely to be parking spaces
        if (rect.area() > 1000 && rect.area() < 10000) {
            // Draw a rectangle around detected parking spaces
            rectangle(result, rect, Scalar(0, 0, 255), 2);
            openSpaces++;
        }
    }

    cout << "Open parking spaces detected: " << openSpaces << endl;

    imshow("Detected Parking Spaces", result);
    waitKey(0);
}

int main() {
    string image_path = "C:/Users/viper/Downloads/parkinglot.png";

    Mat image = imread(image_path, IMREAD_COLOR);

    if (image.empty())  // Check if the image was successfully loaded
    {
        cout << "Could not open or find the image" << endl;
        return -1;
    }

    // Detect parking spaces
    detectParkingSpaces(image);

    return 0;
}
