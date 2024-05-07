#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unistd.h>
using namespace std;

// Creating Struct for user
struct usr {
  string username;
  string password;
};
usr currentUsr; // created to store currently logged in user

// Struct for run data
struct RunData {
  string date;
  double distance;
  double time;
  string comment;
};

// function prototypes so i can call functions
// before i actually make the function
void logInMenu();
void mainMenu();

// Checks Availability of username reading the user.txt file comparing the
// username in the file to the provided username boolean because its either
// taken or not
bool usernameAvailability(const usr &currentUsr) {
  string line, existingUser;
  ifstream inputFile("user.txt"); // REads user.txt file which stores all user
                                  // info such as password and username
  if (inputFile.is_open()) {

    while (getline(inputFile, line)) { // Gets allthe line from the user.txt
                                       // file
      stringstream ss(line);
      // https://www.geeksforgeeks.org/stringstream-c-applications/
      // stringstream allows me to extract the data and compare easily
      // extracting user.txt info using stringstream
      ss >> existingUser;
      // if these existingUser which we collect in this loop ever matches with
      // the username that the user provided matches it will return false not
      // allowing them to create an account w the same username
      if (existingUser == currentUsr.username) {
        inputFile.close();
        return false;
      }
    }
    inputFile.close();
  }
  return true;
}

// Function to register an user
void registerUser(const string &username, const string &password) {
  // If username is taken it will output username already exists
  if (!usernameAvailability({username, password})) {
    cout << "Username already exists" << endl;
    sleep(1);        // stops the code for 1 second
    system("clear"); // clears screen
    return;
  }
  // https://www.geeksforgeeks.org/cpp-program-to-append-a-string-in-an-existing-file/
  // ios::app will append a string in existing text file so it would not
  // overwrite the previous datas
  ofstream outPutFile("user.txt", ios::app);
  if (outPutFile.is_open()) {
    outPutFile << username << ' ' << password << endl;
    outPutFile.close();
    cout << "Successfully Registered" << endl;

  } else {
    cout << "Failed to Register" << endl;
  }
}

//
bool logIn() {
  int contOpt;
  string line, username, password, fileUsername, filePassword;
  // Gets user input
  cout << "\n\tEnter Username: ";
  cin >> currentUsr.username;
  cout << "\n\n\tEnter Password: ";
  cin >> currentUsr.password;
  ifstream inputFile("user.txt");
  if (inputFile.is_open()) {
    bool found = false;
    // reads file line by line and extracting the username and password
    // the username and password matches then it proceeds
    while (inputFile >> fileUsername >> filePassword) {
      if (fileUsername == currentUsr.username &&
          filePassword == currentUsr.password) {
        found = true;
        break;
      }
    }
    inputFile.close();

    if (!found) { // Error Trapping. If there are no matches between username
                  // and password or either one of them it will output the
                  // following message
      cout << "INVALID Username or Password" << endl;
      // https://www.geeksforgeeks.org/sleep-function-in-cpp/
      //  sleep(2); stops function for 2 seconds .
      //  From the #include <unistd.h>
      sleep(1);
      system("clear");
      logInMenu(); // take em back to login menu
      return false;
    } else {
      cout << "\n\nLog In Successful" << endl;
      return true;
    }
  }
}

// Add Run Function
void addRun() {
  // Creating a text file from the username to store a data for every user
  // idea from my friend Jun Cheng
  string filename;
  int orderNum = 1;
  filename = currentUsr.username + "run_data.txt";
  // ios::app will allow a new data to be written at the end of the content so
  // it would not overwrite
  ofstream fileOutput(filename, ios::app);
  // From The struct at the very top
  RunData run;
  char choice =
      'y'; // Setting as y at begenning so the following code runs first
  while (choice == 'y' || choice == 'Y') {
    // GEtting user info about stuff and storing and writing in the textfile
    cout << "Enter Date(DD/MM/YYYY): ";
    cin >> run.date;
    cout << "Enter Distance(km): ";
    cin >> run.distance;
    cout << "Enter Time(min): ";
    cin >> run.time;
    cout << "Any Comment: ";
    cin.ignore();
    getline(cin, run.comment);
    fileOutput << run.date << " " << run.distance << " " << run.time << " "
               << run.comment << endl;
    cout << "\n\nSuccessfully Added The Run\n" << endl;
    cout << "Add Another Run? (y/n): ";
    cin >> choice; // Getting choice if the user does not put a y then it would
                   // end but loops as long as they enter y
  }
  fileOutput.close();
}

// DElete Run Function
void deleteRun() {

  int selectedNum, runToDel, currentLine = 0, numRun = 0;
  string filename, line;
  // filename already commented abt in in addRun() function
  filename = currentUsr.username + "run_data.txt";
  ifstream inPutFile(filename);
  // If the file does not exist then following outputs
  if (!inPutFile.is_open()) {
    cout << "No Runs Saved for " << currentUsr.username << endl;
    return;
  }
  // If it exists we see how many run data exists in the file
  if (inPutFile.is_open()) {
    while (getline(inPutFile, line)) {
      numRun++; // counting how many lines exist in the file
    }
    inPutFile.close();
  }
  // If there are no runs saved in this file It will output the following
  if (numRun == 0) {
    cout << "No Runs Saved for" << currentUsr.username << endl;
    return;
  }
  // If there is something in the file it will open and do the following
  if (numRun > 0) {
    inPutFile.open(filename);
    cout << "------------------------------------------------------------------"
            "-------------\n";
    cout << "Runs Saved for " << currentUsr.username << endl;

    cout << "Date(DD/MM/YYYY)\t\tDistance(km)\t\tTime(min)\t\tComment\n";
    cout << "------------------------------------------------------------------"
            "-------------\n";
    for (int i = 0; i < numRun; i++) {
      // getting the line and outputting the line but we put a number before it
      // so it is easier for the user to get rid of the run data loops until
      // numRun which was counted previously
      getline(inPutFile, line);
      cout << i + 1 << ")\t" << line << endl;
    }
    inPutFile.close();
    // Asking which number to delete
    cout << "\n\nEnter the number of the run to delete(0 to CANCEL): ";
    cin >> runToDel;
    // Error trap if the number is less then 1 or above what belongs in the file
    // it will tell them invalid number
    if (runToDel < 0 || runToDel > numRun) {
      cout << "\n\nInvalid Number";
      return;
    }
    //If they want to cancel 
    if (runToDel == 0) {
      cout << "\nCanceled" << endl;
      return;
    }
    // Creating a temp file to write data and opening the original file for
    // reading
    ifstream inputFile2(filename);
    ofstream outputFile("temp.txt");

    if (outputFile.is_open() && inputFile2.is_open()) {
      // This gets the line from the file but when the currentLine matches the
      // runToDel it will not output the line to the new file
      while (getline(inputFile2, line)) {
        currentLine++;
        if (currentLine != runToDel) {
          outputFile << line << endl;
        }
      }
      inputFile2.close();
      outputFile.close();
      // The original file is removed and a new file is named the original name
      // https://mathbits.com/MathBits/CompSci/Files/Name.htm#:~:text=The%20remove(%20)%20function%20requires%20a,a%20standard%20C%2B%2B%20string%20variable.&text=The%20rename(%20)%20function%20takes%20the,file%20as%20a%20second%20argument.

      remove(filename.c_str());
      rename("temp.txt", filename.c_str());

      cout << "Deleted Successfully" << endl;
    } else {
      cout << "Failed to Delete" << endl;
    }
  }
}

void showHistory() {
  string filename, line;
  // explained previously
  filename = currentUsr.username + "run_data.txt";
  // open file to read
  ifstream inPutFile(filename);
  if (inPutFile.is_open()) {
    // printout header
    cout << "Run History for\t" << currentUsr.username << ":\n" << endl;
    cout << "------------------------------------------------------------------"
            "-------------\n";
    cout << "Date(DD/MM/YYYY)\t\tDistance(km)\t\tTime(min)\t\tComment\n";
    cout << "------------------------------------------------------------------"
            "-------------\n";
    // Reads everything on the file and outputs everything
    while (getline(inPutFile, line)) {
      cout << "\n" << line << endl;
    }
  }
}

void summary(int year, int month) {
  double totalDistance = 0, yearlyDistance = 0, monthDistance = 0,
         weekDistance = 0, totalTime = 0, yearlyTime = 0, monthTime = 0,
         weekTime = 0;
  int runYear, runMonth;
  string line, date, distance, time, comment, letter;
  string filename = currentUsr.username + "run_data.txt";
  ifstream inputFile(filename);

  if (inputFile.is_open()) {
    while (getline(inputFile, line)) {
      // Extracting data from the current users
      // https://stackoverflow.com/questions/30903847/c-stringstream-value-extraction
      stringstream ss(line);
      ss >> date >> distance >> time >> comment;

      // Getting the year and date only works from the format DD/MM/YYYY
      // stoi makes string to integer
      runYear = stoi(date.substr(6, 4));  // sub string of 6,4 represents the
                                          // year
      runMonth = stoi(date.substr(3, 2)); // sub string 3,2 is the month portion

      // Calculate Total distance & time
      // stod makes string into double
      // adds every distance and time
      totalDistance += stod(distance);
      totalTime += stod(time);

      // if the run year and month matches up it adds up the distance and time
      if (runYear == year && runMonth == month) {
        monthDistance += stod(distance);
        monthTime += stod(time);
      }
      // if the year matches up it will add up the distance and time
      if (runYear == year) {
        yearlyDistance += stod(distance);
        yearlyTime += stod(time);
      }
    }
    inputFile.close();
    // The summary outputting what we just calculated
    cout << "\n\n******************** SUMMARY ********************\n\n" << endl;
    cout << "\n\tTotal Distance Ran Overall: " << totalDistance << "km" << endl;
    cout << "\tOverall Time: " << totalTime << "min\n" << endl;
    cout << "\n\tYearly Distance Ran: " << yearlyDistance << "km" << endl;
    cout << "\tTotal Time Ran Year: " << yearlyTime << "min\n" << endl;
    cout << "\n\tDistance Ran in selected year and month (" << month << "/"
         << year << "): " << monthDistance << "km" << endl;
    cout << "\tTotal Time Ran in selected year and month(" << month << "/"
         << year << "): " << monthTime << "min\n"
         << endl;
    cout << "\n\n\nType Any Letter To Return: ";
    cin >> letter;
  } else {
    cout << "File Error";
    return;
  }
}

void racePaceCalc() {
  char option = 'y';
  int choice;
  double timeHr = 0, timeMin = 0, timeSec = 0;
  double distanceLap, pacePerLap, customDistance, totalDistance = 0;
  while (option == 'y' || option == 'Y') {
    cout << "\nEvent Distance\n\t1)1500m\n\t2)1600m\n\t3)1 "
            "mile\n\t4)3km\n\t5)3200m\n\t6)2 "
            "mile\n\t7)5km\n\t8)10km\n\t9)15km\n\t10)Half "
            "Marathon\n\t11)Marathon\n\t12)Custom"
         << endl;
    cout << "Enter Choice (1-12)" << endl;
    cin >> choice;
    switch (choice) {
    // if entered 1
    case 1: {
      cout << "Enter Goal Time(Format  hr m s): ";
      cin >> timeHr >> timeMin >> timeSec;
      cout << "Enter distance per lap(in meters): ";
      cin >> distanceLap;

      // Calculating the Total Second of goal time
      int totalTimeSec = (timeHr * 3600) + (timeMin * 60) + timeSec;

      // Calculate the Pace per lap
      pacePerLap = totalTimeSec / (1500.0 / distanceLap);

      // output calculated result
      cout << "\n\nYou need to run: " << pacePerLap
           << "per lap to achieve your goal time" << endl;
      while (totalDistance < 1500) {
        // Calculate the lap time in second
        double lapTimeSec = totalTimeSec / (1500.0 / totalDistance);
        // Extract hour from lap time
        int lapTimeHr = lapTimeSec / 3600;
        lapTimeSec -=
            lapTimeHr * 3600; // subtracting the seconds accounted for hour
        int lapTimeMin =
            lapTimeSec /
            60; // same thing for the min same as the the 2 lines  above this
        lapTimeSec -=
            lapTimeMin * 60; // subtracting the seconds accounted for min
        // printout the total distance and lap time in hr, min and second
        cout << totalDistance << "m\t" << lapTimeHr << "hr" << lapTimeMin
             << "min" << lapTimeSec << "sec" << endl;
        totalDistance +=
            distanceLap; // adds up the total distance by distance per lap
      }
      // Summarizing the goal time in this case 1500m
      cout << "1500m\t" << timeHr << "hr" << timeMin << "min" << timeSec
           << "sec" << endl;
      cout << "\n\nCalculate Again? (y/n): ";
      cin >> option;
      totalDistance = 0; // resets the total distance to perform the calculation
                         // again if they chose so
      break;
    }
    case 2: {
      // Get user input
      cout << "Enter Goal Time(Format  hr m s): ";
      cin >> timeHr >> timeMin >> timeSec;
      cout << "Enter distance per lap(in meters): ";
      cin >> distanceLap;

      // Calculating the Total Second of goal time
      int totalTimeSec = (timeHr * 3600) + (timeMin * 60) + timeSec;

      // Calculate the Pace per lap
      pacePerLap = totalTimeSec / (1600.0 / distanceLap);

      // output calculated result
      cout << "\n\nYou need to run: " << pacePerLap
           << "sec per lap to achieve your goal time" << endl;
      while (totalDistance < 1600) {
        double lapTimeSec = totalTimeSec / (1600.0 / totalDistance);
        int lapTimeHr = lapTimeSec / 3600;
        lapTimeSec -= lapTimeHr * 3600;
        int lapTimeMin = lapTimeSec / 60;
        lapTimeSec -= lapTimeMin * 60;
        cout << totalDistance << "m\t" << lapTimeHr << "hr" << lapTimeMin
             << "min" << lapTimeSec << "sec" << endl;
        totalDistance += distanceLap;
      }
      cout << "1600m\t" << timeHr << "hr" << timeMin << "min" << timeSec
           << "sec" << endl;
      cout << "\n\nCalculate Again? (y/n): ";
      cin >> option;
      totalDistance = 0;
      break;
    }
    case 3: {
      // GEt User input
      cout << "Enter Goal Time(Format  hr m s): ";
      cin >> timeHr >> timeMin >> timeSec;
      cout << "Enter distance per lap(in meters): ";
      cin >> distanceLap;

      // Calculating the Total Second of goal time
      int totalTimeSec = (timeHr * 3600) + (timeMin * 60) + timeSec;

      // Calculate the Pace per lap
      pacePerLap = totalTimeSec / (1609.344 / distanceLap);

      // output calculated result
      cout << "\n\nYou need to run: " << pacePerLap
           << "sec per lap to achieve your goal time" << endl;
      while (totalDistance < 1609.344) {
        double lapTimeSec = totalTimeSec / (1609.344 / totalDistance);
        int lapTimeHr = lapTimeSec / 3600;
        lapTimeSec -= lapTimeHr * 3600;
        int lapTimeMin = lapTimeSec / 60;
        lapTimeSec -= lapTimeMin * 60;
        cout << totalDistance << "m\t" << lapTimeHr << "hr" << lapTimeMin
             << "min" << lapTimeSec << "sec" << endl;
        totalDistance += distanceLap;
      }
      cout << "1 mile\t" << timeHr << "hr" << timeMin << "min" << timeSec
           << "sec" << endl;
      cout << "\n\nCalculate Again? (y/n): ";
      cin >> option;
      totalDistance = 0;
      break;
    }
    case 4: {
      // GEt user input
      cout << "Enter Goal Time(Format  hr m s): ";
      cin >> timeHr >> timeMin >> timeSec;
      cout << "Enter distance per lap(in meters): ";
      cin >> distanceLap;

      // Calculating the Total Second of goal time
      int totalTimeSec = (timeHr * 3600) + (timeMin * 60) + timeSec;

      // Calculate the Pace per lap
      pacePerLap = totalTimeSec / (3000 / distanceLap);

      // output calculated result
      cout << "\n\nYou need to run: " << pacePerLap
           << "sec per lap to achieve your goal time" << endl;
      while (totalDistance < 3000) {
        double lapTimeSec = totalTimeSec / (3000 / totalDistance);
        int lapTimeHr = lapTimeSec / 3600;
        lapTimeSec -= lapTimeHr * 3600;
        int lapTimeMin = lapTimeSec / 60;
        lapTimeSec -= lapTimeMin * 60;
        cout << totalDistance << "m\t" << lapTimeHr << "hr" << lapTimeMin
             << "min" << lapTimeSec << "sec" << endl;
        totalDistance += distanceLap;
      }
      cout << "3000m\t" << timeHr << "hr" << timeMin << "min" << timeSec
           << "sec" << endl;
      cout << "\n\nCalculate Again? (y/n): ";
      cin >> option;
      totalDistance = 0;
      break;
    }
    case 5: {
      // Get User Input
      cout << "Enter Goal Time(Format  hr m s): ";
      cin >> timeHr >> timeMin >> timeSec;
      cout << "Enter distance per lap(in meters): ";
      cin >> distanceLap;

      // Calculating the Total Second of goal time
      int totalTimeSec = (timeHr * 3600) + (timeMin * 60) + timeSec;

      // Calculate the Pace per lap
      pacePerLap = totalTimeSec / (3200 / distanceLap);

      // output calculated result
      cout << "\n\nYou need to run: " << pacePerLap
           << "sec per lap to achieve your goal time" << endl;
      while (totalDistance < 3200) {
        double lapTimeSec = totalTimeSec / (3200 / totalDistance);
        int lapTimeHr = lapTimeSec / 3600;
        lapTimeSec -= lapTimeHr * 3600;
        int lapTimeMin = lapTimeSec / 60;
        lapTimeSec -= lapTimeMin * 60;
        cout << totalDistance << "m\t" << lapTimeHr << "hr" << lapTimeMin
             << "min" << lapTimeSec << "sec" << endl;
        totalDistance += distanceLap;
      }
      cout << "3200m\t" << timeHr << "hr" << timeMin << "min" << timeSec
           << "sec" << endl;
      cout << "\n\nCalculate Again? (y/n): ";
      cin >> option;
      totalDistance = 0;
      break;
    }
    case 6: {
      // Get User Input
      cout << "Enter Goal Time(Format  hr m s): ";
      cin >> timeHr >> timeMin >> timeSec;
      cout << "Enter distance per lap(in meters): ";
      cin >> distanceLap;

      // Calculating the Total Second of goal time
      int totalTimeSec = (timeHr * 3600) + (timeMin * 60) + timeSec;

      // Calculate the Pace per lap
      pacePerLap = totalTimeSec / (3218.688 / distanceLap);

      // output calculated result
      cout << "\n\nYou need to run: " << pacePerLap
           << "sec per lap to achieve your goal time" << endl;
      while (totalDistance < 3218.688) {
        double lapTimeSec = totalTimeSec / (3218.688 / totalDistance);
        int lapTimeHr = lapTimeSec / 3600;
        lapTimeSec -= lapTimeHr * 3600;
        int lapTimeMin = lapTimeSec / 60;
        lapTimeSec -= lapTimeMin * 60;
        cout << totalDistance << "m\t" << lapTimeHr << "hr" << lapTimeMin
             << "min" << lapTimeSec << "sec" << endl;
        totalDistance += distanceLap;
      }
      cout << "2 mile\t" << timeHr << "hr" << timeMin << "min" << timeSec
           << "sec" << endl;
      cout << "\n\nCalculate Again? (y/n): ";
      cin >> option;
      totalDistance = 0;
      break;
    }
    case 7: {
      // Get User Input
      cout << "Enter Goal Time(Format  hr m s): ";
      cin >> timeHr >> timeMin >> timeSec;
      cout << "Enter distance per lap(in meters): ";
      cin >> distanceLap;

      // Calculating the Total Second of goal time
      int totalTimeSec = (timeHr * 3600) + (timeMin * 60) + timeSec;

      // Calculate the Pace per lap
      pacePerLap = totalTimeSec / (5000 / distanceLap);

      // output calculated result
      cout << "\n\nYou need to run: " << pacePerLap
           << "sec per lap to achieve your goal time" << endl;
      while (totalDistance < 5000) {
        double lapTimeSec = totalTimeSec / (5000 / totalDistance);
        int lapTimeHr = lapTimeSec / 3600;
        lapTimeSec -= lapTimeHr * 3600;
        int lapTimeMin = lapTimeSec / 60;
        lapTimeSec -= lapTimeMin * 60;
        cout << totalDistance << "m\t" << lapTimeHr << "hr" << lapTimeMin
             << "min" << lapTimeSec << "sec" << endl;
        totalDistance += distanceLap;
      }
      cout << "5000m\t" << timeHr << "hr" << timeMin << "min" << timeSec
           << "sec" << endl;
      cout << "\n\nCalculate Again? (y/n): ";
      cin >> option;
      totalDistance = 0;
      break;
    }
    case 8: {
      // Get User Input
      cout << "Enter Goal Time(Format  hr m s): ";
      cin >> timeHr >> timeMin >> timeSec;
      cout << "Enter distance per lap(in meters): ";
      cin >> distanceLap;

      // Calculating the Total Second of goal time
      int totalTimeSec = (timeHr * 3600) + (timeMin * 60) + timeSec;

      // Calculate the Pace per lap
      pacePerLap = totalTimeSec / (10000 / distanceLap);

      // output calculated result
      cout << "\n\nYou need to run: " << pacePerLap
           << "sec per lap to achieve your goal time" << endl;
      while (totalDistance < 10000) {
        double lapTimeSec = totalTimeSec / (10000 / totalDistance);
        int lapTimeHr = lapTimeSec / 3600;
        lapTimeSec -= lapTimeHr * 3600;
        int lapTimeMin = lapTimeSec / 60;
        lapTimeSec -= lapTimeMin * 60;
        cout << totalDistance << "m\t" << lapTimeHr << "hr" << lapTimeMin
             << "min" << lapTimeSec << "sec" << endl;
        totalDistance += distanceLap;
      }
      cout << "10000m\t" << timeHr << "hr" << timeMin << "min" << timeSec
           << "sec" << endl;
      cout << "\n\nCalculate Again? (y/n): ";
      cin >> option;
      totalDistance = 0;
      break;
    }
    case 9: {
      // Get User Input
      cout << "Enter Goal Time(Format  hr m s): ";
      cin >> timeHr >> timeMin >> timeSec;
      cout << "Enter distance per lap(in meters): ";
      cin >> distanceLap;

      // Calculating the Total Second of goal time
      int totalTimeSec = (timeHr * 3600) + (timeMin * 60) + timeSec;

      // Calculate the Pace per lap
      pacePerLap = totalTimeSec / (15000 / distanceLap);

      // output calculated result
      cout << "\n\nYou need to run: " << pacePerLap
           << "sec per lap to achieve your goal time" << endl;
      while (totalDistance < 15000) {
        double lapTimeSec = totalTimeSec / (15000 / totalDistance);
        int lapTimeHr = lapTimeSec / 3600;
        lapTimeSec -= lapTimeHr * 3600;
        int lapTimeMin = lapTimeSec / 60;
        lapTimeSec -= lapTimeMin * 60;
        cout << totalDistance << "m\t" << lapTimeHr << "hr" << lapTimeMin
             << "min" << lapTimeSec << "sec" << endl;
        totalDistance += distanceLap;
      }
      cout << "15000m\t" << timeHr << "hr" << timeMin << "min" << timeSec
           << "sec" << endl;
      cout << "\n\nCalculate Again? (y/n): ";
      cin >> option;
      totalDistance = 0;
      break;
    }
    case 10: {
      // Get User Input
      cout << "Enter Goal Time(Format  hr m s): ";
      cin >> timeHr >> timeMin >> timeSec;
      cout << "Enter distance per lap(in meters): ";
      cin >> distanceLap;

      // Calculating the Total Second of goal time
      int totalTimeSec = (timeHr * 3600) + (timeMin * 60) + timeSec;

      // Calculate the Pace per lap
      pacePerLap = totalTimeSec / (21097.5 / distanceLap);

      // output calculated result
      cout << "\n\nYou need to run: " << pacePerLap
           << "sec per lap to achieve your goal time" << endl;
      while (totalDistance < 21097.5) {
        double lapTimeSec = totalTimeSec / (21097.5 / totalDistance);
        int lapTimeHr = lapTimeSec / 3600;
        lapTimeSec -= lapTimeHr * 3600;
        int lapTimeMin = lapTimeSec / 60;
        lapTimeSec -= lapTimeMin * 60;
        cout << totalDistance << "m\t" << lapTimeHr << "hr" << lapTimeMin
             << "min" << lapTimeSec << "sec" << endl;
        totalDistance += distanceLap;
      }
      cout << "Half Marathon\t" << timeHr << "hr" << timeMin << "min" << timeSec
           << "sec" << endl;
      cout << "\n\nCalculate Again? (y/n): ";
      cin >> option;
      totalDistance = 0;
      break;
    }
    case 11: {
      // Get User Input
      cout << "Enter Goal Time(Format  hr m s): ";
      cin >> timeHr >> timeMin >> timeSec;
      cout << "Enter distance per lap(in meters): ";
      cin >> distanceLap;

      // Calculating the Total Second of goal time
      int totalTimeSec = (timeHr * 3600) + (timeMin * 60) + timeSec;

      // Calculate the Pace per lap
      pacePerLap = totalTimeSec / (42195 / distanceLap);

      // output calculated result
      cout << "\n\nYou need to run: " << pacePerLap
           << "sec per lap to achieve your goal time" << endl;
      while (totalDistance < 42195) {
        double lapTimeSec = totalTimeSec / (42195 / totalDistance);
        int lapTimeHr = lapTimeSec / 3600;
        lapTimeSec -= lapTimeHr * 3600;
        int lapTimeMin = lapTimeSec / 60;
        lapTimeSec -= lapTimeMin * 60;
        cout << totalDistance << "m\t" << lapTimeHr << "hr" << lapTimeMin
             << "min" << lapTimeSec << "sec" << endl;
        totalDistance += distanceLap;
      }
      cout << "Marathon\t" << timeHr << "hr" << timeMin << "min" << timeSec
           << "sec" << endl;
      cout << "\n\nCalculate Again? (y/n): ";
      cin >> option;
      totalDistance = 0;
      break;
    }
    case 12: {
      // Get User Input
      cout << "\n\nEnter Distance(in meters): ";
      cin >> customDistance;
      cout << "\n\nEnter Goal Time(Format  hr m s): ";
      cin >> timeHr >> timeMin >> timeSec;
      cout << "\n\nEnter distance per lap(in meters): ";
      cin >> distanceLap;

      // Calculating the Total Second of goal time
      int totalTimeSec = (timeHr * 3600) + (timeMin * 60) + timeSec;

      // Calculate the Pace per lap
      pacePerLap = totalTimeSec / (customDistance / distanceLap);

      // output calculated result
      cout << "\n\nYou need to run: " << pacePerLap
           << "sec per lap to achieve your goal time" << endl;
      while (totalDistance < customDistance) {
        double lapTimeSec = totalTimeSec / (customDistance / totalDistance);
        int lapTimeHr = lapTimeSec / 3600;
        lapTimeSec -= lapTimeHr * 3600;
        int lapTimeMin = lapTimeSec / 60;
        lapTimeSec -= lapTimeMin * 60;
        cout << totalDistance << "m\t" << lapTimeHr << "hr" << lapTimeMin
             << "min" << lapTimeSec << "sec" << endl;
        totalDistance += distanceLap;
      }
      cout << customDistance << "m\t" << timeHr << "hr" << timeMin << "min"
           << timeSec << "sec" << endl;
      cout << "\n\nCalculate Again? (y/n): ";
      cin >> option;
      totalDistance = 0;
      break;
    }
    default: {
      cout << "Invalid Option" << endl;
      break;
    }
    }
  }
}

void mainMenu() {
  int choice, optionMenu, year, month;
  string option;
  double raceTime, raceDis;
  // while(true) allows it to loop infinitely until this becomes false
  while (true) {
    sleep(1);
    system("clear");
    // main menu header asking for choice
    cout << "\n******************** MAIN MENU ********************" << endl;
    cout << "\n\t1)Update Run\n\n\t2)Summary\n\n\t3)Calculate\n\n\t4)Sign "
            "Out\n\nEnter Choice(1-4): ";
    cin >> choice;
    switch (choice) {
    case 1: {
      // if 1 gives them another choice between 1-4
      cout << "\n******************** UPDATE RUN ********************" << endl;
      cout << "\n\t1)Add Run\n\n\t2)Delete "
              "Run\n\n\t3)History\n\n\t4)Exit\n\nEnter 1-4: ";
      cin >> optionMenu;
      if (optionMenu == 1) {
        // Adding a run
        addRun();
      } else if (optionMenu == 2) {
        // delete Run
        deleteRun();
      } else if (optionMenu == 3) {
        // Showing History of runs previously added
        showHistory();
        cout << "Type Any Letter To Go Back: ";
        cin >> option;
      } else if (optionMenu == 4) {
        // Back to main menu
        mainMenu();
      } else {
        cout << "Invalid Option";
        mainMenu();
      }
      break;
    }
    case 2: {
      // if 2 ask for year and month they want the summary for
      cout << "Select Year and Month of Summary\n" << endl;
      cout << "\tEnter Year: ";
      cin >> year;
      cout << "\tEnter Month: ";
      cin >> month;
      summary(year, month);
      // calling function and passing variables year and month to perform
      // calculation
      break;
    }
    case 3: {
      // if 3
      // calculates
      cout << "\n******************** Calculator ********************";
      cout << "\n\t1)Race Pace Calculator\n\t2)Main Menu\nEnter(1-2): ";
      cin >> optionMenu;
      if (optionMenu == 1) {
        // Runs the race pace calculator function
        racePaceCalc();
      } else if (optionMenu == 2) {
        sleep(1);
        system("clear");
        mainMenu();
        // this clears console and brings them back to mainmenu after 1 second
      } else {
        cout << "Invalid Option";
        // error trapping
      }
      break;
    }
    case 4: {
      // Sign Out. Clearing Current User Info And Going Back to LogInMenu
      currentUsr.username = " ";
      currentUsr.password = " ";
      cout << "You Have Been Signed Out";
      sleep(1);
      system("clear");
      logInMenu();

      break;
    }
    default: {
      cout << "Invalid Option" << endl;
      break;
    }
    }
  }
}

void logInMenu() {
  string username, password;

  int option;
  sleep(1);
  system("clear"); // clears console
  // choice of 1-3
  cout << "\n******************** LogIn Menu ********************\n";
  cout << "\nEnter a Choice 1-3\n\t1)Log In\n\t2)Create New Account\n\t3)End "
          "Program\nEnter 1-3: ";
  cin >> option;
  // if 1
  if (option == 1) {
    // if logIn is true, proceeds to mainMenu function
    if (logIn()) {
      mainMenu();
    } else {
      // if false goes back to loginmenu
      logInMenu();
    }
  } else if (option == 2) {
    // asking for a new username and password
    cout << "\nCreate New Account" << endl;
    cout << "Enter Username: ";
    cin >> username;
    cout << "Enter Password: ";
    cin >> password;
    // calls the registerUser and passing username and password in parameter
    registerUser(username, password);
    // once registerd goes to login menu
    logInMenu();
  } else if (option == 3) {
    // https://stackoverflow.com/questions/35334285/is-there-a-way-to-end-a-program-within-a-void-function
    // Ends program
    exit(0);
  } else {
    cout << "\nINVALID CHOICE";
    logInMenu();
  }
}
