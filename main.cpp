#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>

using namespace cv;
using namespace std;

void detectAndDrawRectangles(Mat& image, const Mat& mask) {
    // Convert mask to grayscale
    Mat gray_mask;
    if (mask.channels() > 1) {
        cvtColor(mask, gray_mask, COLOR_BGR2GRAY);
    }
    else {
        gray_mask = mask;
    }

    // Apply binary threshold
    Mat binary;
    threshold(gray_mask, binary, 240, 255, THRESH_BINARY);

    // Find contours
    vector<vector<Point>> contours;
    vector<Vec4i> hierarchy;
    findContours(binary, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

    if (contours.empty()) {
        cout << "No contours found" << endl;
        return;
    }

    // Draw rectangles around each contour on the original image
    for (const auto& contour : contours) {
        if (contour.size() >= 3) {  // Ensure there are enough points to create a bounding rect
            Rect bounding_rect = boundingRect(contour);
            rectangle(image, bounding_rect, Scalar(0, 255, 0), 2); // Green color, thickness 2
        }
    }

    imshow("Detected Rectangles", image);
    waitKey(0);
}

int main() {
    string image_path = "images/parking01.jpg"; // Change to the original image path
    string mask_path = "images/mask01.jpg"; // Path to the mask image

    Mat image = imread(image_path, IMREAD_COLOR);
    Mat mask = imread(mask_path, IMREAD_COLOR);

    if (image.empty() || mask.empty()) {
        cout << "Could not open or find the image or mask" << endl;
        return -1;
    }

    // Ensure the mask is the same size as the image
    if (image.size() != mask.size()) {
        cout << "Image and mask sizes do not match" << endl;
        return -1;
    }

    // Detect and draw rectangles
    detectAndDrawRectangles(image, mask);

    return 0;
}
