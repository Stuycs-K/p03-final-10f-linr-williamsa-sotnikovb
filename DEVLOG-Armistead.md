# Dev Log:

This document must be updated daily every time you finish a work session.

## Armistead Williams

### 2024-01-06 - Determined Project
Figured out project plan and distributed workload, assigned deadlines and segmented work.

### 2024-01-07 - Planning
Figured out the exact systems-level interactions of our clients and servers. Took the entire period.

### 2025-01-08 - Began work
Began working on project, making a new server that can listening on multiple sockets simultaneously.

### 2025-01-09 - Worked with select function
Figured out functionalities, specifically if we intend to have select monitor both a read set and write set at same time.

### 2025-01-11 - Made server skeleton
Made skeleton for main server, match server and specifics of handling requests not made yet, see comments

### 2025-01-12 - Figured out implementation and bugtested
Figured out how specifics of match server communication with main server would be implemented

### 2025-01-13 - Figured out board implementation
Figured out more specifics of match servers - using structs to store the metainformation of each match - the sockets and pid that the main server could then pair up with the match results.

### 2025-01-14 - Finished server side programming (mostly)
Smoothed out the communication back up to main server, using some structs and some signals.
