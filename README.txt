This C++ program interacts with the NHL API to fetch and process the top 50 point scores of the current NHL season. The application features a command-line menu with six options.

    Fetch Data from NHL API:
        The first option uses the curl library to perform an HTTP GET request to the NHL API endpoint, retrieving skater stats for the top 50 players in terms of points.
        The getUpdatedList function, employing the nlohmann library, parses the JSON response and populates a vector of Player structs (playerList).

    Display Results in Clean Format:
        The second option displays the fetched data in a readable format, showcasing the names and point totals of the top 50 players.
        The showFantasyPoints function utilizes the player information stored in playerList to present a clear list of player names and fantasy points.

    Simulate Shootout Game:
        The shootoutGame function simulates a shootout game by creating two teams, Odds and Even, comprising players ranked from 1st to 9th and every odd-numbered position in the playerList vector.
        Scoring probabilities for each player are calculated based on their points, and goals are simulated for each team, with results displayed.

    Manually Add Player:
        The program allows users to manually add a player to the playerList vector, providing flexibility for additional entries.

    Manually Delete Player:
        Users can manually delete a player from the playerList vector, offering customization of the player roster.

    Close Program:
        The last option gracefully exits the program.

Key Libraries Used:

    curl Library: Utilized for making HTTP requests to the NHL API.
    nlohmann Librrary: Employed for parsing and handling JSON data retrieved from the API.

Summary:

    The program integrates curl and nlohmann libraries to fetch and process NHL API data.
    Users can view a formatted list of the top 50 players' names and point totals, simulate a shootout game, and perform manual operations on the player list.
    Heavily commentented because there are complaex libraries and is a good reference point.