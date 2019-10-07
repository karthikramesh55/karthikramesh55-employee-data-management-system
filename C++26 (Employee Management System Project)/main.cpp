#include <iostream>
#include <iomanip>  //To set the precision of spaces between strings (e.g. titles or columns)
#include <fstream>  //To do file handling operations such as reading from and writing onto a file (e.g. a database file)
#include <sstream> //To use istringstream and ostringstream for string parsing
#include <string>  //To do string handling operations
#include <vector> //To store data such as objects in a vector container (storing employee objects in the vector container)

using namespace std;

//Creating an employee record structure with 6 data members
struct employeeData {
    int employeeId;
    string employeeName;
    double basicSalary;
    double providentFund;
    double healthInsurance;
    string emailId;
};

//Function prototypes for proper functioning of the data management system
void readFile(vector <employeeData> &v);
void printMenu();
void addEmployee(vector <employeeData> &v);
int searchForEmployee(vector <employeeData> &v, int targetEmpId);
void printSingleEmployeeRecord(employeeData emp);
double getNetIncome(employeeData emp);
void printEmployeeRecordSheet(vector <employeeData> &v);
bool deleteAnEmployee(vector <employeeData> &v, int targetEmpId);
void saveEmployeeRecordSheet(vector <employeeData> v); //The parameter is passed by value because, we are not updating anything in driver function


//Function that gets a vector container as parameter, opens and reads from database file, traverse each line until end of file is encountered,
//parse each record token (which are separated by ",") in the file for string operations and also store employee objects in the vector container
void readFile(vector <employeeData> &v) { //Using &v, the reference method to reflect any changes made to vector onto the vTemp, the parent vector
    ifstream fileRead("emp.txt"); //Creating an ifstream object "fileRead" to connect the database file and fetch records from the file as a bridge
    if(!fileRead) {//If the file path is wrong, the error message is displayed
        cout<<"Error: Unable to open the file for further operations, so please check the file path"<<endl;
        exit(1);
    }
    else {
        string line; //A variable used to hold each line in the file
        int recordNumber = 0; //To count the number of records (number of lines) in the file
        while(!fileRead.eof()) { //Keeps traversing the file, until it encounters the end of the file (eof)
            std::getline(fileRead, line); //Fetches string data line by line
            recordNumber++; //Counts the number of records (number of lines) encountered in the file
            //Now, creating an istringstream object "iss" that takes each record (line) as input and string parse each token in the record
            istringstream iss (line);
            //Creating variables to hold the various tokens of record
            string sempId;
            string sempName;
            string sbasicSalary;
            string sprovidentFund;
            string shealthInsurance;
            string semailId;
            //Use the getline() method to feed each token onto their above respective variables with the field separator as ","
            std::getline(iss, sempId, ',');
            std::getline(iss, sempName, ',');
            std::getline(iss, sbasicSalary, ',');
            std::getline(iss, sprovidentFund, ',');
            std::getline(iss, shealthInsurance, ',');
            std::getline(iss, semailId, ',');
            //Now that we have fed each token into their respective variables as strings, we need to convert them into their corresponding data types
            //So, sempId to integer, sbasicSalary and sprovidentFund and shealthInsurance as double while sempName and semailId remain as strings
            employeeData emp; //Creating employee object to store all the above tokens in their respective data members, after data type conversion
            emp.employeeId = atoi(sempId.c_str()); //emp.employeeId is an integer. Convert first as legacy c string and use atoi() for conversion
            emp.employeeName = sempName;
            emp.basicSalary = atof(sbasicSalary.c_str()); //emp.basicSalary is a double. Convert first as legacy c string and then use atof() method
            emp.providentFund = atof(sprovidentFund.c_str());
            emp.healthInsurance = atof(shealthInsurance.c_str());
            emp.emailId = semailId;
            //Now, the "emp" object (employee object) is fed to the vector container
            v.push_back(emp);
        }
    }
    fileRead.close(); //Close the ifstream bridge
}

//Function to print the menu for the data management system
void printMenu() {
    cout<<"1. Add an employee record"<<endl;
    cout<<"2. Print employee record sheet"<<endl;
    cout<<"3. Search for an employee record"<<endl;
    cout<<"4. Delete an employee record"<<endl;
    cout<<"5. Save employee record sheet"<<endl;
    cout<<"6. Exit system"<<endl;
}

//Function to add an employee record
void addEmployee(vector <employeeData> &v) {
    //Making a search to check if the employee ID exists already in the database file
    //Create a temporary employee object for operations in this block
    employeeData emp;
    bool hasDuplicate = false;
    do {
        cout<<"Enter the employee ID: ";
        cin>>emp.employeeId;
        cout<<endl;
        if(searchForEmployee(v, emp.employeeId) != -1) {
            hasDuplicate = true;
            cout<<"The employee ID: "<<emp.employeeId<<" already exists in the database system. Please feed a different employee ID."<<endl<<endl;
        }
    } while(hasDuplicate); //Loop is valid until there is a duplicate employee record in the database system. Otherwise, control moves below

    cout<<"Enter the name of the employee: ";
    cin.clear(); //Make sure to clear the cin buffer since it can lead to unwanted information typed previously using cin to mix up
    cin.ignore(INT_MAX, '\n'); //The cin buffer must ignore the line breaks and return key (enter key) pressed previously
    std::getline(cin, emp.employeeName);
    cout<<endl;
    cout<<"Enter the basic salary of the employee: ";
    cin>>emp.basicSalary;
    cout<<endl;
    cout<<"Enter the provident fund deduction amount of the employee: ";
    cin>>emp.providentFund;
    cout<<endl;
    cout<<"Enter the health insurance deduction amount of the employee: ";
    cin>>emp.healthInsurance;
    cout<<endl;
    cout<<"Enter the email ID of the employee: ";
    cin.clear(); //Make sure to clear the cin buffer since it can lead to unwanted information typed previously using cin to mix up
    cin.ignore(INT_MAX, '\n'); //The cin buffer must ignore the line breaks and return key (enter key) pressed previously
    std::getline(cin, emp.emailId);
    //Feed the above details currently stored in the employee object "emp" into the vector container, which also reflects back into the driver vector
    v.push_back(emp);
    cout<<endl;
    cout<<"A new employee record with ID "<<emp.employeeId<<" has been added successfully"<<endl;
    cout<<"Total number of employees so far: "<<v.size()<<endl;
}

//Function to print out a single employee record (single line) and serves well as a helper function for the printEmployeeRecordSheet() function
void printSingleEmployeeRecord(employeeData emp) {
    cout<<setw(8)<<left<<emp.employeeId
        <<setw(25)<<left<<emp.employeeName
        <<setw(25)<<left<<emp.emailId
        <<setw(10)<<setprecision(2)<<right<<fixed<<emp.basicSalary
        <<setw(10)<<setprecision(2)<<right<<fixed<<emp.providentFund
        <<setw(10)<<setprecision(2)<<right<<fixed<<emp.healthInsurance
        <<setw(10)<<setprecision(2)<<right<<fixed<<getNetIncome(emp)<<endl;
}

//Function to print the existing employee record sheet in a good UI
void printEmployeeRecordSheet(vector <employeeData> &v) {
    //At the end, we need to calculate the total sums of basic salary, provident fund, insurance and net income values of all employee records
    //Declaring the required accumulating variables for holding final total sums
    double totalBasicSalary = 0.0;
    double totalProvFund = 0.0;
    double totalInsurance = 0.0;
    double totalNetIncome = 0.0;
    //ID: 8 spaces, Name: 20 spaces, Email ID: 25 spaces, BasicSalary, ProvidentFund, HealthInsurance, NetIncome: each 10 spaces (right aligned)
    cout<<endl;
    cout<<setw(8)<<left<<"EmpID"
        <<setw(25)<<left<<"Name"
        <<setw(25)<<left<<"EmailID"
        <<setw(10)<<right<<"Basic($)"
        <<setw(10)<<right<<"PF($)"
        <<setw(10)<<right<<"Insurance($)"
        <<setw(10)<<right<<"NetInc($)"<<endl;
    for(int i = 0; i < 100; i++) {
        cout<<"-";
    }
    cout<<endl;
    //Displaying each employee record line by line
    //First calculate the total sums of basicSalary, providentFund and healthInsurance to get the net income using getNetIncome() function
    for(vector <employeeData>::iterator vItr = v.begin(); vItr != v.end(); vItr++) {
        printSingleEmployeeRecord(*vItr);
        totalBasicSalary = totalBasicSalary + vItr->basicSalary; //Or use totalBasicSalary = totalBasicSalary + (*vItr).basicSalary;
        totalProvFund = totalProvFund + vItr->providentFund; //Or use totalProvFund = totalProvFund + (*vItr).providentFund;
        totalInsurance = totalInsurance + vItr->healthInsurance; //Or use totalInsurance = totalInsurance + (*vItr).healthInsurance;
        totalNetIncome = totalNetIncome + getNetIncome(*vItr); //Calling the getNetIncome function to compute the final income after deductions
    }
    for(int i = 0; i < 100; i++) {
        cout<<"-";
    }
    cout<<endl;
    cout<<setw(8)<<left<<"Total($)"
        <<setw(25)<<left<<" "
        <<setw(25)<<left<<" "
        <<setw(10)<<setprecision(2)<<right<<fixed<<totalBasicSalary
        <<setw(10)<<setprecision(2)<<right<<fixed<<totalProvFund
        <<setw(10)<<setprecision(2)<<right<<fixed<<totalInsurance
        <<setw(10)<<setprecision(2)<<right<<fixed<<totalNetIncome<<endl;
    cout<<endl;
}

//Function to calculate the net income for an employee record
double getNetIncome(employeeData emp) {
    return emp.basicSalary - (emp.providentFund + emp.healthInsurance);
}

//Function to search for an employee record duplicates
int searchForEmployee(vector <employeeData> &v, int targetEmpId) {
    //Performing a linear search to search an employee record ID in the vector container that matches with the target employee ID
    for(int i = 0; i < v.size(); i++) {
        if(v[i].employeeId == targetEmpId) {
            return i;
        }
    }
    return -1;
}

//Function to delete an employee
bool deleteAnEmployee(vector <employeeData> &v, int targetEmpId) {
    int index;
    index = searchForEmployee(v, targetEmpId);
    if(index == -1) {
        return false;
    }
    //If not, a valid employee record has been found available with an index
    cout<<"Your target employee record with ID "<<targetEmpId<<" has been found. Ready for deletion"<<endl<<endl;
    cout<<setw(8)<<left<<"EmpID"
        <<setw(25)<<left<<"Name"
        <<setw(25)<<left<<"EmailID"
        <<setw(10)<<right<<"Basic($)"
        <<setw(10)<<right<<"PF($)"
        <<setw(10)<<right<<"Insurance($)"
        <<setw(10)<<right<<"NetInc($)"<<endl;
    for(int i = 0; i < 100; i++) {
        cout<<"-";
    }
    cout<<endl;
    printSingleEmployeeRecord(v[index]);
    cout<<endl<<endl;
    //Confirming deletion from the end user
    cout<<"Press Y to activate deletion and N for non-deletion: ";
    char delChar;
    cin>>delChar;
    if(delChar == 'Y' || delChar == 'y') {
        v.erase(v.begin()+index); //The iterator now points to the vector element (employee object) that needs to be deleted
        return true;
    }
    else {
        return false;
    }
}

//Function to save the existing employee record sheet
void saveEmployeeRecordSheet(vector <employeeData> v) {
    ofstream fileWrite ("emp.txt"); //Creating an ofstream object "fileWrite" to connect with the database file and write the records onto the file
    if(!fileWrite) {
        cout<<"Error: Unable to find the file"<<endl;
        return;
    }
    int recCount = 0; //A counter variable to check if the number of employee records match the size of the vector container
    //Creating a vector iterator to write the records into the file using the "fileWrite" ofstream object
    for(vector <employeeData>::iterator vItr = v.begin(); vItr != v.end(); vItr++) {
        fileWrite<<vItr->employeeId<<","<<vItr->employeeName<<","<<vItr->basicSalary<<","
            <<vItr->providentFund<<","<<vItr->healthInsurance<<","<<vItr->emailId;
        recCount++;
        if(recCount != v.size()) { //Give a new line until the number of employee records matches with the size of the vector
            fileWrite<<endl;
        }
    }
    cout<<"There are "<<recCount<<" employee records in the database system so far"<<endl;
    fileWrite.close(); //Close the ofstream bridge
}

//Function to perform all the tasks
void doTask(vector <employeeData> &v, int choice) {
    switch(choice) {
        //Adding an employee record
        case 1:
            cout<<"You have chosen to add an employee record"<<endl<<endl;
            addEmployee(v);
            break;
        //Printing the employee record sheet
        case 2:
            cout<<"You have chosen to print the employee record sheet"<<endl<<endl;
            printEmployeeRecordSheet(v);
            break;
        //Searching for an employee record
        case 3:
            int targetEmployeeId;
            int index;
            cout<<"You have decided to search for an employee record"<<endl<<endl;
            cout<<"Enter the employee ID to start the search: "<<endl;
            cin>>targetEmployeeId;
            index = searchForEmployee(v, targetEmployeeId);
            if(index == -1) {
                cout<<"Employee ID "<<targetEmployeeId<<" does not exist"<<endl;
            }
            else {
                cout<<endl;
                cout<<setw(8)<<left<<"EmpID"
                    <<setw(25)<<left<<"Name"
                    <<setw(25)<<left<<"EmailID"
                    <<setw(10)<<right<<"Basic($)"
                    <<setw(10)<<right<<"PF($)"
                    <<setw(10)<<right<<"Insurance($)"
                    <<setw(10)<<right<<"NetInc($)"<<endl;
                for(int i = 0; i < 100; i++) {
                    cout<<"-";
                }
                cout<<endl;
                printSingleEmployeeRecord(v[index]);
                cout<<endl;
            }
            break;
        //Deleting an employee record
        case 4:
            int targetEmpId;
            bool deleteStatus;
            cout<<"You have decided to delete an employee record"<<endl<<endl;
            cout<<"Enter the target employee ID that you want to delete: ";
            cin>>targetEmpId;
            deleteStatus = deleteAnEmployee(v, targetEmpId);
            if(deleteStatus == true) {
                cout<<"The employee record has been deleted successfully"<<endl;
            }
            else if(deleteStatus == false) {
                cout<<"The employee record has not been deleted as per your wish";
                cout<<endl;
            }
            break;
        //Saving the existing employee record sheet
        case 5:
            cout<<"You have saved the existing employee record sheet in the database system successfully"<<endl<<endl;
            saveEmployeeRecordSheet(v);
            break;
        //Notification message for invalid input
        default:
            cout<<"Invalid choice. Please refer the choice menu and give a valid input from 1 to 6";
    }
}

//Driver function
int main() {
    vector <employeeData> vEmp;
    readFile(vEmp);
    cout<<"Welcome to the Fusion Inc Employee Management System"<<endl;
    for(int i = 0; i < 52; i++) {
        cout<<'-';
    }
    cout<<endl;
    printMenu();
    cout<<endl;
    bool exitSystem = false;
    //vEmp.size() gives the count of employee records that have been read from the file and fed to the vector container, by far
    cout<<vEmp.size()<<" employee records are present in the database system"<<endl<<endl;
    while(!exitSystem) { // !false = true
        cout<<"What do you want to do?"<<endl<<endl;
        cout<<"Input your option: ";
        int choice;
        cin>>choice;
        cout<<endl<<endl;
        if(choice == 6) {
            exitSystem = true; //The user gets logged out of the employee management system
        }
        else {
            doTask(vEmp, choice); //The vector container and the choice number is passed as arguments to the doTask function
        }

    }
    return EXIT_SUCCESS;
}
