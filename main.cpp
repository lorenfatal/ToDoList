#include <iostream>     // Input & Output Stream library
#include <vector>       // Vector library - dynamic arrays
#include <string>       // String library - manipulating strings
#include <fstream>      // File Stream library - reading and writing files
#include <limits>       // Use std::numeric_limits and std::streamsize

// using declarations
using std::cout;
using std::cin;
using std::string;
using std::vector;
using std::getline;
using std::ofstream;
using std::cerr;
using std::ifstream;
using std::stoi;

/**
 * @brief Structure to represent a task in the To-Do List Manager.
 */
struct Task {
    int id;
    string description;
    bool completed;
};

// Functions declarations
void addTask(vector<Task> &tasks, int &nextId);
void viewTasks(const vector<Task> &tasks);
void markTaskCompleted(vector<Task> &tasks);
void deleteTask(vector<Task> &tasks);
void saveTasks(const vector<Task> &tasks, const string &filename);
void loadTasks(vector<Task> &tasks, int &nextId, const string &filename);

int main() {
    vector<Task> tasks; // Vector that stores the tasks
    int nextId = 1; // Initializes the next task ID
    const string filename = "../data/tasks.txt"; // Path of file which holds tasks

    loadTasks(tasks, nextId, filename); // Load existing tasks from file to Tasks vector

    int choice; // User's menu choice
    do {
        cout << "\n--- To-Do List Manager ---\n";
        cout << "1. Add Task\n";
        cout << "2. View Tasks\n";
        cout << "3. Mark Task as Completed\n";
        cout << "4. Delete Task\n";
        cout << "5. Save & Exit\n";
        cout << "Choose an option: ";
        cin >> choice;

        // Clear input buffer to handle any leftover characters
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        switch(choice) {
            case 1:
                addTask(tasks, nextId);
                break;
            case 2:
                viewTasks(tasks);
                break;
            case 3:
                markTaskCompleted(tasks);
                break;
            case 4:
                deleteTask(tasks);
                break;
            case 5:
                saveTasks(tasks, filename);
                cout << "Tasks saved. Exiting...\n";
                break;
            default:
                cout << "Invalid choice. Try again.\n";
        }
    } while(choice != 5); // Continue until user chooses to save and exit

    return 0;
}

/**
 * @brief Adds a new task to the tasks vector.
 * @param tasks Reference to tasks vector.
 * @param nextId Reference to the next available task ID.
 */
void addTask(vector<Task> &tasks, int &nextId) {
    Task newTask; // Create a new Task instance
    newTask.id = nextId++; // Assign a unique ID and increment nextId
    cout << "Enter task description: ";
    getline(cin, newTask.description); // Reads first line of text from user input and stores it in description
    newTask.completed = false; // Initialize task as not completed
    tasks.push_back(newTask); // Add the new task to the tasks vector
    cout << "Task added.\n";
}

/**
 * @brief Displays all tasks in the tasks vector.
 * @param tasks Constant reference to the vector of tasks.
 * The constant reference to tasks vector is preventing the function from modifying it.
 */
void viewTasks(const vector<Task> &tasks) {
    if(tasks.empty()) { // Check if there are any tasks to display
        cout << "No tasks to display.\n";
        return; // Notify user and exit function if no tasks exist
    }
    cout << "\n--- Current Tasks ---\n";
    for(const auto &task : tasks) { // Iterate over each task in tasks vector
                                    // 'task' is a constant reference, preveting it from modifying the original tasks vector
                                    // 'auto &' recognizes the type of task
        cout << task.id << ". [" 
                  << (task.completed ? "X" : " ") // Display 'X' if completed, else space
                  << "] " << task.description << "\n";
    }
}

/**
 * @brief Marks a specific task as completed based on user input.
 * @param tasks Reference to tasks vector.
 */
void markTaskCompleted(vector<Task> &tasks) {
    int id;
    cout << "Enter task ID to mark as completed: ";
    cin >> id; // Task ID input by the user
    for(auto &task : tasks) { // Iterate over each task in tasks vector
                              // 'task' is a reference, allowing modification of task elements
                              // 'auto &' recognizes the type of task
        if(task.id == id) { // Check if current task ID matches the user-input ID = task has been found
            task.completed = true; // Mark task as completed
            cout << "Task marked as completed.\n";
            return; // Exit function after marking
        }
    }
    cout << "Task ID not found.\n"; // Inform user if task ID not found
}

/**
 * @brief Deletes a specific task from the tasks vector based on user input.
 * @param tasks Reference to tasks vector.
 */
void deleteTask(vector<Task> &tasks) {
    int id;
    cout << "Enter task ID to delete: ";
    cin >> id; // Task ID input by the user
    for(auto it = tasks.begin(); it != tasks.end(); ++it) { // Iterate over each task in tasks vector using iterator 'it'
        if(it->id == id) { // Check if current task ID matches the user-input ID = task has been found
            tasks.erase(it); // Remove task from tasks vector
            cout << "Task deleted.\n";
            return; // Exit function after deletion
        }
    }
    cout << "Task ID not found.\n"; // Inform user if ID not found
}

/**
 * @brief Saves all tasks to a specified file.
 * @param tasks Constant reference to tasks vector.
 * @param filename Constant reference to the filename where tasks will be saved.
 */
void saveTasks(const vector<Task> &tasks, const string &filename) {
    ofstream outFile(filename); // Open file for writing - by creating output file stream named outFile which opens filename
    if(!outFile) { // Check if file opened successfully
        cerr << "Error opening file for writing.\n";
        return;
    }
    for(const auto &task : tasks) { // Iterate over each task in tasks vector
                                    // 'task' is a constant reference, preveting it from modifying the original tasks vector
                                    // 'auto &' recognizes the type of task
        outFile << task.id << "|" << task.completed << "|" << task.description << "\n"; // Write task to file in format (id|completed|description)
    }
    outFile.close(); // Close the file
}

/**
 * @brief Loads tasks from a specified file into the tasks vector.
 *        Also updates the nextId based on the highest existing task ID.
 * @param tasks Reference to the vector of tasks.
 * @param nextId Reference to the next available task ID.
 * @param filename Constant reference to the filename from which tasks will be loaded.
 */
void loadTasks(vector<Task> &tasks, int &nextId, const string &filename) {
    ifstream inFile(filename); // Open file for reading - by creating input file stream named inFile which opens filename
    if(!inFile) { // Check if file opened successfully
        return; // File doesn't exist, start fresh
    }

    string line; // Store each line read from the file

    while(getline(inFile, line)) { // Read file line by line - getline reads characters from inFile into line until a newline character is encountered
        Task task; // Stores data from current line
        size_t pos1 = line.find('|'); // Find first delimiter
        size_t pos2 = line.find('|', pos1 + 1); // Find second delimiter (after pos1)

        if(pos1 == string::npos || pos2 == string::npos) // Check if one of the '|' is missing - meaning the expected format isn't met and the line should be skipped (string::npos signifies that the substring wasn't found)
            continue; 
        
        task.id = stoi(line.substr(0, pos1)); // Extract substring up to first '|' and convert string to integer
        task.completed = (line.substr(pos1 + 1, pos2 - pos1 - 1) == "1"); // Extract substring after first '|' and before second '|', then convert completion status
        task.description = line.substr(pos2 + 1); // Extract substring after second '|' 
        tasks.push_back(task); // Add the task to the tasks vector

        if(task.id >= nextId) // Ensures that nextId is always greater than the highest existing task ID
            nextId = task.id + 1; // Update nextId if necessary
    }
    inFile.close(); // Close the file
}

// Don't forget to compile: g++ -o src/main.exe main.cpp
// Then run the file: src\main.exe