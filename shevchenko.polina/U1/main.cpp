#include "person.hpp"
#include <fstream>

int main(int argc, char* argv[]) {
  using namespace shevchenko;

  std::string in_file, out_file;

  if (!parse_args(argc, argv, in_file, out_file)) {
    std::cerr << "invalid arguments\n";
    return 1;
  }

  std::ifstream in_file_stream;
  std::istream* in = &std::cin;
  if (!in_file.empty()) {
    in_file_stream.open(in_file);
    if (!in_file_stream.is_open()) {
      std::cerr << "cannot open input file: " << in_file << "\n";
      return 2;
    }
    in = &in_file_stream;
  }

  std::ofstream out_file_stream;
  std::ostream* out = &std::cout;
  if (!out_file.empty()) {
    out_file_stream.open(out_file);
    if (!out_file_stream.is_open()) {
      std::cerr << "cannot open output file: " << out_file << "\n";
      return 2;
    }
    out = &out_file_stream;
  }

  BST<Person> persons;
  size_t success_count = 0;
  size_t ignored_count = 0;

  Person p;
  while (read_person(*in, p)) {
    if (p.info.empty()) {
      ++ignored_count;
      continue;
    }
    if (persons.contains(p.id)) {
      ++ignored_count;
      continue;
    }
    persons.insert(p);
    ++success_count;
  }

  persons.print(*out, print_person_func);
  std::cerr << success_count << " " << ignored_count << "\n";

  return 0;
}
