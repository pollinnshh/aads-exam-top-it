#ifndef PERSON_HPP
#define PERSON_HPP

#include <string>
#include <iostream>
#include <cctype>
#include "bstree.hpp"

namespace shevchenko {

struct Person {
  size_t id;
  std::string info;
};

void print_person(const Person& p, std::ostream& out) {
  out << p.id << " " << p.info << "\n";
}

bool read_person(std::istream& in, Person& p) {
  std::string line;
  if (!std::getline(in, line)) {
    return false;
  }
  
  size_t pos = 0;
  while (pos < line.size() && std::isspace(line[pos])) {
    ++pos;
  }
  if (pos >= line.size()) {
    return false;
  }
  
  size_t id = 0;
  while (pos < line.size() && std::isdigit(line[pos])) {
    id = id * 10 + (line[pos] - '0');
    ++pos;
  }
  if (pos == 0) {
    return false;
  }
  
  while (pos < line.size() && std::isspace(line[pos])) {
    ++pos;
  }
  if (pos >= line.size()) {
    return false;
  }
  
  p.id = id;
  p.info = line.substr(pos);
  while (!p.info.empty() && std::isspace(p.info.back())) {
    p.info.pop_back();
  }
  
  return true;
}

}

#endif
