/*
  PHYS30762 Programming in C++
  Assignment 2
  Author: Leo Feasby
  Date: 23/02/2024

  Description:
  This program processes course data by reading from a file, sorting the data based on user preference,
  and filtering and printing course information according to user input. Additionally, it calculates
  statistical measures such as the mean and standard deviation of course marks.
*/

#include <iostream>
#include <iomanip>
#include <fstream>
#include <cmath>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>

// Class to represent course information
class course 
{
public:
  double mark;
  int course_code;
  std::string name;
  std::string full_title;

  // Constructor that initializes course details and constructs the full title
  course(double mark, int course_code, const std::string& name)
    : mark(mark), course_code(course_code), name(name) 
  {
    std::stringstream ss;
    ss << "PHYS " << course_code << " " << name; // Constructing full title
    full_title = ss.str();
  }

  // Function to get the year of the course from its code
  int get_year() const 
  {
    return course_code / 10000;
  }
};

// Declaration of functions used in the program
void print_courses_and_statistics(const std::vector<course>& courses);
void sort_courses(std::vector<course>& courses, const std::string& sort_by);
double calculate_mean(const std::vector<double>& values);
double calculate_standard_deviation(const std::vector<double>& values, double mean);
void filter_and_print_courses(std::vector<course>& courses, char filter_option);

int main() 
{
  std::string data_file;
  std::vector<course> courses;

  // Prompt user for the filename containing course data
  std::cout << "Enter data filename: ";
  std::cin >> data_file;

  // Open the file and check for errors
  std::ifstream course_stream(data_file);
  if (!course_stream.is_open()) 
  {
    std::cerr << "Error opening file.\n";
    return 1;
  }

  // Read course data from file
  double mark;
  int course_code;
  std::string name;
  while (course_stream >> mark >> course_code) 
  {
    std::getline(course_stream, name);
    if (!name.empty()) 
    {
      name = name.substr(1); // Remove leading space
    }
    courses.emplace_back(mark, course_code, name);
  }

  course_stream.close();
  std::cout << "Total Number of Courses Loaded: " << courses.size() << "\n";

  // Sorting courses based on user preference
  std::string sort_option;
  std::cout << "Sort courses by 'title', 'code', or 'mark'? ";
  std::cin >> sort_option;
  sort_courses(courses, sort_option);

  // Filtering and printing courses based on user input
  char filter_option;
  std::cout << "Print all courses (A) or filter by year (1-4)? Enter A or 1-4: ";
  std::cin >> filter_option;
  filter_and_print_courses(courses, filter_option);

  return 0;
}

// Function to filter and print courses based on the specified filter option
void filter_and_print_courses(std::vector<course>& courses, char filter_option) 
{
  int year_filter = filter_option - '0';
  std::vector<course> filtered_courses;

  // Filtering courses
  for (const auto& course : courses) 
  {
    if (filter_option == 'A' || course.get_year() == year_filter) 
    {
      filtered_courses.push_back(course);
    }
  }

  // Printing filtered courses
  std::cout << "Filtered Courses:\n";
  for (const auto& course : filtered_courses) 
  {
    std::cout << course.full_title << " - Mark: " << course.mark << "\n";
  }

  // Calculating and printing statistics if applicable
  if (!filtered_courses.empty()) 
  {
    std::vector<double> marks;
    for (const auto& course : filtered_courses) 
    {
      marks.push_back(course.mark);
    }
    double mean = calculate_mean(marks);
    double std_dev = calculate_standard_deviation(marks, mean);
    std::cout << "Mean for selected courses: " << mean << "\nStandard Deviation: " << std_dev << "\n";
  }
  std::cout << "Number of courses selected: " << filtered_courses.size() << "\n";
}

// Function to sort courses based on the specified criteria
void sort_courses(std::vector<course>& courses, const std::string& sort_by) 
{
  if (sort_by == "title") 
  {
    std::sort(courses.begin(), courses.end(), [](const course& a, const course& b) 
    {
      return a.name < b.name;
    });
  }
  else if (sort_by == "code") 
  {
    std::sort(courses.begin(), courses.end(), [](const course& a, const course& b) 
    {
      return a.course_code < b.course_code;
    });
  }
  else if (sort_by == "mark") 
  {
    std::sort(courses.begin(), courses.end(), [](const course& a, const course& b) 
    {
      return a.mark < b.mark;
    });
  }
}

// Function to calculate the mean of a set of values
double calculate_mean(const std::vector<double>& values) 
{
  double sum = 0;
  for (double value : values) 
  {
    sum += value;
  }
  return sum / values.size();
}

// Function to calculate the standard deviation of a set of values
double calculate_standard_deviation(const std::vector<double>& values, double mean) 
{
  double sum = 0;
  for (double value : values) 
  {
    sum += std::pow(value - mean, 2);
  }
  return std::sqrt(sum / (values.size() - 1));
}
