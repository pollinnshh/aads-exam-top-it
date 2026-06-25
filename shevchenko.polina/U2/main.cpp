#include "person.hpp"
#include "meeting.hpp"
#include <fstream>
#include <sstream>


struct AnonCollector {
  shevchenko::Array<size_t>* anons;
  void operator()(const shevchenko::Person& p) {
    if (p.info.empty()) {
      anons->push_back(p.id);
    }
  }
};

struct PersonCollector {
  std::ostream* out;
  void operator()(const shevchenko::Person& p) {
    if (!p.info.empty()) {
      shevchenko::print_person(p, *out);
    }
  }
};

int main(int argc, char* argv[]) {
  using namespace shevchenko;

  std::string in_file, data_file;

  for (int i = 1; i < argc; ++i) {
    std::string arg = argv[i];
    if (arg.substr(0, 3) == "in:") {
      if (!in_file.empty()) {
        std::cerr << "duplicate in: argument\n";
        return 1;
      }
      in_file = arg.substr(3);
    } else if (arg.substr(0, 5) == "data:") {
      if (!data_file.empty()) {
        std::cerr << "duplicate data: argument\n";
        return 1;
      }
      data_file = arg.substr(5);
    } else {
      std::cerr << "unknown argument: " << arg << "\n";
      return 1;
    }
  }

  if (data_file.empty()) {
    std::cerr << "data: argument is required\n";
    return 1;
  }

  BST<Person> persons;
  if (!in_file.empty()) {
    std::ifstream in_stream(in_file);
    if (!in_stream.is_open()) {
      std::cerr << "cannot open persons file: " << in_file << "\n";
      return 2;
    }
    Person p;
    while (read_person(in_stream, p)) {
      if (p.info.empty()) {
        continue;
      }
      if (!persons.contains(p.id)) {
        persons.insert(p);
      }
    }
  }

  Array<Meeting> meetings;
  std::ifstream data_stream(data_file);
  if (!data_stream.is_open()) {
    std::cerr << "cannot open data file: " << data_file << "\n";
    return 2;
  }

  Meeting m;
  while (read_meeting(data_stream, m)) {
    meetings.push_back(m);
  }

  std::string line;
  while (std::getline(std::cin, line)) {
    std::istringstream iss(line);
    std::string cmd;
    if (!(iss >> cmd)) {
      continue;
    }

    if (cmd == "anons") {
      Array<size_t> anons;
      AnonCollector collector;
      collector.anons = &anons;
      persons.traverse(collector);
      for (size_t i = 0; i < anons.size(); ++i) {
        std::cout << anons[i] << "\n";
      }
    }
    else if (cmd == "deanon") {
      size_t anon_id, real_id;
      if (!(iss >> anon_id >> real_id)) {
        std::cout << "<INVALID COMMAND>\n";
        continue;
      }
      Person* anon = persons.find(anon_id);
      Person* real = persons.find(real_id);
      if (!anon || !real) {
        std::cout << "<INVALID COMMAND>\n";
        continue;
      }
      if (!anon->info.empty() || real->info.empty()) {
        std::cout << "<INVALID COMMAND>\n";
        continue;
      }
      deanon_person(meetings, anon_id, real_id);
      persons.erase(anon_id);
    }
    else if (cmd == "redesc") {
      size_t id;
      if (!(iss >> id)) {
        std::cout << "<INVALID COMMAND>\n";
        continue;
      }
      std::string token;
      std::string full_desc;
      bool in_quotes = false;
      while (iss >> token) {
        if (token.front() == '"' && token.back() == '"') {
          full_desc = token.substr(1, token.size() - 2);
          in_quotes = true;
          break;
        }
        if (token.front() == '"') {
          full_desc = token.substr(1);
          in_quotes = true;
        } else if (in_quotes) {
          full_desc += " " + token;
        }
        if (in_quotes && token.back() == '"') {
          full_desc.pop_back();
          break;
        }
      }
      if (!in_quotes) {
        std::cout << "<INVALID COMMAND>\n";
        continue;
      }

      Person* p = persons.find(id);
      if (p) {
        p->info = full_desc;
      } else {
        Person new_p{id, full_desc};
        persons.insert(new_p);
      }
    }
    else if (cmd == "desc") {
      size_t id;
      if (!(iss >> id)) {
        std::cout << "<INVALID COMMAND>\n";
        continue;
      }
      Person* p = persons.find(id);
      if (!p || p->info.empty()) {
        std::cout << "<ANON>\n";
      } else {
        std::cout << p->info << "\n";
      }
    }
    else if (cmd == "meets") {
      size_t id;
      if (!(iss >> id)) {
        std::cout << "<INVALID COMMAND>\n";
        continue;
      }
      Array<Meeting> filtered = get_meetings_for_person(meetings, id);
      sort_meetings(filtered);
      for (size_t i = 0; i < filtered.size(); ++i) {
        print_meeting(filtered[i], std::cout);
      }
    }
    else if (cmd == "commons") {
      size_t id1, id2;
      if (!(iss >> id1 >> id2)) {
        std::cout << "<INVALID COMMAND>\n";
        continue;
      }
      Array<Meeting> m1 = get_meetings_for_person(meetings, id1);
      Array<Meeting> m2 = get_meetings_for_person(meetings, id2);

      Array<size_t> result;
      for (size_t i = 0; i < m1.size(); ++i) {
        size_t candidate = m1[i].id2;
        bool found = false;
        for (size_t j = 0; j < m2.size(); ++j) {
          if (m2[j].id2 == candidate) {
            found = true;
            break;
          }
        }
        if (found) {
          bool already = false;
          for (size_t k = 0; k < result.size(); ++k) {
            if (result[k] == candidate) {
              already = true;
              break;
            }
          }
          if (!already) {
            result.push_back(candidate);
          }
        }
      }
      for (size_t i = 0; i < result.size(); ++i) {
        for (size_t j = i + 1; j < result.size(); ++j) {
          if (result[i] > result[j]) {
            size_t temp = result[i];
            result[i] = result[j];
            result[j] = temp;
          }
        }
      }
      for (size_t i = 0; i < result.size(); ++i) {
        std::cout << result[i] << "\n";
      }
    }
    else if (cmd == "less" || cmd == "greater") {
      size_t time, id;
      if (!(iss >> time >> id)) {
        std::cout << "<INVALID COMMAND>\n";
        continue;
      }
      bool is_less = (cmd == "less");
      Array<Meeting> filtered = get_meetings_for_person(meetings, id);

      Array<Meeting> result;
      for (size_t i = 0; i < filtered.size(); ++i) {
        bool condition = is_less
        ? filtered[i].duration < time
        : filtered[i].duration > time;
        if (condition) {
          result.push_back(filtered[i]);
        }
      }

      sort_meetings(result);
      for (size_t i = 0; i < result.size(); ++i) {
        print_meeting(result[i], std::cout);
      }
    }
    else if (cmd == "out-persons") {
      std::string filename;
      if (!(iss >> filename)) {
        std::cout << "<INVALID COMMAND>\n";
        continue;
      }
      std::ofstream out_stream(filename);
      if (!out_stream.is_open()) {
        std::cout << "<INVALID COMMAND>\n";
        continue;
      }
      PersonCollector collector;
      collector.out = &out_stream;
      persons.traverse(collector);
    }
    else {
      std::cout << "<INVALID COMMAND>\n";
    }
  }

  return 0;
}
