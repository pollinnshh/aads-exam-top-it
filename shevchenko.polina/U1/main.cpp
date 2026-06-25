#include "person.hpp"
#include "array.hpp"
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

  BST<Person> persons;
  Array<Person> order;
  size_t success_count = 0;
  size_t ignored_count = 0;

  Person p;
  while (read_person(*in, p)) {
    if (p.id == 0 && p.info.empty()) {
      continue;
    }
    if (p.info.empty()) {
      ++ignored_count;
      continue;
    }
    if (persons.contains(p.id)) {
      ++ignored_count;
      continue;
    }
    persons.insert(p);
    order.push_back(p);
    ++success_count;
  }
  if (!in_file.empty()) {
    in_file_stream.close();
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

  for (size_t i = 0; i < order.size(); ++i) {
    print_person(order[i], *out);
  }

  std::cerr << success_count << " " << ignored_count << "\n";

  return 0;
}
