#ifndef MEETING_HPP
#define MEETING_HPP

#include <string>
#include <sstream>
#include <iostream>
#include "array.hpp"
#include "bstree.hpp"
#include "person.hpp"

namespace shevchenko {

struct Meeting {
  size_t id1;
  size_t id2;
  size_t duration;
};

bool read_meeting(std::istream& in, Meeting& m) {
  std::string line;
  if (!std::getline(in, line)) {
    return false;
  }
  
  std::istringstream iss(line);
  size_t id1, id2, duration;
  if (!(iss >> id1 >> id2 >> duration)) {
    return false;
  }
  if (id1 == id2) {
    return false;
  }
  
  m.id1 = id1;
  m.id2 = id2;
  m.duration = duration;
  return true;
}

void print_meeting(const Meeting& m, std::ostream& out) {
  out << m.id2 << " " << m.duration << "\n";
}

bool compare_meeting(const Meeting& a, const Meeting& b) {
  if (a.id2 != b.id2) {
    return a.id2 < b.id2;
  }
  return a.duration < b.duration;
}

void sort_meetings(Array<Meeting>& meetings) {
  for (size_t i = 0; i < meetings.size(); ++i) {
    for (size_t j = i + 1; j < meetings.size(); ++j) {
      if (!compare_meeting(meetings[i], meetings[j])) {
        Meeting temp = meetings[i];
        meetings[i] = meetings[j];
        meetings[j] = temp;
      }
    }
  }
}

Array<Meeting> get_meetings_for_person(const Array<Meeting>& meetings,
                                       size_t person_id) {
  Array<Meeting> result;
  for (size_t i = 0; i < meetings.size(); ++i) {
    const Meeting& m = meetings[i];
    if (m.id1 == person_id) {
      result.push_back(m);
    } else if (m.id2 == person_id) {
      Meeting swapped = m;
      swapped.id1 = person_id;
      swapped.id2 = m.id1;
      result.push_back(swapped);
    }
  }
  return result;
}

void deanon_person(Array<Meeting>& meetings, size_t anon_id, size_t real_id) {
  size_t i = 0;
  while (i < meetings.size()) {
    Meeting& m = meetings[i];
    if (m.id1 == anon_id) {
      m.id1 = real_id;
    }
    if (m.id2 == anon_id) {
      m.id2 = real_id;
    }
    if (m.id1 == m.id2) {
      for (size_t j = i; j < meetings.size() - 1; ++j) {
        meetings[j] = meetings[j + 1];
      }
    } else {
      ++i;
    }
  }
}

}

#endif
