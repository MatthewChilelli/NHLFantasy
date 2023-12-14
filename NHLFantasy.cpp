// Disclaimer, even though I'm not a comment heavy programmer, this will have to
// be a comment heavy application because i am using complex libraries liek curl
// and nlohmann Im new to these so i need to see what they to so i can refrence
// this for future projects.

#include <curl/curl.h>
#include <iostream>
#include <nlohmann/json.hpp>
#include <vector>

// Define a struct to represent a player to get the stuff i need out of the api
struct Player {
    std::string firstName;
    std::string lastName;
    int points;
};

// Function declarations
void printMenu();
void getUpdatedList(std::vector<Player>& playerList);
void showFantasyPoints(const std::vector<Player>& playerList);
void shootoutGame(std::vector<Player>& playerList);
void manuallyAdd(std::vector<Player>& playerList);
void manuallyDelete(std::vector<Player>& playerList);

size_t CurlWriteCallback(void* contents, size_t size, size_t nmemb,
    std::string* output);  
// a callback function used with
// libcurl - looked up online

int main() {
    std::vector<Player> playerList;

    int choice;
    do {
        printMenu();
        std::cout << "Enter your choice (1-6): ";
        std::cin >> choice;

        switch (choice) {
        case 1:
            getUpdatedList(playerList);
            break;
        case 2:
            showFantasyPoints(playerList);
            break;
        case 3:
            shootoutGame(playerList);
            break;
        case 4:
            manuallyAdd(playerList);
            break;
        case 5:
            manuallyDelete(playerList);
            break;
        case 6:
            std::cout << "Closing program.\n";
            break;
        default:
            std::cout << "Invalid choice. Please enter a number between 1 and 6.\n";
        }

    } while (choice != 6);

    return 0;
}
// Menu
void printMenu() {
    std::cout << "1. Get Updated List\n";
    std::cout << "2. Show Fantasy Points\n";
    std::cout << "3. Shootout Game\n";
    std::cout << "4. Manually Add\n";
    std::cout << "5. Manually Delete\n";
    std::cout << "6. Close Program\n";
}

// Callback function for libcurl to handle the received data during an HTTP
// request
size_t CurlWriteCallback(void* contents, size_t size, size_t nmemb,
    std::string* output) {
    size_t total_size = size * nmemb;
    output->append(static_cast<char*>(contents), total_size);
    return total_size;
}

void getUpdatedList(std::vector<Player>& playerList) {
    // URL of the API endpoint
    std::string apiUrl =
        "https://api-web.nhle.com/v1/skater-stats-leaders/20232024/"
        "2?categories=points&limit=50";

    // Initialize libcurl
    CURL* curl = curl_easy_init();

    if (curl) {
        // Set the URL
        curl_easy_setopt(curl, CURLOPT_URL, apiUrl.c_str());

        // Set the callback function to write the response to a string
        std::string response;
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, CurlWriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

        // Perform the HTTP GET request
        CURLcode res = curl_easy_perform(curl);

        // Check for errors
        if (res != CURLE_OK) {
            std::cerr << "Error in curl_easy_perform(): " << curl_easy_strerror(res)
                << std::endl;
        }
        else {
            try {
                // Parse the JSON response
                nlohmann::json jsonData = nlohmann::json::parse(response);

                // Clear existing playerList
                playerList.clear();

                // Extract information for each player and add to playerList
                for (const auto& playerData : jsonData["points"]) {
                    Player player;
                    player.firstName = playerData["firstName"]["default"];
                    player.lastName = playerData["lastName"]["default"];
                    player.points = playerData["value"];
                    playerList.push_back(player);
                }

                std::cout << "Updated player list.\n";
            }
            catch (const nlohmann::json::exception& e) {
                std::cerr << "Error parsing JSON: " << e.what() << std::endl;
            }
        }

        // Cleanup
        curl_easy_cleanup(curl);
    }
    else {
        std::cerr << "Error initializing libcurl." << std::endl;
    }
}

void showFantasyPoints(const std::vector<Player>& playerList) {
    // crud for marks butr also use this list forr the shootout game and Its cool
    // cause it auto update every day from the nhl api, shoot out teams will be
    // different depending on who the point leaders are.
    std::cout << "Fantasy Points:\n";
    for (const auto& player : playerList) {
        std::cout << "Name: " << player.firstName << " " << player.lastName
            << ", Points: " << player.points << "\n";
    }
    std::cout << "\n";
}

void shootoutGame(std::vector<Player>& playerList) {
    if (playerList.size() < 10) {
        std::cout << "Not enough players for a shootout game. Need at least 10 "
            "players.\n";
        return;
    }

    // Create two teams: Odds and Even
    std::vector<Player> oddsTeam;
    std::vector<Player> evenTeam;

    // Assign players to teams
    // Team even will be the point leaders in, 2nd, 4th, 6th, 8th, 10th
    // Team odd will be the point leaders in, 1st, 3rd, 5th, 7th, and 9th
    for (int i = 0; i < 10; ++i) {
        if (i % 2 == 0) {
            evenTeam.push_back(playerList[i]);
        }
        else {
            oddsTeam.push_back(playerList[i]);
        }
    }

    // Display team assignments
    auto calculateScoringProbability = [](const Player& player) {
        // Simple linear relationship: higher points, higher probability
        return static_cast<double>(player.points) / 100.0;
        };

    // Simulate goals for Odds Team
    int oddsTeamGoals = 0;
    std::cout << "Odds Team:\n";
    for (const auto& player : oddsTeam) {
        double scoringProbability = calculateScoringProbability(player);
        std::cout << "Name: " << player.firstName << " " << player.lastName
            << ", Points: " << player.points
            << ", Scoring Probability: " << scoringProbability << "\n";

        // Simulate goal
        int goalDecider =
            std::rand() % 100;  // Generate a random number between 0 and 99
        if (goalDecider < scoringProbability * 100) {
            std::cout << "GOAL!\n";
            oddsTeamGoals++;
        }
        else {
            std::cout << "No goal.\n";
        }
    }
    std::cout << "\n";

    // Simulate goals for Even Team
    int evenTeamGoals = 0;
    std::cout << "Even Team:\n";
    for (const auto& player : evenTeam) {
        double scoringProbability = calculateScoringProbability(player);
        std::cout << "Name: " << player.firstName << " " << player.lastName
            << ", Points: " << player.points
            << ", Scoring Probability: " << scoringProbability << "\n";

        // Simulate goal
        int goalDecider =
            std::rand() % 100;  // Generate a random number between 0 and 99
        if (goalDecider < scoringProbability * 100) {
            std::cout << "GOAL!\n";
            evenTeamGoals++;
        }
        else {
            std::cout << "No goal.\n";
        }
    }
    std::cout << "\n";

    // Determine the winner
    if (oddsTeamGoals > evenTeamGoals) {
        std::cout << "Odds Team wins!\n";
    }
    else if (evenTeamGoals > oddsTeamGoals) {
        std::cout << "Even Team wins!\n";
    }
    else {
        std::cout << "It's a tie!\n";
    }
    // Would have liked to add when later into the season for rwhen the playerrs
    // will be at or close to 100 points, another logic where when they reach 60
    // games divide the probabiblity by 2.
}

void manuallyAdd(std::vector<Player>& playerList) {
    // Crud for marks
    Player player;
    std::cout << "Enter first name: ";
    std::cin >> player.firstName;
    std::cout << "Enter last name: ";
    std::cin >> player.lastName;
    std::cout << "Enter points to add: ";
    std::cin >> player.points;
    playerList.push_back(player);
    std::cout << "Player added successfully.\n";
}

void manuallyDelete(std::vector<Player>& playerList) {
    // Crud for marks
    std::string firstName, lastName;
    std::cout << "Enter first name of player to delete: ";
    std::cin >> firstName;
    std::cout << "Enter last name of player to delete: ";
    std::cin >> lastName;

    auto it = std::remove_if(
        playerList.begin(), playerList.end(), [&](const Player& player) {
            return player.firstName == firstName && player.lastName == lastName;
        });

    if (it != playerList.end()) {
        playerList.erase(it, playerList.end());
        std::cout << "Player deleted successfully.\n";
    }
    else {
        std::cout << "Player not found.\n";
    }
}