const express = require('express');
const router = express.Router();
const { execFile } = require('child_process');
const path = require('path');

/* GET home page. */
router.get('/', function(req, res, next) {
  res.render('index', { title: 'Campus Navigation' });
});

// The Bridge: Runs the C++ executable and returns the JSON path array
router.get('/api/navigate', (req, res) => {
    const startRoom = req.query.start;
    const endRoom = req.query.end;

    // Build the arguments list for the C++ engine
    const args = [startRoom, endRoom];
    
    // Add the flag if the user checked the accessibility box
    if (req.query.accessible === 'true') {
        args.push('--accessible');
    }

    // FIX 1: Removed "Core-Engine" so it targets the actual Visual Studio build output
    const exePath = path.join(__dirname, '../../x64/Debug/Campus-Nav.exe');

    // FIX 2: Force the C++ engine to run from the web-frontend root 
    // so ifstream("public/map_data.json") finds the file perfectly.
    const execOptions = { cwd: path.join(__dirname, '../') };

    execFile(exePath, args, execOptions, (error, stdout, stderr) => {
        if (error) {
            // Logs the actual system error (like ENOENT) if it still can't find the .exe
            console.error("C++ Engine Error:", stderr || error.message);
            return res.status(500).json({ error: "Pathfinding engine failed" });
        }
        
        // Send the JSON array printed by the C++ program directly back to the browser
        res.send(stdout); 
    });
});

module.exports = router;