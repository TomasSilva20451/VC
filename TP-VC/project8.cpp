#include <opencv2/opencv.hpp>
#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <iomanip>
#include <sstream>

// Função para converter os valores RGB em cv::Scalar
cv::Scalar rgb(int r, int g, int b)
{
    return cv::Scalar(b, g, r); // Note que a ordem é (b, g, r) para cv::Scalar
}

std::string decodeResistorValue(const std::vector<std::string> &colors)
{
    if (colors.size() < 4)
        return "Invalid";

    std::map<std::string, int> digitMap = {
        {"Black", 0}, {"Brown", 1}, {"Red", 2}, {"Orange", 3}, {"Yellow", 4}, {"Green", 5}, {"Blue", 6}, {"Violet", 7}, {"Gray", 8}, {"White", 9}};

    std::map<std::string, double> multiplierMap = {
        {"Black", 1}, {"Brown", 10}, {"Red", 100}, {"Orange", 1000}, {"Yellow", 10000}, {"Green", 100000}, {"Blue", 1000000}, {"Gold", 0.1}, {"Silver", 0.01}};

    // Verificando se as cores fornecidas são válidas
    for (const auto &color : colors)
    {
        if (digitMap.find(color) == digitMap.end() && multiplierMap.find(color) == multiplierMap.end())
        {
            return "Invalid";
        }
    }

    // Calculando o valor do resistor
    int firstDigit = digitMap[colors[0]];
    int secondDigit = digitMap[colors[1]];
    double multiplier = multiplierMap[colors[2]];
    double resistanceValue = (firstDigit * 10 + secondDigit) * multiplier;

    // Formatação da saída
    std::ostringstream oss;
    if (resistanceValue >= 1000)
    {
        oss << std::fixed << std::setprecision(1) << resistanceValue / 1000 << "k ohms";
    }
    else
    {
        oss << std::fixed << std::setprecision(0) << resistanceValue << " ohms";
    }

    // Se a quarta cor (tolerância) estiver presente
    if (colors.size() == 4)
    {
        std::map<std::string, std::string> toleranceMap = {
            {"Brown", "1%"}, {"Red", "2%"}, {"Green", "0.5%"}, {"Blue", "0.25%"}, {"Violet", "0.1%"}, {"Gray", "0.05%"}, {"Gold", "5%"}, {"Silver", "10%"}};

        if (toleranceMap.find(colors[3]) != toleranceMap.end())
        {
            oss << " ±" << toleranceMap[colors[3]];
        }
        else
        {
            return "Invalid";
        }
    }

    return oss.str();
}

// Definição das cores RGB para cada cor de faixa
std::map<std::string, cv::Scalar> rgbColorMap = {
    {"Black", rgb(54, 62, 55)},
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

void processFrame(cv::Mat &frame)
{
    // Conversão para HSV
    cv::Mat hsv;
    cv::cvtColor(frame, hsv, cv::COLOR_BGR2HSV);

    // Segmentação por cor (exemplo para a cor vermelha)
    cv::Mat mask;
    cv::inRange(hsv, cv::Scalar(0, 70, 50), cv::Scalar(10, 255, 255), mask);

    // Melhoramento da imagem (remoção de ruído)
    cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(5, 5));
    cv::morphologyEx(mask, mask, cv::MORPH_CLOSE, kernel);

    // Encontrar contornos
    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(mask, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

    for (const auto &contour : contours)
    {
        // Caixa delimitadora
        cv::Rect boundingBox = cv::boundingRect(contour);
        cv::rectangle(frame, boundingBox, cv::Scalar(0, 255, 0), 2);

        // Centroide
        cv::Moments moments = cv::moments(contour);
        cv::Point2f center(moments.m10 / moments.m00, moments.m01 / moments.m00);
        cv::circle(frame, center, 5, cv::Scalar(255, 0, 0), -1);

        // Exemplo de valor fictício para o resistor
        std::vector<std::string> resistorColors = {"Red", "Violet", "Yellow", "Gold"};
        std::string result = decodeResistorValue(resistorColors);

        // Desenhar o valor do resistor
        cv::putText(frame, result, boundingBox.tl(), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(255, 255, 255), 1);
    }
}

int main()
{
    cv::VideoCapture cap("video_resistors.mp4");
    if (!cap.isOpened())
    {
        std::cerr << "Erro ao abrir o vídeo." << std::endl;
        return -1;
    }

    cv::namedWindow("Frame Processado", cv::WINDOW_AUTOSIZE);
    cv::Mat frame;
    while (true)
    {
        cap >> frame;
        if (frame.empty())
            break;

        processFrame(frame);
        cv::imshow("Frame Processado", frame);

        if (cv::waitKey(30) >= 0)
            break;
    }

    cap.release();
    cv::destroyAllWindows();
    return 0;
}
