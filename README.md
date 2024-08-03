This project is a clock application developed using OpenGL and C++. It features both an analog and a digital clock display. The application allows users to switch between these two views and adjust the time zone using keyboard and mouse interactions. The analog clock displays time with hour, minute, and second hands, while the digital clock shows the current time and date in a clear digital format.

Features
Analog Clock:

Displays time with hour, minute, and second hands.
Includes markers for every 6 hours and numbers for 3, 6, 9, and 12 o'clock.
Digital Clock:

Shows current time and date.
Displays time in HH:MM
format and date in DD/MM/YYYY format.
Time Zone Support:

Supports multiple time zones including IST, GMT, UTC, EST, CST, MST, and PST.
Time zone can be changed using keyboard and mouse controls.
Interactive Controls:

Left mouse button click to cycle through time zones.
'a' and 'd' keys to navigate through time zones.
Right-click menu to switch between analog and digital clock views.
Installation
Clone the repository:

sh
Copy code
git clone https://github.com/yourusername/clock-application.git
Navigate to the project directory:

sh
Copy code
cd clock-application
Compile the project:
Ensure you have OpenGL and GLUT installed. Compile the code using:

sh
Copy code
g++ -o clock_application main.cpp -lGL -lGLU -lglut
Run the application:

sh
Copy code
./clock_application
Usage
Switch between Analog and Digital Clock:

Right-click to open the menu and select "Analog Clock" or "Digital Clock".
Change Time Zones:

Click the left mouse button to cycle through available time zones.
Use the 'a' key to go to the previous time zone and 'd' key to go to the next.
Files
main.cpp: Contains the main program logic, including the implementation of the clock functionality.
README.md: This file with project description and instructions.
Contributing
Feel free to fork the repository and submit pull requests. For major changes, please open an issue first to discuss what you would like to change.

License
This project is licensed under the MIT License - see the LICENSE file for details.

