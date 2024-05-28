#include <opencv2/opencv.hpp>
#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <iomanip>
#include <sstream>

// Função para criar cv::Scalar a partir de valores RGB
cv::Scalar rgb(int r, int g, int b)
{
    return cv::Scalar(b, g, r); // OpenCV usa BGR por padrão
}

// Definição das cores RGB para cada cor de faixa
std::map<std::string, cv::Scalar> rgbColorMap = {
    {"Black", rgb(54, 62, 55)},
    {"Brown", rgb(139, 69, 19)},
    {"Red", rgb(203, 63, 76)}, // nao mexer
    {"Orange", rgb(176, 110, 62)},
    {"Yellow", rgb(255, 255, 0)},
    {"Green", rgb(89, 105, 61)},
    {"Blue", rgb(54, 139, 225)}, // nao mexer
    {"Violet", rgb(238, 130, 238)},
    {"Gray", rgb(128, 128, 128)},
    {"White", rgb(255, 255, 255)},
    {"Gold", rgb(255, 215, 0)},
    {"Silver", rgb(192, 192, 192)} // nao mexer
};

// Função para converter RGB para HSV usando OpenCV
cv::Scalar rgbToHsv(int r, int g, int b)
{
    cv::Mat rgb(1, 1, CV_8UC3, cv::Scalar(b, g, r)); // OpenCV usa BGR por padrão
    cv::Mat hsv;
    cv::cvtColor(rgb, hsv, cv::COLOR_BGR2HSV);

    cv::Vec3b hsvVec = hsv.at<cv::Vec3b>(0, 0);
    return cv::Scalar(hsvVec[0], hsvVec[1], hsvVec[2]);
}

// Definição das cores HSV para cada cor de faixa, convertidas de RGB
std::map<std::string, cv::Scalar> hsvColorMap;

void initializeHsvColorMap()
{
    for (const auto &color : rgbColorMap)
    {
        int r = color.second[2];
        int g = color.second[1];
        int b = color.second[0];
        hsvColorMap[color.first] = rgbToHsv(r, g, b);
    }
}

struct Resistor
{
    cv::Rect boundingBox;
    cv::Point centroid;
    std::vector<std::string> colorBands;
    std::string value;
};

std::string decodeResistorValue(const std::vector<std::string> &colors)
{
    if (colors.size() < 3)
        return "Invalid";

    std::map<std::string, int> digitMap = {
        {"Black", 0}, {"Brown", 1}, {"Red", 2}, {"Orange", 3}, {"Yellow", 4}, {"Green", 5}, {"Blue", 6}, {"Violet", 7}, {"Gray", 8}, {"White", 9}};

    std::map<std::string, double> multiplierMap = {
        {"Black", 1}, {"Brown", 10}, {"Red", 100}, {"Orange", 1000}, {"Yellow", 10000}, {"Green", 100000}, {"Blue", 1000000}, {"Gold", 0.1}, {"Silver", 0.01}};

    if (digitMap.find(colors[0]) == digitMap.end() ||
        digitMap.find(colors[1]) == digitMap.end() ||
        multiplierMap.find(colors[2]) == multiplierMap.end())
    {
        return "Invalid";
    }

    int value = digitMap[colors[0]] * 10 + digitMap[colors[1]];
    value *= multiplierMap[colors[2]];
    return std::to_string(value) + " Ohms";
}

void processFrame(cv::Mat &frame, std::vector<Resistor> &resistors)
{
    cv::Mat hsv;
    cv::cvtColor(frame, hsv, cv::COLOR_BGR2HSV); // Função 1 adicional

    for (auto it = hsvColorMap.begin(); it != hsvColorMap.end(); ++it)
    {
        const std::string &colorName = it->first;
        const cv::Scalar &hsvColor = it->second;

        cv::Mat mask;
        cv::inRange(hsv, hsvColor - cv::Scalar(10, 50, 50), hsvColor + cv::Scalar(10, 255, 255), mask);

        std::vector<std::vector<cv::Point>> contours;
        cv::findContours(mask, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE); // Função 2 adicional

        for (const auto &contour : contours)
        {
            cv::Rect boundingBox = cv::boundingRect(contour);
            if (boundingBox.area() > 1000)
            { // Ajuste o limite conforme necessário
                cv::Point centroid = (boundingBox.br() + boundingBox.tl()) * 0.5;
                bool matched = false;

                for (auto &resistor : resistors)
                {
                    if ((boundingBox & resistor.boundingBox).area() > 0)
                    {
                        resistor.colorBands.push_back(colorName);
                        resistor.boundingBox |= boundingBox; // Atualiza a bounding box para incluir o novo contorno
                        matched = true;
                        break;
                    }
                }

                if (!matched)
                {
                    std::vector<std::string> colorBands = {colorName};
                    resistors.push_back({boundingBox, centroid, colorBands, ""});
                }
            }
        }
    }

    for (auto &resistor : resistors)
    {
        if (resistor.colorBands.size() >= 3)
        {
            resistor.value = decodeResistorValue(resistor.colorBands);
        }
        else
        {
            resistor.value = "Invalid";
        }
    }
}

void drawResistors(cv::Mat &frame, const std::vector<Resistor> &resistors)
{
    for (const auto &resistor : resistors)
    {
        cv::rectangle(frame, resistor.boundingBox, cv::Scalar(0, 255, 0), 2);
        cv::circle(frame, resistor.centroid, 5, cv::Scalar(255, 0, 0), -1);
        cv::putText(frame, resistor.value, resistor.boundingBox.tl() - cv::Point(0, 10), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 255, 0), 2);
    }
}

int main()
{
    initializeHsvColorMap();

    cv::VideoCapture cap("video_resistors.mp4");
    if (!cap.isOpened())
    {
        std::cerr << "Erro ao abrir o vídeo!" << std::endl;
        return -1;
    }

    cv::Mat frame;
    while (cap.read(frame))
    {
        if (frame.empty())
        {
            std::cerr << "Erro ao capturar o frame!" << std::endl;
            break;
        }

        std::vector<Resistor> resistors;
        processFrame(frame, resistors);
        drawResistors(frame, resistors);
        cv::imshow("Processed Frame", frame);
        if (cv::waitKey(30) >= 0)
            break;
    }

    return 0;
}
