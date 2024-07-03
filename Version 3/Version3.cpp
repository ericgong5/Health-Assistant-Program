#include <string>
#include <iostream>
#include <limits>
#include <vector>
#include <fstream>
#include <filesystem>
#include <cmath>
#include <typeinfo>



using namespace std;

double getBMI(double weight, double height);
double getUSNavy(string gender,double waist,double neck,double height,double hip);
string getBMIGroup(double bmi);
string getUSNavyGroup(double USNavy,string gender,int age);

struct UserInfo {
    int age;
    int dailyCalories;
    double weight;
    double waist;
    double neck;
    double height;
    double hip;
    double carbs;
    double protein;
    double fat;
    double bodyFatPercentage;
    std::string name;
    std::string gender;
    std::string lifestyle;
    std::string bodyFatCategory;

    UserInfo(): age(0), weight(0), waist(0), neck(0), height(0), hip(0), carbs(0), protein(0), fat(0), name(""), gender(""),
                    lifestyle(""), dailyCalories(0), bodyFatPercentage(0), bodyFatCategory("")
    {}
};

class UserInfoManager
{
    public:
        UserInfoManager(); // constructor. initializes linked list
        ~UserInfoManager(); // destructor. delete allocated memory
        void addUserInfo(); // adds info to list
        void deleteUser(string username); // removes a user
        void readFromFile(string filename); // read and populate list
        void writeToFile(string filename);
        void display(string username);
        UserInfo* findUser(string username);
        vector<UserInfo*>* getMyList();
    private:
        vector<UserInfo*>* mylist; // pointer to vector of userInfo
};

UserInfoManager::UserInfoManager(){
    mylist = new vector<UserInfo*>();
}

UserInfoManager::~UserInfoManager(){
    UserInfo* temp;
    for (int i=0; i < mylist->size(); i++)
    {
       temp = mylist->at(i);
       delete temp;
    }
    delete mylist;
}

//adapted from a2
void UserInfoManager::addUserInfo(){
    int age;
    double weight;
    double waist;
    double neck;
    double height;
    double hip;
    double carbs;
    double protein;
    double fat;
    std::string name;
    std::string gender;
    std::string lifestyle;
    // get data from user and do input validation
    std::cout << "Please enter your name: " << std::endl;
    std::cin >> name;
    if (!std::cin){
        throw "Warning: Please enter a name string";
    }

    std::cout << "Please enter your gender: " << std::endl;
    std::cin >> gender;
    if ((gender != "male" && gender != "female") || !std::cin){
        throw "Warning: Please enter male or female as the gender (all lower case)";
    }
 
    std::cout << "Please enter your age: " << std::endl;
    std::cin >> age;
    if (age < 20 || age > 79 || !std::cin){
        throw "Warning: Please enter a valid age that is between 20 and 79";
    }

    std::cout << "Please enter your body weight in kg: " << std::endl;
    std::cin >> weight;
    if (weight <= 0 || !std::cin){
        throw "Warning: Please enter a valid positive integer body weight in kg greater than 0";
    }
    
    std::cout << "Please enter your waist measurement in cm: " << std::endl;
    std::cin >> waist;
    if (waist <= 0 || !std::cin){
        throw "Warning: Please enter a valid positive integer waist measurement in cm greater than 0";
    }
   
    std::cout << "Please enter your neck measurement in cm: " << std::endl;
    std::cin >> neck;
    if (neck <= 0 || !std::cin || waist - neck <= 0){
        throw "Warning: Please enter a valid positive integer neck measurement in cm that is smaller than your waist and greater than 0";
    }
 
    std::cout << "Please enter your height measurement in cm: " << std::endl;
    std::cin >> height;
    if (height <= 0 || !std::cin){
        throw "Warning: Please enter a valid positive integer height measurement in cm greater than 0";
    }

    std::cout << "Please provide information about your current lifestyle, either sedentary, moderate (moderately active) or active: " << std::endl;
    std::cin >> lifestyle;
    if ((lifestyle != "sedentary" && lifestyle != "moderate" && lifestyle != "active") || !std::cin){
        throw "Warning: Please enter a valid lifestyle, either sedentary, moderate or active (all lower case)";
    }

    if(gender == "female"){
        std::cout << "Please enter your hip measurement in cm: " << std::endl;
        std::cin >> hip;
        if (hip <= 0 || !std::cin || waist + hip - neck <= 0){
            throw "Warning: Please enter a valid positive integer hip measurement in cm greater than 0 and where waist + hip - neck <= 0";
        }
    }
    std::cout << "--------------------------------------------------------------" << std::endl;

    //create new userInfo and populate it
    UserInfo* newUser = new UserInfo();
    newUser->age = age;
    newUser->weight = weight;
    newUser->waist = waist;
    newUser->neck = neck;
    newUser->height = height;
    newUser->hip = hip;
    newUser->name = name;
    newUser->gender = gender;
    newUser->lifestyle = lifestyle;

    mylist->push_back(newUser);
}

void UserInfoManager::deleteUser(string username){
    UserInfo* user;
    for (int i=0; i < mylist->size(); i++)
    {
        user = mylist->at(i);
        if(user->name == username){
            mylist->erase(mylist->begin() + i);
            delete user;
        }
    }
}

// currently, hips is empty string if user is male, doesnt affect results now but could later
// check if spaces are valid characters (will ignore whole row if space is present)
//adapted from a1 but CSV format is now gender,age,weight,waist,neck,hip,height,lifestyle,carbs,protein,fat,name,dailyCalories,bodyFatPercentage,bodyFatCategory
void UserInfoManager::readFromFile(string filename){
    // throw exception if no file found
    std::ifstream csv_file;
    csv_file.open(filename, std::ios::in);
    if(!csv_file.is_open()){
        throw "Warning : CSV file " + filename + " failed to open, please make sure it exists and you have proper privileges!";
    }

    //delete old list and create a new one with the new info from file
    UserInfo* temp;
    for (int i=0; i < mylist->size(); i++)
    {
       temp = mylist->at(i);
       delete temp;
    }
    mylist->clear();
    std::vector<std::string> entry_row;
    std::string curent_line;
    //get line from file
    while (std::getline(csv_file, curent_line)){
        std::string word;
        std::string::iterator character;
        bool empty_line = false;
        //go character by character getting each token delimited by ,
        for (character = curent_line.begin(); character!=curent_line.end(); ++character){
            if(*character != ','){
                word.push_back(*character);
            }else if(*character == ' '){
                bool empty_line = true;
                throw "Warning : Unnecessary space character detected in CSV file " + filename + ". Try again with valid or non-empty CSV file!";
            }else{
                // add token to the user data vector
                entry_row.push_back(word);
                word = "";
            }
        }
        entry_row.push_back(word);
        //add row to the userInfo linked list
        if(!empty_line){
            UserInfo* newUser = new UserInfo();
            newUser->gender = entry_row.at(0);
            newUser->age = stoi(entry_row.at(1));
            newUser->weight = stod(entry_row.at(2));
            newUser->waist = stod(entry_row.at(3));
            newUser->neck = stod(entry_row.at(4));
            if(newUser->gender == "female"){
                newUser->hip =stod(entry_row.at(5));
            }
            newUser->height = stod(entry_row.at(6));
            newUser->lifestyle = entry_row.at(7);
            newUser->carbs = stod(entry_row.at(8));
            newUser->protein = stod(entry_row.at(9));
            newUser->fat = stod(entry_row.at(10));
            newUser->name = entry_row.at(11);
            newUser->dailyCalories = stoi(entry_row.at(12));
            newUser->bodyFatPercentage = stod(entry_row.at(13));
            newUser->bodyFatCategory = entry_row.at(14);

            mylist->push_back(newUser);
        }
        entry_row.clear();
    }
}

//adapted from a1 but CSV format is now gender,age,weight,waist,neck,hip,height,lifestyle,carbs,protein,fat,name,dailyCalories,bodyFatPercentage,bodyFatCategory
void UserInfoManager::writeToFile(string filename){
    //try to delete old file
    try {
        filesystem::remove(filename);
    }catch(const filesystem::filesystem_error& err) {
        
    }

    std::ofstream csv_file;
    csv_file.open(filename, std::ios::out | std::ios::app);
    UserInfo* current;

    for (int i=0; i < mylist->size(); i++)
    {
        UserInfo* current = mylist->at(i);
        std::string message = current->gender + "," + std::to_string(current->age) + "," + std::to_string(current->weight) + "," + std::to_string(current->waist) + "," 
                + std::to_string(current->neck) + ","+ std::to_string(current->hip) + "," + std::to_string(current->height) + "," + current->lifestyle + ","
                + std::to_string(current->carbs) + "," + std::to_string(current->protein) + "," + std::to_string(current->fat) + "," + current->name + ","
                + std::to_string(current->dailyCalories) + "," + std::to_string(current->bodyFatPercentage) + "," + current->bodyFatCategory + '\n';
        csv_file << message;
    }
    csv_file.close();
}

// won't display hip measurment if gender is male
void UserInfoManager::display(string username){
    for (int i=0; i < mylist->size(); i++)
    {
        UserInfo* current = mylist->at(i);
        if(current->name == username || username == "all"){
            //formating for display
            const char separator    = ' ';
            const int wordWidth     = 45;

            std::cout << std::left << std::setw(wordWidth) << std::setfill(separator) << "Name: " << current->name << std::endl;
            std::cout << std::left << std::setw(wordWidth) << std::setfill(separator) << "Gender: " << current->gender << std::endl;
            std::cout << std::left << std::setw(wordWidth) << std::setfill(separator) << "Age: " << current->age << std::endl;
            std::cout << std::left << std::setw(wordWidth) << std::setfill(separator) << "Weight in kg: " << current->weight << std::endl;
            std::cout << std::left << std::setw(wordWidth) << std::setfill(separator) << "Waist in cm: " << current->waist << std::endl;
            std::cout << std::left << std::setw(wordWidth) << std::setfill(separator) << "Neck in cm: " << current->neck << std::endl;
            std::cout << std::left << std::setw(wordWidth) << std::setfill(separator) << "Height in cm: " << current->height << std::endl;
            std::cout << std::left << std::setw(wordWidth) << std::setfill(separator) << "Lifestyle: " << current->lifestyle << std::endl;
            if(current->gender == "female"){
                std::cout << std::left << std::setw(wordWidth) << std::setfill(separator) << "hip in cm: " << current->hip << std::endl;
            }
            std::cout << std::left << std::setw(wordWidth) << std::setfill(separator) << "Body fat percentage: " << current->bodyFatPercentage << "%" << std::endl;
            std::cout << std::left << std::setw(wordWidth) << std::setfill(separator) << "Body fat percentage group: " << current->bodyFatCategory << std::endl;
            std::cout << std::left << std::setw(wordWidth) << std::setfill(separator) << "Daily calorie requirements: " << current->dailyCalories << std::endl;
            std::cout << std::left << std::setw(wordWidth) << std::setfill(separator) << "Daily carbohydrate requirements in grams: " << current->carbs << std::endl;
            std::cout << std::left << std::setw(wordWidth) << std::setfill(separator) << "Daily protein requirements in grams: " << current->protein << std::endl;
            std::cout << std::left << std::setw(wordWidth) << std::setfill(separator) << "Daily fat requirements in grams: " << current->fat << std::endl;
            std::cout << "--------------------------------------------------------------" << std::endl;
        }
    }
}

UserInfo* UserInfoManager::findUser(string username){
    UserInfo* current;
    for (int i=0; i < mylist->size(); i++)
    {
        current = mylist->at(i);
        if(current->name == username){
            return current;
        }
    }
    return nullptr;
}

vector<UserInfo*>* UserInfoManager::getMyList(){
    return mylist;
}

class HealthAssistant {
    public:
        void getUserDetail(); // wrapper method that simply calls addUserInfo in class UserInfoManager
        virtual void getBfp(string username) = 0;
        void getDailyCalories(string username);
        void getMealPrep(string username);
        void display(string username); //wrapper method
        void serialize(string filename); //wrapper method
        void readFromFile(string filename); //wrapper method
        void deleteUser(string username); // wrapper method
        void massLoadAndCompute(string filename);
        UserInfoManager getManager(); //getter for child classes to get the manager
        virtual void getBfp(UserInfo* user) = 0;  // used in mass load and compute
        void getDailyCalories(UserInfo* user);
        void getMealPrep(UserInfo* user);
    protected:
        UserInfoManager manager = UserInfoManager();
};

void HealthAssistant::getUserDetail(){
    manager.addUserInfo();
}


UserInfoManager HealthAssistant::getManager(){
    return manager;
}

void HealthAssistant::getDailyCalories(string username){
    UserInfo* current = manager.findUser(username);
    if(current == nullptr){
        throw "Warning : User " + username + " not found!";
    }

    if(current->lifestyle != "sedentary" && current->lifestyle != "moderate" && current->lifestyle != "active"){
        throw "Warning : Lifestyle has to be sedentary, moderate or active (all lower case), please try again!";
    }
    int daily_calories = 0;
    if(current->gender == "male"){
        if(current->age >= 19 && current->age <= 30){
            if(current->lifestyle == "sedentary"){
                daily_calories = 2400;
            }else if(current->lifestyle == "moderately active"){
                daily_calories = 2600;
            }else if(current->lifestyle == "active"){
                daily_calories = 3000;
            }
        }else if(current->age >= 31 && current->age <= 50){
            if(current->lifestyle == "sedentary"){
                daily_calories = 2200;
            }else if(current->lifestyle == "moderately active"){
                daily_calories = 2400;
            }else if(current->lifestyle == "active"){
                daily_calories = 2800;
            }
        }else if(current->age >= 51){
            if(current->lifestyle == "sedentary"){
                daily_calories = 2000;
            }else if(current->lifestyle == "moderately active"){
                daily_calories = 2200;
            }else if(current->lifestyle == "active"){
                daily_calories = 2400;
            }
        }
    }else if(current->gender == "female"){
        if(current->age >= 19 && current->age <= 30){
            if(current->lifestyle == "sedentary"){
                daily_calories = 2000;
            }else if(current->lifestyle == "moderately active"){
                daily_calories = 2000;
            }else if(current->lifestyle == "active"){
                daily_calories = 2400;
            }
        }else if(current->age >= 31 && current->age <= 50){
            if(current->lifestyle == "sedentary"){
                daily_calories = 1800;
            }else if(current->lifestyle == "moderately active"){
                daily_calories = 2000;
            }else if(current->lifestyle == "active"){
                daily_calories = 2200;
            }
        }else if(current->age >= 51){
            if(current->lifestyle == "sedentary"){
                daily_calories = 1600;
            }else if(current->lifestyle == "moderately active"){
                daily_calories = 1800;
            }else if(current->lifestyle == "active"){
                daily_calories = 2000;
            }
        }
    }
    current->dailyCalories = daily_calories;
}
void HealthAssistant::getMealPrep(string username){
    UserInfo* current = manager.findUser(username);
    if(current == nullptr){
        throw "Warning : User " + username + " not found!";
    }
    if(current->dailyCalories == 0){
        throw "Warning : User " + username + " hasn't calculated daily caloric needs yet! Use getDailyCalories() to do so!";
    }
    int calories_input = current->dailyCalories;
    double calories = (double) calories_input;

    // the calory densities.
    const double carbohydrate_calorie_per_gram = 4.0;
    const double protein_calorie_per_gram = 4.0;
    const double fats_calorie_per_gram = 9.0;

    //the right proportions of protein, fat, and carbohydrates. 
    double carbs_percent = 0.5;
    double protein_percent = 0.3;
    double fat_percent = 0.2;

    double carbs_output = calories * carbs_percent / carbohydrate_calorie_per_gram;
    double protein_output = calories * protein_percent / protein_calorie_per_gram;
    double fat_output = calories * fat_percent / fats_calorie_per_gram;

    current->carbs = carbs_output;
    current->protein = protein_output;
    current->fat = fat_output;
}
void HealthAssistant::display(string username){
    manager.display(username);
}
void HealthAssistant::serialize(string filename){
    manager.writeToFile(filename);
} 
void HealthAssistant::readFromFile(string filename){
    manager.readFromFile(filename);
}
void HealthAssistant::deleteUser(string username){
    manager.deleteUser(username);
}

void HealthAssistant::massLoadAndCompute(string filename){
    //delete old list and create a new one with the new info from file 
    std::ifstream csv_file; 
    csv_file.open(filename, std::ios::in);
    if(!csv_file.is_open()){
        throw "Warning : CSV file failed to open, make sure it exists!";
    }
    //delete old list and create a new one with the new info from file 
    UserInfo* current;
    vector<UserInfo*>* list = manager.getMyList();
    for (int i=0; i < list->size(); i++)
    {
       current = list->at(i);
       delete current;
    }
    list->clear();

    std::vector<std::string> entry_row;
    std::string curent_line;
    //get line from file
    while (std::getline(csv_file, curent_line)){
        std::string word;
        std::string::iterator character;
        bool empty_line = false;
        //go character by character getting each token delimited by ,
        for (character = curent_line.begin(); character!=curent_line.end(); ++character){
            if(*character != ','){
                word.push_back(*character);
            }else if(*character == ' '){
                empty_line = true;
                throw "Warning : Space character detected, please use a valid non-empty CSV file";
            }else{
                // add token to the user data vector
                entry_row.push_back(word);
                word = "";
            }
        }
        entry_row.push_back(word);
        //add row to the userInfo linked list
        if(!empty_line){
            UserInfo* newUser = new UserInfo;
            newUser->gender = entry_row.at(0);
            newUser->age = stoi(entry_row.at(1));
            newUser->weight = stod(entry_row.at(2));
            newUser->waist = stod(entry_row.at(3));
            newUser->neck = stod(entry_row.at(4));
            if(newUser->gender == "female"){
                newUser->hip = stod(entry_row.at(5));
            }
            newUser->height = stod(entry_row.at(6));
            newUser->lifestyle = entry_row.at(7);
            newUser->name = entry_row.at(11);
            getBfp(newUser);
            getDailyCalories(newUser);
            getMealPrep(newUser);

            list->push_back(newUser);
        }
        entry_row.clear();
    }
}

//overloaded for helper where we pass in the userInfo struct instead of a username
void HealthAssistant::getDailyCalories(UserInfo* user){
    UserInfo* current = user;
    if(current == nullptr){
        throw "Warning : User not found!";
    }

    if(current->lifestyle != "sedentary" && current->lifestyle != "moderate" && current->lifestyle != "active"){
        throw "Lifestyle has to be sedentary, moderate or active (all lower case), please try again!";
    }
    int daily_calories = 0;
    if(current->gender == "male"){
        if(current->age >= 19 && current->age <= 30){
            if(current->lifestyle == "sedentary"){
                daily_calories = 2400;
            }else if(current->lifestyle == "moderately active"){
                daily_calories = 2600;
            }else if(current->lifestyle == "active"){
                daily_calories = 3000;
            }
        }else if(current->age >= 31 && current->age <= 50){
            if(current->lifestyle == "sedentary"){
                daily_calories = 2200;
            }else if(current->lifestyle == "moderately active"){
                daily_calories = 2400;
            }else if(current->lifestyle == "active"){
                daily_calories = 2800;
            }
        }else if(current->age >= 51){
            if(current->lifestyle == "sedentary"){
                daily_calories = 2000;
            }else if(current->lifestyle == "moderately active"){
                daily_calories = 2200;
            }else if(current->lifestyle == "active"){
                daily_calories = 2400;
            }
        }
    }else if(current->gender == "female"){
        if(current->age >= 19 && current->age <= 30){
            if(current->lifestyle == "sedentary"){
                daily_calories = 2000;
            }else if(current->lifestyle == "moderately active"){
                daily_calories = 2000;
            }else if(current->lifestyle == "active"){
                daily_calories = 2400;
            }
        }else if(current->age >= 31 && current->age <= 50){
            if(current->lifestyle == "sedentary"){
                daily_calories = 1800;
            }else if(current->lifestyle == "moderately active"){
                daily_calories = 2000;
            }else if(current->lifestyle == "active"){
                daily_calories = 2200;
            }
        }else if(current->age >= 51){
            if(current->lifestyle == "sedentary"){
                daily_calories = 1600;
            }else if(current->lifestyle == "moderately active"){
                daily_calories = 1800;
            }else if(current->lifestyle == "active"){
                daily_calories = 2000;
            }
        }
    }
    current->dailyCalories = daily_calories;
}
//overloaded for helper where we pass in the userInfo struct instead of a username
void HealthAssistant::getMealPrep(UserInfo* user){
    UserInfo* current = user;
    if(current == nullptr){
        throw "Warning : User not found!";
    }
    if(current->dailyCalories == 0){
        throw "User " + current->name + " hasn't calculated daily caloric needs yet! Use getDailyCalories() to do so!";
    }
    int calories_input = current->dailyCalories;
    double calories = (double) calories_input;

    // the calory densities.
    const double carbohydrate_calorie_per_gram = 4.0;
    const double protein_calorie_per_gram = 4.0;
    const double fats_calorie_per_gram = 9.0;

    //the right proportions of protein, fat, and carbohydrates. 
    double carbs_percent = 0.5;
    double protein_percent = 0.3;
    double fat_percent = 0.2;

    double carbs_output = calories * carbs_percent / carbohydrate_calorie_per_gram;
    double protein_output = calories * protein_percent / protein_calorie_per_gram;
    double fat_output = calories * fat_percent / fats_calorie_per_gram;

    current->carbs = carbs_output;
    current->protein = protein_output;
    current->fat = fat_output;
}

class USNavyMethod: public HealthAssistant {
    public:
        void getBfp(string username);
        void getBfp(UserInfo* user); // helper function for mass load and compute
}; 

void USNavyMethod::getBfp(string username){
    UserInfo* current = manager.findUser(username);
    
    if(current == nullptr){
        std::cout << "User " + username + " not found!" << std::endl;
        return;
    }
    double body_fat_percentage;
    std::string body_fat_percentage_group;
    if(current->gender == "male"){
        body_fat_percentage = (495.0 / ( 1.0324 - (0.19077 * log10(current->waist - current->neck)) + (0.15456 * log10(current->height)))) - 450.0;
        if(current->age >= 20 && current->age <= 39){
            if(body_fat_percentage < 21){
                body_fat_percentage_group = "Low";
            }else if(body_fat_percentage >= 21.0 && body_fat_percentage <= 32.9){
                body_fat_percentage_group = "Normal";
            }else if(body_fat_percentage >= 33.0 && body_fat_percentage <= 38.9){
                body_fat_percentage_group = "High";
            }else if(body_fat_percentage >= 39){
                body_fat_percentage_group = "Very High";
            }
        }else if(current->age >= 40 && current->age <= 59){
            if(body_fat_percentage < 23){
                body_fat_percentage_group = "Low";
            }else if(body_fat_percentage >= 23.0 && body_fat_percentage <= 33.9){
                body_fat_percentage_group = "Normal";
            }else if(body_fat_percentage >= 34.0 && body_fat_percentage <= 39.9){
                body_fat_percentage_group = "High";
            }else if(body_fat_percentage >= 40){
                body_fat_percentage_group = "Very High";
            }
        }else if(current->age >= 60 && current->age <= 79){
            if(body_fat_percentage < 24){
                body_fat_percentage_group = "Low";
            }else if(body_fat_percentage >= 24.0 && body_fat_percentage <= 35.9){
                body_fat_percentage_group = "Normal";
            }else if(body_fat_percentage >= 36.0 && body_fat_percentage <= 31.9){
                body_fat_percentage_group = "High";
            }else if(body_fat_percentage >= 42){
                body_fat_percentage_group = "Very High";
            }
        }
    }else{
        body_fat_percentage = (495.0 / ( 1.29579 - (0.35004 * log10(current->waist + current->hip - current->neck)) + (0.22100 * log10(current->height)))) - 450.0;
        if(current->age >= 20 && current->age <= 39){
            if(body_fat_percentage < 8){
                body_fat_percentage_group = "Low";
            }else if(body_fat_percentage >= 8.0 && body_fat_percentage <= 19.9){
                body_fat_percentage_group = "Normal";
            }else if(body_fat_percentage >= 20.0 && body_fat_percentage <= 34.9){
                body_fat_percentage_group = "High";
            }else if(body_fat_percentage >= 25){
                body_fat_percentage_group = "Very High";
            }
        }else if(current->age >= 40 && current->age <= 59){
            if(body_fat_percentage < 11){
                body_fat_percentage_group = "Low";
            }else if(body_fat_percentage >= 11.0 && body_fat_percentage <= 21.9){
                body_fat_percentage_group = "Normal";
            }else if(body_fat_percentage >= 22.0 && body_fat_percentage <= 27.9){
                body_fat_percentage_group = "High";
            }else if(body_fat_percentage >= 28){
                body_fat_percentage_group = "Very High";
            }
        }else if(current->age >= 60 && current->age <= 79){
            if(body_fat_percentage < 13){
                body_fat_percentage_group = "Low";
            }else if(body_fat_percentage >= 13.0 && body_fat_percentage <= 24.9){
                body_fat_percentage_group = "Normal";
            }else if(body_fat_percentage >= 25.0 && body_fat_percentage <= 29.9){
                body_fat_percentage_group = "High";
            }else if(body_fat_percentage >= 30){
                body_fat_percentage_group = "Very High";
            }
        }
    }
    current->bodyFatPercentage = body_fat_percentage;
    current->bodyFatCategory = body_fat_percentage_group;
}

void USNavyMethod::getBfp(UserInfo* user){ 
    if(user == nullptr){
        std::cout << "User not found!" << std::endl;
        return;
    }
    double body_fat_percentage;
    std::string body_fat_percentage_group;
    if(user->gender == "male"){
        body_fat_percentage = (495.0 / ( 1.0324 - (0.19077 * log10(user->waist - user->neck)) + (0.15456 * log10(user->height)))) - 450.0;
        if(user->age >= 20 && user->age <= 39){
            if(body_fat_percentage < 21){
                body_fat_percentage_group = "Low";
            }else if(body_fat_percentage >= 21.0 && body_fat_percentage <= 32.9){
                body_fat_percentage_group = "Normal";
            }else if(body_fat_percentage >= 33.0 && body_fat_percentage <= 38.9){
                body_fat_percentage_group = "High";
            }else if(body_fat_percentage >= 39){
                body_fat_percentage_group = "Very High";
            }
        }else if(user->age >= 40 && user->age <= 59){
            if(body_fat_percentage < 23){
                body_fat_percentage_group = "Low";
            }else if(body_fat_percentage >= 23.0 && body_fat_percentage <= 33.9){
                body_fat_percentage_group = "Normal";
            }else if(body_fat_percentage >= 34.0 && body_fat_percentage <= 39.9){
                body_fat_percentage_group = "High";
            }else if(body_fat_percentage >= 40){
                body_fat_percentage_group = "Very High";
            }
        }else if(user->age >= 60 && user->age <= 79){
            if(body_fat_percentage < 24){
                body_fat_percentage_group = "Low";
            }else if(body_fat_percentage >= 24.0 && body_fat_percentage <= 35.9){
                body_fat_percentage_group = "Normal";
            }else if(body_fat_percentage >= 36.0 && body_fat_percentage <= 31.9){
                body_fat_percentage_group = "High";
            }else if(body_fat_percentage >= 42){
                body_fat_percentage_group = "Very High";
            }
        }
    }else{
        body_fat_percentage = (495.0 / ( 1.29579 - (0.35004 * log10(user->waist + user->hip - user->neck)) + (0.22100 * log10(user->height)))) - 450.0;
        if(user->age >= 20 && user->age <= 39){
            if(body_fat_percentage < 8){
                body_fat_percentage_group = "Low";
            }else if(body_fat_percentage >= 8.0 && body_fat_percentage <= 19.9){
                body_fat_percentage_group = "Normal";
            }else if(body_fat_percentage >= 20.0 && body_fat_percentage <= 34.9){
                body_fat_percentage_group = "High";
            }else if(body_fat_percentage >= 25){
                body_fat_percentage_group = "Very High";
            }
        }else if(user->age >= 40 && user->age <= 59){
            if(body_fat_percentage < 11){
                body_fat_percentage_group = "Low";
            }else if(body_fat_percentage >= 11.0 && body_fat_percentage <= 21.9){
                body_fat_percentage_group = "Normal";
            }else if(body_fat_percentage >= 22.0 && body_fat_percentage <= 27.9){
                body_fat_percentage_group = "High";
            }else if(body_fat_percentage >= 28){
                body_fat_percentage_group = "Very High";
            }
        }else if(user->age >= 60 && user->age <= 79){
            if(body_fat_percentage < 13){
                body_fat_percentage_group = "Low";
            }else if(body_fat_percentage >= 13.0 && body_fat_percentage <= 24.9){
                body_fat_percentage_group = "Normal";
            }else if(body_fat_percentage >= 25.0 && body_fat_percentage <= 29.9){
                body_fat_percentage_group = "High";
            }else if(body_fat_percentage >= 30){
                body_fat_percentage_group = "Very High";
            }
        }
    }
    user->bodyFatPercentage = body_fat_percentage;
    user->bodyFatCategory = body_fat_percentage_group;
}

class BmiMethod: public HealthAssistant {
    public:
        void getBfp(string username);
        void getBfp(UserInfo* user); // helper function for massload and compute
};  

void BmiMethod::getBfp(string username){
    UserInfo* current = manager.findUser(username);
    
    if(current == nullptr){
        std::cout << "User " + username + " not found!" << std::endl;
        return;
    }
    double body_fat_percentage = current->weight / pow(current->height/100.0, 2.0);
    if(body_fat_percentage <= 0){
        throw "Warning : Body fat percentage can't be 0, please recheck your users measurments!";
    }

    std::string body_fat_percentage_group;
    if(body_fat_percentage < 18.5){
        body_fat_percentage_group = "Low";
    }else if(body_fat_percentage >= 18.5  && body_fat_percentage <= 24.9){
        body_fat_percentage_group = "Normal";
    }else if(body_fat_percentage >= 25.0  && body_fat_percentage <= 29.9){
        body_fat_percentage_group = "High";
    }else if(body_fat_percentage >= 30.0){
        body_fat_percentage_group = "Very High";
    }
    current->bodyFatPercentage = body_fat_percentage;
    current->bodyFatCategory = body_fat_percentage_group;
}

void BmiMethod::getBfp(UserInfo* user){
    if(user == nullptr){
        std::cout << "User not found!" << std::endl;
        return;
    }
    double height_in_m = user->height / 100.0;
    double body_fat_percentage = user->weight / pow(height_in_m, 2.0);
    if(body_fat_percentage <= 0){
        throw "Warning : Body fat percentage can't be 0, please recheck your users measurments!";
    }
    std::string body_fat_percentage_group;
    if(body_fat_percentage < 18.5){
        body_fat_percentage_group = "Low";
    }else if(body_fat_percentage >= 18.5  && body_fat_percentage <= 24.9){
        body_fat_percentage_group = "Normal";
    }else if(body_fat_percentage >= 25.0  && body_fat_percentage <= 29.9){
        body_fat_percentage_group = "High";
    }else if(body_fat_percentage >= 30.0){
        body_fat_percentage_group = "Very High";
    }
    user->bodyFatPercentage = body_fat_percentage;
    user->bodyFatCategory = body_fat_percentage_group;
}

class UserStats{
    public:
        UserStats();
        ~UserStats();
        void GetHealthyUsers(string method, string gender);
        void GetUnfitUsers(string method, string gender);
        void GetFullStats();

    private:
        vector<string>* userNames;  // names of all users
        vector<string>* userGender; // gender of all users
        vector<string>* userBMI; // BMI group of all users
        vector<string>* userUSNavy;  // bodyfat group of all users    
};

UserStats::UserStats(){
    userNames = new vector<string>();
    userGender = new vector<string>();
    userBMI = new vector<string>();
    userUSNavy = new vector<string>();

    const char* filenames[2] = {"bmi_user_data.csv", "us_user_data.csv"};
    for(string name : filenames){
        std::ifstream csv_file;
        csv_file.open(name, std::ios::in);
        if(!csv_file.is_open()){
            throw "Warning : CSV file " + name + " failed to open, make sure it exists!";
        }
    
        std::vector<std::string> entry_row;
        std::string curent_line;
        //get line from file
        while (std::getline(csv_file, curent_line)){
            std::string word;
            std::string::iterator character;
            bool empty_line = false;
            //go character by character getting each token delimited by ,
            for (character = curent_line.begin(); character!=curent_line.end(); ++character){
                if(*character != ','){
                    word.push_back(*character);
                }else if(*character == ' '){
                    empty_line = true;
                    throw "Warning : Space character detected, please use a valid non-empty CSV file";
                }else{
                    // add token to the user data vector
                    entry_row.push_back(word);
                    word = "";
                }
            }
            entry_row.push_back(word);
            //add row to the userInfo linked list
            if(!empty_line){
                string gender = entry_row.at(0);
                int age = stoi(entry_row.at(1));
                double weight = stod(entry_row.at(2));
                double waist = stod(entry_row.at(3));
                double neck = stod(entry_row.at(4));
                double hip = stod(entry_row.at(5));
                double height = stod(entry_row.at(6));
                string name = entry_row.at(11);

                double bmi = getBMI(weight,height);
                double USNavy = getUSNavy(gender,waist,neck,height,hip);   // check again with hip if its empty

                string bmiGroup = getBMIGroup(bmi);
                string USNavyGroup = getUSNavyGroup(USNavy,gender,age);

                userNames->push_back(name);
                userGender->push_back(gender);
                userBMI->push_back(bmiGroup);
                userUSNavy->push_back(USNavyGroup);
            }
            entry_row.clear();
        }
    }
}

UserStats::~UserStats(){
    userNames->clear();
    userGender->clear();
    userBMI->clear();
    userUSNavy->clear();

    delete userNames;
    delete userGender;
    delete userBMI;
    delete userUSNavy;
}

// helper function to get bmi
double getBMI(double weight, double height){
    double height_in_m = height / 100.0;
    return weight / pow(height_in_m, 2.0);
}
// helper function to get USNavy fat percentage
double getUSNavy(string gender, double waist, double neck, double height, double hip){
    if(gender == "male"){
        return (495.0 / ( 1.0324 - (0.19077 * log10(waist - neck)) + (0.15456 * log10(height)))) - 450.0;
    }else{
        return (495.0 / ( 1.29579 - (0.35004 * log10(waist + hip - neck)) + (0.22100 * log10(height)))) - 450.0; 
    }
}
// helper function to get bmi group
string getBMIGroup(double bmi){
    std::string body_fat_percentage_group;
    if(bmi < 18.5){
        body_fat_percentage_group = "Low";
    }else if(bmi >= 18.5  && bmi <= 24.9){
        body_fat_percentage_group = "Normal";
    }else if(bmi >= 25.0  && bmi <= 29.9){
        body_fat_percentage_group = "High";
    }else if(bmi >= 30.0){
        body_fat_percentage_group = "Very High";
    }
    return body_fat_percentage_group;
}
// helper function to get Us Navy fat group
string getUSNavyGroup(double body_fat, string gender, int age){
    string body_fat_percentage_group;
    if(gender == "male"){
        if(age >= 20 && age <= 39){
            if(body_fat < 21){
                body_fat_percentage_group = "Low";
            }else if(body_fat >= 21.0 && body_fat <= 32.9){
                body_fat_percentage_group = "Normal";
            }else if(body_fat >= 33.0 && body_fat <= 38.9){
                body_fat_percentage_group = "High";
            }else if(body_fat >= 39){
                body_fat_percentage_group = "Very High";
            }
        }else if(age >= 40 && age <= 59){
            if(body_fat < 23){
                body_fat_percentage_group = "Low";
            }else if(body_fat >= 23.0 && body_fat <= 33.9){
                body_fat_percentage_group = "Normal";
            }else if(body_fat >= 34.0 && body_fat <= 39.9){
                body_fat_percentage_group = "High";
            }else if(body_fat >= 40){
                body_fat_percentage_group = "Very High";
            }
        }else if(age >= 60 && age <= 79){
            if(body_fat < 24){
                body_fat_percentage_group = "Low";
            }else if(body_fat >= 24.0 && body_fat <= 35.9){
                body_fat_percentage_group = "Normal";
            }else if(body_fat >= 36.0 && body_fat <= 31.9){
                body_fat_percentage_group = "High";
            }else if(body_fat >= 42){
                body_fat_percentage_group = "Very High";
            }
        }
    }else{
        if(age >= 20 && age <= 39){
            if(body_fat < 8){
                body_fat_percentage_group = "Low";
            }else if(body_fat >= 8.0 && body_fat <= 19.9){
                body_fat_percentage_group = "Normal";
            }else if(body_fat >= 20.0 && body_fat <= 34.9){
                body_fat_percentage_group = "High";
            }else if(body_fat >= 25){
                body_fat_percentage_group = "Very High";
            }
        }else if(age >= 40 && age <= 59){
            if(body_fat < 11){
                body_fat_percentage_group = "Low";
            }else if(body_fat >= 11.0 && body_fat <= 21.9){
                body_fat_percentage_group = "Normal";
            }else if(body_fat >= 22.0 && body_fat <= 27.9){
                body_fat_percentage_group = "High";
            }else if(body_fat >= 28){
                body_fat_percentage_group = "Very High";
            }
        }else if(age >= 60 && age <= 79){
            if(body_fat < 13){
                body_fat_percentage_group = "Low";
            }else if(body_fat >= 13.0 && body_fat <= 24.9){
                body_fat_percentage_group = "Normal";
            }else if(body_fat >= 25.0 && body_fat <= 29.9){
                body_fat_percentage_group = "High";
            }else if(body_fat >= 30){
                body_fat_percentage_group = "Very High";
            }
        }
    }
    return body_fat_percentage_group;
}
// assumes that if gender is not male or female, it displays all
void UserStats::GetHealthyUsers(string method, string gender = "all"){
    cout << "----------------------------------" << endl;
    for(int i = 0; i < userNames->size(); i++){
        string user_gender = this->userGender->at(i);
        if(method == "USArmy" && this->userUSNavy->at(i) == "Normal"){
            if(gender == user_gender){
                cout << this->userNames->at(i) << endl;
            }else if(gender != "male" && gender != "female"){
                cout << this->userNames->at(i) << endl;
            }
        }else if(method == "bmi" && this->userBMI->at(i) == "Normal"){
            if(gender == user_gender){
                cout << this->userNames->at(i) << endl;
            }else if(gender != "male" && gender != "female"){
                cout << this->userNames->at(i) << endl;
            }
        }else if(method == "all" && this->userUSNavy->at(i) == "Normal" && this->userBMI->at(i) == "Normal"){
            if(gender == user_gender){
                cout << this->userNames->at(i) << endl;
            }else if(gender != "male" && gender != "female"){
                cout << this->userNames->at(i) << endl;
            }
        }
    }
}
// assumes that if gender is not male or female, it displays all
void UserStats::GetUnfitUsers(string method, string gender = "all"){
    cout << "----------------------------------" << endl;
    for(int i = 0; i < userNames->size(); i++){
        string user_gender = this->userGender->at(i);
        if(method == "USArmy" && (this->userUSNavy->at(i) == "High" || this->userUSNavy->at(i) == "Very High")){
            if(gender == user_gender){
                cout << this->userNames->at(i) << endl;
            }else if(gender != "male" && gender != "female"){
                cout << this->userNames->at(i) << endl;
            }
        }else if(method == "bmi" && (this->userBMI->at(i) == "High" || this->userBMI->at(i) == "Very High")){
            if(gender == user_gender){
                cout << this->userNames->at(i) << endl;
            }else if(gender != "male" && gender != "female"){
                cout << this->userNames->at(i) << endl;
            }
        }else if
        (method == "all" && 
            (this->userUSNavy->at(i) == "High" || this->userUSNavy->at(i) == "Very High") && 
            (this->userBMI->at(i) == "High" || this->userBMI->at(i) == "Very High")){
            if(gender == user_gender){
                cout << this->userNames->at(i) << endl;
            }else if(gender != "male" && gender != "female"){
                cout << this->userNames->at(i) << endl;
            }
        }
    }
}

void UserStats::GetFullStats(){
    double total_users = userNames->size();
    double males = 0.0;
    double healthyBMI = 0.0;
    double healthyUSNavy = 0.0;
    double healthyBMIMales = 0.0;
    double healthyBMIFemales = 0.0;
    double healthyUSNavyMales = 0.0;
    double healthyUSNavyFemales = 0.0;
    for(int i=0;i<userNames->size();i++){
        if(userGender->at(i) == "male"){
            males++;
            if(userBMI->at(i) == "Normal"){
                healthyBMI++;
                healthyBMIMales++;
            }
            if(userUSNavy->at(i) == "Normal"){
                healthyUSNavy++;
                healthyUSNavyMales++;
            }
        }else{
            if(userBMI->at(i) == "Normal"){
                healthyBMI++;
                healthyBMIFemales++;
            }
            if(userUSNavy->at(i) == "Normal"){
                healthyUSNavy++;
                healthyUSNavyFemales++;
            }
        }
    }

    cout << "----------------------------------" << endl;
    cout << "Total number of users: " + to_string(total_users) << endl;
    cout << "Percentage of male users is " + to_string(100.0 * males/total_users) + '%' 
        + " and Percentage of female users is " + to_string(100.0 * (1.0 - (males/total_users))) + '%' << endl;
    cout << "Percentage of healthy users according to bmi method: " + to_string(100.0 * healthyBMI/total_users) + '%' << endl;
    cout << "Percentage of healthy users according to USArmy method: " + to_string(100.0 * healthyUSNavy/total_users) + '%' << endl;
    cout << "Percentage of healthy females and healthy males for bmi method: " << endl;

    cout << "Male: " +  to_string(100.0 * healthyBMIMales/total_users) + '%' << endl;
    cout << "Female: " +  to_string(100.0 * healthyBMIFemales/total_users) + '%' << endl;
    cout << "Percentage of healthy females and healthy males for USArmy method: " << endl;
    cout << "Male: " +  to_string(100.0 * healthyUSNavyMales/total_users) + '%' << endl;
    cout << "Female: " +  to_string(100.0 * healthyUSNavyFemales/total_users) + '%' << endl;
}

int main(int argc, char* argv[]) {
   

    
    UserStats stat;
    stat.GetHealthyUsers("bmi", "female");
    stat.GetHealthyUsers("all");
    stat.GetUnfitUsers("USArmy", "male");
    stat.GetFullStats();



}
