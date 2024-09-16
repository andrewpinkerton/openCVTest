#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>

using namespace cv;
using namespace std;

void detectParkingSpaces(Mat& image) {
    // Example rectangles (x, y, width, height)
    vector<Rect> parkingSpaces = {
        Rect(50, 50, 100, 200), // Example rectangle 1
        // Add more rectangles as needed
    };

    // Draw rectangles on the image
    for (const auto& space : parkingSpaces) {
        rectangle(image, space, Scalar(0, 255, 0), 2); // Green color, thickness 2
    }

    imshow("Detected Parking Spaces", image);

    // Wait for a key press
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
