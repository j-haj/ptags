#ifndef __PTAGS_H
#define __PTAGS_H

#include <chrono>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <map>
#include <queue>

namespace ptags {

  class DateTime {
  public:
    static std::string formatted_time_now(std::string fmt="%d-%m-%Y_%H_%M_%S") {
      auto t = std::time(nullptr);
      auto tm = *std::localtime(&t);

      std::ostringstream oss;
      oss << std::put_time(&tm, fmt.c_str());
      return oss.str();
    }
}; // class DateTime

static std::ofstream log_file;
static std::map<std::string,
  std::queue<std::chrono::time_point<std::chrono::high_resolution_clock>>> tag_table;

class Ptag {

  public:
   
    /**
     * @p PTag initializer
     *
     * @param log name of the log file used for storing ptags
     */ 
    static void InitPTags(const std::string log="ptag_logs.out") {
      log_file.open(log, std::ios::out);
    }

    /**
     * Creates start tag in the tag log file
     *
     * @param fn_name name of the calling function (__func__)
     */
    static void start(const char* fn_name) {
      std::string fn = std::string(fn_name);
      auto t = std::chrono::high_resolution_clock::now();
      auto q = tag_table[fn];
      q.push(t);
      tag_table.insert(fn, q); 
      log_file << format_output(fn, t, 0, "start");
    }

    /**
     * Creates a stop tag in the tag log file
     *
     * @param fn_name name of the calling function (__func__)
     */
    static void stop(const char* fn_name) {
      std::string fn = std::string(fn_name);
      auto t = std::chrono::high_resolution_clock::now();
      auto t0 = tag_table[fn].pop();
      auto dif = std::chrono::duration_cast<std::chrono::milliseconds>(t - t0);
      std::cout << std::string(fn_name) << std::endl;
      log_file << format_output(std::string(fn_name), t, dif, "end");
    }

  private:
    Ptag() {}

    /**
     * Returns formatted output with function name, time, time dif and message
     *
     * @param name function name
     * @param t1 time of call
     * @param dif difference between this call and matching prior call
     * @param msg message to print in tag log file
     *
     * @return formatted string (@p std::string)
     */
    std::string format_output(const std::string name,
      const std::chrono::time_point<std::chrono::high_resolution_clock> t1,
      const std::chrono::duration<std::chrono::milliseconds> dif, const std::string msg) {

      std::stringstream ss;
      ss << ptags::DateTime::formatted_time_now() << "\t" << "[" << name << "]"
         << "\t" << dif.count() << "\t" << msg << "\n";

      return ss.str();
    }
}; // class Ptag
}// namespace ptags

#define PTAG_START() {ptags::Ptag::start(__func__);}
#define PTAG_STOP() {ptags::Ptag::stop(__func__);}

#endif // __PTAGS_H
