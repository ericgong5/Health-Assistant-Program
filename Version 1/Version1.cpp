#include <iostream>
#include <string>
#include <ios>
#include <cmath>
#include <utility>
#include <limits>
#include <iomanip>
#include <fstream>
#include <vector>
#include <sstream>

//user measurments
std::string gender = "none";
int age = -1;
double weight = -1;
double waist = -1;
double neck = -1;
double height = -1;
std::string lifestyle = "none";
double hip = -1;

// user body fat percentage
std::pair<int, std::string> body_fat_percentage_and_group (-1,"none") ;

// user daily calorie requirements
int daily_calorie_requirements = -1;

// user macro-nutriment requirements
double carbs_requirements = -1;
double protein_requirements = -1;
double fat_requirements = -1;

// probably not necessary but Q7 said to store each row? so its here
std::vector<std::vector<std::string>> user_info_history;


void getUserDetails();
std::pair<int, std::string> get_bfp(double waist, double neck, double height, double hip, std::string gender, int age);
int get_daily_calories(double age, std::string gender, std::string lifestyle);
void meal_prep(int calories_input, double& carbs_output, double& protein_output, double& fat_output);
void display();
bool verify_all_parameters();   // helper function to ensure initialization before calling display
void serialize(std::string filename);
bool check_if_file_exists(std::string filename);  // helper function to check for file existance
void create_and_write(std::string filename, std::string message, std::string current_hip);  // helper function for creating and writing to files 
void readFromFile(std::string filename);


int main(int argc, char* argv[]) {
// Check if a filename is provided as an argument
    if (argc > 1) {
    // Load existing user data from the specified CSV file
        readFromFile(argv[1]);
    } else {
        // Gather user details
        getUserDetails();
    
        // Calculate body fat percentage
        auto bfpResult = get_bfp(waist, neck, height, hip,
        gender, age);
        std::cout << "Body Fat Percentage: " << bfpResult.first << "% (" << bfpResult.second << std::endl;
        // Calculate daily calorie intake
        int dailyCalories = get_daily_calories(age, gender,
        lifestyle);
        std::cout << "Daily Caloric Intake: " << dailyCalories << "calories" << std::endl;
        // Calculate macronutrient breakdown
        double carbs, protein, fat;
        meal_prep(dailyCalories, carbs, protein, fat);
        std::cout << "Macronutrient Breakdown:\n";
        std::cout << "Carbs: " << carbs << "g, Protein: " << protein << "g,Fat: " << fat << "g" << std::endl;
        // Display user information
        display();
        // Save user data to a CSV file
        serialize("user_data.csv");
    }
    return 0;
}

void getUserDetails(){

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
}

std::pair<int, std::string> get_bfp(double waist, double neck, double height, double hip, std::string gender, int age){
    double body_fat_percentage;
    std::string body_fat_percentage_group;
    if(gender == "male"){
        body_fat_percentage = (495.0 / ( 1.0324 - (0.19077 * log10(waist - neck)) + (0.15456 * log10(height)))) - 450.0;
        if(age >= 20 && age <= 39){
            if(body_fat_percentage < 21){
                body_fat_percentage_group = "Low";
            }else if(body_fat_percentage >= 21.0 && body_fat_percentage <= 32.9){
                body_fat_percentage_group = "Normal";
            }else if(body_fat_percentage >= 33.0 && body_fat_percentage <= 38.9){
                body_fat_percentage_group = "High";
            }else if(body_fat_percentage >= 39){
                body_fat_percentage_group = "Very High";
            }
        }else if(age >= 40 && age <= 59){
            if(body_fat_percentage < 23){
                body_fat_percentage_group = "Low";
            }else if(body_fat_percentage >= 23.0 && body_fat_percentage <= 33.9){
                body_fat_percentage_group = "Normal";
            }else if(body_fat_percentage >= 34.0 && body_fat_percentage <= 39.9){
                body_fat_percentage_group = "High";
            }else if(body_fat_percentage >= 40){
                body_fat_percentage_group = "Very High";
            }
        }else if(age >= 60 && age <= 79){
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
        body_fat_percentage = (495.0 / ( 1.29579 - (0.35004 * log10(waist + hip - neck)) + (0.22100 * log10(height)))) - 450.0;
        if(age >= 20 && age <= 39){
            if(body_fat_percentage < 8){
                body_fat_percentage_group = "Low";
            }else if(body_fat_percentage >= 8.0 && body_fat_percentage <= 19.9){
                body_fat_percentage_group = "Normal";
            }else if(body_fat_percentage >= 20.0 && body_fat_percentage <= 34.9){
                body_fat_percentage_group = "High";
            }else if(body_fat_percentage >= 25){
                body_fat_percentage_group = "Very High";
            }
        }else if(age >= 40 && age <= 59){
            if(body_fat_percentage < 11){
                body_fat_percentage_group = "Low";
            }else if(body_fat_percentage >= 11.0 && body_fat_percentage <= 21.9){
                body_fat_percentage_group = "Normal";
            }else if(body_fat_percentage >= 22.0 && body_fat_percentage <= 27.9){
                body_fat_percentage_group = "High";
            }else if(body_fat_percentage >= 28){
                body_fat_percentage_group = "Very High";
            }
        }else if(age >= 60 && age <= 79){
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

    int body_fat_percentage_int = (int) body_fat_percentage;

    std::pair<int, std::string> answer (body_fat_percentage_int,body_fat_percentage_group);

    //update global parameter
    body_fat_percentage_and_group = answer;


    return answer;
}

int get_daily_calories(double age, std::string gender, std::string lifestyle){
    //check to make sure inputs are valid   return with -1 if not valid
    if(gender != "male" && gender != "female"){
        std::cout << "Gender has to be male or female (all lower case), please try again!" << std::endl;
        return -1;
    }
    if(lifestyle != "sedentary" && lifestyle != "moderate" && lifestyle != "active"){
        std::cout << "Lifestyle has to be sedentary, moderate or active (all lower case), please try again!" << std::endl;
        return -1;
    }
    if(age < 19){
        std::cout << "Age has to be greater than 18, please try again!" << std::endl;
        return -1;
    }

    int daily_calories = 0;
    if(gender == "male"){
        if(age >= 19 && age <= 30){
            if(lifestyle == "sedentary"){
                daily_calories = 2400;
            }else if(lifestyle == "moderately active"){
                daily_calories = 2600;
            }else if(lifestyle == "active"){
                daily_calories = 3000;
            }
        }else if(age >= 31 && age <= 50){
            if(lifestyle == "sedentary"){
                daily_calories = 2200;
            }else if(lifestyle == "moderately active"){
                daily_calories = 2400;
            }else if(lifestyle == "active"){
                daily_calories = 2800;
            }
        }else if(age >= 51){
            if(lifestyle == "sedentary"){
                daily_calories = 2000;
            }else if(lifestyle == "moderately active"){
                daily_calories = 2200;
            }else if(lifestyle == "active"){
                daily_calories = 2400;
            }
        }
    }else if(gender == "female"){
        if(age >= 19 && age <= 30){
            if(lifestyle == "sedentary"){
                daily_calories = 2000;
            }else if(lifestyle == "moderately active"){
                daily_calories = 2000;
            }else if(lifestyle == "active"){
                daily_calories = 2400;
            }
        }else if(age >= 31 && age <= 50){
            if(lifestyle == "sedentary"){
                daily_calories = 1800;
            }else if(lifestyle == "moderately active"){
                daily_calories = 2000;
            }else if(lifestyle == "active"){
                daily_calories = 2200;
            }
        }else if(age >= 51){
            if(lifestyle == "sedentary"){
                daily_calories = 1600;
            }else if(lifestyle == "moderately active"){
                daily_calories = 1800;
            }else if(lifestyle == "active"){
                daily_calories = 2000;
            }
        }
    }
    //update the global parameter
    daily_calorie_requirements = daily_calories;
    return daily_calories;
}

void meal_prep(int calories_input, double& carbs_output, double& protein_output, double& fat_output){
    // check to ensure positive calorie input
    if(calories_input<0){
        std::cout << "Please enter a positive calory input!" << std::endl;
    }
    double calories = (double) calories_input;

    // the calory densities.
    const double carbohydrate_calorie_per_gram = 4.0;
    const double protein_calorie_per_gram = 4.0;
    const double fats_calorie_per_gram = 9.0;

    //the right proportions of protein, fat, and carbohydrates. 
    double carbs_percent = 0.5;
    double protein_percent = 0.3;
    double fat_percent = 0.2;

    carbs_output = calories * carbs_percent / carbohydrate_calorie_per_gram;
    protein_output = calories * protein_percent / protein_calorie_per_gram;
    fat_output = calories * fat_percent / fats_calorie_per_gram;
}


void display(){
    //check to see if users has initialized the appropriate variables
    bool parameters_initialized = verify_all_parameters();

    //formating for display
    const char separator    = ' ';
    const int wordWidth     = 45;

    std::cout << std::left << std::setw(wordWidth) << std::setfill(separator) << "Gender: " << gender << std::endl;
    std::cout << std::left << std::setw(wordWidth) << std::setfill(separator) << "Age: " << age << std::endl;
    std::cout << std::left << std::setw(wordWidth) << std::setfill(separator) << "Weight in kg: " << weight << std::endl;
    std::cout << std::left << std::setw(wordWidth) << std::setfill(separator) << "Waist in cm: " << waist << std::endl;
    std::cout << std::left << std::setw(wordWidth) << std::setfill(separator) << "Neck in cm: " << neck << std::endl;
    std::cout << std::left << std::setw(wordWidth) << std::setfill(separator) << "Lifestyle: " << lifestyle << std::endl;
    if(gender == "female"){
        std::cout << std::left << std::setw(wordWidth) << std::setfill(separator) << "hip in cm: " << hip << std::endl;
    }
    std::cout << std::left << std::setw(wordWidth) << std::setfill(separator) << "Body fat percentage: " << body_fat_percentage_and_group.first << "%" << std::endl;
    std::cout << std::left << std::setw(wordWidth) << std::setfill(separator) << "Body fat percentage group: " << body_fat_percentage_and_group.second << std::endl;
    std::cout << std::left << std::setw(wordWidth) << std::setfill(separator) << "Daily calorie requirements: " << daily_calorie_requirements << std::endl;
    std::cout << std::left << std::setw(wordWidth) << std::setfill(separator) << "Daily carbohydrate requirements in grams: " << carbs_requirements << std::endl;
    std::cout << std::left << std::setw(wordWidth) << std::setfill(separator) << "Daily protein requirements in grams: " << protein_requirements << std::endl;
    std::cout << std::left << std::setw(wordWidth) << std::setfill(separator) << "Daily fat requirements in grams: " << fat_requirements << std::endl;
}

// verifies that all parameters for display have been initialized once by user.
bool verify_all_parameters(){
    if(gender != "male" && gender != "female"){
        std::cout << "Gender has to be male or female (all lower case), please try again!" << std::endl;
        return false;
    }
    if(lifestyle != "sedentary" && lifestyle != "moderate" && lifestyle != "active"){
        std::cout << "Lifestyle has to be sedentary, moderate or active (all lower case), please try again!" << std::endl;
        return false;
    }
    if(age < 19 || age >= 79){
        std::cout << "Age has to be between 19 and 79 inclusively, please try again!" << std::endl;
        return false;
    }
    if(weight < 0){
        std::cout << "Body weight has to be a valid positive integer in kg, please try again!" << std::endl;
        return false;
    }
    if(waist < 0){
        std::cout << "waist measurement has to be a valid positive integer in cm, please try again!" << std::endl;
        return false;
    }
    if(neck < 0){
        std::cout << "Neck measurement has to be a valid positive integer in cm, please try again!" << std::endl;
        return false;
    }
    if(height < 0){
        std::cout << "Height measurement has to be a valid positive integer in cm, please try again!" << std::endl;
        return false;
    }
    if(hip < 0 && gender == "female"){
        std::cout << "Hip measurement has to be a valid positive integer in cm, please try again!" << std::endl;
        return false;
    }
    if(hip < 0 && gender == "female"){
        std::cout << "Hip measurement has to be a valid positive integer in cm, please try again!" << std::endl;
        return false;
    }
    if(body_fat_percentage_and_group.first == -1){
        std::cout << "Body fat percentage and group hasn't been calculated yet, please use get_bfp() to get one!" << std::endl;
        return false;
    }
    if(carbs_requirements < 0){
        std::cout << "Daily carbohydrate needs hasn't been calculated yet, please use meal_prep() to get one!" << std::endl;
        return false;
    }
    if(protein_requirements < 0){
        std::cout << "Daily protein needs hasn't been calculated yet, please use meal_prep() to get one!" << std::endl;
        return false;
    }
    if(fat_requirements < 0){
        std::cout << "Daily fat needs hasn't been calculated yet, please use meal_prep() to get one!" << std::endl;
        return false;
    }
    return true;
}

void serialize(std::string filename){
    //get the appropriate hip according to user info
    std::string current_hip;
    if(gender == "male"){
        current_hip = "";
    }else{
        current_hip = std::to_string((int)hip);
    }
    //check if the file with filename exists
    bool file_exists = check_if_file_exists(filename);

    //create file if it doesn't exist
    if(!file_exists){
        std::string message = gender + "," + std::to_string(age) + "," + std::to_string((int)weight) + "," + std::to_string((int)waist) + "," + std::to_string((int)neck) + "," + current_hip + "," + std::to_string((int)height) + "," + lifestyle;
        create_and_write(filename, message, current_hip);
    }else{
        std::ofstream my_file;
        my_file.open(filename, std::ios::out | std::ios::app);
        std::string message = "\n" + gender + "," + std::to_string(age) + "," + std::to_string((int)weight) + "," + std::to_string((int)waist) + "," + std::to_string((int)neck) + "," + current_hip + "," + std::to_string((int)height) + "," + lifestyle;
        my_file << message;
        my_file.close();
    }
}

//create a new file and write to it
void create_and_write(std::string filename, std::string message, std::string current_hip){
    std::ofstream csv_file;
    csv_file.open(filename, std::ios::out | std::ios::app);
    csv_file << message;
    csv_file.close();
}

// helper function to check if file is openable
bool check_if_file_exists(std::string filename){
    std::ifstream csv_file;
    csv_file.open(filename);
    bool file_open = csv_file.is_open();
    csv_file.close();

    return file_open;
}

// currently, hips is empty string if user is male, doesnt affect results now but could later
// check if spaces are valid characters
void readFromFile(std::string filename){

    std::vector<std::vector<std::string>> entries;
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
        //add row to the history of user datas if line wasnt empty
        if(!empty_line){
            entries.push_back(entry_row);
        }
        entry_row.clear();



    }
    user_info_history = entries;

    // get row of used data to load user data with
    entry_row = user_info_history.back();

    //load user data into current data
    gender = entry_row.at(0);
    age = stoi(entry_row.at(1));
    weight = stod(entry_row.at(2));
    waist = stod(entry_row.at(3));
    neck = stod(entry_row.at(4));
    // check if user is female for hip
    if(gender == "female"){
        hip = stod(entry_row.at(5));
    }
    height = stod(entry_row.at(6));
    lifestyle = entry_row.at(7);
}
