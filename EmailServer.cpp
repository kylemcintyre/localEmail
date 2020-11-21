//  Kyle McIntyre
//  EmailServer.cpp
//
//  EmailServer
//
//  Project for creating a local email server

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>

void MainMenu();
void Login();
void ShowInbox(std::string username, std::ostream & sout);
void LoggedInMenu(std::string username);
void RegisterNewUser();
void DeleteUser();
std::string GetUserFile(std::string username);
std::string GetInboxFile(std::string username);
std::string GetLine(std::istream & sin);
bool DoesUserExist(std::string username);
int GetInboxCount(std::string username);
std::string FormatEmailString(std::string from, std::string to, std::string subject, std::string msg);
void SendEmail(std::string username);
std::string GetPassword(std::string username);

//@include(main)
//@addRule(commentAll)
int main()
{
  MainMenu();
  return 0; // make sure your int main function returns an int
}
//@removeRule(commentAll)

void MainMenu()
{
  std::string input = "";
  int menuItem = 0;

  while (input != "99") // main menu display and switch case for selection
  {
    std::cout << " 1) Login" << "\n";
    std::cout << " 2) Register new user" << "\n";
    std::cout << " 3) Delete user" << "\n";
    std::cout << "99) Exit" << "\n";
    std::cout << "Selection: ";

    std::cin >> input;
    try
    {
      menuItem = stoi(input);
      switch (menuItem)
      {
      case 1:
        Login();
        break;
      case 2:
        RegisterNewUser();
        break;
      case 3:
        DeleteUser();
        break;
      }
    }
    catch (std::invalid_argument & ia) // error handling
    {
      std::cout << "\nBad option!" << std::endl;
      std::cout << ia.what() << std::endl;
    }
  }
}

// Now you fill in the rest

void Login()
{
  std::string username = "";
  std::string password = "";
  std::string getPassword = "";

  std::cout << "Enter username: ";
  std::cin >> username;  // get user input for username
  if (!DoesUserExist(username)) // call function to check if username is already taken
  {
    std::cout << "No such user!\n" << std::endl; // if taken kick back to main menu
    return;
  }
  else
  {
    std::cout << "Enter password: "; // get users password
    std::cin >> password;

    getPassword = (GetPassword(username)); // set variable to function return
    if (getPassword != password) // kick back to menu if password from return doesn't match what user typed
    {
      std::cout << "Password is incorrect!\n" << std::endl;
      return;
    }
    else
    {
      LoggedInMenu(username); // call logged in menu if username and password match
    }
  }
}

void ShowInbox(std::string username, std::ostream & sout)
{
  std::string currentLine;
  std::ifstream fin;

  fin.open(username + "-inbox.txt"); // open users inbox
  std::getline(fin, currentLine); // get first line

  while (!fin.eof()) // while file is not at end run
  {
    if (currentLine == "#email")
    {
      std::getline(fin, currentLine); // if preprocessor get the next line
    }
    else
    {
      std::cout << "\nFrom: " << currentLine << std::endl; // output each line of email from inbox
      std::getline(fin, currentLine);
      std::cout << "To: " << currentLine << std::endl;
      std::getline(fin, currentLine);
      std::cout << "Subject: " << currentLine << std::endl;
      std::getline(fin, currentLine);
      std::cout << "Message: " << currentLine << std::endl;
      std::getline(fin, currentLine);
    }
  }
}

void LoggedInMenu(std::string username)
{
  std::string input = "";
  int menuItem = 0;

  while (input != "99") // logged in menu and switch case for selection
  {
    std::cout << "\n 1) Inbox[" << GetInboxCount(username) << "]" << "\n";
    std::cout << " 2) Send Email" << "\n";
    std::cout << "99) Logout" << "\n";
    std::cout << "Selection: ";

    std::cin >> input;
    try
    {
      menuItem = stoi(input);
      switch (menuItem)
      {
      case 1:
        ShowInbox(username, std::cout);
        break;
      case 2:
        SendEmail(username);
        break;
      }
    }
    catch (std::invalid_argument & ia) // error handling
    {
      std::cout << "Bad option!\n" << std::endl;
      std::cout << ia.what() << std::endl;
    }
  }
}

void RegisterNewUser()
{
  std::string username = "";
  std::string password = "";
  std::string usernameFile, inboxFile;
  std::ofstream createUser;

  std::cout << "Enter username: ";
  std::cin >> username; // get users desired username

  if (username.length() < 3) // check to make sure username is at least 3 characters long
  {
    std::cout << "Username must be at least 3 characters!\n" << std::endl;
    return;
  }
  else
  {
    if (DoesUserExist(username)) // if user already exists return to main menu
    {
      std::cout << "User already exists!\n" << std::endl;
      return;
    }
    else
    {
      std::cout << "Enter password: "; // get users desired password
      std::cin >> password;
    }

    if (password.length() < 6) // check to make sure password is at least 6 characters long
    {
      std::cout << "Password must be at least 6 characters!\n" << std::endl;
      return;
    }
    else
    {
      usernameFile = GetUserFile(username); // call helper functions to get file names for user
      inboxFile = GetInboxFile(username);

      createUser.open(usernameFile); // create username and password file and close
      createUser << "username " << username << std::endl << "password " << password; // write to file
      createUser.close();

      createUser.open(inboxFile); // create users inbox file and close
      createUser << "#email" << std::endl << "mailservice" << std::endl << username << std::endl << "Intro" << std::endl << "Hello, " << username << "! Welcome to our service"; // write to file
      createUser.close();
    }
  }
}

void DeleteUser()
{
  std::string username, removeUser, removeInbox;

  std::cout << "What user would you like to delete? ";
  std::cin >> username; // get username user wants to delete

  if (!DoesUserExist(username)) // make sure user exists
  {
    std::cout << "That user does not exist!\n" << std::endl;
    return;
  }
  else
  {
    removeUser = username + ".txt";
    removeInbox = username + "-inbox.txt";
    std::remove(removeUser.c_str()); // cast users string to a c string to remove
    std::remove(removeInbox.c_str());
  }
}

std::string GetUserFile(std::string username)
{
  std::string usernameFile = "";

  std::transform(username.begin(), username.end(), username.begin(), ::tolower); // format username to lowercase

  usernameFile = username + ".txt"; // set name for users file

  return usernameFile; // return name for users file
}

std::string GetInboxFile(std::string username)
{
  std::string inboxFile = "";

  std::transform(username.begin(), username.end(), username.begin(), ::tolower); // format username to lowercase

  inboxFile = username + "-inbox.txt"; // set name for users inbox file

  return inboxFile; // return name for users inbox file
}

std::string GetLine(std::istream & sin)
{
  std::string nextLine;

  std::getline(sin, nextLine); // get the next line and return

  return nextLine;
}

bool DoesUserExist(std::string username)
{
  std::ifstream checkUser;

  checkUser.open(username + ".txt"); // attempt to open file with specified username

  return checkUser.good(); // return true or false if file has opened or not
}

int GetInboxCount(std::string username)
{
  int emailCount = 0;
  std::string email;
  std::ifstream fin;

  fin.open(username + "-inbox.txt"); // open users inbox

  while (std::getline(fin, email)) // run while getline returns new lines
  {
    if (email == "#email") // if line is preprocessor logic
    {
      ++emailCount; // increment email count 
    }
  }

  fin.close(); // close users inbox file

  return emailCount; // return number of emails
}

std::string FormatEmailString(std::string from, std::string to, std::string subject, std::string msg)
{
  std::string formattedEmail, emailHeader;
  emailHeader = "#email";

  formattedEmail = "\n" + emailHeader + "\n" + from + "\n" + to + "\n" + subject + '\n' + msg; // format for email in inbox

  return formattedEmail; // return the proper format
}

void SendEmail(std::string username)
{
  std::string sendTo, subject, message, formattedEmail;
  std::ofstream fout;

  std::cout << "Send email to who: ";
  std::cin >> sendTo; // get email recipient from user
  if (!DoesUserExist(sendTo)) // make sure that user exists
  {
    std::cout << "User does not exist!\n" << std::endl;
    return;
  }
  else
  {
    std::cout << "Subject: "; // get subject and message from user
    std::cin >> subject;
    std::cout << "Message: ";
    std::cin >> message;

    formattedEmail = FormatEmailString(username, sendTo, subject, message); // format the email

    fout.open(sendTo + "-inbox.txt", std::ofstream::app); // open recipients inbox and append to the end of the file
    fout << formattedEmail; // write formatted email to file
    fout.close(); // close inbox file
    std::cout << "\nEmail sent\n";
  }
}

std::string GetPassword(std::string username)
{
  std::string waste1, waste2, userPassword;
  std::ifstream fin;

  fin.open(username + ".txt"); // open users username and password file
  fin >> waste1 >> username >> waste2 >> userPassword; // scan for username and password saving them to variables

  return userPassword; // return the users password as saved in the file
}
