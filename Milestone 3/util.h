//file util.h 
#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>


void csvRead(std::string& filename, char separator, std::vector<std::vector<std::string>> &csvdata);
void csvPrint(std::vector<std::vector<std::string>> &csvdata);
void trimSpace(std::string& s);

bool taskValid(const std::string s);

bool slotsValid(const std::string s);