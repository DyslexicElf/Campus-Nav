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

    // Relative path to your teammates' compiled C++ program
    const exePath = path.join(__dirname, '../../Core-Engine/x64/Debug/Campus-Nav.exe');

    execFile(exePath, [startRoom, endRoom], (error, stdout, stderr) => {
        if (error) {
            console.error("C++ Engine Error:", stderr);
            return res.status(500).json({ error: "Pathfinding engine failed" });
        }
        
        // Send the JSON array printed by the C++ program directly back to the browser
        res.send(stdout); 
    });
});

module.exports = router;