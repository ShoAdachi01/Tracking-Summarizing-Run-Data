#include <iostream>
#include "functions.h"
using namespace std;

void logInMenu() {
  string username, password;

  int option;

  cout << "******************** LogIn Menu ********************";
  cout << "\nEnter a Choice 1-6\n\t1)Log In\n\t2)Create New Account\n\t3)End "
          "Program\nEnter 1-3: ";
  cin >> option;
  if (option == 1) {
    if (logIn()) {
      mainMenu();
    } else {
      logInMenu();
    }
  } else if (option == 2) {
    cout << "Enter Username: ";
    cin >> username;
    cout << "Enter Password: ";
    cin >> password;
    registerUser(username, password);
    logInMenu();
  } else if (option == 3) {
    // https://stackoverflow.com/questions/35334285/is-there-a-way-to-end-a-program-within-a-void-function
    // Ends program
    exit(0);
  }
}
