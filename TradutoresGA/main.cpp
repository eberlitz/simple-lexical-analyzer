//
//  main.cpp
//  TradutoresGA
//
//  Created by Eduardo Eidelwein Berlitz on 16/04/17.
//  Copyright © 2017 Eduardo Eidelwein Berlitz. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>

int main(int argc, const char * argv[]) {
    std::ifstream input("test.txt");
    if (!input)
    {
        std::cerr << "Error opening \"input.txt\".\r\n";
        return 1;
    }
    
    // Read the file.
    std::string input_text;
    while (std::getline(input, input_text))
    {
        static const char white_space[] = " \t";
        static const char identifier_letters[] = "abcdefghijklmnopqrstuvwxyz";
        std::string::size_type position = 0;
        
        //std::cout << input_text;
        
        if (input_text.length() == 0)
        {
            continue; // Empty line
        }
        
        // Skip white space.
        position = input_text.find_first_not_of(white_space, 0);
        
        if (position == std::string::npos)
        {
            continue; // Blank line.
        }
        
        // Check if the first (non-whitespace character) is a
        //    valid identifier letter
        if (std::isalpha(input_text[position]))
        {
            // First character is a letter.
            // Find the end of the identifier.
            std::string::size_type identifier_end_posn = 0;
            identifier_end_posn = input_text.find_first_not_of(identifier_letters, position);
            if (identifier_end_posn != std::string::npos)
            {
                const int identifier_length = identifier_end_posn - position + 1;
                std::string identifier = input_text.substr(position, identifier_length);
                std::cout << identifier << "\n";
                continue;
            }
        }
    }
    
    return 0;
}
