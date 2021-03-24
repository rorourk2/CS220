#include "parser.h"
#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[]) {
  struct student *head;
  char error_string[1024];
  memset(error_string, 0, sizeof(error_string));

  if (argc != 2) {
    fprintf(stderr, "USAGE: [driver|static-driver] input_file\n");
  } else {
    head = read_txt(argv[1], error_string);
    if (head == NULL) {
      printf("READ failure: %s\n", error_string);
    } else {
      if (SUCCESS != write_txt("output.txt", error_string)) {
        printf("WRITE failure: %s\n", error_string);
      }
      list_students();
      if (SUCCESS != write_bin("output.bin", error_string)) {
        printf("WRITE BIN failure: %s\n", error_string);
      }
      if (NULL == read_bin("output.bin", error_string)) {
        printf("READ BIN failure: %s\n", error_string);
      }
      enroll_student("Alice", 28, 'F', 3.92);
      add_course("Alice", 110, 'A');
      add_course("Alice", 100, 'B');
      add_course("Alice", 105, 'A');
      add_course("Alice", 440, 'A');
      add_course("Alice", 300, 'B');
      if (SUCCESS != write_txt("output1.txt", error_string)) {
        printf("WRITE failure: %s\n", error_string);
      }
      delete_all();
    }
  }
  return 0;
}
