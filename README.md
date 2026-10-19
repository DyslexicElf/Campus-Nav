# Campus-Nav 🗺️

Campus-Nav is a prototype indoor campus navigation application designed to provide dynamic, cross-floor routing. The system utilizes a lightweight web-based frontend paired with a high-performance C++ pathfinding engine to calculate the most efficient routes between classrooms, labs, and facilities.

## 🚀 Current Project State (Sprint 1 Wrap-up)

The initial foundation of the application is complete and ready for backend pathfinding integration. 

**Completed Features:**
* **Frontend Web UI:** Fully responsive map interface with manual room search and floor-switching buttons.
* **QR Code Integration:** Implemented the `html5-qrcode` library for live camera scanning, allowing users to scan a room's physical QR code to establish their "You Are Here" starting node.
* **Admin Print Dashboard:** CSS is optimized with a `@media print` query to cleanly generate and print a "Bulk Room QR Code Sheet" for physical deployment.
* **Shared Database:** `map_data.json` is established as the single source of truth for all room IDs, X/Y coordinate nodes, elevator flags, and floor connection parameters. 
* **Node.js API Bridge:** An Express server successfully serves the web app and uses `child_process` to pass user routing requests to the C++ executable.
* **C++ Engine Scaffolding:** Visual Studio environment is configured, `nlohmann/json` is installed via NuGet, and the architectural boilerplate for Dijkstra's algorithm is in place.

## 🏗️ System Architecture

| Component | Technology | Description |
| :--- | :--- | :--- |
| **Frontend** | HTML, CSS, Vanilla JS | Handles map rendering (SVG overlays), user inputs, and camera permissions. |
| **Web Server** | Node.js, Express | Hosts the static files and acts as an API bridge to the native operating system. |
| **Database** | JSON (`map_data.json`) | Shared mapping database utilized by both the frontend UI and the backend math engine. |
| **Pathfinding** | C++20 | Native executable that ingests JSON and command-line arguments to calculate Dijkstra routes. |

## 🛠️ Sprint 2 Handoff: C++ Backend Tasks

The primary goal for the upcoming sprint is to finalize the pathfinding logic inside `Source.cpp`. The file contains specific `TODO` blocks outlining the required implementations:

1. **Euclidean Distance Math:** Replace the temporary edge-weight placeholders inside the `calculateShortestPath` function with proper 2D distance calculations using the `<cmath>` library.
2. **Accessible Routing:** Implement the `--accessible` command-line flag. When true, the engine must evaluate the `"isElevator"` flag in the JSON database and intentionally skip standard stairwell edges to force an accessible route.
3. **Multi-Stop Journey Stitching:** Update the `main()` execution loop to handle more than two destination arguments. The engine must seamlessly stitch sequential route segments into a single master vector (handling the overlapping start/end nodes to prevent UI stuttering).
4. **Turn-by-Turn Directions:** Implement the `generateTextDirections` function to analyze coordinate deltas (X/Y changes) and generate human-readable navigation steps.
5. **JSON Contract Formatting:** Format the final standard output (`cout`) as a structured JSON object so the Node.js server can cleanly parse it. 
   * *Expected Output Format:* `{ "path": ["n1", "n4", "n12"], "directions": ["Walk straight", "Take elevator to 2nd Floor"] }`

## 💻 Local Setup & Execution

**To run the Web App:**
1. Open a terminal in the project root.
2. Run `npm install` to ensure server dependencies are met.
3. Run `npm start` (or `node server.js`).
4. Navigate to `http://localhost:3000` in your browser.

**To build the C++ Engine:**
1. Open `Campus-Nav.sln` in Visual Studio 2022.
2. Ensure you are targeting `x64` (Debug or Release).
3. Build the Solution (Ctrl + Shift + B). The `.exe` output path must align with the Node.js `child_process` configuration to successfully trigger the algorithm from the web frontend.

---
*Note: Please avoid using code comments (`//` or `/*`) inside `map_data.json`, as it will break the parser. Use dummy keys (e.g., `"_note1": "text"`) if you need to leave notes in the database.*

## 👥 Team Contributions

**Avery Andrews (DyslexicElf)**
* **Frontend Development:** Designed and implemented the responsive web UI (`HTML`/`CSS`/`Vanilla JS`), including the interactive floor maps, dynamic SVG path overlays, and the Turn-by-Turn directions panel.
* **QR Code & Localization:** Integrated the `html5-qrcode` scanner for live camera positioning and engineered the print-optimized Bulk Room QR Code directory for physical campus deployment.
* **Backend API Bridge:** Built the Node.js/Express server and the `child_process` bridge to handle HTTP requests and seamlessly execute the native C++ engine.
* **Database Architecture:** Mapped the physical campus coordinates and authored the unified `map_data.json` waypoint and routing database.
* **C++ Engine Scaffolding:** Drafted the `Source.cpp` boilerplate, configured the `nlohmann/json` NuGet package, and architected the structural outlines for Dijkstra's algorithm, multi-stop routing, and accessibility flags to establish the Sprint 2 foundation.

**Evan Duchette (The-Evan-Duchette)**
* *(Sprint 2)* Implement 2D Euclidean distance math within the pathfinding engine.
* *(Sprint 2)* Build the Turn-by-Turn text direction generator.

**Tuan Khai Pham (KhaiDiLao)**
* *(Sprint 1)* Prepared a transparent background for both the basement and first floors.
* *(Sprint 2)* Completed the basement floor nodes.

**Lida Entezami (Lid182)**
* *(Sprint 1)* Prepared a transparent background for the third floor.
* *(Sprint 2)* Completed the third floor nodes.

**Ella Powers (ella-powers)**
* *(Sprint 1)* Prepared a transparent background for floor two.
* *(Sprint 2)* Completed the second floor nodes.

**Colton Dover (colton-dover)**
* *(Sprint 1)* 
* *(Sprint 2)* 
