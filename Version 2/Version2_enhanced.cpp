#include <string>
#include <iostream>
#include <limits>
#include <vector>
#include <fstream>
#include <filesystem>
#include <cmath>

using namespace std;

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
    UserInfo* next;

    UserInfo(): age(0), weight(0), waist(0), neck(0), height(0), hip(0), carbs(0), protein(0), fat(0), name(""), gender(""),
                    lifestyle(""), dailyCalories(0), bodyFatPercentage(0), bodyFatCategory(""), next(nullptr) 
    {}
};



class UserInfoManager
{
    public:
        static UserInfoManager& getInstance(){ 
            static UserInfoManager userInfoManager;
            return userInfoManager;
        }        
        
        ~UserInfoManager(); // destructor. delete allocated memory
        void addUserInfo(); // adds info to list
        void deleteUser(string username); // removes a user
        void readFromFile(string filename); // read and populate list
        void writeToFile(string filename);
        void display(string username);
        UserInfo* findUser(string username);
        UserInfo* getHead(); //enhanced specific function to get access to usefInfo
        void setHead(UserInfo* user); //sets the head to a user
    private:
        UserInfoManager();//userInfoManager = this;}   // constructor. initializes linked list
        UserInfoManager(const UserInfoManager&) = delete;
        UserInfoManager& operator=(const UserInfoManager&) = delete;
        static UserInfoManager* userInfoManager;
        UserInfo* mylist; // pointer to first element in linked list
};

UserInfoManager::UserInfoManager(){
    mylist = nullptr;
}

UserInfoManager::~UserInfoManager(){
    UserInfo* temp = mylist;
    while (temp != nullptr)
    {
       temp = mylist;
       mylist = mylist->next;
       delete temp;
    }
}

//adapted from a1 
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
    while(!std::cin){
        std::cout << "Please enter a name string: " << std::endl;
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cin >> name;
    }

    std::cout << "Please enter your gender: " << std::endl;
    std::cin >> gender;
    while((gender != "male" && gender != "female") || !std::cin){
        std::cout << "Please enter male or female as the gender (all lower case): " << std::endl;
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cin >> gender;
    }

    std::cout << "Please enter your age: " << std::endl;
    std::cin >> age;
    while(age < 20 || age > 79 || !std::cin){
        std::cout << "Please enter a valid age that is between 20 and 79: " << std::endl;
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cin >> age;
    }

    std::cout << "Please enter your body weight in kg: " << std::endl;
    std::cin >> weight;
    while(weight < 0 || !std::cin){
        std::cout << "Please enter a valid positive integer body weight in kg: " << std::endl;
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cin >> weight;
    }

    std::cout << "Please enter your waist measurement in cm: " << std::endl;
    std::cin >> waist;
    while(waist < 0 || !std::cin){
        std::cout << "Please enter a valid positive integer waist measurement in cm: " << std::endl;
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cin >> waist;
    }

    std::cout << "Please enter your neck measurement in cm: " << std::endl;
    std::cin >> neck;
    while(neck < 0 || !std::cin || waist - neck <= 0){
        std::cout << "Please enter a valid positive integer neck measurement in cm that is smaller than your waist: " << std::endl;
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cin >> neck;
    }

    std::cout << "Please enter your height measurement in cm: " << std::endl;
    std::cin >> height;
    while(height < 0 || !std::cin){
        std::cout << "Please enter a valid positive integer height measurement in cm: " << std::endl;
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cin >> height;
    }

    std::cout << "Please provide information about your current lifestyle, either sedentary, moderate (moderately active) or active: " << std::endl;
    std::cin >> lifestyle;
    while((lifestyle != "sedentary" && lifestyle != "moderate" && lifestyle != "active") || !std::cin){
        std::cout << "Please enter a valid lifestyle, either sedentary, moderate or active (all lower case): " << std::endl;
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cin >> lifestyle;
    }

    if(gender == "female"){
        std::cout << "Please enter your hip measurement in cm: " << std::endl;
        std::cin >> hip;
        while(hip < 0 || !std::cin){
            std::cout << "Please enter a valid positive integer hip measurement in cm: " << std::endl;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cin >> hip;
        }
    }
    std::cout << "--------------------------------------------------------------" << std::endl;

    //create new userInfo and populate it
    UserInfo* newUser = new UserInfo;
    newUser->age = age;
    newUser->weight = weight;
    newUser->waist = waist;
    newUser->neck = neck;
    newUser->height = height;
    newUser->hip = hip;
    newUser->name = name;
    newUser->gender = gender;
    newUser->lifestyle = lifestyle;

    newUser->next = mylist;
    mylist = newUser;
    
}

void UserInfoManager::deleteUser(string username){
    UserInfo* current = mylist;
    UserInfo* previous = mylist;
    while (current != nullptr)
    {
        if(current->name == username){
            previous->next = current->next;
            delete current;
            return;
        }
        previous = current;
        current = current->next;
    }
}

// currently, hips is empty string if user is male, doesnt affect results now but could later
// check if spaces are valid characters (will ignore whole row if space is present)
//adapted from a1 but CSV format is now gender,age,weight,waist,neck,hip,height,lifestyle,carbs,protein,fat,name,dailyCalories,bodyFatPercentage,bodyFatCategory
void UserInfoManager::readFromFile(string filename){
    //delete old list and create a new one with the new info from file
    UserInfo* temp = mylist;
    while (temp != nullptr)
    {
       temp = mylist;
       mylist = mylist->next;
       delete temp;
    }
    mylist = nullptr;
   
    //std::vector<std::vector<std::string>> entries;
    std::vector<std::string> entry_row;

    std::ifstream csv_file;
    csv_file.open(filename, std::ios::in);
    if(!csv_file.is_open()){
        std::cout << "CSV file failed to open, please try again!" << std::endl;
    }
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

            newUser->next = mylist;
            mylist = newUser;
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
        //do nothing
    }

    std::ofstream csv_file;
    csv_file.open(filename, std::ios::out | std::ios::app);
    UserInfo* current = mylist;

    while(current != nullptr){
        std::string message = current->gender + "," + std::to_string(current->age) + "," + std::to_string(current->weight) + "," + std::to_string(current->waist) + "," 
                + std::to_string(current->neck) + ","+ std::to_string(current->hip) + "," + std::to_string(current->height) + "," + current->lifestyle + ","
                + std::to_string(current->carbs) + "," + std::to_string(current->protein) + "," + std::to_string(current->fat) + "," + current->name + ","
                + std::to_string(current->dailyCalories) + "," + std::to_string(current->bodyFatPercentage) + "," + current->bodyFatCategory + '\n';
        csv_file << message;
        current = current->next;
    }
    csv_file.close();
}

// won't display hip measurment if gender is male
void UserInfoManager::display(string username){
    UserInfo* current = mylist;
    while (current != nullptr){
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
        current = current->next;
    }
}

UserInfo* UserInfoManager::findUser(string username){
    UserInfo* current = mylist;
    while (current != nullptr){
        if(current->name == username){
            return current;
        }
        current = current->next;
    }
    return nullptr;
}

UserInfo* UserInfoManager::getHead(){
    UserInfo* current = mylist;
    return current;
}

void UserInfoManager::setHead(UserInfo* user){
    user->next = mylist;
    mylist = user;
}

class HealthAssistant {
    public:
        HealthAssistant() : manager(UserInfoManager::getInstance()) {}
        void getUserDetail(); // wrapper method that simply calls addUserInfo in class UserInfoManager
        void getBfp(string username);
        void getDailyCalories(string username);
        void getMealPrep(string username);
        void display(string username); //wrapper method
        void serialize(string filename); //wrapper method
        void readFromFile(string filename); //wrapper method
        void deleteUser(string username); // wrapper method
        void massLoadAndCompute(string filename); //will load all user data from the specified file, populate the linked list and directly compute the health metrics
    private:
        UserInfoManager& manager;
        void getBfp(UserInfo* user);
        void getDailyCalories(UserInfo* user);
        void getMealPrep(UserInfo* user);
};

void HealthAssistant::getUserDetail(){
    manager.addUserInfo();
}

void HealthAssistant::getBfp(string username){
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

void HealthAssistant::getDailyCalories(string username){
    UserInfo* current = manager.findUser(username);
    if(current == nullptr){
        std::cout << "User " + username + " not found!" << std::endl;
        return;
    }

    if(current->lifestyle != "sedentary" && current->lifestyle != "moderate" && current->lifestyle != "active"){
        std::cout << "Lifestyle has to be sedentary, moderate or active (all lower case), please try again!" << std::endl;
        return;
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
        std::cout << "User " + username + " not found!" << std::endl;
        return;
    }
    if(current->dailyCalories == 0){
        std::cout << "User " + username + " hasn't calculated daily caloric needs yet! Use getDailyCalories() to do so!" << std::endl;
        return;
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
    UserInfo* current = manager.getHead();
    UserInfo* temp;
    while (current != nullptr)
    {
       temp = current;
       current = current->next;
       delete temp;
    }
    current = nullptr;

    //std::vector<std::vector<std::string>> entries;
    std::vector<std::string> entry_row;

    std::ifstream csv_file;
    csv_file.open(filename, std::ios::in);
    if(!csv_file.is_open()){
        std::cout << "CSV file failed to open, please try again!" << std::endl;
    }
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
                newUser->hip =stod(entry_row.at(5));
            }
            newUser->height = stod(entry_row.at(6));
            newUser->lifestyle = entry_row.at(7);
            newUser->name = entry_row.at(11);
            getBfp(newUser);
            getDailyCalories(newUser);
            getMealPrep(newUser);

            newUser->next = manager.getHead();
            manager.setHead(newUser);
        }
        entry_row.clear();
    }
}

//overloaded for helper where we pass in the userInfo struct instead of a username
void HealthAssistant::getBfp(UserInfo* user){
    UserInfo* current = user;
    
    if(current == nullptr){
        std::cout << "User " + current->name + " not found!" << std::endl;
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
//overloaded for helper where we pass in the userInfo struct instead of a username
void HealthAssistant::getDailyCalories(UserInfo* user){
    UserInfo* current = user;
    if(current == nullptr){
        std::cout << "User " + current->name + " not found!" << std::endl;
        return;
    }

    if(current->lifestyle != "sedentary" && current->lifestyle != "moderate" && current->lifestyle != "active"){
        std::cout << "Lifestyle has to be sedentary, moderate or active (all lower case), please try again!" << std::endl;
        return;
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
        std::cout << "User " + current->name + " not found!" << std::endl;
        return;
    }
    if(current->dailyCalories == 0){
        std::cout << "User " + current->name + " hasn't calculated daily caloric needs yet! Use getDailyCalories() to do so!" << std::endl;
        return;
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


int main(int argc, char* argv[]) {
    /*
    HealthAssistant ha;
    // get user details from console input for 1st user
    ha.getUserDetail();
    // get user details from console input for 2nd user
    ha.getUserDetail();
    // display information for john (assuming that john is the name of the first user)
    ha.display("john");
    // display information for all the users
    ha.display("all");
    // get bfp for the 1st user
    ha.getBfp("john");
    ha.getDailyCalories("john");
    ha.getMealPrep("john");
    // write all the data for all the users to file
    ha.serialize("user_data.csv");
    // Now let's get the data from file using a new instance of HealthAssistant 
    HealthAssistant ha2;
    ha2.readFromFile("user_data.csv");
    ha2.display("all");
    ha2.deleteUser("jack"); // assuming 2nd user is jack
    ha2.display("all"); // should only display for john at this point
    // now use the first object to display all
    ha.display("all"); // both john and jack should be present

    */
   HealthAssistant ha;
   ha.getUserDetail();
   ha.getUserDetail();
   ha.display("all");
   HealthAssistant ha2;
   ha2.deleteUser("jack");
   ha2.display("all");
   ha.display("all");


}
