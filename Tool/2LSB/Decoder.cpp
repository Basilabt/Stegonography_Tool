#include <iostream>
#include <fstream>
#include <string>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

// DEC.

bool isBitSetToOne(char, const int);
void shiftRightBy(char&, const int);
bool isProgramExecutedSuccesfully(const int);
void printError(const string);
void calculatePSNR(const Mat&,const Mat&);

int main(int argc, char** argv) {
    if (!isProgramExecutedSuccesfully(argc)) {
        printError("Missing Parameters! Correct Execution: ./decoder [Image]");
        return -1;
    }

    Mat originalImage = imread(argv[1]);
    if (originalImage.empty()) {
        printError("Failed to Load Image!");
        return -1;
    }

    Mat imageCopy = originalImage.clone();

    char c = 0;
    int currentBits = 0;

    for (int row = 0; row < imageCopy.rows; row++) {
        for (int col = 0; col < imageCopy.cols; col++) {
            for (int scheme = 0; scheme < 3; scheme++) {
                Vec3b currentPixel = imageCopy.at<Vec3b>(Point(row, col));

                c |= (currentPixel[scheme] & 3) << (6 - currentBits);

                currentBits += 2;

                if (currentBits == 8) {
                    if (c == '\0') {
                        goto BREAKNESTEDLOOPS;
                    }

                    currentBits = 0;
                    cout << c;
                    c = 0;
                }
            }
        }
    }
BREAKNESTEDLOOPS:;

	calculatePSNR(originalImage, imageCopy);

    return 0;
}

// DEF.

bool isBitSetToOne(char c, const int position) {
    shiftRightBy(c, position);
    return (c & 1) != 0;
}

void shiftRightBy(char& c, const int numberOfShifts) {
    c = c >> numberOfShifts;
}

bool isProgramExecutedSuccesfully(const int argc) {
    return argc == 2;
}

void printError(const string errorMessage) {
    cout << "\n[-] ERROR: " << errorMessage << "\n";
}



void calculatePSNR(const Mat& original, const Mat& encoded) {
    if (original.size() != encoded.size()) {
        std::cerr << "Error: Images must have the same dimensions!" << std::endl;
        return;
    }

    // Calculate MSE (Mean Squared Error)
    double mse = 0.0;
    for (int i = 0; i < original.rows; i++) {
        for (int j = 0; j < original.cols; j++) {
            Vec3b pixelOriginal = original.at<Vec3b>(i, j);
            Vec3b pixelEncoded = encoded.at<Vec3b>(i, j);

            // Calculate squared differences for each channel (RGB)
            for (int c = 0; c < 3; c++) {
                double diff = pixelOriginal[c] - pixelEncoded[c];
                mse += diff * diff;
            }
        }
    }

    mse /= (original.rows * original.cols * 3); // Average over all pixels and channels

    if (mse == 0) {
        cout << "PSNR = " << 99.0 << "\n";  // Perfect match, return very high PSNR
        return;
    }

    // Calculate PSNR
    double psnr = 10.0 * log10(255 * 255 / mse);
    cout << "PSNR = " << psnr << "\n";
}







