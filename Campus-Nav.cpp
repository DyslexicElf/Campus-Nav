// Campus Navigation Core Engine
// Team: Avery A. - Ella P. - Evan D. - Colton D. - Lida E. - Tuan Khai P.

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

            // [COMPLETED: Stairwell / Accessibility Check]
            // If the user requires an accessible route and the connection changes floors,
            // both the current node and the neighbor MUST have the "isElevator" flag.
            if (requiresAccessible && (floor1 != floor2)) {
                bool currentIsElevator = mapData["waypoints"][current].value("isElevator", false);
                bool neighborIsElevator = mapData["waypoints"][neighbor].value("isElevator", false);

                if (!currentIsElevator || !neighborIsElevator) {
                    continue; // Skip this path, it is a stairwell
                }
            }

            double x1 = mapData["waypoints"][current]["x"];
            double y1 = mapData["waypoints"][current]["y"];
            double x2 = mapData["waypoints"][neighbor]["x"];
            double y2 = mapData["waypoints"][neighbor]["y"];

            // [COMPLETED: 2D Euclidean Distance Math]
            double distance2D = sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));

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

        // [COMPLETED: Call calculateShortestPath]
        vector<string> segmentPath = calculateShortestPath(currentStart, currentEnd, mapData, requiresAccessible);

        // Stitch segments together
        for (size_t j = 0; j < segmentPath.size(); ++j) {
            if (i > 0 && j == 0) continue;

            // [COMPLETED: push_back the node into fullMasterPath]
            fullMasterPath.push_back(segmentPath[j]);
        }
    }

    // REAL JSON OUTPUT FORMATTER (Simple Array)
    cout << "[";
    for (size_t i = 0; i < fullMasterPath.size(); ++i) {
        cout << "\"" << fullMasterPath[i] << "\"";
        if (i < fullMasterPath.size() - 1) cout << ", ";
    }
    cout << "]" << endl;

    return 0;
}