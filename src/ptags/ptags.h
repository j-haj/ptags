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


#define PTAG_START() {ptags::Ptag::start(__func__);}
#define PTAG_STOP() {ptags::Ptag::stop(__func__);}

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


class Ptag {

  public:

    using ms = std::chrono::milliseconds;
    using hi_res_clock = std::chrono::high_resolution_clock;
    using hi_res_time_point = std::chrono::time_point<hi_res_clock>;

    static std::ofstream log_file;
    static std::map<std::string, std::queue<long>> tag_table;
   
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
      auto t = std::chrono::duration_cast<ms>(hi_res_clock::now().time_since_epoch()).count();
      tag_table[fn].push(t); 

      log_file << format_output(fn, t, t, "start");
    }

    /**
     * Creates a stop tag in the tag log file
     *
     * @param fn_name name of the calling function (__func__)
     */
    static void stop(const char* fn_name) {
      std::string fn = std::string(fn_name);
      auto t = std::chrono::duration_cast<ms>(hi_res_clock::now().time_since_epoch()).count();
      
      long t0 = tag_table[fn].front();
      tag_table[fn].pop();
      
      log_file << format_output(fn, t0, t, "end");
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
    static std::string format_output(const std::string name,
      const long t1,
      const long t2,
      const std::string msg) {
      

      std::cout << "t1: " << t1 << "\nt2: " << t2 << std::endl;
      std::cout << "dif: " << t2 - t1 << std::endl;
      std::stringstream ss;
      ss << ptags::DateTime::formatted_time_now() << "\t" << "[" << name << "]"
         << "\t" << t2 - t1 << "\t" << msg << "\n";

      return ss.str();
    }
}; // class Ptag

// Instantiate static member vars
std::ofstream Ptag::log_file;
std::map<std::string, std::queue<long>> Ptag::tag_table;

}// namespace ptags
#endif // __PTAGS_H
