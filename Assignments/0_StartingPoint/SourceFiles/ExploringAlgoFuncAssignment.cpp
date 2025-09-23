

#include "imgui.h"
#include "imgui-SFML.h"

#include <iostream> // For Debugging.
#include <vector> // For Vector Usage.
#include <numeric> // For Specialized Vector Usage.

#include "../HeaderFiles/ExploringAlgoFuncAssignment.h"

void ExploringAlgoFuncAssignment::Start()
{
    // Console Notification.
    std::cout << "Assignment Start: Begin!" << std::endl;

    // Use functions from <algorithm> and <functional> to complete the following assignments.

    // Split the vector into 2 new vectors: one with everything alphabetically before 'purple', one with everything after.
    {
        // Console SubTitle.
        std::cout << "-" << std::endl;
        std::cout << "Part A" << std::endl;

        // Original Vector.
        std::vector<std::string> colorsVectorOne
        { "red", "green", "white", "blue", "orange", "green", "orange", "black", "purple" };

        // Sort Alphabetically Original Vector.
        std::sort(colorsVectorOne.begin(), colorsVectorOne.end());

        // Split Original Vector into two from purple element.
        auto splitBefore = std::lower_bound(colorsVectorOne.begin(), colorsVectorOne.end(), "purple");
        std::vector<std::string> colorsVectorPrequelPurple(colorsVectorOne.begin(), splitBefore);

        auto splitAfter = std::upper_bound(colorsVectorOne.begin(), colorsVectorOne.end(), "purple");
        std::vector<std::string> colorsVectorSequelPurple(splitAfter, colorsVectorOne.end());

        // Console all Vectors.
        std::cout << "" << std::endl;
        std::cout << "colorsVectorOne [Contains]" << std::endl;
        for (int i = 0; i < colorsVectorOne.size(); i++) 
        {
            std::cout << i << ":" << colorsVectorOne[i] << std::endl;
        }

        std::cout << "" << std::endl;
        std::cout << "colorsVectorPrequelPurple [Contains]" << std::endl;
        for (int i = 0; i < colorsVectorPrequelPurple.size(); i++)
        {
            std::cout << i << ":" << colorsVectorPrequelPurple[i] << std::endl;
        }

        std::cout << "" << std::endl;
        std::cout << "colorsVectorSequelPurple [Contains]" << std::endl;
        for (int i = 0; i < colorsVectorSequelPurple.size(); i++)
        {
            std::cout << i << ":" << colorsVectorSequelPurple[i] << std::endl;
        }

        std::cout << "" << std::endl;
    }

    // Make all elements UPPERCASE.
    {
        // Console SubTitle.
        std::cout << "-" << std::endl;
        std::cout << "Part B" << std::endl;

        // Original Vector.
        std::vector<std::string> colorsVectorTwo
        { "red", "green", "white", "blue", "orange", "green", "orange", "black", "purple" };

        // Go through all colors and uppercase each string.
        std::transform(colorsVectorTwo.begin(), colorsVectorTwo.end(), colorsVectorTwo.begin(),
            [](const std::string& string) 
            { 
                // Copy string, uppercase each character and return it.
                std::string upperString = string;
                std::transform(upperString.begin(), upperString.end(), upperString.begin(), 
                    [](unsigned char c) 
                    { return std::toupper(c); 
                    });
                return upperString;
            });

        // Console all Vectors.
        std::cout << "" << std::endl;
        std::cout << "colorsVectorTwo [Contains]" << std::endl;
        for (int i = 0; i < colorsVectorTwo.size(); i++)
        {
            std::cout << i << ":" << colorsVectorTwo[i] << std::endl;
        }

        std::cout << "" << std::endl;
    }

    // Remove all duplicate elements.
    {
        // Console SubTitle.
        std::cout << "-" << std::endl;
        std::cout << "Part C" << std::endl;

        // Original Vector.
        std::vector<std::string> colorsVectorThree
        { "red", "green", "white", "blue", "orange", "green", "orange", "black", "purple" };

        // Sort Alphabetically Original Vector.
        std::sort(colorsVectorThree.begin(), colorsVectorThree.end());

        // Make colors unique.
        auto last = std::unique(colorsVectorThree.begin(), colorsVectorThree.end());

        // Now erase all doubles.
        colorsVectorThree.erase(last, colorsVectorThree.end());

        // Console all Vectors.
        std::cout << "" << std::endl;
        std::cout << "colorsVectorThree [Contains]" << std::endl;
        for (int i = 0; i < colorsVectorThree.size(); i++)
        {
            std::cout << i << ":" << colorsVectorThree[i] << std::endl;
        }

        std::cout << "" << std::endl;
    }

    // Remove all negative elements.
    {
        // Console SubTitle.
        std::cout << "-" << std::endl;
        std::cout << "Part D" << std::endl;

        // Original Vector.
        std::vector<double> numbersVectorOne
        { 10, 324422, 6, -23, 234.5, 654.1, 3.1242, -9.23, 635 };

        // Mark negatives for erasing.
        auto last = std::remove_if(numbersVectorOne.begin(), numbersVectorOne.end(),
            [](const double& number) 
            {
                if (number < 0) 
                    return true;
                else 
                    return false;
            });

        // Actually erase them.
        numbersVectorOne.erase(last, numbersVectorOne.end());


        // Console all Vectors.
        std::cout << "" << std::endl;
        std::cout << "numbersVectorOne [Contains]" << std::endl;
        for (int i = 0; i < numbersVectorOne.size(); i++)
        {
            std::cout << i << ":" << numbersVectorOne[i] << std::endl;
        }

        std::cout << "" << std::endl;
    }

    // Determine for all elements whether they are even or odd.
    {
        // Console SubTitle.
        std::cout << "-" << std::endl;
        std::cout << "Part E" << std::endl;

        // Original Vector.
        std::vector numbersVectorTwo
        { 10, 324422, 6, -23, 234, 654, 3, -9, 635 };
        
        // Copy all even to seperate vector.
        std::vector<int> numbersVectorEven;
        std::copy_if(numbersVectorTwo.begin(), numbersVectorTwo.end(),
            std::back_inserter(numbersVectorEven),
            [](int x)
            {
                return x % 2 == 0;
            });

        // Copy all uneven to seperate vector.
        std::vector<int> numbersVectorUneven;
        std::copy_if(numbersVectorTwo.begin(), numbersVectorTwo.end(),
            std::back_inserter(numbersVectorUneven),
            [](int x)
            {
                return x % 2 != 0;
            });

        // Console all Vectors.
        std::cout << "" << std::endl;
        std::cout << "numbersVectorTwo [Contains]" << std::endl;
        for (int i = 0; i < numbersVectorTwo.size(); i++)
        {
            std::cout << i << ":" << numbersVectorTwo[i] << std::endl;
        }
        std::cout << "" << std::endl;

        std::cout << "numbersVectorEven [Contains]" << std::endl;
        for (int i = 0; i < numbersVectorEven.size(); i++)
        {
            std::cout << i << ":" << numbersVectorEven[i] << std::endl;
        }
        std::cout << "" << std::endl;

        std::cout << "numbersVectorUneven [Contains]" << std::endl;
        for (int i = 0; i < numbersVectorUneven.size(); i++)
        {
            std::cout << i << ":" << numbersVectorUneven[i] << std::endl;
        }

        std::cout << "" << std::endl;
    }

    // Calculate the sum, the average, and the product of all numbers.
    {
        // Console SubTitle.
        std::cout << "-" << std::endl;
        std::cout << "Part F" << std::endl;

        // Original Vector.
        std::vector<double> numbersVectorThree
        { 10, 324422.1, 6, -23, 234.5, 654.1, 3.1242, -9.23, 635 };

        // Calculates Sum, Product & Average.
        double sum = std::accumulate(numbersVectorThree.begin(), numbersVectorThree.end(), 0);
        double product = std::accumulate(numbersVectorThree.begin(), numbersVectorThree.end(), 1, std::multiplies<int>());
        double average = sum / numbersVectorThree.size();

        // Console all Vectors.
        std::cout << "" << std::endl;
        std::cout << "numbersVectorThree [Contains]" << std::endl;
        for (int i = 0; i < numbersVectorThree.size(); i++)
        {
            std::cout << i << ":" << numbersVectorThree[i] << std::endl;
        }
        std::cout << "" << std::endl;

        std::cout << "Average: " << average << std::endl;
        std::cout << "Sum: " << sum << std::endl;
        std::cout << "Product: " << product << std::endl;

        std::cout << "" << std::endl;
    }

    // Console Notification.
    std::cout << "Assignment Start: End!" << std::endl;
}

void ExploringAlgoFuncAssignment::Update()
{
    // ...
}

void ExploringAlgoFuncAssignment::Render(sf::RenderWindow& window)
{
    // ...
}
