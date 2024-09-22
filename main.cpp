#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>

using namespace cv;
using namespace std;

#include <opencv2/opencv.hpp>

int checkColorsInRectangle(Mat& image, const Rect& rect, int rect_count) {
    // Define the color ranges to check for gray
    Scalar lower_color(0, 0, 120);   // Lower bound for gray (low saturation, medium brightness)
    Scalar upper_color(180, 30, 170); // Upper bound for gray

    // Create a mask for the area within the rectangle
    Mat roi = image(rect);

    // Convert the ROI to HSV for better color detection
    Mat hsv_roi;
    cvtColor(roi, hsv_roi, COLOR_BGR2HSV);

    // Threshold the HSV image to get only the desired colors
    Mat mask;
    inRange(hsv_roi, lower_color, upper_color, mask);

    int blue_pixel_count = 0;

    // Check if there are any pixels of the desired color
    if (countNonZero(mask) > 0) {
        
        // Change the detected gray pixels to blue in the original image
        for (int y = 0; y < roi.rows; ++y) {
            for (int x = 0; x < roi.cols; ++x) {
                if (mask.at<uchar>(y, x) > 0) {
                    blue_pixel_count++;
                    //image.at<Vec3b>(rect.y + y, rect.x + x) = Vec3b(255, 0, 0); // Set to blue (BGR format)
                }
            }
        }

        cout << "Color found in rectangle at: " << rect_count << endl;
        cout << "Pixels detected: " << blue_pixel_count << endl;
        return blue_pixel_count;

    } else {
        cout << "No desired color found in rectangle at: " << rect << endl;
        return 0;
    }
}


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

    int rect_count = 0;
    Range grayRange(100, 200); // Example range for gray pixels

    // Draw rectangles around each contour on the original image
    for (const auto& contour : contours) {
        if (contour.size() >= 3) {  // Ensure there are enough points to create a bounding rect
            Rect bounding_rect = boundingRect(contour);
            rect_count++;

            // Check for colors within the rectangle
            if (checkColorsInRectangle(image, bounding_rect, rect_count) > 370)
            {
                rectangle(image, bounding_rect, Scalar(0, 0, 255), 1); // Red color
            }

            // Draw the 4th rectangle in red, others in green
            if ((rect_count > 63 && rect_count <= 94 && rect_count != 92) || (rect_count >= 109 && rect_count <= 138)) {
                // Check for colors within the rectangle
                if (checkColorsInRectangle(image, bounding_rect, rect_count) > 310)
                {
                    rectangle(image, bounding_rect, Scalar(0, 0, 255), 1); // Red color
                }
            }
            else {
                //rectangle(image, bounding_rect, Scalar(0, 255, 0), 2); // Green color
            }

            // Add the rectangle count as text on the image
            string count_text = to_string(rect_count);
            putText(image, count_text, Point(bounding_rect.x + 3, bounding_rect.y - 6), FONT_HERSHEY_SIMPLEX, 0.3, Scalar(255, 255, 255), 1);
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
