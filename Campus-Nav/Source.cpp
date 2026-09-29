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

// TODO: Install nlohmann/json via NuGet or vcpkg to read map_data.json
// #include <nlohmann/json.hpp> 
// using json = nlohmann::json;

using namespace std;

// Helper structure to keep the queue organized by shortest distance
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
vector<string> calculateShortestPath(string startNode, string endNode, const json& mapData) {
    unordered_map<string, double> distances;
    unordered_map<string, string> previous;
    priority_queue<NodeRecord, vector<NodeRecord>, greater<NodeRecord>> queue;

    // 1. Initialize all hallway nodes to Infinity
    for (auto& [nodeId, nodeData] : mapData["waypoints"].items()) {
        distances[nodeId] = numeric_limits<double>::infinity();
        previous[nodeId] = "";
    }

    distances[startNode] = 0;
    queue.push({ startNode, 0 });

    // 2. Evaluate paths
    while (!queue.empty()) {
        string current = queue.top().id;
        double currentDist = queue.top().distance;
        queue.pop();

        if (current == endNode) break; // Reached the destination
        if (currentDist > distances[current]) continue; // Skip obsolete paths

        // 3. Check all connected neighbors
        for (string neighbor : mapData["waypoints"][current]["neighbors"]) {

            // X and Y coordinates for 2D physical distance
            double x1 = mapData["waypoints"][current]["x"];
            double y1 = mapData["waypoints"][current]["y"];
            double x2 = mapData["waypoints"][neighbor]["x"];
            double y2 = mapData["waypoints"][neighbor]["y"];

            // NEW: Floor coordinates to account for 3D elevator/stair travel
            int floor1 = mapData["waypoints"][current]["floor"];
            int floor2 = mapData["waypoints"][neighbor]["floor"];
            double floorPenalty = (floor1 != floor2) ? 50.0 : 0.0; // Adds weight to floor changes

            double weight = sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2)) + floorPenalty;
            double altDistance = currentDist + weight;

            // If this is a faster route to the neighbor, save it
            if (altDistance < distances[neighbor]) {
                distances[neighbor] = altDistance;
                previous[neighbor] = current;
                queue.push({ neighbor, altDistance });
            }
        }
    }

    // 4. Reconstruct the final path by walking backwards
    vector<string> path;
    string curr = endNode;
    while (curr != "") {
        path.push_back(curr);
        curr = previous[curr];
    }

    // Reverse the array so it goes from Start -> End
    reverse(path.begin(), path.end());
    return path;
}

// ==========================================
// NODE.JS INTEGRATION CONTRACT
// ==========================================
int main(int argc, char* argv[]) {
    if (argc < 3) {
        cerr << "Error: Missing arguments. Usage: Campus-Nav.exe <startNode> <endNode>" << endl;
        return 1;
    }

    string startNode = argv[1];
    string endNode = argv[2];

    // TODO: Read the JSON database 
    // (Note: Path is relative to the Node.js execution directory)
    // ifstream file("public/map_data.json");
    // json mapData = json::parse(file);

    // TODO: Calculate the route
    // vector<string> shortestPath = calculateShortestPath(startNode, endNode, mapData);

    // TEMPORARY MOCK OUTPUT (Delete this when json parser is working)
    cout << "[\"n1\", \"n3\"]" << endl;

    /*
    // TODO: REAL JSON OUTPUT FORMATTER (Uncomment this when json parser is working)
    // This perfectly formats the C++ vector into the strict JSON array Express expects
    cout << "[";
    for (size_t i = 0; i < shortestPath.size(); ++i) {
        cout << "\"" << shortestPath[i] << "\"";
        if (i < shortestPath.size() - 1) cout << ", ";
    }
    cout << "]" << endl;
    */

    return 0;
}