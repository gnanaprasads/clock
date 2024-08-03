#include <GL/glut.h>
#include <cmath>
#include <ctime>
#include <string>
#include <vector>


// Constants
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// Point structure
struct Point {
    GLint x;
    GLint y;
};

// Analog clock variables
Point pC = { 320, 240 };
GLint radius = 150;
GLint hRadius = 80;
GLint mRadius = 110;
GLint sRadius = 130;

// Digital clock variables
struct TimeZone {
    std::string name;
    int offset; // Offset in minutes from UTC
};

std::vector<TimeZone> timeZones = {
    
    {"IST", 5 * 60 + 30},
    {"GMT", 0},
    {"UTC", 0},
    {"EST", -5 * 60},
    {"CST", -6 * 60},
    {"MST", -7 * 60},
    {"PST", -8 * 60}
};

int currentTZIndex = 0;
bool isAnalog = true;

// Function prototypes
void init();
void display();
void Timer(int value);
void mouse(int button, int state, int x, int y);
void keyboard(unsigned char key, int x, int y);
void createMenu();
void menu(int num);
void draw_circle(Point pC, GLfloat radius);
void draw_markers(Point pC, GLint radius);
void draw_numbers(Point pC, GLint radius);
void displayTimeZoneName();
void drawSegment(float x1, float y1, float x2, float y2, float thickness);
void drawDigit(float x, float y, float size, int digit);
void drawTime(float x, float y, float size, int hour, int minute, int second);
void drawDate(float x, float y, float size, int day, int month, int year);
void getLocalTime(struct tm* timeInfo);
void drawText(float x, float y, const char* text);
void displayAnalogClock();
void displayDigitalClock();

void init() {
    glClearColor(1.0, 1.0, 1.0, 0.0);
    glColor3f(0.0f, 0.0f, 0.0f);
    glPointSize(1.0f);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, 640.0, 0.0, 480.0);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    if (isAnalog) {
        displayAnalogClock();
    }
    else {
        displayDigitalClock();
    }
    glFlush();
}

void Timer(int value) {
    glutTimerFunc(1000, Timer, 0);
    glutPostRedisplay();
}

void mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        currentTZIndex = (currentTZIndex + 1) % timeZones.size();
        glutPostRedisplay();
    }
}

void keyboard(unsigned char key, int x, int y) {
    if (key == 'a') {
        currentTZIndex = (currentTZIndex - 1 + timeZones.size()) % timeZones.size();
        glutPostRedisplay();
    }
    if (key == 'd') {
        currentTZIndex = (currentTZIndex + 1) % timeZones.size();
        glutPostRedisplay();
    }
}

void createMenu() {
    glutCreateMenu(menu);
    glutAddMenuEntry("Analog Clock", 1);
    glutAddMenuEntry("Digital Clock", 2);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
}

void menu(int num) {
    if (num == 1) {
        isAnalog = true;
    }
    else if (num == 2) {
        isAnalog = false;
    }
    glutPostRedisplay();
}

// Analog clock functions
void draw_circle(Point pC, GLfloat radius) {
    GLfloat step = 2.0f * M_PI / 360;
    GLfloat x, y;

    glLineWidth(5.0f); // Thicker line for the circle
    glColor3f(0.f, 0.0f, 0.0f); // Black color
    glBegin(GL_LINE_LOOP);
    for (GLfloat theta = 0; theta < 2.0f * M_PI; theta += step) {
        x = pC.x + (radius * cos(theta));
        y = pC.y + (radius * sin(theta));
        glVertex2i(round(x), round(y));
    }
    glEnd();
    glLineWidth(1.0f);
}

void draw_markers(Point pC, GLint radius) {
    for (int i = 0; i < 360; i += 6) {
        GLfloat theta = i * M_PI / 180.0;
        GLint x1 = pC.x + (radius * cos(theta));
        GLint y1 = pC.y + (radius * sin(theta));
        GLint x2 = pC.x + ((radius - (i % 30 == 0 ? 15 : 5)) * cos(theta));
        GLint y2 = pC.y + ((radius - (i % 30 == 0 ? 15 : 5)) * sin(theta));

        glLineWidth(i % 30 == 0 ? 2.0f : 1.0f);
        glBegin(GL_LINES);
        glVertex2i(x1, y1);
        glVertex2i(x2, y2);
        glEnd();
    }
    glLineWidth(1.0f);
}

void draw_numbers(Point pC, GLint radius) {
    void* font = GLUT_BITMAP_HELVETICA_18;
    int num_spacing = 25;

    std::string numbers[4] = { "3", "6", "9", "12" };

    Point positions[4] = {
        { pC.x + radius - num_spacing, pC.y - 10 },   // "3"
        { pC.x - 10, pC.y - radius + num_spacing },   // "6"
        { pC.x - radius + num_spacing - 10, pC.y - 10 }, // "9"
        { pC.x - 10, pC.y + radius - num_spacing }    // "12"
    };

    for (int i = 0; i < 4; i++) {
        glRasterPos2i(positions[i].x, positions[i].y);
        for (char c : numbers[i]) {
            glutBitmapCharacter(font, c);
        }
    }
}

void displayTimeZoneName() {
    void* font = GLUT_BITMAP_HELVETICA_18;
    glColor3f(0.0, 0.0, 0.0);
    std::string tzName = "Time Zone: " + timeZones[currentTZIndex].name;
    glRasterPos2i(10, 460);
    for (char c : tzName) {
        glutBitmapCharacter(font, c);
    }
}

void displayAnalogClock() {
    time_t now = time(0);
    struct tm currentTime;
    gmtime_s(&currentTime, &now); // Convert to UTC

    int tzOffset = timeZones[currentTZIndex].offset;
    currentTime.tm_min += tzOffset;
    mktime(&currentTime); // Normalize

    double hDegree = 360.0 * (currentTime.tm_hour % 12 + currentTime.tm_min / 60.0) / 12.0;
    double mDegree = 360.0 * currentTime.tm_min / 60.0;
    double sDegree = 360.0 * currentTime.tm_sec / 60.0;

    hDegree = (90 - hDegree) * M_PI / 180.0;
    mDegree = (90 - mDegree) * M_PI / 180.0;
    sDegree = (90 - sDegree) * M_PI / 180.0;

    Point pHour, pMinute, pSecond;

    pHour.y = pC.y + (hRadius * sin(hDegree));
    pHour.x = pC.x + (hRadius * cos(hDegree));

    pMinute.y = pC.y + (mRadius * sin(mDegree));
    pMinute.x = pC.x + (mRadius * cos(mDegree));

    pSecond.y = pC.y + (sRadius * sin(sDegree));
    pSecond.x = pC.x + (sRadius * cos(sDegree));

    glClear(GL_COLOR_BUFFER_BIT);

    glColor3f(0.6, 0.3, 0.1); // Black color for the circle
    draw_circle(pC, radius);
    draw_markers(pC, radius);
    draw_numbers(pC, radius);

    glColor3f(1.0, 0.0, 0.0); // Red color for the hour hand
    glLineWidth(3.0f);
    glBegin(GL_LINES);
    glVertex2i(pC.x, pC.y);
    glVertex2i(pHour.x, pHour.y);
    glEnd();

    glColor3f(0.0, 0.0, 1.0); // Blue color for the minute hand
    glLineWidth(2.0f);
    glBegin(GL_LINES);
    glVertex2i(pC.x, pC.y);
    glVertex2i(pMinute.x, pMinute.y);
    glEnd();

    glColor3f(0.0, 1.0, 0.0); // Green color for the second hand
    glLineWidth(1.0f);
    glBegin(GL_LINES);
    glVertex2i(pC.x, pC.y);
    glVertex2i(pSecond.x, pSecond.y);
    glEnd();

    displayTimeZoneName();

    glFlush();
}

// Digital clock functions
void drawSegment(float x1, float y1, float x2, float y2, float thickness) {
    glLineWidth(thickness);
    glBegin(GL_LINES);
    glVertex2f(x1, y1);
    glVertex2f(x2, y2);
    glEnd();
}

void drawDigit(float x, float y, float size, int digit) {
    const float thickness = 5.0f;

    bool segments[10][7] = {
        { true, true, true, true, true, true, false }, // 0
        { false, true, true, false, false, false, false }, // 1
        { true, true, false, true, true, false, true }, // 2
        { true, true, true, true, false, false, true }, // 3
        { false, true, true, false, false, true, true }, // 4
        { true, false, true, true, false, true, true }, // 5
        { true, false, true, true, true, true, true }, // 6
        { true, true, true, false, false, false, false }, // 7
        { true, true, true, true, true, true, true }, // 8
        { true, true, true, true, false, true, true }  // 9
    };

    float segmentLength = size / 2.0f;
    float segmentGap = 2.0f;

    // Segments' coordinates
    float x1 = x - segmentLength;
    float x2 = x;
    float x3 = x + segmentLength;
    float y1 = y + size;
    float y2 = y;
    float y3 = y - size;

    // Draw each segment if it's part of the digit
    if (segments[digit][0]) drawSegment(x1, y1, x3, y1, thickness); // Top
    if (segments[digit][1]) drawSegment(x3, y1, x3, y2, thickness); // Top-right
    if (segments[digit][2]) drawSegment(x3, y2, x3, y3, thickness); // Bottom-right
    if (segments[digit][3]) drawSegment(x1, y3, x3, y3, thickness); // Bottom
    if (segments[digit][4]) drawSegment(x1, y2, x1, y3, thickness); // Bottom-left
    if (segments[digit][5]) drawSegment(x1, y1, x1, y2, thickness); // Top-left
    if (segments[digit][6]) drawSegment(x1, y2, x3, y2, thickness); // Middle
}

void drawTime(float x, float y, float size, int hour, int minute, int second) {
    float digitSpacing = size * 1.5f;

    // Draw hours
    drawDigit(x - digitSpacing * 3.5f, y, size, hour / 10);
    drawDigit(x - digitSpacing * 2.5f, y, size, hour % 10);

    // Draw colon
    drawSegment(x - digitSpacing * 1.5f, y + size / 2.5f, x - digitSpacing * 1.5f, y - size / 2.5f, 2.0f);

    // Draw minutes
    drawDigit(x - digitSpacing * 0.5f, y, size, minute / 10);
    drawDigit(x + digitSpacing * 0.5f, y, size, minute % 10);

    // Draw colon
    drawSegment(x + digitSpacing * 1.5f, y + size / 2.5f, x + digitSpacing * 1.5f, y - size / 2.5f, 2.0f);

    // Draw seconds
    drawDigit(x + digitSpacing * 2.5f, y, size, second / 10);
    drawDigit(x + digitSpacing * 3.5f, y, size, second % 10);
}

void drawDate(float x, float y, float size, int day, int month, int year) {
    float digitSpacing = size * 1.5f;

    // Draw day
    drawDigit(x - digitSpacing * 3.5f, y, size, day / 10);
    drawDigit(x - digitSpacing * 2.5f, y, size, day % 10);

    // Draw slash
    drawSegment(x - digitSpacing * 1.5f, y, x - digitSpacing * 1.0f, y, 2.0f);

    // Draw month
    drawDigit(x - digitSpacing * 0.1f, y, size, month / 10);
    drawDigit(x + digitSpacing * 0.9f, y, size, month % 10);

    // Draw slash
    drawSegment(x + digitSpacing * 1.5f, y, x + digitSpacing * 2.0f, y, 2.0f);

    // Draw year
    drawDigit(x + digitSpacing * 2.9f, y, size, (year / 1000) % 10);
    drawDigit(x + digitSpacing * 3.9f, y, size, (year / 100) % 10);
    drawDigit(x + digitSpacing * 4.9f, y, size, (year / 10) % 10);
    drawDigit(x + digitSpacing * 5.9f, y, size, year % 10);
}

void getLocalTime(struct tm* timeInfo) {
    time_t rawtime;
    time(&rawtime);
    localtime_s(timeInfo, &rawtime);
}

void drawText(float x, float y, const char* text) {
    glRasterPos2f(x, y);
    while (*text) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *text++);
    }
}

void displayDigitalClock() {
    time_t now = time(0);
    struct tm currentTime;
    gmtime_s(&currentTime, &now); // Convert to UTC

    int tzOffset = timeZones[currentTZIndex].offset;
    currentTime.tm_min += tzOffset;
    mktime(&currentTime); // Normalize

    glClear(GL_COLOR_BUFFER_BIT);

    int hour = currentTime.tm_hour;
    int minute = currentTime.tm_min;
    int second = currentTime.tm_sec;

    int day = currentTime.tm_mday;
    int month = currentTime.tm_mon + 1;
    int year = currentTime.tm_year + 1900;

    // Draw time
    drawTime(320, 300, 40, hour, minute, second);

    // Draw date
    drawDate(320, 200, 20, day, month, year);

    // Display time zone name
    glColor3f(0.0, 0.0, 0.0);
    displayTimeZoneName();

    glFlush();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(640, 480);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Clock Application");

    init();

    glutDisplayFunc(display);
    glutTimerFunc(1000, Timer, 0);
    glutMouseFunc(mouse);
    glutKeyboardFunc(keyboard);
    createMenu();

    glutMainLoop();
    return 0;
}