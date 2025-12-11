#include <opencv2/opencv.hpp>
#include <iostream>
#include <algorithm>
using namespace cv;
using namespace std;

int *VerticalSeamSelect(double **EnergyMatrix, int imgHeight, int imgWidth)
{

    double **dp = new double *[imgHeight];
    for (int i = 0; i < imgHeight; i++)
    {
        dp[i] = new double[imgWidth];
    }

    for (int i = imgHeight - 1; i >= 0; i--)
    {
        for (int j = 0; j < imgWidth; j++)
        {
            if (i == imgHeight - 1)
            {
                dp[i][j] = EnergyMatrix[i][j];
            }
            else
            {
                if (j == 0)
                    dp[i][j] = EnergyMatrix[i][j] + min(dp[i + 1][j], dp[i + 1][j + 1]);
                else if (j == imgWidth - 1)
                    dp[i][j] = EnergyMatrix[i][j] + min(dp[i + 1][j], dp[i + 1][j - 1]);
                else
                    dp[i][j] = EnergyMatrix[i][j] + min({dp[i + 1][j - 1], dp[i + 1][j], dp[i + 1][j + 1]});
            }
        }
    }

    int *index = new int[imgHeight];
    int minIndex = 0;
    for (int j = 1; j < imgWidth; j++)
    {
        if (dp[0][j] < dp[0][minIndex])
        {
            minIndex = j;
        }
    }
    index[0] = minIndex;

    for (int i = 1; i < imgHeight; i++)
    {
        int prevIndex = index[i - 1];
        int minJ = prevIndex;

        if (prevIndex > 0 && dp[i][prevIndex - 1] < dp[i][minJ])
        {
            minJ = prevIndex - 1;
        }
        if (prevIndex < imgWidth - 1 && dp[i][prevIndex + 1] < dp[i][minJ])
        {
            minJ = prevIndex + 1;
        }
        index[i] = minJ;
    }

    for (int i = 0; i < imgHeight; i++)
    {
        delete[] dp[i];
    }
    delete[] dp;

    return index;
}

int *HorizontalSeamSelect(double **EnergyMatrix, int imgHeight, int imgWidth)
{

    double **dp = new double *[imgHeight];
    for (int i = 0; i < imgHeight; i++)
    {
        dp[i] = new double[imgWidth];
    }

    for (int i = 0; i < imgHeight; i++)
    {
        dp[i][0] = EnergyMatrix[i][0];
    }

    for (int j = 1; j < imgWidth; j++)
    {
        for (int i = 0; i < imgHeight; i++)
        {

            dp[i][j] = EnergyMatrix[i][j] + dp[i][j - 1];

            if (i > 0)
            {
                dp[i][j] = std::min(dp[i][j], EnergyMatrix[i][j] + dp[i - 1][j - 1]);
            }

            if (i < imgHeight - 1)
            {
                dp[i][j] = std::min(dp[i][j], EnergyMatrix[i][j] + dp[i + 1][j - 1]);
            }
        }
    }

    int *seam = new int[imgWidth];
    int minIndex = 0;

    for (int i = 1; i < imgHeight; i++)
    {
        if (dp[i][imgWidth - 1] < dp[minIndex][imgWidth - 1])
        {
            minIndex = i;
        }
    }

    seam[imgWidth - 1] = minIndex;

    for (int j = imgWidth - 2; j >= 0; j--)
    {
        int prevIndex = seam[j + 1];

        int bestIndex = prevIndex;
        if (prevIndex > 0 && dp[prevIndex - 1][j] < dp[bestIndex][j])
        {
            bestIndex = prevIndex - 1; // Up-left
        }
        if (prevIndex < imgHeight - 1 && dp[prevIndex + 1][j] < dp[bestIndex][j])
        {
            bestIndex = prevIndex + 1; // Down-left
        }

        seam[j] = bestIndex; // Store the best row index for this column
    }


    for (int i = 0; i < imgHeight; i++)
    {
        delete[] dp[i];
    }
    delete[] dp;

    return seam;
}

// int *HorizontalSeamSelect(double **EnergyMatrix, int imgHeight, int imgWidth)
// {
//     double **dp = new double *[imgHeight];
//     for (int i = 0; i < imgHeight; ++i)
//     {
//         dp[i] = new double[imgWidth];
//     }

//    /* for (int j = 0; j < imgWidth; ++j)
//     {
//         dp[0][j] = EnergyMatrix[0][j];
//     }*/

//     for (int i = imgHeight-1; i >=0; i--)
//     {
//         for (int j = 0; j < imgWidth; ++j)
//         {
//             if (j == 0)
//             {
//                 dp[i][j] = EnergyMatrix[i][j]; //+ min(dp[i - 1][j], dp[i - 1][j + 1]);
//             }

//             else
//             {
//                 if(i == 0)
//                 {
//                     dp[i][j] = EnergyMatrix[i][j] + min(dp[i][j+1], dp[i+1][j+1]);
//                 }
//                 else if(i == imgHeight-1)
//                 {
//                     dp[i][j] = EnergyMatrix[i][j] + min(dp[i][j+1], dp[i-1][j+1]);
//                 }
//                 else
//                 {
//                     dp[i][j] = EnergyMatrix[i][j] + min({dp[i][j+1], dp[i+1][j+1], dp[i-1][j+1]});
//                 }
//             }
//             /*else if (j == imgWidth - 1)
//             {
//                 dp[i][j] = EnergyMatrix[i][j] + min(dp[i - 1][j], dp[i - 1][j - 1]);
//             }
//             else
//             {
//                 dp[i][j] = EnergyMatrix[i][j] + min({dp[i - 1][j - 1], dp[i - 1][j], dp[i - 1][j + 1]});
//             }*/
//         }
//     }

//     // Find the minimum energy seam in the last row
//     int *index = new int[imgWidth];
//     int minIndex = 0;
//     for (int i = 1; i < imgHeight; ++i)
//     {
//         if (dp[i][0] < dp[minIndex][0])
//         {
//             minIndex = i;
//         }
//     }
//     index[0] = minIndex;

//     for (int j = 1; j < imgWidth; ++j)
//     {
//         int prevIndex = index[j - 1];
//         int minI = prevIndex;

//         if (prevIndex > 0 && dp[prevIndex - 1][j] < dp[minI][j])
//         {
//             minI = prevIndex - 1;
//         }
//         if (prevIndex < imgHeight - 1 && dp[prevIndex + 1][j] < dp[minI][j])
//         {
//             minI = prevIndex + 1;
//         }
//         index[j] = minI;
//     }

//     // Clean up
//     for (int i = 0; i < imgHeight; ++i)
//     {
//         delete[] dp[i];
//     }
//     delete[] dp;

//     return index;
// }

double CalculateEnergy(Vec3b top, Vec3b down, Vec3b left, Vec3b right)
{
    int deltaSquareY = (top[0] - down[0]) * (top[0] - down[0]) +
                       (top[1] - down[1]) * (top[1] - down[1]) +
                       (top[2] - down[2]) * (top[2] - down[2]);

    int deltaSquareX = (right[0] - left[0]) * (right[0] - left[0]) +
                       (right[1] - left[1]) * (right[1] - left[1]) +
                       (right[2] - left[2]) * (right[2] - left[2]);

    return sqrt(deltaSquareX + deltaSquareY);
}

int main()
{
    string str;
    cout<<"Enter the full name of image. For example: sample1.jpeg: ";
    cin>>str;
    Mat img = imread(str);
    if (img.empty())
    {
        cout << "Could not open or find the image\n";
        return -1;
    }

    int imgWidth = img.cols;
    int imgHeight = img.rows;

    cout << "Image Width: " << imgWidth << " Image Height: " << imgHeight << endl;

    double **EnergyMatrix = new double *[imgHeight];
    for (int i = 0; i < imgHeight; i++)
    {
        EnergyMatrix[i] = new double[imgWidth];
    }

    for (int i = 0; i < imgHeight; i++)
    {
        for (int j = 0; j < imgWidth; j++)
        {
            Vec3b top, down, left, right;

            if (i > 0)
            {
                top = img.at<Vec3b>(i - 1, j);
            }
            else
            {
                top = img.at<Vec3b>(imgHeight - 1, j);
            }

            if (i < imgHeight - 1)
            {
                down = img.at<Vec3b>(i + 1, j);
            }
            else
            {
                down = img.at<Vec3b>(0, j);
            }

            if (j > 0)
            {
                left = img.at<Vec3b>(i, j - 1);
            }
            else
            {
                left = img.at<Vec3b>(i, imgWidth - 1);
            }

            if (j < imgWidth - 1)
            {
                right = img.at<Vec3b>(i, j + 1);
            }
            else
            {
                right = img.at<Vec3b>(i, 0);
            }

            EnergyMatrix[i][j] = CalculateEnergy(top, down, left, right);
        }
    }

    int reducedWidth;
    cout << "Enter the reduced width: ";
    cin >> reducedWidth;
    int reducedHeight;
    cout << "Enter the reduced height: ";
    cin >> reducedHeight;

    while (imgWidth > reducedWidth)
    {
        int *seam = VerticalSeamSelect(EnergyMatrix, imgHeight, imgWidth);
        for (int i = 0; i < imgHeight; i++)
        {
            img.at<Vec3b>(i, seam[i]) = Vec3b(0, 0, 255);
        }
        imshow("test file", img);
        waitKey(30);
        Mat reducedImg(imgHeight, imgWidth - 1, img.type());
        for (int i = 0; i < imgHeight; i++)
        {
            for (int j = 0, k = 0; j < imgWidth; j++)
            {
                if (j != seam[i])
                {
                    reducedImg.at<Vec3b>(i, k++) = img.at<Vec3b>(i, j);
                }
            }
        }

        for (int i = 0; i < imgHeight; i++)
        {
            for (int j = seam[i] + 1; j < imgWidth; j++)
            {
                EnergyMatrix[i][j - 1] = EnergyMatrix[i][j];
            }
        }

        img = reducedImg;
        imgWidth--;

        delete[] seam;
    }

    // Display or save the result
    // imwrite("reduced_image.png", img);

    // Free memory
    /*for (int i = 0; i < imgHeight; i++)
    {
        delete[] EnergyMatrix[i];
    }
    delete[] EnergyMatrix;*/

    // Horizontal seam removal
    while (imgHeight > reducedHeight)
    {
        int *seam = HorizontalSeamSelect(EnergyMatrix, imgHeight, imgWidth);

        for (int i = 0; i < imgWidth; i++)
        {
            img.at<Vec3b>(seam[i], i) = Vec3b(0, 0, 255);
        }
        imshow("test file", img);
        waitKey(30);

        Mat reducedImg(imgHeight - 1, imgWidth, img.type());
        for (int j = 0; j < imgWidth; j++)
        {
            for (int i = 0, k = 0; i < imgHeight; i++)
            {
                if (i != seam[j])
                {
                    reducedImg.at<Vec3b>(k++, j) = img.at<Vec3b>(i, j);
                }
            }
        }
        img = reducedImg;

        for (int j = 0; j < imgWidth; j++)
        {
            for (int i = seam[j] + 1; i < imgHeight; i++)
            {
                EnergyMatrix[i - 1][j] = EnergyMatrix[i][j];
            }
        }

        imgHeight--;
        delete[] seam;
    }


    imwrite("reduced_image.jpeg", img);

    for (int i = 0; i < imgHeight; i++)
    {
        delete[] EnergyMatrix[i];
    }
    delete[] EnergyMatrix;

    imshow("SeamCarving",img);
    waitKey(0);
    return 0;
}
