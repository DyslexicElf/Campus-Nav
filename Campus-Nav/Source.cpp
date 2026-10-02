// Campus Navigation Core Engine
// Team: Avery A. & [Teammate Names]

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <queue>
#include <cmath>
#include <limits>
#include <algorithm>

using namespace std;
#include <nlohmann/json.hpp> 
using json = nlohmann::json;

struct NodeRecord {
    string id;
    double distance;
    bool operator>(const NodeRecord& other) const {
        return distance > other.distance;
    }
};

// ==========================================
// PATHFINDING ENGINE (DIJKSTRA)
// ==========================================
vector<string> calculateShortestPath(string startNode, string endNode, const json& mapData, bool requiresAccessible) {
    unordered_map<string, double> distances;
    unordered_map<string, string> previous;
    priority_queue<NodeRecord, vector<NodeRecord>, greater<NodeRecord>> queue;

    for (auto& [nodeId, nodeData] : mapData["waypoints"].items()) {
        distances[nodeId] = numeric_limits<double>::infinity();
        previous[nodeId] = "";
    }

    distances[startNode] = 0;
    queue.push({ startNode, 0 });

    while (!queue.empty()) {
        string current = queue.top().id;
        double currentDist = queue.top().distance;
        queue.pop();

        if (current == endNode) break;
        if (currentDist > distances[current]) continue;

        for (string neighbor : mapData["waypoints"][current]["neighbors"]) {

            int floor1 = mapData["waypoints"][current]["floor"];
            int floor2 = mapData["waypoints"][neighbor]["floor"];

            // HINT 1: Stairwell check goes here!
            // If requiresAccessible == true AND (floor1 != floor2) AND it is NOT an elevator node, use 'continue;'

            double x1 = mapData["waypoints"][current]["x"];
            double y1 = mapData["waypoints"][current]["y"];
            double x2 = mapData["waypoints"][neighbor]["x"];
            double y2 = mapData["waypoints"][neighbor]["y"];

            // [TODO: Calculate the 2D Euclidean distance between Node 1 and Node 2]
            // HINT 2: Use sqrt() and pow() from the <cmath> library to find the physical distance between (x1, y1) and (x2, y2).
            double distance2D = 0.0; // Replace this!

            double floorPenalty = (floor1 != floor2) ? 50.0 : 0.0;
            double weight = distance2D + floorPenalty;
            double altDistance = currentDist + weight;

            if (altDistance < distances[neighbor]) {
                distances[neighbor] = altDistance;
                previous[neighbor] = current;
                queue.push({ neighbor, altDistance });
            }
        }
    }

    vector<string> path;
    string curr = endNode;
    while (curr != "") {
        path.push_back(curr);
        curr = previous[curr];
    }

    reverse(path.begin(), path.end());
    return path;
}

// ==========================================
// TURN-BY-TURN DIRECTIONS ENGINE (NEW)
// ==========================================
vector<string> generateTextDirections(const vector<string>& fullPath, const json& mapData) {
    vector<string> directions;

    // HINT 3: If the path has fewer than 2 nodes, return an empty vector.

    // HINT 4: Loop through the 'fullPath' vector (from i = 0 to size - 2).

    // HINT 5: For each segment, compare the X/Y coordinates of fullPath[i] and fullPath[i+1].
    // If X changes a lot but Y barely changes, they are moving horizontally (East/West).
    // If Y changes a lot, they are moving vertically (North/South).
    // If the floor changes, tell them to take the stairs or elevator.

    // [TODO: Write the loop and push human-readable strings (e.g., "Walk straight to n5", "Take elevator to Floor 3") into the directions vector]

    return directions;
}

// ==========================================
// NODE.JS INTEGRATION CONTRACT 
// ==========================================
int main(int argc, char* argv[]) {

    if (argc < 3) {
        cerr << "Error: Missing arguments." << endl;
        return 1;
    }

    // --- JSON PARSER (Completed) ---
    ifstream file("public/map_data.json");
    if (!file.is_open()) {
        cerr << "Error: Could not open map_data.json. Check file path." << endl;
        return 1;
    }
    json mapData;
    file >> mapData;
    // -------------------------------

    bool requiresAccessible = false;
    vector<string> routeNodes;

    // Isolate the nodes from the accessibility flag
    for (int i = 1; i < argc; ++i) {
        string arg = argv[i];
        if (arg == "--accessible") {
            requiresAccessible = true;
        }
        else {
            routeNodes.push_back(arg);
        }
    }

    vector<string> fullMasterPath;

    // Multi-Stop Segment Loop
    for (size_t i = 0; i < routeNodes.size() - 1; ++i) {
        string currentStart = routeNodes[i];
        string currentEnd = routeNodes[i + 1];

        // [TODO: Call calculateShortestPath() using currentStart, currentEnd, mapData, and requiresAccessible]
        vector<string> segmentPath; // Replace with function call 

        // Stitch segments together
        for (size_t j = 0; j < segmentPath.size(); ++j) {
            if (i > 0 && j == 0) continue;
            // [TODO: push_back the node into fullMasterPath]
        }
    }

    // [TODO: Call generateTextDirections() using your stitched fullMasterPath]
    // vector<string> textDirections = ...

    // HINT 6: Update the JSON formatter loop!
    // The Express frontend now expects a structured JSON object containing BOTH arrays, not just a single array.
    // [TODO: Write the cout loops to print a JSON object exactly matching this format: ]
    // { "path": ["n1", "n3"], "directions": ["Go North", "Arrive"] }

    return 0;
}