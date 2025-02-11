#include "functions.h"
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>
#include <cstring>
#include <algorithm>
#include <cctype>
#include <iostream>

pid_t create_process()
{
    pid_t pid;

    do {
	pid = fork();
    } while ((pid == -1) && (errno == EAGAIN));

    return pid;
}

void read_count(int fd, void *buf, size_t count)
{
  //bytes read per call
  ssize_t bytes;
  for (size_t i = 0; i < count; i += bytes) {
    errno = 0;
    bytes = read(fd, (void*)((char*)buf+i), count-i);
    if ((bytes == -1) || (bytes == 0 && errno > 0)) {
      std::cerr << "Read pipe encountered fatal error." << std::endl;
      exit(EXIT_FAILURE);
    }
  }
}

void write_count(int fd, void *buf, size_t count)
{
  //bytes written per call
  ssize_t bytes;
  for (size_t i = 0; i < count; i += bytes) {
    errno = 0;
    bytes = write(fd, (void*)((char*)buf+i), count-i);
    if ((bytes == -1) || (bytes == 0 && errno > 0)) {
      std::cerr << "Write pipe encountered fatal error." << std::endl;
      exit(EXIT_FAILURE);
    }
  }
}

bool file_exists(const std::string& name) {
  struct stat buffer;
  return (stat (name.c_str(), &buffer) == 0);
}

char* concat(const char *s1, const char *s2)
{
    char *result = (char*)malloc(strlen(s1)+strlen(s2)+1); //+1 for the null-terminator
    //in real code you would check for errors in malloc here
    strcpy(result, s1);
    strcat(result, s2);
    return result;
}

bool strstri(const std::string & strHaystack, const std::string & strNeedle)
{
  auto it = std::search(
    strHaystack.begin(), strHaystack.end(),
    strNeedle.begin(),   strNeedle.end(),
    [](char ch1, char ch2) { return std::toupper(ch1) == std::toupper(ch2); }
  );
  return (it != strHaystack.end() );
}

void mkdir_default(const char *pathname)
{
    int mkdir_error = mkdir(pathname, S_IRWXU | S_IRWXG | S_IROTH);
    if (mkdir_error != 0 && errno != EEXIST) {
		    std::cerr << "Unable to create the folder " << pathname << ", errno " << errno << ")." << std::endl;
        exit(EXIT_FAILURE);
    }
}